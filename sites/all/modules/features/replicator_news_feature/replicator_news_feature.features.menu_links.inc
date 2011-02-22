<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function replicator_news_feature_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-links:nyheder
  $menu_links['primary-links:nyheder'] = array(
    'menu_name' => 'primary-links',
    'link_path' => 'nyheder',
    'router_path' => 'nyheder',
    'link_title' => 'Nyheder',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Nyheder');


  return $menu_links;
}
