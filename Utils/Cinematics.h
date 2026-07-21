#ifndef _CINEMATICS_H_
#define _CINEMATICS_H_

#include "types.h"

struct SMKFLIC;

void			SmkInitialize();
void			SmkShutdown();
SMKFLIC			*SmkPlayFlic(const CHAR8 *filename, UINT32 left, UINT32 top, BOOLEAN autoClose);
BOOLEAN			SmkPollFlics();
void			SmkCloseFlic(SMKFLIC *flic);

#endif
