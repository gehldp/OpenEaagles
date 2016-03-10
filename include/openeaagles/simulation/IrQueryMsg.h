//------------------------------------------------------------------------------
// Class: IrQueryMsg
//------------------------------------------------------------------------------
#ifndef __oe_simulation_IrQueryMsg_H__
#define __oe_simulation_IrQueryMsg_H__

#include "openeaagles/simulation/SensorMsg.h"

namespace oe {

namespace base { class List; }

namespace simulation {

class IrSensor;

//------------------------------------------------------------------------------
// Class: IrQueryMsg
//
// Description: General IR query message -- Used to request an IR Signature
//
// Factory name: IrQueryMsg
//
//------------------------------------------------------------------------------
class IrQueryMsg : public SensorMsg
{
    DECLARE_SUBCLASS(IrQueryMsg, SensorMsg)

public:
   IrQueryMsg();

      // Angle of target off gimbal boresight
   const double getAngleOffBoresight() const { return angleOffBoresight; }

   // Set angle off gimbal boresight
   void setAngleOffBoresight(const double aob) { angleOffBoresight = aob; }

   // angleAspect to target
   const double getAngleAspect() const { return angleAspect; }

   // Set relative azimuth
   void setRelativeAzimuth(const double & n) { raz = n; }

   // Relative azimuth of target
   const double getRelativeAzimuth() const { return raz; }

   // Set relative azimuth
   void setRelativeElevation(const double & n) { rel = n; }

   // Relative azimuth of target
   const double getRelativeElevation() const { return rel; }

   // Set angle aspect.
   void setAngleAspect(const double & n) { angleAspect = n; }

   // Position of target player
   const osg::Vec3& getPosVec() const { return pos; }

   // Sets the target position
   void setPosVec(const osg::Vec3& v) { pos = v; }

   // Velocity of target player
   const osg::Vec3& getVelocityVec() const { return vel; }

   // Sets the target velocity
   void setVelocityVec(const osg::Vec3& v) { vel = v; }

   // acceleration of target player
   const osg::Vec3& getAccelVec() const { return accel; }

   // Sets the target acceleration
   void setAccelVec(const osg::Vec3& v) { accel = v; }

   // Lower wavelength  (microns)
   double getLowerWavelength() const {return lowerWavelength;}

   //Set the lower wavelength (microns)
   void setLowerWavelength(const double lw) {lowerWavelength = lw; }

   // Upper wavelength  (microns)
   double getUpperWavelength() const {return upperWavelength; }

   //Set the upper wavelength (microns)
   void setUpperWavelength(const double uw) {upperWavelength = uw; }

   //Instantaneous Field of View
   double getInstantaneousFieldOfView() const {return instantaneousFieldOfView; }

   //Set the instantaneous Field of View
   void setInstantaneousFieldOfView(const double ifov) {instantaneousFieldOfView = ifov; }

   // Noise Equivalent Irradiance
   double getNEI() const {return nei;}

   // Set the Noise Equivalent Irradiance
   void setNEI(const double n) {nei = n; }

   //Signature at Range
   double getSignatureAtRange() const {return signatureAtRange; }

   //Set the Signature at Range
   void setSignatureAtRange (const double sig) {signatureAtRange = sig; }

   //Signature by waveband
   const double* getSignatureByWaveband() const {return signatureByWaveband; }

   //Set the signature by waveband
   void setSignatureByWaveband (double* const sig);

   //Transmissivity
   double getEmissivity() const {return emissivity; }

   //Set the transmissivity
   void setEmissivity (const double emiss) {emissivity = emiss; }

   //Projected Area in field of view
   double getProjectedArea() const {return projectedArea; }

   //Set the projected area in the field of view
   void setProjectedArea(const double area) {projectedArea = area; }

   double getSignalToNoiseRatio() const {return signalToNoiseRatio;}
   void setSignalToNoiseRatio (double const newSN) {signalToNoiseRatio = newSN; }

   double getBackgroundNoiseRatio() const {return backgroundNoiseRatio;}
   void setBackgroundNoiseRatio (double const newSN) {backgroundNoiseRatio = newSN; }

   IrSensor* getSendingSensor() {return sendingSensor;}
   const IrSensor* getSendingSensor() const {return sendingSensor;}
   void setSendingSensor(IrSensor* const newSensor);

   // IrQueryMsg class interface
   virtual void clearIrSignature();

   void setRange(const double r) override;
   void clear() override;

   // FAB - valuable to keep info about merging
   enum MergedQueryStatus {
      NOT_MERGED,     // 0 = this query not involved in a merging
      MERGED,         // 1 = one or more other queries merged into this query
      MERGED_OUT      // 2 = this query merged into another query
   };

   MergedQueryStatus getQueryMergeStatus() const { return mergedQueryStatus; }
   void setQueryMergeStatus(MergedQueryStatus status) { mergedQueryStatus = status; }

private:

   double         lowerWavelength;           //Lower wavelength          (microns)
   double         upperWavelength;           //Upper wavelength          (microns)
   double         instantaneousFieldOfView;  //IFOV                      (steradians)
   double         nei;                       //NEI
   double         signatureAtRange;          //Transmitted signal
   double* signatureByWaveband;              //Transmitted signal by waveband  2d array where x = number of
                                             // waveband bins, y = 0 (lower wavelength) , 1 (upper wavelength), 2 (sig)
   double         emissivity;                //Emissivity (unitless)
   double         projectedArea;             //Projected area observed   (m^2)
   double         signalToNoiseRatio;        // ratio of Signal to Noise
   double         backgroundNoiseRatio;      // ratio of background noise
   IrSensor*      sendingSensor;             //sensor that reported this message.
   osg::Vec3      pos;                       // position of the target.
   osg::Vec3      vel;                       // velocity of target.
   osg::Vec3      accel;                     // acceleration of target.
   double         angleAspect;               // Angle aspect to target.
   double         angleOffBoresight;         // Angle of target off boresight of sensor
   double         rel;                       // The relative elevation from target to sensor
   double         raz;                       // The relative azimuth from target to sensor
   MergedQueryStatus mergedQueryStatus;      // Flags status of query merging   // FAB
};

} // End simulation namespace
} // End oe namespace

#endif
