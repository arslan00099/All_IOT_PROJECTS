#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <AsyncWebServer.h>


const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<head>
  <title>Toggle Switches</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
      margin: 10px;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: grey;
      -webkit-transition: .4s;
      transition: .4s;
      border-radius: 34px;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .4s;
      transition: .4s;
      border-radius: 50%;
    }

    input:checked + .slider {
      background-color: green;
    }

    input:checked + .slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }

    .red input:checked + .slider {
      background-color: red;
    }

    .red input:checked + .slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }

    .yellow input:checked + .slider {
      background-color: yellow;
    }

    .yellow input:checked + .slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }
    button {
  padding: 10px 20px;
  color: white;
  border: none;
  cursor: pointer;
  margin-right: 10px;
}
.circularbutton {
      display: inline-block;
      border:2px solid black;
      width: 50px;
      height: 50px;
      border-radius: 50%;
      background-color: #ccc;
      text-align: center;
      line-height: 50px;
      font-size: 20px;
      cursor: pointer;
    }
  </style>
</head>
<body>
  <center>
<h2>TITLE 1</h2><br>
<label ><p id="titletwo"></p>Title2 - ON</label>  <label style="margin-left:550px" id="titlethree">Title3 - OFF</label><br>
 <button id="ledf1" style=" border:2px solid red;">LED_F1</button>
   <button  id="ledf2" style="margin-left:520px; border:2px solid red;">LED_F2</button><br><br><br><br><br>
   <br>
   <div>  
  <lable>Title8</labe><br>
    <button class="circularbutton"  id="upButton"></button>
  </div>
  <div>  <lable>Title6</labe>   <lable style="margin-left:30px">Title4</labe><lable style="margin-left:480px">Title5</labe><lable style="margin-left:30px">Title7</labe><br>

  <button  id="ledc"  style="border:2px solid red;">LED_C</button>
  <button  id="lede1" style="margin-left:10px; border:2px solid red;" >LED_E1</button>
    <button class="circularbutton" style="margin-left: 170px;" id="downButton"></button>
    <button  id="lede2" style="margin-left: 170px; border:2px solid red;">LED_E2</button>
  <button  id="ledm" style="margin-left:10px; border:2px solid yellow;" >LED_M</button><br>
  <lable>Title9</labe>
  </div>
 <br><br><br><br>
    </center>
    <div style="margin-left: 50px;">
    <label class="switch red">
      <input type="checkbox" id="toggle1" onchange="toggleSwitch(1)">
      <span class="slider round"></span>
    </label>
    <label for="toggle1">LED_F1 LED_F2 LED_C + Buzzer</label>
  </div>
  <div style="margin-left: 50px;">
    <label class="switch red">
      <input type="checkbox" id="toggle2" onchange="toggleSwitch(2)">
      <span class="slider round"></span>
    </label>
    <label for="toggle2">LED_E1 LED_E2 + Buzzer</label>
  </div>
  <div style="margin-left: 50px;">
    <label class="switch yellow">
      <input type="checkbox" id="toggle3" onchange="toggleSwitch(3)">
      <span class="slider round"></span>
    </label>
    <label for="toggle3">LED_M + Buzzer</label>
  </div>

</body>
<script>
  var switches = [false, false, false]; // Tracks the state of the switches

  function toggleSwitch(switchIndex) {
    switches[switchIndex - 1] = !switches[switchIndex - 1]; // Toggle the switch state

    // Update switch classes based on state
    for (var i = 0; i < switches.length; i++) {
      var toggle = document.getElementById('toggle' + (i + 1));
      toggle.parentNode.classList.remove('red', 'yellow');

      if (switches[i]) {
        if (i === 2) {
          toggle.parentNode.classList.add('yellow');
        } else {
          toggle.parentNode.classList.add('red');
        }
      }
    }

    if (switches[switchIndex - 1]) {
      switch (switchIndex) {
        case 1:
          // Execute function for switch 1 when it is toggled ON
          functionForSwitch1On();
          break;
        case 2:
          // Execute function for switch 2 when it is toggled ON
          functionForSwitch2On();
          break;
        case 3:
          // Execute function for switch 3 when it is toggled ON
          functionForSwitch3On();
          break;
        default:
          break;
      }
    } else {
      switch (switchIndex) {
        case 1:
          // Execute function for switch 1 when it is toggled OFF
          functionForSwitch1Off();
          break;
        case 2:
          // Execute function for switch 2 when it is toggled OFF
          functionForSwitch2Off();
          break;
        case 3:
          // Execute function for switch 3 when it is toggled OFF
          functionForSwitch3Off();
          break;
        default:
          break;
      }
    }
  }

  // Functions to be executed when each switch is toggled ON
  function functionForSwitch1On() {
    console.log("Switch 1 is ON");
    var a = "toggleone";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=ON" , true);
xhr.send();
  }

  function functionForSwitch2On() {
    console.log("Switch 2 is ON");
    var a = "toggletwo";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=ON" , true);
xhr.send();

  }

  function functionForSwitch3On() {
    console.log("Switch 3 is ON");
    var a = "togglethree";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=ON" , true);
xhr.send();
  }

  // Functions to be executed when each switch is toggled OFF
  function functionForSwitch1Off() {
    console.log("Switch 1 is OFF");
    var a = "toggleone";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=OFF" , true);
xhr.send();
  }

  function functionForSwitch2Off() {
    console.log("Switch 2 is OFF");
    var a = "toggletwo";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=OFF" , true);
xhr.send();
  }

  function functionForSwitch3Off() {
    console.log("Switch 3 is OFF");
    var a = "togglethree";
var xhr = new XMLHttpRequest();
xhr.open("GET", "/DATA?output=" + a + "&state=OFF" , true);
xhr.send();
  }


  function getData() {
                  var xhttp = new XMLHttpRequest();
                  xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                      var myObj = JSON.parse(this.responseText);
                      console.log(myObj);
                    
                      var ledfone = myObj.ledf1;
                      var ledftwo = myObj.ledf2;
                      var ledeone=myObj.lede1;
                      var ledetwo=myObj.lede2;
                      var up=myObj.round1;
                      var down=myObj.round2;
                       var ledcc=myObj.ledc;
                       var ledmm=myObj.ledm;
                      // var titlewo = myObj.title2;
                      // var titlethree = myObj.title3;
                     
                      // var ttwo = document.getElementById('titletwo');
                      // var tthree = document.getElementById('titlethree');
                      // ttwo.value= titlewo;
                      // tthree.value=titlethree;



                      var button = document.getElementById('ledf1');
                      if(ledfone == 1){
                        button.style.backgroundColor = 'red';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                      var button = document.getElementById('ledf2');
                      if(ledftwo == 1){
                        button.style.backgroundColor = 'red';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                      var button = document.getElementById('lede1');
                      if(ledeone == 1){
                        button.style.backgroundColor = 'red';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                      var button = document.getElementById('lede2');
                      if(ledetwo == 1){
                        button.style.backgroundColor = 'red';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                      var button = document.getElementById('upButton');
                      if(up == 1){
                        button.style.backgroundColor = 'blue';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                      var button = document.getElementById('downButton');
                      if(down == 1){
                        button.style.backgroundColor = 'blue';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                      var button = document.getElementById('ledc');
                      if(ledcc == 1){
                        button.style.backgroundColor = 'red';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                      var button = document.getElementById('ledm');
                      if(ledmm == 1){
                        button.style.backgroundColor = 'yellow';
                      }
                      else{
                        button.style.backgroundColor = '#ccc';
                      }

                     
                    }
                  };
                  xhttp.open("GET", "/getledstatus", true);
                  xhttp.send();
                }
                setInterval(function () {
                  getData();
                }, 500);

  </script>
</html>

)rawliteral";