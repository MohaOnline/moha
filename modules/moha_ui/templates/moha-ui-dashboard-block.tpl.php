<?php
/**
 * @file
 *   Template file.
 *
 * @see moha_ui_theme()
 */
?>
<div class="box box-primary">
  <div class="box-header with-border">
    <?php if (isset($element['#title'])): ?>
    <h3 class="box-title"><?php print $element['#title']; ?></h3>
    <?php endif; ?>

    <div class="box-tools pull-right">
      <button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i></button>
    </div>
  </div>
  <!-- /.box-header -->

  <div class="box-body">
    <div class="row">
      <div <?php print isset($element['#id']) ? 'id="' . $element['#id'] . '"': ''; ?> class="col-md-12 <?php print isset($element['#content_wrapper_selector']) ? $element['#content_wrapper_selector'] : ''; ?>">
        <?php if (isset($element['#data']) && is_string($element['#data'])) { print $element['#data']; } ?>
        <!-- /.chart-responsive -->
      </div>
      <!-- /.col -->
    </div>
    <!-- /.row -->
  </div>
  <?php if (isset($element['#footer']) && !empty($element['#footer'])): ?>
  <div class="box-footer" style="">
    <?php print render($element['#footer']); ?>
    <!-- /.box-footer -->
  </div>
  <?php endif; ?>
</div>
