<?php

/**
 * Implementation of the gridalizer configuration form.
 *
 */
function gridalizer_admin_form(&$form_state) {
  $form = array();

  // Find all modules that implements hook_block and is allowed to user with
  // context filter.
  $selected = array_filter(variable_get('context_filter_block_types', array()));
  if (empty($selected)) {
    // Set message to configure context filter.
    drupal_set_message(t('You have not yet configured "Context filter". Click <a href="@link">here</a> to configure it.',
            array('@link' => url('admin/build/context/filter'))));

    // Find all modules that implements blocks.
    foreach (module_implements('block') as $module) {
      $selected[$module] = $module;
    }
  }
  
  // Block provider field set.
  $form['gridalizer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block type'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#description' => t('Select the types of blocks to add grid classes to.'),
    '#tree' => TRUE,
  );

  // Available block types (filter by context filter),
  $default = variable_get('gridalizer', array());
  $form['gridalizer']['modules'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Block providers:'),
    '#options' => $selected,
    '#default_value' => $default['modules'],
  );

  // Regions selection.
  $form['gridalizer_regions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Regions'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#description' => t('Select regions that have blocks that should be modified.'),
    '#tree' => TRUE,
  );

  // Load default region values
  $default_regions = variable_get('gridalizer_regions', array());
  
  // Get list of themes
  $themes = list_themes();
  foreach ($themes as $key => $theme) {
    if ($theme->status) {
      $theme_key = $theme->name;

      // Set a fieldset for each theme
      $form['gridalizer_regions'][$theme_key] = array(
        '#type' => 'fieldset',
        '#title' => t('Regions for "@theme"', array('@theme' => $theme->info['name'])),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // Build options array
      $options = array();
      $regions = $theme->info['regions'];
      foreach ($regions as $key => $region) {
        $options[$key] = $region;
      }

      // Build checkboxes for each theme
      $form['gridalizer_regions'][$theme_key]['enabled_regions'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Available regions'),
        '#options' => $options,
        '#default_value' => isset($default_regions[$theme_key]) ? $default_regions[$theme_key]['enabled_regions'] : array(),
      );
    }
  }

  return system_settings_form($form);
}