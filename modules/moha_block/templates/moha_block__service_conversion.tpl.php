<?php
/**
 * @file
 * Template file for Moha Block: Service Conversion.
 *
 * @see _moha_block_service_conversion_contents().
 * @link /sites/all/modules/custom/moha/modules/moha_ui/bower_components/admin-lte/index.html @endlink
 */
?>
<div class="panel panel-default panel-square moha-block-service-conversion">
  <div class="panel-heading">
    <?php print t('Unit Conversion'); ?>
  </div>
  <ul class="nav nav-tabs" role="tablist">
    <li role="presentation" class="active"><a href="#home" aria-controls="MySQL" role="tab" data-toggle="tab"><?php print t('MySQL'); ?></a></li>
  </ul>
  <div class="panel-body form-horizontal">
    <!-- Nav tabs -->

    <div class="form-group">
      <label for="mysql-timestamp" class="col-sm-4 control-label"><?php print t('Timestamp'); ?></label>
      <div class="col-sm-8">
        <input id="mysql-timestamp" type="text" class="form-control col-sm-10" placeholder="<?php print t('MySQL Timestamp'); ?>">
      </div>
    </div>
    <div class="form-group">
      <label class="col-sm-4 control-label"><?php print t('Date'); ?></label>
      <div class="col-sm-8">
        <p id="mysql-human-date" class="form-control-static"></p>
      </div>
    </div>
    <button id="mysql-timestamp-convert" type="button" class="btn btn-primary"><?php print t('Convert'); ?></button>
  </div>
</div>