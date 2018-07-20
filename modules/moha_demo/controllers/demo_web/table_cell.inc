<?php
/**
 * @file
 */


function __moha_demo_web() {
  $content['#theme_wrappers'] = array('moha_demo_web_table_cell');
  $content['#attached'] = array(
    'css' => array(
      MOHA_DEMO__RELATIVE_PATH. '/controllers/demo_web/css/table_cell.css'
    )
  );

  return $content;
}