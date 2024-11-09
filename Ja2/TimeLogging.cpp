#include "TimeLogging.h"
#include "time.h"
#include <stdio.h>


clock_t starttime;
clock_t t0;
clock_t t1;
FILE* fp_timelog = nullptr;


static void indent(int n)
{
	for (int i = 0; i < n; i++)
		fputc('\t', fp_timelog);
}

void TimingLogInitialize(const CHAR8* filename)
{
	starttime = clock();
	t1 = starttime;

	if (!fp_timelog)
	{
		fp_timelog = fopen(filename, "a");
	}
}


void TimingLog(const CHAR8* logEvent, int n)
{
	if (fp_timelog)
	{
		t0 = t1;
		t1 = clock();
		fprintf(fp_timelog, "%s", logEvent);
		indent(n);
		fprintf(fp_timelog, ": %f s\n", ((float)(t1 - t0) / CLOCKS_PER_SEC));
	}
}


void TimingLogTotalTime(const CHAR8* logEvent, int n)
{
	if (fp_timelog)
	{
		t1 = clock();
		fprintf(fp_timelog, "%s", logEvent);
		indent(n);
		fprintf(fp_timelog, ": %f s\n", ((float)(t1 - starttime) / CLOCKS_PER_SEC));
	}
}


void TimingLogWrite(const CHAR8* text)
{
	if (fp_timelog)
	{
		fprintf(fp_timelog, text);
	}
}


void TimingLogStop()
{
	if (fp_timelog)
	{
		fprintf(fp_timelog, "\n");
		fclose(fp_timelog);
		fp_timelog = nullptr;
	}
}
