#include <progress_bar.h>


ja2xp::ProgressBar::ProgressBar(int size, int numElements)
: iSIZE(size), num_elements(numElements), count(0)
{
	empty_str.resize(iSIZE);
	for(int i=0; i<iSIZE; ++i)
	{
		empty_str[i] = L' ';
	}
};

void ja2xp::ProgressBar::Next(vfs::String const& message)
{
	count++;
	float percent = (float)count/float(num_elements);
	wchar_t bar[21] = L"                    ";
	int bar_end = (int)(20*percent);
	bar_end = std::min<int>(20,bar_end);
	for(int i=0; i < bar_end; ++i)
	{
		bar[i] = L'=';
	}
	std::vector<wchar_t> infoZ(iSIZE+1);
	std::wstring str = message.c_wcs();
	int start = std::min<int>(iSIZE,(int)str.length());
	wcsncpy(&infoZ[0],str.c_str(),start);
	memcpy(&infoZ[start],&empty_str[0],(iSIZE-start)*sizeof(wchar_t));
	infoZ[iSIZE] = 0;
	wprintf(L"\r[%.20s] - %.54s", bar, &infoZ[0]);
}
