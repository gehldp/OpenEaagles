
#include "openeaagles/models/dynamics/RacModel.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/osg/Matrix"
#include "openeaagles/base/osg/Vec3"
#include "openeaagles/base/osg/Quat"

#include <cmath>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(RacModel, "RacModel")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(RacModel)
    "minSpeed",    // 1 Minimum Velocity        (kts)
    "speedMaxG",   // 2 Velocity we reach max G (kts)
    "maxg",        // 3 Max G's (at "speedMaxG" or above)
    "maxAccel",    // 4 Maximum Acceleration    (m/s/s)
    "cmdAltitude", // 5 Command Altitude
    "cmdHeading",  // 6 Command Heading
    "cmdSpeed",    // 7 Command speed
END_SLOTTABLE(RacModel)

//------------------------------------------------------------------------------
// Slot Map
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(RacModel)
    ON_SLOT( 1, setSlotMinSpeed, base::Number)
    ON_SLOT( 2, setSlotSpeedMaxG, base::Number)
    ON_SLOT( 3, setSlotMaxG,base::Number)
    ON_SLOT( 4, setSlotMaxAccel, base::Number)
    ON_SLOT( 5, setSlotCmdAltitude, base::Distance)
    ON_SLOT( 6, setSlotCmdHeading, base::Angle)
    ON_SLOT( 7, setSlotCmdVelocity, base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
RacModel::RacModel()
{
   STANDARD_CONSTRUCTOR()

   vpMin = 0.0;
   vpMaxG = 250.0;
   gMax = 4.0;
   maxAccel = 10.0;

   cmdAltitude = -9999.0;
   cmdHeading = -9999.0;
   cmdVelocity = -9999.0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void RacModel::copyData(const RacModel& org, const bool)
{
   BaseClass::copyData(org);

   vpMin = org.vpMin;
   vpMaxG = org.vpMaxG;
   gMax = org.gMax;
   maxAccel = org.maxAccel;

   cmdAltitude = org.cmdAltitude;
   cmdHeading = org.cmdHeading;
   cmdVelocity = org.cmdVelocity;
}

EMPTY_DELETEDATA(RacModel)

//------------------------------------------------------------------------------
// reset() --
//------------------------------------------------------------------------------
void RacModel::reset()
{
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void RacModel::dynamics(const double dt)
{
    updateRAC(dt);
    BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// Get Vehicle data
//------------------------------------------------------------------------------
double RacModel::getGload() const
{
   return -1;
}

double RacModel::getMach() const
{
   return 0.5;
}

double RacModel::getAngleOfAttack() const
{
   return 0.0;
}

double RacModel::getSideSlip() const
{
   return 0.0;
}

double RacModel::getFlightPath() const
{
   const simulation::Player* pp = static_cast<const simulation::Player*>( findContainerByType(typeid(simulation::Player)) );
   if (pp == nullptr) return 0;
   return static_cast<double>(pp->getPitchR());
}

double RacModel::getCalibratedAirspeed() const
{
   const simulation::Player* pp = static_cast<const simulation::Player*>( findContainerByType(typeid(simulation::Player)) );
   if (pp == nullptr) return 0;
   return pp->getTotalVelocityKts();
}

//------------------------------------------------------------------------------
// Autopilot controls
//------------------------------------------------------------------------------
bool RacModel::isHeadingHoldOn() const
{
   return true;
}

double RacModel::getCommandedHeadingD() const
{
   return cmdHeading;
}

// setHeadingHoldOn() --   Enable/Disable heading hold
bool RacModel::setHeadingHoldOn(const bool)
{
   return true;
}

// setCommandedHeadingD() --   Sets commanded heading (true: degs)
bool RacModel::setCommandedHeadingD(const double degs, const double, const double)
{
   cmdHeading = degs;
   return true;
}

bool RacModel::isVelocityHoldOn() const
{
   return true;
}

double RacModel::getCommandedVelocityKts() const
{
   return cmdVelocity;
}

// setVelocityHoldOn() --   Enable/Disable velocity hold
bool RacModel::setVelocityHoldOn(const bool)
{
   return true;
}

// setCommandedVelocityKts() --   Sets commanded velocity (kts)
bool RacModel::setCommandedVelocityKts(const double v, const double vNps)
{
   cmdVelocity = v;
   return true;
}

bool RacModel::isAltitudeHoldOn() const
{
   return true;
}

double RacModel::getCommandedAltitude() const
{
   return cmdAltitude;
}

// setAltitudeHoldOn() --   Enable/Disable altitude hold
bool RacModel::setAltitudeHoldOn(const bool)
{
   return true;
}

// setCommandedAltitude() --   Sets commanded altitude (meters)
bool RacModel::setCommandedAltitude(const double m, const double, const double)
{
   cmdAltitude = m;
   return false;
}

//------------------------------------------------------------------------------
// updateRAC -- update Robot Aircraft
//------------------------------------------------------------------------------
void RacModel::updateRAC(const double dt)
{
   // Get our Player (must have one!)
   simulation::Player* pp = static_cast<simulation::Player*>( findContainerByType(typeid(simulation::Player)) );
   if (pp == nullptr) return;

   // Acceleration of Gravity (M/S)
   double g = base::ETHG * base::Distance::FT2M;

   // Set default commanded values
   if (cmdAltitude < -9000.0)
       cmdAltitude = static_cast<double>(pp->getAltitudeM());
   if (cmdHeading < -9000.0)
       cmdHeading = static_cast<double>(pp->getHeadingD());
   if (cmdVelocity < -9000.0)
       cmdVelocity = pp->getTotalVelocityKts();

   // ---
   // Compute delta altitude; commanded vertical velocity and
   // commanded flight path angle
   // ---

   // Max altitude rate 6000 ft /min converted to M/S
   double maxAltRate = (3000.0 / 60.0) * base::Distance::FT2M;

   // commanded vertical velocity is delta altitude limited to max rate
   double cmdAltRate = (cmdAltitude - pp->getAltitudeM());
   if (cmdAltRate > maxAltRate) cmdAltRate = maxAltRate;
   else if (cmdAltRate < -maxAltRate) cmdAltRate = -maxAltRate;

   // Compute commanded flight path angle (gamma)
   double cmdPitch = 0;
   double vt = pp->getTotalVelocity();
   if (vt > 0) {
      cmdPitch = std::asin( cmdAltRate/vt );
   }

   // ---
   // Compute Max G
   // ---
   double gmax = gMax;                     // Max g,s
   if(pp->getTotalVelocity() < vpMaxG) {
      gmax = 1.0f + (gMax - 1.0f) * (pp->getTotalVelocity() - vpMin) / (vpMaxG - vpMin);
   }

   // ---
   // Computer max turn rate, max/min pitch rates
   // ---
   double ra_max = gmax * g / pp->getTotalVelocity();    // Turn rate base on vp and g,s (rad/sec)
   double qa_max = ra_max;                           // Max pull up pitch rate (rad/sec)
   double qa_min = -qa_max;                          // Max pushover pitch rate (rad/sec)
   if(gmax > 2.0) {
      // Max yaw rate (rad/sec)
      qa_min = -( 2.0f * g / pp->getTotalVelocity());
   }

   // ---
   // Get old angular values
   // ---
   const osg::Vec3 oldRates = pp->getAngularVelocities();
   //double pa1 = oldRates[simulation::Player::IROLL];
   double qa1 = oldRates[simulation::Player::IPITCH];
   double ra1 = oldRates[simulation::Player::IYAW];

   // ---
   // Find pitch rate and update pitch
   // ---
   double qa = base::Angle::aepcdRad(cmdPitch - pp->getPitchR()) * 0.1;
   if(qa > qa_max) qa = qa_max;
   if(qa < qa_min) qa = qa_min;

   // Find turn rate
   double ra = base::Angle::aepcdRad((cmdHeading  * base::Angle::D2RCC) - pp->getHeadingR()) * 0.1;
   if(ra > ra_max) ra = ra_max;
   if(ra < -ra_max) ra = -ra_max;

   // Damage
   double dd = pp->getDamage();
   if (dd > 0.5) {
      ra += (dd - 0.5) * ra_max;
      qa -= (dd - 0.5) * qa_max;
   }

   // Using Pitch rate, integrate pitch
   double newTheta = static_cast<double>(pp->getPitch() + (qa + qa1) * dt / 2.0);

   // Use turn rate integrate heading
   double newPsi = static_cast<double>(pp->getHeading() + (ra + ra1) * dt / 2.0);
   if(newPsi > 2.0 * base::PI) newPsi -= 2.0 * base::PI;
   if(newPsi < 0.0) newPsi += 2.0 * base::PI;

   // Roll angle is proportional to max turn rate - filtered
   double pa = 0.0;
   double newPhi = 0.98 * pp->getRollR() + 0.02 * (ra / ra_max * (base::Angle::D2RCC * 60.0));

   // Find Acceleration
   double cmdVelMPS = cmdVelocity * (base::Distance::NM2M / 3600.0);
   double vpdot = (cmdVelMPS - pp->getTotalVelocity()) * 0.05;
   if(vpdot > maxAccel)  vpdot = maxAccel;
   if(vpdot < -maxAccel) vpdot = -maxAccel;

   // Compute new velocity (body coordinates)
   double newVP = pp->getTotalVelocity() + vpdot * dt;

   // Set our angular values
   pp->setEulerAngles(newPhi, newTheta, newPsi);
   pp->setAngularVelocities(pa, qa, ra);

   // Set our velocity vector (body coordinates)
   pp->setVelocityBody( static_cast<double>(newVP), 0.0, 0.0);

   // Set our acceleration vector (body coordinates)
   pp->setAccelerationBody( static_cast<double>(vpdot), 0.0, 0.0);
}


//------------------------------------------------------------------------------
// slot methods
//------------------------------------------------------------------------------

bool RacModel::setSlotMinSpeed(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       vpMin = msg->getReal();
       ok = true;
    }
    return ok;
}

bool RacModel::setSlotSpeedMaxG(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       vpMaxG = msg->getReal();
       ok = true;
    }
    return ok;
}

bool RacModel::setSlotMaxG(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       gMax = msg->getReal();
       ok = true;
    }
    return ok;
}

bool RacModel::setSlotMaxAccel(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       maxAccel = msg->getReal();
       ok = true;
    }
    return ok;
}

bool RacModel::setSlotCmdAltitude(const base::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       double value = base::Meters::convertStatic( *msg );
       cmdAltitude = value;
       ok = true;
    }
    return ok;
}

bool RacModel::setSlotCmdHeading(const base::Angle* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       double value  = base::Degrees::convertStatic( *msg );
       cmdHeading = value;
       ok = true;
    }
    return ok;
}

bool RacModel::setSlotCmdVelocity(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       double value = msg->getReal();
       cmdVelocity = value;
       ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Graphic
//------------------------------------------------------------------------------
base::Object* RacModel::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& RacModel::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "minSpeed: " << vpMin << std::endl;

    indent(sout,i+j);
    sout << "speedMaxG: " << vpMaxG << std::endl;

    indent(sout,i+j);
    sout << "maxg: " << gMax << std::endl;

    indent(sout,i+j);
    sout << "maxAccel: " << maxAccel << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
