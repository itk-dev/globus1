<?php

// Based loosely on the mothership theme - check it out: http://drupal.org/project/mothership

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('replicator_rebuild_registry')) {
  drupal_rebuild_theme_registry();
}

function replicator_preprocess(&$vars, $hook) {

  switch ($hook) {
    case 'page':

      global $theme_info;
      // Get the path to the theme to make the code more efficient and simple.
      $path = drupal_get_path('theme', $theme_info->name);

      // Set variables for the logo and site_name.
      if (!empty($vars['logo'])) {
        // Return the site_name even when site_name is disabled in theme settings.
        $vars['logo_alt_text'] = (empty($vars['logo_alt_text']) ? variable_get('site_name', '') : $vars['logo_alt_text']);
        $vars['site_logo'] = '<a id="site-logo" href="'. $vars['front_page'] .'" title="'. $vars['logo_alt_text'] .'" rel="home"><img src="'. $vars['logo'] .'" alt="'. $vars['logo_alt_text'] .'" /></a>';
      }

      if (theme_get_setting('replicator_aakb_topbar')) {

        $aak_topbar = array();

        $aak_topbar['url']        = 'http://www.aarhus.dk';
        $aak_topbar['title']      = 'Aarhus Kommune';
        $aak_topbar['link_name']  = 'G&aring; til Aarhus.dk';

        $vars['site_aak_topbar'] = '<div class="aak-topbar"><div class="aak-topbar-inner container-12"><a href="'. $aak_topbar['url'] .'" title="'. $aak_topbar['title'] .'" class="aak-logo"><img src="/sites/all/themes/replicator/images/aak-topbar/aak-logo.png" alt="'. $aak_topbar['title'] .'"></a><a href="'. $aak_topbar['url'] .'" title="'. $aak_topbar['title'] .'" class="aak-link">'. $aak_topbar['link_name'] .'</a></div></div>';
      }

      // conditional styles
      // xpressions documentation  -> http://msdn.microsoft.com/en-us/library/ms537512.aspx

      // syntax for .info
      // top stylesheets[all][] = style/reset.css
      // ie stylesheets[ condition ][all][] = ie6.css
      // ------------------------------------------------------------------------

      // Check for IE conditional stylesheets.
      if (isset($theme_info->info['ie stylesheets']) AND theme_get_setting('replicator_stylesheet_conditional')) {

        $ie_css = array();

        // Format the array to be compatible with drupal_get_css().
        foreach ($theme_info->info['ie stylesheets'] as $condition => $media) {
          foreach ($media as $type => $styles) {
            foreach ($styles as $style) {
              $ie_css[$condition][$type]['theme'][$path . '/' . $style] = TRUE;
            }
          }
        }
        // Append the stylesheets to $styles, grouping by IE version and applying
        // the proper wrapper.
        foreach ($ie_css as $condition => $styles) {
          $vars['styles'] .= '<!--[' . $condition . ']>' . "\n" . drupal_get_css($styles) . '<![endif]-->' . "\n";
        }
      }
      break;

    case 'block';
      // Fix context blocks sizes based on region content.
      $blocks = replicator_region_blocks();
      $list = replicator_context_region_blocks();

      // Find the right grid to use.
      if (!$blocks['right']) {
        $grid = 'grid-3';
        if (!$blocks['left']) {
          $grid = 'grid-4';
        }
      }

      // Find the block that needs to have the grid added.
      if (!$blocks['right'] && $vars['block']->region == 'content_top') {
        if (count($list['content_top']) == 3) {
          foreach ($blocks['content_top'] as $block) {
            // Test that current block is insert with the context module.
            if ($vars['block']->module == $block['module'] && $vars['block']->bid == $block['bid']) {
              // Block was not insert with the context module, so do not add
              // grid class.
              $grid = NULL;
              continue;
            }
          }
          $vars['block']->grid = $grid;
        }
      }
      break;
  }
}

/**
 * Helper function that creates an array of blocks for each region in the
 * current theme.
 *
 * @global object $theme_info
 * @staticvar array $block_list
 * @return array $block_list
 */
function replicator_region_blocks() {
  global $theme_info;
  static $block_list;

  if (!isset($block_list)) {
    $block_list = array();
    foreach ($theme_info->info['regions'] as $region => $value) {
      // Create empty array foreach region.
      if (!isset($block_list[$region])) {
        $block_list[$region] = array();
      }

      // Get blocks for current region.
      $blocks = block_list($region);
      foreach ($blocks as $block) {
        $block_list[$region][] = array('module' => $block->module, 'bid' => $block->bid);
      }
    }
  }

  return $block_list;
}

/**
 * Helper function that findes all blocks inserted by the context module into
 * a given region from the current theme.
 *
 * @staticvar array $region_blocks
 * @return array $region_blocks
 */
function replicator_context_region_blocks() {
  static $region_blocks;

  if (!isset($region_blocks)) {
    $region_blocks = array();

    // Get all active contexts.
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      // Get all blocks in this context.
      foreach ($context->reactions['block']['blocks'] as $block) {
        if (!isset($region_blocks[$block['region']])) {
          $region_blocks[$block['region']] = array();
        }
        $region_blocks[$block['region']][] = $block;
      }
    }
  }
  
  return $region_blocks;
}

/**
 * Add current page to breadcrumb
 */
function replicator_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    $title = drupal_get_title();
    if (!empty($title)) {
      // Get separator
      global $theme_info;
      if (theme_get_setting('replicator_breadcrumb_separator')) {
        $sep = '<span class="breadcrumb-sep">'. theme_get_setting('replicator_breadcrumb_separator').'</span>';
      }
      $breadcrumb[]='<span class="breadcrumb-current">'. $title .'</span>';
    }
    return '<div class="breadcrumb">'. implode($sep, $breadcrumb) .'</div>';
  }
}

// 960 ns function
function ns() {
  $args = func_get_args();
  $default = array_shift($args);
  // Get the type of class, i.e., 'grid', 'pull', 'push', etc.
  // Also get the default unit for the type to be procesed and returned.
  list($type, $return_unit) = explode('-', $default);

  // Process the conditions.
  $flip_states = array('var' => 'int', 'int' => 'var');
  $state = 'var';
  foreach ($args as $arg) {
    if ($state == 'var') {
      $var_state = !empty($arg);
    }
    elseif ($var_state) {
      $return_unit = $return_unit - $arg;
    }
    $state = $flip_states[$state];
  }

  $output = '';
  // Anything below a value of 1 is not needed.
  if ($return_unit > 0) {
    $output = $type . '-' . $return_unit;
  }
  return $output;
}

?>