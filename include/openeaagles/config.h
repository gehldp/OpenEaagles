// =============================================================================
// Main configuration file for setting framework parameters
//
// Edit as needed, or redefine using compiler command line options
//
// Used within C++ source code and EDL/EPP files
// Important: only define C pre-processor directives in this file
//
// =============================================================================
#ifndef __oe_config_H__
#define __oe_config_H__

// OpenEaagles release version number (16.03)
#ifndef OE_VERSION
#define OE_VERSION                         1603
#endif

// Max number of 'registered' classes (see Object.h)
#ifndef OE_CONFIG_MAX_CLASSES
#define OE_CONFIG_MAX_CLASSES              1000
#endif

// Max number of interval timers (see Timers.h)
#ifndef OE_CONFIG_MAX_INTERVAL_TIMERS
#define OE_CONFIG_MAX_INTERVAL_TIMERS      500
#endif

// Max number of "player's of interest" (see Gimbal.h)
#ifndef OE_CONFIG_MAX_PLAYERS_OF_INTEREST
#define OE_CONFIG_MAX_PLAYERS_OF_INTEREST  4000
#endif

// Max size of the RF emission queues (see RfSystem.h)
#ifndef OE_CONFIG_RF_MAX_EMISSIONS
#define OE_CONFIG_RF_MAX_EMISSIONS         800
#endif

// Max tracks (see TrackManager.h)
#ifndef OE_CONFIG_MAX_TRACKS
#define OE_CONFIG_MAX_TRACKS               200
#endif

// Max number of radar reports (see Radar.h and TrackManager.h)
#ifndef OE_CONFIG_MAX_REPORTS
#define OE_CONFIG_MAX_REPORTS              200
#endif

// Max number of networked entites (see NetIO.h)
#ifndef OE_CONFIG_MAX_NETIO_ENTITIES
#define OE_CONFIG_MAX_NETIO_ENTITIES       5000
#endif

// Max number of networked entity types (see NetIO.h)
#ifndef OE_CONFIG_MAX_NETIO_ENTITY_TYPES
#define OE_CONFIG_MAX_NETIO_ENTITY_TYPES   1000
#endif

//  Max number of new, outgoing players published per frame (see NetIO.h)
#ifndef OE_CONFIG_MAX_NETIO_NEW_OUTGOING
#define OE_CONFIG_MAX_NETIO_NEW_OUTGOING   150
#endif

#endif
