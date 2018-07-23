<?php
/**
 * @file
 */
?>
<!DOCTYPE html>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
  <style type="text/css">
    body, html,#baidu-map {
      width: 100%;
      height: 100%;
      overflow: hidden;
      margin: 0;
      font-family: "微软雅黑",  sans-serif;
    }
    h1,h2,h3,p {
      margin: 1px 0;
    }
    .moha-map-wrapper img {
      float: right;
      max-width: 150px;
    }
  </style>
  <title><?php echo moha_an2e($content, 'title'); ?></title>
</head>
<body>
<div id="baidu-map"></div>
</body>
</html>
<script type="text/javascript">
  // 百度地图API功能
  function loadJScript() {
    let script = document.createElement("script");
    script.type = "text/javascript";
    script.src = "//api.map.baidu.com/api?v=2.0&callback=init&ak=<?php echo $content['baidu_access_key']?>";
    document.body.appendChild(script);
  }

  function init() {
    let map = new BMap.Map("baidu-map");
    let point = new BMap.Point(<?php echo $content['lonlat']?>);
    map.centerAndZoom(point, 18);
    let marker = new BMap.Marker(point);  // 创建标注
    map.addOverlay(marker);               // 将标注添加到地图中

    let infoContents =
        '<div class="moha-map-wrapper"><?php echo $content['info_window_contents']; ?></div>';

    let infoWindow = new BMap.InfoWindow(infoContents);

    marker.openInfoWindow(infoWindow);

    marker.addEventListener("click", function(){
      this.openInfoWindow(infoWindow);
      document.getElementsByClassName('moha-map-wrapper');
      document.getElementsByClassName('info-window-img').onload = function (){
        infoWindow.redraw();   //防止在网速较慢，图片未加载时，生成的信息框高度比图片的总高度小，导致图片部分被隐藏
      }
    });

  }
  window.onload = loadJScript;  //异步加载地图

</script>