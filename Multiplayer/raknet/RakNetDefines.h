/// Define __GET_TIME_64BIT to have RakNetTime use a 64, rather than 32 bit value.  A 32 bit value will overflow after about 5 weeks.
/// However, this doubles the bandwidth use for sending times, so don't do it unless you have a reason to.
/// Disabled by default.
// #define __GET_TIME_64BIT

/// Makes RakNet threadsafe
/// Define this if you use the same instance of RakPeer from multiple threads
/// Otherwise leave it undefined, since it makes things an order of magnitude slower.
/// Disabled by default
// #define _RAKNET_THREADSAFE

/// Define __BITSTREAM_NATIVE_END to NOT support endian swapping in the BitStream class.  This is faster and is what you should use
/// unless you actually plan to have different endianness systems connect to each other
/// Enabled by default.
#define __BITSTREAM_NATIVE_END

#if defined(_CONSOLE_2)
#undef __BITSTREAM_NATIVE_END
#endif

/// Maximum (stack) size to use with _alloca before using new and delete instead.
#define MAX_ALLOCA_STACK_ALLOCATION 1048576

// Use WaitForSingleObject instead of sleep.
// Defining it plays nicer with other systems, and uses less CPU, but gives worse RakNet performance
// Undefining it uses more CPU time, but is more responsive and faster.
#define USE_WAIT_FOR_MULTIPLE_EVENTS

// If you need it
#define RAKNET_VERSION "3.0 09/07/07"
