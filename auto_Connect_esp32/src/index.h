#include <Arduino.h>
#include <WiFi.h>          // Replace with WiFi.h for ESP32
#include <WebServer.h>     // Replace with WebServer.h for ESP32
#include <AutoConnect.h>
WebServer Server;          // Replace with WebServer for ESP32
AutoConnect Portal(Server);

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
            box-sizing: border-box; /* Include padding in the width */
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
            /* border: 2px solid black; */
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
            width: 100px; /* Adjust the width and height to make it round */
            height: 100px;
            background-color: #007bff; /* Change this to your desired button color */
            color: #fff; /* Text color */
            border: none;
            border-radius: 50%; /* Makes the button round */
            text-align: center;
            font-size: 18px;
            line-height: 100px; /* Vertically centers text */
            text-decoration: none;
            cursor: pointer;
        }
        .round-buttonLeft {
            display: inline-block;
            width: 100px; /* Adjust the width and height to make it round */
            height: 100px;
            background-color: #007bff; /* Change this to your desired button color */
            color: #fff; /* Text color */
            border: none;
            border-radius: 50%; /* Makes the button round */
            text-align: center;
            font-size: 18px;
            line-height: 100px; /* Vertically centers text */
            text-decoration: none;
            cursor: pointer;
        }
    </style>
</head>
<body>

<div class="container" id="first">
<p>Total</p>
    <input type="text" class="textbox" placeholder="Top Center" id="one" name="one">   
    </div>

    <div class="container" id="topCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="two" name="two">
        <p>UP</p>
    </div>

    <div class="container" id="leftCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="three" name="three">
        <p>Left</p>
    </div>

    <div class="container" id="rightCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="four" name="four">
        <p>Right</p>
    </div>

    <div class="container" id="bottomCenter">
    <input type="text" class="textbox" placeholder="Top Center" id="five" name="five">
        <p>Down</p>
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
                button.style.backgroundColor = "red"; /* Change the button color when toggled */
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
        var total = myObj.total;
        var up = myObj.up;
        var down = myObj.down;
        var left = myObj.left;
        var right = myObj.right;
        one.value = total;
        two.value = up;
        five.value = down;
        three.value = left;
        four.value = right;
        
      }
    };
    xhr.open("GET", "/readings", true);
    xhr.send();
  }

  // Call the function initially
  fetchData();

  // Set up an interval to call the function every 1 second (1000 milliseconds)
  setInterval(fetchData, 3000);
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