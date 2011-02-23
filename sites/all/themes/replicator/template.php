<?php

// Based loosely on the mothership theme - check it out: http://drupal.org/project/mothership

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('replicator_rebuild_registry')) {
  drupal_rebuild_theme_registry();
}

function replicator_preprocess_page(&$vars, $hook) {
  global $theme_info;
  
  // Get the path to the theme to make the code more efficient and simple.
  $path = drupal_get_path('theme', $theme_info->name);

  // Modify body-classes
  switch(theme_get_setting('replicator_typography_links_icons')) {
    case 'single':
      $vars['body_classes'] .= ' icon_single';
      break;
      
    case 'separate':
      $vars['body_classes'] .= ' icon_separate';
      break;
  }
  
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

    $vars['site_aak_topbar'] = '<div class="aak-topbar"><div class="aak-topbar-inner container-12"><a href="'. $aak_topbar['url'] .'" title="'. $aak_topbar['title'] .'" class="aak-logo"><img src="/sites/all/themes/replicator/images/aak-topbar/aak-logo.png" alt="'. $aak_topbar['title'] .'" /></a><a href="'. $aak_topbar['url'] .'" title="'. $aak_topbar['title'] .'" class="aak-link">'. $aak_topbar['link_name'] .'</a></div></div>';
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

// 960 ns function (modified)
function ns() {
  $args = func_get_args();
  $default = array_shift($args);

  // Get the type of class, i.e., 'grid', 'pull', 'push', etc.
  // Also get the default unit for the type to be procesed and returned.
  list($type, $return_unit) = explode('-', $default);

  // Check if panels are aktive. If this is a panels page, use rc value  
  if ($args[count($args)-2] == 'rc') {
    $val = array_pop($args);
    $key = array_pop($args);
    $pm_page = page_manager_get_current_page();
    if ($pm_page && $pm_page['handler']->conf['recalculate_ns']) {
      return $type . '-' . $val;
    }
  }

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