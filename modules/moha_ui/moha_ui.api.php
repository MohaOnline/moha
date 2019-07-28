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
  $items['features']['admin/moha/dashboard'] = array(
    '#markup' => l('<i class="fa fa-fa-tachometer"></i> <span>' . t('Overview') . '</span>', 'admin/moha/dashboard', array('html' => TRUE)),
  );

  return $items;
}
