/**
 * @file
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object, weui:object */

(function ($) {

  Drupal.behaviors.mohaUIImageMobileUploader = {
    attach: function(context, settings) {

      /* Bind gallery preview feature for each image field used moha_ui_image_mobile_uploader widget. */
      for (let i=0; i<settings.moha_ui.mobileImageWidgetWrapperSelector.length; i++) {
        const wrapperSelector = settings.moha_ui.mobileImageWidgetWrapperSelector[i];

        $(wrapperSelector + ' .moha_ui_image_mobile_preview.weui-uploader__files', context).once('moha_ui_image_mobile_preview', function () {

          this.addEventListener('click', function (e) {
            var target = e.target;

            while (target && target.classList && !target.classList.contains('weui-uploader__file') ) {
              target = target.parentNode;
            }
            if (!target || !target.classList) {
              return;
            }

            const url = target.getAttribute('data-img') || '';
            const id = target.getAttribute('data-id');

            const gallery = weui.gallery(url, {
              onDelete: function () {
                weui.confirm(Drupal.t('Cancel this image?'), function () {
                  const event = document.createEvent('HTMLEvents');
                  Drupal.ajax[id].eventResponse(event);
                  target.remove();
                  gallery.hide();
                });
              }
            });

          });

        }); // once

        $(wrapperSelector + ' .moha_ui_image_mobile_uploader', context).once('moha_ui_image_mobile_uploader', function () {

          let uploadCustomFileList = [];

          weui.uploader(wrapperSelector + ' .moha_ui_image_mobile_uploader', {
            auto: false,
            compress: {
              width: 1600,
              height: 1600,
              quality: 0.8
            },
            onBeforeQueued: function(file, files) {
              console.log(this);
            },
            onQueued: function() {
              uploadCustomFileList.push(this);
            }
          });

          // Image mobile uploader preview.
          $(wrapperSelector + ' .moha_ui_image_mobile_uploader_preview .weui-uploader__files').each(function (index) {
            this.addEventListener('click', function (e) {
              var target = e.target;

              while (target && target.classList && !target.classList.contains('weui-uploader__file') ) {
                target = target.parentNode;
              }

              if (!target || !target.classList) {
                return;
              }

              let url = target.getAttribute('style') || '';
              const id = target.getAttribute('data-id');

              if (url) {
                url = url.match(/url\((.*?)\)/)[1].replace(/"/g, '');
              }
              var gallery = weui.gallery(url, {
                onDelete: function () {
                  weui.confirm(Drupal.t('Cancel this image?'), function () {
                    var index;
                    for (var i = 0, len = uploadCustomFileList.length; i < len; ++i) {
                      var file = uploadCustomFileList[i];
                      if (file.id == id) {
                        index = i;
                        break;
                      }
                    }
                    if (index !== undefined) {
                      uploadCustomFileList.splice(index, 1);
                    }

                    target.remove();
                    gallery.hide();
                    $('.weui-uploader__input', context).val('');
                    $('.weui-uploader__input-box', context).show();
                  });
                }
              });
            });
          }); // each.

        }); // moha_ui_image_mobile_uploader once end.

      } // Widget wrappers for end.

    } // function attach.
  };

})(jQuery);
