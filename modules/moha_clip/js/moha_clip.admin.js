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

        // Fix CKEditor toolbar.
        window.addEventListener('scroll', function(){
          const content             = document.getElementsByClassName('cke_contents').item(1);
          const toolbar             = document.getElementsByClassName('cke_top').item(1);
          const editor              = document.getElementsByClassName('cke').item(1);
          const inner               = document.getElementsByClassName('cke_inner').item(1);
          const scrollvalue         = document.documentElement.scrollTop > document.body.scrollTop ? document.documentElement.scrollTop : document.body.scrollTop;

          toolbar.style.width     = content.offsetWidth + "px";
          toolbar.style.top       = "0px";
          toolbar.style.margin    = "0 auto";
          toolbar.style.boxSizing = "border-box";

          if(toolbar.offsetTop <= scrollvalue){
            toolbar.style.top       = "29px";
            toolbar.style.position   = "fixed";
            content.style.paddingTop = toolbar.offsetHeight + "px";
          }

          if(editor.offsetTop > scrollvalue && (editor.offsetTop + editor.offsetHeight) >= (scrollvalue + toolbar.offsetHeight)){
            toolbar.style.position   = "relative";
            content.style.paddingTop = "0px";
          }

          if((editor.offsetTop + editor.offsetHeight) < (scrollvalue + toolbar.offsetHeight)){
            toolbar.style.position = "absolute";
            toolbar.style.top      = "calc(100% - " + toolbar.offsetHeight + "px)";
            inner.style.position   = "relative";
            content.style.paddingTop = "0px";
          }
        }, false);
      });

    }
  };
})(jQuery);