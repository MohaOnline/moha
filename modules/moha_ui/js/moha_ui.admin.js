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

})(jQuery);
