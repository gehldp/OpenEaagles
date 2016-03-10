
#include "openeaagles/iodevice/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/iodevice/Ai2DiSwitch.h"
#include "openeaagles/iodevice/AnalogInput.h"
#include "openeaagles/iodevice/AnalogOutput.h"
#include "openeaagles/iodevice/DiscreteInput.h"
#include "openeaagles/iodevice/DiscreteOutput.h"
#include "openeaagles/iodevice/IoData.h"
#include "openeaagles/iodevice/SignalGen.h"

#include <cstring>

#if defined(WIN32)
   #include "./windows/UsbJoystickImp.h"
#else
   #include "./linux/UsbJoystickImp.h"
#endif

namespace oe {
namespace iodevice {

base::Object* factory(const char* name)
{
    base::Object* obj = nullptr;

    // Data buffers
    if ( std::strcmp(name, IoData::getFactoryName()) == 0 ) {
        obj = new IoData();
    }

    // Data Handlers
    else if ( std::strcmp(name, DiscreteInput::getFactoryName()) == 0 ) {
        obj = new DiscreteInput();
    }
    else if ( std::strcmp(name, DiscreteOutput::getFactoryName()) == 0 ) {
        obj = new DiscreteOutput();
    }
    else if ( std::strcmp(name, AnalogInput::getFactoryName()) == 0 ) {
        obj = new AnalogInput();
    }
    else if ( std::strcmp(name, AnalogOutput::getFactoryName()) == 0 ) {
        obj = new AnalogOutput();
    }

    // Signal converters and generators
    else if ( std::strcmp(name, Ai2DiSwitch::getFactoryName()) == 0 ) {
        obj = new Ai2DiSwitch();
    }
    else if ( std::strcmp(name, SignalGen::getFactoryName()) == 0 ) {
        obj = new SignalGen();
    }

    // ---
    // Device handler implementations (Linux and/or Windows)
    // ---
    else if ( std::strcmp(name, UsbJoystickImp::getFactoryName()) == 0 ) {
        obj = new UsbJoystickImp();
    }

    return obj;
}

}
}
