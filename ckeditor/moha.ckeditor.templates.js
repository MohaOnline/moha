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
      }
    ]
  });
