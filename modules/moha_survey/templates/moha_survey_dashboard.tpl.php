<?php
/**
 * @var $content array
 */
?>

<div class="row">
  <div class="col-md-12 col-lg-7 moha-survey moha-survey-surveys">
    <div class="box box-primary box-solid">
      <div class="box-header">
        <h3 class="box-title">Surveys</h3>
      </div>
      <!-- /.box-header -->
      <div class="box-body no-padding">

      </div>
      <!-- /.box-body -->
    </div>
    <!-- /.box -->
  </div>

  <div class="col-md-12 col-lg-5 moha-survey moha-survey-templates">
    <div class="box box-success box-solid">
      <div class="box-header">
        <h3 class="box-title">Templates</h3>
      </div>
      <!-- /.box-header -->
      <div class="box-body no-padding"><?php print $content['templates'];?></div>
      <!-- /.box-body -->
    </div>
    <!-- /.box -->
  </div>
</div>
