
//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}

#include "OscMassenger.h"



OscMassenger::OscMassenger(Stream* stream)
  : Massenger(stream),
   _slipEscaping(false), 
   slip (stream)
{_needToEmptyOscInput = false;}

void OscMassenger::flush()
{
  Massenger::flush();
  
}

int8_t OscMassenger::nextByte(bool* error) {
  int8_t v;
  _nextBlock((uint8_t*)&v, sizeof(int8_t), error);
  return v;
}

int16_t OscMassenger::nextInt(bool* error)
{
  int16_t v;
  _nextBlock((uint8_t*)&v, sizeof(int16_t), error);
  return v;
}

int32_t OscMassenger::nextLong(bool* error)
{
  int32_t v;
  _nextBlock((uint8_t*)&v, sizeof(int32_t), error);
  return v;
}

float OscMassenger::nextFloat(bool* error)
{
  float v;
  _nextBlock((uint8_t*)&v, sizeof(float), error);
  return v;
}

double OscMassenger::nextDouble(bool* error)
{
  double v;
  _nextBlock((uint8_t*)&v, sizeof(double), error);
  return v;
}

void OscMassenger::sendBegin(const char* address)
{
	oscOutput.setAddress(address);
	/*
  _stream->print(address);
  _stream->write(uint8_t(0)); // null character (end of string)
  */

}

void OscMassenger::sendByte(uint8_t value)
{
 //_sendSlipData(&value, sizeof(uint8_t));
  oscOutput.add(value);
}

void OscMassenger::sendInt(int16_t value)
{
 //_sendSlipData(&value, sizeof(uint16_t));
 oscOutput.add(value);
}

void OscMassenger::sendLong(int32_t value)
{
  //_sendSlipData(&value, sizeof(uint32_t));
  oscOutput.add(value);
}

void OscMassenger::sendFloat(float value)
{
  //_sendSlipData(&value, sizeof(float));
  oscOutput.add(value);
}

void OscMassenger::sendDouble(double value)
{
  //_sendSlipData(&value, sizeof(double));
  oscOutput.add(value);
}

void OscMassenger::sendEnd()
{
	slip.beginPacket();
    oscOutput.send(  slip );
	slip.endPacket();
    oscOutput.empty();
  //_stream->write(BINARY_MASSENGER_SLIP_END);
}


bool OscMassenger::receive()
{
  // Flush.
  flush();

 if ( _needToEmptyOscInput ) {
 	_oscInput.empty();
 	_needToEmptyOscInput = false;
 }
  // Read stream.

  int size = slip.available();
  while (size--)
        _oscInput.fill(slip.read());

   if ( slip.endOfPacket() ) {
   	_needToEmptyOscInput = true;
   	  if (!_oscInput.hasError()) return true;
   }

  return false;
}


bool OscMassenger::_process(int streamByte)
{
	/*
  byte value = 0;

  // Check if we've reached the end of the buffer.
  if (_messageSize >= MASSENGER_BUFFERSIZE)
  {
    _messageSize = MASSENGER_BUFFERSIZE;
    return true;
  }

  // Process byte.
  _slipEscaping = false; // reset
  switch (streamByte) {
    case BINARY_MASSENGER_SLIP_END:
      if (_messageSize > 0) // only process this if we are *not* at beginning
      {
        // Position _nextIndex after command address string.
        _nextIndex = 0;
        _updateNextIndex();

        return true;
      }
      break;

    case BINARY_MASSENGER_SLIP_ESC:
      _slipEscaping = true;
      break;

    case BINARY_MASSENGER_SLIP_ESC_END:
      _write(_slipEscaping ? BINARY_MASSENGER_SLIP_END : BINARY_MASSENGER_SLIP_ESC_END);
      break;
    case BINARY_MASSENGER_SLIP_ESC_ESC:
      _write(_slipEscaping ? BINARY_MASSENGER_SLIP_ESC : BINARY_MASSENGER_SLIP_ESC_ESC);
      break;

    default:
      _write(streamByte);
  }

  */
  return false;
}

bool OscMassenger::_hasNext() const {
  return (_nextIndex < _messageSize);
}

bool OscMassenger::_updateNextIndex()
{
  while (_buffer[_nextIndex] != 0)
    _nextIndex++;
  _nextIndex++;
  return (_nextIndex < _messageSize);
}

void OscMassenger::_nextBlock(uint8_t* value, size_t n, bool* error)
{
  // Check for errors.
  bool err = !_hasNext();
  if (err)
    memset(value, 0, n); // set to zero (default)

  if (error)
    *error = err;

  // Process data.
  if (!err)
  {
    memcpy(value, &_buffer[_nextIndex], n);
    _nextIndex += n;
  }
}
