<?php
foreach ($surveys as $key => $survey): ?>
  <?php
    //$preview_path = url('node/'.$survey->nid);
    $edit_path = url('admin/moha/survey/edit/'.$survey->nid);
  ?>
  <div>
    <span><a href="<?php echo $edit_path ?>" ><?php echo $survey->survey_name; ?></a></span>
  </div>
<?php endforeach; ?>
