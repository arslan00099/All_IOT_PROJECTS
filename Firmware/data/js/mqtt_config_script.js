document.addEventListener("DOMContentLoaded", function () {
  // Form submission event listener
  document.getElementById("mqttConfigForm").addEventListener("submit", function (event) {
    event.preventDefault();
    event.stopPropagation();
    if (this.checkValidity()) {
      saveMQTTConfig();
    } else {
      this.classList.add("was-validated");
    }
  });

  // Function to save MQTT Configuration
  function saveMQTTConfig() {
    const mqttConfig = {
      host: document.getElementById("host").value,
      port: parseInt(document.getElementById("port").value),
      topics: document.getElementById("topics").value.split(","),
      keep_alive: parseInt(document.getElementById("keepAlive").value),
      clean_session: document.getElementById("cleanSession").value === "true",
      timeout: parseInt(document.getElementById("timeout").value),
    };

    // You can now send this MQTT configuration to the device using your preferred communication method (e.g., HTTP POST or MQTT message).

    // Display success message
    const statusDiv = document.getElementById("status");
    statusDiv.textContent = "MQTT Configuration saved successfully.";
    statusDiv.style.color = "green";

    // Clear the status message after 3 seconds
    setTimeout(function () {
      statusDiv.textContent = "";
      statusDiv.style.color = "";
    }, 3000);
  }
});
 
