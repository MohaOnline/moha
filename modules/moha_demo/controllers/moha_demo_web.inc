<?php
/**
 * @file
 */

/**
 * @return mixed render array
 */
function __moha_demo_domjs() {

  $content['#theme'] = array('moha_demo_web_dom_js'); /** @see dom.js.tpl.php */

  $content['#attached'] = array(
    'css' => array(
      MOHA_DEMO__PATH. '/css/moha_demo_web_dom_js.css'
    ),
    'js' => array(
    MOHA_DEMO__PATH. '/js/moha_demo_web_dom_js.js'
    ),
  );

  if (defined('__MOHA_UI')) {
    $content['#attached']['library'][] = array(__MOHA_UI, __MOHA_UI);
  }

  return $content;
}
