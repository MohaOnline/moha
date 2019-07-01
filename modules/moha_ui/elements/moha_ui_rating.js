/**
 * @file
 *   Rating rendering JS.
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object, weui:object */

(function ($) {

  Drupal.behaviors.mohaUIRatingElement = {
    attach: function(context, settings) {

      /* Bind gallery preview feature for each image field used moha_ui_image_mobile_uploader widget. */
      for (let i=0; i<settings.moha_ui.mohaUIRatingElements.length; i++) {
        const element = settings.moha_ui.mohaUIRatingElements[i];

        $('.form-item-' + element.wrapper + '.form-type-moha-ui-rating', context).once('moha-ui-rating-element', function () {
          const ratingOptions = {
            selectors: {
              starsSelector: '.' + element.wrapper + '.rating-stars',
              starSelector: '.' + element.wrapper + '.rating-stars .rating-star',
              starActiveClass: 'is--active',
              starHoverClass: 'is--hover',
              starNoHoverClass: 'is--no-hover',
              targetFormElementSelector: '.' + element.wrapper + ' input[type="hidden"]'
            }
          };

          const mohaUIRating = $('.' + element.wrapper + '.rating-stars', context).ratingStars(ratingOptions);

          // Rating star initial status.
          if (element.thresholdScore > 0 && element.score <= element.thresholdScore) {
            $('.form-item-' + element.wrapper + ' .form-type-textfield', context).css('display', 'inherit');
          }

          if (element.thresholdScore > 0) {
            mohaUIRating.on('ratingChanged', function (ev, data) {
              if (data.ratingValue <= element.thresholdScore) {
                $('.form-item-' + element.wrapper + ' .form-type-textfield', context).css('display', 'inherit');
              }
              else {
                $('.form-item-' + element.wrapper + ' .form-type-textfield', context).css('display', 'none');
              }
            });
          }

        }); // moha_ui_rating_element once

      } // Widget wrappers for loop end.

    } // function attach.
  };

})(jQuery);
