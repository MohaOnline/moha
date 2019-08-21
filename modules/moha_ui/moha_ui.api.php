<?php
/**
 * @file
 * API file of the module.
 */

/**
 * Return items of dashboard sidebar.
 *
 * @link https://fontawesome.com/v4.7.0/icons/ @endlink
 */
function hook_moha_ui_dashboard_sidebar() {
  if (!user_access(MOHA_UI__PERMISSION__DASHBOARD)) {
    return array();
  }

  $items['features']['admin/moha/dashboard'] = array(
    '#markup' => l('<i class="fa fa-fa-tachometer"></i> <span>' . t('Overview') . '</span>', 'admin/moha/dashboard', array('html' => TRUE)),
  );

  return $items;
}

/**
 * Return items of dashboard overview.
 *
 * @return array
 */
function hook_moha_ui_dashboard_overview() {
  if (!user_access(MOHA_UI__PERMISSION__DASHBOARD)) {
    return array();
  }

  $items['summary']['module_prefix_unique_name'] = array(
    '#markup' => '
<div class="info-box bg-aqua">
  <span class="info-box-icon"><i class="fa fa-bookmark-o"></i></span>

  <div class="info-box-content">
    <span class="info-box-text">Bookmarks</span>
    <span class="info-box-number">41,410</span>

    <div class="progress">
      <div class="progress-bar" style="width: 70%"></div>
    </div>
        <span class="progress-description">
          70% Increase in 30 Days
        </span>
  </div>
  <!-- /.info-box-content -->
</div>    
    ',
  );

  return $items;
}
