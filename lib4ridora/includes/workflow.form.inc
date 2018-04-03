<?php

/**
 * @file
 * Workflow form landing page for LIB4RI.
 */

/**
 * Landing form for mini workflow.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array
 *   The form to be rendered.
 */
function lib4ridora_mini_workflow_document_management_form($form, $form_state, AbstractObject $object) {
  module_load_include('inc', 'islandora_mini_workflow', 'includes/utilities');
  if (in_array('ir:citationCModel', $object->models) && isset($object['MODS']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['MODS'])) {
    $form['citation']['#markup'] = citeproc_bibliography_from_mods(citeproc_default_style(), $object['MODS']->content);
  }
  $view = views_embed_view('lib4ridora_document_management');
  $form['views_links'] = array(
    '#type' => 'item',
    '#markup' => $view,
  );
  $current_status = islandora_mini_workflow_get_values($object);
  if (!empty($current_status) && $current_status['state'] == ISLANDORA_MINI_WORKFLOW_DEFAULT_STATE) {
    $form['pid'] = array(
      '#type' => 'value',
      '#value' => $object->id,
    );
    $form['approve'] = array(
      '#type' => 'submit',
      '#value' => t('Approve Object'),
    );
  }
  return $form;
}

/**
 * Submit handler to approve an object from the sent-to-approval state.
 */
function lib4ridora_mini_workflow_document_management_form_submit($form, &$form_state) {
  global $user;
  module_load_include('inc', 'islandora_mini_workflow', 'includes/utilities');
  $object = islandora_object_load($form_state['values']['pid']);
  islandora_mini_workflow_set_values($object, array(
    'state' => 'approved',
    'assignee' => $user->name,
  ));
  drupal_set_message(t('The workflow state has been updated.'));
}
