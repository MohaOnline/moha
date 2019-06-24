<?php
foreach ($surveys as $key => $survey): ?>
  <?php
    //$preview_path = url('node/'.$survey->nid);
    $edit_path = url('admin/moha/survey/edit/'.$survey->nid);
  ?>
  <div>
    <span><a href="<?php print $edit_path ?>" ><?php print $survey->survey_name; ?></a></span>
  </div>
<?php endforeach; ?>
