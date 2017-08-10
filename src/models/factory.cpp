
#include "openeaagles/models/factory.hpp"

#include "openeaagles/base/Object.hpp"

// dynamics models
#include "openeaagles/models/dynamics/JSBSimModel.hpp"
#include "openeaagles/models/dynamics/RacModel.hpp"
#include "openeaagles/models/dynamics/LaeroModel.hpp"

// environment models
#include "openeaagles/models/environment/IrAtmosphere.hpp"
#include "openeaagles/models/environment/IrAtmosphere1.hpp"

// navigation models
#include "openeaagles/models/navigation/Bullseye.hpp"
#include "openeaagles/models/navigation/Gps.hpp"
#include "openeaagles/models/navigation/Ins.hpp"
#include "openeaagles/models/navigation/Navigation.hpp"
#include "openeaagles/models/navigation/Route.hpp"
#include "openeaagles/models/navigation/Steerpoint.hpp"

// player models
#include "openeaagles/models/player/Aam.hpp"
#include "openeaagles/models/player/Agm.hpp"
#include "openeaagles/models/player/AirVehicle.hpp"
#include "openeaagles/models/player/Bomb.hpp"
#include "openeaagles/models/player/Buildings.hpp"
#include "openeaagles/models/player/Bullet.hpp"
#include "openeaagles/models/player/Effects.hpp"
#include "openeaagles/models/player/GroundVehicle.hpp"
#include "openeaagles/models/player/LifeForms.hpp"
#include "openeaagles/models/player/Player.hpp"
#include "openeaagles/models/player/Missile.hpp"
#include "openeaagles/models/player/Sam.hpp"
#include "openeaagles/models/player/SamVehicles.hpp"
#include "openeaagles/models/player/Ships.hpp"
#include "openeaagles/models/player/SpaceVehicle.hpp"

// sensor models
#include "openeaagles/models/sensor/Gmti.hpp"
#include "openeaagles/models/sensor/Tws.hpp"
#include "openeaagles/models/sensor/Stt.hpp"

// world models
#include "openeaagles/models/WorldModel.hpp"

// system models
#include "openeaagles/models/system/AngleOnlyTrackManager.hpp"
#include "openeaagles/models/system/Antenna.hpp"
#include "openeaagles/models/system/Autopilot.hpp"
#include "openeaagles/models/system/AvionicsPod.hpp"
#include "openeaagles/models/system/CollisionDetect.hpp"
#include "openeaagles/models/system/Datalink.hpp"
#include "openeaagles/models/system/ExternalStore.hpp"
#include "openeaagles/models/system/FuelTank.hpp"
#include "openeaagles/models/system/Gimbal.hpp"
#include "openeaagles/models/system/Guns.hpp"
#include "openeaagles/models/system/Iff.hpp"
#include "openeaagles/models/system/IrSeeker.hpp"
#include "openeaagles/models/system/IrSensor.hpp"
#include "openeaagles/models/system/Jammer.hpp"
#include "openeaagles/models/system/MergingIrSensor.hpp"
#include "openeaagles/models/system/OnboardComputer.hpp"
#include "openeaagles/models/system/Pilot.hpp"
#include "openeaagles/models/system/Radar.hpp"
#include "openeaagles/models/system/Radio.hpp"
#include "openeaagles/models/system/RfSensor.hpp"
#include "openeaagles/models/system/Rwr.hpp"
#include "openeaagles/models/system/Sar.hpp"
#include "openeaagles/models/system/ScanGimbal.hpp"
#include "openeaagles/models/system/StabilizingGimbal.hpp"
#include "openeaagles/models/system/Stores.hpp"
#include "openeaagles/models/system/StoresMgr.hpp"
#include "openeaagles/models/system/System.hpp"
#include "openeaagles/models/system/TrackManager.hpp"

// misc
#include "openeaagles/models/Actions.hpp"
#include "openeaagles/models/AircraftIrSignature.hpp"

#include "openeaagles/models/IrShapes.hpp"
#include "openeaagles/models/IrSignature.hpp"
#include "openeaagles/models/Signatures.hpp"

#include "openeaagles/models/SimAgent.hpp"
#include "openeaagles/models/MultiActorAgent.hpp"

#include "openeaagles/models/TargetData.hpp"
#include "openeaagles/models/Track.hpp"

#include <string>

namespace oe {
namespace models {

base::Object* factory(const std::string& name)
{
   base::Object* obj = nullptr;

   // dynamics models
   if ( name == RacModel::getFactoryName() ) {              // RAC
      obj = new RacModel();
   }
   else if ( name == JSBSimModel::getFactoryName() ) {      // JSBSim
      obj = new JSBSimModel();
   }
   else if ( name == LaeroModel::getFactoryName() ) {       // Laero
      obj = new LaeroModel();
   }

   // environment
   else if ( name == IrAtmosphere::getFactoryName() ) {
      obj = new IrAtmosphere;
   }
   else if ( name == IrAtmosphere1::getFactoryName() ) {
      obj = new IrAtmosphere1;
   }

   // sensor models
   if ( name == Gmti::getFactoryName() ) {
      obj = new Gmti();
   }
   else if ( name == Stt::getFactoryName() ) {
      obj = new Stt();
   }
   else if ( name == Tws::getFactoryName() ) {
      obj = new Tws();
   }

   // world models
   else if ( name == WorldModel::getFactoryName() ) {
      obj = new WorldModel();
   }
   
   // Players
   else if ( name == Player::getFactoryName() ) {
      obj = new Player();
   }
   else if ( name == AirVehicle::getFactoryName() ) {
      obj = new AirVehicle();
   }
   else if ( name == Building::getFactoryName() ) {
      obj = new Building();
   }
   else if ( name == GroundVehicle::getFactoryName() ) {
      obj = new GroundVehicle();
   }
   else if ( name == LifeForm::getFactoryName() ) {
      obj = new LifeForm();
   }
   else if ( name == Ship::getFactoryName() ) {
      obj = new Ship();
   }
   else if ( name == SpaceVehicle::getFactoryName() ) {
      obj = new SpaceVehicle();
   }

   // Air Vehicles
   else if ( name == Aircraft::getFactoryName() ) {
      obj = new Aircraft();
   }
   else if ( name == Helicopter::getFactoryName() ) {
      obj = new Helicopter();
   }
   else if ( name == UnmannedAirVehicle::getFactoryName() ) {
      obj = new UnmannedAirVehicle();
   }

   // Ground Vehicles
   else if ( name == Tank::getFactoryName() ) {
      obj = new Tank();
   }
   else if ( name == ArmoredVehicle::getFactoryName() ) {
      obj = new ArmoredVehicle();
   }
   else if ( name == WheeledVehicle::getFactoryName() ) {
      obj = new WheeledVehicle();
   }
   else if ( name == Artillery::getFactoryName() ) {
      obj = new Artillery();
   }
   else if ( name == SamVehicle::getFactoryName() ) {
      obj = new SamVehicle();
   }
   else if ( name == GroundStation::getFactoryName() ) {
      obj = new GroundStation();
   }
   else if ( name == GroundStationRadar::getFactoryName() ) {
      obj = new GroundStationRadar();
   }
   else if ( name == GroundStationUav::getFactoryName() ) {
      obj = new GroundStationUav();
   }

   // Space Vehicles
   else if ( name == MannedSpaceVehicle::getFactoryName() ) {
      obj = new MannedSpaceVehicle();
   }
   else if ( name == UnmannedSpaceVehicle::getFactoryName() ) {
      obj = new UnmannedSpaceVehicle();
   }
   else if ( name == BoosterSpaceVehicle::getFactoryName() ) {
      obj = new BoosterSpaceVehicle();
   }

   // System
   else if ( name == System::getFactoryName() ) {
      obj = new System();
   }
   else if ( name == AvionicsPod::getFactoryName() ) {
      obj = new AvionicsPod();
   }

   // Basic Pilot types
   else if ( name == Pilot::getFactoryName() ) {
      obj = new Pilot();
   }
   else if ( name == Autopilot::getFactoryName() ) {
      obj = new Autopilot();
   }

   // Navigation types
   else if ( name == Navigation::getFactoryName() ) {
      obj = new Navigation();
   }
   else if ( name == Ins::getFactoryName() ) {
      obj = new Ins();
   }
   else if ( name == Gps::getFactoryName() ) {
      obj = new Gps();
   }
   else if ( name == Route::getFactoryName() ) {
      obj = new Route();
   }
   else if ( name == Steerpoint::getFactoryName() ) {
      obj = new Steerpoint();
   }

   // Target Data
   else if ( name == TargetData::getFactoryName() ) {
      obj = new TargetData();
   }

   // Bullseye
   else if ( name == Bullseye::getFactoryName() ) {
      obj = new Bullseye();
   }

   // Actions
   else if ( name == ActionImagingSar::getFactoryName() ) {
      obj = new ActionImagingSar();
   }
   else if ( name == ActionWeaponRelease::getFactoryName() ) {
      obj = new ActionWeaponRelease();
   }
   else if ( name == ActionDecoyRelease::getFactoryName() ) {
      obj = new ActionDecoyRelease();
   }
   else if ( name == ActionCamouflageType::getFactoryName() ) {
      obj = new ActionCamouflageType();
   }

   // Bombs and Missiles
   else if ( name == Bomb::getFactoryName() ) {
      obj = new Bomb();
   }
   else if ( name == Missile::getFactoryName() ) {
      obj = new Missile();
   }
   else if ( name == Aam::getFactoryName() ) {
      obj = new Aam();
   }
   else if ( name == Agm::getFactoryName() ) {
      obj = new Agm();
   }
   else if ( name == Sam::getFactoryName() ) {
      obj = new Sam();
   }

   // Effects
   else if ( name == Chaff::getFactoryName() ) {
      obj = new Chaff();
   }
   else if ( name == Decoy::getFactoryName() ) {
      obj = new Decoy();
   }
   else if ( name == Flare::getFactoryName() ) {
      obj = new Flare();
   }

   // Stores, stores manager and external stores (FuelTank, Gun & Bullets (used by the Gun))
   else if ( name == Stores::getFactoryName() ) {
      obj = new Stores();
   }
   else if ( name == SimpleStoresMgr::getFactoryName() ) {
      obj = new SimpleStoresMgr();
   }
   else if ( name == FuelTank::getFactoryName() ) {
      obj = new FuelTank();
   }
   else if ( name == Gun::getFactoryName() ) {
      obj = new Gun();
   }
   else if ( name == Bullet::getFactoryName() ) {
      obj = new Bullet();
   }

   // Data links
   else if ( name == Datalink::getFactoryName() ) {
      obj = new Datalink();
   }

   // Gimbals, Antennas and Optics
   else if ( name == Gimbal::getFactoryName() ) {
      obj = new Gimbal();
   }
   else if ( name == ScanGimbal::getFactoryName() ) {
      obj = new ScanGimbal();
   }
   else if ( name == StabilizingGimbal::getFactoryName() ) {
      obj = new StabilizingGimbal();
   }
   else if ( name == Antenna::getFactoryName() ) {
      obj = new Antenna();
   }
   else if ( name == IrSeeker::getFactoryName() ) {
      obj = new IrSeeker();
   }

   // R/F Signatures
   else if ( name == SigConstant::getFactoryName() ) {
      obj = new SigConstant();
   }
   else if ( name == SigSphere::getFactoryName() ) {
      obj = new SigSphere();
   }
   else if ( name == SigPlate::getFactoryName() ) {
      obj = new SigPlate();
   }
   else if ( name == SigDihedralCR::getFactoryName() ) {
      obj = new SigDihedralCR();
   }
   else if ( name == SigTrihedralCR::getFactoryName() ) {
      obj = new SigTrihedralCR();
   }
   else if ( name == SigSwitch::getFactoryName() ) {
      obj = new SigSwitch();
   }
   else if ( name == SigAzEl::getFactoryName() ) {
      obj = new SigAzEl();
   }
   // IR Signatures
   else if ( name == IrSignature::getFactoryName() ) {
      obj = new IrSignature();
   }
   else if ( name == AircraftIrSignature::getFactoryName() ) {
      obj = new AircraftIrSignature;
   }
   else if ( name == IrShape::getFactoryName() ) {
      obj = new IrShape();
   }
   else if ( name == IrSphere::getFactoryName() ) {
      obj = new IrSphere();
   }
   else if ( name == IrBox::getFactoryName() ) {
      obj = new IrBox();
   }
    // Onboard Computers
   else if ( name == OnboardComputer::getFactoryName() ) {
      obj = new OnboardComputer();
   }
   // Radios
   else if ( name == Radio::getFactoryName() ) {
      obj = new Radio();
   }
   else if ( name == CommRadio::getFactoryName() ) {
      obj = new CommRadio();
   }
   else if ( name == Iff::getFactoryName() ) {
      obj = new Iff();
   }
   // Sensors
   else if ( name == RfSensor::getFactoryName() ) {
      obj = new RfSensor();
   }
   else if ( name == SensorMgr::getFactoryName() ) {
      obj = new SensorMgr();
   }
   else if ( name == Radar::getFactoryName() ) {
      obj = new Radar();
   }
   else if ( name == Rwr::getFactoryName() ) {
      obj = new Rwr();
   }
   else if ( name == Sar::getFactoryName() ) {
      obj = new Sar();
   }
   else if ( name == Jammer::getFactoryName() ) {
      obj = new Jammer();
   }
   else if ( name == IrSensor::getFactoryName() ) {
      obj = new IrSensor();
   }
   else if ( name == MergingIrSensor::getFactoryName() ) {
      obj = new MergingIrSensor();
   }

   // Tracks
   else if ( name == Track::getFactoryName() ) {
      obj = new Track();
   }

   // Track Managers
   else if ( name == GmtiTrkMgr::getFactoryName() ) {
      obj = new GmtiTrkMgr();
   }
   else if ( name == AirTrkMgr::getFactoryName() ) {
      obj = new AirTrkMgr();
   }
   else if ( name == RwrTrkMgr::getFactoryName() ) {
      obj = new RwrTrkMgr();
   }
   else if ( name == AirAngleOnlyTrkMgr::getFactoryName() ) {
      obj = new AirAngleOnlyTrkMgr();
   }

   // UBF Agents
   else if ( name == SimAgent::getFactoryName() ) {
      obj = new SimAgent();
   }
   else if ( name == MultiActorAgent::getFactoryName() ) {
      obj = new MultiActorAgent();
   }

   // Collision detection component
   else if ( name == CollisionDetect::getFactoryName() ) {
      obj = new CollisionDetect();
   }

   return obj;
}

}
}
