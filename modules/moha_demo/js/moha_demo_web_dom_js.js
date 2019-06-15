/**
 * @file
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object */

(function ($) {
  Drupal.behaviors.mohaDemoWebDomJS = {

    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     */
    attach: function(context, settings) {
      $('body', context).once('moha-demo-web-dom-js-attach', function () {
        // Inspect Drupal object shipped with Drupal.
        console.info('Inspect keys of Drupal object:');
        for (let key in Drupal) {
          console.info(key);
        }

        //

        // once mohaDemoWebDomJSAttach finished.
      });
    }
  };

})(jQuery);
