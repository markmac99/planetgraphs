<?php
require_once('calendar/classes/tc_calendar.php');
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="Planetary Observability Data.">
    <meta name="author" content="Mark McIntyre">
	<meta name="keywords" content="******"/>

    <title>Planetary Observability Calculations</title>

    <!-- Bootstrap Core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet">

    <!-- MetisMenu CSS -->
    <link href="css/plugins/metisMenu/metisMenu.min.css" rel="stylesheet">

    <!-- Timeline CSS -->
    <link href="css/plugins/timeline.css" rel="stylesheet">

    <!-- Custom CSS -->
    <link href="css/dragontail.css" rel="stylesheet">

    <!-- Morris Charts CSS -->
    <link href="css/plugins/morris.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="font-awesome-4.5.0/css/font-awesome.min.css" rel="stylesheet" type="text/css">

	<link href="calendar/calendar.css" rel="stylesheet" type="text/css" />
	<script language="javascript" src="calendar/calendar.js"></script>

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
        <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->

    <style>
	  #google_map1 {
	    width: 110%;
	  }
	  html, body {
	    width: 110%;
	    margin: 0;
	    padding: 0;
	  }
	</style>

</head>

<body>
    <div id="wrapper">
        <!-- Navigation -->
        <nav class="navbar navbar-default navbar-static-top" role="navigation" style="margin-bottom: 0">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                    <span class="sr-on5ly">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="https://www.marymcintyreastronomy.co.uk/"><img src="images/maryslogo.png" alt="Mary McIntyre, Logo"/></a>
            </div>
            <!-- /.navbar-header -->

            <ul class="nav navbar-top-links navbar-right">
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-envelope fa-fw"></i>  <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-messages">
                        <li>
                            <a href="#">
                                <div>
                                    <strong>Email me </strong>
                                    <span class="pull-right text-muted">
                                        <em>mark.jm.mcintyre@cesmail.net</em>
                                    </span>
                                </div>
                                <div>Email me with questions etc</div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a class="text-center" href="#">
                                <strong><a href="mailto:mark.jm.mcintyre@cesmail.net?Subject=Communication from Web Site" target="_top">Send Mail</a></strong>
                                <i class="fa fa-angle-right"></i>
                            </a>
                        </li>
                    </ul>
                    <!-- /.dropdown-messages -->
                </li>
                <!-- /.dropdown -->
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-user fa-fw"></i>  <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-user">
                        <li><a href="#"><i class="fa fa-upload fa-fw"></i> Link *****</a>
                        </li>
                        <li class="divider"></li>
						<li><a href="aboutthis.html"><i class="fa fa-user fa-fw"></i> About</a>
                        </li>

                    </ul>
                    <!-- /.dropdown-user -->
                </li>
                <!-- /.dropdown -->
            </ul>
            <!-- /.navbar-top-links -->

            <div class="navbar-default sidebar" role="navigation">
                <div class="sidebar-nav navbar-collapse">
                    <ul class="nav" id="side-menu">
                        <li>
                            <a href="#"><i class="fa fa-bar-chart-o fa-fw"></i> Charts<span class="fa arrow"></span></a>
                            <ul class="nav nav-second-level">
                                <li>
                                    <a href="MercuryGraphs.html">Mercury</a>
                                </li>
                                <li>
                                    <a href="VenusGraphs.html">Venus</a>
                                </li>
                                <li>
                                    <a href="MarsGraphs.html">Mars</a>
                                </li>
                                <li>
                                    <a href="JupiterGraphs.html">Jupiter</a>
                                </li>
                                <li>
                                    <a href="SaturnGraphs.html">Saturn</a>
                                </li>
                                <li>
                                    <a href="UranusGraphs.html">Uranus</a>
                                </li>
                                <li>
                                    <a href="NeptuneGraphs.html">Neptune</a>
                                </li>
                            </ul>
                            <!-- /.nav-second-level -->
                        </li>
                    </ul>
                </div>
                <!-- /.sidebar-collapse -->
            </div>
            <!-- /.navbar-static-side -->
        </nav>
        <!-- Page Content -->
        <div id="page-wrapper">
            <div class="container-fluid">
            <div class="row">
                <div class="col-lg-8">
                    <div class="panel panel-primary">
                        <div class="panel-heading">
                            Create Custom Graphs
                        </div>
                        <div class="panel-body">
							  <p class="largetxt"><b>Select start date and drag the marker to your location</b></p>
							  <form name="form1" method="GET" action="./refreshdata.php">
							  <?php
							  $myCalendar = new tc_calendar("date5", true, false);
							  $myCalendar->setTimezone("Europe/London");
							  $myCalendar->setIcon("calendar/images/iconCalendar.gif");
							  $myCalendar->setPath("calendar/");
							  $myCalendar->setYearInterval(2018, 2026);
							  $myCalendar->setDateFormat('j F Y');
							  $myCalendar->setAlignment('left', 'bottom');
							  $myCalendar->writeScript();
							  ?>
							  <div id="google_map" style="width:600px;height:600px;"></div>
							  <div id="Latmark">Nothing yet...</div>
							  <div id="Longmark">Nothing yet...</div>
							  <input type="hidden" name="Lat" id="Lat">
							  <input type="hidden" name="Long" id="Long">
  							  <script>
								  function initMap() {
									var oxford = {lat: 51.88, lng: -1.31};
									var map = new google.maps.Map(document.getElementById('google_map'), {
									  zoom: 6,
									  center: oxford
									});
									var marker = new google.maps.Marker({
									  position: oxford,
									  draggable: true,
									  map: map
									});
									google.maps.event.addListener(marker, 'dragend', function (evt) {
									    document.getElementById('Latmark').innerHTML = evt.latLng.lat().toFixed(3);
									    document.getElementById('Longmark').innerHTML = evt.latLng.lng().toFixed(3);
									    document.getElementById('Lat').value = evt.latLng.lat().toFixed(3);
									    document.getElementById('Long').value = evt.latLng.lng().toFixed(3);
									});
									google.maps.event.addListener(marker, 'dragstart', function (evt) {
									    document.getElementById('Latmark').innerHTML = '<p>Currently dragging marker...</p>';
									    document.getElementById('Longmark').innerHTML = '<p>Currently dragging marker...</p>';
									});
								  }
								</script>
								<script type="text/javascript" async defer
								  src="https://maps.googleapis.com/maps/api/js?key=AIzaSyA3j6m0AQ0AwqetHgGg3OE6by4TqUCb-oE&callback=initMap">
								</script>
							  <input type="submit" name="button" id="button" value="Use these Values">
							  </form>
                        </div>
                        <div class="panel-footer">
                            Good luck and may the weather be with you.
                        </div>
                    </div>
                </div>
                <!-- /.col-lg-4 -->
            </div>
                <!-- /.row -->
            </div>
            <!-- /.container-fluid -->
        </div>
        <!-- /#page-wrapper -->

    </div>
    <!-- /#wrapper -->

    <!-- jQuery -->
    <script src="js/jquery.js"></script>


    <!-- Bootstrap Core JavaScript -->
    <script src="js/bootstrap.min.js"></script>

    <!-- Metis Menu Plugin JavaScript -->
    <script src="js/plugins/metisMenu/metisMenu.min.js"></script>

    <!-- Custom Theme JavaScript -->
    <script src="js/sb-admin-2.js"></script>
	<script>
	</script>
</body>

</html>
