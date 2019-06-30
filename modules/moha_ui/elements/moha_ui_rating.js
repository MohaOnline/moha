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

        $('.form-item-' + element.wrapper + '.form-type-moha-ui-rating', context).once('moha_ui_rating_element', function () {
          const ratingOptions = {
            selectors: {
              starsSelector: '.' + element.wrapper + '.rating-stars',
              starSelector: '.' + element.wrapper + '.rating-stars .rating-star',
              starActiveClass: 'is--active',
              starHoverClass: 'is--hover',
              starNoHoverClass: 'is--no-hover',
              targetFormElementSelector: '.form-item-' + element.wrapper + ' input[type="hidden"]'
            }
          };

          $('.' + element.wrapper + '.rating-stars').ratingStars(ratingOptions);


        }); // moha_ui_rating_element once

      } // Widget wrappers for loop end.

    } // function attach.
  };

})(jQuery);
