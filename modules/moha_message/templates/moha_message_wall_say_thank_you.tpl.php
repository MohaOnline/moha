<div id="moha-message-wall-say-thank-you" class="swiper-container">
  <div class="swiper-wrapper">
    <div class="moha-message-cover-html swiper-slide"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-cover-1.jpg"></div>
    <div class="moha-message-cover-html swiper-slide"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-cover-2.jpg"></div>
    <div class="moha-message-contents-placeholder swiper-slide">
      <div class="moha-message-contents alertify-logs"></div>
      <div id="moha-message-contents-header"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-contents-header.png"></div>
      <div id="moha-button-say-thank-you">
        <a href="/moha/message-wall-submit/SayThankYou"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-button.png"></a>
      </div>
    </div>
    <div class="moha-message-cover-html swiper-slide"><img src="/sites/all/modules/custom/moha/modules/moha_message/img/message-success.jpg"></div>
  </div>
  <?php if (empty($contents['messages'])): ?>
    <p>Still empty message wall.</p>
  <?php endif; ?>
</div>

<?php if (!empty($contents['messages'])): ?>
  <script src="<?php echo MOHA__PATH;?>/js/alertify.js"></script>

  <script>
    (function ($) {
      Drupal.behaviors.moha_message = {
        attach: function (context, settings) {
          // Add collapsible effect for image and video field.
          $('#moha-message-wall-say-thank-you', context).once('alertify-js', function () {

            let messages = <?php echo json_encode($contents['messages'])?>;
            const messages_count = messages.length;
            const messages_init_count = 16;

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
      text-align: right;
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
<?php endif; ?>