/**
* @file
* @author Flugente (bears-pit.com)
* @brief This file contains definitions of classes and functions used for dynamic dialogue
*/

#include "DynamicDialogue.h"
#include "DynamicDialogueWidget.h"

#include "SaveLoadGame.h"
#include "GameVersion.h"
#include "Text.h"
#include "MercTextBox.h"
#include "Meanwhile.h"
#include "soldier profile type.h"
#include "Soldier Profile.h"
#include "Interface.h"
#include "CampaignStats.h"
#include "Drugs And Alcohol.h"
#include "Overhead.h"
#include "Food.h"
#include "finances.h"
#include "message.h"
#include "Soldier macros.h"
#include "Dialogue Control.h"
#include "Soldier Add.h"
#include "random.h"
#include "strategicmap.h"
#include "connect.h"
#include "Campaign.h"
#include "Points.h"

extern INT32 ReadFieldByField( HWFILE hFile, PTR pDest, UINT32 uiFieldSize, UINT32 uiElementSize, UINT32  uiCurByteCount );

// event id counter
// has to be saved/loaded/reset
UINT32 gDynamicDialogueEventCounter = 0;

// stores data of the currently used event and is used to identify wether dialogue is added to the current one or is from a new event
// has to be saved/loaded/reset
DynamicOpinionSpeechEvent_EventData gCurrentDynamicDialogueEventData;

// when we add an event, we add it to this vector. 
// we create speech from this and add it to gDynamicOpinionSpeechEventVector
// has to be saved/loaded/reset
std::vector<DynamicOpinionSpeechEvent> gDynamicOpinionSpeechEventArchiveVector;

// this vector contains speech events that are a result of dynamic opinions and the 'dialogue' between mercs that results from this
// the vector is filled when we add an opinion event and then create spech out of it
// it is emptied when 'sending' the events to the dialogue system
// this vector is not saved and should be cleaned every time we reload
std::vector<DynamicOpinionSpeechEvent> gDynamicOpinionSpeechEventVector;

// collects all opinion events that happen as a RESULT of the ongoing dialogue. The event that starts the dialogue itself is not used here!
// This vector is not saved. It's contents will be added and cleared once a dialogue chain finishes.
std::vector<OpinionEvent> gOpionionEventsDuringDialogue;

// stores the events used in the current dialogue. Used to determine coordinates and dailogue reactions
// This vector is not saved. It's contents will be added and cleared once a dialogue chain finishes.
std::vector<DynamicOpinionSpeechEvent> gDynamicOpinionSpeechInCurrentDialogue;

// Flugente: calculate A's opinion of B
INT8	SoldierRelation( SOLDIERTYPE* pSoldierA, SOLDIERTYPE* pSoldierB )
{
	INT8 bOpinion = 0;

	if ( !OKToCheckOpinion( pSoldierA->ubProfile ) || !OKToCheckOpinion( pSoldierB->ubProfile ) )
		return 0;

	MERCPROFILESTRUCT*	pProfile = &(gMercProfiles[pSoldierA->ubProfile]);

	bOpinion = pProfile->bMercOpinion[pSoldierB->ubProfile];

	// Flugente: evaluate appearance/refinement/hated nationalities

	// some people loathe ugly people and like beautiful people. It's a mean world.
	// we also handle sexism here
	switch ( gMercProfiles[pSoldierB->ubProfile].bAppearance )
	{
		case APPEARANCE_UGLY:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance * 2;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance * 4;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[pSoldierB->ubProfile].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism * 2;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism * 4;
			}
		}
			break;

		case APPEARANCE_HOMELY:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance * 2;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[pSoldierB->ubProfile].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism * 2;
			}
		}
			break;

		case APPEARANCE_ATTRACTIVE:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance * 2;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[pSoldierB->ubProfile].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism * 2;
			}
		}
			break;

		case APPEARANCE_BABE:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance * 2;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance * 4;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[pSoldierB->ubProfile].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism * 2;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism * 4;
			}
		}
			break;
	}

	// some people care about how distuingished other people are. Malus if on different ends of the spectrum, a small bonus if on the same and its really important to the person
	// also give a malus if the other person is a slob or snob and we are average but care extremely (we don't like people who behave differently)
	// if we don't care, doesn't matter
	if ( pProfile->bRefinementCareLevel == CARELEVEL_NONE )
	{
		// nothing to do...
	}
	// if we care somewhat, malus on slob/snob
	else if ( pProfile->bRefinementCareLevel == CARELEVEL_SOME )
	{
		if ( pProfile->bRefinement * gMercProfiles[pSoldierB->ubProfile].bRefinement == 2 )
			bOpinion -= gGameExternalOptions.sMoraleModRefinement;
	}
	// if we care extremely, reward for similarity, malus otherwise
	else //if( pProfile->bRefinementCareLevel == CARELEVEL_EXTREME )
	{
		if ( pProfile->bRefinement * gMercProfiles[pSoldierB->ubProfile].bRefinement == 2 )
			bOpinion -= 2 * gGameExternalOptions.sMoraleModRefinement;
		else if ( pProfile->bRefinement * gMercProfiles[pSoldierB->ubProfile].bRefinement == 0 )
			bOpinion -= gGameExternalOptions.sMoraleModRefinement;
		else
			bOpinion += gGameExternalOptions.sMoraleModRefinement;
	}

	// some people hate other nationalities (do not mix up with racism, which uses bRace)
	if ( pProfile->bHatedNationality > -1 && gMercProfiles[pSoldierB->ubProfile].bNationality == pProfile->bHatedNationality )
	{
		if ( pProfile->bHatedNationalityCareLevel == CARELEVEL_SOME )
			bOpinion -= gGameExternalOptions.sMoraleModHatedNationality;
		else if ( pProfile->bHatedNationalityCareLevel == CARELEVEL_EXTREME )
			bOpinion -= gGameExternalOptions.sMoraleModHatedNationality * 2;
	}

	// some people are racists
	if ( pProfile->bRacist && gMercProfiles[pSoldierB->ubProfile].bRace != pProfile->bRace )
	{
		if ( pProfile->bRacist == RACIST_SOME )
			bOpinion -= gGameExternalOptions.sMoraleModRacism;
		else if ( pProfile->bRacist == RACIST_VERY )
			bOpinion -= gGameExternalOptions.sMoraleModRacism * 2;
	}

	// Flugente: backgrounds
	if ( pSoldierA->GetBackgroundValue( BG_DISLIKEBG ) && pSoldierA->GetBackgroundValue( BG_DISLIKEBG ) == -pSoldierB->GetBackgroundValue( BG_DISLIKEBG ) )
	{
		bOpinion -= 2;
	}

	// smoker
	INT16 smokerA = pSoldierA->GetBackgroundValue( BG_SMOKERTYPE );
	INT16 smokerB = pSoldierB->GetBackgroundValue( BG_SMOKERTYPE );
	if ( smokerA && smokerB )
	{
		if ( smokerA != smokerB )
			bOpinion -= 2;
		else
			bOpinion += 1;
	}

	if ( pSoldierA->HasBackgroundFlag( BACKGROUND_XENOPHOBIC ) && pSoldierB->ubProfile != NO_PROFILE && gMercProfiles[pSoldierA->ubProfile].usBackground != gMercProfiles[pSoldierB->ubProfile].usBackground )
		bOpinion -= gGameExternalOptions.sMoraleModXenophobicBackGround;

	// Flugente: dynamic opinions
	if ( gGameExternalOptions.fDynamicOpinions )
	{
		for ( UINT8 opinionevent = OPINIONEVENT_FRIENDLYFIRE; opinionevent < OPINIONEVENT_MAX; ++opinionevent )
		{
			bOpinion += GetDynamicOpinion( pSoldierA->ubProfile, pSoldierB->ubProfile, opinionevent );
		}
	}

	// long-term memory
	bOpinion += gMercProfiles[pSoldierA->ubProfile].sDynamicOpinionLongTerm[pSoldierB->ubProfile];

	// reasonable values
	bOpinion = min( BUDDY_OPINION, bOpinion );
	bOpinion = max( HATED_OPINION, bOpinion );

	return bOpinion;
}

INT8	SoldierRelation( UINT8 usProfileA, UINT8 usProfileB )
{
	INT16 idA = GetSoldierIDFromMercID( usProfileA );
	INT16 idB = GetSoldierIDFromMercID( usProfileB );

	if ( idA < 0 || idB < 0 )
		return 0;

	SOLDIERTYPE* pSoldierA = MercPtrs[idA];
	SOLDIERTYPE* pSoldierB = MercPtrs[idB];

	if ( !pSoldierA || !pSoldierB )
		return 0;

	return SoldierRelation( pSoldierA, pSoldierB );
}


// it could happen that we load events incorrectly. To detect that early, we write an extra eyecacher befoer and after the list of events. Should we fail to read it correctly, we will knowm where to look. 
UINT32 gDynDiaSaveCheck = 0xF123456F;
BOOLEAN SaveDynamicDialogue( HWFILE hwFile )
{
	UINT32 uiNumBytesWritten = 0;

	if ( !FileWrite( hwFile, &gCurrentDynamicDialogueEventData, sizeof(DynamicOpinionSpeechEvent_EventData), &uiNumBytesWritten ) )
		return(FALSE);

	if ( !FileWrite( hwFile, &gDynamicDialogueEventCounter, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	// in order to save a vector, we first save its size and then its content
	UINT32 size = gDynamicOpinionSpeechEventArchiveVector.size( );
	if ( !FileWrite( hwFile, &size, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	if ( !FileWrite( hwFile, &gDynDiaSaveCheck, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	for ( UINT32 i = 0; i < size; ++i )
	{
		if ( !FileWrite( hwFile, &gDynamicOpinionSpeechEventArchiveVector[i], sizeof(DynamicOpinionSpeechEvent), &uiNumBytesWritten ) )
			return(FALSE);
	}

	if ( !FileWrite( hwFile, &gDynDiaSaveCheck, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	return(TRUE);
}

BOOLEAN LoadDynamicDialogue( HWFILE hwFile )
{
	// clear any artefacts prior to loading
	DestroyAllDynamicDialogueBoxes( );

	gDynamicDialogueEventCounter = 0;

	gCurrentDynamicDialogueEventData.usId = 0;
	gCurrentDynamicDialogueEventData.usPriority = 0;

	gDynamicOpinionSpeechEventVector.clear( );

	gDynamicOpinionSpeechEventArchiveVector.clear( );

	gOpionionEventsDuringDialogue.clear( );

	gDynamicOpinionSpeechInCurrentDialogue.clear( );

	SetfDynamicOpinionSpeechInProgress( FALSE );

	if ( guiCurrentSaveGameVersion >= DYNAMIC_DIALOGUE )
	{
		UINT32 numBytesRead = 0;

		numBytesRead = ReadFieldByField( hwFile, &gCurrentDynamicDialogueEventData, sizeof(gCurrentDynamicDialogueEventData), sizeof(DynamicOpinionSpeechEvent_EventData), numBytesRead );
		numBytesRead = ReadFieldByField( hwFile, &gDynamicDialogueEventCounter, sizeof(gDynamicDialogueEventCounter), sizeof(UINT32), numBytesRead );

		UINT32 size = 0;
		numBytesRead = ReadFieldByField( hwFile, &size, sizeof(size), sizeof(UINT32), numBytesRead );

		UINT32 safetycheck1 = 0;
		numBytesRead = ReadFieldByField( hwFile, &safetycheck1, sizeof(safetycheck1), sizeof(UINT32), numBytesRead );

		for ( UINT32 i = 0; i < size; ++i )
		{
			DynamicOpinionSpeechEvent event;
			numBytesRead = ReadFieldByField( hwFile, &event, sizeof(event), sizeof(DynamicOpinionSpeechEvent), 0 );

			gDynamicOpinionSpeechEventArchiveVector.push_back( event );
		}

		UINT32 safetycheck2 = 0;
		numBytesRead = ReadFieldByField( hwFile, &safetycheck2, sizeof(safetycheck2), sizeof(UINT32), numBytesRead );

		if ( safetycheck1 != gDynDiaSaveCheck || safetycheck2 != gDynDiaSaveCheck )
		{
			ScreenMsg( FONT_MCOLOR_LTGREEN, MSG_INTERFACE, L"Eyecatcher corruption detected while loading dynamic dialogue events!" );
			return FALSE;
		}
	}

	return TRUE;
}

void CleanDynamicOpinionSpeechEvents( )
{
	gDynamicOpinionSpeechEventVector.clear( );
}

void AddDynamicOpinionEvent_New( OpinionEvent aEvent )
{
	DynamicOpinionSpeechEvent_EventData data;
	data.event = aEvent;
	data.ubProfileINTERJECTOR = NO_PROFILE;
	data.usId = 0;					// has to be 0 here, do not use a different value!
	data.usPriority = DOST_PRIORITY_LOW;
	memset( data.ubProfileSideCause, NO_PROFILE, DOST_INTERJECTORS_MAX * sizeof(UINT8) );
	memset( data.ubProfileSideComplainant, NO_PROFILE, DOST_INTERJECTORS_MAX * sizeof(UINT8) );

	// get the profile of a merc that can interjet. Pick an IMP if possible.
	UINT8 impprofile = GetFittingInterjectorProfile( aEvent.ubEventId, aEvent.ubProfileComplainant, aEvent.ubProfileCause );
	
	if ( impprofile != NO_PROFILE )
		data.ubProfileINTERJECTOR = impprofile;

	DynamicOpinionSpeechEvent event;
	event.data = data;
	event.ubEventType = DOST_VICTIM_INITIATE;
	event.usQueueNumber = 0;
	event.usSpeaker = event.data.event.ubProfileComplainant;
	event.usSide = DOST_POSITION_LEFT;
	event.usNumonside = 0;
	event.usStarttime = 0;

	AddDynamicOpinionEvent_Continue( event );
}

void AddDynamicOpinionEvent_Continue( DynamicOpinionSpeechEvent aEvent )
{
	// If this event already has an Id that is already used in our list of current events, then this must be a reaction from one of our mercs in an ongoing dialogue. 
	// In this case, don't add it to the archive, but continue the dialogue
	if ( aEvent.data.usId > 0 && (aEvent.data.event.ubEventId == gCurrentDynamicDialogueEventData.event.ubEventId) && (aEvent.data.usId == gCurrentDynamicDialogueEventData.usId) )
	{
		CreateSpeechEventsFromDynamicOpinionEvent( aEvent );
	}
	else
	{
		++gDynamicDialogueEventCounter;

		// aEvent gets a new Id. This it the only place where this is supposed to happen!
		aEvent.data.usId = gDynamicDialogueEventCounter;
		aEvent.data.usPriority = DOST_PRIORITY_LOW;

		gDynamicOpinionSpeechEventArchiveVector.push_back( aEvent );
	}
}


void CreateSpeechEventsFromDynamicOpinionEvent( DynamicOpinionSpeechEvent aEvent )
{
	// This is the point where we could determine the 'dialogue' between mercs
	// we determine dialogue an resulting dialogue for each type of event an determine dialogue for it
	// that way, if several opinon events regarding the same thing are set, we will have dialogue over all of that

	UINT8 cnt = 0;
	aEvent.usStarttime = GetJA2Clock( ) + cnt * gGameExternalOptions.sDynamicDialogueTimeOffset;

	UINT8 queuenumber = aEvent.usQueueNumber + 1;

	// interjector responeses are handled later in the if-else statement
	if ( aEvent.ubEventType < DOST_INTERJECTOR_TO_VICTIM_DENY )
	{
		gDynamicOpinionSpeechEventVector.push_back( aEvent );
		gDynamicOpinionSpeechInCurrentDialogue.push_back( aEvent );
		++cnt;
	}

	// if appropriate, new opinion events will be created here and added to gOpionionEventsDuringDialogue
	if ( aEvent.ubEventType == DOST_VICTIM_INITIATE )
	{
		// cause either denies or agrees with complainant

		UINT32 chancetoagree = 0;	

		// the more we dislike the other guy, the higher the chance we disagree
		// personality also influences this
		INT8 agreemodifier_personal = SoldierRelation( aEvent.data.event.ubProfileCause, aEvent.data.event.ubProfileComplainant );

		switch ( gMercProfiles[aEvent.data.event.ubProfileCause].bCharacterTrait )
		{
		case(CHAR_TRAIT_NORMAL) : agreemodifier_personal += 0; break;
		case(CHAR_TRAIT_SOCIABLE) : agreemodifier_personal += 10; break;
		case(CHAR_TRAIT_LONER) : agreemodifier_personal += -3; break;
		case(CHAR_TRAIT_OPTIMIST) : agreemodifier_personal += 5; break;
		case(CHAR_TRAIT_ASSERTIVE) : agreemodifier_personal += -5; break;
		case(CHAR_TRAIT_INTELLECTUAL) : agreemodifier_personal += 2; break;
		case(CHAR_TRAIT_PRIMITIVE) : agreemodifier_personal += -4; break;
		case(CHAR_TRAIT_AGGRESSIVE) : agreemodifier_personal += -10; break;
		case(CHAR_TRAIT_PHLEGMATIC) : agreemodifier_personal += -20; break;
		case(CHAR_TRAIT_DAUNTLESS) : agreemodifier_personal += -10; break;
		case(CHAR_TRAIT_PACIFIST) : agreemodifier_personal += 20; break;
		case(CHAR_TRAIT_MALICIOUS) : agreemodifier_personal += -8; break;
		case(CHAR_TRAIT_SHOWOFF) : agreemodifier_personal += -3; break;
		}

		// if the event is positive, then we are much more likely to agree
		if ( gDynamicOpinionEvent[aEvent.data.event.ubEventId].sOpinionModifier > 0 )
			agreemodifier_personal += 50;

		chancetoagree = max( 0, 25 + agreemodifier_personal );

		// if he agrees, then this dialogue will soon be over - interjector won't have to play his part
		if ( Chance( chancetoagree ) )
		{
			DynamicOpinionSpeechEvent response;
			response.data = aEvent.data;
			response.ubEventType = DOST_CAUSE_TO_VICTIM_AGREE;
			response.usSpeaker = aEvent.data.event.ubProfileCause;
			response.usQueueNumber = queuenumber++;
			response.usSide = DOST_POSITION_LEFT;
			response.usNumonside = (UINT8)GetSidePosition( response.usSide );
			response.usStarttime = GetJA2Clock( ) + cnt * gGameExternalOptions.sDynamicDialogueTimeOffset;

			gDynamicOpinionSpeechEventVector.push_back( response );
			gDynamicOpinionSpeechInCurrentDialogue.push_back( response );
			++cnt;
		}
		else
		{
			DynamicOpinionSpeechEvent response;
			response.data = aEvent.data;
			response.ubEventType = DOST_CAUSE_TO_VICTIM_DENY;
			response.usSpeaker = aEvent.data.event.ubProfileCause;
			response.usQueueNumber = queuenumber++;
			response.usSide = DOST_POSITION_RIGHT;
			response.usNumonside = (UINT8)GetSidePosition( response.usSide );
			response.usStarttime = GetJA2Clock( ) + cnt * gGameExternalOptions.sDynamicDialogueTimeOffset;

			gDynamicOpinionSpeechEventVector.push_back( response );
			gDynamicOpinionSpeechInCurrentDialogue.push_back( response );
			++cnt;

			// chance that an INTERJECTOR tries to interfere
			if ( aEvent.data.ubProfileINTERJECTOR != NO_PROFILE )
			{
				DynamicOpinionSpeechEvent impaction;
				impaction.data = aEvent.data;
				impaction.usSpeaker = aEvent.data.ubProfileINTERJECTOR;
				impaction.usSide = DOST_POSITION_MIDDLE;

				BOOLEAN fIsImp = FALSE;
				INT16 idInterjector = GetSoldierIDFromMercID( aEvent.data.ubProfileINTERJECTOR );
				if ( idInterjector != NOBODY && idInterjector >= 0 )
				{
					if ( MercPtrs[idInterjector]->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER )
						fIsImp = TRUE;

					// if it's an IMP, we have a choice, otherwise pick one answer randomly
					if ( fIsImp )
					{
						impaction.ubEventType = DOST_INTERJECTOR_DIALOGUESELECTION;
					}
					else
					{
						// determine interjectors reaction

						// chance to say nothing
						UINT8 chance_nothing = 10;
						if ( gMercProfiles[aEvent.data.ubProfileINTERJECTOR].bCharacterTrait == CHAR_TRAIT_LONER )
							chance_nothing += 20;
						else if ( gMercProfiles[aEvent.data.ubProfileINTERJECTOR].bCharacterTrait == CHAR_TRAIT_LONER )
							chance_nothing += 20;

						if ( Chance( chance_nothing ) )
						{
							// say nothing...
							impaction.ubEventType = DOST_INTERJECTOR_NOTHING;
						}
						else
						{
							// chance to try to stop the argument (either with reason or authority)
							UINT8 chance_to_reason = 30;
							UINT8 chance_to_agression = 30;
							switch ( gMercProfiles[aEvent.data.ubProfileINTERJECTOR].bCharacterTrait )
							{
							case(CHAR_TRAIT_NORMAL) :
								chance_to_reason += 0;
								chance_to_agression += 0;
								break;
							case(CHAR_TRAIT_SOCIABLE) :
								chance_to_reason += 40;
								chance_to_agression += -15;
								break;
							case(CHAR_TRAIT_LONER) :
								chance_to_reason += 0;
								chance_to_agression += 0;
								break;
							case(CHAR_TRAIT_OPTIMIST) :
								chance_to_reason += 5;
								chance_to_agression += -5;
								break;
							case(CHAR_TRAIT_ASSERTIVE) :
								chance_to_reason += 5;
								chance_to_agression += 10;
								break;
							case(CHAR_TRAIT_INTELLECTUAL) :
								chance_to_reason += 10;
								chance_to_agression += -3;
								break;
							case(CHAR_TRAIT_PRIMITIVE) :
								chance_to_reason += 0;
								chance_to_agression += 6;
								break;
							case(CHAR_TRAIT_AGGRESSIVE) :
								chance_to_reason += -15;
								chance_to_agression += 30;
								break;
							case(CHAR_TRAIT_PHLEGMATIC) :
								chance_to_reason += 0;
								chance_to_agression += 0;
								break;
							case(CHAR_TRAIT_DAUNTLESS) :
								chance_to_reason += 0;
								chance_to_agression += 10;
								break;
							case(CHAR_TRAIT_PACIFIST) :
								chance_to_reason += 50;
								chance_to_agression += -20;
								break;
							case(CHAR_TRAIT_MALICIOUS) :
								chance_to_reason += -20;
								chance_to_agression += 40;
								break;
							case(CHAR_TRAIT_SHOWOFF) :
								chance_to_reason += -2;
								chance_to_agression += 2;
								break;
							}

							if ( Chance( chance_to_reason ) )
							{
								impaction.ubEventType = DOST_INTERJECTOR_SOLVE_REASON;
							}
							else if ( Chance( chance_to_agression ) )
							{
								impaction.ubEventType = DOST_INTERJECTOR_SOLVE_AGGRESSIVE;
							}
							else
							{
								// we side with one of the argument's sides
								INT8 agreewithcausechance = 50 + SoldierRelation( aEvent.data.ubProfileINTERJECTOR, aEvent.data.event.ubProfileCause ) - SoldierRelation( aEvent.data.ubProfileINTERJECTOR, aEvent.data.event.ubProfileComplainant );

								if ( Chance( agreewithcausechance ) )
								{
									if ( Random( 2 ) )
										impaction.ubEventType = DOST_INTERJECTOR_TO_CAUSE_AGREE;
									else
										impaction.ubEventType = DOST_INTERJECTOR_TO_VICTIM_DENY;

									impaction.usSide = DOST_POSITION_RIGHT;
								}
								else
								{
									if ( Random( 2 ) )
										impaction.ubEventType = DOST_INTERJECTOR_TO_VICTIM_AGREE;
									else
										impaction.ubEventType = DOST_INTERJECTOR_TO_CAUSE_DENY;

									impaction.usSide = DOST_POSITION_LEFT;
								}
							}
						}
					}

					if ( impaction.ubEventType != DOST_INTERJECTOR_NOTHING )
					{
						impaction.usQueueNumber = queuenumber++;
						impaction.usNumonside = (UINT8)GetSidePosition( impaction.usSide );
						impaction.usStarttime = GetJA2Clock( ) + cnt * gGameExternalOptions.sDynamicDialogueTimeOffset;

						gDynamicOpinionSpeechEventVector.push_back( impaction );
						gDynamicOpinionSpeechInCurrentDialogue.push_back( impaction );
						++cnt;
					}
				}
			}
		}

		// if other people are involved too, they can speak too
		for ( UINT8 i = 0; i < DOST_INTERJECTORS_MAX; ++i )
		{
			if ( aEvent.data.ubProfileSideComplainant[i] != NO_PROFILE )
			{
				DynamicOpinionSpeechEvent response;
				response.data = aEvent.data;
				response.ubEventType = DOST_SIDEWITH_VICTIM;
				response.usSpeaker = aEvent.data.ubProfileSideComplainant[i];
				response.usQueueNumber = queuenumber++;
				response.usSide = DOST_POSITION_LEFT;
				response.usNumonside = (UINT8)GetSidePosition( response.usSide );
				response.usStarttime = GetJA2Clock( ) + cnt * gGameExternalOptions.sDynamicDialogueTimeOffset;

				gDynamicOpinionSpeechEventVector.push_back( response );
				gDynamicOpinionSpeechInCurrentDialogue.push_back( response );
				++cnt;
			}

			if ( aEvent.data.ubProfileSideCause[i] != NO_PROFILE )
			{
				DynamicOpinionSpeechEvent response;
				response.data = aEvent.data;
				response.ubEventType = DOST_SIDEWITH_CAUSE;
				response.usSpeaker = aEvent.data.ubProfileSideCause[i];
				response.usQueueNumber = queuenumber++;
				response.usSide = DOST_POSITION_RIGHT;
				response.usNumonside = (UINT8)GetSidePosition( response.usSide );
				response.usStarttime = GetJA2Clock( ) + cnt * gGameExternalOptions.sDynamicDialogueTimeOffset;

				gDynamicOpinionSpeechEventVector.push_back( response );
				gDynamicOpinionSpeechInCurrentDialogue.push_back( response );
				++cnt;
			}
		}
	}
	else if ( aEvent.ubEventType >= DOST_INTERJECTOR_TO_VICTIM_DENY && aEvent.ubEventType <= DOST_INTERJECTOR_SOLVE_AGGRESSIVE )
	{
		DynamicOpinionSpeechEvent impinterference;
		impinterference.data = aEvent.data;
		impinterference.ubEventType = aEvent.ubEventType;
		impinterference.usSpeaker = aEvent.data.ubProfileINTERJECTOR;
		impinterference.usQueueNumber = queuenumber++;

		switch ( aEvent.ubEventType )
		{
		case DOST_INTERJECTOR_TO_VICTIM_DENY:
		case DOST_INTERJECTOR_TO_CAUSE_AGREE:
			impinterference.usSide = DOST_POSITION_RIGHT;
			break;

		case DOST_INTERJECTOR_TO_VICTIM_AGREE:
		case DOST_INTERJECTOR_TO_CAUSE_DENY:
			impinterference.usSide = DOST_POSITION_LEFT;
			break;

		case DOST_INTERJECTOR_SOLVE_REASON:
		case DOST_INTERJECTOR_SOLVE_AGGRESSIVE:
			impinterference.usSide = DOST_POSITION_MIDDLE;
			break;
		}

		impinterference.usNumonside = (UINT8)GetSidePosition( impinterference.usSide );
		impinterference.usStarttime = GetJA2Clock( ) + cnt * gGameExternalOptions.sDynamicDialogueTimeOffset;

		gDynamicOpinionSpeechEventVector.push_back( impinterference );
		gDynamicOpinionSpeechInCurrentDialogue.push_back( impinterference );
		++cnt;

		// our response can have consequences on the opinions of others
		if ( Chance( 70 ) )
		{
			OpinionEvent opinionevent;
			opinionevent.ubProfileCause = aEvent.usSpeaker;

			switch ( aEvent.ubEventType )
			{
				case DOST_INTERJECTOR_TO_VICTIM_DENY:
				{
					opinionevent.ubEventId = OPINIONEVENT_AGAINST_US;
					opinionevent.ubProfileComplainant = aEvent.data.event.ubProfileComplainant;
				}
					break;

				case DOST_INTERJECTOR_TO_CAUSE_AGREE:
				{
					opinionevent.ubEventId = OPINIONEVENT_FOR_US;
					opinionevent.ubProfileComplainant = aEvent.data.event.ubProfileCause;
				}
					break;

				case DOST_INTERJECTOR_TO_VICTIM_AGREE:
				{
					opinionevent.ubEventId = OPINIONEVENT_FOR_US;
					opinionevent.ubProfileComplainant = aEvent.data.event.ubProfileComplainant;
				}
					break;

				case DOST_INTERJECTOR_TO_CAUSE_DENY:
				{
					opinionevent.ubEventId = OPINIONEVENT_AGAINST_US;
					opinionevent.ubProfileComplainant = aEvent.data.event.ubProfileCause;
				}
					break;

				case DOST_INTERJECTOR_SOLVE_REASON:
				{
					opinionevent.ubEventId = OPINIONEVENT_SOLVECONFLICT_REASON_GOOD;
					opinionevent.ubProfileComplainant = aEvent.data.event.ubProfileComplainant;
				}
					break;

				case DOST_INTERJECTOR_SOLVE_AGGRESSIVE:
				{
					opinionevent.ubEventId = OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_BAD;
					opinionevent.ubProfileComplainant = aEvent.data.event.ubProfileComplainant;
				}
					break;
			}

			gOpionionEventsDuringDialogue.push_back( opinionevent );
		}

		// we directly inject new dialogue without waiting. The normal way of starting requires the old even to finish, but we want to be a part of it
		StartDynamicOpinionDialogue( );
	}
}

extern BOOLEAN DynamicDialogueBoxesActive( );

void HandleDynamicOpinionSpeechEvents( )
{
	// if dialogue is in progress, check wether it is finished
	if ( DynamicOpinionSpeechInProgress( ) )
	{
		if ( !DynamicDialogueBoxesActive( ) )
		{
			// dialogue is finished, delete handled event from list of events
			std::vector<DynamicOpinionSpeechEvent>::iterator itend = gDynamicOpinionSpeechEventArchiveVector.end( );
			for ( std::vector<DynamicOpinionSpeechEvent>::iterator it = gDynamicOpinionSpeechEventArchiveVector.begin( ); it != itend; ++it )
			{
				DynamicOpinionSpeechEvent event = (*it);

				// if this is the current event, delete it 
				if ( (event.data.event.ubEventId == gCurrentDynamicDialogueEventData.event.ubEventId) && (event.data.usId == gCurrentDynamicDialogueEventData.usId) )
				{
					gDynamicOpinionSpeechEventArchiveVector.erase( it );
					gCurrentDynamicDialogueEventData.usId = 0;
					gCurrentDynamicDialogueEventData.usPriority = 0;
					break;
				}
			}

			// award any opinion events that have happened...
			// these better not start dialogues themselves, or we might cause a loop here
			UINT16 opinioneentscollectedsize = gOpionionEventsDuringDialogue.size( );
			for ( UINT16 i = 0; i < opinioneentscollectedsize; ++i )
			{
				OpinionEvent event = gOpionionEventsDuringDialogue[i];

				AddOpinionEvent( event.ubProfileComplainant, event.ubProfileCause, event.ubEventId, FALSE );
			}

			gOpionionEventsDuringDialogue.clear( );
			gDynamicOpinionSpeechInCurrentDialogue.clear( );

			SetfDynamicOpinionSpeechInProgress( FALSE );
		}
	}
	// if no dialogue is in progress, we can start something new
	else
	{
		// we now select the next event to be turned into dialogue
		// This isn't necessarily the first one, different events can have different priorities, making later events more urgent
		if ( !gDynamicOpinionSpeechEventArchiveVector.empty( ) )
		{
			// this is the place were we can also merge events together...
			PerformPossibleDynamicOpinionEventMerges( );

			DynamicOpinionSpeechEvent bestevent = (*gDynamicOpinionSpeechEventArchiveVector.begin( ));

			std::vector<DynamicOpinionSpeechEvent>::iterator itend = gDynamicOpinionSpeechEventArchiveVector.end( );
			for ( std::vector<DynamicOpinionSpeechEvent>::iterator it = gDynamicOpinionSpeechEventArchiveVector.begin( ); it != itend; ++it )
			{
				DynamicOpinionSpeechEvent event = (*it);

				if ( event.data.usPriority > bestevent.data.usPriority )
					bestevent = event;
			}

			// important: we do NOT delete the event now. During the dialogue, different events can happen depedning on the player's actions
			// When reloading the game, we cannot start in a dialogue section, as it would be very confusing. We thus have to restart the entire dialogue.
			// We thus only delete an event once it's dialogue chain of events is over, and only award opinion changes at that point

			gCurrentDynamicDialogueEventData = bestevent.data;
			SetfDynamicOpinionSpeechInProgress( TRUE );

			CreateSpeechEventsFromDynamicOpinionEvent( bestevent );
		}
		// no pending events...
		else
		{
			// ... and no pending dialogue either. Time for some clean up.
			if ( !DynamicDialogueBoxesActive( ) )
			{
				gDynamicDialogueEventCounter = 0;
				gCurrentDynamicDialogueEventData.usId = 0;
				gCurrentDynamicDialogueEventData.usPriority = 0;
			}
		}
	}

	// if we have any pending event dialogue bits, play them now
	StartDynamicOpinionDialogue( );
}


void StartDynamicOpinionDialogue( )
{
	std::vector<DynamicOpinionSpeechEvent>::iterator itend = gDynamicOpinionSpeechEventVector.end( );
	for ( std::vector<DynamicOpinionSpeechEvent>::iterator it = gDynamicOpinionSpeechEventVector.begin( ); it != itend; ++it )
	{
		DynamicOpinionSpeechEvent event = (*it);

		// say this info
		DynamicOpinionTacticalCharacterDialogue( event );
	}

	CleanDynamicOpinionSpeechEvents( );
}

// if possible, merge events if possible, thereby creating fewer but deeper dialogues
void PerformPossibleDynamicOpinionEventMerges( )
{
	for ( std::vector<DynamicOpinionSpeechEvent>::iterator it = gDynamicOpinionSpeechEventArchiveVector.begin( ); it != gDynamicOpinionSpeechEventArchiveVector.end( ); )	// not having ++it here is intentional!
	{
		// if two events have the same ubEventId, then they are about the same kind of event.
		// Depending on the kin of people involved, it might be reasonable to merge them.
		// On a merge, the mercs involed in the second and following events simply shortly agree with cause or complainant
		DynamicOpinionSpeechEvent& event1 = (*it);

		// loop over all following events and compare
		for ( std::vector<DynamicOpinionSpeechEvent>::iterator it2 = it + 1; it2 != gDynamicOpinionSpeechEventArchiveVector.end( ); )	// not having ++it here is intentional!
		{
			DynamicOpinionSpeechEvent event2 = (*it2);

			BOOLEAN fErased = FALSE;

			if ( event1.data.event.ubEventId == event2.data.event.ubEventId )
			{
				// if cause are the same
				if ( event1.data.event.ubProfileCause == event2.data.event.ubProfileCause )
				{
					// if there is still room, add to the list of complainants and delete this event
					for ( UINT8 i = 0; i < DOST_INTERJECTORS_MAX; ++i )
					{
						if ( event1.data.ubProfileSideComplainant[i] == NO_PROFILE )
						{
							event1.data.ubProfileSideComplainant[i] = event2.data.event.ubProfileComplainant;

							it2 = gDynamicOpinionSpeechEventArchiveVector.erase( it2 );

							fErased = TRUE;

							break;
						}
					}
				}

				if ( !fErased && event1.data.event.ubProfileComplainant == event2.data.event.ubProfileComplainant )
				{
					// if there is still room, add to the lsit of complainants and delete this event
					for ( UINT8 i = 0; i < DOST_INTERJECTORS_MAX; ++i )
					{
						if ( event1.data.ubProfileSideCause[i] == NO_PROFILE )
						{
							event1.data.ubProfileSideCause[i] = event2.data.event.ubProfileCause;

							it2 = gDynamicOpinionSpeechEventArchiveVector.erase( it2 );

							fErased = TRUE;

							break;
						}
					}
				}
			}

			if ( !fErased )
			{
				++it2;
			}
		}

		++it;
	}
}

CHAR16 stupidarray[DOST_CHOICE_MAX][100];

BOOLEAN DynamicOpinionTacticalCharacterDialogue( DynamicOpinionSpeechEvent& aEvent )
{
	// Haydent
	if ( is_client )
		return(FALSE); //somewhere amongst all this it causes a paase of merc movement while making the quote which throws out the movement sync between  clients... : hayden.

	SOLDIERTYPE* pSoldier = FindSoldierByProfileID( aEvent.usSpeaker, TRUE );

	if ( !pSoldier || !pSoldier->bActive )
		return FALSE;

	if ( pSoldier->ubProfile == NO_PROFILE )
		return(FALSE);

#if (defined JA2UB) 
	//Ja25 no meanwhiles
#else
	if ( AreInMeanwhile( ) )
	{
		return(FALSE);
	}
#endif

	// let's see what happens...
	if ( pSoldier->stats.bLife < OKLIFE )
		return(FALSE);

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
		return(FALSE);

	if ( AM_A_ROBOT( pSoldier ) )
		return(FALSE);

	if ( pSoldier->bAssignment == ASSIGNMENT_POW )
		return(FALSE);

	CHAR16					gzQuoteStr[500];

	// remove old box, in case that still exists
	RemoveDDBox( aEvent.usSpeaker );

	DDBox *pDDBox = GetDDBox( aEvent.usSpeaker );

	if ( !pDDBox )
	{
		return FALSE;
	}

	UINT16 sX = 10;
	if ( aEvent.usSide == DOST_POSITION_RIGHT )
		sX = SCREEN_WIDTH - (MYBOX_FACE_OFFSET + MYBOX_TEXT_MAXWIDTH);
	else if ( aEvent.usSide == DOST_POSITION_MIDDLE )
		sX = (SCREEN_WIDTH - (MYBOX_FACE_OFFSET + MYBOX_TEXT_MAXWIDTH) - 120) / 2;

	UINT16 sY = aEvent.usNumonside * 50;

	switch ( aEvent.ubEventType )
	{
		// victim denies what INTERJECTOR said
	case DOST_VICTIM_TO_INTERJECTOR_DENY:
		// cause denies what imp said
	case DOST_CAUSE_TO_INTERJECTOR_DENY:
		// yep, same dialogue
		wcscpy( gzQuoteStr, szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_DENY[Random( 28 )] );
		break;

		// victim agrees what imp said
	case DOST_VICTIM_TO_INTERJECTOR_AGREE:
		// cause agrees what imp said
	case DOST_CAUSE_TO_INTERJECTOR_AGREE:
		// yep, same dialogue
		wcscpy( gzQuoteStr, szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_AGREE[Random( 17 )] );
		break;
		
		
		// interjector decides to say nothing
	case DOST_INTERJECTOR_NOTHING:
		wcscpy( gzQuoteStr, L"I don't want to say anything, but a bug forces me to! Help! Inform Flugente!" );
		break;

		// another person sides with victim
	case DOST_SIDEWITH_VICTIM:
		wcscpy( gzQuoteStr, szDynamicDialogueText_DOST_SIDEWITH_VICTIM[Random( 19 )] );
		break;

		// another person sides with cause
	case DOST_SIDEWITH_CAUSE:
		wcscpy( gzQuoteStr, szDynamicDialogueText_DOST_SIDEWITH_CAUSE[Random( 18 )] );
		break;

	default:
		wcscpy( gzQuoteStr, szDynamicDialogueText[aEvent.data.event.ubEventId][aEvent.ubEventType] );
		break;
	}

	// insert character nicknames
	ReplaceTextWithOtherText( gzQuoteStr, L"$VICTIM$", gMercProfiles[aEvent.data.event.ubProfileComplainant].zNickname );
	ReplaceTextWithOtherText( gzQuoteStr, L"$CAUSE$", gMercProfiles[aEvent.data.event.ubProfileCause].zNickname );
	ReplaceTextWithOtherText( gzQuoteStr, L"$INTERJECTOR$", gMercProfiles[aEvent.data.ubProfileINTERJECTOR].zNickname );
	ReplaceTextWithOtherText( gzQuoteStr, L"$SPEAKER$", gMercProfiles[aEvent.usSpeaker].zNickname );

	// insert 'he'/'she', 'him'/'her'
	ReplaceTextWithOtherText( gzQuoteStr, L"$VICTIM_GENDER$", (gMercProfiles[aEvent.data.event.ubProfileComplainant].bSex == MALE) ? szDynamicDialogueText_GenderText[0] : szDynamicDialogueText_GenderText[1] );
	ReplaceTextWithOtherText( gzQuoteStr, L"$VICTIM_PRONOUN$", (gMercProfiles[aEvent.data.event.ubProfileComplainant].bSex == MALE) ? szDynamicDialogueText_GenderText[2] : szDynamicDialogueText_GenderText[3] );
	ReplaceTextWithOtherText( gzQuoteStr, L"$CAUSE_GENDER$", (gMercProfiles[aEvent.data.event.ubProfileCause].bSex == MALE) ? szDynamicDialogueText_GenderText[0] : szDynamicDialogueText_GenderText[1] );
	ReplaceTextWithOtherText( gzQuoteStr, L"$CAUSE_PRONOUN$", (gMercProfiles[aEvent.data.event.ubProfileCause].bSex == MALE) ? szDynamicDialogueText_GenderText[2] : szDynamicDialogueText_GenderText[3] );
	ReplaceTextWithOtherText( gzQuoteStr, L"$INTERJECTOR_GENDER$", (gMercProfiles[aEvent.data.ubProfileINTERJECTOR].bSex == MALE) ? szDynamicDialogueText_GenderText[0] : szDynamicDialogueText_GenderText[1] );
	ReplaceTextWithOtherText( gzQuoteStr, L"$INTERJECTOR_PRONOUN$", (gMercProfiles[aEvent.data.ubProfileINTERJECTOR].bSex == MALE) ? szDynamicDialogueText_GenderText[2] : szDynamicDialogueText_GenderText[3] );
	
	pDDBox->SetText( gzQuoteStr );
	pDDBox->SetEvent( aEvent );
	pDDBox->Create( sX, sY );
	pDDBox->SetStartTime( aEvent.usStarttime );

	// print out the dialogue to the log, too
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", pSoldier->GetName(), gzQuoteStr );

	// delay destruction of all current boxes - as long as new dialogue comes in, they will be kept
	DelayBoxDestruction( aEvent.usStarttime + gGameExternalOptions.sDynamicDialogueTimeOffset * 3.5f );

	// on certain dialogues, we can choose an answer
	if ( aEvent.ubEventType == DOST_INTERJECTOR_DIALOGUESELECTION )
	{
		std::vector<std::pair<INT16, STR16> > entryvec;
		for ( UINT8 i = 0; i < DOST_CHOICE_MAX; ++i )
		{
			wcscpy( stupidarray[i], szDynamicDialogueText_DOST_INTERJECTOR_DIALOGUESELECTION_SHORTTEXT[i] );

			// insert character nicknames
			ReplaceTextWithOtherText( stupidarray[i], L"$VICTIM$", gMercProfiles[aEvent.data.event.ubProfileComplainant].zNickname );
			ReplaceTextWithOtherText( stupidarray[i], L"$CAUSE$", gMercProfiles[aEvent.data.event.ubProfileCause].zNickname );
			ReplaceTextWithOtherText( stupidarray[i], L"$INTERJECTOR$", gMercProfiles[aEvent.data.ubProfileINTERJECTOR].zNickname );
			ReplaceTextWithOtherText( stupidarray[i], L"SPEAKER", gMercProfiles[aEvent.usSpeaker].zNickname );

			// insert 'he'/'she', 'him'/'her'
			ReplaceTextWithOtherText( stupidarray[i], L"$VICTIM_GENDER$", (gMercProfiles[aEvent.data.event.ubProfileComplainant].bSex == MALE) ? szDynamicDialogueText_GenderText[0] : szDynamicDialogueText_GenderText[1] );
			ReplaceTextWithOtherText( stupidarray[i], L"$VICTIM_PRONOUN$", (gMercProfiles[aEvent.data.event.ubProfileComplainant].bSex == MALE) ? szDynamicDialogueText_GenderText[2] : szDynamicDialogueText_GenderText[3] );
			ReplaceTextWithOtherText( stupidarray[i], L"$CAUSE_GENDER$", (gMercProfiles[aEvent.data.event.ubProfileCause].bSex == MALE) ? szDynamicDialogueText_GenderText[0] : szDynamicDialogueText_GenderText[1] );
			ReplaceTextWithOtherText( stupidarray[i], L"$CAUSE_PRONOUN$", (gMercProfiles[aEvent.data.event.ubProfileCause].bSex == MALE) ? szDynamicDialogueText_GenderText[2] : szDynamicDialogueText_GenderText[3] );

			entryvec.push_back( std::make_pair( i, stupidarray[i] ) );
		}

		IMPDialogueChooseBox_Static::getInstance( ).SetEvent( aEvent );
		IMPDialogueChooseBox_Static::getInstance( ).SetEntries( entryvec );
		IMPDialogueChooseBox_Static::getInstance( ).SetHelpText( szDynamicDialogueText[aEvent.data.event.ubEventId][DOST_INTERJECTOR_DIALOGUESELECTION] );
		IMPDialogueChooseBox_Static::getInstance( ).Create( sX, sY + 60 );
		IMPDialogueChooseBox_Static::getInstance( ).SetStartTime( aEvent.usStarttime );
		IMPDialogueChooseBox_Static::getInstance( ).SetEndTime( aEvent.usStarttime + 2 * gGameExternalOptions.sDynamicDialogueTimeOffset );
	}

	return TRUE;
}

INT8 GetSidePosition( UINT8 aDostPosition )
{
	INT8 bestpos = -1;

	UINT8 size = gDynamicOpinionSpeechInCurrentDialogue.size( );
	for ( UINT8 i = 0; i < size; ++i )
	{
		if ( gDynamicOpinionSpeechInCurrentDialogue[i].usSide == aDostPosition )
		{
			bestpos = max( bestpos, gDynamicOpinionSpeechInCurrentDialogue[i].usNumonside );
		}
	}

	return bestpos + 1;
}

BOOLEAN fDynamicOpinionSpeechInProgress = FALSE;
BOOLEAN DynamicOpinionSpeechInProgress( )
{
	return fDynamicOpinionSpeechInProgress;
}

void SetfDynamicOpinionSpeechInProgress( BOOLEAN aVal )
{
	fDynamicOpinionSpeechInProgress = aVal;
}


void AddOpinionEvent( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent, BOOLEAN fStartDialogue )
{
	if ( usProfileA == NO_PROFILE || usProfileB == NO_PROFILE )
		return;

	if ( usProfileA == usProfileB )
		return;

	if ( usEvent >= OPINIONEVENT_MAX )
		return;

	// don't use event if opinion modifier is off (thereby providing an easy way to turn off parts of this feature)
	if ( !gDynamicOpinionEvent[usEvent].sOpinionModifier )
		return;

	// we don't want any events for people not on our team or vehicles
	SOLDIERTYPE* pSoldier = FindSoldierByProfileID( usProfileA, TRUE );
	if ( !pSoldier || pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		return;

	pSoldier = FindSoldierByProfileID( usProfileB, TRUE );
	if ( !pSoldier || pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		return;

	UINT32 ustmpFlagmask[OPINION_FLAGMASKS_NUMBER];
	for ( UINT8 i = 0; i < OPINION_FLAGMASKS_NUMBER; ++i )
	{
		ustmpFlagmask[i] = gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i];
	}

	// we add a flag signifiying that an event happened today
	switch ( usEvent )
	{
	case OPINIONEVENT_FRIENDLYFIRE:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_FRIENDLYFIRE;	break;
	case OPINIONEVENT_SNITCHSOLDMEOUT:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_SNITCHSOLDMEOUT;	break;
	case OPINIONEVENT_SNITCHINTERFERENCE:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_INTERFERENCE;	break;
	case OPINIONEVENT_FRIENDSWITHHATED:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_FRIENDSWITHHATED;	break;
	case OPINIONEVENT_CONTRACTEXTENSION:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_CONTRACTEXTENSION;	break;
	case OPINIONEVENT_ORDEREDRETREAT:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_ORDEREDRETREAT;	break;
	case OPINIONEVENT_CIVKILLER:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_CIVKILLER;	break;
	case OPINIONEVENT_SLOWSUSDOWN:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_SLOWSUSDOWN;	break;

	case OPINIONEVENT_NOSHARINGFOOD:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_NOSHARINGFOOD;	break;
	case OPINIONEVENT_ANNOYINGDISABILITY:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_ANNOYINGDISABILITY;	break;
	case OPINIONEVENT_ADDICT:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_ADDICT;	break;
	case OPINIONEVENT_THIEF:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_THIEF;	break;
	case OPINIONEVENT_WORSTCOMMANDEREVER:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_WORSTCOMMANDEREVER;	break;
	case OPINIONEVENT_RICHGUY:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_RICHGUY;	break;
	case OPINIONEVENT_BETTERGEAR:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_BETTERGEAR;	break;
	case OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS:	gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_YOUMOUNTEDAGUNONMYBREASTS;	break;

	case OPINIONEVENT_BANDAGED:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_BANDAGED;	break;

		// drinking flags are a bit different - drinking can apply the GOOD or BAD flag, and they cancel each other out.
		// GOOD on GOOD causes SUPER, which cannot be removed (and stops this for today)
		// BAD on BAD causes WORSE, which cannot be removed (and stops this for today)
	case OPINIONEVENT_DRINKBUDDIES_GOOD:
		// if we do not already have the SUPER flag...
		if ( !(gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & (OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER | OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE)) )
		{
			// if we already have GOOD, get SUPER
			if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD )
			{
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER;
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD;
			}
			// if we have BAD, cancel it out
			else if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD )
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD;
			// if we have nothing, get GOOD
			else
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD;
		}
		break;

	case OPINIONEVENT_DRINKBUDDIES_BAD:
		// if we do not already have the SUPER flag...
		if ( !(gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & (OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER | OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE)) )
		{
			// if we already have BAD, get WORSE
			if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD )
			{
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE;
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD;
			}
			// if we have GOOD, cancel it out
			else if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD )
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD;
			// if we have nothing, get BAD
			else
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD;
		}
		break;

	case OPINIONEVENT_AGAINST_US:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_AGAINST_US;	break;
	case OPINIONEVENT_FOR_US:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_FOR_US;	break;
	case OPINIONEVENT_AGAINST_ENEMY:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_AGAINST_ENEMY;	break;

	case OPINIONEVENT_FOR_ENEMY:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_FOR_ENEMY;	break;
	case OPINIONEVENT_SOLVECONFLICT_REASON_GOOD:	gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_SOLVECONFLICT_REASON_GOOD;	break;
	case OPINIONEVENT_SOLVECONFLICT_REASON_BAD:		gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_SOLVECONFLICT_REASON_BAD;	break;
	case OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_GOOD:gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_SOLVECONFLICT_AGGRESSIVE_GOOD;	break;
	case OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_BAD:	gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_SOLVECONFLICT_AGGRESSIVE_BAD;	break;
	case OPINIONEVENT_DISEASE_DISGUSTING:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_DISEASE_DISGUSTING;	break;
	case OPINIONEVENT_DISEASE_TREATMENT:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_DISEASE_TREATMENT;	break;
	case OPINIONEVENT_BRUTAL_GOOD:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] |= OPINIONFLAG_STAGE1_BRUTAL_GOOD;	break;

	case OPINIONEVENT_BRUTAL_BAD:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_BRUTAL_BAD;	break;
	case OPINIONEVENT_TEACHER:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_TEACHER;	break;
	case OPINIONEVENT_BESTCOMMANDEREVER:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_BESTCOMMANDEREVER;	break;
	case OPINIONEVENT_BATTLE_SAVIOUR:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_BATTLE_SAVIOUR;	break;
	case OPINIONEVENT_FRAGTHIEF:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_FRAGTHIEF;	break;
	case OPINIONEVENT_BATTLE_ASSIST:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_BATTLE_ASSIST;	break;
	case OPINIONEVENT_BATTLE_TOOK_PRISONER:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_BATTLE_TOOK_PRISONER;	break;
	case OPINIONEVENT_CIV_ATTACKER:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] |= OPINIONFLAG_STAGE1_CIV_ATTACKER;	break;
		
	default:		break;
	}

	// if we set a flag, then this was the first time an event happened today - we can start a dialogue
	BOOLEAN fSomethingChanged = FALSE;
	for ( UINT8 i = 0; i < OPINION_FLAGMASKS_NUMBER; ++i )
	{
		if ( ustmpFlagmask[i] != gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i] )
		{
			fSomethingChanged = TRUE;
			break;
		}
	}

	if ( fSomethingChanged )
	{
		// if this option is turned on, a small message will show us how opinions have changed
		if ( gGameExternalOptions.fDynamicOpinionsShowChange )
		{
			if ( gDynamicOpinionEvent[usEvent].sOpinionModifier >= 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTGREEN, MSG_INTERFACE, L"%s: %s +%d", gMercProfiles[usProfileA].zNickname, gMercProfiles[usProfileB].zNickname, gDynamicOpinionEvent[usEvent].sOpinionModifier );
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"%s: %s %d", gMercProfiles[usProfileA].zNickname, gMercProfiles[usProfileB].zNickname, gDynamicOpinionEvent[usEvent].sOpinionModifier );
			}
		}

		// if dialogue is allowed, set up dialogue
		if ( fStartDialogue && gGameExternalOptions.fDynamicDialogue )
		{
			// we can set up dialogues now
			OpinionEvent opinionevent;
			opinionevent.ubEventId = usEvent;					// the dynamic opinion event this is all about
			opinionevent.ubProfileComplainant = usProfileA;		// the merc who started this dialogue by complaining
			opinionevent.ubProfileCause = usProfileB;			// the merc who caused the event

			INT16 idA = GetSoldierIDFromMercID( usProfileA );

			if ( idA >= 0 && MercPtrs[idA] )
			{
				opinionevent.usSector = SECTOR( MercPtrs[idA]->sSectorX, MercPtrs[idA]->sSectorY );

				AddDynamicOpinionEvent_New( opinionevent );
			}
		}
	}
}

// get usProfileA's opinion of usProfileB concerning usEvent on one of the last 4 days
INT8 GetDynamicOpinionDay( UINT8 usProfileA, UINT8 usProfileB, UINT8 usDay )
{
	INT32 opinion = 0;

	if ( usProfileA == NO_PROFILE || usProfileA == NO_PROFILE )
		return opinion;

	// only for today (0) or one of the last 3 days (1-3)
	if ( usDay > 3 )
		return opinion;

	for ( UINT8 event = 0; event < OPINIONEVENT_MAX; ++event )
	{
		// every 8 events, we use another mask (one mask = 32 bits, one event use 4 bits)
		UINT8 mask = event / 8;

		UINT32 flag = 0x00000001 << (4 * (event % 8));

		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][mask] & flag << usDay )
			opinion += gDynamicOpinionEvent[event].sOpinionModifier;
	}

	// cut it down to INT8
	return (INT8)opinion;
}

// get usProfileA's opinion of usProfileB concerning usEvent
INT8 GetDynamicOpinion( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent )
{
	INT32 opinion = 0;

	if ( usProfileA == NO_PROFILE || usProfileA == NO_PROFILE )
		return opinion;

	if ( usEvent >= OPINIONEVENT_MAX )
		return opinion;

	// count how many relevant flags are set
	UINT8 numflags = 0;
	switch ( usEvent )
	{
	case OPINIONEVENT_FRIENDLYFIRE:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_FRIENDLYFIRE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_FRIENDLYFIRE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_FRIENDLYFIRE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_FRIENDLYFIRE )	++numflags;
		break;

	case OPINIONEVENT_SNITCHSOLDMEOUT:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_SNITCHSOLDMEOUT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_SNITCHSOLDMEOUT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_SNITCHSOLDMEOUT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_SNITCHSOLDMEOUT )	++numflags;
		break;

	case OPINIONEVENT_SNITCHINTERFERENCE:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_INTERFERENCE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_INTERFERENCE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_INTERFERENCE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_INTERFERENCE )	++numflags;
		break;

	case OPINIONEVENT_FRIENDSWITHHATED:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_FRIENDSWITHHATED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_FRIENDSWITHHATED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_FRIENDSWITHHATED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_FRIENDSWITHHATED )	++numflags;
		break;

	case OPINIONEVENT_CONTRACTEXTENSION:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_CONTRACTEXTENSION )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_CONTRACTEXTENSION )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_CONTRACTEXTENSION )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_CONTRACTEXTENSION )	++numflags;
		break;

	case OPINIONEVENT_ORDEREDRETREAT:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_ORDEREDRETREAT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_ORDEREDRETREAT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_ORDEREDRETREAT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_ORDEREDRETREAT )	++numflags;
		break;

	case OPINIONEVENT_CIVKILLER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_CIVKILLER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_CIVKILLER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_CIVKILLER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_CIVKILLER )	++numflags;
		break;

	case OPINIONEVENT_SLOWSUSDOWN:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_SLOWSUSDOWN )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_SLOWSUSDOWN )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_SLOWSUSDOWN )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_SLOWSUSDOWN )	++numflags;
		break;

	case OPINIONEVENT_NOSHARINGFOOD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_NOSHARINGFOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_NOSHARINGFOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_NOSHARINGFOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_NOSHARINGFOOD )	++numflags;
		break;

	case OPINIONEVENT_ANNOYINGDISABILITY:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_ANNOYINGDISABILITY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_ANNOYINGDISABILITY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_ANNOYINGDISABILITY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_ANNOYINGDISABILITY )	++numflags;
		break;

	case OPINIONEVENT_ADDICT:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_ADDICT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_ADDICT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_ADDICT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_ADDICT )	++numflags;
		break;

	case OPINIONEVENT_THIEF:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_THIEF )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_THIEF )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_THIEF )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_THIEF )	++numflags;
		break;

	case OPINIONEVENT_WORSTCOMMANDEREVER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_WORSTCOMMANDEREVER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_WORSTCOMMANDEREVER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_WORSTCOMMANDEREVER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_WORSTCOMMANDEREVER )	++numflags;
		break;

	case OPINIONEVENT_RICHGUY:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_RICHGUY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_RICHGUY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_RICHGUY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_RICHGUY )	++numflags;
		break;

	case OPINIONEVENT_BETTERGEAR:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_BETTERGEAR )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_BETTERGEAR )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_BETTERGEAR )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_BETTERGEAR )	++numflags;
		break;

	case OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_YOUMOUNTEDAGUNONMYBREASTS )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_YOUMOUNTEDAGUNONMYBREASTS )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_YOUMOUNTEDAGUNONMYBREASTS )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_YOUMOUNTEDAGUNONMYBREASTS )	++numflags;
		break;

	case OPINIONEVENT_BANDAGED:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_BANDAGED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_BANDAGED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_BANDAGED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_BANDAGED )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_GOOD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_GOOD )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_SUPER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_SUPER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_SUPER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_SUPER )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_BAD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_BAD )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_WORSE:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_WORSE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_WORSE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_WORSE )	++numflags;
		break;

	case OPINIONEVENT_AGAINST_US:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_AGAINST_US )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_AGAINST_US )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_AGAINST_US )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_AGAINST_US )	++numflags;
		break;

	case OPINIONEVENT_FOR_US:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_FOR_US )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_FOR_US )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_FOR_US )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_FOR_US )	++numflags;
		break;

	case OPINIONEVENT_AGAINST_ENEMY:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_AGAINST_ENEMY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_AGAINST_ENEMY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_AGAINST_ENEMY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_AGAINST_ENEMY )	++numflags;
		break;

	case OPINIONEVENT_FOR_ENEMY:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_FOR_ENEMY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_FOR_ENEMY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_FOR_ENEMY )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_FOR_ENEMY )	++numflags;
		break;

	case OPINIONEVENT_SOLVECONFLICT_REASON_GOOD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_SOLVECONFLICT_REASON_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_SOLVECONFLICT_REASON_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_SOLVECONFLICT_REASON_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_SOLVECONFLICT_REASON_GOOD )	++numflags;
		break;

	case OPINIONEVENT_SOLVECONFLICT_REASON_BAD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_SOLVECONFLICT_REASON_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_SOLVECONFLICT_REASON_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_SOLVECONFLICT_REASON_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_SOLVECONFLICT_REASON_BAD )	++numflags;
		break;

	case OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_GOOD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_SOLVECONFLICT_AGGRESSIVE_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_SOLVECONFLICT_AGGRESSIVE_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_SOLVECONFLICT_AGGRESSIVE_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_SOLVECONFLICT_AGGRESSIVE_GOOD )	++numflags;
		break;

	case OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_BAD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_SOLVECONFLICT_AGGRESSIVE_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_SOLVECONFLICT_AGGRESSIVE_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_SOLVECONFLICT_AGGRESSIVE_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_SOLVECONFLICT_AGGRESSIVE_BAD )	++numflags;
		break;

	case OPINIONEVENT_DISEASE_DISGUSTING:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_DISEASE_DISGUSTING )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_DISEASE_DISGUSTING )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_DISEASE_DISGUSTING )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_DISEASE_DISGUSTING )	++numflags;
		break;

	case OPINIONEVENT_DISEASE_TREATMENT:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_DISEASE_TREATMENT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_DISEASE_TREATMENT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_DISEASE_TREATMENT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_DISEASE_TREATMENT )	++numflags;
		break;

	case OPINIONEVENT_BRUTAL_GOOD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE1_BRUTAL_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE2_BRUTAL_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE3_BRUTAL_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_BRUTAL_GOOD )	++numflags;
		break;

	case OPINIONEVENT_BRUTAL_BAD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_BRUTAL_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_BRUTAL_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_BRUTAL_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_BRUTAL_BAD )	++numflags;
		break;

	case OPINIONEVENT_TEACHER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_TEACHER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_TEACHER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_TEACHER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_TEACHER )	++numflags;
		break;
		
	case OPINIONEVENT_BESTCOMMANDEREVER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_BESTCOMMANDEREVER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_BESTCOMMANDEREVER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_BESTCOMMANDEREVER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_BESTCOMMANDEREVER )	++numflags;
		break;

	case OPINIONEVENT_BATTLE_SAVIOUR:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_BATTLE_SAVIOUR )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_BATTLE_SAVIOUR )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_BATTLE_SAVIOUR )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_BATTLE_SAVIOUR )	++numflags;
		break;

	case OPINIONEVENT_FRAGTHIEF:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_FRAGTHIEF )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_FRAGTHIEF )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_FRAGTHIEF )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_FRAGTHIEF )	++numflags;
		break;

	case OPINIONEVENT_BATTLE_ASSIST:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_BATTLE_ASSIST )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_BATTLE_ASSIST )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_BATTLE_ASSIST )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_BATTLE_ASSIST )	++numflags;
		break;

	case OPINIONEVENT_BATTLE_TOOK_PRISONER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_BATTLE_TOOK_PRISONER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_BATTLE_TOOK_PRISONER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_BATTLE_TOOK_PRISONER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_BATTLE_TOOK_PRISONER )	++numflags;
		break;

	case OPINIONEVENT_CIV_ATTACKER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE1_CIV_ATTACKER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE2_CIV_ATTACKER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE3_CIV_ATTACKER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][4] & OPINIONFLAG_STAGE4_CIV_ATTACKER )	++numflags;
		break;

	default:
		break;
	}

	// event opinion is number of times this happened times opinion modifer
	opinion = numflags * gDynamicOpinionEvent[usEvent].sOpinionModifier;

	// cut it down to INT8
	return (INT8)opinion;
}

// daily refresh of opinions
void HandleDynamicOpinionsDailyRefresh( )
{
	// a day has passed. We move all opinions one day over, but first, we remember the events of day 3 to 'long-term memory'
	for ( UINT8 usProfileA = 0; usProfileA < NUM_PROFILES; ++usProfileA )
	{
		for ( UINT8 usProfileB = 0; usProfileB < NUM_PROFILES; ++usProfileB )
		{
			// only do this if there are any flags to begin with
			if ( (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_ALL)
				 || (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_ALL)
				 || (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_ALL)
				 || (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][3] & OPINIONFLAG_STAGE4_ALL) )
			{
				INT8 oldday3opinion = GetDynamicOpinionDay( usProfileA, usProfileB, 3 );

				// we as time passes, past events aren't that important to us anymore - remember only a fraction of it
				oldday3opinion = oldday3opinion / 5;

				// add this to our long-term memory
				gMercProfiles[usProfileA].sDynamicOpinionLongTerm[usProfileB] += oldday3opinion;
			}								
		}

		// each profile has its opinions on everyone else renewed
		RolloverDynamicOpinions( usProfileA );
	}

	SOLDIERTYPE*		pSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE &&
			 !(pSoldier->bAssignment == IN_TRANSIT ||
			 pSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			// or each profile, check wether everyone else is a friend of someone else we hate
			CheckForFriendsofHated( pSoldier );

			// get annoyed on mercs that receive a lot more money than we do
			HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_RICHGUY, FALSE, FALSE );

			// get annoyed if other people have better gear
			HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_BETTERGEAR, FALSE, FALSE );
		}
	}
}

// a day has passed, 'age' opinions 
void RolloverDynamicOpinions( UINT8 usProfileA )
{
	for ( UINT8 usProfileB = 0; usProfileB < NUM_PROFILES; ++usProfileB )
	{
		for ( UINT8 i = 0; i < OPINION_FLAGMASKS_NUMBER; ++i )
		{
			// events that are at stage 4 are forgotten
			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i] &= ~OPINIONFLAG_STAGE4_ALL;

			// all other events move up one stage
			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i] = gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i] << 1;
		}
	}
}

// check wether other people are friends with someone else we hate. All persons must be in Arulco
void CheckForFriendsofHated( SOLDIERTYPE* pSoldier )
{
	INT8									bMercID, bOtherID, bThirdID;
	INT8									bOpinion = -1;
	INT8									bSecondOpinion = -1;
	INT8									bLastTeamID;
	SOLDIERTYPE*							pOtherSoldier;
	SOLDIERTYPE*							pThirdSoldier;

	// make sure we ourselves aren't in transit
	if ( !pSoldier->bActive || pSoldier->ubProfile == NO_PROFILE || pSoldier->bAssignment == IN_TRANSIT || pSoldier->bAssignment == ASSIGNMENT_DEAD )
		return;

	bMercID = pSoldier->ubID;
	bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;

	// loop through all other mercs
	bOtherID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	for ( pOtherSoldier = MercPtrs[bOtherID]; bOtherID <= bLastTeamID; ++bOtherID, pOtherSoldier++ )
	{
		// skip past ourselves and all inactive mercs
		if ( bOtherID != bMercID && pOtherSoldier->bActive && pOtherSoldier->ubProfile != NO_PROFILE &&
			 !(pOtherSoldier->bAssignment == IN_TRANSIT ||
			 pOtherSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			bOpinion = SoldierRelation( pSoldier, pOtherSoldier );

			// we cannot simply check for HATED_OPINION here - this very feature makes opinions not so easy anymore. Simply check for a range
			if ( bOpinion < -20 )
			{
				// there is someone in our team that we hate. We dislike his friends somewhat, purely because they like our foe
				// loop through all other mercs
				bThirdID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
				for ( pThirdSoldier = MercPtrs[bThirdID]; bThirdID <= bLastTeamID; ++bThirdID, pThirdSoldier++ )
				{
					// skip past ourselves and all inactive mercs
					if ( bThirdID != bMercID && bThirdID != bOtherID && pThirdSoldier->bActive && pThirdSoldier->ubProfile != NO_PROFILE &&
						 !(pThirdSoldier->bAssignment == IN_TRANSIT ||
						 pThirdSoldier->bAssignment == ASSIGNMENT_DEAD) )
					{
						bSecondOpinion = SoldierRelation( pThirdSoldier, pOtherSoldier );

						if ( bSecondOpinion > 20 )
						{
							// this guy is friends with someone we hate! We dislike him a bit for that
							AddOpinionEvent( pSoldier->ubProfile, pThirdSoldier->ubProfile, OPINIONEVENT_FRIENDSWITHHATED );
						}
					}
				}
			}
		}
	}
}

void HandleDynamicOpinionOnContractExtension( UINT8 ubCode, UINT8 usProfile )
{
	if ( usProfile == NO_PROFILE )
		return;

	if ( ubCode == EXTENDED_CONTRACT_BY_1_DAY || ubCode == EXTENDED_CONTRACT_BY_1_WEEK || ubCode == EXTENDED_CONTRACT_BY_2_WEEKS )
	{
		INT16 id = GetSoldierIDFromMercID( usProfile );
		if ( id > -1 )
		{
			SOLDIERTYPE* pSoldierWhoGotPaid = MercPtrs[id];

			// only for AIM mercs
			if ( pSoldierWhoGotPaid->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
				return;

			// determine the remaining length of his contract BEFORE it go renewed
			INT32 oldcontract = pSoldierWhoGotPaid->iEndofContractTime;
			if ( ubCode == EXTENDED_CONTRACT_BY_1_DAY )
				oldcontract -= 1440;
			else if ( ubCode == EXTENDED_CONTRACT_BY_1_WEEK )
				oldcontract -= 7 * 1440;
			else if ( ubCode == EXTENDED_CONTRACT_BY_2_WEEKS )
				oldcontract -= 14 * 1440;

			// someones contract got extended. Other mercs who have less time on their contract will be annoyed by this, as they feel they shoul be paid first
			SOLDIERTYPE*		pSoldier = NULL;
			UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
			UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
			for ( pSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pSoldier++ )
			{
				if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE && pSoldier->ubProfile != usProfile &&
					 !(pSoldier->bAssignment == IN_TRANSIT ||
					 pSoldier->bAssignment == ASSIGNMENT_DEAD) )
				{
					// only for AIM mercs
					if ( pSoldier->ubProfile == NO_PROFILE || pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
						continue;

					if ( pSoldier->iEndofContractTime < oldcontract )
					{
						// this guy got paid at a point where we had less time than he did! Favouritism!
						AddOpinionEvent( pSoldier->ubProfile, usProfile, OPINIONEVENT_CONTRACTEXTENSION );
					}
				}
			}
		}
	}
}

// depending on what happened in the battle, our mercs might complain or celebrate
void HandleDynamicOpinionBattleFinished( BOOLEAN fBattleWon )
{
	UINT32 ourlosses = 0;
	UINT32 enemylosses = 0;
	UINT32 oursidesize = 0;
	UINT32 enemysidesize = 0;
	
	// pick the 'leader' who gets all the praise (or blame, depending on how this went)
	UINT8 leaderid = GetBestMercLeaderInSector( SECTORX( gCurrentIncident.usSector ), SECTORY( gCurrentIncident.usSector ), (INT8)gCurrentIncident.usLevel );

	if ( leaderid != NOBODY )
	{
		// while it would be logical to also take civilian losses into account, this would lead to mercs always disapproving engaging hostile civilain factions like Kingpin and the Hicks
		for ( UINT16 i = 0; i < CAMPAIGNHISTORY_SD_CIV; ++i )
		{
			ourlosses += 10 * gCurrentIncident.usKills[i];
			ourlosses += gCurrentIncident.usWounds[i];
			ourlosses += 8 * gCurrentIncident.usPrisoners[i];		// having our side being captured is not quite as bad as having them killed

			oursidesize += gCurrentIncident.usParticipants[i];
		}

		for ( UINT16 i = CAMPAIGNHISTORY_SD_ENEMY_ADMIN; i < CAMPAIGNHISTORY_SD_MAX; ++i )
		{
			enemylosses += 10 * gCurrentIncident.usKills[i];
			enemylosses += 11 * gCurrentIncident.usPrisoners[i];	// capturing the enemy is even better than killing them

			enemysidesize += gCurrentIncident.usParticipants[i];
		}

		// if we've taken more losses than the enemy, our mercs will complain - even if we won. Our mercs don't seem to like phyrric victories...
		if ( ourlosses > enemylosses && ourlosses > 75 )
		{
			HandleDynamicOpinionChange( MercPtrs[leaderid], OPINIONEVENT_WORSTCOMMANDEREVER, TRUE, TRUE );
		}
		else
		{
			// our mercs praise their leader if
			// they won the battle
			// the enemy outnumbered them at least 2:1
			// the enemy had at least 4 times their losses
			// the enemy patrol was of significant size (this is supposed to be given for huge battles only)
			if ( fBattleWon && 2 * oursidesize < enemysidesize && 4 * ourlosses < enemylosses && enemylosses > 10 * gGameExternalOptions.iMaxEnemyGroupSize )
			{
				HandleDynamicOpinionChange( MercPtrs[leaderid], OPINIONEVENT_BESTCOMMANDEREVER, TRUE, TRUE );
			}
		}
	}
}

void HandleDynamicOpinionRetreat( )
{
	// This was a disaster (Ignoring of how high the enemies losses were to create drama :-) )! Let's blame the player -> blame an IMP!
	std::vector<UINT8> aTaboo;
	UINT8 impid = GetBestMercLeaderInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	// we've found someone competent. Let's all blame him for this disaster!
	if ( impid != NOBODY )
	{
		HandleDynamicOpinionChange( MercPtrs[impid], OPINIONEVENT_ORDEREDRETREAT, TRUE, TRUE );
	}
}

void HandleDynamicOpinionTeamDrinking( SOLDIERTYPE* pSoldier )
{
	// need to be drunk for this
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE || pSoldier->newdrugs.drinkstaken <= 0.0 )
		return;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// everybody other merc in the same sector can get updated if they are drugged too
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			 pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ &&
			 pTeamSoldier->newdrugs.drinkstaken > 0.0 &&
			 !(pTeamSoldier->bAssignment == IN_TRANSIT ||
			 pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			// both mercs drink together, they opinion either improve or worsen
			// added a few chance calls, because this can lead to an absurd amount of events
			if ( Chance( 50 ) )
			{
				if ( Chance( 67 ) )
					AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_GOOD, Chance( 25 ) );
				else
					AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_BAD, Chance( 25 ) );
			}

			if ( Chance( 50 ) )
			{
				if ( Chance( 67 ) )
					AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_GOOD, Chance( 25 ) );
				else
					AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_BAD, Chance( 25 ) );
			}
		}
	}
}

void HandleDynamicOpinionTeaching( SOLDIERTYPE* pSoldier, UINT8 ubStat )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE  )
		return;

	// because this code is as annoying as it is, ubStat has a different numbering than bTrainStat. The only reasonable explanation for that is that the designers decided to be jerks on purpose
	// we have to get the correct bTrainStat
	INT8 trainstat = 0;
	switch ( ubStat )
	{
	case STRAMT:		trainstat = STRENGTH; break;
	case DEXTAMT:		trainstat = DEXTERITY; break;
	case AGILAMT:		trainstat = AGILITY; break;
	case HEALTHAMT:		trainstat = HEALTH; break;
	case LDRAMT:		trainstat = LEADERSHIP; break;
	case MARKAMT:		trainstat = MARKSMANSHIP; break;
	case EXPLODEAMT:	trainstat = EXPLOSIVE_ASSIGN; break;
	case MEDICALAMT:	trainstat = MEDICAL; break;
	case MECHANAMT:		trainstat = MECHANICAL; break;

	default:
		// other stats cannot be trained (including wisdom), so get out of here
		return;
		break;
	}

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// award event for every trainer in this sector
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			 pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ &&
			  !(pTeamSoldier->bAssignment == IN_TRANSIT || pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			// if he's training teammates in this stat
			if ( (pTeamSoldier->bAssignment == TRAIN_TEAMMATE) && (pTeamSoldier->bTrainStat == trainstat) && (EnoughTimeOnAssignment( pTeamSoldier )) && !pTeamSoldier->flags.fMercAsleep )
			{
				AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_TEACHER );
			}
		}
	}
}

// some events require a 'leader' -  merc the team will regard as the one being in charge, and subsequently being praised or damned for the way things develop
UINT32 GetSoldierLeaderRating( SOLDIERTYPE* pSoldier )
{
	if ( !pSoldier )
		return 0;

	// rating is based on leadership, experience and the squad leader trait. IMPs get a higher rating, as they represent the player closest
	UINT32 rating = 0;

	rating += pSoldier->stats.bLeadership;
	rating += 10 * pSoldier->stats.bExpLevel;
	rating += 30 * NUM_SKILL_TRAITS( pSoldier, SQUADLEADER_NT );
	rating += 50 * (pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER);

	return rating;
}


UINT8 GetBestMercLeaderInSector( INT16 sX, INT16 sY, INT8 sZ )
{
	UINT32				highestrating = 0;
	UINT8				bestid = NOBODY;

	SOLDIERTYPE*		pSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, ++pSoldier )
	{
		// everybody other merc in the same sector gets annoyed
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE &&
			 pSoldier->sSectorX == sX && pSoldier->sSectorY == sY && pSoldier->bSectorZ == sZ &&
			 !(pSoldier->bAssignment == IN_TRANSIT || pSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			UINT32 rating = GetSoldierLeaderRating( pSoldier );

			if ( rating > highestrating )
			{
				highestrating = rating;
				bestid = bMercID;
			}
		}
	}

	return bestid;
}

UINT8 GetRandomMercInSectorNotInList( INT16 sX, INT16 sY, INT8 sZ, std::vector<UINT8> aTaboo, BOOLEAN fImpOnly )
{
	std::vector<UINT16>	resultvector;
	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, ++pTeamSoldier )
	{
		// everybody other merc in the same sector gets annoyed
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE &&
			 pTeamSoldier->sSectorX == sX && pTeamSoldier->sSectorY == sY && pTeamSoldier->bSectorZ == sZ &&
			 (!fImpOnly || pTeamSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER) &&
			 !(pTeamSoldier->bAssignment == IN_TRANSIT ||
			 pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			// only add if not already in list
			if ( std::find( aTaboo.begin( ), aTaboo.end( ), pTeamSoldier->ubProfile ) == aTaboo.end( ) )
				resultvector.push_back( bMercID );
		}
	}

	if ( !resultvector.empty( ) )
	{
		return resultvector[Random( resultvector.size( ) )];
	}

	return NOBODY;
}

UINT8 GetFittingInterjectorProfile( UINT8 usEvent, UINT8 usProfileVictim, UINT8 usProfileCause )
{
	if ( usEvent >= OPINIONEVENT_MAX )
		return NO_PROFILE;

	INT16 idVictim = GetSoldierIDFromMercID( usProfileVictim );
	INT16 idCause = GetSoldierIDFromMercID( usProfileCause );

	if ( idVictim < 0 || idCause < 0 )
		return NO_PROFILE;

	SOLDIERTYPE* pSoldierVictim = MercPtrs[idVictim];
	SOLDIERTYPE* pSoldierCause = MercPtrs[idCause];

	if ( !pSoldierVictim || !pSoldierCause )
		return NO_PROFILE;

	std::vector<UINT8>  impprofilevector;
	std::vector<UINT8>  profilevector;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[pSoldierVictim->bTeam].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[pSoldierVictim->bTeam].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// only people that are here
		if ( !pTeamSoldier->bActive || pTeamSoldier->bAssignment == IN_TRANSIT || pTeamSoldier->bAssignment == ASSIGNMENT_DEAD || pTeamSoldier->bAssignment == ASSIGNMENT_POW )
			continue;

		// if fSameSector is TRUE then the teammate must be in the same sector
		if ( gDynamicOpinionEvent[usEvent].fSameSector && (pTeamSoldier->sSectorX != pSoldierVictim->sSectorX || pTeamSoldier->sSectorY != pSoldierVictim->sSectorY || pTeamSoldier->bSectorZ != pSoldierVictim->bSectorZ) )
			continue;

		// if fSameAssignment is TRUE then both mercs must be on the same assignment
		if ( gDynamicOpinionEvent[usEvent].fSameAssignment && pTeamSoldier->bAssignment != pSoldierVictim->bAssignment )
			continue;

		// if fAwake is TRUE then only awake mercs count
		if ( gDynamicOpinionEvent[usEvent].fAwake && pTeamSoldier->flags.fMercAsleep )
			continue;

		// if fSquadsOnly is TRUE then only mercs in combat squads count
		if ( gDynamicOpinionEvent[usEvent].fSquadsOnly && pTeamSoldier->bAssignment >= ON_DUTY )
			continue;

		// exclude victim and cause
		if ( pTeamSoldier->ubProfile == usProfileVictim || pTeamSoldier->ubProfile == usProfileCause )
			continue;
				
		profilevector.push_back( pTeamSoldier->ubProfile );

		if ( pTeamSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER )
			impprofilevector.push_back( pTeamSoldier->ubProfile );
	}

	// return a random IMP, if there is one, otherwise just pick a random team member
	if ( !impprofilevector.empty() )
		return impprofilevector[Random( impprofilevector.size( ) )];

	if ( !profilevector.empty( ) )
		return profilevector[Random( profilevector.size( ) )];

	return NO_PROFILE;
}

UINT8 HighestInventoryCoolness( SOLDIERTYPE* pSoldier )
{
	UINT8 coolness = 0;

	if ( !pSoldier )
		return coolness;

	// search for food in our inventory
	INT8 invsize = (INT8)pSoldier->inv.size( );									// remember inventorysize, so we don't call size() repeatedly
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )							// ... for all items in our inventory ...
	{
		// ... if Item exists
		if ( pSoldier->inv[bLoop].exists( ) )
		{
			coolness = max( coolness, Item[pSoldier->inv[bLoop].usItem].ubCoolness );
		}
	}

	return coolness;
}

void HandleDynamicOpinionChange( SOLDIERTYPE* pSoldier, UINT8 usEvent, BOOLEAN fOffender, BOOLEAN fStartDialogue )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
		return;

	// we might have to compare the soldier to other teammates, determine relevant values
	INT32 iPercentEncumbranceOffended = 0;
	BOOLEAN fCheckFood = FALSE;
	BOOLEAN fCheckDrink = FALSE;
	UINT8 highestcoolness = 0;
	UINT32 meanwage = 0;
	UINT8 explevel = pSoldier->stats.bExpLevel;

	switch ( usEvent )
	{
	case OPINIONEVENT_CONTRACTEXTENSION:
	case OPINIONEVENT_ORDEREDRETREAT:
	case OPINIONEVENT_CIVKILLER:
	case OPINIONEVENT_ANNOYINGDISABILITY:
	case OPINIONEVENT_ADDICT:
	case OPINIONEVENT_THIEF:
	case OPINIONEVENT_WORSTCOMMANDEREVER:
	case OPINIONEVENT_DISEASE_DISGUSTING:
	case OPINIONEVENT_DISEASE_TREATMENT:
	case OPINIONEVENT_BRUTAL_GOOD:
	case OPINIONEVENT_BRUTAL_BAD:
	case OPINIONEVENT_TEACHER:
	case OPINIONEVENT_BESTCOMMANDEREVER:
	case OPINIONEVENT_BATTLE_SAVIOUR:
	case OPINIONEVENT_BATTLE_TOOK_PRISONER:
	case OPINIONEVENT_FRAGTHIEF:
	case OPINIONEVENT_BATTLE_ASSIST:
	case OPINIONEVENT_CIV_ATTACKER:
		break;

	case OPINIONEVENT_SLOWSUSDOWN:
		iPercentEncumbranceOffended = CalculateCarriedWeight( pSoldier );
		break;

	case OPINIONEVENT_NOSHARINGFOOD:
		fCheckFood = (pSoldier->bFoodLevel < FoodMoraleMods[FOOD_MERC_START_SHOW_HUNGER_SYMBOL].bThreshold);
		fCheckDrink = (pSoldier->bDrinkLevel < FoodMoraleMods[FOOD_MERC_START_SHOW_HUNGER_SYMBOL].bThreshold);

		//  no hunger - no problem
		if ( !fCheckFood && !fCheckDrink )
			return;

		break;

	case OPINIONEVENT_RICHGUY:
		if ( !pSoldier->iTotalContractLength || (pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC && pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__MERC && pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT) )
			return;

		// determine our mean daily wage
		meanwage = gMercProfiles[pSoldier->ubProfile].uiTotalCostToDate / pSoldier->iTotalContractLength;

		// hu?
		if ( !explevel )
			return;

		break;

	case OPINIONEVENT_BETTERGEAR:
		highestcoolness = HighestInventoryCoolness( pSoldier );
		break;

	default:
		// either unknown event, or event is handled elsewhere - exit
		return;
		break;
	}

	UINT8				usEventUsed;		// it is possible that the individaul event is switched
	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// we test several conditions before we allow adding an opinion
		// other merc must be active, have a profile, be someone else and not be in transit or dead
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			 !(pTeamSoldier->bAssignment == IN_TRANSIT || pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			// if fSameSector is TRUE then the teammate must be in the same sector
			if ( gDynamicOpinionEvent[usEvent].fSameSector && (pTeamSoldier->sSectorX != pSoldier->sSectorX || pTeamSoldier->sSectorY != pSoldier->sSectorY || pTeamSoldier->bSectorZ != pSoldier->bSectorZ) )
				continue;

			// if fSameAssignment is TRUE then both mercs must be on the same assignment
			if ( gDynamicOpinionEvent[usEvent].fSameAssignment && pTeamSoldier->bAssignment != pSoldier->bAssignment )
				continue;

			// if fAwake is TRUE then only awake mercs count
			if ( gDynamicOpinionEvent[usEvent].fAwake && pTeamSoldier->flags.fMercAsleep )
				continue;

			// if fSquadsOnly is TRUE then only mercs in combat squads count
			if ( gDynamicOpinionEvent[usEvent].fSquadsOnly && pTeamSoldier->bAssignment >= ON_DUTY )
				continue;

			usEventUsed = usEvent;

			// event-specific checks
			switch ( usEventUsed )
			{
			case OPINIONEVENT_CONTRACTEXTENSION:
				break;

			case OPINIONEVENT_ORDEREDRETREAT:
				break;

			case OPINIONEVENT_CIVKILLER:
				// only continue if this is a good guy
				if ( !(gMercProfiles[pTeamSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) )
					continue;
				break;

			case OPINIONEVENT_SLOWSUSDOWN:
			{
				INT32 iPercentEncumbrance = CalculateCarriedWeight( pTeamSoldier );

				if ( iPercentEncumbrance > 100 || iPercentEncumbrance > 0.7 * iPercentEncumbranceOffended )
					continue;
			}
				break;

			case OPINIONEVENT_NOSHARINGFOOD:
				// do not add if we don't have food
				if ( !HasFoodInInventory( pTeamSoldier, fCheckFood, fCheckDrink ) )
					continue;
				break;

			case OPINIONEVENT_ANNOYINGDISABILITY:
				// do not add if we have the same disability
				if ( gMercProfiles[pTeamSoldier->ubProfile].bDisability == gMercProfiles[pSoldier->ubProfile].bDisability )
					continue;
				break;

			case OPINIONEVENT_ADDICT:
				// do not add if we are an addict too
				if ( pTeamSoldier->HasBackgroundFlag( BACKGROUND_DRUGUSE ) )
					continue;
				break;

			case OPINIONEVENT_THIEF:
				// do not add if we are a thief too
				if ( pTeamSoldier->HasBackgroundFlag( BACKGROUND_SCROUNGING ) )
					continue;
				break;

			case OPINIONEVENT_WORSTCOMMANDEREVER:
				break;

			case OPINIONEVENT_RICHGUY:
			{
				if ( !pTeamSoldier->iTotalContractLength || (pTeamSoldier->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC && pTeamSoldier->ubWhatKindOfMercAmI != MERC_TYPE__MERC && pTeamSoldier->ubWhatKindOfMercAmI != MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT) )
					continue;

				// A gets offended of B if
				// mean wage(B) >= ( WAGE_ACCEPTANCE_FACTOR  * level(B) / level(A) ) * mean wage(A)
				// which equals
				// mean wage(B) / level(B) >= WAGE_ACCEPTANCE_FACTOR * mean wage(A) / level(A)

				// their wage
				UINT32 theirmeanwage = gMercProfiles[pTeamSoldier->ubProfile].uiTotalCostToDate / pTeamSoldier->iTotalContractLength;

				// adjust this for experience levels
				FLOAT explevelfactor = gGameExternalOptions.fDynamicWageFactor * pTeamSoldier->stats.bExpLevel / explevel;

				// abort if their mean wage isn't that high
				if ( theirmeanwage < explevelfactor * meanwage )
					continue;
			}
				break;

			case OPINIONEVENT_BETTERGEAR:
				// do not add if they don't have cooler stuff
				if ( HighestInventoryCoolness( pTeamSoldier ) < highestcoolness + 2 )
					continue;
				break;

			case OPINIONEVENT_DRINKBUDDIES_GOOD:
				break;

			case OPINIONEVENT_DRINKBUDDIES_BAD:
				break;

			case OPINIONEVENT_DISEASE_DISGUSTING:
				// not if the other guy is a doctor, is currently used as a doctor, or has disease themself
				if ( IS_DOCTOR(pTeamSoldier->bAssignment) || pTeamSoldier->bAssignment == DISEASE_DOCTOR_SECTOR || HAS_SKILL_TRAIT( pTeamSoldier, DOCTOR_NT ) || pTeamSoldier->HasDisease(TRUE, FALSE, FALSE) )
					continue;
				break;

			case OPINIONEVENT_DISEASE_TREATMENT:
				break;

			case OPINIONEVENT_BRUTAL_GOOD:
			case OPINIONEVENT_BRUTAL_BAD:
				// if we are a good guy or a pacifist, then we dislike excessive violence
				if ( gMercProfiles[pTeamSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY ||
					 DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_PACIFIST ) )
					usEventUsed = OPINIONEVENT_BRUTAL_BAD;
				// if we are malicious, agressive or a psycho, then we like it
				else if ( DoesMercHaveDisability( pTeamSoldier, PSYCHO ) ||
						  DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_AGGRESSIVE ) ||
						  DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_MALICIOUS ) )
					usEventUsed = OPINIONEVENT_BRUTAL_GOOD;
				else
					continue;
				break;

			case OPINIONEVENT_TEACHER:
			case OPINIONEVENT_BESTCOMMANDEREVER:
				break;
				
			case OPINIONEVENT_FRAGTHIEF:
				// we care if we were going for the same target at the same location
				if ( pTeamSoldier->ubTargetID == pSoldier->ubTargetID && pTeamSoldier->sTargetGridNo == pSoldier->sTargetGridNo )
				{
					// are we ambitious enough to care about kill counts?
					if ( DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_ASSERTIVE ) || DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_SHOWOFF ) )
					{
						// did we have enough AP to claim we were about to finish them?
						if ( !EnoughPoints( pTeamSoldier, 60, 0, FALSE ) )
							continue;
					}
					else
					{
						// we are grateful for the assistance instead
						usEventUsed = OPINIONEVENT_BATTLE_ASSIST;
					}
				}
				else
					continue;
				break;

			case OPINIONEVENT_BATTLE_TOOK_PRISONER:
				// we only care if we prefer the enemy to be taken alive
				if ( !(gMercProfiles[pTeamSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY ||
					DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_INTELLECTUAL ) ||
					DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_PACIFIST ) ) )
					continue;
				break;

			case OPINIONEVENT_CIV_ATTACKER:
				// psychos or malicious characters dont care
				if ( DoesMercHavePersonality( pTeamSoldier, CHAR_TRAIT_MALICIOUS ) ||
					 DoesMercHaveDisability( pTeamSoldier, PSYCHO ) )
					continue;
				break;

			default:
				// either unknown event, or event is handled elsewhere - exit
				return;
				break;
			}

			if ( fOffender )
				AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, usEventUsed, fStartDialogue );
			else
				AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, usEventUsed, fStartDialogue );
		}
	}
}
