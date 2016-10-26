//------------------------------------------------------------------------------
// Class: Behavior
//------------------------------------------------------------------------------

#include "openeaagles/base/ubf/Behavior.hpp"

#include "openeaagles/base/Number.hpp"

namespace oe {
namespace base {
namespace ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(Behavior, "UbfBehavior")
EMPTY_DELETEDATA(Behavior)
EMPTY_COPYDATA(Behavior)
EMPTY_SERIALIZER(Behavior)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Behavior)
   "vote"            //  1) default vote/weight value for actions generated by this behavior
END_SLOTTABLE(Behavior)

//  mapping of slots to handles
BEGIN_SLOT_MAP(Behavior)
   ON_SLOT(1, setSlotVote, base::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Behavior::Behavior()
{
   STANDARD_CONSTRUCTOR()
   vote = 0;
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// [ 1 .. 65535 ]
bool Behavior::setSlotVote(const base::Number* const num)
{
   bool ok = false;
   int vote = num->getInt();
   if (vote > 0 && vote <= 65535) {
      setVote(static_cast<unsigned int>(vote));
      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* Behavior::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

}
}
}

