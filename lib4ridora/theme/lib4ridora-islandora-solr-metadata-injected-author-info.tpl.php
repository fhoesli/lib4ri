<?php
/**
 * @file
 * Default template for lib4ridora-islandora-solr-metadata-injected-author-info.
 *
 * Somewhat gross, due to the default nl2br-type stuffs.
 *
 * Available variables:
 * - $author_attributes: An associative array which may contain:
 *   - href: URL for the author.
 * - $org_attributes: An associative array which may contain:
 *   - href: URL for the author's org.
 * - $info: Associative array containing:
 *   - author
 *     - name
 *     - id
 *   - org
 *     - name
 *     - id
 */

$author_element_type = isset($author_attributes['href']) ? 'a' : 'span';
$org_element_type = isset($org_attributes['href']) ? 'a' : 'span';
?>
<span class="<?php print $classes;?>"><<?php print $author_element_type; print drupal_attributes($author_attributes);?>><?php print $info['author']['name']; ?></<?php print $author_element_type;?>><?php
  if ($info['org']['name']):?> (<<?php print $org_element_type; print drupal_attributes($org_attributes);?>><?php print $info['org']['name']; ?></<?php print $org_element_type;?>>)<?php
  endif;?></span>
