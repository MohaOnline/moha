<style>
  @keyframes rotating {
    from {
      transform: rotate(0deg)
    }
    to {
      transform: rotate(360deg)
    }
  }

  @-moz-keyframes rotating {
    from {
      -moz-transform: rotate(0deg)
    }
    to {
      -moz-transform: rotate(360deg)
    }
  }

  #moha-music-wrap {
    position: absolute;
    z-index: 200;
    top: 20px;
    left: 20px;
    width: 50px;
    height: 50px;
    background-size: contain;
  }

  #moha-music-wrap.rotate {
    background-image: url("/sites/all/modules/custom/moha/img/music.gif");
  }

  #moha-music-button {
    width: 30px;
    height: 30px;
    background: url("/sites/all/modules/custom/moha/img/music.svg") no-repeat;
    background-size: contain;
  }

  .rotate #moha-music-button {
    -webkit-animation: rotating 1.2s linear infinite;
    -moz-animation: rotating 1.2s linear infinite;
    -o-animation: rotating 1.2s linear infinite;
    animation: rotating 1.2s linear infinite;
  }

  #logo {
    position: absolute;
    right: 10px;
    top: 10px;
    z-index: 200;
  }

  #logo img {
    width: 50px;
  }

  img.melody {
    margin-top: -20px;
  }

  .background {
    height: 100%;
    width: 100%;
  }

  .background img {
    height: 100%;
    width: 100%;
  }

  .moha-1-arrow {
    margin-bottom: -20px;
  }

  .moha-2-arrow {
    margin-left: -45%;
    margin-bottom: -20px;
  }
</style>

<div id="moha-music-wrap">
  <div id="moha-music-button">
    <audio id="moha-music" autoplay preload="auto" loop="loop" src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/389429-Uplifting-Joyful-Funny.mp3"></audio>
  </div>
</div>
<div id="logo"><img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/common/ge-logo-blue.png"></div>
<div id="moha-message-wall-say-thank-you" class="swiper-container">
  <div class="swiper-wrapper">
    <div class="moha-message-cover-html swiper-slide">
      <div class="moha-message-cover-next background">
        <img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-cover-1-background.jpg">
      </div>
      <div class="moha-message-cover-next"><img class="moha-1-arrow" src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/arrow-down.gif"></div>
      <img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-cover-1-contents.png">
      <div id="moha-message-contents-header"><img class="melody" src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/melody.gif"></div>
    </div>


    <div class="moha-message-cover-html swiper-slide">
      <div class="moha-message-cover-next background">
        <img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-cover-1-background.jpg">
      </div>
      <div class="moha-message-cover-next"><img class="moha-2-arrow" src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/arrow-down.gif"></div>
      <div class="moha-message-cover-next"><img class="moha-22-arrow" src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/couple.gif"></div>
      <img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-cover-2-contents.png">
      <div id="moha-message-contents-header"><img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-cover-2-header.png"></div>
    </div>


    <div class="moha-message-contents-placeholder swiper-slide">
      <div class="moha-message-contents alertify-logs"></div>
      <div id="moha-message-contents-header"><img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-contents-header.png"></div>
      <div id="moha-button-say-thank-you">
        <a href="/moha/message-wall-submit/SayThankYou"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-button.png"></a>
      </div>
    </div>

    <div class="moha-message-cover-html swiper-slide">
      <div class="moha-message-cover-next">
        <img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-success-next.jpg">
      </div>
      <div class="moha-message-cover-next"><img class="moha-22-arrow" src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/coffee.gif"></div>
      <img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-success-contents.png">
      <div id="moha-message-contents-header"><img src="https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-success-header.png"></div>
    </div>
  </div>
  <?php if (empty($contents['messages'])): ?>
    <p>Still empty message wall.</p>
  <?php endif; ?>
</div>

  <script src="<?php echo MOHA__PATH;?>/js/alertify.js"></script>

  <script>
    (function ($) {
      Drupal.behaviors.moha_message = {
        attach: function (context, settings) {
          // Add collapsible effect for image and video field.
          $('#moha-message-wall-say-thank-you', context).once('alertify-js', function () {

            // Music initial.
            document.addEventListener("WeixinJSBridgeReady", function () {
              WeixinJSBridge.invoke('getNetworkType', {}, function (e) {
                document.getElementById('moha-music').play();
              });
            }, false);

            const audio = document.getElementById('moha-music');

            audio.onplay = function(){
              $musicButton.addClass('rotate');
            };

            audio.oncanplay = function() {
              audio.play();
              audio.oncanplay = function() {};
            };

            const $musicButton = $('#moha-music-wrap');

            $musicButton.click(function () {

              if (audio.paused !== false) {
                audio.play();
              }
              else {
                audio.pause();
                $musicButton.removeClass('rotate');
              }
            });

            // Change start slide after sumbit.
            let page = window.location.href.substr(window.location.href.lastIndexOf('/')+1, 1);

            if (page !== '2'){
              page = 0;
            }

            let messages = <?php if (!empty($contents['messages'])) { echo json_encode($contents['messages']); } else { echo 'null'; } ?>;

            if (messages === null) {
              messages = [];
            }

            const messages_count = messages.length;
            const messages_init_count = 16;

            let current_message_index = 0;
            let current_message_type = true;

            alertify.maxLogItems(messages_init_count);

            /**
             * Push and fetch message from message pool.
             */
            function show_message() {
              if (messages_count === 0) {
                return;
              }

              if (current_message_type) {
                alertify.delay(0).success(messages[current_message_index++].message, function(ev) {
                  // The click event is in the event variable, so you can use it here.
                  ev.preventDefault();
                });
              }
              else {
                alertify.delay(0).log(messages[current_message_index++].message, function(ev) {
                  // The click event is in the event variable, so you can use it here.
                  ev.preventDefault();
                });
              }

              // Switch message type: right / left.
              current_message_type = !current_message_type;

              if (current_message_index >= messages_count) {
                current_message_index = 0;
              }
            }

            for (let i = 0; i < messages_init_count; i++){
              show_message();
            }

            if (messages_count !== 0) {
              setInterval(show_message, 3000);
            }

            const swiper = new Swiper('.swiper-container', {
              direction: 'vertical',
              initialSlide: page,
              watchActiveIndex: true,
            });

          });
        }
      };

<?php
      try {
        $account = _moha_wx_moha_account('default');
        moha_wx_jsapi_config($account);
      }
      catch (Exception $e) {
        watchdog_exception(__FILE__, $e);
      }

?>
      $(function() {

        wx.ready(function(){
          // wx.hideOptionMenu();
          wx.onMenuShareTimeline({
            title: '哥家的表白墙',
            desc: '向你心中的英雄表白吧～～',
            link: '<?php echo (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]"; ?>',
            imgUrl: 'https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-icon.jpg',
            success: function () {
              // 用户确认分享后执行的回调函数
              //alert('分享到朋友圈成功');
            },
            cancel: function () {
              // 用户取消分享后执行的回调函数
              //alert('你没有分享到朋友圈');
            }
          });
          wx.onMenuShareAppMessage({
            title: '哥家的表白墙',
            desc: '向你心中的英雄表白吧～～',
            link: '<?php echo (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]"; ?>',
            imgUrl: 'https://gestorage.blob.core.chinacloudapi.cn/campaign/20180515/message-icon.jpg',
            trigger: function (res) {
              // 不要尝试在trigger中使用ajax异步请求修改本次分享的内容，因为客户端分享操作是一个同步操作，这时候使用ajax的回包会还没有返回
            },
            success: function (res) {
              //alert('分享给朋友成功');
            },
            cancel: function (res) {
              //alert('你没有分享给朋友');
            },
            fail: function (res) {
              //alert(JSON.stringify(res));
            }
          });
        });
      });

    })(jQuery);
  </script>

  <style>
    body.page-moha-message-wall div.alertify-logs {
      left: 10px;
      right: 10px;
      bottom: 80px;
      z-index: -1;
      background: transparent;
      position: absolute;
    }

    /* set background of message wall and placeholder in swiper. */
    .moha-message-contents-placeholder.swiper-slide {
      background: url(/sites/all/modules/custom/moha/modules/moha_message/img/message-background.jpg) no-repeat top center;
      -webkit-background-size: cover;
      -moz-background-size: cover;
      -o-background-size: cover;
      background-size: cover;
      padding-left: 0;
      padding-right: 0;
      color: transparent;
      position: relative;
      z-index: -1;
    }

    body {
      background: url(/sites/all/modules/custom/moha/modules/moha_message/img/message-background.jpg) no-repeat top center;
      -webkit-background-size: cover;
      -moz-background-size: cover;
      -o-background-size: cover;
      background-size: cover;
    }

    div#moha-message-contents-header {
      position: absolute;
      left: 0;
      right: 0;
      top: 0;
      padding-left: 0;
      padding-right: 0;
      z-index: 1;
    }

    div#moha-button-say-thank-you {
      position: absolute;
      text-align: center;
      left: 0;
      right: 0;
      bottom: 0;
      padding-left: 0;
      padding-right: 0;
      z-index: 1;
    }

    div#moha-button-say-thank-you a {
      width: 100%;
      display: inline;
      vertical-align: middle;
      text-align: center;
    }

    div#moha-message-contents-header img, div#moha-button-say-thank-you a img {
      width: 100%;
      z-index: 1;
    }

    .alertify-logs>*, .alertify-logs>.default, .alertify-logs>.success {
      background-color: #ffffff;
      border-radius: 200px;
      color: #004a98;
      border: solid #00b3e3 3px;
      padding: 20px;
    }

    .alertify-logs>.success {
      float: right;
      /*text-align: right;*/
    }

    html, body, div.main-container, div#block-system-main,
    div.main-container div.row, div.main-container div.row section,
    div.main-container div.row section div.region-content{
      position: relative;
      height: 100%;
    }

    body {
      font-family: Helvetica Neue, Helvetica, Arial, sans-serif;
      font-size: 14px;
      color:#000;
      margin: 0;
      padding: 0;
    }

    div.main-container div.row section {
      padding-left: 0;
      padding-right: 0;
    }

    .moha-message-cover-html {
      z-index: 2;
      overflow: hidden;
      position: relative;
    }

    .moha-message-cover-next {
      position: absolute;
      bottom: 0;
      width: 100%;
      z-index: -1;
    }

    .swiper-container {
      width: 100%;
      height: 100%;
    }

    .moha-message-cover-html img {
      width: 100%;
    }

    .swiper-slide {
      text-align: center;
      font-size: 18px;

      /* Center slide text vertically */
      display: -webkit-box;
      display: -ms-flexbox;
      display: -webkit-flex;
      display: flex;
      -webkit-box-pack: center;
      -ms-flex-pack: center;
      -webkit-justify-content: center;
      justify-content: center;
      -webkit-box-align: center;
      -ms-flex-align: center;
      -webkit-align-items: center;
      align-items: center;
    }

  </style>