# aw-watcher-table

An Activity Watch watcher which monitors whether you have set your height-adjustable table to sitting or standing.

![1](img/aw-watcher-table-1.jpg)

# Idea

Unfortunately my adjustable table has no API or something similar. Therefore I need an external device which measures the height of the table. This external device must have internet access and a distance sensor.

# Implementation

## Hardware
### Electronics
I have used an [ESP8266](https://en.wikipedia.org/wiki/ESP8266) as micro controller. It has in-build Wifi and GPIOs to controll sensors. As distance sensor I have used an [HC-SR04 Ultrasonic Sensor](http://wiki.sunfounder.cc/index.php?title=Ultrasonic_Module), because I had it laying around.

### Case
I printed my case using the Prusa I3 MK3. As modeling software I used Fusion360. You can download the 3D model [here (Thingiverse)](https://www.thingiverse.com/thing:4619348).

![0](img/aw-watcher-table-0.jpg)

### Price

| Name            | Price  |
|-----------------|--------|
| ESP8266         | 6.50€  |
| HC-SR04         | 5.16€  |
| 3D Printed Case | ~0.25€ |
| Sum             | 11.91€ |

Definitely affordable.

### Test
When the hardware is finished and powered on it should be possible to get the table height via the curl command:
```
# curl http://192.168.2.4/measure
{"table_height":128}
```

## Software
### Install
```
virtualenv venv # create virtual environment
venv\Scripts\activate # activate virtual environment
poetry install # install required packages
```
### Run
```
poetry run aw-watcher-table [--testing] [-v] [--verbose]
```
### Build for Windows (WIP)
```
pyinstaller --clean aw.spec
```
