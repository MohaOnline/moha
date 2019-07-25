<?php
/**
 * @var string $breadcrumb
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
      <a href="#" class="seven-branding-toggle" data-toggle="push-menu" role="button">
        <span class="sr-only">Toggle branding</span>
      </a>
    </div>
  </nav>
</header>

  <!-- Content Wrapper. Contains page content -->
  <div class="content-wrapper">
    <!-- Main content -->
    <section class="content container-fluid">
      <?php print render($page['content']); ?>
    </section>
    <!-- /.content -->
  </div>
  <!-- /.content-wrapper -->

  <!-- Main Footer -->
  <footer class="main-footer">
    <!-- To the right -->
    <div class="pull-right hidden-xs">

    </div>
    <!-- Default to the left -->
    <strong>Copyleft &copy; 2017-2019</strong>
  </footer>

  <!-- Control Sidebar -->
  <aside class="main-sidebar">

    <!-- sidebar: style can be found in sidebar.less -->
    <section class="sidebar">

      <!-- Sidebar Menu -->
      <ul class="sidebar-menu" data-widget="tree">
        <li class="header">Features</li>
        <!-- Optionally, you can add icons to the links -->
        <!-- https://fontawesome.com/v4.7.0/cheatsheet/ -->
        <li><a href="#"><i class="fa fa-microchip"></i> <span>IT Steward</span></a></li>
        <li><a href="#"><i class="fa fa-scribd"></i> <span>Survey</span></a></li>
        <li><a href="#"><i class="fa fa-envelope"></i> <span>Mailer</span></a></li>
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
