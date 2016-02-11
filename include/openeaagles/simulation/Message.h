//------------------------------------------------------------------------------
// Class: Message
//----------------------------------------------------------------------------------
#ifndef __oe_simulation_Message_H__
#define __oe_simulation_Message_H__

#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/List.h"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Class: Message
// Description: 'Generic' Message. Used as a base class for other generic messages
//----------------------------------------------------------------------------------
class Message : public basic::Object
{
   DECLARE_SUBCLASS(Message, basic::Object)

public:
   //left as enum for future use
   enum AckCodes { ACK_REQUESTED, ACK };

public:
   Message();

   const basic::String* getSenderName() const;  // Name of the message sender
   const basic::List* getDestNames() const;     // List of destination name String

   double getTimeStamp() const;     // time at which this message was sent (if on receive end)
   double getLifeSpan() const;      // length of time in seconds which this message is valid
   AckCodes getAckCode() const;     // the ack code for this message

   // set functions
   virtual void setSenderName(const basic::String* const sName);
   virtual void setSenderName(const char* const sName);
   virtual void setTimeStamp(const double ts);
   virtual void setLifeSpan(const double ls);
   virtual void setAckCode(const AckCodes a);

   // adds a destination name to the list of intended recipients of this message
   virtual bool addDestName(basic::String* const name);
   virtual bool addDestName(const char* const name);

   // removes all destination names that fully match the value passed in as a parameter
   virtual void removeDestName(basic::String* const name);

   // clears the list of destination names
   virtual void clearDestNames();

private:
   void initData();

   basic::String* senderName;    // name of sending player
   basic::List* destNames;       // player names to whome this message is intended for
   double timeStamp;             // time at which this message was sent
   double lifeSpan;              // seconds
   AckCodes ack;                 // ack code
};

//
inline const basic::String* Message::getSenderName() const              { return senderName;    }
inline const basic::List* Message::getDestNames() const                 { return destNames;     }
inline double Message::getTimeStamp() const                             { return timeStamp;     }
inline double Message::getLifeSpan() const                              { return lifeSpan;      }
inline Message::AckCodes Message::getAckCode() const                    { return ack;           }

inline void Message::setTimeStamp(const double ts)                      { timeStamp = ts;       }
inline void Message::setLifeSpan(const double ls)                       { lifeSpan = ls;        }
inline void Message::setAckCode(const AckCodes a)                       { ack = a;              }

} // End simulation
} // End oe Namespace

#endif
