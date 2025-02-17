# Q2 Radio

## About
This is an unofficial Linux client for the Q2 Wi-Fi Internet Radio.
It uses libusb for the communication with the device in userspace.
This program provides some basic functionality for loading radio stations to each side of the radio.

A [windows executable](htts://github.com/vporpo/q2radio/releases) can be found in the releases.

## Requirements
* libusb: You need to have the libusb developers package (The header file libusb.h is required).
* A c++ compiler
* make

## Build Instructions
```bash
$ git clone https://github.com/vporpo/q2radio.git
$ cd q2radio
$ make
$ sudo make install
```
### Windows build (thanks to user @Untitled-Document-1)
You can use [MSYS2](https://www.msys2.org/) as a build environment.
```
# In the MSYS2 UCRT64 terminal window:
pacman -S git mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-libusb
git clone https://github.com/vporpo/q2radio.git
make -C q2radio/src windows # Don't forget the 'windows' target
# Run
./q2radio/src/q2radio.exe
```
The chances are that you will get an error.

You need to make sure the Q2Radio device uses the WinUSB driver.
There is a tool for changing the driver called [Zadiag](https://zadig.akeo.ie) (check out the [libusb windows documentation](https://github.com/libusb/libusb/wiki/Windows#how-to-use-libusb-on-windows).
In the Zadiag tool Q2Radio should show up as a "USB Serial Device" in the drop-down list.
Switch it to the "WinUSB" driver and try running `q2radio.exe` once again, this time it should work.

## Run examples:
```bash
$ ./q2radio --list # List the current state of the radio
$ ./q2radio --side 0 --name "New Radio" --url "http://stream/url"
$ ./q2radio --wifi-ssid "<SSID>" --wifi-key "<WIFI KEY>"

```
