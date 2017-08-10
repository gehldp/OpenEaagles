
#include "openeaagles/base/concurrent/ThreadPool.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/concurrent/ThreadPoolManager.hpp"
#include "openeaagles/base/concurrent/ThreadPoolThread.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(ThreadPool, "ThreadPool")
EMPTY_SERIALIZER(ThreadPool)

BEGIN_SLOTTABLE(ThreadPool)
   "numThreads",  // Number of threads to use - 0 = don't use threading
   "priority"     // Thread priority (zero(0) is lowest, one(1) is highest)
END_SLOTTABLE(ThreadPool)

BEGIN_SLOT_MAP(ThreadPool)
   ON_SLOT( 1, setSlotNumThreads, Number)
   ON_SLOT( 2, setSlotPriority,   Number)
END_SLOT_MAP()

ThreadPool::ThreadPool()
{
   STANDARD_CONSTRUCTOR()
}

ThreadPool::ThreadPool( ThreadPoolManager* mgr )
{
   STANDARD_CONSTRUCTOR()
   setManager(mgr);
}

ThreadPool::ThreadPool( ThreadPoolManager* mgr, const unsigned int num )
   : numThreads(num)
{
   STANDARD_CONSTRUCTOR()
   setManager(mgr);
}

ThreadPool::ThreadPool( ThreadPoolManager* mgr, const unsigned int num, const double pri )
   : numThreads(num), priority(pri)
{
   STANDARD_CONSTRUCTOR()
   setManager(mgr);
}

//------------------------------------------------------------------------------
// Public interface
//------------------------------------------------------------------------------

void ThreadPool::setManager(ThreadPoolManager* mgr)
{
   if (manager != nullptr) {
      manager->unref();
   }
   manager = mgr;
   if (manager != nullptr) {
      manager->ref();
   }
}

void ThreadPool::initialize(Component* const parent)
{
   destroy();

   // Create the thread pool
   if (numThreads > 0)
   {
      std::cout << "Running thread pool in multi-threaded mode" << std::endl;
      for (unsigned int i = 0; i < numThreads; i++)
      {
         //Get the callback object for this thread
         Object* callbackObj = nullptr;
         if (manager != nullptr)
            callbackObj = manager->initialize();

         //Add the thread to the master array
         allThreads[actualThreads] = new ThreadPoolThread(parent, this, manager, priority, callbackObj);

         //Create the thread
         bool ok = allThreads[actualThreads]->create();
         if (ok)
         {
            std::cout << "Created thread pool thread[" << actualThreads << "] = " << allThreads[actualThreads] << std::endl;
            availableThreads[actualThreads] = allThreads[actualThreads];
            actualThreads++;
         }
         else
         {
            allThreads[actualThreads]->unref();
            allThreads[actualThreads] = nullptr;
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "ThreadPool::initialize(): ERROR, failed to create a thread pool thread!" << std::endl;
            }
         }
      }
   }

   //Use single-threaded mode if we're not using threads or if threading failed
   if (actualThreads == 0)
   {
      std::cout << "Running thread pool in single-threaded mode" << std::endl;
      if(manager != nullptr)
         unthreadedObj = manager->initialize();
   }
}

void ThreadPool::execute()
{
   execute(nullptr);
}

void ThreadPool::execute(Object* cur)
{
   //If we're unthreaded, just use this thread
   if (actualThreads == 0)
   {
      if (manager != nullptr)
      {
         manager->prepare(unthreadedObj);
         manager->execute(unthreadedObj);
         manager->execute(unthreadedObj, cur);
      }
      return;
   }

   //Try to get an available thread from the pool
   ThreadPoolThread* availableThread = getAvailableThread();

   //If we didn't get one, we'll have to wait
   if (availableThread == nullptr)
   {
      //Wait for one to become available
      SyncTask** pp = reinterpret_cast<SyncTask**>( &allThreads[0] );
      if (SyncTask::waitForAnyCompleted(pp, actualThreads) == -1)
      {
         //Error
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "ThreadPool::execute(): ERROR, unknown error while waiting for completed thread signal!" << std::endl;
         }
         return;
      }

      //Should have at least one now - try again
      availableThread = getAvailableThread();
   }

   //Do we have one now (we should)?
   if (availableThread == nullptr)
   {
      //Error
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "ThreadPool::execute(): ERROR, could not get an available thread!" << std::endl;
      }
      return;
   }

   //Prepare the thread
   if (manager != nullptr)
      manager->prepare(availableThread->getPersistentObj());

   //Launch the thread
   availableThread->setCurrentObj(cur);
   availableThread->signalStart();
}

ThreadPoolThread* ThreadPool::getAvailableThread()
{
   ThreadPoolThread* availableThread = nullptr;
   lock(availableThreadsLock);
   for (int i = actualThreads - 1 ; i >= 0 ; i--)
   {
      if (availableThreads[i] != nullptr)
      {
         availableThread = availableThreads[i];
         availableThreads[i] = nullptr;
         break;
      }
   }
   unlock(availableThreadsLock);
   return availableThread;
}

void ThreadPool::threadAvailable(ThreadPoolThread* availableThread)
{
   lock(availableThreadsLock);
   for (unsigned int i = 0 ; i < actualThreads ; i++)
   {
      if (availableThreads[i] == nullptr)
      {
         availableThreads[i] = availableThread;
         break;
      }
   }
   unlock(availableThreadsLock);
}

void ThreadPool::destroy()
{
   //Delete all threads
   for (unsigned int i = 0; i < actualThreads; i++) {
      allThreads[i]->terminate();
      allThreads[i]->unref();
      allThreads[i] = nullptr;
   }
   lock(availableThreadsLock);
   for (unsigned int i = 0; i < actualThreads; i++) {
      availableThreads[i] = nullptr;
   }
   unlock(availableThreadsLock);
   actualThreads = 0;

   //Destroy the unthreaded object
   if (unthreadedObj != nullptr)
   {
      if (manager != nullptr)
         manager->destroy(unthreadedObj);
      unthreadedObj = nullptr;
   }
}

//------------------------------------------------------------------------------
// Object overloads
//------------------------------------------------------------------------------

void ThreadPool::copyData(const ThreadPool& org, const bool)
{
   BaseClass::copyData(org);
   destroy();

   // Copy the manager, number of threads, and priority
   if (org.manager != nullptr)
      setManager( static_cast<ThreadPoolManager*>(org.manager->clone()) );
   else
      setManager(nullptr);
   numThreads = org.numThreads;
   priority = org.priority;
}

void ThreadPool::deleteData()
{
   destroy();
   setManager(nullptr);
}

//------------------------------------------------------------------------------
// Slot methods
//------------------------------------------------------------------------------

bool ThreadPool::setSlotNumThreads(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int num = msg->getInt();
      if (num >= 0 && num <= static_cast<int>(MAX_THREADS)) {
         numThreads = static_cast<unsigned int>(num);
         ok = true;
      }
      else {
         std::cerr << "ThreadPool::setSlotNumThreads: numThreads is invalid, range: [0 .. " << MAX_THREADS << "]" << std::endl;
      }
   }
   return ok;
}

bool ThreadPool::setSlotPriority(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double pri = msg->getReal();
      if (pri >= 0 && pri <= 1.0f) {
         priority = pri;
         ok = true;
      }
      else {
         std::cerr << "ThreadPool::setSlotPriority: priority is invalid, range: [0 .. 1]" << std::endl;
      }
   }
   return ok;
}

}
}
