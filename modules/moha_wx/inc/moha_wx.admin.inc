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
 * @param string $wechat_name
 *    passed from URL.
 *
 * @return array $form
 */
function _moha_wx_account_form($form, &$form_state, $wechat_name = ''){

  $account = (object) array(
    'account_name' => '',   // Account human name.
    'wechat_name' => $wechat_name, // machine name.
    'is_enterprise' => 0,
    'agent_id' => '',
    'app_id' => '',
    'app_secret' => '',
    'token' => '',
    'encoding_aes_key' => '',
    'text' => '',
    'news_title' => '',
    'news_cover' => '',
    'news_link' => '',
  );

  $submit_op = MOHA__FORM_OP__SAVE;
  $wechat_name = check_plain($wechat_name);

  // Load account information per passed $account_name.
  if (is_string($wechat_name)) {
    /** @var \PDOStatement $query */
    $query = db_query("SELECT * FROM {moha_wx_accounts} moha_wx_accounts WHERE wechat_name = :wechat_name",
      array(
        ':wechat_name' => $wechat_name,
      )
    );

    if ($record = $query->fetchObject()) {
      $account = $record;
      $submit_op = MOHA__FORM_OP__UPDATE;
    }
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

  $form['is_enterprise'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enterprise Account'),
    '#return_value' => 1,
    '#default_value' => $account->is_enterprise,
    '#description' => t("Indicates if current account is an enterprise account."),
  );

  $form['agent_id'] = array(
    '#type' => 'textfield',
    '#title' => t('AgentID'),
    '#default_value' => $account->agent_id,
    '#description' => t("Accompany with the enterprise account."),
    '#states' => array(
      'required' => array(
        ':input[name="is_enterprise"]' => array('checked' => TRUE),
      ),
    ),
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
    '#value' => t($submit_op),
  );

  return $form;
}

/**
 * Save or update WeChat Account information to moha_wx_accounts.
 * Welcome message related process.
 *
 * @param $form
 * @param $form_state
 */
function _moha_wx_account_form_submit($form, &$form_state){

  // Extracts account information from submitted data.
  $account = $form_state['values'];

  // Extracts submission type: Save or Update.
  $op = $form_state['values']['op'];
  $op_result = '';

  try {
    global $user;

    if ($op == t(MOHA__FORM_OP__SAVE)) {

      $result_num = db_insert('moha_wx_accounts')
        ->fields(array(
          'uid' => $user->uid,
          'account_name' => $account['account_name'],
          'wechat_name' => $account['wechat_name'],
          'app_id' => $account['app_id'],
          'app_secret' => $account['app_secret'],
          'token' => $account['token'],
          'encoding_aes_key' => $account['encoding_aes_key'],
          'access_token' => '',
          'news_title' => $account['news_title'],
          'news_cover' => $account['news_cover'],
          'news_link' => $account['news_link'],
          'text' => $account['text']['value'],
          'is_enterprise' => $account['is_enterprise'],
          'agent_id' => $account['agent_id'],
          'updated' => REQUEST_TIME,
          'created' => REQUEST_TIME,
        ))
        ->execute();

      $op_result = t("This account has been saved.");
    }
    elseif ($op == t(MOHA__FORM_OP__UPDATE)) {
      $result_num = db_update('moha_wx_accounts')
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
          'is_enterprise' => $account['is_enterprise'],
          'agent_id' => $account['agent_id'],
          'updated' => REQUEST_TIME,
        ))
        ->condition('wechat_name', $account['wechat_name'])
        ->execute();

      $op_result = t("This account has been updated.");
    }

    // Insert ID or affected number exists.
    if (isset($result_num)) {
      // Provides feedback of submission.
      drupal_set_message($op_result);

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
    moha_show_exception($e);
  }
}

/**
 * Form for confirming WeChat menu before to be deployed or to be removed.
 *
 * @see admin/moha/wx/menu/$wechat_name.
 *
 * @param $form
 * @param $form_state
 * @param string $wechat_name
 *
 * @return array $form
 */
function _moha_wx_menu_form($form, &$form_state, $wechat_name = ''){

   try {
    $wechat_name = check_plain($wechat_name);
    $account = _moha_wx_moha_account($wechat_name);

    if (empty($account['wx_menu'])) {
      _moha_wx_get_menu($account);
    }

    $form['#attached']['css'] = array(
      MOHA__RELATIVE_PATH . '/css/moha.admin.css',
      MOHA_WX__RELATIVE_PATH . '/css/moha_wx.admin.css',
    );

    $form['wechat_name'] = array(
      '#type' => 'hidden',
      '#value' => $account['wechat_name'],
    );

    $form['helps'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="moha-admin-hints">
      <p>Click "Save Menu" button to save below menu configurations to DB and upload same to WeChat account.</p>
      <p>Click "Delete Menu" button to clean configuration in DB and remove customized menu from WeChat account.</p>
  </div>',
    );

    $form['wx_menu'] = array(
      '#type' => 'textarea',
      '#rows' => 20,
      '#title' => t('WeChat Menu'),
      '#default_value' => $account['wx_menu'],
      '#description' => t("Put JSON configurations of WeChat menu here."),
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
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
    moha_show_exception($e);
  }

}

/**
 * Save or delete WeChat menu.
 *
 * @inheritdoc
 */
function _moha_wx_menu_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  if ($values['op'] == t('Delete Menu')) {
    $values['wx_menu'] = '';
  }

  try {
    $affected_row_number = db_update('moha_wx_accounts')
      ->fields(array(
        'wx_menu' => $values['wx_menu'],
        'updated' => REQUEST_TIME,
      ))
      ->condition('wechat_name', $values['wechat_name'])
      ->execute();

    $account = _moha_wx_moha_account($values['wechat_name']);

    if ($values['op'] == 'Delete Menu') {
      _moha_wx_delete_menu($account);
    }
    else {
      _moha_wx_create_menu($account);
    }

  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
    moha_show_exception($e);
  }
}
