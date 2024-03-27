// Importing necessary libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>



//html and css code starts here
const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE HTML><html>
<head>
  <title>TITLE 1</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 60px; height: 30px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius:50px}
    .slider:before {position: absolute; content: ""; height: 30px; width: 30px; left: 5px; ; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 50px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(20px); -ms-transform: translateX(20px); transform: translateX(20px)}
 button {
  padding: 10px 20px;
  color: white;
  border: none;
  cursor: pointer;
  margin-right: 10px;
}


  </style>
</head>
<body>
  <h2>TITLE 1</h2>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <lable>LED A</labe> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <lable>LED U</labe><br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <button id="btn1">LED A</button>
   <button  id="btn4" >LED U</button><br><br>
     <lable>LED B</labe> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <lable>LED C</labe>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br>
  <button  id="btn2" >LED B</button>
  <button  id="btn3" >LED C</button>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
 <br><br><br><br>
    <lable>Title 5</labe>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <lable>Title 4</labe><br>
 <button id="btn5"  style="border-radius: 80px; width:50px;height:50px;"></button>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;


    %BUTTONPLACEHOLDER%
 

<script>
// togglecheckbox function is used to send the text on change slider postion 
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}

// this function will be repeated to get he json data about the status of leds from esp32 and also check the conditon is led status is 1 color will be red if 0 color will be green if 2 color will be grey
window.onload = function () {
  // Define a function to make the XMLHttpRequest
  function makeRequest() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var myObj = JSON.parse(this.responseText);
        console.log(myObj);
        //decoding json keys 
        var led1Status = myObj.led1;
      var led2Status = myObj.led2;
      var led3Status = myObj.led3;
      var led4Status = myObj.led4;
      var led5Status = myObj.led5;
           var status = myObj.slider;

        // Access the button element
      var button = document.getElementById('btn1');

      // Change the button color based on led1Status
      if (led1Status == 1) {
        button.style.backgroundColor = 'red';
      }
      else if(led1Status == 2){ button.style.backgroundColor = '#ccc';} else {
        button.style.backgroundColor = 'green';
      }

            var button = document.getElementById('btn2');

      // Change the button color based on led2Status
      if (led2Status == 1) {
        button.style.backgroundColor = 'red';
      } else if(led2Status == 2){ button.style.backgroundColor = '#ccc';} else {
        button.style.backgroundColor = 'green';
      }

            var button = document.getElementById('btn3');

      // Change the button color based on led3Status
      if (led3Status == 1) {
        button.style.backgroundColor = 'red';
      }  else if(led3Status == 2){ button.style.backgroundColor = '#ccc';}else {
        button.style.backgroundColor = 'green';
      }

      var button = document.getElementById('btn4');

      // Change the button color based on led4Status
      if (led4Status == 1) {
        button.style.backgroundColor = 'red';
      }  else if(led4Status == 2){ button.style.backgroundColor = '#ccc';}else {
        button.style.backgroundColor = 'green';
      }


          // Access the button element
      var button = document.getElementById('btn5');
         var slider = document.getElementById('5');

      // Change the button color based on led5Status
      if (led5Status == 1) {
        button.style.backgroundColor = 'yellow';
      } else {
        button.style.backgroundColor = '#ccc';
      }
      if(status == 1){
           document.getElementById("5").checked = false;
           }

      // Use the retrieved values as needed
      console.log("LED1 Status: " + led1Status);
      console.log("LED2 Status: " + led2Status);
      console.log("LED3 Status: " + led3Status);
      console.log("LED4 Status: " + led4Status);
      }
    };
    xhr.open("GET", "/getledstatus", true);
    xhr.send();
  }

  // Call the function immediately
  makeRequest();

  // Repeat the function every second (1000 milliseconds)
  setInterval(makeRequest, 500);
};

</script>
</body>
</html>
)rawliteral";