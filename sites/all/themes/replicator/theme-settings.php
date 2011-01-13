<?php
// $Id$
/**
 * @file
 * themesettings.php
 */

/**
 * Return the theme settings' default values from the .info and save them into the database.
 *
 * @param $theme
 *   The name of theme.
 */

function replicator_theme_get_default_settings($theme) {

  $themes = list_themes();

  // Get the default values from the .info file.
  $defaults = !empty($themes[$theme]->info['settings']) ? $themes[$theme]->info['settings'] : array();

  if (!empty($defaults)) {
    // Get the theme settings saved in the database.
    $settings = theme_get_settings($theme);
    // Don't save the toggle_node_info_ variables.
    if (module_exists('node')) {
      foreach (node_get_types() as $type => $name) {
        unset($settings['toggle_node_info_' . $type]);
      }
    }
    // Save default theme settings.
    variable_set(
      str_replace('/', '_', 'theme_' . $theme . '_settings'),
      array_merge($defaults, $settings)
    );
    // If the active theme has been loaded, force refresh of Drupal internals.
    if (!empty($GLOBALS['theme_key'])) {
      theme_get_setting('', TRUE);
    }
  }

  // Return the default settings.
  return $defaults;
}

//function phptemplate_settings($saved_settings) {
function replicator_settings($saved_settings, $subtheme_defaults = array()) {

  // Get the default values from the .info file.
  $defaults = replicator_theme_get_default_settings('replicator');

  // Allow a subtheme to override the default values.
  $defaults = array_merge($defaults, $subtheme_defaults); //zen ftw

  // Merge the saved variables and their default values.
  $settings = array_merge($defaults, $saved_settings);

  // Merge the saved variables and their default values.
  $settings = array_merge($defaults, $saved_settings);
  GLOBAL $vars;
  
  // Stylesheets
  $form['stylesheets']['replicator_stylesheet_conditional'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('ie specific stylesheet conditions in the .info file'),
    '#default_value' => $settings['replicator_stylesheet_conditional'],
    '#description'   => t('.info file: <strong>ie stylesheets[ condition ][all][] = ie.css</strong> condition exampels [if lt IE 7] , [if IE 7] , [if IE 6]'),
  );

  // Theme development
  $form['development']['replicator_rebuild_registry'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Rebuild theme registry on every page.'),
    '#default_value' => $settings['replicator_rebuild_registry'],
    '#description'   => t('During theme development, it can be very useful to continuously <a href="!link">rebuild the theme registry</a>. WARNING: this is a huge performance penalty and must be turned off on production websites.', array('!link' => 'http://drupal.org/node/173880#theme-registry')),
  );

  // Return form
  return $form;

}

?>