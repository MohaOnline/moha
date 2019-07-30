/**
 * @file
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object, _:object */

(function ($) {
  Drupal.behaviors.mohaBlock = {

    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     */
    attach: function(context, settings) {
      $('.moha-block-service-conversion', context).once('moha-block-service-conversion', function () {
        const serviceConversionBlock = this;
        $('#mysql-timestamp-convert', serviceConversionBlock).on('click', function (e) {
          const timestamp = $('#mysql-timestamp', serviceConversionBlock).val();
          if (_.toNumber(timestamp)) {
            const d = new Date(timestamp * 1000);
            $('#mysql-human-date', serviceConversionBlock).html(d.toLocaleString('zh-CN'));
          }
        });

        // moha-block-service-conversion once finished.
      });
    }
  };

})(jQuery);