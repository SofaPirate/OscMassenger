#ifndef OscMassenger_h
#define OscMassenger_h

#include "Massenger/Massenger.h"
#include "Osc/OSCBundle.h"
#include "Slip/Slip.h"

/// Main OscMassenger class.
class OscMassenger : public Massenger
{
public:
  /// Constructor.
  OscMassenger(Stream* stream=&Serial);

  /// Flushes current message in buffer (if any).
  virtual void flush();

  /// Reads next byte.
  virtual int8_t nextByte(bool* error=0);

  /// Reads next int.
  virtual int16_t nextInt(bool* error=0);

  /// Reads next long.
  virtual int32_t nextLong(bool* error=0);

  /// Reads next float.
  virtual float nextFloat(bool* error=0);

  /// Reads next double.
  virtual double nextDouble(bool* error=0);

  /// Begins the sending of a message.
  virtual void sendBegin(const char* address);

  /// Sends a byte.
  virtual void sendByte(uint8_t value);
  virtual void sendByte(const char *address, uint8_t value)
  { Massenger::sendByte(address, value); }

  /// Sends an int.
  virtual void sendInt(int16_t value);
  virtual void sendInt(const char *address, int16_t value)
  { Massenger::sendInt(address, value); }

  /// Sends a long.
  virtual void sendLong(int32_t value);
  virtual void sendLong(const char *address, int32_t value)
  { Massenger::sendLong(address, value); }

  /// Sends a float.
  virtual void sendFloat(float value);
  virtual void sendFloat(const char *address, float value)
  { Massenger::sendFloat(address, value); }

  /// Sends a double.
  virtual void sendDouble(double value);
  virtual void sendDouble(const char *address, double value)
  { Massenger::sendDouble(address, value); }

  /// Ends the sending of a message.
  virtual void sendEnd();

  virtual bool receive();

protected:
  /// Processes a single value read from the serial stream.
  virtual bool _process(int serialByte);

private:
   // Moves nextIndex to the next token.
  bool _updateNextIndex();

  // Returns true iff it is still possible to call next*().
  bool _hasNext() const;

  // Helper function to read next block of data.
  void _nextBlock(uint8_t* value, size_t n, bool* error);


  // Index in the buffer of next argument to read.
  uint8_t _nextIndex;

  // SLIP decoding state: currently escaping.
  bool _slipEscaping;
  
  // OSC output message
  OSCMessage oscOutput;

  bool _needToEmptyOscInput;
  OSCBundle _oscInput;
  
  Slip slip;

};




#endif
