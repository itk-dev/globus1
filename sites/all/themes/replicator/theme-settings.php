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
  global $vars;

  $form['navigation'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Navigation'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  // Breadcrumb
  $form['navigation']['replicator_breadcrumb_separator'] = array(
    '#type'          => 'textfield',
    '#size'          => 3,
    '#title'         => t('Breadcrumb separator'),
    '#default_value' => $settings['replicator_breadcrumb_separator'],
    '#description'   => t('This is the symbol used for separating items in the breadcrumb. Leave blank if you don\'t want a separator.'),
  );

  // Stylesheets
  $form['stylesheets'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Styles'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['stylesheets']['replicator_stylesheet_conditional'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use IE specific stylesheet conditions in the .info file'),
    '#default_value' => $settings['replicator_stylesheet_conditional'],
    '#description'   => t('Use IE specific stylesheets in the .info file: <strong>IE stylesheets[ condition ][all][] = ie.css</strong> condition exampels [if lt IE 7] , [if IE 7] , [if IE 6]'),
  );

  // Typography
  $form['typography'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Typography'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['typography']['replicator_typography_links_icons'] = array(
    '#type'          => 'radios',
    '#options'       => array('no' => t('Don\'t show document icons'),'single' => t('Show general file icon'), 'separate' => t('Show specific file icons')),
    '#title'         => t('Show icons on links to downloadable files'),
    '#default_value' => isset($settings['replicator_typography_links_icons']) ? $settings['replicator_typography_links_icons'] : 'no',
    '#description'   => t('Show icons on links to downloadable files like .pdfs or word-documents. Options are either to treat such links like any other link, show a general document icon or different icons for different file types'),
  );
  
  // Misc
  $form['misc'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Misc'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  $form['misc']['replicator_aakb_topbar'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display Aarhus Kommune topbar'),
    '#default_value' => $settings['replicator_aakb_topbar'],
    '#description'   => t('Display Aarhus Kommunes topbar.'),
  );

  // Theme development
  $form['development'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Development'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

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