<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function spotboxes_user_default_permissions() {
  $permissions = array();

  // Exported permission: create spotbox content
  $permissions['create spotbox content'] = array(
    'name' => 'create spotbox content',
    'roles' => array(),
  );

  // Exported permission: edit field_spotbox_external_link
  $permissions['edit field_spotbox_external_link'] = array(
    'name' => 'edit field_spotbox_external_link',
    'roles' => array(),
  );

  // Exported permission: edit field_spotbox_image
  $permissions['edit field_spotbox_image'] = array(
    'name' => 'edit field_spotbox_image',
    'roles' => array(),
  );

  // Exported permission: view field_spotbox_external_link
  $permissions['view field_spotbox_external_link'] = array(
    'name' => 'view field_spotbox_external_link',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_spotbox_image
  $permissions['view field_spotbox_image'] = array(
    'name' => 'view field_spotbox_image',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  return $permissions;
}
