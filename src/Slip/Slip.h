/*
Extends the Serial class to encode SLIP over serial
*/

#ifndef Slip_h
#define Slip_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Stream.h>



class Slip: public Stream{
	
private:
	enum erstate {SLIPDATA, SLIPEOT, SLIPESC } rstate;
	
	//the serial port used
	Stream * serial;
    
    bool receivedData;

	
public:
	
	//the serial port used
	Slip(Stream * );

	
	int available();
	int read();
	int peek();
	void flush();

    
    //SLIP specific method which begins a transmitted packet
	void beginPacket();
	
	//SLIP specific method which ends a transmittedpacket
	void endPacket();
	// SLIP specific method that reads if EOT was received 
	bool  readPacketEnd();
	
	
//the arduino and wiring libraries have different return types for the write function
#if defined(WIRING) || defined(BOARD_DEFS_H)
	void write(uint8_t b);
    void write(const uint8_t *buffer, size_t size);

#else
	//overrides the Stream's write function to encode SLIP
	size_t write(uint8_t b);
    size_t write(const uint8_t *buffer, size_t size);

	//using Print::write;
#endif

};


#endif