<?php
/**
 * @file
 * @Created: 01/12/2017 5:37 AM
 */

/**
 * @return mixed render array
 */
function __moha_demo_web_flex() {

  /** @see flex.tpl.php */
  /** @see moha_demo_theme() */
  $content['#theme'] = array('moha_demo_web_flex');
  $content['#attached'] = array(
    'css' => array(
      MOHA_DEMO__PATH. '/controllers/demo_web/css/moha_demo.flex.css'
    )
  );

  return $content;
}
