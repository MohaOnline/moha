jQuery( document ).ready(function() {

  function tocFloat() {
    //
    const scopeArea = jQuery('div.col-md-3.col-xs-12');
    const scopeStartPoint = scopeArea.offset().top;
    const scopeHeight = scopeArea.height();

    const occupiedHeight = jQuery('div.author-block').outerHeight();

    const tocBlock = jQuery("div.toc-block");
    const tocHeight = tocBlock.outerHeight();

    if (console) {
      console.log('scopeStartPoint: ' + scopeStartPoint);
      console.log('occupiedHeight: ' + occupiedHeight);
    }

    // Height between top of page and top of window.
    let scrolledHeight = jQuery(window).scrollTop();

    if (console) {
      console.log('pageOffsetY: ' + scrolledHeight);
    }

    if ( scrolledHeight > (scopeStartPoint + occupiedHeight) ) {
      let tocTop = scrolledHeight - ( scopeStartPoint + occupiedHeight );

      if (tocTop > scopeHeight - occupiedHeight - tocHeight) {
        tocTop = scopeHeight - occupiedHeight - tocHeight;
      }

      tocBlock.stop().animate({ top:tocTop }, 800, 'swing');
    }
    else if (scrolledHeight <= (scopeStartPoint + occupiedHeight)) {
      tocBlock.stop().animate({ top:0 }, 800, 'swing');
    }

  }

  jQuery(window).scroll(tocFloat);

});
