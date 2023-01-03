#include "MPXmlTeams.hpp"
#include "sgp.h"
#include "FileMan.h"
#include "expat.h"
#include "XML.h"
#include "random.h"
#include <cstdio>

MultiplayerTeams mpTeams = MultiplayerTeams();

// fallback data
const int MultiplayerTeams::random_merc_teams[NUM_RANDOM_MERC_TEAMS][RANDOM_MERC_TEAM_SIZE] = {
	{ 16, 10, 19, 25, 4 , 11, 39 },		// Gus , Shadow, Spider , Raven , Vicki , Red , Meltdown
	{ 29, 36, 28, 2 , 22, 8 , 32 },		// Magic , Scope, Danny , Lynx , Hitman , Steroid , Malice
	{ 12, 5, 20, 23, 48, 34, 17 },		// Reaper , Trevor, Cliff , Buzz , Cougar , Nails , Buns
	{ 31, 7, 33, 35, 27, 37, 1 },		// Scully , Ivan , Dr Q  , Thor , Len , Wolf , Blood
};

void MultiplayerTeams::SerializeProfiles(int* dest)
{
	int id;
	int r = Random(teams.size());

	// if team is already used, choose next
	while (teamsTaken.at(r))
	{
		r++;
		if (r == teams.size())
			r = 0;
	}

	teamsTaken.at(r) = true;
	id = r;

	this->SerializeProfiles(dest, id);
}

void MultiplayerTeams::SerializeProfiles(int* dest, int teamID)
{
	teamID = teamID % this->teams.size();
	for (int i = 0; i < RANDOM_MERC_TEAM_SIZE; i++)
	{
		// poor man's memcpy
		*dest = teams.at(teamID).members.at(i);
		dest++;
	}
	return;
}

void MultiplayerTeams::HandleServerStarted()
{
	// reset
	teamsTaken = std::vector<bool>(teams.size(), false);
}

void MultiplayerTeams::UseFallbackDataIfNecessary()
{
	for (size_t team = 0; team < max(NUM_RANDOM_MERC_TEAMS, teams.size()); team++)
	{
		if (teams.size() <= team)
		{
			// create teams 1-4
			teams.push_back(MPTeam());
		}

		// fill any empty merc slots
		for (int merc = teams.at(team).members.size(); merc < RANDOM_MERC_TEAM_SIZE; merc++)
		{
			teams.at(team).members.push_back(MultiplayerTeams::random_merc_teams[team%NUM_RANDOM_MERC_TEAMS][merc]);
		}
	}
}


BOOLEAN MultiplayerTeams::ReadInMPTeams(STR fileName)
{
	BOOLEAN result = this->ReadXMLFile(fileName);
	this->UseFallbackDataIfNecessary();
	return result;
}

BOOLEAN MultiplayerTeams::ReadXMLFile(STR fileName)
{
	
	HWFILE hFile;
	UINT32 uiBytesRead;
	UINT32 uiFSize;
	CHAR8 *lpcBuffer;


	CHAR8 msg[128];
	sprintf(msg, "Loading %s", fileName);
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, msg);

	if(!FileExists(fileName))
	{
		return FALSE;
	}

	// open file
	hFile = FileOpen(fileName, FILE_ACCESS_READ, FALSE);
	if (!hFile)
	{
		// this might issue an error in the caller
		// but we can proceed, actually
		return FALSE;
	}

	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR*) MemAlloc(uiFSize);

	// read in block
	if (!FileRead(hFile, lpcBuffer, uiFSize, &uiBytesRead))
	{
		MemFree(lpcBuffer);
		return FALSE;
	}

	FileClose(hFile);

	XML_Parser parser = XML_ParserCreate(NULL);
	struct teamsParseData parseData = { };
	parseData.mpTeams = this;

	XML_SetElementHandler(parser, &MultiplayerTeams::teamsStartElementHandler, &MultiplayerTeams::teamsEndElementHandler);
	XML_SetCharacterDataHandler(parser, this->teamsCharacterDataHandler);
	XML_SetUserData(parser, &parseData);


	if (this->initialized)
		this->TrashTeams();

	if (!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		// Cannot load, but that's okay. There is fallback data.
		MemFree(lpcBuffer);
		XML_ParserFree(parser);
		return FALSE;
	}
	else
		this->initialized = true;


	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}

MultiplayerTeams::MultiplayerTeams()
{
	this->initialized = false;
}

MultiplayerTeams::~MultiplayerTeams()
{
}

void MultiplayerTeams::TrashTeams()
{
	this->teams.clear();
	this->initialized = false;
}

void XMLCALL MultiplayerTeams::teamsStartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts)
{
	struct teamsParseData * pData = (struct teamsParseData *)userData;

	if (pData->currentDepth <= pData->maxReadDepth)
	{
		if (!strcmp(name, "mp_teams") && pData->curElement == ELEMENT_NONE)
		{
			// root element
			pData->curElement = ELEMENT_LIST;
			pData->maxReadDepth++;
		}
		else if (!strcmp(name, "team") && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;
			MPTeam team = MPTeam();

			if (atts[0] && !strcmp(atts[0], "name"))
			{
				team.name = std::string(atts[1]);
			}
			else
			{
				team.name = std::string("unnamed team");
			}

			pData->mpTeams->teams.push_back(team);

			pData->maxReadDepth++;
		}
		else if (!strcmp(name, "id"))
		{
			pData->curElement = ELEMENT_PROPERTY;
			pData->maxReadDepth++;
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

	return;
}

void XMLCALL MultiplayerTeams::teamsEndElementHandler(void *userData, const XML_Char *name)
{
	struct teamsParseData *pData = (struct teamsParseData *)userData;

	if (pData->currentDepth <= pData->maxReadDepth)
	{
		if (!strcmp(name, "mp_teams"))
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if (!strcmp(name, "team"))
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(!strcmp(name, "id"))
		{
			pData->curElement = ELEMENT;

			INT8 id = atoi(pData->szCharData);
			pData->mpTeams->teams.back().members.push_back(id);
		}

		pData->maxReadDepth--;
	}
	
	pData->currentDepth--;

	return;
}

void XMLCALL MultiplayerTeams::teamsCharacterDataHandler(void *userData, const XML_Char *s, int len)
{
	struct teamsParseData *pData = (struct teamsParseData *)userData;

	if ((pData->currentDepth <= pData->maxReadDepth)
		&& (strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH))
	{
		strncat(pData->szCharData, s, __min((unsigned int)len, MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}

	return;
}
