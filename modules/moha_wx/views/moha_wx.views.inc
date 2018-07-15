<?php

/**
 * Implements hook_views_data().
 *
 * Exposes table moha_wx_accounts to views.
 */
function moha_wx_views_data() {
  $data = array();

  $data['moha_wx_accounts']['table']['base'] = array (
    'title' => t('Moha WeChat accounts'),
    'help' => '',
  );
  $data['moha_wx_accounts']['table']['group'] = t('Moha WeChat account');

  return $data;
}