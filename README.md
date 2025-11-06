# Q2 Radio

## About
This is an unofficial Linux client for the Q2 Wi-Fi Internet Radio.
It uses libusb for the communication with the device in userspace.
This program provides some basic functionality for loading radio stations to each side of the radio.

A [windows executable](https://github.com/vporpo/q2radio/releases) can be found in the releases. Please download the `q2radio_win.zip` file containing the windows executable along with the dlls and unzip it.

## Run examples:
- Linux:
```bash
$ ./q2radio        # List the current state of the radio and exit
$ ./q2radio --list # List the current state of the radio and exit
$ ./q2radio --side 0 --name 'New Radio' --url 'http://stream/url'
$ ./q2radio --wifi-ssid '<SSID>' --wifi-key '<WIFI KEY>'
$ ./q2radio --power-on-volume 10   # Set the default volume (1 to 17)
$ ./q2radio --passthru '<command>' # Pass <command> directly to radio. e.g., --passthru 'help'
```
- Windows:
```
> q2radio.exe        # List the current state of the radio and exit
> q2radio.exe --list # List the current state of the radio and exit
> q2radio.exe --side 0 --name "New Radio" --url "http://stream/url"
> q2radio.exe --wifi-ssid "<SSID>" --wifi-key "<WIFI KEY>"
> q2radio.exe --power-on-volume 10   # Set the default volume (1 to 17)
> q2radio.exe --passthru "<command>" # Pass <command> directly to radio. e.g., --passthru "help"
```
### On Windows (command line prompt a.k.a. cmd)
```
C:\the\unzipped\q2radio\path\> q2radio.exe --help
```

The chances are that you will get the error like:
```
ERROR: Could not connect to the device!
Please make sure it is connected and turned on!
```
In this case you will need to use the [Zadig](https://zadig.akeo.ie) tool to change the driver to `WinUSB` as explained below:

#### Switching to the WinUSB driver using [Zadig](https://zadig.akeo.ie)

You need to make sure the Q2Radio device uses the WinUSB driver.
There is a tool for changing the driver called [Zadig](https://zadig.akeo.ie) (check out the [libusb windows documentation](https://github.com/libusb/libusb/wiki/Windows#how-to-use-libusb-on-windows)).
In the Zadig tool:
- click on Options > List All Devices. Then Q2Radio should show up as a "USB Serial Device" in the drop-down list.
- Switch it to the "WinUSB" driver by selecting it on the right hand side of the green arrow, and
- click on the "Replace Driver" button.

You are all set. Run `q2radio.exe` once again, this time it should connect to the device.

## Build Instructions (for developers)

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


# Changelog
- v0.2: Adds `--power-on-volume` option for setting up the default volume when the radio powers on
- v0.3: Fixes whitespaces in SSID/key and fixes crash when passing unsupported arguments