<?php
	$url=$_SERVER['REQUEST_URI'];
	header("Refresh: 5; URL=$url");
	/* Database connection settings */
	$servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "test";
    $conn = mysqli_connect($servername, $username, $password, $dbname);
    

	$time = '';
	$value = '';
	$id = '';
	$today = date("Y-m-d H:i:s");
	$tommorow = date('Y-m-d H:i:s', strtotime($today . ' +1 day'));

	//query to get data from the table
	//'2021-04-13 00:00:00'
	$sql = "SELECT * FROM sensor WHERE sensor.time BETWEEN CURRENT_DATE() AND CURRENT_DATE()+1 ORDER BY id ASC";
	//$sql = "SELECT * FROM sensor WHERE id BETWEEN 50 AND 100";
	//"SELECT order_date, SUM(grand_total) AS total_grand  FROM orders GROUP BY order_date LIMIT = 15";
    $result = mysqli_query($conn, $sql);

	//loop through the returned data
	while ($row = mysqli_fetch_array($result)) {

		$id = $id . '"'. $row['id'].'",';
		$time = $time . '"'. $row['time'].'",';
		$value = $value . '"'. $row['value'] .'",';
	}

	$id = trim($id,",");
	$time = trim($time,",");
	$value = trim($value,",");
?>

<!DOCTYPE html>
<html>
	<head>
		<meta name="author" content="Clement-XVII">
    	<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.2/Chart.bundle.min.js"></script>
		<title>Accelerometer data</title>

		<style type="text/css">			
			body{
				font-family: Arial;
			    margin: 80px 100px 10px 100px;
			    padding: 0;
			    color: white;
			    text-align: center;
			    background: #555652;
			    background-image: url('test.jpg');
			    background-size: cover;
			    background-attachment: fixed;
			}

			.container {
				color: #E8E9EB;
				background: #222;
				border: #555652 1px solid;
				padding: 10px;
			}
		    .button {
			    display: inline-block;
			    padding: 15px 25px;
			    font-size: 24px;
			    cursor: pointer;
			    text-align: center;
			    text-decoration: none;
			    outline: none;
			    color: #fff;
			    background-color: #222;
			    border: none;
			    border-radius: 15px;
			    box-shadow: 0 9px #999;
		    }

		    .button:hover {background-color: red}

		    .button:active {
			    background-color: red;
			    box-shadow: 0 5px #666;
			    transform: translateY(4px);
			}
		</style>

	</head>

	<body>   
	    <div class="container">	
	    <h1>Suivi de température</h1>     
			<canvas id="chart" style="width: 100%; height: 65vh; background: #222; border: 1px solid #555652; margin-top: 10px;"></canvas>
			<script>
				var ctx = document.getElementById("chart").getContext('2d');
    			var myChart = new Chart(ctx, {
        		type: 'line',
		        data: {
		            labels: [<?php echo $time; ?>],
		            datasets: 
		            [{
		            	label: 'Temperature',
		                data: [<?php echo $value; ?>],
			            backgroundColor: 'rgba(0, 255, 251, 0.25)',
			            borderColor: 'rgb(0, 255, 251)',
		                borderWidth: 3	
		            }]
		        },
		     
				options: {
					legend: {
            			display: true,
           				labels: {
                		fontColor: 'rgb(255, 255, 255)'
                		}
                	},
				    scales: {
            			yAxes: [{
                			ticks: {
                    			suggestedMin: 0,
                    			suggestedMax: 50,
                    			fontColor: 'rgba(255, 255, 255, 0.75)'
                			}
            			}],
            			xAxes: [{
                			ticks: {
                    			fontColor: 'rgba(255, 255, 255, 0.75)'
                			}
            			}]
        			}
    			}
			});
			</script>
	    </div>
	</body><br>
	<form>
	<button class=button type="button" onclick="document.location='http://192.168.1.46/test/datenter.php'">Tableau</button>
	<button class=button type="button" onclick="document.location='http://192.168.1.46/test/index.html'">Accueil</button>
	<button class=button type="button" onclick="document.location='http://192.168.1.46/test/jauge.php'">Jauge</button>		
		</form>
</html>

<?php
/*		            		yAxes: {
		            			min: 0,
		            			max: 50,
		            			ticks: {
		            				stepSize: 5
		            					}
		            				},
		            		 xAxes: {
		            		 	min: 0,
		            		 	max: 50,
		            		 	ticks: {
		            		 		stepSize: 1
		            		 			}
		            		 		}
		            		 	}

,
		            tooltips:{mode: 'index'},
		            legend:{display: true, position: 'top', labels: {fontColor: 'rgb(255,255,255)', fontSize: 16}}
		            		 	*/
		            		 	?>