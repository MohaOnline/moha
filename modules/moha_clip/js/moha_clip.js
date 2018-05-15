jQuery(document).ready(function () {

  function blockFloat() {
    const debug = false;

    // Revise for fixed top admin menu.
    const adminMenu = jQuery("#admin-menu");
    const adminMenuHeight = Number(adminMenu.height());
    const needFixAdminMenuHeight = adminMenu.css("position") === "fixed";

    // Float area information.
    const floatArea = jQuery('div.col-md-3.col-xs-12');
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

      floatBlock.stop().animate({top: blockTopRevised}, 800, 'swing');
    }
    else if (scrolledHeight <= (floatAreaTopY + occupiedHeight)) {
      floatBlock.stop().animate({top: 0}, 800, 'swing');
    }
  }

  jQuery(window).scroll(blockFloat);

});
