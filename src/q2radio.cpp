// Control the Q2 Internet Wi-Fi Radio by command line
// Q2Radio is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free 
// Software Foundation; either version 3, or (at your option) any later
// version.
//                                                                            
// Q2Radio is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
// for more details. 
//                                                                            
// You should have received a copy of the GNU General Public License
// along with Q2Radio; see the file COPYING3.  If not see 
// <http://www.gnu.org/licenses/>.  

// #define DBG(CMD) CMD
#define DBG(CMD)

#include <iostream>
#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <iomanip> 		// setfill(), setw()
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>

int ENDPOINT = 2;

void printDev(libusb_device *dev) {
  libusb_device_descriptor desc;
  if (libusb_get_device_descriptor(dev, &desc) < 0) {
    std::cerr << "libusb_get_device_descriptor() failed\n";
    return;
  }
  std::cout << "Num of configurations: " << (int) desc.bNumConfigurations << "\n";
  std::cout << "Device class: " << (int) desc.bDeviceClass << "\n";
  fprintf(stdout, "%04x:%04x\n", desc.idVendor, desc.idProduct);
  libusb_config_descriptor *config;
  libusb_get_config_descriptor(dev, 0, &config);
  int numIFs = (int)config->bNumInterfaces;
  std::cout << "Interfaces: " << numIFs << "\n";

  for (int i = 0; i < numIFs; ++i) {
    const libusb_interface *IF = &config->interface[i];
    int numAltSettings = IF->num_altsetting;
    std::cout << "  Num of alternate settings: " << numAltSettings << "\n";
    for (int altI = 0; altI < numAltSettings; ++altI) {
      const libusb_interface_descriptor *IFdesc = &IF->altsetting[altI];
      std::cout << "    IF num: " << (int)IFdesc->bInterfaceNumber << "\n";
      int numOfEndpoints = (int)IFdesc->bNumEndpoints;
      std::cout << "    Num of Endpoints: " << numOfEndpoints << "\n";
      for (int EP = 0; EP < numOfEndpoints; ++EP) {
	const libusb_endpoint_descriptor *EPdesc = &IFdesc->endpoint[EP];
	std::cout << "    Descriptor Type: " << (int)EPdesc->bDescriptorType << "\n";
	std::cout << "    EP Address: " << (int)EPdesc->bEndpointAddress << "\n";
      }
    }
  }
}

void printAllDevs(libusb_device **devs, ssize_t dev_cnt) {
  for (int dev_i = 0; dev_i < dev_cnt; ++dev_i) {
    std::cout << "\n" << dev_i << ".\n";
    printDev(devs[dev_i]);
  }
}


unsigned char end1 = (unsigned char) '\r';
unsigned char end2 = (unsigned char) '\n';

void sendStr(libusb_device_handle *handle, const char *dataStr) {
  DBG(std::cout << "sendStr: " << dataStr << "\n");
  int dataLen = strlen((const char *)dataStr);
  unsigned char *data = (unsigned char *) dataStr;
  int actual;
  if (! (libusb_bulk_transfer(handle, (ENDPOINT | LIBUSB_ENDPOINT_OUT),
			      data, dataLen, &actual, 0) == 0) && actual == dataLen) {
    std::cerr << "Error writing data\n";
    exit(1);
  }

  if (! (libusb_bulk_transfer(handle, (ENDPOINT | LIBUSB_ENDPOINT_OUT),
			      &end1, 1, &actual, 0) == 0) && actual == 1) {
    std::cerr << "Error writing 0xd\n";
    exit(1);
  }

  if (! (libusb_bulk_transfer(handle, (ENDPOINT | LIBUSB_ENDPOINT_OUT),
			      &end2, 1, &actual, 0) == 0) && actual == 1) {
    std::cerr << "Error writing 0xa\n";
    exit(1);
  }
  
}


bool getStr(libusb_device_handle *handle, char *buf, size_t size) {
  int i = 0;

  while (1) {
    #define TMPSIZE 400
    unsigned char tmpBuf[TMPSIZE];

    int actual;
    int succ = libusb_bulk_transfer(handle, (ENDPOINT | LIBUSB_ENDPOINT_IN),
				    tmpBuf, TMPSIZE, &actual, 100);
    if (succ != 0) {
      // std::cerr << "libusb_bulk_transfer(IN) failed!\n";
      return 0;
    }
    if (actual > TMPSIZE) {
      // std::cerr << "ERROR: actual (=" << actual << ") > TMPSIZE (=" << TMPSIZE << ")\n";
      return 0;
    }

    memcpy(buf + i, tmpBuf, actual);
    i+=actual;

    if (i >= 3 && buf[i-2] == '\r' && buf[i-1] == '\n') {
      break;
    }
  }
  assert(i < size && "Increase the buffer size!");
  return 1;
}

void printStrV(const char *buf) {
  for (int i = 0; i < strlen(buf); ++i) {
    fprintf(stderr, "%c(=%02x)\n", (buf[i] >= 32 && buf[i] <= 126) ? buf[i] : ' ', buf[i]);
  }
}

void printStr(const char *buf) {
  fprintf(stderr, "%s", buf);
}


struct Args {
  Args(void) {
    side = -1;
    stationName = NULL;
    stationUrl = NULL;
    list = false;
    custom = NULL;
  };
  int side;
  char *stationName;
  char *stationUrl;
  bool list;
  char *custom;

  void check(void) {
    if (side != -1) {
      if (! stationName) {
	std::cerr << "ERROR: Name empty!\n";
	exit(1);
      }
      if (! stationUrl) {
	std::cerr << "ERROR: URL empty!\n";
	exit(1);
      }
    }
  }
  
  void dump(void) {
    std::cout << "--------------\n";
    std::cout << "Side: " << side << "\n";
    std::cout << "StationName: ";
    if (stationName) {
      std::cout << stationName;
    }
    std::cout << "\n";

    std::cout << "StationUrl: ";
    if (stationUrl) {
      std::cout << stationUrl;
    }
    std::cout << "\n";
    std::cout << "list: " << list << "\n";
    std::cout << "Custom: ";
    if (custom) {
      std::cout << custom;
    }
    std::cout << "\n";
    std::cout << "--------------\n";
  }
  bool isEmpty(void) {
    return side == -1;
  }
};


static struct option long_options[] = {
  {"side", required_argument, 0, 's'},
  {"name", required_argument, 0, 'n'},
  {"url", required_argument, 0, 'u'},
  {"list", no_argument, 0, 'l'},
  {"custom", required_argument, 0, 'c'},
  // WARNING: Keep "help" last for automatically generated usage() message
  {"help", no_argument, 0, 'h'}	
};

void usage(char **argv) {
  std::cerr << "Available options:\n";
  int i = 0;
  while (1) {
    std::cerr << "--" << long_options[i].name;
    switch (long_options[i].has_arg) {
    case required_argument:
      std::cerr << " <ARG>";
      break;
    case no_argument:
      break;
    case optional_argument:
      std::cerr << " [ARG]";
      break;
    default:
      std::cerr << "???";
      break;
    }
    std::cerr << "\n";
    if (strcmp(long_options[i].name, "help") == 0) {
      break;
    }
    ++i;
  }
  std::cerr << "Example: --side 0 --name \"Rebel State Radio\" --url \"http://eco.onestreaming.com:8142\"\n";
  std::cerr << "Example: --custom \"gpre 0 name\":\n";
}

void parseArgs(int argc, char **argv, Args *args) {
  int opt;
  while (1) {
    int option_index = 0;

    int c = getopt_long(argc, argv, "s:n:u:hl", long_options, &option_index);
    if (c == -1) {
      break;
    }
    switch (c) {
    case 's':
      args->side = atoi(optarg);
      if (!(args->side >= 0 && args->side < 4)) {
	std::cerr << "Bad side: " << args->side << ". Should be 0-4\n";
	exit(1);
      }
      break;
    case 'n':
      args->stationName = (char *)optarg;
      break;
    case 'u':
      args->stationUrl = (char *)optarg;
      break;
    case 'h':
      usage(argv);
      exit(1);
    case 'l':
      args->list = true;
      return;
    case 'c':
      args->custom = (char *)optarg;
      break;
    default:
      fprintf(stderr, "Bad argument\n");
      break;
    }
  }
  if (optind < argc) {
    fprintf(stderr, "Bad arguments: ");
    while (optind < argc) {
      fprintf(stderr, "%s ", argv[optind++]);
    }
    fprintf(stderr, "\n");
  }
}


#define SIZE 400

void printResponse(libusb_device_handle *handle, bool out = true) {
  char buf[SIZE];

  while(1) {
    memset(buf, 0, SIZE);
    bool ok = getStr(handle, buf, SIZE-1);
    if (! ok) {
      break;
    }
    if (out) {
      printStr(buf);
    }
  }
}

void banner(void) {
  std::cout << "+--------------- Q2 Radio Setup --------------+\n";
  std::cout << "|                                             |\n";
  std::cout << "+---------------------------------------------+\n";
}

int main(int argc, char **argv) {
  banner();
  Args args;
  parseArgs(argc, argv, &args);
  args.check();
  // args.dump();

  libusb_device **devs; 	// list of devices
  libusb_context *ctx = NULL;
  int r;
  if ((r = libusb_init(&ctx)) < 0) {
    std::cerr << "libusb_init() error\n";
    exit(1);
  }

  libusb_set_debug(ctx, 3);
  // ssize_t dev_cnt;
  // if ((dev_cnt = libusb_get_device_list(ctx, &devs)) < 0) {
  //   std::cerr << "libusb_get_device_list() error\n";
  //   exit(1);
  // };
  // printAllDevs(devs, dev_cnt);

  libusb_device_handle *handle;
  int VID = 0x1f2e;
  int PID = 0x000a;
  if ((handle = libusb_open_device_with_vid_pid(ctx, VID, PID)) == NULL) {
    DBG(fprintf(stderr, "libusb_open_device_with_vid_pid(%04x:%04x)\n", VID, PID));
    exit(1);
  }
  // libusb_free_device_list(devs, 1);

  if (libusb_kernel_driver_active(handle, 0) == 1) {
    DBG(std::cerr << "Kernel driver active\n");
    if (libusb_detach_kernel_driver(handle, 0) == 0) {
      DBG(std::cerr << "Detached!\n");
    }
  }

  int IF = 0;
  if (libusb_claim_interface(handle, IF) < 0) {
    std::cerr << "libusb_claim_interface(" << IF << ") failed!\n";
    exit(1);
  }

  if (args.custom) {
    sendStr(handle, args.custom);
    printResponse(handle);
    return 0;
  }
  
  // Query radio for all sides and print them
  if (args.list || args.side == -1) {
    printResponse(handle, false); // skip any outstanding output
    for (int side = 0; side != 4; ++side) {
      std::cout << "\nSide: " << side << "\n";
      char buf[800];
      snprintf(buf, 800, "gpre %d name", side);
      sendStr(handle, buf);
      printResponse(handle);

      snprintf(buf, 800, "gpre %d url", side);
      sendStr(handle, buf);
      printResponse(handle);
    }
    return 0;
  }

  if (args.side != -1) {
    #define BUFSIZE 400
    char buf[BUFSIZE];
    snprintf(buf, BUFSIZE, "spre %d name \"%s\"", args.side, args.stationName);
    sendStr(handle, buf);
    printResponse(handle);

    snprintf(buf, BUFSIZE, "spre %d url \"%s\"", args.side, args.stationUrl);
    sendStr(handle, buf);
    printResponse(handle);
    return 0;
  }
  

  if (libusb_release_interface(handle, 0) != 0) {
    std::cerr << "libusb_release_interface() failed!\n";
    exit(1);
  }

  libusb_close(handle);
  libusb_exit(ctx);
  return 0;
}
