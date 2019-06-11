/**
 * @file
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object, weui:object */

(function ($) {

  Drupal.behaviors.mohaUIImageMobileUploader = {
    attach: function(context, settings) {

   //   $('.moha_ui_image_mobile_uploader', context).once('moha_ui_image_mobile_uploader', function () {

        let uploadCustomFileList = [];

        weui.uploader('.moha_ui_image_mobile_uploader', {
          auto: false,
          compress: {
            width: 1600,
            height: 1600,
            quality: 0.8
          },
          onBeforeQueued: function(file, files) {
            console.log(this);
            console.log(file);
            console.log(files);
          },
          onQueued: function() {
            uploadCustomFileList.push(this);
            console.log(uploadCustomFileList);
          }
        });

        // 缩略图预览
        var preview_window = document.querySelector('.weui-uploader__files');
        if (preview_window) {
          document.querySelector('.weui-uploader__files').addEventListener('click', function (e) {
            var target = e.target;

            while (!target.classList.contains('weui-uploader__file') && target) {
              target = target.parentNode;
            }
            if (!target) {
              return;
            }

            var url = target.getAttribute('style') || '';
            var id = target.getAttribute('data-id');

            if (url) {
              url = url.match(/url\((.*?)\)/)[1].replace(/"/g, '');
            }
            var gallery = weui.gallery(url, {
              onDelete: function () {
                weui.confirm('确定删除该图片？', function () {
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
        }

  //    });



    }
  };

})(jQuery);
