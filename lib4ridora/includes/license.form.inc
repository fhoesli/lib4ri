<?php

/**
 * @file
 * Ingest step form for licensing.
 */

/**
 * License form.
 */
function lib4ridora_license_form($form, &$form_state) {
  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'lib4ridora') . '/css/license.css',
    ),
  );
  $form['license'] = array(
    '#type' => 'item',
    '#title' => t('Deposit License'),
    '#markup' => t('For full-text publications to be made public available.'),
  );
  $form['rights'] = array(
    '#type' => 'item',
    '#title' => t('Your right to publish your work'),
    '#markup' => t('Rights granted to the repository through this agreement are non-exclusive. You are free to publish the Work in its present or future version(s) elsewhere and no ownership is assumed by the repository when storing your Work.'),
  );
  $form['declaration'] = array(
    '#type' => 'item',
    '#title' => t("Depositor's declaration"),
    'agree' => array(
      '#markup' => t('By agreeing to this license, you (the author(s), copyright owner or assignee), grant a non-exclusive license to the repository that authorises the following:'),
    ),
    'list' => array(
      '#theme' => 'item_list',
      '#type' => 'ol',
      '#items' => array(
        t('You are the author, copyright holder or assignee and have authority to make this agreement, and give the Lib4RI the right to publish the Work in a repository.'),
        t("That the work is original and does not, to the best of you knowledge infringe upon anyone's copyright"),
      ),
      '#attributes' => array(
        'class' => array('lib4ri-license-list'),
      ),
    ),
  );
  $form['permission'] = array(
    '#type' => 'item',
    '#title' => t('Permissions given to the repository'),
    '#markup' => t('The repository shall distribute electronic copies of the Work for the lifetime of the repository and translate it as necessary to ensure it can be read by computer systems in the future. No charge will be made for electronic distribution.'),
  );
  $form['withdraw'] = array(
    '#type' => 'item',
    '#title' => t('Withdrawing your work'),
    '#markup' => t('You may request the Work is removed at any point in the future. Equally, the Lib4RI reserves the right to remove the Work for any professional, administrative or legal reason. A metadata record indicating the Work was stored in the repository will remain visible in perpetuity.'),
  );
  $form['disclaimer'] = array(
    '#type' => 'item',
    '#title' => t('Disclaimer'),
    '#markup' => t('While every care will be taken to preserve the Work, the Lib4RI is not liable for loss or damage to the work or other data while it is stored within the repository.'),
  );
  $form['definition'] = array(
    '#type' => 'item',
    '#title' => t('Definition & terms'),
    '#markup' => t('In this licence document ‘Work’ means the electronic version of the scientific publication being deposited including abstract, text, images and related data.'),
  );
  return $form;
}
