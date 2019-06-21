<?php

function moha_survey_weui_checkboxex_labels($options, $default_value){
  $output='';
  foreach($options as $key => $option){
    if(in_array($key,$default_value )){
      $default_checked = 'checked="checked"';
    } else {
      $default_checked = '';
    }
    $output .= ' <label class="weui-cell weui-check__label" for="'. $key . '">
      <div class="weui-cell__hd">
          <input type="checkbox" name="checkbox1" class="weui-check" id="' . $key . '" '.$default_checked. '>
          <i class="weui-icon-checked"></i>
      </div>
      <div class="weui-cell__bd">
          <p>' . $option . '</p>
      </div>
  </label>';
  }

  return $output;
}

?>

<div class="weui-cells__title"><?php print $element['#title'];  ?></div>
<div class="weui-cells weui-cells_checkbox">
  <?php print moha_survey_weui_checkboxex_labels($element['#options'], $element['#default_value']); ?>
</div>