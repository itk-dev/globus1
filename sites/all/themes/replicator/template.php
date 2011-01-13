<?php

// Based loosely on the mothership theme - check it out: http://drupal.org/project/mothership

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('replicator_rebuild_registry')) {
  drupal_rebuild_theme_registry();
}

function replicator_preprocess(&$vars, $hook) {

  if ($hook == "page") {
    // conditional styles
    // xpressions documentation  -> http://msdn.microsoft.com/en-us/library/ms537512.aspx

    // syntax for .info
    // top stylesheets[all][] = style/reset.css
    // ie stylesheets[ condition ][all][] = ie6.css
    // ------------------------------------------------------------------------

    // Check for IE conditional stylesheets.

    if (theme_get_setting('replicator_stylesheet_conditional')) {
      GLOBAL $theme_info;
      // Get the path to the theme to make the code more efficient and simple.
      $path = drupal_get_path('theme', $theme_info->name);
    }

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

}

function replicator_preprocess_page(&$vars, $hook) {
  // Set variables for the logo and site_name.
  if (!empty($vars['logo'])) {
    // Return the site_name even when site_name is disabled in theme settings.
    $vars['logo_alt_text'] = (empty($vars['logo_alt_text']) ? variable_get('site_name', '') : $vars['logo_alt_text']);
    $vars['site_logo'] = '<a id="site-logo" href="'. $vars['front_page'] .'" title="'. $vars['logo_alt_text'] .'" rel="home"><img src="'. $vars['logo'] .'" alt="'. $vars['logo_alt_text'] .'" /></a>';
  }
}

?>