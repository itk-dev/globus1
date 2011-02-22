<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function replicator_news_feature_user_default_permissions() {
  $permissions = array();

  // Exported permission: create news content
  $permissions['create news content'] = array(
    'name' => 'create news content',
    'roles' => array(
      '0' => 'contributor',
      '1' => 'editor',
      '2' => 'webmaster',
    ),
  );

  // Exported permission: delete any news content
  $permissions['delete any news content'] = array(
    'name' => 'delete any news content',
    'roles' => array(
      '0' => 'editor',
      '1' => 'webmaster',
    ),
  );

  // Exported permission: delete own news content
  $permissions['delete own news content'] = array(
    'name' => 'delete own news content',
    'roles' => array(
      '0' => 'editor',
      '1' => 'webmaster',
    ),
  );

  // Exported permission: edit any news content
  $permissions['edit any news content'] = array(
    'name' => 'edit any news content',
    'roles' => array(
      '0' => 'editor',
      '1' => 'webmaster',
    ),
  );

  // Exported permission: edit field_maincontent
  $permissions['edit field_maincontent'] = array(
    'name' => 'edit field_maincontent',
    'roles' => array(
      '0' => 'contributor',
      '1' => 'editor',
      '2' => 'webmaster',
    ),
  );

  // Exported permission: edit field_news_teaser
  $permissions['edit field_news_teaser'] = array(
    'name' => 'edit field_news_teaser',
    'roles' => array(
      '0' => 'contributor',
      '1' => 'editor',
      '2' => 'webmaster',
    ),
  );

  // Exported permission: edit field_news_titleimage
  $permissions['edit field_news_titleimage'] = array(
    'name' => 'edit field_news_titleimage',
    'roles' => array(
      '0' => 'contributor',
      '1' => 'editor',
      '2' => 'webmaster',
    ),
  );

  // Exported permission: edit own news content
  $permissions['edit own news content'] = array(
    'name' => 'edit own news content',
    'roles' => array(
      '0' => 'contributor',
      '1' => 'editor',
      '2' => 'webmaster',
    ),
  );

  // Exported permission: view field_maincontent
  $permissions['view field_maincontent'] = array(
    'name' => 'view field_maincontent',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_news_teaser
  $permissions['view field_news_teaser'] = array(
    'name' => 'view field_news_teaser',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_news_titleimage
  $permissions['view field_news_titleimage'] = array(
    'name' => 'view field_news_titleimage',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view imagecache news_listimage
  $permissions['view imagecache news_listimage'] = array(
    'name' => 'view imagecache news_listimage',
    'roles' => array(),
  );

  // Exported permission: view imagecache news_titleimage
  $permissions['view imagecache news_titleimage'] = array(
    'name' => 'view imagecache news_titleimage',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  return $permissions;
}
