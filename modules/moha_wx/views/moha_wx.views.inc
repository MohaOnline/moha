<?php
/**
 * @file
 *   Stores hook_views_data, hook_views_data_alter(), hook_views_plugins(), hook_views_handlers().
 */

/**
 * Implements hook_views_data().
 *
 * Exposes table moha_wx_accounts to views.
 *
 * @return array
 *   array key: table name without prefix.
 *
 */
function moha_wx_views_data() {
  $data = array();

  $data['moha_wx_accounts']['table']['group'] = t('Moha WeChat account'); // Group name before field name.
  $data['moha_wx_accounts']['table']['base'] = array (   // Entry for creating views.
    'field' => 'aid',  // Primary Key.
    'title' => t('Moha WeChat accounts'),
    'help' => t('Records of Moha WeChat accounts.'),
  );

  // User ID / 'uid', @see user.views.inc.
  $data['moha_wx_accounts']['uid'] = array(
    'title' => t('Owner'),
    'help' => t('The user who owns the account. If you need more fields than the uid add the content: owner relationship'),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    // Declares relationship which can be added later on views_ui.
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'title' => t('Owner'),
      'help' => t('Relate account to the user who owns it.'),
      'handler' => 'views_handler_relationship',
      'label' => t('Owner'),
    ),
  );

  // The raw user ID / 'uid_raw'.
  $data['moha_wx_accounts']['uid_raw'] = array(
    'help' => t('The raw numeric user ID.'),
    'real field' => 'uid',
    'filter' => array(
      'title' => t('The user ID'),
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  // Fields: see https://www.moha.online/help/views/api-tables.
  $data['moha_wx_accounts']['account_name'] = array(
    'title' => t('Account name'),
    'help' => t('Human readable account name.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['moha_wx_accounts']['access_token_updated'] = array(
    'title' => t('Access Token Refreshed Time'),
    'help' => t('Stores timestamp when access token is refreshed.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}
