<?php
/**
 * @file
 * Administrative page for the module.
 */

/**
 * Forward user to centralized dashboard entry.
 * Forward user from admin/moha/its/dashboard to admin/moha/dashboard/its.
 */
function moha_its_dashboard_jump() {
  drupal_goto('admin/moha/dashboard/its');
}

/**
 * Prepare contents of ITS dashboard.
 * Styles and layout on information block on dashboard shipped with block.
 *
 * @link admin/moha/dashboard/its @endlink
 *
 * @return array
 *   Render array for rendering.
 */
function moha_its_admin_dashboard() {

  $content[MOHA_UI__DASHBOARD__CONTAINER] = array(
    '#type' => 'container'
  );

  $container = &$content[MOHA_UI__DASHBOARD__CONTAINER];

  /**
   * Load block content.
   *
   * @see moha_its_block_view
   * @see moha_its_block_view_azure_cn_expenses
   */
  $block = module_invoke(__MOHA_ITS, 'block_view', MOHA_ITS__BLOCK__AZURE_CHINA_EXPENSE_DETAILS);
  if (!empty($block['content'])) {
    $container['block_azure_cn_expense_details'] = $block['content'];
  }

  return $content;
}


