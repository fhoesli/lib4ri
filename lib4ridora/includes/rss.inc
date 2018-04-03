<?php

/**
 * @file
 * Helper function(s) for generating the RSS feed.
 */

/**
 * Pare down MODS to what is wanted for RSS.
 *
 * @param DOMDocument $doc
 *   The MODS document to pare down.
 *
 * @return DOMDocument
 *   The pared MODS document.
 */
function lib4ridora_mods_for_rss(DOMDocument $doc) {
  $pared = lib4ridora_run_xslt($doc, drupal_get_path('module', 'lib4ridora') . '/xml/mods-pare.xslt');
  return lib4ridora_run_xslt($pared, drupal_get_path('module', 'xml_form_builder') . '/self_transforms/islandora_cleanup_mods_extended.xsl');
}

/**
 * Helper function to run an XSLT.
 *
 * @param DOMDocument $input
 *   The document to tranform.
 * @param string $xslt
 *   Path to the XSLT to transform.
 *
 * @return DOMDocument
 *   The transformed XML, as a DOMDocument.
 */
function lib4ridora_run_xslt(DOMDocument $input, $xslt) {
  $xslt_processor = new XSLTProcessor();
  $xslt_doc = new DOMDocument();
  $xslt_doc->load($xslt);
  $xslt_processor->importStylesheet($xslt_doc);
  return $xslt_processor->transformToDoc($input);
}
