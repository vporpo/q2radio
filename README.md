# Q2 Radio

## About
This is an unofficial Linux client for the Q2 Wi-Fi Internet Radio.
It uses libusb for the communication with the device in userspace.
This program provides some basic functionality for loading radio stations to each side of the radio.

A [windows executable](https://github.com/vporpo/q2radio/releases) can be found in the releases. Please download the `q2radio_win.zip` file containing the windows executable along with the dlls and unzip it.

## Run examples:
- Linux:
```bash
$ ./q2radio --list # List the current state of the radio
$ ./q2radio --side 0 --name "New Radio" --url "http://stream/url"
$ ./q2radio --wifi-ssid "<SSID>" --wifi-key "<WIFI KEY>"

```

- Windows (command prompt)
```
C:\the\unzipped\q2radio\path\> q2radio.exe --help
```

## Build Instructions

### Dependencies
* libusb: You need to have the libusb developers package (The header file libusb.h is required).
* A c++ compiler
* make

```bash
$ git clone https://github.com/vporpo/q2radio.git
$ make -C q2radio/src
$ sudo make install # or just run the binary: q2radio/src/q2radio
```
### Windows build (thanks to user @Untitled-Document-1)
You can use [MSYS2 UCRT64](https://www.msys2.org/) as a build environment.
```
# In the MSYS2 UCRT64 terminal window:
pacman -S git make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-libusb
git clone https://github.com/vporpo/q2radio.git
make -C q2radio/src windows # Don't forget the 'windows' target
# Run
./q2radio/src/q2radio.exe
```
The chances are that you will get an error.

You need to make sure the Q2Radio device uses the WinUSB driver.
There is a tool for changing the driver called [Zadig](https://zadig.akeo.ie) (check out the [libusb windows documentation](https://github.com/libusb/libusb/wiki/Windows#how-to-use-libusb-on-windows).
In the Zadig tool click on Options > List All Devices. Then Q2Radio should show up as a "USB Serial Device" in the drop-down list.
Switch it to the "WinUSB" driver by selecting it on the right hand side of the green arrow, and click on the "Replace Driver" button.
Then try running `q2radio.exe` once again, this time it should work.

