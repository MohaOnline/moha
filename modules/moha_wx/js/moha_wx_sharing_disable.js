(function($) { $(function() {

  function wxBridgeReady() {
    WeixinJSBridge.invoke('hideOptionMenu');
  }

  if (typeof WeixinJSBridge === "object" && typeof WeixinJSBridge.invoke === "function") {
    wxBridgeReady();
  }
  else {
    if (document.addEventListener) {
      document.addEventListener('WeixinJSBridgeReady', wxBridgeReady, false);
    }
    else if (document.attachEvent) {
      document.attachEvent('WeixinJSBridgeReady', wxBridgeReady);
      document.attachEvent('onWeixinJSBridgeReady', wxBridgeReady);
    }
  }

}); }) (jQuery);