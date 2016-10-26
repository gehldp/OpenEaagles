
#include "openeaagles/models/sensors/Stt.hpp"

#include "openeaagles/simulation/Antenna.hpp"
#include "openeaagles/simulation/Player.hpp"
#include "openeaagles/simulation/Track.hpp"
#include "openeaagles/simulation/TrackManager.hpp"

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Stt, "Stt")
EMPTY_DELETEDATA(Stt)

Stt::Stt()
{
    STANDARD_CONSTRUCTOR()
}

void Stt::copyData(const Stt& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// dynamics() --  Update dynamics
//------------------------------------------------------------------------------
void Stt::dynamics(const double dt)
{
    BaseClass::dynamics(dt);

    // ---
    // Update the antenna's Reference position
    // ---
    simulation::TrackManager* tm = getTrackManager();
    if (getAntenna() != nullptr && getOwnship() != nullptr && tm != nullptr) {

        base::safe_ptr<simulation::Track> trackList[2];
        int n = tm->getTrackList(trackList,2);

        if (n > 0) {
            // ---
            // Point the antenna at the first track
            // ---

            // Relative position vector to track
            osg::Vec3d dpoi = trackList[0]->getPosition();

            // rotate to ownship heading
            double sinHdg = getOwnship()->getSinHeading();
            double cosHdg = getOwnship()->getCosHeading();
            double x =  dpoi[simulation::Player::INORTH] * cosHdg + dpoi[simulation::Player::IEAST] * sinHdg;
            double y = -dpoi[simulation::Player::INORTH] * sinHdg + dpoi[simulation::Player::IEAST] * cosHdg;
            double z = dpoi[simulation::Player::IDOWN];

            // Compute az & el to track
            double grng = std::sqrt(x*x + y*y);
            double az = std::atan2(y,x);
            double el = std::atan2(-z,grng);

            // Get current antenna limits
            double leftLim(0.0), rightLim(0.0);
            double lowerLim(0.0), upperLim(0.0);
            getAntenna()->getAzimuthLimits(&leftLim, &rightLim);
            getAntenna()->getElevationLimits(&lowerLim, &upperLim);

            // Limit to antenna limits
            if (az < leftLim) az = leftLim;
            else if (az > rightLim) az = rightLim;
            if (el < lowerLim) el = lowerLim;
            else if (el > upperLim) el = upperLim;

            // Set the reference 'look' angles and conical scan mode
            getAntenna()->setRefAzimuth(az);
            getAntenna()->setRefElevation(el);
            getAntenna()->setScanMode(simulation::Antenna::CONICAL_SCAN);
        }
        else {
            // ---
            // when we don't have any tracks, enter the default (from input file) search mode
            // ---
            getAntenna()->setRefAzimuth(0.0);
            getAntenna()->setRefElevation(0.0);
            getAntenna()->setScanMode(simulation::Antenna::HORIZONTAL_BAR_SCAN);
        }
    }
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Stt::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // ###DPG need to print slots
    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
