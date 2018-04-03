<?php

/**
 * @file
 * Journal impact factor adjustment form.
 */

/**
 * Form building function; build form for allowing the management of factors.
 */
function lib4ridora_jif_json_form($form, &$form_state, $object_id) {
  form_load_include($form_state, 'inc', 'lib4ridora', 'includes/jif_json.form');
  $object = islandora_object_load($object_id);

  $wrapper_id = 'lib4ridora_jif_json_form_wrapper';
  $form['#tree'] = TRUE;
  $form['#prefix'] = "<div id='$wrapper_id'>";
  $form['#suffix'] = '</div>';

  if (!isset($form_state['factors'])) {
    if (isset($object[LIB4RIDORA_FACTOR_DSID])) {
      $form_state['factors'] = json_decode($object[LIB4RIDORA_FACTOR_DSID]->content, TRUE);
    }
    else {
      $form_state['factors'] = array();
    }
  }
  elseif (isset($form_state['triggering_element'])) {
    $year = drupal_array_get_nested_value($form_state['values'], array('input', 'year'));
    $factor = drupal_array_get_nested_value($form_state['values'], array('input', 'factor'));

    $trigger = $form_state['triggering_element'];
    if ($trigger['#parents'] == array('existing', 'remove')) {
      $selected = array_filter(drupal_array_get_nested_value($form_state['values'], array('existing', 'table')));
      $form_state['factors'] = array_diff_key($form_state['factors'], drupal_map_assoc($selected));
    }
    elseif ($trigger['#parents'] == array('input', 'add') ||
      $trigger['#parents'] == array('input', 'set')) {
      $form_state['factors'][$year] = $factor;
    }
  }

  $rows = $form_state['factors'];
  array_walk($rows, 'lib4ridora_jif_json_map_to_row');
  ksort($rows, SORT_NUMERIC);
  $form['existing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Impact Factors'),
    'table' => array(
      '#type' => 'tableselect',
      '#header' => array('year' => t('Year'), 'factor' => t('Impact Factor')),
      '#options' => $rows,
      '#empty' => t('No entries...'),
    ),
    'remove' => array(
      '#type' => 'button',
      '#value' => t('Remove selected'),
      '#ajax' => array(
        'wrapper' => $wrapper_id,
        'callback' => 'lib4ridora_jif_json_form_refresh_ajax',
      ),
    ),
  );
  $form['input'] = array(
    '#type' => 'fieldset',
    '#title' => t('Input'),
    '#description' => t('"Add" requires that an entry for the year to be absent. "Set" requires one to exist for the given year.'),
    'year' => array(
      '#type' => 'textfield',
      '#title' => t('Year'),
      '#element_validate' => array(
        'element_validate_integer_positive',
      ),
    ),
    'factor' => array(
      '#type' => 'textfield',
      '#title' => t('Impact Factor'),
      '#element_validate' => array(
        'element_validate_number',
      ),
    ),
    'add' => array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#ajax' => array(
        'wrapper' => $wrapper_id,
        'callback' => 'lib4ridora_jif_json_form_refresh_ajax',
      ),
    ),
    'set' => array(
      '#type' => 'button',
      '#value' => t('Set'),
      '#ajax' => array(
        'wrapper' => $wrapper_id,
        'callback' => 'lib4ridora_jif_json_form_refresh_ajax',
      ),
    ),
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Map JSON structure to rows for use in a tableselect.
 *
 * Callback for array_walk().
 */
function lib4ridora_jif_json_map_to_row(&$factor, $year) {
  $factor = array(
    'year' => $year,
    'factor' => $factor,
  );
}

/**
 * Form validation callback; ensure values are provided when required.
 */
function lib4ridora_jif_json_form_validate(&$form, &$form_state) {
  $trigger = $form_state['triggering_element'];

  $year = drupal_array_get_nested_value($form_state['values'], array('input', 'year'));
  $factor = drupal_array_get_nested_value($form_state['values'], array('input', 'factor'));

  if ($trigger['#parents'] == array('existing', 'remove')) {
    $selected = array_filter(drupal_array_get_nested_value($form_state['values'], array('existing', 'table')));
    if (empty($selected)) {
      form_set_error('', t('Select at least one factor to remove.'));
    }
  }
  elseif ($trigger['#parents'] == array('input', 'add')) {
    if (empty($year)) {
      form_error($form['input']['year'], t('A "Year" value must be provided.'));
    }
    if (empty($factor)) {
      form_error($form['input']['factor'], t('A "Factor" value must be provided.'));
    }
    if (isset($form_state['factors'][$year])) {
      form_error($form['input']['year'], t('An entry already exists for the given year. Use "Set" if you wish to update it.'));
    }
  }
  elseif ($trigger['#parents'] == array('input', 'set')) {
    if (empty($year)) {
      form_error($form['input']['year'], t('A "Year" value must be provided.'));
    }
    if (empty($factor)) {
      form_error($form['input']['factor'], t('A "Factor" value must be provided.'));
    }
    if (!isset($form_state['factors'][$year])) {
      form_error($form['input']['year'], t('A value does not exist for the given year. Use "Add" to add a new factor.'));
    }
  }
}

/**
 * Form submission callback; save the impact factors to the object.
 */
function lib4ridora_jif_json_form_submit(&$form, &$form_state) {
  list($object_id) = $form_state['build_info']['args'];

  $object = islandora_object_load($object_id);

  $datastream = isset($object[LIB4RIDORA_FACTOR_DSID]) ?
    $object[LIB4RIDORA_FACTOR_DSID] :
    $object->constructDatastream(LIB4RIDORA_FACTOR_DSID, 'M');
  if ($datastream->mimetype != 'application/json') {
    $datastream->mimetype = 'application/json';
  }
  if ($datastream->label != 'Journal Impact Factors') {
    $datastream->label = 'Journal Impact Factors';
  }
  $datastream->content = json_encode($form_state['factors']);

  if (isset($object[LIB4RIDORA_FACTOR_DSID])) {
    drupal_set_message(t('Updated impact factors in @journal_pid.', array(
      '@journal_pid' => $object_id,
    )));
  }
  else {
    $object->ingestDatastream($datastream);
    drupal_set_message(t('Added impact factors to @journal_pid.', array(
      '@journal_pid' => $object_id,
    )));
  }
}

/**
 * AJAX callback.
 *
 * Dump out a message and return ALL of the form.
 */
function lib4ridora_jif_json_form_refresh_ajax(&$form, &$form_state) {
  drupal_set_message(t('Note: Values set, added or removed are not persisted on the object until you hit "@save_text".', array(
    '@save_text' => t('Save'),
  )), 'warning');
  return $form;
}
