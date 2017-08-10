
#include "openeaagles/iodevice/DiscreteOutput.hpp"

#include "openeaagles/base/io/IoData.hpp"
#include "openeaagles/base/io/IoDevice.hpp"
#include "openeaagles/base/io/IoHandler.hpp"
#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace iodevice {

IMPLEMENT_SUBCLASS(DiscreteOutput, "DiscreteOutput")
EMPTY_DELETEDATA(DiscreteOutput)

BEGIN_SLOTTABLE(DiscreteOutput)
    "do",         // 1) Discrete Output location (IoData's DO channel)
    "port",       // 2) Device port number (default: 0)
    "channel",    // 3) Device channel (bit) number on the port
    "value",      // 4) Initial value (default: false)
    "inverted",   // 5) Inverted bit flag (default: false)
    "num"         // 6) Number of DOs to manage starting at 'do' and 'channel'
END_SLOTTABLE(DiscreteOutput)

BEGIN_SLOT_MAP(DiscreteOutput)
    ON_SLOT( 1, setSlotLocation, base::Number)
    ON_SLOT( 2, setSlotPort,     base::Number)
    ON_SLOT( 3, setSlotChannel,  base::Number)
    ON_SLOT( 4, setSlotValue,    base::Number)
    ON_SLOT( 5, setSlotInverted, base::Number)
    ON_SLOT( 6, setSlotNum,      base::Number)
END_SLOT_MAP()

DiscreteOutput::DiscreteOutput()
{
   STANDARD_CONSTRUCTOR()
}

void DiscreteOutput::copyData(const DiscreteOutput& org, const bool)
{
   BaseClass::copyData(org);

   devEnb = org.devEnb;
   location = org.location;
   port = org.port;
   channel = org.channel;
   value = org.value;
   invert = org.invert;
   num = org.num;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int DiscreteOutput::getLocation() const
{
   return location;
}

unsigned int DiscreteOutput::getPort() const
{
   return port;
}

unsigned int DiscreteOutput::getChannel() const
{
   return channel;
}

bool DiscreteOutput::getValue() const
{
   return value;
}

bool DiscreteOutput::getInvertFlag() const
{
   return invert;
}

int DiscreteOutput::getNumber() const
{
   return num;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool DiscreteOutput::setLocation(const unsigned int v)
{
   location = v;
   return true;
}

bool DiscreteOutput::setPort(const unsigned int v)
{
   port = v;
   devEnb = true;
   return true;
}

bool DiscreteOutput::setChannel(const unsigned int v)
{
   channel = v;
   devEnb = true;
   return true;
}

bool DiscreteOutput::setValue(const bool f)
{
   value = f;
   return true;
}

bool DiscreteOutput::setInvertFlag(const bool f)
{
   invert = f;
   return true;
}

bool DiscreteOutput::setNumber(const int n)
{
   num = n;
   return true;
}

//------------------------------------------------------------------------------
// process inputs
//------------------------------------------------------------------------------
void DiscreteOutput::processInputs(const double, const base::IoDevice* const, base::IoData* const)
{
}

//------------------------------------------------------------------------------
// process outputs
//------------------------------------------------------------------------------
void DiscreteOutput::processOutputs(const double, const base::IoData* const outData, base::IoDevice* const device)
{
   if (device != nullptr && devEnb) {
      unsigned int chan = channel;
      unsigned int loc  = location;
      unsigned int n = ((num >= 0) ? num : -num);

      for (unsigned int i = 0; i < n; i++) {

         // Get the bit from the cockpit output data handler
         if (outData != nullptr) {
            outData->getDiscreteOutput(loc, &value);
         }

         // Send the bit to the DO card
         bool value0 = value;
         if (invert) value0 = !value;
         device->setDiscreteOutput(value0, chan, port);

         chan++;
         if (num >= 0) loc++;
         else if (loc > 0) loc--;
      }
   }
}


//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

// location: Output array index (location)
bool DiscreteOutput::setSlotLocation(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setLocation( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

// port: DiHandler's port number
bool DiscreteOutput::setSlotPort(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setPort( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

// channel: DiHandler's channel (bit) number on the port
bool DiscreteOutput::setSlotChannel(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int v = msg->getInt();
      if (v >= 0) {
         ok = setChannel( static_cast<unsigned int>(v) );
      }
   }
   return ok;
}

// value: Initial value (default: false)
bool DiscreteOutput::setSlotValue(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setValue( msg->getBoolean() );
   }
   return ok;
}

// invert: Inverted bit flag (default: false)
bool DiscreteOutput::setSlotInverted(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setInvertFlag( msg->getBoolean() );
   }
   return ok;
}

bool DiscreteOutput::setSlotNum(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setNumber( msg->getInt() );
   }
   return ok;
}


std::ostream& DiscreteOutput::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "do: " << getLocation() << std::endl;

   indent(sout,i+j);
   sout << "port: " << getPort() << std::endl;

   indent(sout,i+j);
   sout << "channel: " << getChannel() << std::endl;

   indent(sout,i+j);
   sout << "value: " << getValue() << std::endl;

   indent(sout,i+j);
   sout << "inverted: " << getInvertFlag() << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
