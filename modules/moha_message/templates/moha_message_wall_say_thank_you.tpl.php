<div id="moha-message-wall-say-thank-you">
  <?php if (empty($contents['messages'])): ?>
    <p>Still empty message wall.</p>
  <?php endif; ?>
</div>

<div id="moha-button-say-thank-you">
  <a class="btn btn-primary" href="/moha/message-wall-submit/SayThankYou">Say Thank You</a>
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

          });
        }
      };

    })(jQuery);
  </script>

  <style>
    body.page-moha-message-wall div.alertify-logs {
      left: 0;
      right: 0;
      bottom: 60px;
      z-index: -1;
    }

    div#moha-button-say-thank-you {
      position: fixed;
      text-align: center;
      height: 40px;
      left: 0;
      right: 0;
      bottom: 0;
    }

    div#moha-button-say-thank-you a {
      width: 100%;
      display: inline;
      vertical-align: middle;
      text-align: center;
    }

    .alertify-logs>*, .alertify-logs>.default, .alertify-logs>.success {
      background: rgba(0,0,0,.8);
      border-radius: 5px;
    }

    .alertify-logs>.success {
      float: right;
      text-align: right;
    }
  </style>
<?php endif; ?>