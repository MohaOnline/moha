<?php
/**
 * @file
 * @Created: 31/03/2018 4:32 PM
 */

/**
 * Determines if segment exists as a service account.
 *
 * @param string $wechat_name
 *   The ID of the text format to check.
 *
 * @return bool
 *   TRUE if segment exists as a service account, FALSE otherwise.
 *
 */
function _wechat_lite_service_accounts_segment_exists($wechat_name) {

  return FALSE;
}

/**
 * @param array $form
 * @param array $form_state
 * @param string $account_name
 */
function _moha_wx_account_form($form, &$form_state, $account_name = NULL){

  $account = (object) array(
    'account_name' => '',
    'wechat_name' => '',
    'app_id' => '',
    'app_secret' => '',
    'token' => '',
    'encoding_aes_key' => '',
    'text' => '',
    'news_title' => '',
    'news_cover' => '',
    'news_link' => '',
  );

  $result = db_query("SELECT * FROM {moha_wx_accounts} moha_wx_accounts WHERE wechat_name = :wechat_name",
    array(
      ':wechat_name' => $account_name,
    )
  );

  foreach ($result as $record) {
    $account = $record;
  }


  $form['account_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Name'),
    '#description' => t('Give a name to identify each WeChat Service Account.'),
    '#size' => 40,
    '#default_value' => $account->account_name,
    '#required' => TRUE,
  );

  $form['wechat_name'] = array(
    '#type' => 'textfield',
    '#title' => t('WeChat Name'),
    '#required' => TRUE,
    '#default_value' => $account->wechat_name,
    '#size' => 40,
  );

  $form['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('AppID'),
    '#default_value' => $account->app_id,
    '#required' => TRUE,
  );

  $form['app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('AppSecret'),
    '#description' => t(<<<'TAG'
If this information is lost, reset it from basic configuration page.
TAG
    ),
    '#default_value' => $account->app_secret,
    '#required' => TRUE,
  );

  $form['token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#default_value' => $account->token,
    '#required' => TRUE,
  );

  $form['encoding_aes_key'] = array(
    '#type' => 'textfield',
    '#title' => t('EncodingAESKey'),
    '#default_value' => $account->encoding_aes_key,
  );

  $form['news_title'] = array(
    '#type' => 'textfield',
    '#title' => t('News Title'),
    '#default_value' => $account->news_title,
  );

  $form['news_cover'] = array(
    '#type' => 'textfield',
    '#title' => t('News Cover'),
    '#maxlength' => 256,
    '#size' => 160,
    '#default_value' => $account->news_cover,
  );

  $form['news_link'] = array(
    '#type' => 'textfield',
    '#title' => t('News Link'),
    '#maxlength' => 256,
    '#size' => 160,
    '#default_value' => $account->news_link,
  );

  $form['text'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#title' => t('Text'),
    '#default_value' => $account->text,
    '#description' => t("Put text welcome message here."),
    '#format' => 'plain_text',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Account'),
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function _moha_wx_account_form_submit($form, &$form_state){

  $account = $form_state['values'];

  $affacted_row_number = db_update('moha_wx_accounts')
    ->fields(array(
      'account_name' => $account['account_name'],
      'app_id' => $account['app_id'],
      'app_secret' => $account['app_secret'],
      'token' => $account['token'],
      'encoding_aes_key' => $account['encoding_aes_key'],
      'news_title' => $account['news_title'],
      'news_cover' => $account['news_cover'],
      'news_link' => $account['news_link'],
      'text' => $account['text']['value'],
      'updated' => time(),
    ))
    ->condition('wechat_name', $account['wechat_name'])
    ->execute();

  if ($affacted_row_number == 1) {
    // Fetch full information of account.
    $account = db_query("SELECT * FROM {moha_wx_accounts} moha_wx_accounts WHERE wechat_name = :wechat_name",
      array(
        ':wechat_name' => $account['wechat_name'],
      )
    )->fetchAssoc();

    // Get news cover and news link for welcome message.
    if (!empty($account['news_title']) && empty($account['news_link'])){
      try {
        _moha_wx_get_access_token($account);

        _moha_wx_update_welcome_news($account);
      }
      catch (Exception $e) {
        // Output error log with related failed API.
        watchdog( __FUNCTION__,
          "%msg",
          array(
            "%msg" => $e->getMessage(),
          ),
          WATCHDOG_ERROR
        );
      }
    } // specified news_title.

  }
}