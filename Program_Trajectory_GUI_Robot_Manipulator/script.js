let port;
let writer;
let isConnected = false;


async function connectToSerial() {
  try {
    port = await navigator.serial.requestPort();  
    await port.open({ baudRate: 115200 });  
    writer = port.writable.getWriter();
    document.getElementById('status').textContent = 'Connected to OpenCM 9.04';
    isConnected = true;
  } catch (error) {
    console.error('Connection failed:', error);
    document.getElementById('status').textContent = 'Failed to connect. Please try again.';
  }
}


async function sendDataToOpenCM(data) {
  if (!isConnected) {
    document.getElementById('status').textContent = 'Not connected. Please connect first.';
    return;
  }

  try {
    const encoder = new TextEncoder();
    await writer.write(encoder.encode(data + '\n'));  
    document.getElementById('status').textContent = 'Data sent successfully!';
  } catch (error) {
    console.error('Failed to send data:', error);
    document.getElementById('status').textContent = 'Failed to send data.';
  }
}


document.getElementById('control-form').addEventListener('submit', function (event) {
  event.preventDefault();

  // ini tuh mengambil data input dari HTML yang sudah dibuat
  const startPos1 = parseFloat(document.getElementById('startPos1').value);
  const startPos2 = parseFloat(document.getElementById('startPos2').value);
  const endPos1 = parseFloat(document.getElementById('endPos1').value);
  const endPos2 = parseFloat(document.getElementById('endPos2').value);
  const velocity = parseFloat(document.getElementById('velocity').value);
  const trajectory = parseInt(document.getElementById('trajectory').value);
  const trajectoryDelay = parseInt(document.getElementById('trajectoryDelay').value);

  const data = `startPos1:${startPos1},startPos2:${startPos2},endPos1:${endPos1},endPos2:${endPos2},velocity:${velocity},trajectory:${trajectory},trajectoryDelay:${trajectoryDelay}\n`;

  sendDataToOpenCM(data);
});

document.getElementById('connect-btn').addEventListener('click', connectToSerial);
