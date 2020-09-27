echo Arduino-cli upload wipeout
arduino-cli upload -p COM3 -b esp8266:esp8266:generic:xtal=160,ssl=basic,eesz=4M,sdk=nonosdk_191122,wipe=all,baud=921600 -v