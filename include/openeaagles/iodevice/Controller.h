
#ifndef __oe_iodevice_Controller_H__
#define __oe_iodevice_Controller_H__

#include "openeaagles/base/IoDevice.h"

namespace oe {

namespace base { class Number; }

namespace iodevice {

//------------------------------------------------------------------------------
// Class:  Controller
//
// Description:  Communicates with a standard USB Joystick device.
//
// Notes:
//    1) Standard MS Windows joysticks have 8 channels, which are mapped ...
//          channel     Axis
//             0         X
//             1         Y
//             2         Z
//             3         R
//             4         U
//             5         V
//             6         POV: back(1.0); forward(-1.0); center(0.0)
//             7         POV: right(1.0); left(-1.0); center(0.0)
//
//    2) Standard Linux just maps one for one (axis to channel)
//
//
//
// Factory name: UsbJoystick
//
// Slots:
//    deviceIndex <Number>    Unit index
//
//------------------------------------------------------------------------------
class Controller : public base::IoDevice
{
    DECLARE_SUBCLASS(Controller, base::IoDevice)

public:
   Controller();

   virtual unsigned int getDeviceIndex() const;
   virtual bool setDeviceIndex(const int value);

   unsigned short getNumDiscreteInputChannels() const override;
   unsigned short getNumDiscreteInputPorts() const override;
   bool getDiscreteInput(bool* const value, const unsigned int channel, const unsigned int port) const override;
   unsigned short getNumAnalogInputs() const override;
   bool getAnalogInput(double* const value, const unsigned int channel) const override;

protected:

   // Slot functions
   virtual bool setSlotDeviceIndex(const base::Number* const msg);

   unsigned int deviceIndex; // Device index

   // ---
   // analog
   // ---
   static const unsigned short MAX_AI = 16;
   unsigned short numAI;   // Number of analog channels
   double inData[MAX_AI];   // Input analog array

   // ---
   // digital bits
   // ---
   static const unsigned short MAX_DI = 32;
   unsigned short numDI;      // Number of input bits
   bool inBits[MAX_DI];       // Input bit array

private:
   void initData();
};

}
}

#endif
