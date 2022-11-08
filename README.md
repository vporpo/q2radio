# Q2 Radio

## About
This is an unofficial Linux client for the Q2 Wi-Fi Internet Radio.
It uses libusb for the communication with the device in userspace.
This program provides some basic functionality for loading radio stations to each side of the radio.

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

## Run examples:
```bash
$ ./q2radio --list # List the current state of the radio
$ ./q2radio --side 0 --name "New Radio" --url "http://stream/url"
```
