<?php
/**
 * @file
 * Template file for Moha Block: Service Conversion.
 *
 * @see _moha_block_service_conversion_contents().
 * @link /sites/all/modules/custom/moha/modules/moha_ui/bower_components/admin-lte/index.html @endlink
 * @link /sites/all/modules/custom/moha/modules/moha_ui/bower_components/admin-lte/pages/UI/general.html @endlink
 */
?>
<div class="panel panel-default panel-square moha-block-service-conversion">
  <div class="panel-heading">
    <?php print t('Toolbox'); ?>
  </div>
  <ul role="tablist" class="nav nav-tabs">
    <li role="presentation" class="active"><a href="#mysql-timestamp-pane" aria-controls="MySQL" role="tab" data-toggle="tab"><?php print t('MySQL'); ?></a></li>
    <li role="presentation"><a href="#password-pane" aria-controls="Password" role="tab" data-toggle="tab"><?php print t('Password'); ?></a></li>
  </ul>
  <div class="tab-content panel-body form-horizontal">
    <div class="tab-pane active" id="mysql-timestamp-pane">
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
      <button id="mysql-timestamp-convert" type="button" class="btn btn-primary"><?php print t('Convert'); ?></button></div>

    <div class="tab-pane" id="password-pane">
      <div class="form-group">
        <label for="generated-password" class="col-sm-4 control-label"><?php print t('Length'); ?></label>
        <div class="col-sm-8">
          <input id="generated-password" type="text" class="form-control col-sm-10" placeholder="<?php print t('16'); ?>">
        </div>
      </div>
      <div class="form-group">
        <label class="col-sm-4 control-label"><?php print t('Password'); ?></label>
        <div class="col-sm-8">
          <p id="password-placeholder" class="form-control-static"></p>
        </div>
      </div>
      <button id="password-generate" type="button" class="btn btn-primary"><?php print t('Generate'); ?></button></div>
  </div>
</div>