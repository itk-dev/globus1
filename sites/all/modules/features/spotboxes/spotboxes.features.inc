<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function spotboxes_imagecache_default_presets() {
  $items = array(
    'spotbox' => array(
      'presetname' => 'spotbox',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '300',
            'height' => '100',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function spotboxes_node_info() {
  $items = array(
    'spotbox' => array(
      'name' => t('Spotbox'),
      'module' => 'features',
      'description' => t('Create a spotbox for your site. There are several different types to choose from.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function spotboxes_views_api() {
  return array(
    'api' => '2',
  );
}
