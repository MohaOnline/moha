/**
 * @file
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object, _:object */

(function ($) {
  const Password = {
    _pattern : /[a-zA-Z0-9_#=@]/,

    _getRandomByte : function()
    {
      // http://caniuse.com/#feat=getrandomvalues
      if(window.crypto && window.crypto.getRandomValues)
      {
        const result = new Uint8Array(1);
        window.crypto.getRandomValues(result);
        return result[0];
      }
      else if(window.msCrypto && window.msCrypto.getRandomValues)
      {
        const result = new Uint8Array(1);
        window.msCrypto.getRandomValues(result);
        return result[0];
      }
      else
      {
        return Math.floor(Math.random() * 256);
      }
    },

    generate : function(length)
    {
      return Array.apply(null, {'length': length})
          .map(function()
          {
            let result;
            while(true)
            {
              result = String.fromCharCode(this._getRandomByte());
              if(this._pattern.test(result))
              {
                return result;
              }
            }
          }, this)
          .join('');
    }

  };

  Drupal.behaviors.mohaBlock = {

    /**
     * Attach behaviors to enhance UIs.
     *
     * @param context document
     * @param settings Drupal.settings
     *
     * @see Drupal.attachBehaviors JSDoc.
     */
    attach: function(context, settings) {
      $('.moha-block-service-conversion', context).once('moha-block-service-conversion', function () {
        const serviceConversionBlock = this;
        // Convert MySQL Timestamp.
        $('#mysql-timestamp-convert', serviceConversionBlock).on('click', function (e) {
          const timestamp = $('#mysql-timestamp', serviceConversionBlock).val();
          if (_.toNumber(timestamp)) {
            const d = new Date(timestamp * 1000);
            $('#mysql-human-date', serviceConversionBlock).html(d.toLocaleString('zh-CN'));
          }
        });

        // Generate random password.
        $('button#password-generate', serviceConversionBlock).on('click', function (e) {
            $('p#password-placeholder', serviceConversionBlock).html(Password.generate(16));

        });

        // moha-block-service-conversion once finished.
      });


    }
  };

})(jQuery);
