(function ($) {
  $(function() {

    $('#form-cancel').click(function (event){
      event.preventDefault();
      window.close();

      // @see https://mp.weixin.qq.com/wiki?t=resource/res_main&id=mp1421141115 10.3.
      if (wx !== undefined) {
        wx.closeWindow();
      }

    });

  });
})(jQuery);
