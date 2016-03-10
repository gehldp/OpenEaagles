
#ifndef __oe_models_RacModel_H__
#define __oe_models_RacModel_H__

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"

namespace oe {

namespace base { class Angle; class Distance; class Number; }

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

    double getGload() const override;                   // G's (+up, one at level)
    double getMach() const override;                    // mach number
    double getAngleOfAttack() const override;           // radians
    double getSideSlip() const override;                // radians
    double getFlightPath() const override;              // radians
    double getCalibratedAirspeed() const override;      // Kts

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
    void dynamics(const double dt) override;            // One pass model update; called from Player::dynamics()

    void reset() override;

    // Slot methods
    virtual bool setSlotMinSpeed(const base::Number* const msg);
    virtual bool setSlotSpeedMaxG(const base::Number* const msg);
    virtual bool setSlotMaxG(const base::Number* const msg);
    virtual bool setSlotMaxAccel(const base::Number* const msg);
    virtual bool setSlotCmdAltitude(const base::Distance* const msg);
    virtual bool setSlotCmdHeading(const base::Angle* const msg);
    virtual bool setSlotCmdVelocity(const base::Number* const msg);

private:
    void updateRAC(const double dt);

    double      vpMin;        // Minimum Velocity              (m/s)
    double      vpMaxG;       // Velocity for Max G's          (g's)
    double      gMax;         // Max G's                       (g's)
    double      maxAccel;     // Max longitudinal acceleration (m/s/s)
    double      cmdAltitude;  // Commanded Altitude            (meters)
    double      cmdHeading;   // Commanded Heading             (degs)
    double      cmdVelocity;  // Commanded speed               (kts)
};

}
}

#endif
