<?php
/**
 * @file
 *   Backend page for different form elements.
 */

function __moha_demo_web_input() {

  $content['#theme_wrappers'] = array('moha_demo_web_input');
  $content['#attached'] = array(
    'css' => array(
      MOHA_DEMO__PATH. '/controllers/demo_web/css/input.css',
      MOHA_DEMO__PATH. '/controllers/demo_web/css/input-login.css',
    )
  );

  return $content;
}