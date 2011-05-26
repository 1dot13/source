#ifndef _PROGRESSBAR_H_
#define _PROGRESSBAR_H_

#include <vector>
#include <vfs/Core/vfs_string.h>

namespace ja2xp
{
	class ProgressBar
	{
	public:
		ProgressBar(int size, int numElements);

		void Next(vfs::String const& message);
	private:
		const int iSIZE;
		const int num_elements;
		int count;
		std::vector<wchar_t> empty_str;
	};
};

#endif // _PROGRESSBAR_H_
