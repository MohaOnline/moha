jQuery( document ).ready(function() {

      function pageScroll(){
        var currentY = Number(jQuery(window).scrollTop());

        if (console) {
          console.log('currentY: ' + currentY);
        }

        // Related Product Area
        var occupiedY = Number(jQuery('div.author-block').css('height').replace('px', ''));

        var scopeY = Number(jQuery('div.col-md-3.col-xs-12').css('height').replace('px',''));

        var floatTop = Number(jQuery('div.toc-block').css('top').replace('px',''));

        if (console) {
          console.log('floatTop: ' + floatTop);
        }

        var excludeScroll = Number(jQuery('div.author-block').css('height').replace('px', ''));

        if (currentY > ( excludeScroll + occupiedY ) && ((floatTop+0)<(scopeY-occupiedY))){

          var newTop = currentY- ( excludeScroll + occupiedY ) + 0;

          if (newTop>(Number(scopeY)-occupiedY - 0)){
            newTop = Number(scopeY)-occupiedY - 0-1;
          }

          jQuery('div.toc-block').stop().animate({ top:newTop }, 800, 'swing');
        }
        else if (currentY <= ( excludeScroll + occupiedY )){
          if (floatTop != 0){
            jQuery('div.toc-block').stop().animate({ top:0 }, 800, 'swing');
          }
        }
      }

        jQuery(window).scroll(pageScroll);

});
