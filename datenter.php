<?php
$url=$_SERVER['REQUEST_URI'];
header("Refresh: 5; URL=$url");  // Refresh the webpage every 5 seconds
?>

<html>
<head>
    <meta name="author" content="Clement-XVII">
    <title>Sensor temp DHT22</title>
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

    table, th, td {
        border: 1px solid gray;
        background: #555652;
        opacity: 0.9;
        }

    table{
        width: 90%;
        text-align: center;
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
        <h1>Température en direct de la journée</h1>
    <table border="0" cellspacing="0" cellpadding="4">
      <tr>
            <th>DATE</th>
            <th>TEMPERATURE</th>
      </tr>
      
    <?php
    // Connect to database
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "test";
    $conn = mysqli_connect($servername, $username, $password, $dbname);
// Check connection
    if (!$conn) {
        die("Connection failed: " . mysqli_connect_error());
    }
       
    // Retrieve all records and display them   
    //$sql = "SELECT * FROM sensor WHERE sensor.time BETWEEN '2021-01-01 00:00:00' AND '2021-12-31 23:59:59' ORDER BY id DESC LIMIT 24";
    $sql = "SELECT * FROM sensor WHERE sensor.time BETWEEN CURRENT_DATE() AND CURRENT_DATE()+1 ORDER BY id DESC";
    //$sql = "SELECT time, value FROM sensor ORDER BY id DESC LIMIT 5";

    $result = mysqli_query($conn, $sql);
    // Process every record
    
    while($row = mysqli_fetch_array($result))
    {      
        echo "<tr>";
        echo "<td>" . $row['time'] . "</td>";
        echo "<td>" . $row['value'] . "</td>";
        echo "</tr>";
        
    }
        
    // Close the connection   
    mysqli_close($conn);
    ?>
    </table>
    <br>
    </body>
    <form>
    <button class=button type="button" onclick="document.location='http://192.168.1.46/test/index.html'">Accueil</button>
    <button class=button type="button" onclick="document.location='http://192.168.1.46/test/graph.php'">Graphique</button>
    <button class=button type="button" onclick="document.location='http://192.168.1.46/test/jauge.php'">Jauge</button>
    </form>
</html>
