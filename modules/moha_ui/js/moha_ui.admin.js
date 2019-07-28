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
      $('body', context).once('moha-ui-admin-body-attach', function () {



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
