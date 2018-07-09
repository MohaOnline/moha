<?php

/**
 * Implements hook_views_data().
 *
 * Exposes table moha_wx_accounts to views.
 */
function moha_wx_views_data() {
  $data = array();

  $data['moha_wx_accounts'] = array(
    'table' => array(
      'base' => array (
        'title' => 'Moha WeChat accounts',
        'help' => '',
      )
    ),
  );

  return $data;
}