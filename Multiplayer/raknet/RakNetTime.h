#ifndef __RAKNET_TIME_H
#define __RAKNET_TIME_H

// Define __GET_TIME_64BIT if you want to use large types for GetTime (takes more bandwidth when you transmit time though!)
// You would want to do this if your system is going to run long enough to overflow the millisecond counter (over a month)
#ifdef __GET_TIME_64BIT
#if defined(_MSC_VER) && _MSC_VER < 1300
typedef unsigned __int64 RakNetTime;
typedef unsigned __int64 RakNetTimeNS;
typedef unsigned __int64 RakNetTimeMS;
typedef unsigned __int64 RakNetTimeUS;
#else
typedef unsigned long long RakNetTime;
typedef unsigned long long RakNetTimeNS;
typedef unsigned long long RakNetTimeMS;
typedef unsigned long long RakNetTimeUS;
#endif
#else
typedef unsigned int RakNetTime;
typedef unsigned int RakNetTimeMS;
#if defined(_MSC_VER) && _MSC_VER < 1300
typedef unsigned __int64 RakNetTimeNS;
typedef unsigned __int64 RakNetTimeUS;
#else
typedef unsigned long long RakNetTimeNS;
typedef unsigned long long RakNetTimeUS;
#endif
#endif

#endif
