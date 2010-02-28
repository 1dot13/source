#ifndef MPXMLTEAMS_HPP
#define MPXMLTEAMS_HPP

#include "types.h"
#include "XML.h"
#include "expat.h"
#include <vector>


#define NUM_RANDOM_MERC_TEAMS	4
#define RANDOM_MERC_TEAM_SIZE	7

/* MPTeams.xml structure:
	
	- name attribute is optional and not used anywhere yet

	<mp_teams>
		<team name="team1">
			<id>42</id>
			<id>23</id>
			...
		</team>
		<team>
			<id>97</id>
			...
		</team>
		...
	</mp_teams>
*/

class MultiplayerTeams
{
private:
	struct MPTeam
	{
		std::string name;
		std::vector<int> members;
	};

public:
	BOOLEAN ReadInMPTeams(STR fileName);
	void HandleServerStarted();

	MultiplayerTeams();
	~MultiplayerTeams();

	void SerializeProfiles(int* dest);
	void SerializeProfiles(int* dest, int teamID);


private:
	std::vector< MPTeam > teams;
	std::vector<bool> teamsTaken;
	bool initialized;

	BOOLEAN ReadXMLFile(STR fileName);
	void UseFallbackDataIfNecessary();
	void TrashTeams();
	
	static void XMLCALL teamsStartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts);
	static void XMLCALL teamsEndElementHandler(void *userData, const XML_Char *name);
	static void XMLCALL teamsCharacterDataHandler(void *userData, const XML_Char *s, int len);

	struct teamsParseData
	{
		PARSE_STAGE curElement;
		CHAR8 szCharData[MAX_CHAR_DATA_LENGTH+1];

		UINT32 currentDepth;
		UINT32 maxReadDepth;

		MultiplayerTeams *mpTeams;
	};

	static const int random_merc_teams[NUM_RANDOM_MERC_TEAMS][RANDOM_MERC_TEAM_SIZE];
};

// make global
extern MultiplayerTeams mpTeams;

#endif
