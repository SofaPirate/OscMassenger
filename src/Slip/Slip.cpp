#include "Slip.h"

static const uint8_t eot = 0300;
static const uint8_t slipesc = 0333;
static const uint8_t slipescend = 0334;
static const uint8_t slipescesc = 0335;
/*
 CONSTRUCTOR
 */
//instantiate with the tranmission layer
//use HardwareSerial
Slip::Slip(Stream * s){
	serial = s;
	rstate = SLIPDATA;
	receivedData = false;
}


/*
 SERIAL METHODS
 */

 bool Slip::readPacketEnd() {
 	if  (rstate == SLIPEOT) {
 		rstate = SLIPDATA;
 		bool result = receivedData;
 		receivedData = false;
 		return result;
 	}
 	return false;
 	
 }

int Slip::available(){
    
 while ( serial->available()  )  {
  
		uint8_t c =serial->peek();
        
 		if ( c == slipesc ) {
 			
 			rstate = SLIPESC;
			serial->read(); // throw it on the floor
 			
		}  else if ( c == eot ) {
			rstate = SLIPEOT;
			serial->read(); // throw it on the floor
			if ( receivedData ) return 1;
			
		} else {
			return 1;
		}
 	 
 	
 }
 return 0;
}

//reads a byte from the buffer
int Slip::read(){
    
	if ( rstate == SLIPEOT ) {
		return -1;
	} else {
		int c = serial->read();
		if ( rstate == SLIPESC ) {
			rstate = SLIPDATA;
			if(c==slipescend) { 
				receivedData = true;
				return eot; 
			} else if(c==slipescesc) {
				receivedData = true;
				return slipesc;
			} else {
				// insert some error code here
				return -1;
			}
		} else {
			receivedData = true;
			return c;
		}
	}
}
	 


/*
back:
	uint8_t c = serial->read();
   
   if ( c > - 1) {
   	   if ( c == eot ) {
   	   	return c;
   	   } else if(rstate==SLIPESC) {
		if(c==slipescend)
			return eot;
		else if(c==slipescesc)
			return slipesc;
			else {
				// insert some error code here
				return -1;
			}

	}
   	} else {
   		return -1;
   	}




	if(rstate==CHAR)
	{
		if(c==slipesc)
		{
			rstate=SLIPESC;
			goto back;
		}	
		else if(c==eot){
		
			return -1; // xxx this is an error
		}
		return c;
	}
	else
	if(rstate==SLIPESC)
	{
		rstate=CHAR;
		if(c==slipescend)
			return eot;
		else if(c==slipescesc)
			return slipesc;
			else {
				// insert some error code here
				return -1;
			}

	}
	else
		return -1;
}
*/
// as close as we can get to correct behavior
int Slip::peek(){
	uint8_t c = serial->peek();
	if(rstate==SLIPESC)
	{
		if(c==slipescend)
			return eot;
		else if(c==slipescesc)
			return slipesc;
	}
	return c; 
}

//the arduino and wiring libraries have different return types for the write function
#if defined(WIRING) || defined(BOARD_DEFS_H)

//encode SLIP
 void Slip::write(uint8_t b){
	if(b == eot){ 
		serial->write(slipesc);
		return serial->write(slipescend); 
	} else if(b==slipesc) {  
		serial->write(slipesc);
		return serial->write(slipescesc); 
	} else {
		return serial->write(b);
	}	
}
void Slip::write(const uint8_t *buffer, size_t size) {  while(size--) write(*buffer++); }
#else
//encode SLIP
size_t Slip::write(uint8_t b){
	if(b == eot){ 
		serial->write(slipesc);
		return serial->write(slipescend); 
	} else if(b==slipesc) {  
		serial->write(slipesc);
		return serial->write(slipescesc); 
	} else {
		return serial->write(b);
	}	
}
size_t Slip::write(const uint8_t *buffer, size_t size) { size_t result; while(size--) result = write(*buffer++); return result; }

#endif


//SLIP specific method which begins a transmitted packet
void Slip::beginPacket() { 	serial->write(eot); }

//signify the end of the packet with an EOT
void Slip::endPacket(){
	serial->write(eot);

}

void Slip::flush(){
	serial->flush();
}

