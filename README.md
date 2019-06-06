## Build and Run on Raspberri Pi

### Prerequisite
Building from source code requires some tools which can be installed by:
```
sudo apt update
sudo apt install build-essential
sudo apt install cmake
```

### Dependencies
The following libraries are required
```
libi2c-dev
libmodbus-dev
```

Dependencies can be install by the following commands:

```
sudo apt-get update
sudo apt install libi2c-dev
sudo apt install libmodbus-dev
```

### Enable Software I2C interface:
(supports clock stretching)

Open config.txt
```
sudo nano /boot/config.txt
```

Add the following entry to the file
```
dtoverlay=i2c-gpio,bus=3
```

Save the file by hitting Ctrl+O. Reboot the Pi.

A new I2c interface can be found at `/dev/i2c-3`

SDA is BCM23 (header pin 16)

CLK is BCM24 (header pin 18)

Software I2C requires manual pull-up resistors of about 2.2k on SDA and CLK to Vcc. It may be possible to enable GPIO internal pull-up by the following commands:

```
# enable internal pull-up on pin BCM23
gpio -g mode 23 up
# enable internal pull-up on pin BCM24
gpio -g mode 24 up
```

### Compile from source

Clone source code and build source:
```
git clone https://bitbucket.org/omega_engineering/omega-smartsensor-c.git
cd omega-smartsensor-c
mkdir build
cd build
cmake  ..
make
```

To run basic_sensor example (in build/example/linux directory). Note that hardware connection must match with configuration specified in `linux/basic_sensor.c`

The default configuration would be: 

> Bus : /dev/i2c-3    
Bus type: I2C  
Interrupt Pin: 16 

```
cd examples/linux
./basic_sensor
```

To clean build artifacts:
```
rm -rf build
```