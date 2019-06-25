/**
 * @file
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object */

(function ($) {
  Drupal.behaviors.mohaUIRating = {

    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     */
    attach: function(context, settings) {
      $('body', context).once('moha_ui_rating', function () {
        $('#edit-nps', context).change(function () {
          if ($(this).is(':checked')){
            const $extra_options = $(this).parent().parent();
            $extra_options.find('#edit-max-score').val(10);
            $extra_options.find('#edit-threshold-score').val(6);
          }
        });

        // once finished.
      });
    }
  };

})(jQuery);