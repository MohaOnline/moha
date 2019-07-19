<?php
/**
 * @file
 * Token callbacks for the module.
 */

/**
 * Implements hook_token_info().
 */
function moha_token_info() {
  $info['types']['future'] = array(
    'name' => t('Future'),
    'description' => t('Tokens to show future date.'),
  );
  $info['tokens']['future']['php'] = array(
    'name' => t('PHP Expression'),
    'description' => t('Calculate date per PHP Expression, refer to Example section of !php-strtotime, eg: [future:+7 day:Y年m月j日].',
      array('!php-strtotime' => l(t('the PHP function strtotime documentation'), 'http://php.net/manual/en/function.strtotime.php'))),
  );
  $info['tokens']['future']['php']['dynamic'] = TRUE;

  return $info;
}

/**
 * Implements hook_tokens().
 *
 * @inheritDoc
 */
function moha_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if($type == 'future') {
    $date = array();
    // Calculate future date then pass to format.
    foreach ($tokens as $name => $original) {
      $details = explode(':', $name, 2);
      $date['custom:' . $details[1]] = $original;
      $replacements += token_generate('date', $date, array('date' => strtotime($details[0])), $options);
    }
  }
  return $replacements;
}
