<?php

/**
 * @file
 * Form code for uploading impact factors.
 */

/**
 * Build a form for uploading the impact factor CSV.
 */
function lib4ridora_journal_impact_factor_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $form['year'] = array(
    '#type' => 'textfield',
    '#title' => t('Year'),
    '#description' => t('The year the CSV applies to.'),
    '#required' => TRUE,
  );
  $form['delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('Delimiter'),
    '#description' => t('The column separator in the CSV.'),
    '#required' => TRUE,
    '#default_value' => ';',
  );
  $form['csv'] = array(
    '#title' => t('CSV'),
    '#description' => t('CSV file containint journal impact factors.'),
    '#default_value' => isset($form_state['values']['csv']) ? $form_state['values']['csv'] : NULL,
    '#required' => TRUE,
    '#type' => 'managed_file',
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('csv'),
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set Impact Factors'),
  );
  return $form;
}

/**
 * Validate a form for uploading the impact factor CSV.
 */
function lib4ridora_journal_impact_factor_form_validate(array $form, array &$form_state) {
  if (!ctype_digit($form_state['values']['year']) || strlen($form_state['values']['year']) != 4) {
    form_error($form['year'], t('Year must be a in the form YYYY.'));
  }
}

/**
 * Submit handler for the form for uploading the impact factor CSV.
 */
function lib4ridora_journal_impact_factor_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'lib4ridora', 'includes/journal_impact.batch');
  $values = &$form_state['values'];
  $csv = file_load($values['csv']);
  $csv->status &= ~FILE_STATUS_PERMANENT;
  file_save($csv);
  batch_set(
    lib4ridora_impact_factor_batch(
      $values['year'],
      $values['delimiter'],
      $csv
    )
  );
}
