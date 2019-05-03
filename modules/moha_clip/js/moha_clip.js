jQuery(document).ready(function () {

  // Per sidebar height, show scroll to top button
  function showScroll2top(visible) {
    const button = jQuery("div.moha-clip-scroll-2-top");

    if (visible) {
      button.addClass('moha-clip-scroll-2-top-display');
    }
    else {
      button.removeClass('moha-clip-scroll-2-top-display');
    }
  }

  function blockFloat() {
    const debug = false;
    // If add animation effect to float content block.
    const animationFloat = false;

    // Revise for fixed top admin menu.
    const adminMenu = jQuery("#admin-menu");
    const adminMenuHeight = Number(adminMenu.height());
    const needFixAdminMenuHeight = adminMenu.css("position") === "fixed";

    // Float area information.
    const floatArea = jQuery('div.col-md-3.col-xs-12');
    if (floatArea.offset() === undefined) {
      return;
    }
    const floatAreaTopY = floatArea.offset().top;
    const floatAreaHeight = floatArea.height();

    // Blocks above floating block.
    const occupiedHeight = jQuery('div.author-block').outerHeight();

    const floatBlock = jQuery("div.toc-block");
    const floatBlockHeight = floatBlock.outerHeight();

    if (debug) {
      console.log('floatAreaTopY: ' + floatAreaTopY);
      console.log('occupiedHeight: ' + occupiedHeight);
    }

    // Height between top of page and top of window.
    // Distance mouse has scrolled on page.
    let scrolledHeight = jQuery(window).scrollTop();

    if (debug) {
      console.log('scrolledHeight: ' + scrolledHeight);
    }

    // Start point of page scroll is bottom of admin menu when admin menu is fixed.
    if (needFixAdminMenuHeight) {
      scrolledHeight += adminMenuHeight;
    }

    // Basing on scrolled distance to calculate need revised top Y coordinate.
    if (scrolledHeight > (floatAreaTopY + occupiedHeight)) {
      let blockTopRevised = scrolledHeight - (floatAreaTopY + occupiedHeight);

      // Float block must be within float area, if exceed, need set to maximum value.
      if (blockTopRevised > floatAreaHeight - occupiedHeight - floatBlockHeight) {
        blockTopRevised = floatAreaHeight - occupiedHeight - floatBlockHeight;
      }

      // Stop current scrolling then scroll to new position.
      if (animationFloat) {
        floatBlock.stop().animate({top: blockTopRevised}, 800, 'swing');
      }
      else {
        floatBlock.css('top', blockTopRevised);
      }

      showScroll2top(true);
    }
    else if (scrolledHeight <= (floatAreaTopY + occupiedHeight)) {
      // Gives second chance to move content block to correct position.
      if (animationFloat) {
        floatBlock.stop().animate({top: 0}, 800, 'swing');
      }
      else {
        floatBlock.css('top', 0);
      }

      showScroll2top(false);
    }

  }

  jQuery(window).scroll(blockFloat);

  // Bind scroll to top action to button.
  jQuery('.moha-clip-scroll-2-top').click(function () {
    jQuery("html, body").animate({ scrollTop: 0 });
  });

  new Swiper('.moha-clip-widget.swiper-container', {
    // Optional parameters
    speed: 800,
    loop: false,

    // If we need pagination
    pagination: {
      el: '.swiper-pagination',
      dynamicBullets: true,
      type: 'fraction',
      clickable: true
    },

    navigation: {
      nextEl: '.swiper-button-next',
      prevEl: '.swiper-button-prev',
    }
  });

});
