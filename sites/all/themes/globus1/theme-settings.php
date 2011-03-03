<?php
// $Id$
/**
 * @file
 * include the theme settings from replicator
 */
/*
This is a shameless copy from the zen subtheme (starterkit) theme-settings
go grap the code and send john Albin a beer :)
drupal.org/project/zen
 */
include_once './' . drupal_get_path('theme', 'replicator') . '/theme-settings.php';


function globus1_settings($saved_settings) {
  // Get the default values from the .info file.
  $defaults = replicator_theme_get_default_settings('globus1'); //EDIT THIS!

  $settings = array_merge($defaults, $saved_settings);
  // Add the base theme's settings.
  $form = array();
  $form += replicator_settings($saved_settings, $defaults);

  // Return the form
  return $form;
}
