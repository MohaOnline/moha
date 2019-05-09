<?php
/**
 * @file
 * @Created: 01/12/2017 5:37 AM
 */

function __moha_demo_web_flex() {

  $content['#theme_wrappers'] = array('moha_demo_web_flex');
  $content['#attached'] = array(
    'css' => array(
      MOHA_DEMO__PATH. '/controllers/demo_web/css/flex.css'
    )
  );

  return $content;
}
