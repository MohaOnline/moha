/**
 *
 */

var primary_area_template = '';
primary_area_template += '<div class="moha-spotlight primary-area">';
primary_area_template += '<div class="moha-spotlight-icon fa fa-book"><span>&nbsp;</span></div>';
primary_area_template += '<div class="moha-spotlight-details"><p>&nbsp;</p></div>';
primary_area_template += '</div>';

var info_area_template = '';
info_area_template += '<div class="moha-spotlight info-area">';
info_area_template += '<div class="moha-spotlight-icon fa fa-info"><span>&nbsp;</span></div>';
info_area_template += '<div class="moha-spotlight-details"><p>&nbsp;</p></div>';
info_area_template += '</div>';

var reference_links_template = '';
reference_links_template += '<div class="moha-spotlight reference-area">';
reference_links_template += '<div class="moha-spotlight-icon fa fa-external-link"><span>&nbsp;</span></div>';
reference_links_template += '<div class="moha-spotlight-details reference-details"><p>&nbsp;</p></div>';
reference_links_template += '</div>';

var warning_area_template = '';
warning_area_template += '<div class="moha-spotlight warning-area">';
warning_area_template += '<div class="moha-spotlight-icon fa fa-warning"><span>&nbsp;</span></div>';
warning_area_template += '<div class="moha-spotlight-details warning-details"><p>&nbsp;</p></div>';
warning_area_template += '</div>';

var danger_area_template = '';
danger_area_template += '<div class="moha-spotlight danger-area">';
danger_area_template += '<div class="moha-spotlight-icon fa fa-times"><span>&nbsp;</span></div>';
danger_area_template += '<div class="moha-spotlight-details"><p>&nbsp;</p></div>';
danger_area_template += '</div>';

let table_template = '<div class="moha-clip-widget"><div class="moha-clip-widget-editable">';
table_template += '<table class="moha-table table table-striped">';
table_template += '<thead><tr><th>&nbsp</th><th>&nbsp</th></tr></thead>';
table_template += '<tbody><tr><td>&nbsp</td><td>&nbsp</td></tr><tr><td>&nbsp</td><td>&nbsp</td></tr></tbody>';
table_template += '</table></div></div>';

let image_gallery_template = '<div class="moha-clip-widget swiper-container">';
image_gallery_template += '<div class="moha-clip-widget-editable swiper-wrapper">';
image_gallery_template += '<p class="moha-clip-widget moha-clip-widget-editable swiper-slide">&nbsp;</p></div>';
image_gallery_template += '<div class="swiper-pagination"><span>Paging</span></div><div class="swiper-button-next"><span>Next</span></div><div class="swiper-button-prev"><span>Prev</span></div></div>';

CKEDITOR.addTemplates('moha',
  {
    imagesPath: Drupal.settings.moha.ckeditor_template_image_path,

    templates: [
      {
        title: 'Block: Primary',
        image: 'block-primary.png',
        html: primary_area_template
      },
      {
        title: 'Block Info',
        image: 'block-info.png',
        description: 'Gives info messages.',
        html: info_area_template
      },
      {
        title: 'Block Reference',
        image: 'block-reference.png',
        description: 'Collects related reference links.',
        html: reference_links_template
      },
      {
        title: 'Block Warning',
        image: 'block-warning.png',
        description: 'Gives warning messages.',
        html: warning_area_template
      },
      {
        title: 'Block Danger',
        image: 'block-danger.png',
        description: 'Gives danger alerts.',
        html: danger_area_template
      },
      {
        title: 'Table',
        image: 'block-table.png',
        description: 'Insert table to host data.',
        html: table_template
      },
      {
        title: 'Image Gallery',
        image: 'block-image-slideshow.png',
        description: 'Insert image gallery to host multiple images.',
        html: image_gallery_template
      }
    ]
  });
