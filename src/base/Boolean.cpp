
#include "openeaagles/base/Boolean.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Boolean,"boolean")
EMPTY_SLOTTABLE(Boolean)

EMPTY_COPYDATA(Boolean)
EMPTY_DELETEDATA(Boolean)


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Boolean::serialize(std::ostream& sout, const int, const bool) const
{
   sout << val;
   return sout;
}

}
}
