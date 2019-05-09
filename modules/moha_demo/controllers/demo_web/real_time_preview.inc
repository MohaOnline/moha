<?php
/**
 * @file
 */


function __moha_demo_web_real_time_preview() {
  $content['preview_url'] = 'http://dev.moha.online/cn/';
  $content['#theme'] = 'moha_demo_web_real_time_preview';
  $content['#attached'] = array(
    'css' => array(
      MOHA_DEMO__PATH. '/controllers/demo_web/css/real_time_preview.css',
    ),
    'js' => array(
      MOHA_DEMO__PATH. '/controllers/demo_web/js/real_time_preview.js',
    ),
  );

  return $content;
}
