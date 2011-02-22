<?php

function gridalizer_settings_admin_form(&$form_state) {
  $form = array();

  // Select themes
  $options = array();
  $themes = list_themes();
  foreach ($themes as $theme) {
    if ($theme->status) {
      $options[$theme->name] = $theme->info['name'];
    }
  }

  $form['gridalizer_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Active themes:'),
    '#options' => $options,
    '#description' => t('Selected the theme that gridalizer should be used on.'),
    '#default_value' => variable_get('gridalizer_theme', ''),
  );

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

  // Available block types (filter by context filter),
  $form['gridalizer_blocks'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Block providers:'),
    '#options' => $selected,
    '#description' => t('The block providers selected here, will be the once that will have the grid classes added'),
    '#default_value' => variable_get('gridalizer_blocks', array()),
  );

  $form['gridalizer_max_blocks'] = array(
    '#type' => 'textfield',
    '#title' => t('Max. blocks in row'),
    '#description' => t('The maximum number of blocks in a single row.'),
    '#default_value' => variable_get('gridalizer_max_blocks', '4'),
  );

//  $form['gridalizer_on_block'] = array(
//    '#type' => 'checkboxes',
//    '#title' => t('Set classes on block'),
//    '#options' => array('yes' => t('Yes')),
//    '#description' => t('Enables the setting of grid classes on edit block pages'),
//    '#default_value' => variable_get('gridalizer_on_block', array()),
//  );

  // return
  return system_settings_form($form);
}

function gridalizer_regions_admin_form(&$form_state) {
  $form = array();

  // Check that a theme have been selected
  $selected_theme = variable_get('gridalizer_theme', NULL);
  if (!isset($selected_theme)) {
    drupal_set_message(t('You have not yet selected a theme for "Gridalizer". Click <a href="@link">here</a> to configure it.',
          array('@link' => url('admin/build/context/gridalizer'))));
    return;
  }

  // Regions avaliable in selected theme
  $regions = system_region_list($selected_theme);

  // Load defaults
  $defaults = variable_get('gridalizer_regions', array());
  $form['gridalizer_regions'] = array(
      '#tree' => TRUE,
  );
  
  // Selecte side regions
  $form['gridalizer_regions']['side'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select side regions'),
    '#options' => $regions,
    '#description' => t('If these regions are empty, they will effect the grid number'),
    '#default_value' => isset($defaults['side']) ? $defaults['side'] : array(),
  );

  // Select active regions
  $form['gridalizer_regions']['active'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Gridalizes regions'),
    '#options' => $regions,
    '#description' => t('Select the regions that should be gridalizes'),
    '#default_value' => isset($defaults['active']) ? $defaults['active'] : array(),
  );

  // Regions without sides
  // Select active regions
  $form['gridalizer_regions']['nosides'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Regions without sides'),
    '#options' => $regions,
    '#description' => t('Select the regions that should be gridalizes that have no sides. Classes from empty side regions will be used.'),
    '#default_value' => isset($defaults['nosides']) ? $defaults['nosides'] : array(),
  );

  return system_settings_form($form);
}

function gridalizer_classes_admin_form(&$form_state) {
  $form = array();

  // Load selected regions
  $regions = variable_get('gridalizer_regions', NULL);
  if (!isset($regions)) {
    drupal_set_message(t('You have not yet selected regions for "Gridalizer". Click <a href="@link">here</a> to configure it.',
          array('@link' => url('admin/build/context/gridalizer/regions'))));
    return;
  }

  // Clean regions
  $regions['side'] = array_filter($regions['side']);
  $regions['active'] = array_filter($regions['active']);

  // Find number of combinations
  $sides = array_values($regions['side']);
  $combinations = gridalizer_combin_array($sides);

  // Out form field set
  $form['gridalizer_combinations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default grid classes'),
    '#description' => t('Add default grid classes to blocks in different situations.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  // Load configuration
  $max_blocks = variable_get('gridalizer_max_blocks', '4');
  $defaults = variable_get('gridalizer_combinations', array());
  $providers = array_filter(variable_get('gridalizer_blocks', array()));

  foreach ($combinations as $combination) {

    // Create database key
    if (count($combination) > 1) {
      sort($combination);
    }
    $key = empty($combination) ? 'empty' : implode('_', $combination);

    // Create form elements
    $title = empty($combination) ? t('non of the regions') : implode(' + ', $combination);
    $form['gridalizer_combinations'][$key] = array(
      '#type' => 'fieldset',
      '#title' => t('Content in %comb', array('%comb' => $title)),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['gridalizer_combinations'][$key][0] = array(
      '#type' => 'textfield',
      '#title' => t('Default classe'),
      '#description' => t('The maximum number of blocks in a single row.'),
      '#default_value' => isset($defaults[$key]) ? $defaults[$key][0] : '',
    );

    for ($i = 1; $i <= (int)$max_blocks; $i++) {
      $form['gridalizer_combinations'][$key][$i] = array(
        '#type' => 'textfield',
        '#title' => t('%number blocks', array('%number' => $i)),
        '#description' => t('The maximum number of blocks in a single row.'),
        '#default_value' => isset($defaults[$key]) ? $defaults[$key][$i] : '',
      );
    }
  }
  
  return system_settings_form($form);
}

function gridalizer_combin_array($array) {
  // Initialize the set
  $results = array(array());

  foreach ($array as $element) {
    foreach ($results as $combination) {
      array_push($results, array_merge(array($element), $combination));
    }
  }
  return $results;
}
