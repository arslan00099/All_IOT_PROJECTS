#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <Arduino_JSON.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><!DOCTYPE HTML>
<!DOCTYPE HTML>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            background-image: url('bg');
            background-size: cover;
            background-position: center;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin-top: 100px;
        }

        button {
            color: #fff !important;
            text-transform: uppercase;
            text-decoration: none;
            background: #000;
            border: 2px solid #ffffff;
            padding: 10px;
            border-radius: 30px;
            display: inline-block;
            border: 2px solid #ffffff;
            transition: all 0.4s ease 0s;
            width: 500px;
            font-size: 40px;
            font-family: "Times New Roman", Times, serif;
            font-weight: bold;
        }

        lockbutton {
            color: #fff !important;
            text-transform: uppercase;
            text-decoration: none;
            background: #000;
            border: 2px solid #ffffff;
            padding: 10px;
            border-radius: 30px;
            display: inline-block;
            border: 2px solid #ffffff;
            transition: all 0.4s ease 0s;
          
            font-size: 20px;
            font-family: "Times New Roman", Times, serif;
            font-weight: bold;
        }

        button:hover {
            text-shadow: 0px 0px 6px rgba(255, 255, 255, 1);
            -webkit-box-shadow: 0px 5px 40px -10px rgba(0, 0, 0, 0.57);
            -moz-box-shadow: 0px 5px 40px -10px rgba(0, 0, 0, 0.57);
            transition: all 0.4s ease 0s;
        }

        .active_state {
            background: #FF0000;
        }

          .inactive_state {
            background: #000;
        }

        
    </style>
</head>
<body>
    <center>
        <button name="one" id="one" value="" class="antenna_button inactive_state" onclick="toggleActiveState(this, 'ANTENNA1')" style=" margin-top: 80px;">loading...</button><br><br>
        <button name="two" id="two" value="" class="antenna_button inactive_state" onclick="toggleActiveState(this, 'ANTENNA2')" >loading...</button><br><br>
        <button name="three" id="three" value="" class="antenna_button inactive_state" onclick="toggleActiveState(this, 'ANTENNA3')" >loading...</button><br><br>
        <button name="four" id="four" value="" class="antenna_button inactive_state" onclick="toggleActiveState(this, 'ANTENNA4')" >loading...</button><br><br>
         </center>
    <img id="settingImage" src="setting" style="margin-left: 50px; margin-top:0px; cursor: pointer;" onclick="onSettingImageClick()"><br><br>
   <button name="toggleButton" id="toggleButton" class="lockbutton" onclick="toggleAllButtons()" style="margin-left: 50px;padding: 10px; background-color: #3498db; color: #fff; border: none; cursor: pointer; font-size: 16px; border-radius: 5px; width:90px;margin-bottom:10px;">Lock</button>
 <button name="nextpage" id="nextpage" class="lockbutton" onclick="nextpage()" style="margin-left: 50px;padding: 10px; background-color: #3498db; color: #fff; border: none; cursor: pointer; font-size: 16px; border-radius: 5px; width:90px;margin-bottom:10px;">Names</button><br><br>

    <script>
       function toggleActiveState(clickedButton, buttonName) {
    // Reset all buttons to inactive_state
    var buttons = document.querySelectorAll('.antenna_button');
    buttons.forEach(function (button) {
        button.style.backgroundColor = "black";
        button.classList.remove('active_state');
        button.classList.add('inactive_state');
    });

    // Toggle the state of the clicked button
    if (clickedButton.classList.contains('inactive_state')) {
        clickedButton.style.backgroundColor = "red"; // Set the background color to red for the clicked button
        clickedButton.classList.remove('inactive_state');
        clickedButton.classList.add('active_state');
    } else {
        clickedButton.style.backgroundColor = "black"; // Reset the background color for the clicked button
        clickedButton.classList.remove('active_state');
        clickedButton.classList.add('inactive_state');
    }

    // Log which button is clicked
    console.log(buttonName + " is clicked");

    // Your additional code here...
    var a = "message";
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/DATA?output=" + a + "&state=" + buttonName, true);
    xhr.send();
}

        function onSettingImageClick() {
            // Add your logic for the image click event here
            console.log("Setting image clicked!");
                  var xhr = new XMLHttpRequest();
                setTimeout(function () { window.open("/wifi_open", "_self"); }, 1000);
        }

          function nextpage() {
            // Add your logic for the image click event here
            console.log("NEXT page click!");
                  var xhr = new XMLHttpRequest();
                setTimeout(function () { window.open("/names", "_self"); }, 1000);
        }

        window.onload = function () {
   
    secondFunction();
    firstFunction();
};

function firstFunction() {
    console.log("Executing first function");
    var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function () {
                  if (this.readyState == 4 && this.status == 200) {
                    var myObj = JSON.parse(this.responseText);
                    console.log(myObj);
                    var assid = myObj.ssid;
                    var pass = myObj.pass;
                    var onee =myObj.one;
                    var twoo=myObj.two;
                    var threee=myObj.three;
                    var fourr=myObj.four;                   
                    
                  document.getElementById("one").innerHTML = onee;
            document.getElementById("two").innerHTML = twoo;
            document.getElementById("three").innerHTML = threee;
            document.getElementById("four").innerHTML = fourr;


                  }
                };
                xhr.open("GET", "/getnames", true);
                xhr.send();
}

function secondFunction() {
    console.log("Executing second function");
     var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var myObj = JSON.parse(this.responseText);
            console.log(myObj);
            var aone =myObj.ANTENNA1;
             var atwo =myObj.ANTENNA2;
             var athree =myObj.ANTENNA3;
             var afour =myObj.ANTENNA4;
             if (aone==1){
 var button = document.getElementById("one");
  button.style.backgroundColor = "red";
    console.log('one 111');
  }
             if (atwo==1){
 var button = document.getElementById("two");
  button.style.backgroundColor = "red";
  }
           if (athree==1){
 var button = document.getElementById("three");
  button.style.backgroundColor = "red";
  }
           if (afour==1){
 var button = document.getElementById("four");
  button.style.backgroundColor = "red";
  }
        }
    };

    xhr.open("GET", "/getbuttonstatus", true);
    xhr.send();
}

function toggleAllButtons() {
    var toggleButton = document.getElementById('toggleButton');
    var buttons = document.querySelectorAll('.antenna_button');
    var settingImage = document.getElementById('settingImage');
    
    if (toggleButton.classList.contains('active_state')) {
        // Deactivate the toggle button and enable all other buttons
        toggleButton.classList.remove('active_state');
        toggleButton.classList.add('inactive_state');
        buttons.forEach(function (button) {
            button.disabled = false;
        });
          settingImage.removeAttribute('disabled');
        toggleButton.innerText = 'Lock';
    } else {
        // Activate the toggle button and disable all other buttons
        toggleButton.classList.remove('inactive_state');
        toggleButton.classList.add('active_state');
        buttons.forEach(function (button) {
            button.disabled = true;
        });
           settingImage.setAttribute('disabled', 'disabled');
        toggleButton.innerText = 'Unlock';
    }
}


    </script>
</body>
</html>

)rawliteral";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//############################################################################################################

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

 const char wifi_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
    <html>

    <head>
      <title>ESP Web Server</title>
      <meta name="apple-mobile-web-app-capable" content="yes">
      <meta content="text/html; charset=utf-8" http-equiv="content-type">
      <meta name="viewport" content="width=device-width, intial-scale=1">
      <style>
        html {
          font-family: Arial;
          display: inline-block;
          text-align: center;
        }

        body {
            background-image: url('bg');
            background-size: cover;
            height: 100vh;
            margin-top: 100px;
        }


      

        .card-grid {
          max-width: 400px;
          margin: 0 auto;
          display: grid;

          grid-gap: 2rem;
          grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
        }

        .card {
          
          border-radius: 10px;
          margin-top:110px;
          box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
        }

        .maindiv {
          background-color: white;
          margin-top: 20px;
          border-radius: 40px;
          width: 550px;
          box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
        }

.settings-image {
      margin-right: 900px;
      cursor: pointer;
    }

    .settings-content {
      padding: 20px; /* Add padding for better spacing */
       flex-grow: 1;
    }


      
      </style>
    
      

    </head>
  


    <body>
         
        <center>      </br><br><br><br>
<label style="padding-right: 10px; font-weight: bold; color: #FFFFFF; font-size: 40px; font-family: 'Times New Roman', Times, serif;">SSID</label><br>
<input type="text" name="name" id="ssid" value="" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 40px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;" /><br>

<label style="padding-right: 10px; font-weight: bold; color: #FFFFFF; font-size: 40px; font-family: 'Times New Roman', Times, serif;">PASSWORD</label><br>
<input type="text" name="name" id="passwordd" value="" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 40px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;" /><br><br>

<button onclick="postdata();" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 40px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;"><b>SAVE</b></button>
   </center>
        <div class="settings-image">
      <img id="settingImage" src="back" onclick="onSettingImageClick()">
  

     <label id="ssidLabel" style="padding-right: 10px; font-weight: bold; color: #FFFFFF; font-size: 40px; font-family: 'Times New Roman', Times, serif; margin-left:1000px;"></label>
  </div>


            <script type="text/javascript">

             function postdata() {
                var ssidvalue = document.getElementById("ssid").value;
                var a = "SSID";
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/DATA?output=" + a + "&state=" + ssidvalue, true);
                xhr.send();
                var passwordvalue = document.getElementById("passwordd").value;
                var b = "PASSWORD";
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/DATA?output=" + b + "&state=" + passwordvalue, true);
                xhr.send();
              }

              window.onload = function () {
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function () {
                  if (this.readyState == 4 && this.status == 200) {
                    var myObj = JSON.parse(this.responseText);
                    console.log(myObj);
                    var assid = myObj.ssid;
                    var pass = myObj.pass;
                    var ipp=myObj.ip;

                    ssid.value = assid;
                    passwordd.value = pass;
 ssidLabel.textContent =ipp;
                  }
                };
                xhr.open("GET", "/readings", true);
                xhr.send();
              }

               function onSettingImageClick() {
            // Add your logic for the image click event here
            console.log("BACK clicked");
                  var xhr = new XMLHttpRequest();
                setTimeout(function () { window.open("/", "_self"); }, 1000);
        }

            </script>
    </body>
   
   

    </html>
    )rawliteral";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//############################################################################################################

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

 const char names_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
    <html>

    <head>
      <title>ESP Web Server</title>
      <meta name="apple-mobile-web-app-capable" content="yes">
      <meta content="text/html; charset=utf-8" http-equiv="content-type">
      <meta name="viewport" content="width=device-width, intial-scale=1">
      <style>
        html {
          font-family: Arial;
          display: inline-block;
          text-align: center;
        }

        body {
            background-image: url('bg');
            background-size: cover;
            background-position: center;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin-top: 100px;
        }


      

        .card-grid {
          max-width: 400px;
          margin: 0 auto;
          display: grid;

          grid-gap: 2rem;
          grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
        }

        .card {
          background-color: white;
          border-radius: 10px;
          margin-top:110px;
          box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
        }

        .maindiv {
          background-color: white;
          margin-top: 20px;
          border-radius: 40px;
          width: 550px;
          box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
        }

      
      </style>
      <center>
      

    </head>
  


    <body><br><br><br><br><br>

      <label style="padding-right: 10px; font-weight: bold; color: #FFFFFF; font-size: 20px; font-family: 'Times New Roman', Times, serif; margin-top:100px;">Antena 1 :</label>
<input type="text" name="one" id="one" value="" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 20px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;" /><br><br>

      <label style="padding-right: 10px; font-weight: bold; color: #FFFFFF; font-size: 20px; font-family: 'Times New Roman', Times, serif;">Antena 2 :</label>
<input type="text" name="two" id="two" value="" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 20px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;" /><br><br>

              <label style="padding-right: 10px; font-weight: bold; color: #FFFFFF; font-size: 20px; font-family: 'Times New Roman', Times, serif;">Antena 3 :</label>
<input type="text" name="three" id="three" value="" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 20px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;" /><br><br>
              
               <label style="padding-right: 10px; font-weight: bold; color: #FFFFFF; font-size: 20px; font-family: 'Times New Roman', Times, serif;">Antena 4 :</label>
<input type="text" name="four" id="four" value="" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 20px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;" /><br><br>
           
           
          </br>
          <button onclick="savenames();" style="padding-right: 10px; font-weight: bold; color: #fff; font-size: 30px; font-family: 'Times New Roman', Times, serif; background: #000; border: 2px solid #ffffff; border-radius: 10px; padding: 10px;"><b>SAVE</b></button>
 
           




            <script type="text/javascript">

              function savenames() {
                var onee = document.getElementById("one").value;
                var twoo = document.getElementById("two").value;
                var three = document.getElementById("three").value;
                var fourr = document.getElementById("four").value;
                
                var myJSON = {
        "one": onee,
        "two": twoo,
        "three": three,
        "four": fourr,
    };
     var jsonString = JSON.stringify(myJSON);

                var b = "names";
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/DATA?output=" + b + "&state=" + jsonString, true);
                xhr.send();


              }

              window.onload = function () {
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function () {
                  if (this.readyState == 4 && this.status == 200) {
                    var myObj = JSON.parse(this.responseText);
                    console.log(myObj);
                    var assid = myObj.ssid;
                    var pass = myObj.pass;
                    var onee =myObj.one;
                    var twoo=myObj.two;
                    var threee=myObj.three;
                    var fourr=myObj.four;                  
                    
                    one.value = onee;
                    two.value= twoo;
                    three.value=threee;
                    four.value=fourr;


                  }
                };
                xhr.open("GET", "/getnames", true);
                xhr.send();
              }

               function onSettingImageClick() {
            // Add your logic for the image click event here
            console.log("BACK clicked");
                  var xhr = new XMLHttpRequest();
                setTimeout(function () { window.open("/", "_self"); }, 1000);
        }

            </script>

          <br><br>
         </center>
        <img id="settingImage" src="back" style="margin-left: 50px; cursor: pointer;" onclick="onSettingImageClick()">
 

    </body>
   
   

    </html>
    )rawliteral";