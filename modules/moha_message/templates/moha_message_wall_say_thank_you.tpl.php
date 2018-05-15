<script src="/sites/all/libraries/swiper/dist/js/swiper.min.js"></script>
<link rel="stylesheet" href="/sites/all/libraries/swiper/dist/css/swiper.min.css">

<div id="moha-message-contents-header"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-contents-header.png"></div>

<div id="moha-message-wall-say-thank-you" class="swiper-container">
  <div class="swiper-wrapper">
    <div class="moha-message-cover-html swiper-slide"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-cover-1.jpg"></div>
    <div class="moha-message-cover-html swiper-slide"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-cover-2.jpg"></div>
    <div class="moha-message-contents-placeholder swiper-slide">Message here ~~</div>
  </div>
  <?php if (empty($contents['messages'])): ?>
    <p>Still empty message wall.</p>
  <?php endif; ?>
</div>

<div id="moha-button-say-thank-you">
  <a href="/moha/message-wall-submit/SayThankYou"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-button.png"></a>
</div>

<?php if (!empty($contents['messages'])): ?>
  <link rel="stylesheet" href="<?php echo MOHA__PATH;?>/css/alertify.css">
  <script src="<?php echo MOHA__PATH;?>/js/alertify.js"></script>

  <script>
    (function ($) {
      Drupal.behaviors.moha_message = {
        attach: function (context, settings) {
          // Add collapsible effect for image and video field.
          $('#moha-message-wall-say-thank-you', context).once('alertify-js', function () {

            let messages = <?php echo json_encode($contents['messages'])?>;
            const messages_count = messages.length;
            const messages_init_count = 20;

            let current_message_index = 0;
            let current_message_type = true;

            alertify.maxLogItems(messages_init_count);

            /**
             * Push and fetch message from message pool.
             */
            function show_message() {
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

            setInterval(show_message, 3000);

            const swiper = new Swiper('.swiper-container', {
              direction: 'vertical',
              watchActiveIndex: true,
              on: {
                slideChangeTransitionEnd: function () {
                  if (this.activeIndex === 2) {
                    $('body.page-moha-message-wall div.alertify-logs').css('visibility', 'visible').css('z-index', 1);
                    $('div#moha-button-say-thank-you').css('z-index', 2);
                    $('div#moha-message-contents-header').css('z-index', 2);
                  }
                  else {
                    $('body.page-moha-message-wall div.alertify-logs').css('visibility', 'hidden').css('z-index', -1);
                    $('div#moha-button-say-thank-you').css('z-index', -1);
                    $('div#moha-message-contents-header').css('z-index', -1);
                  }
                }
              }
            });

          });
        }
      };

    })(jQuery);
  </script>

  <style>
    body.page-moha-message-wall div.alertify-logs {
      left: 10px;
      right: 10px;
      bottom: 80px;
      z-index: -1;
      visibility: hidden;
      background: transparent;
    }

    /* set background of message wall and placeholder in swiper. */
    .moha-message-contents-placeholder.swiper-slide {
      background: url(/sites/all/modules/custom/moha/modules/moha_message/img/message-background.jpg) no-repeat top center;
      -webkit-background-size: contain;
      -moz-background-size: contain;
      -o-background-size: contain;
      background-size: contain;
    }

    div#moha-message-contents-header {
      position: fixed;
      left: 0;
      right: 0;
      top: 0;
      padding-left: 0;
      padding-right: 0;
      z-index: -1;
    }

    div#moha-button-say-thank-you {
      position: fixed;
      text-align: center;
      left: 0;
      right: 0;
      bottom: 0;
      padding-left: 0;
      padding-right: 0;
    }

    div#moha-button-say-thank-you a {
      width: 100%;
      display: inline;
      vertical-align: middle;
      text-align: center;
    }

    div#moha-message-contents-header img, div#moha-button-say-thank-you a img {
      width: 100%;
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
      text-align: right;
    }

    html, body, div.main-container, div#block-system-main,
    div.main-container div.row, div.main-container div.row section,
    div.main-container div.row section div.region-content{
      position: relative;
      height: 100%;
    }

    body {
      background: #eee;
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

    .moha-message-cover-html img, .swiper-container {
      width: 100%;
      height: 100%;
    }

    .swiper-slide {
      text-align: center;
      font-size: 18px;
      background: #fff;

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

    .moha-message-contents-placeholder {
      color: transparent;
      z-index: -10;
    }

  </style>
<?php endif; ?>