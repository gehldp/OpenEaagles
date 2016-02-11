
#ifndef __oe_models_RacModel_H__
#define __oe_models_RacModel_H__

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"

namespace oe {

namespace basic { class Angle; class Distance; class Number; }

namespace models {

//------------------------------------------------------------------------------
// Class: RacModel
//
// Description: Robot Aircraft (RAC) Air Vehicle Dynamics Model
//                -- Very simple dynamics model
//
// Factory name: RacModel
// Slots:
//    minSpeed       <Number>    ! Minimum Velocity        (kts)
//    speedMaxG      <Number>    ! Velocity we reach max G (kts)
//    maxg           <Number>    ! Max G's (at "speedMaxG" or above)
//    maxAccel       <Number>    ! Maximum Acceleration    (m/s/s)
//    cmdAltitude    <Distance>  ! Command Altitude
//    cmdHeading     <Angle>     ! Command Heading
//    cmdSpeed       <Number>    ! Command speed           (kts)
//------------------------------------------------------------------------------
class RacModel : public simulation::AerodynamicsModel
{
    DECLARE_SUBCLASS(RacModel, simulation::AerodynamicsModel)

public:
    RacModel();

    LCreal getGload() const override;                   // G's (+up, one at level)
    LCreal getMach() const override;                    // mach number
    LCreal getAngleOfAttack() const override;           // radians
    LCreal getSideSlip() const override;                // radians
    LCreal getFlightPath() const override;              // radians
    LCreal getCalibratedAirspeed() const override;      // Kts

    bool isHeadingHoldOn() const override;
    double getCommandedHeadingD() const override;
    bool setHeadingHoldOn(const bool b) override;
    bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0) override;
    bool isVelocityHoldOn() const override;
    double getCommandedVelocityKts() const override;
    bool setVelocityHoldOn(const bool b) override;
    bool setCommandedVelocityKts(const double v, const double vNps = 0) override;
    bool isAltitudeHoldOn() const override;
    double getCommandedAltitude() const override;
    bool setAltitudeHoldOn(const bool b) override;
    bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0) override;
    void dynamics(const LCreal dt) override;            // One pass model update; called from Player::dynamics()

    void reset() override;

    // Slot methods
    virtual bool setSlotMinSpeed(const basic::Number* const msg);
    virtual bool setSlotSpeedMaxG(const basic::Number* const msg);
    virtual bool setSlotMaxG(const basic::Number* const msg);
    virtual bool setSlotMaxAccel(const basic::Number* const msg);
    virtual bool setSlotCmdAltitude(const basic::Distance* const msg);
    virtual bool setSlotCmdHeading(const basic::Angle* const msg);
    virtual bool setSlotCmdVelocity(const basic::Number* const msg);

private:
    void updateRAC(const LCreal dt);

    LCreal      vpMin;        // Minimum Velocity              (m/s)
    LCreal      vpMaxG;       // Velocity for Max G's          (g's)
    LCreal      gMax;         // Max G's                       (g's)
    LCreal      maxAccel;     // Max longitudinal acceleration (m/s/s)
    double      cmdAltitude;  // Commanded Altitude            (meters)
    double      cmdHeading;   // Commanded Heading             (degs)
    double      cmdVelocity;  // Commanded speed               (kts)
};

}
}

#endif
