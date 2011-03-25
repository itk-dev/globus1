<?php
/**
* Preprocess date navigation
* @param array $vars
* @return void
*/
function globus1_preprocess_date_navigation(&$vars) {
  $view = $vars['view'];

  $link = FALSE;
  // Month navigation titles are used as links in the block view.
  if (!empty($view->date_info->block) && $view->date_info->granularity == 'month') {
    $link = TRUE;
  }

  // Set Dutch format for days and weeks
  $format = NULL;
  switch ($view->date_info->granularity) {
    case 'day':
      $format = 'd. F';
      break;
    case 'week':
      $format = 'W';
      break;
  }
  $vars['nav_title'] = theme('date_nav_title', $view->date_info->granularity, $view, $link, $format);
}
