
#Arduino Lib for ESP

https://github.com/bportaluri/WiFiEsp
https://github.com/bblanchon/ArduinoJson
https://wandbox.org/permlink/HBrUTPgJmauExToR

# Arduino uno R3
https://arduino-kit.ru/product/arduino-uno-r3-s-wi-fi-esp8266


https://www.deviceplus.com/how-tos/arduino-guide/esp8266-setup-tutorial-using-arduino/

#Flash ESP8266 AT commands flash (default)

```bash
# cd default_esp_flash/bin/at/512+512/

# esptool.py --port /dev/ttyUSB0 \
           --baud 115200 write_flash \
            --verify -fs 8m  0x00000 ../../boot_v1.6.bin \ 
                             0x01000 user1.1024.new.2.bin \ 
                             0x81000 user2.1024.new.2.bin \ 
                             0xfc000 ../../esp_init_data_default.bin \ 
                             0xfe000 ../../blank.bin

```

