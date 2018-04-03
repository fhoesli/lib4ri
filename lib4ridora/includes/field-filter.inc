<?php

/**
 * @file
 * Filter XML form fields.
 */

/**
 * Prevent access to protected fields.
 *
 * @param array $element
 *   A reference to an array representing a Drupal form API element.
 * @param bool $parent_enabled_filtering
 *   A boolean used primarily for recursion... Providing as TRUE will force
 *   our filtering constraints to be applied.
 */
function lib4ridora_filter_xml_form_field(&$element, $parent_enabled_filtering = FALSE) {
  $element['#access'] = lib4ridora_filter_xml_form_access($element, $parent_enabled_filtering);
  if ($element['#access']) {
    // If access, process children.
    foreach (element_children($element) as $key) {
      lib4ridora_filter_xml_form_field($element[$key], $parent_enabled_filtering);
    }
  }
  else {
    // No-op; No access to this element means no access to children, no need
    // to check them.
  }
}

/**
 * Determine if the current user should have access to this element.
 *
 * @param array $element
 *   An array representing a Drupal form API element.
 * @param bool $parent
 *   A reference to a boolean value, so we can indicate if we have turned on
 *   the role filtering in this tree.
 *
 * @return bool
 *   TRUE if either filtering has been enabled on this element (or a parent) and
 *   the user has one of the allowed roles on this element, or this element has
 *   no roles defined. FALSE if filtering is enabled, roles are specified and we
 *   do /not/ have an allowed role.
 */
function lib4ridora_filter_xml_form_access($element, &$parent = FALSE) {
  $parent = $parent || (isset($element['#user_data']['lib4ridora_field_filter']) && $element['#user_data']['lib4ridora_field_filter']);
  if ($parent) {
    global $user;

    $preg_grep_keys = function ($pattern, $input, $flags = 0) {
      // Function adapted from
      // http://php.net/manual/en/function.preg-grep.php#111673
      return array_intersect_key($input, array_flip(preg_grep($pattern, array_keys($input), $flags)));
    };

    if (isset($element['#user_data'])) {
      $roles = $preg_grep_keys('/^lib4ridora_field_filter_role(?:_.*)?$/', $element['#user_data']);
      $permissions = $preg_grep_keys('/^lib4ridora_field_filter_permission(?:_.*)?$/', $element['#user_data']);
      return (empty($roles) && empty($permissions)) ||
        array_intersect($roles, $user->roles) ||
        array_filter($permissions, 'user_access');
    }
  }

  return TRUE;
}
