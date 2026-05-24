arduino-cli compile --upload -p /dev/cu.usbserial-0001 -b esp32:esp32:esp32
echo "compile completed, upload completed, monitoring..."

arduino-cli monitor -p /dev/cu.usbserial-0001  --config 115200
