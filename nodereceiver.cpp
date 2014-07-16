/*

SensorThing Receiver

Receives messages on radio and outputs in parsable format

Based heavily on https://github.com/tmrh20/RF24 examples.

*/

#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <ctime>
#include <stdio.h>
#include <time.h>

/**
 * g++ -L/usr/lib main.cc -I/usr/include -o main -lrrd
 **/
//using namespace std;

// Setup for GPIO 22 CE and GPIO 25 CSN with SPI Speed @ 1Mhz
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_18, BCM2835_SPI_SPEED_1MHZ);

RF24Network network(radio);

// Our node ID
const uint16_t this_node = 0;

// Structure of our payload
struct payload_t {
  unsigned long ms;
  unsigned long counter;
  float hum;
  float temp;
  float vcc;
};

// Buffer flushes are for python wrapper
// Format is nodeid|ms|humidity|temperature|voltage
int main(int argc, char** argv) 
{
	radio.begin();
	
	delay(5);
	network.begin( 90, this_node);

	printf("Receiver started\n");
	fflush(stdout);
	
	while(1)
	{

		  network.update();
  		  while ( network.available() ) {
    			
		 	RF24NetworkHeader header;
			payload_t payload;
  			network.read(header,&payload,sizeof(payload));
			
			printf("%u|%lu|%f|%f|%f\n",header.from_node,payload.ms,payload.hum,payload.temp,payload.vcc);
			fflush(stdout);
		  }
	}

	return 0;
}

