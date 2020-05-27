<?php
/**
 * @var string $breadcrumb
 * @var string $messages
 * @var array $secondary_local_tasks
 * @var array $page
 */
?>
<div id="branding" class="clearfix">
  <?php print $breadcrumb; ?>
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h1 class="page-title"><?php print $title; ?></h1>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php print render($primary_local_tasks); ?>
</div>

<div class="wrapper">
  <?php if (user_access(MOHA_UI__PERMISSION__DASHBOARD)): ?>
  <header class="main-header">

    <!-- Logo -->
    <a href="/admin/moha/dashboard" class="logo">
      <!-- mini logo for sidebar mini 50x50 pixels -->
      <span class="logo-mini"><b>D</b></span>
      <!-- logo for regular state and mobile devices -->
      <span class="logo-lg"><b>D</b>ashboard</span>
    </a>

    <!-- Header Navbar -->
    <nav class="navbar navbar-static-top" role="navigation">
      <!-- Sidebar toggle button-->
      <a href="#" class="sidebar-toggle" data-toggle="push-menu" role="button">
        <span class="sr-only">Toggle navigation</span>
      </a>


      <!-- Navbar Right Menu -->
      <div class="navbar-custom-menu">
        <!-- branding of seven toggle button-->
        <a title="Toggle Drupal Admin Menu" href="#" class="moha-toggle-class"
           data-toggle="toggle-class" data-toggle-class="drupal-admin-ui"
           role="button">
          <span class="sr-only">Toggle Drupal admin UI</span>
        </a>
      </div>
    </nav>
  </header>
  <?php endif; ?>

  <!-- Content Wrapper. Contains page content -->
  <div class="content-wrapper">
    <!-- Main content -->
    <section class="content container-fluid">
      <?php print $messages; ?>
      <?php print render($page['content']); ?>
    </section>
    <!-- /.content -->
  </div>
  <!-- /.content-wrapper -->


  <?php if (user_access(MOHA_UI__PERMISSION__DASHBOARD)): ?>
  <!-- Main Footer -->
  <footer class="main-footer">
    <!-- Showcase container on right side of footer. -->
    <div class="pull-right hidden-xs">

    </div>
    <!-- Default to the left -->
    <strong>Copyleft &copy; 2017-2020</strong>
  </footer>
  <?php endif; ?>


  <!-- Control Sidebar -->
  <aside class="main-sidebar">

    <!-- sidebar: style can be found in sidebar.less -->
    <section class="sidebar">

      <!-- Sidebar Menu -->
      <ul class="sidebar-menu" data-widget="tree">
        <!-- Optionally, you can add icons to the links -->
        <?php echo moha_ui_dashboard_sidebar_build(); ?>
      </ul>
      <!-- /.sidebar-menu -->
    </section>
    <!-- /.sidebar -->
  </aside>
  <!-- /.control-sidebar -->
  <!-- Add the sidebar's background. This div must be placed
  immediately after the control sidebar -->
  <div class="control-sidebar-bg"></div>

</div> <!-- class="wrapper" -->
