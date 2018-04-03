<?php

/**
 * @file
 * Contains the callback functions for citation MODS manipulations.
 */

/**
 * Grab the correct MODS edit form for citation objects and load it.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object to edit.
 *
 * @return array
 *   Returns the drupal form to load.
 */
function lib4ridora_citation_select_mods_edit_form($form, &$form_state, AbstractObject $object) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/associations');
  form_load_include($form_state, 'inc', 'lib4ridora', 'includes/utilities');
  $form_state['pid'] = $object->id;
  $form_name = lib4ridora_determine_form_name($object);
  if (empty($form_name) && isset($form_state['form_name'])) {
    $form_name = $form_state['form_name'];
  }
  if ($form_name) {
    foreach (xml_form_builder_get_associations(array($form_name), array(), array('MODS')) as $key => $association) {
      // Update the content_model to be ir:citationCModel so that regardless
      // of the form used it will only have the ir:citationCModel.
      $association['content_model'] = "ir:citationCModel";
      // Load the form builder form.
      $metadata_form = xml_form_builder_get_form(array(), $form_state, $association['form_name'], $object['MODS']->content, array());
      // Need to make the #type markup because forms within forms are bad.
      $metadata_form['#type'] = 'markup';
      $metadata_form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update'),
      );
      $form_state['association'] = $association;
      if (isset($metadata_form['identifiers']) && isset($metadata_form['identifiers']['doi'])) {
        $metadata_form['identifiers']['doi']['#element_validate'][] = 'lib4ridora_doi_validate';
      }
      return $metadata_form;
    }
    drupal_set_message(t('The XML form could not be loaded. Verify that the %form_name form has an association.', array(
      '%form_name' => $form_name,
    )), 'error');
  }

  // Fallback to load the ingest form if there is no current association so that
  // an association can be set.
  form_load_include($form_state, 'inc', 'lib4ridora', 'includes/ingest-selector.form');
  $form = lib4ridora_ingest_selector_form($form, $form_state, $object);
  return $form;
}

/**
 * Handle the submit for the ingest-selector and the loaded form builder form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function lib4ridora_citation_select_mods_edit_form_submit($form, &$form_state) {
  if (array_key_exists('subtype_semantics', $form_state['values'])) {
    module_load_include('inc', 'lib4ridora', 'includes/citation.subtypes');
    $genre_map = lib4ridora_citation_get_genre_map(FALSE);
    $this_genre = $genre_map[$form_state['values']['publication_type']];
    $default_subtype_semantics = lib4ridora_citation_get_genre_semantics_map();
    $this_subtype_semantics = variable_get('lib4ridora_citation_subtype_semantics', $default_subtype_semantics);
    $form_state['values']['subtype_semantics'] = $this_subtype_semantics[$this_genre];
  }
  if (isset($form_state['values']['ingest_selector'])) {
    $subtype = $form_state['values']['ingest_selector'];
    $subtypes = lib4ridora_citation_form_subtypes();
    $form_state['form_name'] = $subtypes[$subtype]['form'];
    $form_state['rebuild'] = TRUE;
  }
  else {
    $pid = $form_state['pid'];
    $object = islandora_object_load($pid);
    $association = $form_state['association'];
    $label = NULL;
    if ($association['title_field']) {
      $title_field = drupal_array_get_nested_value($form, $association['title_field']);
      $label = $title_field['#value'];
    }
    // Handle all formbuilder things.
    $xml_form = new XMLForm($form_state, array());
    // Pass only the portion of the form that contains the formbuilder markup
    // and spoof the form_state['values'] so formbuilder operates as normal.
    $document = $xml_form->submit($form, $form_state);
    // Update the label.
    xml_form_builder_update_object($object, $form_state['association'], $document, $label);
    usleep(500000); // give solr some time to update...
    $form_state['redirect'] = "islandora/object/{$object->id}";
  }
}
