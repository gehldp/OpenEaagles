//------------------------------------------------------------------------------
// Class:       PushButton
// Base class:  basic::Object -> basic::Component -> graphics::Graphic -> Button -> PushButton
// Description: Button which will select one or another graphic based on being pressed or not.
//------------------------------------------------------------------------------
#ifndef __oe_instruments_PushButton_H__
#define __oe_instruments_PushButton_H__

#include "openeaagles/instruments/buttons/Button.h"

namespace oe {
namespace instruments {

class PushButton : public Button
{
    DECLARE_SUBCLASS(PushButton,Button)

public:
    PushButton();

   // gets the pushbutton type ie momentary = false or maintained = true
   bool getFunction() const     { return functionType; };

   // gets the current state of the pushbutton ie off = false or on = true
   bool getCurrentState() const { return currentState; };

   // sets the pushbutton type ie momentary = false or maintained = true
   virtual bool setFunction(const bool x);

   // The left mouse button has been depressed
   virtual bool onMouseDown();

   bool onSingleClick() override;
   bool onCancel() override;

   void updateData(const LCreal dt = 0.0) override;
   bool event(const int event, basic::Object* const obj = nullptr) override;

protected:
   // sets the pushbutton type to momentary = false or maintained = true
   bool setSlotFunction(const basic::Number* const newFunction);
   bool setSlotStartState(const basic::Number* const newFunction);

private:
   bool  functionType;     //false = momentary, true = maintained
   bool  currentState;     //false = off, true = on
   bool  mouseDown;        //mouse press down status
   SendData buttonStatusSD; // what is our button doing?
   bool initState;          // initial state
};

} // end instruments namespace
} // end oe namespace

#endif
