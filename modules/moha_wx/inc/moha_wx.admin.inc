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
 * URL: admin/moha/wx/config/$account_name.
 *
 * @param array $form
 * @param array $form_state
 * @param string $account_name
 *    passed from URL.
 *
 * @return array $form
 */
function _moha_wx_account_form($form, &$form_state, $account_name = ''){

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
    '#description' => t('Give a name to identify this WeChat Service Account.'),
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
    '#title' => t('Welcome message: title'),
    '#description' => t("Specify the title of the news which will be used as welcome message."),
    '#default_value' => $account->news_title,
  );

  $form['news_cover'] = array(
    '#type' => 'textfield',
    '#title' => t('Welcome message: URL of cover image'),
    '#maxlength' => 256,
    '#size' => 160,
    '#default_value' => $account->news_cover,
  );

  $form['news_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Welcome message: URL of article itself'),
    '#description' => t("Leave blank to fetch news URL from WeChat backend"),
    '#maxlength' => 256,
    '#size' => 160,
    '#default_value' => $account->news_link,
  );

  $form['text'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#title' => t('Contact us text'),
    '#default_value' => $account->text,
    '#description' => t("Put text contact us message here."),
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
 * Save WeChat Account information.
 * Welcome message related process.
 *
 * @param $form
 * @param $form_state
 */
function _moha_wx_account_form_submit($form, &$form_state){

  $account = $form_state['values'];

  try {
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
      $account = _moha_wx_moha_account($account['wechat_name']);

      // Get news cover and news link for welcome message.
      if (!empty($account['news_title']) && (empty($account['news_link']) || empty($account['news_cover']))) {
          _moha_wx_refresh_welcome_news_info($account);
      } // news_title is specified.
    }

  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
  }
}

/**
 * URL: admin/moha/wx/menu/$wechat_name.
 *
 * @param $form
 * @param $form_state
 * @param string $wechat_name
 *
 * @return array
 */
function _moha_wx_menu_form($form, &$form_state, $wechat_name = ''){

  drupal_add_css(MOHA_WX__RELATIVE_PATH . '/css/moha_wx.admin.css');

  $account = (object) array(
    'wechat_name' => '',
    'wx_menu' => '',
  );

  $account = db_query("SELECT * FROM {moha_wx_accounts} moha_wx_accounts WHERE wechat_name = :wechat_name",
    array(
      ':wechat_name' => $wechat_name,
    )
  )->fetchAssoc();

  if (empty($account['wx_menu'])) {
    try {
      _moha_wx_get_menu($account);
    } catch (Exception $e) {
      // Output error log with related failed API.
      watchdog_exception(__FUNCTION__, $e);
    }
  }

  $form['wechat_name'] = array(
    '#type' => 'hidden',
    '#value' => $account['wechat_name'],
  );

  $form['helps'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="moha-admin-hints">
    <p>Click "Save Menu" button to save below menu configurations to DB and upload same to WeChat account.</p>
    <p>Click "Delete Menu" button to remove customized menu from WeChat account.</p>
</div>',
  );

  $form['wx_menu'] = array(
    '#type' => 'textarea',
    '#rows' => 20,
    '#title' => t('WeChat Menu'),
    '#default_value' => $account['wx_menu'],
    '#description' => t("Put JSON configuration of WeChat menu here. URL need be manually proceeded, copy and paste \\ before every /."),
    '#format' => 'plain_text',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Menu'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Menu'),
  );

  return $form;

}

/**
 * Save or delete WeChat menu.
 *
 * @inheritdoc
 */
function _moha_wx_menu_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  if ($values['op'] == 'Delete Menu') {
    $values['wx_menu'] = '';
  }

  $affacted_row_number = db_update('moha_wx_accounts')
    ->fields(array(
      'wx_menu' => $values['wx_menu'],
      'updated' => time(),
    ))
    ->condition('wechat_name', $values['wechat_name'])
    ->execute();

  $account = db_query("SELECT * FROM {moha_wx_accounts} moha_wx_accounts WHERE wechat_name = :wechat_name",
    array(
      ':wechat_name' => $values['wechat_name'],
    )
  )->fetchAssoc();

  try {
    if ($values['op'] == 'Delete Menu') {
      _moha_wx_delete_menu($account);
    }
    else {
      _moha_wx_create_menu($account);
    }

  } catch (Exception $e) {
    // Output error log with related failed API.
    watchdog(__FUNCTION__,
      "%msg",
      array(
        "%msg" => $e->getMessage(),
      ),
      WATCHDOG_ERROR
    );
  }
}
