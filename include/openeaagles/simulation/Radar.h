//------------------------------------------------------------------------------
// Class: Radar
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Radar_H__
#define __oe_simulation_Radar_H__

#include "openeaagles/simulation/RfSensor.h"
#include "openeaagles/basic/safe_queue.h"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Class: Radar
// Description: Generic Radar Model
//
// Default R/F sensor type ID is "RADAR"
//
// Factory name: Radar
// Slots:
//    igain    <basic::Number>     ! Integrator gain (no units; default: 1.0f)
//             <basic::Decibel>    ! Integrator gain (dB)
//
//------------------------------------------------------------------------------
class Radar : public RfSensor
{
   DECLARE_SUBCLASS(Radar,RfSensor)

public:
   // Max number of reports (per scan)
   static const unsigned int MAX_REPORTS = OE_CONFIG_MAX_REPORTS;

   static const unsigned int NUM_SWEEPS = 121;          // Number of sweeps in Real-Beam display
   static const unsigned int PTRS_PER_SWEEP = 128;      // Number of points per sweep in RB display

public:
   Radar();

   const LCreal* getSweep(const unsigned int n) const     { return (n < NUM_SWEEPS ?  sweeps[n] : 0); }
   const LCreal* getClosure(const unsigned int n) const   { return (n < NUM_SWEEPS ?  vclos[n]  : 0); }
   unsigned int getNumSweeps() const                      { return NUM_SWEEPS; }
   unsigned int getPtrsPerSweep() const                   { return PTRS_PER_SWEEP; }

   unsigned int getMaxReports() const                     { return MAX_REPORTS; }
   unsigned int getNumReports() const                     { return numReports; }

   // Returns the number of emission reports, up to 'max', that are loaded into the 'list'
   // Emission pointers are pre-ref()'d, so unref() when finished.
   unsigned int getReports(const Emission** list, const unsigned int max) const;

   // For debugging purposes
   // returns the amount of jamming signal to be considered, 0 if no jamming
   LCreal getRecvJamSignal() const                 { return currentJamSignal; }
   LCreal getRecvJamSignalDb() const               { return 10.0f * lcLog10(currentJamSignal); }

   // Returns integration gain
   LCreal getIGain() const                         { return rfIGain; }

   // return the current number of emissions that have been jammed.
   int getNumberOfJammedEmissions() const          { return numberOfJammedEmissions; }

   // Sets integration gain
   virtual bool setIGain(const LCreal);

   // Slot functions
   virtual bool setSlotIGain(basic::Number* const msg);

   bool killedNotification(Player* const killedBy = 0) override;

   void updateData(const LCreal dt = 0.0) override;
   void reset() override;

protected:
   bool onEndScanEvent(const basic::Integer* const bar) override;

   void transmit(const LCreal dt) override;
   void receive(const LCreal dt) override;
   void process(const LCreal dt) override;

   bool shutdownNotification() override;

protected: // (#temporary#) allow subclasses to access and use report queue

   // Semaphore to protect 'rptQueue', 'rptSnQueue', 'reports' and 'rptMaxSn'
   mutable long myLock;

   // Queues
   basic::safe_queue<Emission*>   rptQueue;       // Reporting emission queue
   basic::safe_queue<LCreal>      rptSnQueue;     // Reporting Signal/Nose queue  (dB)

   // Reports
   Emission*   reports[MAX_REPORTS];   // Best emission for this report
   LCreal      rptMaxSn[MAX_REPORTS];  // Signal/Nose value            (dB)
   unsigned int numReports;            // Number of reports this sweep

private:
   void initData();
   void clearTracksAndQueues();
   void clearSweep(const unsigned int i);
   void ageSweeps();
   unsigned int computeSweepIndex(const LCreal az);
   unsigned int computeRangeIndex(const LCreal rng);

   bool        endOfScanFlg;           // End of scan flag

   LCreal      sweeps[NUM_SWEEPS][PTRS_PER_SWEEP];
   LCreal      vclos[NUM_SWEEPS][PTRS_PER_SWEEP];
   int         csweep;                     // Current sweep

   LCreal      currentJamSignal;
   int         numberOfJammedEmissions;

   LCreal      rfIGain;                // Integrator gain (default: 1.0) (no units)
};

} // End simulation namespace
} // End oe namespace

#endif
