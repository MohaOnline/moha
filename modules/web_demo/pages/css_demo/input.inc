<?php
/**
 * @file
 * @Created: 01/12/2017 5:37 AM
 */

function __web_demo_css_input() {

  $content['#theme_wrappers'] = array('web_demo_css_input');
  $content['#attached'] = array(
    'css' => array(
      drupal_get_path('module', WEB_DEMO__MACHINE_NAME) . '/css/input.css'
    )
  );

  return $content;
}
