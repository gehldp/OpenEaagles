
#include "openeaagles/simulation/SimTcThread.hpp"

#include "openeaagles/simulation/Simulation.hpp"

#include "openeaagles/base/Component.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(SimTcThread, "SimTcThread")
EMPTY_SLOTTABLE(SimTcThread)
EMPTY_COPYDATA(SimTcThread)
EMPTY_DELETEDATA(SimTcThread)
EMPTY_SERIALIZER(SimTcThread)

SimTcThread::SimTcThread(base::Component* const parent, const double priority): base::SyncTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()
}

void SimTcThread::start(
         base::PairStream* const pl1,
         const double dt1,
         const unsigned int idx1,
         const unsigned int n1
      )
{
   pl0 = pl1;
   dt0 = dt1;
   idx0 = idx1;
   n0 = n1;

   signalStart();
}

unsigned long SimTcThread::userFunc()
{
   // Make sure we've a player list and our index is valid ...
   if (pl0 != nullptr && idx0 > 0 && idx0 <= n0) {
      // then call the simulation executives update TC player list functions
      Simulation* sim = static_cast<Simulation*>(getParent());
      sim->updateTcPlayerList(pl0, dt0, idx0, n0);
   }

   return 0;
}

}
}
