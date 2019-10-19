/**
 * @file
 *   Provides useful js functions.
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object */
var moha = moha || {};

(function ($) {
  Drupal.behaviors.mohaUIAdmin = {

    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     */
    attach: function(context, settings) {
      // move required mark.
      $('body table.sticky-table', context).once('moha-ui-admin-sticky-table-attach', function () {
        $(this).find('span.form-required').each(function () {
          $(this).closest('tr').find('td:first-child').append($(this));
        });
        // once body finished.
      });
    }
  };

  Drupal.behaviors.mohaUIAdminDashboard = {
    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     */
    attach: function(context, settings) {
      $('body.page-admin-moha-dashboard', context).once('moha-ui-admin-dashboard-body-attach', function () {


        // once body finished.
      });
    }
  };

  Drupal.behaviors.mohaUIAdminDateRange = {
    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     * @link https://www.daterangepicker.com @endlink
     */
    attach: function(context, settings) {
      $('.form-type-moha-ui-date-range button', context).once('form-type-moha-ui-date-range', function () {
        const $widget = $(this);
        _.forEach(settings.moha_ui.moha_ui_date_range, function(config){
          if (config.id === $widget.prop('id')){
            $widget.daterangepicker(
              {
                "startDate": config.start,
                "endDate": config.end,
                "locale": {
                  "applyLabel": "确定",
                  "cancelLabel": "放弃",
                  "format": "YYYY/MM/DD",
                  "daysOfWeek": [
                    "日",
                    "一",
                    "二",
                    "三",
                    "四",
                    "五",
                    "六"
                  ],
                  "monthNames": [
                    "一月 ",
                    "二月 ",
                    "三月 ",
                    "四月 ",
                    "五月 ",
                    "六月 ",
                    "七月 ",
                    "八月 ",
                    "九月 ",
                    "十月 ",
                    "十一月 ",
                    "十二月 "
                  ]
                }
              },
              function(start, end, label) {
                const range = start.format('YYYY/MM/DD') + ' - ' + end.format('YYYY/MM/DD');
                console.log(range);
                $widget.find('span').html('<i class="fa fa-calendar"></i> ' + range);
                $widget.next().val(range);
                $widget.closest('form').submit();
              });
          }
        });
        
      })
      // once body finished.
      
    }
  };

})(jQuery);
