<?php
/**
 * @file
 */
?>
<div class="table-responsive">
  <table class="table table-bordered table-striped">
    <thead>
    <tr>
      <th>操作</th>
      <th>#</th>
      <th>Event Date</th>
      <th>Modality</th>
      <th>User</th>
      <th>Split %</th>
      <th>SSO</th>
      <th>Name</th>
      <th>Status</th>
    </tr>
    </thead>

    <tbody>
    <tr>
      <th scope="row"><?php
        print l('详细',
          format_string(MOHA_SALES__URL__COMMISSION_DETAIL, array('@commission_id' => '1')),
          array('attributes' => array(
            'class' => array('btn', 'btn-info', 'btn-sm'),
          ))
        );
      ?></th>
      <td class="weui-cells_checkbox">
        <label class="weui-check__label" for="r1">
          <div>
            <input type="checkbox" class="weui-check" name="record" id="r1" checked="checked">
            <i class="weui-icon-checked"></i>
          </div>
        </label>
      </td>
      <td>2018Q1</td>
      <td>UltraSound</td>
      <td>北京**公益基金会</td>
      <td>100%</td>
      <td>503xxx752</td>
      <td>Wang, Eric</td>
      <th>在职</th>
    </tr>
    <tr>
      <th scope="row"><?php
        print l('详细',
          format_string(MOHA_SALES__URL__COMMISSION_DETAIL, array('@commission_id' => 2)),
          array('attributes' => array(
            'class' => array('btn', 'btn-info', 'btn-sm'),
          ))
        );
        ?></th>
      <td class="weui-cells_checkbox">
        <label class="weui-check__label" for="r2">
          <div>
            <input type="checkbox" class="weui-check" name="record" id="r2" checked="checked">
            <i class="weui-icon-checked"></i>
          </div>
        </label>
      </td>
      <td>2018Q1</td>
      <td>UltraSound</td>
      <td>北京**公益基金会</td>
      <td>100%</td>
      <td>503xxx752</td>
      <td>Wang, Eric</td>
      <th>在职</th>
    </tr>
    <tr>
      <th scope="row"><button type="button" class="btn btn-info btn-sm">详细</button></th>
      <td class="weui-cells_checkbox">
        <label class="weui-check__label" for="r3">
          <div>
            <input type="checkbox" class="weui-check" name="record" id="r3" checked="checked">
            <i class="weui-icon-checked"></i>
          </div>
        </label>
      </td>
      <td>2018Q1</td>
      <td>UltraSound</td>
      <td>北京**公益基金会</td>
      <td>100%</td>
      <td>503xxx752</td>
      <td>Wang, Eric</td>
      <th>在职</th>
    </tr>
    <tr>
      <th scope="row"><button type="button" class="btn btn-info btn-sm">详细</button></th>
      <td class="weui-cells_checkbox">
        <label class="weui-check__label" for="r4">
          <div>
            <input type="checkbox" class="weui-check" name="record" id="r4">
            <i class="weui-icon-checked"></i>
          </div>
        </label>
      </td>
      <td>2018Q1</td>
      <td>UltraSound</td>
      <td>北京**公益基金会</td>
      <td>100%</td>
      <td>503xxx752</td>
      <td>Wang, Eric</td>
      <th>在职</th>
    </tr>
    <tr>
      <th scope="row"><button type="button" class="btn btn-info btn-sm">详细</button></th>
      <td class="weui-cells_checkbox">
        <label class="weui-check__label" for="r5">
          <div>
            <input type="checkbox" class="weui-check" name="record" id="r5">
            <i class="weui-icon-checked"></i>
          </div>
        </label>
      </td>
      <td>2018Q1</td>
      <td>UltraSound</td>
      <td>北京**公益基金会</td>
      <td>100%</td>
      <td>503xxx752</td>
      <td>Wang, Eric</td>
      <th>在职</th>
    </tr>
    <tr>
      <th scope="row"><button type="button" class="btn btn-info btn-sm">详细</button></th>
      <td class="weui-cells_checkbox">
        <label class="weui-check__label" for="r6">
          <div>
            <input type="checkbox" class="weui-check" name="record" id="r6">
            <i class="weui-icon-checked"></i>
          </div>
        </label>
      </td>
      <td>2018Q1</td>
      <td>UltraSound 6</td>
      <td>北京**公益基金会</td>
      <td>100%</td>
      <td>503xxx752</td>
      <td>Wang, Eric</td>
      <th>在职</th>
    </tr>
    <tr>
      <th scope="row"><button type="button" class="btn btn-info btn-sm">详细</button></th>
      <td class="weui-cells_checkbox">
        <label class="weui-check__label" for="r7">
          <div>
            <input type="checkbox" class="weui-check" name="record" id="r7">
            <i class="weui-icon-checked"></i>
          </div>
        </label>
      </td>
      <td>2018Q1</td>
      <td>UltraSound 7</td>
      <td>北京**公益基金会</td>
      <td>100%</td>
      <td>503xxx752</td>
      <td>Wang, Eric</td>
      <th>在职</th>
    </tr>
    </tbody>
  </table>
</div>

<div class="btn-group btn-group-justified" role="group" aria-label="">
  <div class="btn-group" role="group">
    <button type="button" class="btn btn-info">全选</button>
  </div>
  <div class="btn-group" role="group">
    <button type="button" class="btn btn-default">重置</button>
  </div>
  <div class="btn-group" role="group">
    <button type="button" class="btn btn-primary">批量确认</button>
  </div>
</div>
