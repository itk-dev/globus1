<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <?php print $scripts; ?>
  </head>
  <body <?php print drupal_attributes($attr) ?>>

  <?php if ($site_aak_topbar): ?>
    <?php print $site_aak_topbar ?>
  <?php endif; ?>

  <div class="header">
    <div class="header-inner container-12">

      <?php if ($site_logo): ?>
        <div id="logo" class="logo"><?php print $site_logo ?></div>
      <?php endif; ?>

      <?php if ($site_slogan): ?>
        <div id="site-slogan" class="site-slogan"><?php print $site_slogan ?></div>
      <?php endif; ?>

      <?php if ($header): ?>
        <?php print $header; ?>
      <?php endif; ?>
        
    </div>
  </div>

  <?php if ($breadcrumb): ?>
    <div id="breadcrumb" class="container-12">
      <?php print $breadcrumb; ?>
    </div>
   <?php endif; ?>

  <div id="page" class="container-12">
    <div class="page-inner">
      
      <?php if ($tabs): ?>
        <div class="tabs">
          <?php print $tabs; ?>
        </div>
      <?php endif; ?>

      <?php if ($sidebar_first): ?>
        <div id="left" class="grid-3">
          <?php print $sidebar_first ?>
        </div>
      <?php endif ?>

      <div id="main" class="<?php print ns('grid-12', $sidebar_first, 3, $sidebar_second, 3); ?>">

        <?php print $messages; ?>
        <?php print $help; ?>

        <div id="content"><?php print $content ?></div>
      </div>

      <?php if ($sidebar_second): ?>
        <div id="right" class="grid-3">
          <?php print $sidebar_second ?>
        </div>
      <?php endif ?>
    </div>
  </div>

  <div id="footer"><div class="container-12 clear">
    <?php print $footer ?>
    <?php print $footer_message ?>
  </div></div>

  <?php print $closure ?>

  </body>
</html>