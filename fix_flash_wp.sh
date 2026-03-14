#!/bin/bash
# Fix ESP32 Boya flash write-protection issue
# Run this before uploading if you get MD5 mismatch errors

ESPTOOL="/c/Users/AVENGER/AppData/Local/Arduino15/packages/esp32/tools/esptool_py/5.1.0/esptool.exe"
PORT="COM4"

echo "=== Reading flash status ==="
"$ESPTOOL" --port $PORT read-flash-status --bytes 2

echo ""
echo "=== Clearing write-protect bits ==="
"$ESPTOOL" --port $PORT write-flash-status --bytes 2 0x0200

echo ""
echo "=== Verifying fix ==="
"$ESPTOOL" --port $PORT read-flash-status --bytes 2

echo ""
echo "=== Erasing flash ==="
"$ESPTOOL" --port $PORT erase-flash

echo ""
echo "Done! You can now upload from Arduino IDE."
