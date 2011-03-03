  <?php if ($content['top']) { ?>
  <div class="clear panel-col-top">
    <?php print $content['top']; ?>
  </div>
  <?php } ?>
  
  <?php if ($content['left']) { ?>
    <div class="grid-3 alpha panel-col-left">
      <?php print $content['left']; ?>
    </div>
  <?php } ?>

  <?php if ($content['center']) { ?>
  <div class="<?php if (!$content['left']) { ?>prefix-2 alpha <?php } ?><?php if (!$content['right']) { ?><?php } ?>grid-6 panel-col-center">
    <?php print $content['center']; ?>
  </div>
  <?php } ?>
  
  <?php if ($content['right']) { ?>
  <div class="grid-3 omega panel-col-right">
    <?php print $content['right']; ?>
  </div>
  <?php } ?>
  
  <?php if ($content['bottom']) { ?>
  <div class="clear panel-col-bottom">
    <?php print $content['bottom']; ?>
  </div>
  <?php } ?>
