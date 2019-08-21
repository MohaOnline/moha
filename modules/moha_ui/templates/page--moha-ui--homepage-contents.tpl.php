<?php
/**
 * @file
 */

?>
<div class="moha-homepage">
  <div class="moha-homepage-main col-sm-7">
    <?php echo $element['moha_clip_list'];?>
  </div>
  <div class="moha-homepage-sidebar col-sm-5">
    <?php foreach ($element['blocks'] as $block): ?>
      <?php echo $block; ?>
    <?php endforeach; ?>
  </div>
</div>
