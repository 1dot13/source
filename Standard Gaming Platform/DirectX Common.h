#ifndef	__DirectX_Common_H__
#define	__DirectX_Common_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

	// local functions
	STR8 	DirectXErrorDescription ( INT32 iDXReturn );
	void	DirectXAttempt ( INT32 iErrorCode, INT32 nLine, STR8 szFilename );
	void	DirectXAssert ( BOOLEAN fValue, INT32 nLine, STR8 szFilename );
	void	DirectXZeroMem ( void* pMemory, int nSize );

	#undef	ATTEMPT
	#define	ATTEMPT(x)	DirectXAttempt ((x),__LINE__,__FILE__)

	#undef	ZEROMEM
	#define	ZEROMEM(x)	DirectXZeroMem ( (void*)&(x), sizeof(x) )

	#undef	DEBUGMSG
	#define	DEBUGMSG(x)	OutputDebugString(x)

#ifdef __cplusplus
}
#endif

#endif