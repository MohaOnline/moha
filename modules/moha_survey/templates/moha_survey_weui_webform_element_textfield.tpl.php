
<div class="weui-cells__title"><?php print $element['#title'] ?></div>

<div class="weui-cells">
  <div class="weui-cell">
    <div class="weui-cell__bd">
      <input class="weui-input" type="text" placeholder="请输入文本"
        id="<?php print $element['#id'] ?>" name="<?php print $element['#name'] ?>"
        value="<?php print $element['#value'] ?>" size="<?php print $element['#size'] ?>"
        maxlength="<?php  print $element['#maxlength'] ?>"
      >
    </div>
  </div>
</div>