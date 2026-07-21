#include "ImportStrings.h"
#include "LocalizedStrings.h"

#include <vfs/Tools/vfs_tools.h>
#include <vfs/Core/vfs.h>

#include <cmath>

void Loc::ImportStrings()
{
	Loc::AssociateWithFile(Loc::AIM_BIOGRAPHY,	L"Localization/AimBiographies.xml");
	Loc::AssociateWithFile(Loc::AIM_HISTORY,	L"Localization/AimHistory.xml");
	Loc::AssociateWithFile(Loc::AIM_POLICY,		L"Localization/AimPolicy.xml");
	Loc::AssociateWithFile(Loc::GAME_STRINGS,	L"Localization/GameStrings.xml");

	vfs::String bio, add, bio2;
	Loc::GetString(Loc::AIM_BIOGRAPHY,L"Bio",L"0",add);
	Loc::GetString(Loc::AIM_BIOGRAPHY,L"Add",L"10",bio);
	Loc::GetString(Loc::AIM_BIOGRAPHY,L"Bio",23,bio2);

	for(int i=0; i<200; ++i)
	{
		std::wstringstream wss;
		for(int exp=2; exp>=0; --exp)
		{
			int t = (int)std::pow((double)10,(double)exp);
			wss << (i % (t*10)) / t;
		}
		vfs::String s = wss.str() + L".EDT.xml";
		vfs::Path filename(L"Localization/Dialogue");
		filename += vfs::Path(s);
		if(getVFS()->fileExists(filename))
		{
			Loc::AssociateWithFile(Loc::DIALOGUE,filename,vfs::toString<wchar_t>(i));
		}
	}
}
