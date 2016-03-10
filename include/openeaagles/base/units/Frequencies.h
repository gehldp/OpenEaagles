//------------------------------------------------------------------------------
// Classes:  Frequency, Hertz  // ADD MORE and find credible source
// Base class:  Object -> Number -> Frequency
//              Object -> Number -> Frequency -> Hertz
//              Object -> Number -> Frequency -> MilliHertz
//              Object -> Number -> Frequency -> MicroHertz
//              Object -> Number -> Frequency -> NanoHertz
//              Object -> Number -> Frequency -> KiloHertz
//              Object -> Number -> Frequency -> MegaHertz
//              Object -> Number -> Frequency -> GigaHertz
//              Object -> Number -> Frequency -> TeraHertz
//
// Description:  Numbers as frequencies -- All metric equivalents of Hertz (kilo, nano, etc..)
//               Base unit for Frequency derivations is a hertz.
//
//
// Public methods (Defined in Frequency, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Frequency derived instance with an double.
//
//     set(const Frequency& n)
//        Sets, and converts if necessary, a Frequency derived instance with
//        another Frequency derived instance.
//
//     double convert(const Frequency& n)
//        Converts the value of a Frequency derived instance into
//        the units of another Frequency derived instance.
//
//     Conversion routines:
//        Not used here, because they are all metric conversions...
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Frequency& n)
//        Sends "( <the Frequency derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes: Hertz):
//
//     double convertStatic(const Frequency& n)
//        Static function to convert the given Frequency derived
//        instance into the units of a specific Frequency derived
//        class.
//
//------------------------------------------------------------------------------
#ifndef __oe_base_Frequencies_H__
#define __oe_base_Frequencies_H__

#include "openeaagles/base/Number.h"

namespace oe {
namespace base {

// -----------------------------------------------------------------------------
// Defined constants:
// All constants were collected via the internet from
//
// All metric conversions were based on the conversion tables found in
// the Transportation Energy Data Book: Edition 22 - 2002, which can be found out
// http://www-cta.ornl.gov/cta/data/tedb22/Edition22_AppendixB.pdf
// Since all frequency classes are based in hertz, all conversions will either
// convert to or from hertz.
///////////////////////////////////////////////

/////////////////////////////////////////////
//------------------------------------------------------------------------------
// Class:  Frequency
// Base class:  Object -> Number -> Frequency
// Description:  Base class for frequencies.  Defined as a hertz which is
//               equivalent to an instance of Hertz with its value equal
//               to 1.0.
//------------------------------------------------------------------------------
class Frequency : public Number
{
    DECLARE_SUBCLASS(Frequency, Number)

public:
    Frequency();
    Frequency(const double value);

    void set(const double v)     { val = v; }
    void set(const Frequency& n) { val = fromFrequency(n.toFrequency()); }

    virtual double toFrequency() const = 0;
    virtual double fromFrequency(const double a) const = 0;
    double convert(const Frequency& n) { return fromFrequency(n.toFrequency()); }

    // Conversion constants
    static const double Hz2KHz;  // Hertz -> KiloHertz
    static const double KHz2Hz;  // KiloHertz -> Hertz
    static const double Hz2MHz;  // Hertz -> MegaHertz
    static const double MHz2Hz;  // MegaHertz -> Hertz
    static const double Hz2GHz;  // Hertz -> GigaHertz
    static const double GHz2Hz;  // GigaHertz -> Hertz
    static const double Hz2THz;  // TeraHertz -> Hertz
    static const double THz2Hz;  // Hertz -> TeraHertz
};


inline std::ostream& operator<<(std::ostream& sout, const Frequency& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class:  Hertz
// Base class:  Object -> Number -> Frequency -> Hertz
// Description:  Base unit for frequency, with a hertz being an instance of Hertz
//               with its value equal to 1.0.
//------------------------------------------------------------------------------
class Hertz : public Frequency
{
    DECLARE_SUBCLASS(Hertz, Frequency)

public:
    Hertz();
    Hertz(const double value);
    Hertz(const Frequency& value);

    static double convertStatic(const Frequency &n)      { return n.toFrequency(); }
    double toFrequency() const override                  { return static_cast<double>(val); }
    double fromFrequency(const double a) const override  { return a; }
};

//------------------------------------------------------------------------------
// Class:  KiloHertz
// Base class:  Object -> Number -> Frequency -> KiloHertz
// Description:  Hertz * 0.001
//------------------------------------------------------------------------------
class KiloHertz : public Frequency
{
    DECLARE_SUBCLASS(KiloHertz, Frequency)

public:
    KiloHertz();
    KiloHertz(const double value);
    KiloHertz(const Frequency& value);

private:
    static double convertStatic(const Frequency &n)      { return n.toFrequency() * Hz2KHz; }
    double toFrequency() const override                  { return static_cast<double>(val * KHz2Hz); }
    double fromFrequency(const double a) const override  { return a * Hz2KHz; }
};


//------------------------------------------------------------------------------
// Class:  MegaHertz
// Base class:  Object -> Number -> Frequency -> MegaHertz
// Description:  Hertz * 0.000001
//------------------------------------------------------------------------------
class MegaHertz : public Frequency
{
    DECLARE_SUBCLASS(MegaHertz, Frequency)

public:
    MegaHertz();
    MegaHertz(const double value);
    MegaHertz(const Frequency& value);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * Hz2MHz; }
    double toFrequency() const override                  { return static_cast<double>(val * MHz2Hz); }
    double fromFrequency(const double a) const override  { return a * Hz2MHz; }
};


//------------------------------------------------------------------------------
// Class:  GigaHertz
// Base class:  Object -> Number -> Frequency -> GigaHertz
// Description:  Hertz * 0.000000001
//------------------------------------------------------------------------------
class GigaHertz : public Frequency
{
    DECLARE_SUBCLASS(GigaHertz, Frequency)

public:
    GigaHertz();
    GigaHertz(const double value);
    GigaHertz(const Frequency& value);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * Hz2GHz; }
    double toFrequency() const override                  { return static_cast<double>(val * GHz2Hz); }
    double fromFrequency(const double a) const override  { return a * Hz2GHz; }
};


//------------------------------------------------------------------------------
// Class:  TeraHertz
// Base class:  Object -> Number -> Frequency -> TeraHertz
// Description:  Hertz * 0.000000000001
//------------------------------------------------------------------------------
class TeraHertz : public Frequency
{
    DECLARE_SUBCLASS(TeraHertz, Frequency)

public:
    TeraHertz();
    TeraHertz(const double value);
    TeraHertz(const Frequency& value);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * Hz2THz; }
    double toFrequency() const override                  { return static_cast<double>(val * THz2Hz); }
    double fromFrequency(const double a) const override  { return a * Hz2THz; }
};

} // End base namespace
} // End oe namespace

#endif
