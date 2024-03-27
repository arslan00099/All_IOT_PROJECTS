 
// Function to validate IP address format
function isValidIP(ip) {
  const ipRegex = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
  const matches = ip.match(ipRegex);
  return matches !== null && matches.slice(1).every(octet => octet >= 0 && octet <= 255);
}

// Function to handle form submission
function handleSubmit(event) {
  event.preventDefault();
  event.stopPropagation();
  
  const form = event.currentTarget;
  if (!form.checkValidity()) {
    form.classList.add('was-validated');
    return;
  }
  
  // Get the input values
  const ip = document.getElementById('ip').value;
  const gateway = document.getElementById('gateway').value;
  const subnet = document.getElementById('subnet').value;
  
  // Validate IP address format
  if (!isValidIP(ip) || !isValidIP(gateway) || !isValidIP(subnet)) {
    alert('Invalid IP address or subnet mask format!');
    return;
  }
  
  // TODO: Add code to send the DHCP configuration to the device (via API or other method)
  // For demonstration purposes, we'll just display a success message here.
  document.getElementById('status').textContent = 'Configuration saved successfully!';
}

// Add event listener for form submission
document.getElementById('dhcpConfigForm').addEventListener('submit', handleSubmit);
