/**
 * @file
 *   Provides useful js functions.
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object */
var moha = moha || {};

/**
 * Convert to celsius degree.
 *
 * @param degree
 *
 * @returns {number}
 */
moha.toCelsius = function (degree) {
  const real = (degree - 32) * 5 / 9;
  return Math.round(real);
};

/**
 * Convert to fahrenheit degree.
 *
 * @param degree
 *
 * @returns {number}
 */
moha.toFahrenheit = function (degree) {
  const real = degree * 9 / 5 + 32;
  return Math.round(real);
};

(function ($) {
  Drupal.behaviors.mohaUI = {

    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     */
    attach: function(context, settings) {
      $('body', context).once('moha-ui-body-attach', function () {



        // once body finished.
      });
    }
  };

})(jQuery);
