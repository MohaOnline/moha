<?php
/**
 * @file
 * @Created: 01/12/2017 5:37 AM
 */

function __moha_demo_web_input() {

  $content['#theme_wrappers'] = array('moha_demo_web_input');
  $content['#attached'] = array(
    'css' => array(
      MOHA_DEMO__RELATIVE_PATH. '/pages/demo_web/css/input.css'
    )
  );

  return $content;
}
