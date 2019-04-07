#!/bin/bash

DIR_BIN=default_esp_flash/bin
DIR_USER=$DIR_BIN/at/512+512

esptool --port /dev/ttyUSB0 \
      	--baud 115200 write_flash \
       	--verify -fs 8m  0x00000 $DIR_BIN/boot_v1.6.bin \
        	0x01000 $DIR_USER/user1.1024.new.2.bin \
	      	0x81000 $DIR_USER/user2.1024.new.2.bin \
	      	0xfc000 $DIR_BIN/esp_init_data_default.bin \
	      	0xfe000 $DIR_BIN/blank.bin
