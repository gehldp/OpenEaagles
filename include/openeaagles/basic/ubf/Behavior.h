//------------------------------------------------------------------------------
// Class: Behavior
//------------------------------------------------------------------------------
#ifndef __oe_basic_ubf_Behavior_H__
#define __oe_basic_ubf_Behavior_H__

#include "openeaagles/basic/Component.h"

namespace oe {
namespace basic {
namespace ubf {

class State;
class Action;

//------------------------------------------------------------------------------
// Class: Behavior
// Description: Abstract base class for all behaviors.  Generates an optional
//              action based on our current state.
//
// Factory name: UbfBehavior
// Slots:
//    vote     <Number>    ! default vote/weight value for actions generated
//                         ! by this behavior
//------------------------------------------------------------------------------
class Behavior : public basic::Component
{
   DECLARE_SUBCLASS(Behavior, basic::Component)

public:
   Behavior();

   // Returns a pre-ref'd Action (or zero if no action is generated)
   virtual Action* genAction(const State* const state, const LCreal dt) = 0;

protected:
   unsigned int getVote() const;
   virtual void setVote(const unsigned int x);

   bool setSlotVote(const basic::Number* const num);

private:
   unsigned int vote;
};

inline void Behavior::setVote(const unsigned int x)    { vote = x; }
inline unsigned int Behavior::getVote() const          { return vote; }


} // End ubf namespace
} // End basic namespace
} // End oe namespace

#endif
