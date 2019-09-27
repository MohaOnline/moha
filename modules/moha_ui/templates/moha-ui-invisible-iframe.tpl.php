<?php
/**
 * @file
 *   Template file.
 *
 * @see moha_ui_theme()
 *
 * @var $variables array
 *   Passed from set render array.
 */

$element = $variables['element'];
$urls = $element['#urls'];
foreach ($urls as $url):
?>
<iframe src="<?php echo $url?>" class="moha-ui-invisible-iframe"></iframe>
<?php endforeach; ?>

