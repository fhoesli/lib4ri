<?php
/**
 * @file
 * lib4ridora_journal_view_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function lib4ridora_journal_view_feature_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_islandora_solr_metadata_configurations_default_fields().
 */
function lib4ridora_journal_view_feature_islandora_solr_metadata_configurations_default_fields() {
  return array(
    'lib4ridora journal view' => array(
      'fields' => array(
        'mods_titleInfo_title_s' => array(
          'solr_field' => 'mods_titleInfo_title_s',
          'display_label' => 'Journal Title',
          'hyperlink' => 0,
          'date_format' => NULL,
          'permissions' => NULL,
          'structured_data' => NULL,
          'weight' => 1,
        ),
        'mods_titleInfo_abbreviated_title_s' => array(
          'solr_field' => 'mods_titleInfo_abbreviated_title_s',
          'display_label' => 'Abbreviated Journal Title',
          'hyperlink' => 0,
          'date_format' => NULL,
          'permissions' => NULL,
          'structured_data' => NULL,
          'weight' => 2,
        ),
        'mods_originInfo_publisher_s' => array(
          'solr_field' => 'mods_originInfo_publisher_s',
          'display_label' => 'Publisher',
          'hyperlink' => 0,
          'date_format' => NULL,
          'permissions' => NULL,
          'structured_data' => NULL,
          'weight' => 3,
        ),
        'mods_identifier_issn_s' => array(
          'solr_field' => 'mods_identifier_issn_s',
          'display_label' => 'ISSN',
          'hyperlink' => 0,
          'date_format' => NULL,
          'permissions' => NULL,
          'structured_data' => NULL,
          'weight' => 4,
        ),
        'mods_identifier_e-issn_s' => array(
          'solr_field' => 'mods_identifier_e-issn_s',
          'display_label' => 'E-ISSN',
          'hyperlink' => 0,
          'date_format' => NULL,
          'permissions' => NULL,
          'structured_data' => NULL,
          'weight' => 5,
        ),
      ),
      'cmodels' => array(
        'lib4ri:journalCModel' => array(
          'cmodel' => 'lib4ri:journalCModel',
        ),
      ),
      'description' => array(
        'description_field' => NULL,
        'description_label' => NULL,
      ),
      'name' => 'Journal View',
    ),
  );
}
