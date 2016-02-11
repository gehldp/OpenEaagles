//------------------------------------------------------------------------------
// Classes: TrackManager, AirTrkMgr, GmtiGrkMgr, RwrGrkMgr
//------------------------------------------------------------------------------
#ifndef __oe_simulation_TrackManager_H__
#define __oe_simulation_TrackManager_H__

#include "openeaagles/simulation/System.h"
#include "openeaagles/basic/safe_queue.h"

namespace oe {
namespace simulation {

class Emission;
class Player;
class Track;

//==============================================================================
// Class: TrackManager
//
// Description: Abstract class for Track Managers
//
// Factory name: TrackManager
// Slots:
//    maxTracks       <Number>   ! Maximum number of tracks (default: MAX_TRKS)
//
//    maxTrackAge     <Time>     ! Maximum track age (default: 3) ### NES: the comment in the src says 2 sec
//    maxTrackAge     <Number>   ! Maximum track age (seconds)
//
//    firstTrackId    <Integer>  ! First track ID (default: 1000)
//
//    alpha           <Number>   ! Alpha (default: 1.0)
//    beta            <Number>   ! Beta  (default: 0.0)
//    gamma           <Number>   ! Gamma (default: 0.0)
//
//    logTrackUpdates <Boolean>  ! True to log all updates to tracks (default: true)
//
//==============================================================================
class TrackManager : public System
{
   DECLARE_SUBCLASS(TrackManager,System)

public:
   TrackManager();

   virtual LCreal getMaxTrackAge() const;
   virtual bool setMaxTrackAge(const double sec);

   virtual unsigned int getMaxTracks() const;
   virtual unsigned int getNumTracks() const;

   virtual int getTrackList(basic::safe_ptr<Track>* const slist, const unsigned int max) const;
   virtual int getTrackList(basic::safe_ptr<const Track>* const slist, const unsigned int max) const;

   // Note: Tracks have been ref() before being returned and need to
   // be unref() by the user.
   virtual int getTrackList(Track* tlist[], const unsigned int max);
   virtual int getTrackList(const Track* tlist[], const unsigned int max) const;

   // Type of tracks managed (see enum TypeBits in Track.h)
   virtual bool isType(const short t) const;
   virtual short getType() const;
   virtual void setType(const short t);
   virtual void setSubtype(const short t);

   virtual bool getLogTrackUpdates() const;
   virtual bool setLogTrackUpdates(const bool b);

   // Add a track
   virtual bool addTrack(Track* const t);

   // Clear all tracks and queues
   virtual void clearTracksAndQueues();

   // Add a new emission report (RF track managers only)
   virtual void newReport(Emission* em, LCreal snDbl);

   bool killedNotification(Player* const killedBy = 0) override;

   void reset() override;

protected:
   static const unsigned int MAX_TRKS = OE_CONFIG_MAX_TRACKS;         // Max tracks
   static const unsigned int MAX_REPORTS = OE_CONFIG_MAX_REPORTS;     // Max number of reports

   unsigned int getNewTrackID()                             { return nextTrkId++; }

   virtual void processTrackList(const LCreal dt) =0;                   // Derived class unique

   virtual Emission* getReport(LCreal* const sn);                       // Get the next 'new' report from the queue
   virtual bool setSlotMaxTracks(const basic::Number* const num);       // Sets the maximum number of track files
   virtual bool setSlotMaxTrackAge(const basic::Number* const num);     // Sets the maximum age of tracks
   virtual bool setSlotFirstTrackId(const basic::Number* const num);    // Sets the first (starting) track id number
   virtual bool setSlotAlpha(const basic::Number* const num);           // Sets alpha
   virtual bool setSlotBeta(const basic::Number* const num);            // Sets beta
   virtual bool setSlotGamma(const basic::Number* const num);           // Sets gamma
   virtual bool setSlotLogTrackUpdates(const basic::Number* const num); // Sets logTrackUpdates

   // Track List
   Track*              tracks[MAX_TRKS];   // Tracks
   unsigned int        nTrks;              // Number of tracks
   unsigned int        maxTrks;            // Max number of tracks (input)
   mutable long        trkListLock;        // Semaphore to protect the track list

   // Prediction parameters
   void makeMatrixA(const LCreal dt);
   LCreal              A[3][3];            // A Matrix
   bool                haveMatrixA;        // Matrix A has be initialized
   LCreal              alpha;              // Alpha parameter
   LCreal              beta;               // Beta parameter
   LCreal              gamma;              // Gamma parameter

   unsigned int        nextTrkId;          // Next track ID
   unsigned int        firstTrkId;         // First (starting) track ID

   basic::safe_queue<Emission*>   emQueue; // Emission input queue
   basic::safe_queue<LCreal>      snQueue; // S/N input queue.
   mutable long        queueLock;          // Semaphore to protect both emQueue and snQueue

   // System class Interface -- phase() callbacks
   void process(const LCreal dt) override;     // Phase 3

   // basic::Component protected interface
   bool shutdownNotification() override;

private:
   void initData();

   LCreal              maxTrackAge;        // Max Track age (sec)
   short               type;               // Track type: the bit-wise OR of various type bits (see enum TypeBits in Track.h)
   bool                logTrackUpdates;    // input slot; if false, updates to tracks are not logged.
};

//==============================================================================
// Class: AirTrkMgr
//
// Description: Track Manager for A/A modes (e.g., TWS, ACM, SST)
// Factory name: AirTrkMgr
// Slots:
//   positionGate   <basic::Number>  ! Position Gate (meters) (default: 2.0f * NM2M)
//   rangeGate      <basic::Number>  ! Range Gate (meters) (default: 500.0f)
//   velocityGate   <basic::Number>  ! Velocity Gate (m/s) (default: 10.0f)
//
//==============================================================================
class AirTrkMgr : public TrackManager
{
   DECLARE_SUBCLASS(AirTrkMgr,TrackManager)
public:
   AirTrkMgr();

   LCreal getPosGate()                             { return posGate;}
   LCreal getRngGate()                             { return rngGate;}
   LCreal getVelGate()                             { return velGate;}

protected:
   void processTrackList(const LCreal dt) override;

private:
   void initData();
   bool setPositionGate(const basic::Number* const num);
   bool setRangeGate(const basic::Number* const num);
   bool setVelocityGate(const basic::Number* const num);

   // Prediction parameters
   LCreal              posGate;            // Position Gate (meters)
   LCreal              rngGate;            // Range Gate (meters)
   LCreal              velGate;            // Velocity Gate (m/s)

   // Used by processTrackList()
   bool** report2TrackMatch;                 // Report/Track matched matrix
   unsigned int* reportNumMatches;           // Number of matches for each report
   unsigned int* trackNumMatches;            // Number of matcher for each track
};

//==============================================================================
// Class: GmtiTrkMgr
//
// Description: Very simple Ground Moving Target Indication (GMTI) Track Manager
// Factory name: GmtiTrkMgr
//
//==============================================================================
class GmtiTrkMgr : public TrackManager
{
   DECLARE_SUBCLASS(GmtiTrkMgr,TrackManager)
public:
   GmtiTrkMgr();
protected:
   void processTrackList(const LCreal dt) override;

private:
   void initData();

   // Used by processTrackList()
   bool** report2TrackMatch;                 // Report/Track matched matrix
   unsigned int* reportNumMatches;           // Number of matches for each report
   unsigned int* trackNumMatches;            // Number of matcher for each track
};

//==============================================================================
// Class: RwrTrkMgr
//
// Description: RADAR Warning Receiver (RWR) Track Manager
// Factory name: RwrTrkMgr
//
//==============================================================================
class RwrTrkMgr : public TrackManager
{
   DECLARE_SUBCLASS(RwrTrkMgr,TrackManager)
public:
   RwrTrkMgr();
protected:
   void processTrackList(const LCreal dt) override;

private:
   void initData();

   // Used by processTrackList()
   bool** report2TrackMatch;                 // Report/Track matched matrix
   unsigned int* reportNumMatches;           // Number of matches for each report
   unsigned int* trackNumMatches;            // Number of matcher for each track
};

} // End simulation namespace
} // End oe namespace

#endif
