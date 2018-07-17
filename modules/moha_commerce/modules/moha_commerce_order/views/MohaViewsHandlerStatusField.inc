<?php
/**
 * @file
 */

/**
 * Class MohaViewsHandlerStatusField
 *
 *
 */
class MohaViewsHandlerStatusField extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function option_definition() {
    return parent::option_definition();
  }

  function query() {
    parent::query();
    $this->add_additional_fields();
  }

  function pre_render(&$values) {
    parent::pre_render($values);


  }

}