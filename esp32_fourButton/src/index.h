#include <Arduino.h>
#include <WiFi.h>
 #include <ESPAsyncWebSrv.h>
#define pesoMorto_av 1250  // because it is made of two cells dx sx 
#define pesoMorto_sxdx 2500
#define pesoMorto 2500
#define updateInterval 500

//const char* ssid = "Almimaar-2022";
//const char* password = "2006-1954-1966";
const char* ssid = "TP-Link_Extender";
const char* password = "19541966";
// Define the pins for the HX711 modules for each load cell
const int dtPin_peso = 32;  // DT of peso
const int sckPin_peso = 33; // SCK of peso

const int dtPin_lat = 17;   // DT pin laterale
const int sckPin_lat = 5;   // SCK pin laterale

const int dtPin_as = 19;    // DT of avanti sinistra
const int sckPin_as = 18;   // SCK of avanti sinistra

const int dtPin_ad = 25;    // DT of avanti dx
const int sckPin_ad = 26;   // SCK of avanti dx

const char* htmlContent = R"(
<!DOCTYPE html>
<html>
<head>
    <style>
        body {
            margin: 0;
            padding: 0;
            font-family: Arial, sans-serif;
            background-color: #6392c2;
        }

        .textbox {
            width: 100%;
            padding: 5px;
            box-sizing: border-box; / Include padding in the width /
        }

        @media (min-width: 768px) {
            .container {
                width: 45%;
                margin: 20px;
            }
        }

        @media (min-width: 1200px) {
            .container {
                width: 30%;
            }
        }
        .container {
            position: absolute;
            width: 180px;
            / border: 2px solid black; /
            border-radius: 10px;
            padding: 20px;
            text-align: center;
        }

        #topCenter {
            top: 20px;
            left: 50%;
            transform: translateX(-50%);
        }

        #TopRight {
            top: 20px;
            left: 95%;
            transform: translateX(-95%);
        }

        #first {
            top: 5px;
            left: 1%;
            transform: translateX(-1%);
        }


        #leftCenter {
            top: 50%;
            left: 20px;
            transform: translateY(-50%);
        }

        #rightCenter {
            top: 50%;
            right: 20px;
            transform: translateY(-50%);
        }

        #bottomCenter {
            bottom: 20px;
            left: 50%;
            transform: translateX(-50%);
        }
        #bottomRight {
            bottom: 20px;
            left: 90%;
            transform: translateX(-90%);
        }
     
        #bottomLeftMost {
            
            bottom: 0px;
            left: 0%;
            transform: translateX(-0%);
        }

        #bottomLeft {
            bottom: 20px;
            left: 10%;
            transform: translateX(-10%);
        }

        .round-buttonRight {
            display: inline-block;
            width: 100px; / Adjust the width and height to make it round /
            height: 100px;
            background-color: #007bff; / Change this to your desired button color /
            color: #fff; / Text color /
            border: none;
            border-radius: 50%; / Makes the button round /
            text-align: center;
            font-size: 18px;
            line-height: 100px; / Vertically centers text /
            text-decoration: none;
            cursor: pointer;
        }
        .round-buttonLeft {
            display: inline-block;
            width: 100px; / Adjust the width and height to make it round /
            height: 100px;
            background-color: #007bff; / Change this to your desired button color /
            color: #fff; / Text color /
            border: none;
            border-radius: 50%; / Makes the button round /
            text-align: center;
            font-size: 18px;
            line-height: 100px; / Vertically centers text /
            text-decoration: none;
            cursor: pointer;
        }
    </style>
</head>
<body>

<div class="container" id="first">
<p>Peso</p>
    <input type="text" class="textbox" placeholder="Top Center" id="one" name="one">   
    </div>

    <div class="container" id="topCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="two" name="two">
        <p>av</p>
    </div>

    <div class="container" id="leftCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="three" name="three">
        <p>latsx</p>
    </div>

    <div class="container" id="rightCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="four" name="four">
        <p>latdx</p>
    </div>

    <div class="container" id="bottomCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="five" name="five">
        <p>retr</p>
    </div>
    <div class="container" id="bottomRight">
    <a href="#" class="round-buttonRight" id="toggleButtonRight">Start</a>
    </div>
    <div class="container" id="bottomLeft">
    <a href="#" class="round-buttonLeft" id="toggleButtonLeft">Tune</a>
    </div>
    <div class="container" id="bottomLeftMost">
    <label>Units</label>
    </div>
    <div class="container" id="TopRight">
    <label>Nur Engineering</label>
    </div>
</body>


<script>
        const button = document.getElementById("toggleButtonRight");

        button.addEventListener("click", function () {
            if (button.innerHTML === "Start") {
                button.innerHTML = "Stop";
                button.style.backgroundColor = "red"; / Change the button color when toggled /
                startdata();
            } else {
                button.innerHTML = "Start";
                button.style.backgroundColor = "#007bff";
                stopdata();
            }
        });



window.onload = function () {
  // Create a function to fetch the data
  function fetchData() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var myObj = JSON.parse(this.responseText);
        console.log(myObj);
        var peso = myObj.peso;
        var av = myObj.av;
        var retr = myObj.retr;
        var latsx = myObj.latsx;
        var latdx = myObj.latdx;
        one.value = peso;
        two.value = av;
        five.value = retr;
        three.value = latsx;
        four.value = latdx;
        
      }
    };
    xhr.open("GET", "/readings", true);
    xhr.send();
  }

  // Call the function initially
  fetchData();

  // Set up an interval to call the function every 1 second (1000 milliseconds)
  setInterval(fetchData, 1000);
};

function startdata() {
var a = "start";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=Start"  , true);
xhr.send();
}

function stopdata() {
var a = "stop";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=Start"  , true);
xhr.send();
}


    </script>
</html>
)";