#include "vfs_settings.h"

static bool s_UseUnicode = true;

void vfs::Settings::setUseUnicode(bool useUnicode)
{
	s_UseUnicode = useUnicode;
}

bool vfs::Settings::getUseUnicode()
{
	return s_UseUnicode;
}
