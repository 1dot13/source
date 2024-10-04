#pragma once
#include "Types.h"

void TimingLogInitialize(const CHAR8* filename);
void TimingLog(const CHAR8* logEvent, int n);
void TimingLogTotalTime(const CHAR8* logEvent, int n);
void TimingLogWrite(const CHAR8* text);
void TimingLogStop();
