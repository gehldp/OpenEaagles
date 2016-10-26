// Bullseye

#include "openeaagles/simulation/Bullseye.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Bullseye,"Bullseye")
EMPTY_SERIALIZER(Bullseye)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Bullseye::Bullseye()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Bullseye::copyData(const Bullseye& org, const bool)
{
    BaseClass::copyData(org);
}


//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Bullseye::deleteData()
{
}

}
}
