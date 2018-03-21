(function ($) {

  Drupal.behaviors.moha_clip_admin = {
    attach: function (context, settings) {
      // Add collapsible effect for image and video field.
      $('form', context).once('collapsed', function () {
        var $fieldset = $(this).find('.field-type-image fieldset, .field-type-video fieldset');

        $fieldset.addClass('collapsible');
        $fieldset.addClass('collapsed');
      });

      $('.field-type-image fieldset, .field-type-video fieldset', context).once('collapsible', function () {
        var $fieldset = $(this);

        $fieldset.addClass('collapsible');
      });

    }
  };
})(jQuery);
