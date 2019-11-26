/**
 * @file
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object, _:object */

(function ($) {

  /* globals Drupal:object */

  Drupal.behaviors.moha_clip_admin = {
    load_preview: function(){
      /** @see moha_clip_form_moha_clip_node_form_alter() */
      jQuery('iframe#moha_node_preview').attr('src', Drupal.settings.moha_clip.preview_url);
    },

    load_anonymous_preview: function() {
      var tmp_cookie = document.cookie;
      document.cookie = '';
      jQuery.get(Drupal.settings.moha_clip.preview_url).success(function (data) {
        document.getElementById('moha_node_preview').srcdoc = data;
        // var context = jQuery('iframe#moha_node_preview')[0].contentWindow.document;
        // var body = jQuery('body', context);
        // body.html(data);
      });
      document.cookie = tmp_cookie;
    },

    attach: function (context, settings) {
      // Hide success message automatically.
      $(".messages.status", context).once('success-message-fade-out', function(){
        $(this).delay(6000).fadeTo(1500, 0.05).slideUp(500);
      });

      // Add collapsible effect for image and video field.
      // Need: drupal_add_library('system', 'drupal.collapse'); to active collapsible effect.
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
          const headerHeight        = document.getElementById('branding').offsetHeight;
          const content             = document.getElementsByClassName('cke_contents').item(1);
          const toolbar             = document.getElementsByClassName('cke_top').item(1);
          const toolbarAfterAdminY  = '29px';
          const editor              = document.getElementsByClassName('cke').item(1);
          const inner               = document.getElementsByClassName('cke_inner').item(1);
          // Current scrolled distance.
          const currentScrollY      = document.documentElement.scrollTop > document.body.scrollTop ? document.documentElement.scrollTop : document.body.scrollTop;

          if (toolbar == null) { return; }

          toolbar.style.width     = content.offsetWidth + "px";
          toolbar.style.top       = "0px";
          toolbar.style.margin    = "0 auto";
          toolbar.style.boxSizing = "border-box";

          // Scroll over editor, so fix toolbar after admin menu.
          if(currentScrollY > headerHeight + editor.offsetTop){
            toolbar.style.top        = toolbarAfterAdminY;
            toolbar.style.position   = "fixed";
            content.style.paddingTop = toolbar.offsetHeight + "px";
          }

          if(currentScrollY <= headerHeight + editor.offsetTop ){
            toolbar.style.position   = "relative";
            content.style.paddingTop = "0px";
          }

          if((editor.offsetTop + editor.offsetHeight) < (currentScrollY + toolbar.offsetHeight)){
            toolbar.style.position = "absolute";
            toolbar.style.top      = "calc(100% - " + toolbar.offsetHeight + "px)";
            inner.style.position   = "relative";
            content.style.paddingTop = "0px";
          }
        }, false);
      });

      // Adjust iFrame preview window, if width of window is too narrow, hide preview window.
      var windowWidth = jQuery(window).width();
      if (windowWidth > 1800) {
        jQuery('div#moha_node_preview_wrapper').css('display', 'block').insertBefore('div#edit-body');
      }
      else {
        jQuery('div#moha_node_preview_wrapper').css('display', 'none');
      }

      $('div#moha_node_preview_wrapper', context).once('fix-preview-wrapper-height', function () {
        $('iframe#moha_node_preview').load(function () {
          /* this.contentWindow is iframe window. */
          $('div#moha_node_preview_wrapper').height(this.contentWindow.document.body.offsetHeight + 50);
        });
      });

    }
  };
})(jQuery);
