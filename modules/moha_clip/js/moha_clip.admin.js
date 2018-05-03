(function ($) {

  Drupal.behaviors.moha_clip_admin = {
    attach: function (context, settings) {
      // Hide success message automatically.
      $(".messages.status", context).once('success-message-fade-out', function(){
        $(this).delay(5000).fadeTo(1500, 0.05).slideUp(500);
      });

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

      $('#edit-silent-save', context).once('edit-silent-save', function () {
        document.addEventListener("keydown", function(e) {
          // Overwrite Cmd + S on Mac or Ctrl + S on Win.
          if ((window.navigator.platform.match("Mac") ? e.metaKey : e.ctrlKey)  && e.keyCode == 83) {
            e.preventDefault();
            // Save node silently.
            $('#edit-silent-save').trigger('mousedown');

          }
        }, false);
      });

    }
  };
})(jQuery);