#ifndef _TRANSFER_RULES_H_
#define _TRANSFER_RULES_H_

#include <vfs/Core/vfs_path.h>
#include <vfs/Core/Interface/vfs_file_interface.h>

#include <list>

class CTransferRules
{
public:
	enum EAction
	{
		DENY,
		ACCEPT,
	};
public:
	CTransferRules();

	bool	initFromTxtFile(vfs::Path const& sPath);
	bool	initFromTxtFile(vfs::tReadableFile* pFile);

	void	setDefaultAction(EAction act);
	EAction	getDefaultAction();

	EAction	applyRule(vfs::String const& sStr);
private:
	struct SRule
	{
		EAction		action;
		vfs::Path	pattern;
	};
	typedef std::list<SRule> tPatternList;
	tPatternList	m_listRules;
	EAction			m_eDefaultAction;
};


#endif // _TRANSFER_RULES_H_
