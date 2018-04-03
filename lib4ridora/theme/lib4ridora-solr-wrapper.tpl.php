<?php
/**
 * @file
 * Override of Islandora solr search results wrapper template
 *
 * Variables available:
 * - $variables: all array elements of $variables can be used as a variable.
 *   e.g. $base_url equals $variables['base_url']
 * - $base_url: The base url of the current website. eg: http://example.com .
 * - $user: The user object.
 *
 * - $secondary_profiles: Rendered secondary profiles
 * - $results: Rendered search results (primary profile)
 * - $islandora_solr_result_count: Solr result count string
 * - $solrpager: The pager
 * - $solr_debug: debug info
 *
 * Override the $secondary_profiles output to include &citation=true at the end
 * of the url.
 */
?>

<div id="islandora-solr-top">
  <?php
    // Check to see if the secondary profile includes an rss link and if it does
    // add &citation=true to the end of the url.
    $delimiter = "solr_profile=rss";
    $pieces = explode($delimiter, $secondary_profiles);
    if (count($pieces) > 1) {
      $pieces[0] = $pieces[0].$delimiter."&amp;citation=true";
      $secondary_profiles = implode("",$pieces);
    }
    print $secondary_profiles;
  ?>
  <div id="islandora-solr-result-count"><?php print $islandora_solr_result_count; ?></div>
</div>
<div class="islandora-solr-content">
  <?php print $solr_pager; ?>
  <?php print $results; ?>
  <?php print $solr_debug; ?>
  <?php print $solr_pager; ?>
</div>
