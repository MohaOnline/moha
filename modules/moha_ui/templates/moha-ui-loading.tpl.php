<?php
/**
 * @file
 *   Template file.
 *
 * @see moha_ui_theme()
 * @see moha_ui_loading()
 *
 * @var $variables array
 *   Passed from set render array.
 */

$element = $variables['element'];
?>

<div id="<?php echo $element['#id']; ?>" class="moha-ui-loading">
  <div class="moha-ui-loading-icon">
    <img src="<?php echo moha_url(MOHA_UI__PATH . '/img/loading-single-rotating-blue-light.gif', TRUE); ?>" alt="Loading..." />
  </div>
  <p id="<?php echo $element['#id']; ?>-text" class="moha-ui-loading-text"><?php echo $element['#text_loading'] ?></p>
</div>

