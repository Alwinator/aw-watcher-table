# aw-watcher-table

An [Activity Watch](https://github.com/ActivityWatch/activitywatch) watcher which monitors whether you have set your height-adjustable table to sitting or standing.

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
### Build
#### Windows
```
pyinstaller --clean aw.spec
```
### Test
#### Windows
1. Move the aw-table folder inside the dist folder into the Activity Watch installation location
2. Restart [Activity Watch](https://github.com/ActivityWatch/activitywatch)
3. Right click the Activity Watch tray icon. Under modules you should see the aw-table-watcher. Check it to start the aw-watcher-table.

![2](img/aw-watcher-table-deployment.jpg)

### Deploy
1. Go in to the config directory. You can find the path for your OS [here](https://docs.activitywatch.net/en/latest/getting-started.html#config).
2. In the aw-qt directory you should find a **aw-qt.ini** file.
3. Add the aw-table-watcher to autostart_modules to enable auto-start. It should look like this:

```
[aw-qt]
autostart_modules = ["aw-server", "aw-watcher-afk", "aw-watcher-window", "aw-table-watcher"]

[aw-qt-testing]
autostart_modules = ["aw-server", "aw-watcher-afk", "aw-watcher-window", "aw-table-watcher"]
```