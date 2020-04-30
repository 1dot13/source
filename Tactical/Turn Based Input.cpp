#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#include "Language Defines.h"
#include "HelpScreen.h"
#include "Prebattle Interface.h"
#include "ambient control.h"
#include "DisplayCover.h"
#include "_Ja25Englishtext.h"
#else
#include "builddefines.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "debug.h"
#include "math.h"
#include "jascreens.h"
#include "pathai.h"
#include "Animation Control.h"
#include "Animation Data.h"
#include "Event Pump.h"
#include "Timer Control.h"
#include "Handle UI.h"
#include "Isometric Utils.h"
#include "input.h"
#include "overhead.h"
#include "Sys Globals.h"
#include "screenids.h"
#include "interface.h"
#include "cursor control.h"
#include "points.h"
#include "Interactive Tiles.h"
#include "interface cursors.h"
#include "weapons.h"
#include "lighting.h"
#include "sound control.h"
#include "environment.h"
#include "Music Control.h"
#include "ai.h"
#include "renderworld.h"
#include "font control.h"
#include "worldman.h"
#include "Handle UI Plan.h"
#include "message.h"
#ifdef NETWORKED
#include "Networking.h"
#include "Communication.h"
#endif
#include "overhead map.h"
#include "world items.h"
#include "Game Clock.h"
#include "interface items.h"
#include "physics.h"
#include "ui cursors.h"
#include "worldman.h"
#include "strategicmap.h"
#include "soldier profile.h"
#include "soldier create.h"
#include "soldier add.h"
#include "dialogue control.h"
#include "interface dialogue.h"
#include "opplist.h"
#include "interactive tiles.h"
#include "messageboxscreen.h"
#include "gameloop.h"

#include "spread burst.h"
#include "explosion control.h"
#include "QuestDebug.h"
#include "assignments.h"
#include "EditScreen.h"
#include "SaveLoadGame.h"
#include "los.h"
#include "Exit Grids.h"
#include "Strategic Exit GUI.h"
#include "auto bandage.h"
#include	"Options Screen.h"
#include "squads.h"
#include "interface panels.h"
#include "soldier tile.h"
#include "Soldier functions.h"
#include "Game Events.h"
#include "english.h"
#include "Random.h"
#include "Assignments.h"
#include "Map Screen Interface.h"
#include "renderworld.h"
#include	"Quest Debug System.h"
//#include "medical.h"
#include "Arms Dealer Init.h"
#include "ShopKeeper Interface.h"
#include "GameSettings.h"
#include "vehicles.h"
#include "SaveLoadScreen.h"
#include "Air raid.h"
#include "meanwhile.h"
#include "text.h"
#include "Inventory Choosing.h"
#include "soldier macros.h"
#include "HelpScreen.h"
#include "cheats.h"
#include "displaycover.h"
#include "Tactical Save.h"
#include "history.h"
#include "Queen Command.h"
#include "PreBattle Interface.h"
#include "_Ja25EnglishText.h"
#include "Militia Control.h"
#include "Ambient Control.h"
#include "Strategic AI.h"
#include "VehicleMenu.h"
#endif

#include "Quest Debug System.h"
#include "connect.h"
#include "fresh_header.h"

#include "IMP Skill Trait.h"			// added by Flugente
#include "SkillMenu.h"					// added by Flugente
#include "Map Screen Interface Map Inventory.h"//dnl ch75 021113

#include "DisplayCover.h"				// added by Sevenfm
#include "InterfaceItemImages.h"		// added by Sevenfm
#include "DynamicDialogueWidget.h"		// added by Flugente for DelayBoxDestructionBy(...)
#include "Utilities.h"					// added by Flugente

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


extern UIKEYBOARD_HOOK					gUIKeyboardHook;
extern BOOLEAN	fRightButtonDown;
extern BOOLEAN	fLeftButtonDown;
extern BOOLEAN fIgnoreLeftUp;
extern UINT32	guiCurrentEvent;
extern UINT8	gubIntTileCheckFlags;
extern UINT32	guiCurrentUICursor;
extern SOLDIERTYPE *gpSMCurrentMerc;
extern INT32 gsOverItemsGridNo;
extern INT16 gsOverItemsLevel;
extern BOOLEAN	gfUIShowExitSouth;
extern BOOLEAN	fMiddleButtonDown;
extern BOOLEAN	fX1ButtonDown;
extern BOOLEAN	fX2ButtonDown;

extern INT32	giStartingMemValue;


extern BOOLEAN gfBeginBurstSpreadTracking;
extern BOOLEAN gfRTClickLeftHoldIntercepted;

extern BOOLEAN gfReportHitChances;

BOOLEAN gfFirstCycleMovementStarted = FALSE;

extern UINT32 guiVObjectSize;
extern UINT32 guiVSurfaceSize;

extern BOOLEAN gfNextShotKills;
UINT32 guiSoldierFlags;
UINT32 guiUITargetSoldierId = NOBODY;

// sevenfm: for cover dialog
extern COVER_DRAW_MODE gubDrawMode;

extern  MOUSE_REGION    gMPanelRegion;

void HandleTalkingMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent );
void HandleMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent );
void HandleItemMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent );
void HandleOpenDoorMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent );
void HandleSectorExitMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent );
void HandleStanceChangeFromUIKeys( UINT8 ubAnimHeight );

extern BOOLEAN ValidQuickExchangePosition( );


BOOLEAN HandleUIReloading( SOLDIERTYPE *pSoldier );


extern SOLDIERTYPE *FindNextActiveSquad( SOLDIERTYPE *pSoldier );
extern SOLDIERTYPE *FindPrevActiveSquad( SOLDIERTYPE *pSoldier );
extern void ToggleItemGlow( BOOLEAN fOn );
extern void HandleTalkingMenuBackspace( void );
extern void BeginKeyPanelFromKeyShortcut( );



extern INT32		iSMPanelButtons[ NUM_SM_BUTTONS ];
extern INT32		iTEAMPanelButtons[ NUM_TEAM_BUTTONS ];
extern INT32		giSMStealthButton;

SOLDIERTYPE *gpExchangeSoldier1;
SOLDIERTYPE *gpExchangeSoldier2;

// Flugente: fortification
INT16	gCurrentFortificationStructure = -1;
UINT8	gCurrentFortificationTileLibraryIndex = 0;

extern UINT32 guiMessageBoxImage;
extern UINT16 guiMessageBoxImageIndex;

BOOLEAN ConfirmActionCancel( INT32 usMapPos, INT32 usOldMapPos );

BOOLEAN	gfNextFireJam = FALSE;
INT16 brstmode = 0; //dddd
extern INT16 ITEMDESC_START_X;
extern INT16 ITEMDESC_START_Y;


BOOLEAN gfMouseLockedOnBorder = FALSE;
extern int iWindowedMode;

extern OBJECTTYPE *gpItemDescObject;
extern BOOLEAN gfInItemStackPopup;
extern BOOLEAN gfInKeyRingPopup;


//Little functions called by keyboard input
void SwapGoggles(SOLDIERTYPE *pTeamSoldier);
// HEADROCK HAM B2.8: Function to switch team's goggles uniformly
void SwapGogglesUniformly(SOLDIERTYPE *pTeamSoldier, BOOLEAN fToNightVision);
void SeperateItems();
void StackAndSort( BOOLEAN fRestrictToAmmo );
void CreateRandomItem();
void MakeSelectedSoldierTired();
void ToggleRealTime( UINT32 *puiNewEvent );
void ToggleViewAllMercs();
void ToggleViewAllItems();
void TestExplosion();
void CycleSelectedMercsItem();
void ToggleWireFrame();
void RefreshSoldier();
void ChangeSoldiersBodyType( UINT8 ubBodyType, BOOLEAN fCreateNewPalette );
void TeleportSelectedSoldier();
void ToggleTurnMode();// ary-05/05/2009 : add forced turn mode
void ToggleTreeTops();
void ToggleZBuffer();
void TogglePlanningMode();
void SetBurstMode();
void SetScopeMode( INT32 usMapPos );
void ObliterateSector();
void RandomizeMercProfile();
void CreateNextCivType();
void ToggleCliffDebug();
void CreateCow();
void CreateBloodCat();
void CreatePlayerControlledCow();
void ToggleRealTimeConfirm();
void TestMeanWhile( INT32 iID );
void CreatePlayerControlledMonster();
void ChangeCurrentSquad( INT32 iSquad );
void HandleSelectMercSlot( UINT8 ubPanelSlot, INT8 bCode );
void EscapeUILock( );
void TestCapture( );

#ifdef JA2BETAVERSION
void ToggleMapEdgepoints();
#endif
#ifdef JA2TESTVERSION
void ToggleMercsNeverQuit();
#endif
#ifdef JA2TESTVERSION
void DumpSectorDifficultyInfo( void );
#endif

void HandleStealthChangeFromUIKeys( );

extern BOOLEAN CompatibleAmmoForGun( OBJECTTYPE *pTryObject, OBJECTTYPE *pTestObject );
extern void DetermineWhichAssignmentMenusCanBeShown( void );
extern void DetermineWhichMilitiaControlMenusCanBeShown( void ); //lalien
// The_Bob - real time sneaking, 01-06-09
extern BOOLEAN WeSeeNoOne(void); // Needed to control entering turn-based with ctrl-x

void	QueryTBMButton( UINT32 *puiNewEvent );
void	QueryTBWheel( UINT32 *puiNewEvent );
void	QueryTBX1Button( UINT32 *puiNewEvent );
void	QueryTBX2Button( UINT32 *puiNewEvent );

void	ItemCreationCallBack( UINT8 ubResult );
void	CheatCreateItem();
// silversurfer: added for merc portrait swapping in tactical
void	SwapMercPortraits ( SOLDIERTYPE *pSoldier, INT8 bDirection );
extern	INT8 GetTeamSlotFromPlayerID( UINT8 ubID );
extern FACETYPE	*gpCurrentTalkingFace;

// Flugente:  toggle display of enemy role indicators
extern BOOLEAN gDisplayEnemyRoles;

// sevenfm: new mouse commands
void HandleAltMouseTBWheel( void );
void HandleAltMouseTBMButton( UINT32 *puiNewEvent );
void HandleAltMouseTBX1Button( UINT32 *puiNewEvent );
void HandleAltMouseTBX2Button( UINT32 *puiNewEvent );

// sevenfm: original mouse commands functionality
void HandleMouseTBWheel( void );
void HandleMouseTBMButton( UINT32 *puiNewEvent );
void HandleMouseTBX1Button( UINT32 *puiNewEvent );
void HandleMouseTBX2Button( UINT32 *puiNewEvent );

// sevenfm: common functionality
void HandleTBSelectAllMercs( void );
void HandleTBToggleStealthAll( void );
void HandleTBToggleFireMode( void );
void HandleTBJumpThroughWindow( void );
void HandleTBJump( void );
void HandleTBLook( UINT32 *puiNewEvent );
void HandleTBSelectAllMercs( void );
void HandleTBCycleThroughKnownEnemies( void );
void HandleTBCycleThroughKnownEnemiesBackward( void );
void HandleTBCycleThroughVisibleEnemies( void );
void HandleTBCycleThroughVisibleEnemiesBackward( void );
void HandleTBLocateSoldier( void );
void HandleTBChangeLevel( void );
void HandleTBSwapHands( void );
void HandleTBSwapGunsling( void );
void HandleTBSwapKnife( void );
void HandleTBSwapSidearm( void );
void HandleTBSwapGoogles( void );
void HandleTBToggleFormation( void );
void HandleTBEnterTurnbased( void );
void HandleTBToggleSneak( void );
void HandleTBToggleStealth( void );
void HandleTBReload( void );
void HandleTBReloadAll( void );
void HandleTBShowCover( void );
void HandleTBShowMines( void );
void HandleTBShowLOS( void );
void HandleTBToggleTrapNetworkView( void );
void HandleTBGotoHigherStance(void);
void HandleTBGotoLowerStance(void);
void HandleTBLocateNextMerc( void );
void HandleTBLocatePrevMerc( void );
void HandleTBDropBackpacks( void );
void HandleTBPickUpBackpacks( void );
void HandleTBSoldierRun( void );

void HandleTacticalReload(void);
void HandleTacticalTakeItem( void );
void HandleTacticalDropItem( UINT8 ubSlot );
void HandleTacticalTakeInvItem( INT32 iType );
void HandleTacticalStoreInvItem( void );
INT32 InvItemType( UINT16 usItem );
void HandleTacticalInventoryMenu( void );
void HandleTacticalMoveItems( void );
void TacticalInventoryMessageBoxCallBack( UINT8 ubExitValue );
void FortificationSettingCallBack( UINT8 ubExitValue );
void HandleTacticalCoverMenu( void );
void TacticalCoverMessageBoxCallBack( UINT8 ubExitValue );
void ActivateCheatsMessageBoxCallBack( UINT8 ubExitValue );
void HandleTacticalAmmoCrates( UINT8 magType );
void HandleTacticalTransformItem( void );
void HandleTacticalTransformFlashlight(void);
void HandleTacticalTransformLaser(void);
void HandleTacticalTransformScope(void);
BOOLEAN FindTransformation( UINT16 usItem, TransformInfoStruct **pTransformation );
BOOLEAN FindLaserTransformation(UINT16 usItem, TransformInfoStruct **pTransformation);
BOOLEAN FindScopeTransformation(UINT16 usItem, TransformInfoStruct **pTransformation);

void	GetTBMouseButtonInput( UINT32 *puiNewEvent )
{
	QueryTBLeftButton( puiNewEvent );
	QueryTBRightButton( puiNewEvent );
	QueryTBWheel( puiNewEvent );
	QueryTBMButton( puiNewEvent  );
	QueryTBX1Button( puiNewEvent  );
	QueryTBX2Button( puiNewEvent  );
}

void	QueryTBLeftButton( UINT32 *puiNewEvent )
{
	SOLDIERTYPE								*pSoldier;
	INT32 usMapPos;
	static BOOLEAN	fClickHoldIntercepted = FALSE;
	static BOOLEAN	fCanCheckForSpeechAdvance = FALSE;
	static INT32		sMoveClickGridNo					= 0;


	// LEFT MOUSE BUTTON
	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		if (!GetMouseMapPos( &usMapPos ) && !gfUIShowExitSouth )
		{
			return;
		}

		if ( gViewportRegion.ButtonState & MSYS_LEFT_BUTTON )
		{
			if ( !fLeftButtonDown )
			{
				fLeftButtonDown = TRUE;
				RESETCOUNTER( LMOUSECLICK_DELAY_COUNTER );

				{
					switch( gCurrentUIMode )
					{
					case CONFIRM_ACTION_MODE:

						if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
						{
							pSoldier->sStartGridNo = usMapPos;
						}
						break;

					case MOVE_MODE:

						if ( giUIMessageOverlay != -1 )
						{
							EndUIMessage( );
						}
						else
						{
							if ( !HandleCheckForExitArrowsInput( FALSE ) && gpItemPointer == NULL )
							{
								// First check if we clicked on a guy, if so, make selected if it's ours
								if ( gfUIFullTargetFound && ( guiUIFullTargetFlags & OWNED_MERC )	)
								{
									if ( !( guiUIFullTargetFlags & UNCONSCIOUS_MERC ) )
									{
										fClickHoldIntercepted = TRUE;

										// Select guy
										if(	GetSoldier( &pSoldier, gusUIFullTargetID ) && gpItemPointer == NULL )
										{
											if( pSoldier->bAssignment >= ON_DUTY )
											{
												// do nothing
												fClickHoldIntercepted = FALSE;
											}
											else
											{
												*puiNewEvent = I_SELECT_MERC;
											}
										}
										else
										{
											*puiNewEvent = I_SELECT_MERC;
										}
									}
								}
								else
								{
									if ( InUIPlanMode( ) )
									{
										AddUIPlan( usMapPos, UIPLAN_ACTION_MOVETO );
									}
									else
									{
										// We're on terrain in which we can walk, walk
										// If we're on terrain,
										if ( gusSelectedSoldier != NOBODY )
										{
											GetSoldier( &pSoldier, gusSelectedSoldier );

											INT8 bReturnVal = HandleMoveModeInteractiveClick( usMapPos, puiNewEvent );

											// All's OK for interactive tile?
											if ( bReturnVal == -2 )
											{
												// Confirm!
												if ( SelectedMercCanAffordMove(	)	)
												{
													*puiNewEvent = C_WAIT_FOR_CONFIRM;
												}
											}
											else if ( bReturnVal == 0 )
											{
												if ( gfUIAllMoveOn )
												{
													*puiNewEvent = C_WAIT_FOR_CONFIRM;
												}
												else
												{
													if ( gsCurrentActionPoints == 0 )
													{
														ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
													}
													else if ( SelectedMercCanAffordMove(	)	)
													{
														BOOLEAN fResult;

														if ( ( fResult = UIOKMoveDestination( MercPtrs[ gusSelectedSoldier ], usMapPos ) ) == 1 )
														{
															// ATE: CHECK IF WE CAN GET TO POSITION
															// Check if we are not in combat
															GetSoldier( &pSoldier, gusSelectedSoldier );

															if ( gsCurrentActionPoints == 0 )
															{
																ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
															}
															else
															{
																*puiNewEvent = C_WAIT_FOR_CONFIRM;
															}
														}
														else
														{
															if ( fResult == 2 )
															{
																ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NOBODY_USING_REMOTE_STR ] );
															}
															else
															{
																//if ( sMapPos != sMoveClickGridNo || pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
																//{
																//	sMoveClickGridNo					= sMapPos;

																//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANT_MOVE_THERE_STR ] );
																// Goto hand cursor mode....
																//	*puiNewEvent					= M_CHANGE_TO_HANDMODE;
																//	gsOverItemsGridNo				= sMapPos;
																//	gsOverItemsLevel				= gsInterfaceLevel;
																//}
																//else
																//{
																//	sMoveClickGridNo = 0;
																//	*puiNewEvent = M_CHANGE_TO_HANDMODE;
																//}

															}
															//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Invalid move destination." );
														}
													}
												}
											}
											// OK, our first right-click is an all-cycle
											gfUICanBeginAllMoveCycle = FALSE;
										}
										fClickHoldIntercepted = TRUE;

									}
								}
							}
							else
							{
								fClickHoldIntercepted = TRUE;
								fIgnoreLeftUp = TRUE;
							}

						}
						break;
					}
				}
				if ( gfUIWaitingForUserSpeechAdvance )
				{
					fCanCheckForSpeechAdvance = TRUE;
				}

			}

			// HERE FOR CLICK-DRAG CLICK
			switch( gCurrentUIMode )
			{

			case MOVE_MODE:

				// First check if we clicked on a guy, if so, make selected if it's ours
				if ( gfUIFullTargetFound )
				{
					// Select guy
					if ( ( guiUIFullTargetFlags & SELECTED_MERC) && !( guiUIFullTargetFlags & UNCONSCIOUS_MERC ) && !( MercPtrs[ gusUIFullTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
					{
						*puiNewEvent = M_CHANGE_TO_ADJPOS_MODE;
						fIgnoreLeftUp = FALSE;
					}
				}
				break;
			}

			// IF HERE, DO A CLICK-HOLD IF IN INTERVAL
			if ( COUNTERDONE( LMOUSECLICK_DELAY_COUNTER ) && !fClickHoldIntercepted )
			{
				/* removed warning C4060 (jonathanl)
				// Switch on UI mode
				switch( gCurrentUIMode )
				{

				}
				*/
			}


		}
		else
		{
			if ( fLeftButtonDown	)
			{
				if ( !fIgnoreLeftUp )
				{

					// FIRST CHECK FOR ANYTIME ( NON-INTERVAL ) CLICKS
					switch( gCurrentUIMode )
					{
					case ADJUST_STANCE_MODE:

						// If button has come up, change to mocve mode
						*puiNewEvent = PADJ_ADJUST_STANCE;
						break;
					}

					// CHECK IF WE CLICKED-HELD
					if ( COUNTERDONE( LMOUSECLICK_DELAY_COUNTER ) )
					{
						// LEFT CLICK-HOLD EVENT
						// Switch on UI mode
						switch( gCurrentUIMode )
						{
						case CONFIRM_ACTION_MODE:

							if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
							{
								if ( pSoldier->bDoBurst )
								{
									pSoldier->sEndGridNo = usMapPos;

									gfBeginBurstSpreadTracking = FALSE;

									if ( pSoldier->sEndGridNo != pSoldier->sStartGridNo )
									{
										pSoldier->flags.fDoSpread = TRUE;

										PickBurstLocations( pSoldier );

										*puiNewEvent = CA_MERC_SHOOT;
									}
									else
									{
										pSoldier->flags.fDoSpread = FALSE;
									}

									fClickHoldIntercepted = TRUE;

								}
							}
							break;

						}

					}

					{
						//LEFT CLICK NORMAL EVENT
						// Switch on UI mode
						if ( !fClickHoldIntercepted )
						{
							if ( giUIMessageOverlay != -1 )
							{
								EndUIMessage( );
							}
							else
							{
								if ( !HandleCheckForExitArrowsInput( TRUE ) )
								{
									if ( gpItemPointer != NULL )
									{
										if ( HandleItemPointerClick( usMapPos ) )
										{
											// getout of mode
											EndItemPointer( );

											*puiNewEvent = A_CHANGE_TO_MOVE;
										}
									}
									else
									{
										// Check for wiating for keyboard advance
										if ( gfUIWaitingForUserSpeechAdvance && fCanCheckForSpeechAdvance)
										{
											// We have a key, advance!
											DialogueAdvanceSpeech( );
										}
										else
										{
											switch( gCurrentUIMode )
											{
											case MENU_MODE:

												// If we get a hit here and we're in menu mode, quit the menu mode
												EndMenuEvent( guiCurrentEvent );
												break;

											case IDLE_MODE:

												// First check if we clicked on a guy, if so, make selected if it's ours
												if ( gfUIFullTargetFound )
												{
													// Select guy
													if ( guiUIFullTargetFlags & OWNED_MERC && !( guiUIFullTargetFlags & UNCONSCIOUS_MERC ) )
													{
														*puiNewEvent = I_SELECT_MERC;
													}

												}
												break;

											case MOVE_MODE:

												// Check if we should activate an interactive tile!
												// Select guy
												if ( ( guiUIFullTargetFlags & OWNED_MERC ) && !( guiUIFullTargetFlags & UNCONSCIOUS_MERC ) )
												{
													// Select guy
													if(	GetSoldier( &pSoldier, gusUIFullTargetID ) && ( gpItemPointer == NULL ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
													{
														if( pSoldier->bAssignment >= ON_DUTY )
														{
															PopupAssignmentMenuInTactical( pSoldier );
														}
													}
												}
												break;

											case CONFIRM_MOVE_MODE:

												*puiNewEvent = C_MOVE_MERC;
												break;

											case HANDCURSOR_MODE:

												HandleHandCursorClick( usMapPos, puiNewEvent );
												break;

											case JUMPOVER_MODE:

												if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
												{
													if ( EnoughPoints( pSoldier, gsCurrentActionPoints, 0, TRUE ) )
													{
														*puiNewEvent = JP_JUMP;
													}
												}
												break;

											case ACTION_MODE:

												if ( InUIPlanMode( ) )
												{
													AddUIPlan( usMapPos, UIPLAN_ACTION_FIRE );
												}
												else
												{
													if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
													{
														if ( !HandleUIReloading( pSoldier ) )
														{
															// ATE: Reset refine aim..
															pSoldier->aiData.bShownAimTime = 0;

															if ( gsCurrentActionPoints == 0 )
															{
																ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
															}
															// Determine if we have enough action points!
															else if ( UIMouseOnValidAttackLocation( pSoldier ) && SelectedMercCanAffordAttack( ) )
															{
																*puiNewEvent = A_CHANGE_TO_CONFIM_ACTION;
																pSoldier->sStartGridNo = usMapPos;
															}
														}

													}
												}
												break;

											case CONFIRM_ACTION_MODE:

												*puiNewEvent = CA_MERC_SHOOT;
												break;

											case LOOKCURSOR_MODE:
												// If we cannot actually do anything, return to movement mode
												*puiNewEvent = LC_LOOK;
												break;

											case TALKCURSOR_MODE:

												if ( HandleTalkInit( ) )
												{
													*puiNewEvent = TA_TALKINGMENU;
												}
												break;

											case GETTINGITEM_MODE:

												// Remove menu!
												// RemoveItemPickupMenu( );
												break;

											case TALKINGMENU_MODE:

												//HandleTalkingMenuEscape( TRUE );
												break;

											case EXITSECTORMENU_MODE:

												RemoveSectorExitMenu( FALSE );
												break;

											case OPENDOOR_MENU_MODE:

												CancelOpenDoorMenu( );
												HandleOpenDoorMenu( );
												*puiNewEvent = A_CHANGE_TO_MOVE;
												break;

											}
										}
									}
								}
							}
						}
					}
				}

				// Reset flag
				fLeftButtonDown = FALSE;
				fIgnoreLeftUp	= FALSE;
				fClickHoldIntercepted = FALSE;
				fCanCheckForSpeechAdvance = FALSE;
				gfFirstCycleMovementStarted = FALSE;

				// Reset counter
				RESETCOUNTER( LMOUSECLICK_DELAY_COUNTER );

			}

		}

	}
	else
	{
		// Set mouse down to false
		//fLeftButtonDown = FALSE;

		// OK, handle special cases like if we are dragging and holding for a burst spread and
		// release mouse over another mouse region
		if ( gfBeginBurstSpreadTracking )
		{
			if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
			{
				pSoldier->flags.fDoSpread = FALSE;
			}
			gfBeginBurstSpreadTracking = FALSE;
		}
	}

}

void	QueryTBRightButton( UINT32 *puiNewEvent )
{
	static BOOLEAN	fClickHoldIntercepted = FALSE;
	static BOOLEAN	fClickIntercepted = FALSE;
	SOLDIERTYPE		*pSoldier;
	INT32 usMapPos;
	BOOLEAN				fDone = FALSE;
	if (!GetMouseMapPos( &usMapPos ) )
	{
		return;
	}

	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{

		// RIGHT MOUSE BUTTON
		if ( gViewportRegion.ButtonState & MSYS_RIGHT_BUTTON )
		{
			if ( !fRightButtonDown )
			{
				fRightButtonDown = TRUE;
				RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );
			}

			// CHECK COMBINATIONS
			if ( fLeftButtonDown )
			{
			}
			else
			{
				if ( gpItemPointer == NULL )
				{
					// IF HERE, DO A CLICK-HOLD IF IN INTERVAL
					if ( COUNTERDONE( RMOUSECLICK_DELAY_COUNTER ) && !fClickHoldIntercepted )
					{
						// Switch on UI mode
						switch( gCurrentUIMode )
						{
						case IDLE_MODE:
						case ACTION_MODE:
						case HANDCURSOR_MODE:
						case LOOKCURSOR_MODE:
						case TALKCURSOR_MODE:
						case MOVE_MODE:

							// Check if we're on terrain
							//if ( !gfUIFullTargetFound )
							//{
							// ATE:
							fDone = FALSE;

							if ( ( guiUIFullTargetFlags & OWNED_MERC ) && !( guiUIFullTargetFlags & UNCONSCIOUS_MERC ) )
							{
								// Select guy
								if(	GetSoldier( &pSoldier, gusUIFullTargetID ) && ( gpItemPointer == NULL ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
								{
									//if( pSoldier->bAssignment >= ON_DUTY )
									{
										PopupAssignmentMenuInTactical( pSoldier );
										fClickHoldIntercepted = TRUE;
									}
								}
							}

							if( fDone == TRUE )
							{
								break;
							}

							if ( gusSelectedSoldier != NOBODY && !fClickHoldIntercepted )
							{
								*puiNewEvent = U_MOVEMENT_MENU;
								fClickHoldIntercepted = TRUE;
							}
							//}
							//else
							//{
							// If we are on a selected guy
							//	if ( guiUIFullTargetFlags & SELECTED_MERC && !( guiUIFullTargetFlags & UNCONSCIOUS_MERC ) )
							//	{
							//*puiNewEvent = U_POSITION_MENU;
							//fClickHoldIntercepted = TRUE;
							//	}
							//		else if ( guiUIFullTargetFlags & OWNED_MERC )
							//		{
							// If we are on a non-selected guy selected guy
							//		}

							//}
							break;
						}
					}
				}

			}

		}
		else
		{
			if ( fRightButtonDown )
			{

				if ( fLeftButtonDown )
				{
					fIgnoreLeftUp = TRUE;

					if ( gpItemPointer == NULL )
					{
						// ATE:
						if ( gusSelectedSoldier != NOBODY )
						{
							switch( gCurrentUIMode )
							{
							case CONFIRM_MOVE_MODE:
							case MOVE_MODE:

								if ( gfUICanBeginAllMoveCycle )
								{
									*puiNewEvent = M_CYCLE_MOVE_ALL;
								}
								else
								{
									if ( !gfFirstCycleMovementStarted )
									{
										gfFirstCycleMovementStarted = TRUE;

										// OK, set this guy's movement mode to crawling fo rthat we will start cycling in run.....
										if ( MercPtrs[ gusSelectedSoldier ]->usUIMovementMode != RUNNING )
										{
											// ATE: UNLESS WE ARE IN RUNNING MODE ALREADY
											MercPtrs[ gusSelectedSoldier ]->usUIMovementMode = CRAWLING;
										}
									}

									// Give event to cycle movement
									*puiNewEvent = M_CYCLE_MOVEMENT;
									break;
								}
							}

							// ATE: Added cancel of burst mode....
							if ( gfBeginBurstSpreadTracking )
							{
								gfBeginBurstSpreadTracking = FALSE;
								gfRTClickLeftHoldIntercepted = TRUE;
								MercPtrs[ gusSelectedSoldier ]->flags.fDoSpread				= FALSE;
								fClickHoldIntercepted = TRUE;
								*puiNewEvent = A_END_ACTION;
								gCurrentUIMode = MOVE_MODE;
							}
						}
					}
				}
				else
				{
					if ( !fClickHoldIntercepted && !fClickIntercepted )
					{
						if ( gpItemPointer == NULL )
					{
						// ATE:
						if ( gusSelectedSoldier != NOBODY )
						{
							// sevenfm: ALT+RMB - TraitsMenu
							if(_KeyDown(ALT))
							{
								switch( gCurrentUIMode )
								{
								case MOVE_MODE:
								case TALKCURSOR_MODE:
								case ACTION_MODE:
								case CONFIRM_ACTION_MODE:
								case LOOKCURSOR_MODE:
									TraitsMenu(usMapPos);
									fClickIntercepted = TRUE;
									break;
								}
							}
							// anv: CTRL+RMB - VehicleMenu
							else if(_KeyDown(CTRL))
							{
								switch( gCurrentUIMode )
								{										
								case HANDCURSOR_MODE:
									HandleHandCursorRightClick( usMapPos, puiNewEvent );
									break;
								}
							}
							else
							{
								// Switch on UI mode
								switch( gCurrentUIMode )
								{
								case IDLE_MODE:

									break;

								case MOVE_MODE:
									// anv: don't switch if passengers are blocked from attacking
									pSoldier = MercPtrs[ gusSelectedSoldier ];
									if( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
									{
										SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );
										INT8 bSeatIndex = GetSeatIndexFromSoldier( pSoldier );
										if( gNewVehicle[ pVehicleList[ pSoldier->iVehicleId ].ubVehicleType ].VehicleSeats[bSeatIndex].fBlockedShots == TRUE )
										{
											ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pVehicleSeatsStrings[0] );
											break;
										}
									}
									// We have here a change to action mode
									*puiNewEvent = M_CHANGE_TO_ACTION;
									fClickIntercepted = TRUE;
									break;

								case ACTION_MODE:

									// We have here a change to action mode
									*puiNewEvent = A_CHANGE_TO_MOVE;
									fClickIntercepted = TRUE;
									break;

								case CONFIRM_MOVE_MODE:

									*puiNewEvent = A_CHANGE_TO_MOVE;
									fClickIntercepted = TRUE;
									break;

								case HANDCURSOR_MODE:
									// If we cannot actually do anything, return to movement mode
									*puiNewEvent = A_CHANGE_TO_MOVE;
									break;

								case LOOKCURSOR_MODE:
								case TALKCURSOR_MODE:

									// If we cannot actually do anything, return to movement mode
									*puiNewEvent = A_CHANGE_TO_MOVE;
									break;

								case CONFIRM_ACTION_MODE:

									if ( GetSoldier( &pSoldier, gusSelectedSoldier ) )
									{
										HandleRightClickAdjustCursor( pSoldier, usMapPos );
									}
									fClickIntercepted = TRUE;
									break;

								case MENU_MODE:

									// If we get a hit here and we're in menu mode, quit the menu mode
									EndMenuEvent( guiCurrentEvent );
									fClickIntercepted = TRUE;
									break;

								}
							}
							}
					}
					else
					{
						if ( gfUIFullTargetFound )
							{
								gfItemPointerDifferentThanDefault = !gfItemPointerDifferentThanDefault;
							}
					}
					}
				}
			}
			// Reset flag
			fRightButtonDown = FALSE;
			fClickHoldIntercepted = FALSE;
			fClickIntercepted = FALSE;

			// Reset counter
			RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );

		}
	}

}


extern BOOLEAN	gUIActionModeChangeDueToMouseOver;

void GetTBMousePositionInput( UINT32 *puiNewEvent )
{
	INT32 usMapPos;
	static INT32			usOldMapPos = 0;
	SOLDIERTYPE		*pSoldier;
	BOOLEAN						bHandleCode;
	static BOOLEAN		fOnValidGuy = FALSE;
	static UINT32			uiMoveTargetSoldierId = NOBODY;


	if (!GetMouseMapPos( &usMapPos ) )
	{
		return;
	}

	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		// Handle highlighting stuff
		HandleObjectHighlighting( );

		// Check if we have an item in our hands...
		if ( gpItemPointer != NULL )
		{
			*puiNewEvent = A_ON_TERRAIN;
			return;
		}

		// Switch on modes
		switch( gCurrentUIMode )
		{
		case LOCKUI_MODE:
			*puiNewEvent = LU_ON_TERRAIN;
			break;

		case LOCKOURTURN_UI_MODE:
			*puiNewEvent = LA_ON_TERRAIN;
			break;

		case IDLE_MODE:
			*puiNewEvent = I_ON_TERRAIN;
			break;

		case ENEMYS_TURN_MODE:
			*puiNewEvent = ET_ON_TERRAIN;
			break;

		case LOOKCURSOR_MODE:
			*puiNewEvent = LC_ON_TERRAIN;
			break;

		case TALKCURSOR_MODE:
			if ( uiMoveTargetSoldierId != NOBODY )
			{
				if( gfUIFullTargetFound )
				{
					if ( gusUIFullTargetID != uiMoveTargetSoldierId )
					{
						*puiNewEvent = A_CHANGE_TO_MOVE;
						return;
					}
				}
				else
				{
					*puiNewEvent = A_CHANGE_TO_MOVE;
					return;
				}
			}
			*puiNewEvent = T_ON_TERRAIN;
			break;

		case MOVE_MODE:

			uiMoveTargetSoldierId = NOBODY;

			// Check for being on terrain
			if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
			{
				if ( IsValidJumpLocation( pSoldier, usMapPos, TRUE ) )
				{
					gsJumpOverGridNo = usMapPos;
					*puiNewEvent = JP_ON_TERRAIN;
					return;
				}
				else
				{
					if( gfUIFullTargetFound )
					{
						// ATE: Don't do this automatically for enemies......
						if ( MercPtrs[ gusUIFullTargetID ]->bTeam != ENEMY_TEAM )
						{
							uiMoveTargetSoldierId = gusUIFullTargetID;
							if ( IsValidTalkableNPC( (UINT8)gusUIFullTargetID, FALSE, FALSE, FALSE ) && !_KeyDown( SHIFT ) && !AM_AN_EPC( pSoldier ) && !ValidQuickExchangePosition( ) )
							{
								*puiNewEvent = T_CHANGE_TO_TALKING;
								return;
							}
						}
					}
				}
			}
			*puiNewEvent = M_ON_TERRAIN;
			break;

		case ACTION_MODE:

			// First check if we are on a guy, if so, make selected if it's ours
			// Check if the guy is visible
			guiUITargetSoldierId = NOBODY;

			fOnValidGuy = FALSE;

			if ( gfUIFullTargetFound	)
				//if ( gfUIFullTargetFound )
			{
				if ( IsValidTargetMerc( (UINT8)gusUIFullTargetID ) )
				{
					guiUITargetSoldierId = gusUIFullTargetID;

					if ( MercPtrs[ gusUIFullTargetID ]->bTeam != gbPlayerNum )
					{
						fOnValidGuy = TRUE;
						//ddd turn off flag that indicates aim/autofire mode adjustment (aimed burst/auto)
						brstmode = 0;
					}
					else
					{
						if ( gUIActionModeChangeDueToMouseOver )
						{
							*puiNewEvent = A_CHANGE_TO_MOVE;
							return;
						}
					}
				}
			}
			else
			{
				if ( gUIActionModeChangeDueToMouseOver )
				{
					*puiNewEvent = A_CHANGE_TO_MOVE;
					return;
				}
			}
			*puiNewEvent = A_ON_TERRAIN;
			break;

		case GETTINGITEM_MODE:

			break;

		case TALKINGMENU_MODE:

			if ( HandleTalkingMenu( ) )
			{
				*puiNewEvent = A_CHANGE_TO_MOVE;
			}
			break;

		case EXITSECTORMENU_MODE:

			if ( HandleSectorExitMenu( ) )
			{
				*puiNewEvent = A_CHANGE_TO_MOVE;
			}
			break;

		case OPENDOOR_MENU_MODE:
			bHandleCode = HandleOpenDoorMenu( ); // I think its nicer to avoid assignments in conditionals (jonathanl)
			if ( bHandleCode )
			{
				// OK, IF we are not canceling, set ui back!
				if ( bHandleCode == 2 )
				{
					*puiNewEvent = A_CHANGE_TO_MOVE;
				}
				else
				{
				}
			}
			break;

		case JUMPOVER_MODE:

			// ATE: Make sure!
			if ( gsJumpOverGridNo != usMapPos )
			{
				*puiNewEvent = A_CHANGE_TO_MOVE;
			}
			else
			{
				*puiNewEvent = JP_ON_TERRAIN;
			}
			break;

		case CONFIRM_MOVE_MODE:

			if ( usMapPos != usOldMapPos )
			{
				// Switch event out of confirm mode
				*puiNewEvent = A_CHANGE_TO_MOVE;

				// Set off ALL move....
				gfUIAllMoveOn = FALSE;

				// ERASE PATH
				ErasePath( TRUE );
			}
			break;

		case CONFIRM_ACTION_MODE:

			// DONOT CANCEL IF BURST
			if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
			{
				if ( pSoldier->bDoBurst )
				{
					pSoldier->sEndGridNo = usMapPos;

					if ( pSoldier->sEndGridNo != pSoldier->sStartGridNo && fLeftButtonDown )
					{
						pSoldier->flags.fDoSpread = TRUE;
						gfBeginBurstSpreadTracking = TRUE;
					}

					if ( pSoldier->flags.fDoSpread )
					{
						// Accumulate gridno
						AccumulateBurstLocation( usMapPos );

						*puiNewEvent = CA_ON_TERRAIN;
						break;
					}
				}
			}

			// First check if we are on a guy, if so, make selected if it's ours
			if ( gfUIFullTargetFound )
			{
				if ( guiUITargetSoldierId != gusUIFullTargetID )
				{
					// Switch event out of confirm mode
					*puiNewEvent = CA_END_CONFIRM_ACTION;
				}
				else
				{
					*puiNewEvent = CA_ON_TERRAIN;
				}
			}
			else
			{
				// OK, if we were on a guy, and now we are off, go back!
				if ( fOnValidGuy )
				{
					// Switch event out of confirm mode
					*puiNewEvent = CA_END_CONFIRM_ACTION;
				}
				else
				{
					if ( ConfirmActionCancel( usMapPos, usOldMapPos ) )
					{
						// Switch event out of confirm mode
						*puiNewEvent = CA_END_CONFIRM_ACTION;
					}
					else
					{
						*puiNewEvent = CA_ON_TERRAIN;
					}
				}
			}
			break;

		case HANDCURSOR_MODE:

			*puiNewEvent = HC_ON_TERRAIN;
			break;

		}

		usOldMapPos = usMapPos;

	}
}


void GetPolledKeyboardInput( UINT32 *puiNewEvent )
{
	static BOOLEAN	fShifted = FALSE;
	static BOOLEAN	fShifted2 = FALSE;
	static BOOLEAN	fCtrlDown = FALSE;
	static BOOLEAN	fAltDown = FALSE;
	static BOOLEAN	fDeleteDown=FALSE;
	static BOOLEAN	fEndDown=FALSE;

	// CHECK FOR POLLED KEYS!!
	// CHECK FOR CTRL
	switch( gCurrentUIMode )
	{
	case DONT_CHANGEMODE:
	case CONFIRM_MOVE_MODE:
	case CONFIRM_ACTION_MODE:
	case LOOKCURSOR_MODE:
	case TALKCURSOR_MODE:
	case IDLE_MODE:
	case MOVE_MODE:
	case ACTION_MODE:
	case HANDCURSOR_MODE:

		if ( _KeyDown( CTRL )	)
		{
			if ( fCtrlDown == FALSE )
			{
				ErasePath( TRUE );
				gfPlotNewMovement = TRUE;
			}
			fCtrlDown = TRUE;
			*puiNewEvent = HC_ON_TERRAIN;

		}
		if ( !(_KeyDown( CTRL ) ) && fCtrlDown )
		{
			fCtrlDown = FALSE;
			*puiNewEvent = M_ON_TERRAIN;
			gfPlotNewMovement = TRUE;
		}
		break;
	}

	// CHECK FOR ALT
	switch( gCurrentUIMode )
	{
	case MOVE_MODE:

		if ( _KeyDown( ALT )	)
		{
			if ( fAltDown == FALSE )
			{
				// Get currently selected guy and change reverse....
				if ( gusSelectedSoldier != NOBODY )
				{
					gUIUseReverse = TRUE;
					ErasePath( TRUE );
					gfPlotNewMovement = TRUE;
				}
			}
			fAltDown = TRUE;

		}
		if ( !(_KeyDown( ALT ) ) && fAltDown )
		{
			if ( gusSelectedSoldier != NOBODY )
			{
				gUIUseReverse = FALSE;
				ErasePath( TRUE );
				gfPlotNewMovement = TRUE;
			}

			fAltDown = FALSE;
		}
		break;
	}

	// Check realtime input!
	if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
	{
		//if ( _KeyDown( CAPS )	) //&& !fShifted )
		//{
		//	fShifted = TRUE;
		//	if ( gCurrentUIMode != ACTION_MODE && gCurrentUIMode != CONFIRM_ACTION_MODE )
		//	{
		//		*puiNewEvent = CA_ON_TERRAIN;
		//	}
		//}
		//if ( !(_KeyDown( CAPS ) ) && fShifted )
		//{
		//	fShifted = FALSE;
		//	{
		//		*puiNewEvent = M_ON_TERRAIN;
		//	}
		//}


		if ( _KeyDown( SHIFT )	) //&& !fShifted )
		{
			fShifted2 = TRUE;
			if ( gCurrentUIMode != MOVE_MODE && gCurrentUIMode != CONFIRM_MOVE_MODE )
			{
				//puiNewEvent = M_ON_TERRAIN;
			}
		}
		if ( !(_KeyDown( SHIFT ) ) && fShifted2 )
		{
			fShifted2 = FALSE;
			if ( gCurrentUIMode != ACTION_MODE && gCurrentUIMode != CONFIRM_ACTION_MODE )
			{
				//	*puiNewEvent = A_ON_TERRAIN;
			}
		}

	}

	if( _KeyDown( DEL ) && !fDeleteDown)
	{

		if ( _KeyDown( ALT )	)
			SwitchToHostileTrapsView();	// added by Flugente
		else
			SwitchToEnemyView();

		fDeleteDown = TRUE;
	}

	if( !_KeyDown( DEL ) && fDeleteDown )
	{

		if ( _KeyDown( ALT )	)
			SwitchMineViewOff();	// added by Flugente
		else
			SwitchViewOff();
		
		fDeleteDown = FALSE;
	}

	if( _KeyDown( END ) && !fEndDown)
	{

		if ( _KeyDown( ALT )	)
			SwitchToTrapNetworkView();	// added by Flugente
		else
			SwitchToMercView();

		fEndDown = TRUE;
	}

	if( !_KeyDown( END ) && fEndDown )
	{
		if ( _KeyDown( ALT )	)
			SwitchMineViewOff();	// added by Flugente
		else
			SwitchViewOff();
		

		fEndDown = FALSE;
	}
}

UINT16 usitem = 1623;

void CheatCreateItem( )
{
	if ( gusSelectedSoldier != NOBODY )
	{
		INT16 status = 100;
		FLOAT temperature = 0.0;

		if ( Item[usitem].foodtype )
			temperature = OVERHEATING_MAX_TEMPERATURE;

		// Flugente: spawn items while debugging
		OBJECTTYPE newobj;
		CreateItem( usitem, status, &newobj );
		(newobj)[0]->data.bTemperature = temperature;
					
		if ( !AutoPlaceObject( MercPtrs[ gusSelectedSoldier ], &newobj, FALSE ) )
			AddItemToPool( MercPtrs[ gusSelectedSoldier ]->sGridNo, &newobj, 1, 0, 0, -1 );
	}
}

void ItemCreationCallBack( UINT8 ubResult )
{
	if (ubResult == MSG_BOX_RETURN_OK && wcscmp(gszMsgBoxInputString,L"") > 0)
	{
		usitem = _wtoi( gszMsgBoxInputString );
		CheatCreateItem( );

	}
	memset(gszMsgBoxInputString,0,sizeof(gszMsgBoxInputString));
}

extern	BOOLEAN		gfDisableRegionActive;
extern	BOOLEAN		gfUserTurnRegionActive;

void GetKeyboardInput( UINT32 *puiNewEvent )
{
	InputAtom					InputEvent;
	BOOLEAN						fKeyTaken = FALSE;
	POINT	MousePos;
	//SOLDIERTYPE				*pSoldier;
	static BOOLEAN	fShifted = FALSE;
	static BOOLEAN	fShifted2 = FALSE;
	static BOOLEAN	fAltDown = FALSE;
	INT32 usMapPos;
	
	CHAR16	zString[128]; 

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	GetMouseMapPos( &usMapPos );

	while (DequeueEvent(&InputEvent) == TRUE)
	{
		// HOOK INTO MOUSE HOOKS
		switch(InputEvent.usEvent)
		{
		case LEFT_BUTTON_DOWN:
			MouseSystemHook(LEFT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
			break;
		case LEFT_BUTTON_UP:
			MouseSystemHook(LEFT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y ,_LeftButtonDown, _RightButtonDown);
			break;
		case RIGHT_BUTTON_DOWN:
			MouseSystemHook(RIGHT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
			break;
		case RIGHT_BUTTON_UP:
			MouseSystemHook(RIGHT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
			break;

		}

		// handle for fast help text for interface stuff
		if( IsTheInterfaceFastHelpTextActive() )
		{
			if( InputEvent.usEvent == KEY_UP )
			{
				ShutDownUserDefineHelpTextRegions( );
			}

			continue;
		}

		// Check for waiting for keyboard advance
		if ( gfUIWaitingForUserSpeechAdvance && InputEvent.usEvent == KEY_UP )
		{
			// We have a key, advance!
			DialogueAdvanceSpeech( );

			// Ignore anything else
			continue;
		}

		// ATE: if game paused because fo player, unpasue with any key
		if ( gfPauseDueToPlayerGamePause && InputEvent.usEvent == KEY_UP )
		{
			HandlePlayerPauseUnPauseOfGame( );

			continue;
		}

		if ( (InputEvent.usEvent == KEY_DOWN ) )
		{
			if ( giUIMessageOverlay != -1 )
			{
				EndUIMessage( );
				continue;
			}

			// End auto bandage if we want....
			if ( gTacticalStatus.fAutoBandageMode )
			{
				AutoBandage( FALSE );
				*puiNewEvent = LU_ENDUILOCK;
			}
		}

		if ( gUIKeyboardHook != NULL )
		{
			fKeyTaken = gUIKeyboardHook( &InputEvent );
		}
		if ( fKeyTaken )
		{
			continue;
		}

		/*
		if( (InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == ) )
		{
		HandlePlayerPauseUnPauseOfGame( );
		}
		*/

		if ((InputEvent.usEvent == KEY_UP )&& ( InputEvent.usParam == PAUSE ) && !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV) )
		{
			// Pause game!
			HandlePlayerPauseUnPauseOfGame( );
		}

		// FIRST DO KEYS THAT ARE USED EVERYWHERE!
		if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == 'x') && ( InputEvent.usKeyState & ALT_DOWN ))
		{
			HandleShortCutExitState( );
			//*puiNewEvent = I_EXIT;
		}
#ifdef JA2UB
//Ja25 No meanwhiles
#else
		if ((InputEvent.usEvent == KEY_UP )&& ( InputEvent.usParam == ESC) )
		{
			if ( AreInMeanwhile() && gCurrentMeanwhileDef.ubMeanwhileID != INTERROGATION )
			{
				DeleteTalkingMenu();
				EndMeanwhile();
			}

			// Flugente: speed up dynamic dialogue boxes
			// Note that this will behave oddly on the morning of January 1st 1970 :-)
			DelayBoxDestructionBy( -gGameExternalOptions.sDynamicDialogueTimeOffset );
		}
#endif

		// BEGIN: by Lejardo for mouse-locking in game screen
		// WANNE: If the game runs in windowed mode, the user can "lock" / "unlock" the mouse cursor
		if ( iWindowedMode && !gfInKeyRingPopup && !gfInItemStackPopup)
		{
			if ((InputEvent.usEvent == KEY_DOWN ) && InputEvent.usParam == 'z' )
			{			
				if( (InputEvent.usKeyState & CTRL_DOWN) && !(InputEvent.usKeyState & ALT_DOWN))
				{
					// Unlock the mouse cursor
					if (gfMouseLockedOnBorder)
					{
						FreeMouseCursor( FALSE );

						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_WINDOWED_MODE_RELEASE_MOUSE] );
					}
					// Lock the mouse cursor
					else
					{
						SGPRect			LJDRect;

						LJDRect.iLeft 	= 0;
						LJDRect.iTop 	= 0;
						LJDRect.iRight 	= SCREEN_WIDTH;
						LJDRect.iBottom = SCREEN_HEIGHT;
						RestrictMouseCursor( &LJDRect );

						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_WINDOWED_MODE_LOCK_MOUSE] );
					}

					gfMouseLockedOnBorder = !gfMouseLockedOnBorder;
				}
			}
		}
		// END: by Lejardo for mouse-locking in game screen

		// WANNE: Just disabled for now, because we don't need it yet.
		/*						
		// WANNE: If the game hangs on enemy turn, just press ALT + CTRL + E and the game will continue with enemy turn!!
		if ( (InputEvent.usEvent == KEY_DOWN )&& (InputEvent.usKeyState & CTRL_DOWN) && ( InputEvent.usParam == 'e') )
		{
			if( InputEvent.usKeyState & ALT_DOWN )
			{
				if (gTacticalStatus.ubCurrentTeam != 0)
				{
					if( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
					{
						EndTurn( 1 );
					}
				}
			}		
		}
		*/
			
		if (!is_networked)
		{
			// Allow to load everywhere
			if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == 'l') )
			{
				if( InputEvent.usKeyState & ALT_DOWN )
				{
					if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
					{
						gTacticalStatus.ubDisablePlayerInterrupts = FALSE;

						gfSaveGame = FALSE;
						gfCameDirectlyFromGame = TRUE;

						guiPreviousOptionScreen = GAME_SCREEN;
						
						// cancel, the player can move when it is not its turn!
						//Heinz: 28.02.09 BUGFIX: player doesn't need to see save/load screen
						//LeaveTacticalScreen( SAVE_LOAD_SCREEN );
						DoQuickLoad();
					}
				}
				else if( InputEvent.usKeyState & CTRL_DOWN )
				{
					// WANNE: Do not allow saving via the save screen when it is not our turn,
					// because there is an exploit when you close the save window without saving, you can move your merc even it is not your turn
					// IF UI HAS LOCKED, ONLY ALLOW EXIT!
					if ( gfDisableRegionActive || gfUserTurnRegionActive )
					{
						continue;
					}

					if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
					{
						gfSaveGame = FALSE;
						gfCameDirectlyFromGame = TRUE;

						guiPreviousOptionScreen = GAME_SCREEN;
						LeaveTacticalScreen( SAVE_LOAD_SCREEN );
					}
				}
			}						
		}

		if (is_networked)
		{
			// WANNE: Disabled the quick saving in a multiplayer game, because if you quit out of the save dialog,
			// you can move your merc even if it is not your turn.
			/*
			if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == 's') )//allow saving 'always'//hayden
			{
				if( InputEvent.usKeyState & ALT_DOWN )
				{
					if( !fDisableMapInterfaceDueToBattle && !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
					{
						//if the game CAN be saved
						if( CanGameBeSaved() )
						{
							guiPreviousOptionScreen = GAME_SCREEN;
							//guiPreviousOptionScreen = guiCurrentScreen;
							DoQuickSave();
						}
						else
						{
							//Display a message saying the player cant save now
							DoMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__IRON_MAN_CANT_SAVE_NOW ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
						}
					}
				}
				else if( InputEvent.usKeyState & CTRL_DOWN )
				{
					if( !fDisableMapInterfaceDueToBattle && !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
					{
						//if the game CAN be saved
						if( CanGameBeSaved() )
						{
							gfSaveGame = TRUE;
							gfCameDirectlyFromGame = TRUE;

							guiPreviousOptionScreen = GAME_SCREEN;
							LeaveTacticalScreen( SAVE_LOAD_SCREEN );
						}
						else
						{
							//Display a message saying the player cant save now
							DoMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__IRON_MAN_CANT_SAVE_NOW ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL);
						}
					}
				}
			}
			*/
		
//#ifdef DISABLE_MP_INTERRUPTS_IN_COOP

			// WANNE: Not needed to show the override turn dialog
//#else

			if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == 'e') )
			{
				if( InputEvent.usKeyState & ALT_DOWN )
				{
					if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
					{
						overide_turn();
					}
				}			
			}

//#endif

			if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == 'k') )
			{
				if( InputEvent.usKeyState & ALT_DOWN )
				{
					if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
					{
						kick_player();
					}
				}
			}

			// OJW - 090209 - ingame chat
			if (InputEvent.usEvent == KEY_UP && InputEvent.usParam == 'y')
			{
				if( !(InputEvent.usKeyState & ALT_DOWN) &&  !(InputEvent.usKeyState & CTRL_DOWN))
				{
					OpenChatMsgBox();
					continue;
				}
			}
		} // end is_networked


		// Break of out IN CONV...
		if ( CHEATER_CHEAT_LEVEL( ) )
		{
			if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == ENTER ) && ( InputEvent.usKeyState & ALT_DOWN ) )
			{
				if ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )
				{
					gTacticalStatus.uiFlags &= (~ENGAGED_IN_CONV);
					giNPCReferenceCount = 0;
				}
			}
		}

		if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			{
				if ( gTacticalStatus.ubCurrentTeam != gbPlayerNum )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == ENTER ) && ( InputEvent.usKeyState & ALT_DOWN ) )
						{
							// ESCAPE ENEMY'S TURN
							EndAIDeadlock();

							// Decrease global busy	counter...
							gTacticalStatus.ubAttackBusyCount = 0;
#ifdef DEBUG_ATTACKBUSY
							OutputDebugString( "Resetting attack busy due to keyboard interrupt.\n");
#endif
							guiPendingOverrideEvent = LU_ENDUILOCK;
							UIHandleLUIEndLock( NULL );

						}
					}
					//Madd: removed from cheat mode
					if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == ENTER ) && ( InputEvent.usKeyState & CTRL_DOWN ) )
					{
						EscapeUILock( );
					}
				}
				else
				{
					if ( CHEATER_CHEAT_LEVEL( ) && (InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == ENTER ) && ( InputEvent.usKeyState & CTRL_DOWN ) )
					{
						//UNLOCK UI
						EscapeUILock( );
					}
					else if ( (InputEvent.usEvent == KEY_DOWN ) && InputEvent.usParam == ENTER	)
					{
						// Cycle through enemys
						CycleThroughKnownEnemies( );
					}
				}
			}
		}

		if ( gfInTalkPanel )
		{
			HandleTalkingMenuKeys( &InputEvent, puiNewEvent );
		}

		// Do some checks based on what mode we are in
		switch( gCurrentUIMode )
		{
		case EXITSECTORMENU_MODE:

			HandleSectorExitMenuKeys( &InputEvent, puiNewEvent );
			continue;

		case GETTINGITEM_MODE:

			HandleItemMenuKeys( &InputEvent, puiNewEvent );
			continue;

		case MENU_MODE:

			HandleMenuKeys( &InputEvent, puiNewEvent );
			continue;

		case OPENDOOR_MENU_MODE:

			HandleOpenDoorMenuKeys( &InputEvent, puiNewEvent );
			continue;
		}


		// CHECK ESC KEYS HERE....
		if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == ESC ))
		{
			//EscapeUILock( );
#ifdef JA2TESTVERSION
			if ( InAirRaid( ) )
			{
				EndAirRaid( );
			}
#endif

			// Cancel out of spread burst...
			gfBeginBurstSpreadTracking = FALSE;
			gfRTClickLeftHoldIntercepted = TRUE;
			if ( gusSelectedSoldier != NOBODY )
			{
				MercPtrs[ gusSelectedSoldier ]->flags.fDoSpread				= FALSE;
			}


			// Befone anything, delete popup box!
			EndUIMessage( );

			// CANCEL FROM PLANNING MODE!
			if ( InUIPlanMode( ) )
			{
				EndUIPlan( );
			}

			if ( InItemDescriptionBox( ) )
			{
				DeleteItemDescriptionBox( );
			}
			else if( InKeyRingPopup( ) )
			{
				DeleteKeyRingPopup( );
			}

			if ( gCurrentUIMode == MENU_MODE )
			{
				// If we get a hit here and we're in menu mode, quit the menu mode
				EndMenuEvent( guiCurrentEvent );
			}

			if ( gCurrentUIMode == HANDCURSOR_MODE )
			{
				*puiNewEvent = A_CHANGE_TO_MOVE;
			}

			if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
			{
				if ( gusSelectedSoldier != NOBODY )
				{
					// If soldier is not stationary, stop
					MercPtrs[ gusSelectedSoldier ]->StopSoldier(	);
					*puiNewEvent = A_CHANGE_TO_MOVE;
				}
				// ATE: OK, stop any mercs who are moving by selection method....
				StopRubberBandedMercFromMoving( );
			}

		}

		// CHECK ESC KEYS HERE....
		if ((InputEvent.usEvent == KEY_DOWN ) && ( InputEvent.usParam == BACKSPACE ) )
		{
			StopAnyCurrentlyTalkingSpeech( );
		}


		// IF UI HAS LOCKED, ONLY ALLOW EXIT!
		if ( gfDisableRegionActive || gfUserTurnRegionActive )
		{
			continue;
		}

		// Check all those we want if enemy's turn
		if ((InputEvent.usEvent == KEY_UP )&& ( InputEvent.usParam == 'q') )
		{
			if ( InputEvent.usKeyState & ALT_DOWN )
			{
				if ( CHEATER_CHEAT_LEVEL( ) )
				{
					static BOOLEAN fShowRoofs = TRUE;
					INT32 x;
					UINT16 usType;

					// Toggle removal of roofs...
					fShowRoofs = !fShowRoofs;

					for ( x = 0; x < WORLD_MAX; x++ )
					{
						for ( usType = FIRSTROOF; usType <= LASTSLANTROOF; usType++ )
						{
							HideStructOfGivenType( x, usType, (BOOLEAN)(!fShowRoofs) );
						}
					}
					InvalidateWorldRedundency();
					SetRenderFlags( RENDER_FLAG_FULL );
				}

			}
			else if ( InputEvent.usKeyState & CTRL_DOWN )
			{
#ifdef JA2BETAVERSION
				if ( CHEATER_CHEAT_LEVEL( ) )
				{
					LeaveTacticalScreen( ANIEDIT_SCREEN );
				}
#endif
			}
			else
			{
				if ( INFORMATION_CHEAT_LEVEL( ) )
				{
					*puiNewEvent = I_SOLDIERDEBUG;
				}
				else
				{
					//if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
					//	{
					//		if ( gusSelectedSoldier != NOBODY )
					//		{
					//			if ( gTacticalStatus.uiFlags & TURNBASED && !MercPtrs[ gusSelectedSoldier ]->fDontChargeReadyAPs )
					//			{
					//				INT16 apCost = GetAPsToReadyWeapon( MercPtrs[ gusSelectedSoldier ], 0 );
					//				if (MercPtrs[ gusSelectedSoldier ]->bActionPoints >= apCost )
					//				{
					//					DeductPoints( MercPtrs[ gusSelectedSoldier ],apCost , 0 );
					//					DirtyMercPanelInterface( MercPtrs[ gusSelectedSoldier ], DIRTYLEVEL2 );
					//					SoldierReadyWeapon( MercPtrs[ gusSelectedSoldier ] );
					//				}
					//			}
					//			else
					//			{
					//				SoldierReadyWeapon( MercPtrs[ gusSelectedSoldier ] );
					//			}

					//		}
					//	}
				}
			}
		}

#ifdef JA2TESTVERSION
		if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == '0') && ( InputEvent.usKeyState & ALT_DOWN ))
		{
			INT32 i = 0;
			INT32 sGridNo;
			INT32 iTime = GetJA2Clock( );
			UINT8	ubLevel;

			for ( i = 0; i < 1000; i++ )
			{
				CalculateLaunchItemChanceToGetThrough( MercPtrs[ gusSelectedSoldier ], &(MercPtrs[ gusSelectedSoldier ]->inv[ HANDPOS ] ), usMapPos, 0, 0, &sGridNo, TRUE, (INT8 *)&ubLevel, TRUE );
			}

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Physics 100 times: %d", ( GetJA2Clock( ) - iTime )	);

		}
#endif


#ifdef NETWORKED
		// DEF: Test Networking
		if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == '0') && ( InputEvent.usKeyState & ALT_DOWN ))
		{
			DisplayMultiPlayerInfo();
		}
		if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == '9') && ( InputEvent.usKeyState & ALT_DOWN ))
		{
			DisplayDirectPlayInfo();
		}
		if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == '8') && ( InputEvent.usKeyState & ALT_DOWN ))
		{
			DisplayDirectPlayPlayerInfo();
		}

		if ((InputEvent.usEvent == KEY_DOWN )&& ( InputEvent.usParam == '7') && ( InputEvent.usKeyState & ALT_DOWN ))
		{
			SetDisplayFlag();
		}
#endif

		if( InputEvent.usEvent == KEY_DOWN )
		{
			BOOLEAN fAlt, fCtrl, fShift;
			fAlt = InputEvent.usKeyState & ALT_DOWN ? TRUE : FALSE;
			fCtrl = InputEvent.usKeyState & CTRL_DOWN ? TRUE : FALSE;
			fShift = InputEvent.usKeyState & SHIFT_DOWN ? TRUE : FALSE;
			switch( InputEvent.usParam )
			{
			case SPACE:

				// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
				if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )	&&
					( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ NEXTMERC_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
				{
					if ( !InKeyRingPopup( ) )
					{
						
						if (_KeyDown(CTRL))
						{							
							checkLBEArrayIntegrity();
						}
						else if (_KeyDown(ALT))
						{
							checkLBEArrayIntegrity(true);
						}
						else if (_KeyDown(SHIFT))
						{
							SOLDIERTYPE *pNewSoldier;
							INT32		iCurrentSquad;

							if ( gusSelectedSoldier != NOBODY )
							{
								// only allow if nothing in hand and if in SM panel, the Change Squad button must be enabled
								if ( ( ( gsCurInterfacePanel != TEAM_PANEL ) || ( ButtonList[ iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
								{
									//Select next squad
									iCurrentSquad = CurrentSquad( );

									pNewSoldier = FindNextActiveSquad( MercPtrs[ gusSelectedSoldier ] );

									if ( pNewSoldier->bAssignment != iCurrentSquad )
									{
										HandleLocateSelectMerc( pNewSoldier->ubID, LOCATEANDSELECT_MERC );

										if ( gGameExternalOptions.fUseXMLSquadNames && pNewSoldier->bAssignment < ON_DUTY )
											ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE_STRING ], SquadNames[pNewSoldier->bAssignment].squadname );
										else
											//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE ], ( CurrentSquad( ) + 1 ) );
											ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE ], ( pNewSoldier->bAssignment + 1 ) );

										// Center to guy....
										//LocateSoldier( gusSelectedSoldier, SETLOCATOR );
									}
								}
							}
						}
						else
						{
							if ( gusSelectedSoldier != NOBODY )
							{ //Select next merc
								UINT8 bID;

								bID = FindNextMercInTeamPanel( MercPtrs[ gusSelectedSoldier ], FALSE, FALSE );

								HandleLocateSelectMerc( bID, LOCATEANDSELECT_MERC );

								// Center to guy....
								LocateSoldier( gusSelectedSoldier, SETLOCATOR );
							}
						}

						*puiNewEvent = M_ON_TERRAIN;
					}
				}
				break;

			case LEFTARROW:
				// used for map scrolling
				if ( fCtrl )
				{
					if ( gusSelectedSoldier != NOBODY )
						SwapMercPortraits( MercPtrs[ gusSelectedSoldier ], -1 );
				}
				break;

			case RIGHTARROW:
				// used for map scrolling
				if ( fCtrl )
				{
					if ( gusSelectedSoldier != NOBODY )
						SwapMercPortraits( MercPtrs[ gusSelectedSoldier ], 1 );
				}
				break;

			case INSERT:
				GoIntoOverheadMap();
				break;

			case DEL:
				// used for LOS display
				break;

			case HOME:
				if ( gGameSettings.fOptions[ TOPTION_3D_CURSOR ] )
				{
					gGameSettings.fOptions[ TOPTION_3D_CURSOR ] = FALSE;

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_3DCURSOR_OFF ] );
				}
				else
				{
					gGameSettings.fOptions[ TOPTION_3D_CURSOR ] = TRUE;

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_3DCURSOR_ON ] );
				}
				break;

			case END:
				// Lalien: commented out, to fix "end move & LOS bug" (same button has two different actions)
				//		if ( gusSelectedSoldier != NOBODY )
				//		{
				//			if ( CheckForMercContMove( MercPtrs[ gusSelectedSoldier ] ) )
				//			{
				//				// Continue
				//				ContinueMercMovement( MercPtrs[ gusSelectedSoldier ] );
				//				ErasePath( TRUE );
				//			}
				//		}
				break;

			case PGUP:
				if ( CHEATER_CHEAT_LEVEL( ) )
				{
					if( fCtrl )
						AttemptToChangeFloorLevel( -1 ); //try to go up towards ground level
				}

				if ( guiCurrentScreen != DEBUG_SCREEN )
				{
					if ( gusSelectedSoldier != NOBODY )
					{
						// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
						if ( ( gpItemPointer == NULL ) )
						{
							GotoHeigherStance( MercPtrs[ gusSelectedSoldier ] );
						}
					}
				}
				break;

			case PGDN:
				if ( CHEATER_CHEAT_LEVEL( ) )
				{
					if( fCtrl )
						AttemptToChangeFloorLevel( +1 ); //try to enter a lower underground level
				}

				if ( guiCurrentScreen != DEBUG_SCREEN )
				{
					if ( gusSelectedSoldier != NOBODY )
					{
						// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
						if ( ( gpItemPointer == NULL ) )
						{
							GotoLowerStance( MercPtrs[ gusSelectedSoldier ] );
						}
					}
				}
				break;

			case BACKSPACE:
				// used for stopping speech
				break;

			case TAB:
				// go to next tab in enhanced description box
				if( ( fCtrl ) && InItemDescriptionBox( ) && gGameSettings.fOptions[ TOPTION_ENHANCED_DESC_BOX ] == TRUE )
				{			
					if ( (Item[ gpItemDescObject->usItem ].usItemClass & IC_WEAPON || Item[ gpItemDescObject->usItem ].usItemClass & IC_PUNCH) 
						&& gubDescGenPage == 0 && gubDescBoxPage == 1 )
					{
						gubDescGenPage = 1;
					}
					else if ( gubDescBoxPage < 2 )
					{
						gubDescGenPage = 0;
						gubDescBoxPage++;
					}
					else
					{					
						gubDescBoxPage = 0;
					}
					HandleItemDescTabButton( );
				}
				// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
				else if ( ( gpItemPointer == NULL ) &&
					( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
				{
					UIHandleChangeLevel( NULL );

					if ( gsCurInterfacePanel == SM_PANEL )
					{
						// Remember soldier's new value
						gpSMCurrentMerc->bUIInterfaceLevel = (INT8)gsInterfaceLevel;
					}
				}
				break;

			case F1:
				if( fShift )
				{
					HandleSelectMercSlot( 0, LOCATE_MERC_ONCE );
				}
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 15 );
				}
				else if( fCtrl )
				{
					TestMeanWhile( 10 );
				}
#endif
				else
					HandleSelectMercSlot( 0, LOCATEANDSELECT_MERC );
				break;

			case F2:
				if( fShift )
					HandleSelectMercSlot( 1, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 1 );
				}
				else if( fCtrl )
				{
					TestMeanWhile( 11 );
				}
#endif
				else
					HandleSelectMercSlot( 1, LOCATEANDSELECT_MERC );
				break;

			case F3:
				if( fShift )
					HandleSelectMercSlot( 2, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 2 );
				}
				else if( fCtrl )
				{
					TestMeanWhile( 12 );
				}
#endif
				else
					HandleSelectMercSlot( 2, LOCATEANDSELECT_MERC );
				break;

			case F4:
				if( fShift )
					HandleSelectMercSlot( 3, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 3 );
				}
				else if( fCtrl )
				{
					TestMeanWhile( 13 );
				}
#endif
				else
					HandleSelectMercSlot( 3, LOCATEANDSELECT_MERC );
				break;

			case F5:
				if( fShift )
					HandleSelectMercSlot( 4, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 4 );
				}
				else if( fCtrl )
				{
					TestMeanWhile( 14 );
				}
#endif
				else
					HandleSelectMercSlot( 4, LOCATEANDSELECT_MERC );
				break;

			case F6:
				if( fShift )
					HandleSelectMercSlot( 5, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 5 );
				}
				else if( fCtrl )
				{
					TestMeanWhile( 15 );
				}
#endif
				else
					HandleSelectMercSlot( 5, LOCATEANDSELECT_MERC );
				break;

			case F7:
				if( fShift )
					HandleSelectMercSlot( 6, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 16 );
				}
#endif
				else
					HandleSelectMercSlot( 6, LOCATEANDSELECT_MERC );
				break;

			case F8:
				if( fShift )
					HandleSelectMercSlot( 7, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 7 );
				}
#endif
				else
					HandleSelectMercSlot( 7, LOCATEANDSELECT_MERC );
				break;

			case F9:
				if( fShift )
					HandleSelectMercSlot( 8, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fCtrl )
				{
					TestMeanWhile( 8 );
				}
#endif
				else
				{
#ifdef JA2EDITOR
					*puiNewEvent = I_ENTER_EDIT_MODE;
					gfMercResetUponEditorEntry = !fAlt;
					break;
#endif
					HandleSelectMercSlot( 8, LOCATEANDSELECT_MERC );
				}
				break;

			case F10:
				if( fShift )
					HandleSelectMercSlot( 9, LOCATE_MERC_ONCE );
#ifdef JA2TESTVERSION
				else if( fAlt )
				{
					TestMeanWhile( 9 );
				}
#endif
				else
					HandleSelectMercSlot( 9, LOCATEANDSELECT_MERC );
				break;

			case F11:
				if( fAlt )
				{
#ifdef JA2TESTVERSION
					SOLDIERTYPE *pSoldier;

					// Get selected soldier
					if( GetSoldier( &pSoldier, gusSelectedSoldier ) )
					{
						if( pSoldier->ubID == 46 )
						{
							// Change guy to drunk larry
							ForceSoldierProfileID( pSoldier, 47 );
						}
						else
						{
							// Change guy to normal larry
							ForceSoldierProfileID( pSoldier, 46 );
						}

						// Dirty interface
						DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
					}
#endif
				}

				else
				{
					if( DEBUG_CHEAT_LEVEL( ) )
					{
						GetMouseMapPos( &gsQdsEnteringGridNo );
						LeaveTacticalScreen( QUEST_DEBUG_SCREEN );
					}
				}
				break;

			case F12:
#ifdef JA2TESTVERSION
				if( fAlt )
				{
					UINT8 ubProfile = TONY;

					GetMouseMapPos( &gsQdsEnteringGridNo );
					AddShopkeeperToGridNo( ubProfile, gsQdsEnteringGridNo );
					EnterShopKeeperInterfaceScreen( ubProfile );
				}
#endif
				//clear tactical of messages
				if( fCtrl )
				{
					ClearTacticalMessageQueue( );
				}
				else if ( !fAlt )
				{
					ClearDisplayedListOfTacticalStrings( );
				}
				break;

			case '-':
			case '_':
				//if the display cover or line of sight is being displayed
				if( _KeyDown( END ) || _KeyDown( DEL ) )
				{
					//if( _KeyDown( DEL ) )
						//ChangeSizeOfDisplayCover( gGameSettings.ubSizeOfDisplayCover - 1 );

					//if( _KeyDown( END ) )
						//ChangeSizeOfLOS( gGameSettings.ubSizeOfLOS - 1 );
				}
				else
				{
					if( fAlt )
					{
						if(MusicGetVolume() >= 20)
							MusicSetVolume(MusicGetVolume()-20);
						else
							MusicSetVolume(0);
					}
					else if( fCtrl )
					{
#ifdef JA2TESTVERSION
						gTacticalStatus.bRealtimeSpeed = max( 1, gTacticalStatus.bRealtimeSpeed - 1 );
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Decreasing Realtime speed to %d", gTacticalStatus.bRealtimeSpeed );
#endif
					}
					else
					{
#ifdef JA2TESTVERSION
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Using Normal Scroll Speed"	);
						gubCurScrollSpeedID = 1;
#endif
					}
				}
				break;

			case '+':
#ifdef JA2TESTVERSION
				if( fAlt )
				{
					if(MusicGetVolume() <= 107)
						MusicSetVolume(MusicGetVolume()+20);
					else
						MusicSetVolume(127);
				}
				else if( fCtrl )
				{
					gTacticalStatus.bRealtimeSpeed = min( MAX_REALTIME_SPEED_VAL, gTacticalStatus.bRealtimeSpeed+1 );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Increasing Realtime speed to %d", gTacticalStatus.bRealtimeSpeed );
				}
				else
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Using Higher Scroll Speed"	);
					gubCurScrollSpeedID = 2;
				}
#endif
				break;

			case '=':
				//if the display cover or line of sight is being displayed
				if( _KeyDown( END ) || _KeyDown( DEL ) )
				{
					//f( _KeyDown( DEL ) )
						//ChangeSizeOfDisplayCover( gGameSettings.ubSizeOfDisplayCover + 1 );

					//if( _KeyDown( END ) )
						//ChangeSizeOfLOS( gGameSettings.ubSizeOfLOS + 1 );
				}
				else
				{

#ifdef JA2TESTVERSION
					if( fAlt )
					{
						WarpGameTime( 60, TRUE );
						break;
					}
#endif

					// ATE: This key will select everybody in the sector
					// Buggler: Disabled check for all merc display cover in turnbased mode
					//if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
					//{
						HandleTBSelectAllMercs();
					//}
				}
				break;

			case ',':
				// Flugente: alternate for people without mousewheel to aim auto-only weapons
				if ( gGameExternalOptions.bAimedBurstEnabled && gCurrentUIMode == CONFIRM_ACTION_MODE )
				{
					SOLDIERTYPE *		pSoldier;

					if ( GetSoldier( &pSoldier, gusSelectedSoldier ) )
					{
						HandleRightClickAdjustCursor( pSoldier, usMapPos );
					}
				}
				break;

			// added by Flugente
			case '.':
				if ( fCtrl && fAlt ) //jikuja: Create item by reusing old entered number
 				{
					if ( CHEATER_CHEAT_LEVEL( ) ) 
					{
						CheatCreateItem( );
					}
				}
				else if ( fCtrl )
				{
					if ( gusSelectedSoldier != NOBODY )
					{
						HandleTacticalFunctionSelection(MercPtrs[ gusSelectedSoldier ],  usMapPos );
					}
				}
				else if ( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
 					{
						if ( gusSelectedSoldier != NOBODY )
						{
							DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, L"Enter ItemID", GAME_SCREEN, MSG_BOX_FLAG_INPUTBOX, ItemCreationCallBack, NULL );
						}
					}
				}
				else
				{
					SetScopeMode( usMapPos );
				}

				break;

			case '/':
				// Center to guy....
				if ( gusSelectedSoldier != NOBODY )
				{
					LocateSoldier( gusSelectedSoldier, 10 );
				}
				break;

#if 0//dnl ch75 021113
			case '\"':
				Testing(1);
				break;
			case '\'':
				Testing(2);
				break;
#endif

			case '`':

				if( fAlt )
				{
					HandleTacticalStoreInvItem( );
				}
				else
				// Switch panels...
				{
					ToggleTacticalPanels();
				}
				break;

			case '1':
				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						//ChangeSoldiersBodyType( TANK_NW, TRUE );
						// MercPtrs[ gusSelectedSoldier ]->flags.uiStatusFlags |= SOLDIER_CREATURE;
						//EVENT_InitNewSoldierAnim( MercPtrs[ gusSelectedSoldier ], CRIPPLE_BEG, 0 , TRUE );
					}
					else
					{
						HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem1 );
					}
				}
				else
					ChangeCurrentSquad( 0 );
				break;

			case '2':
				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						ChangeSoldiersBodyType( INFANT_MONSTER, TRUE );
					}
					else
					{
						HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem2 );
					}
				}
				else if( fCtrl ) //toggle between the different npc debug modes
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						ToggleQuestDebugModes( QD_NPC_MSG );
					}
				}
				else
					ChangeCurrentSquad( 1 );
				break;

			case '3':
				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						MercPtrs[ gusSelectedSoldier ]->EVENT_InitNewSoldierAnim( KID_SKIPPING, 0 , TRUE );

						//ChangeSoldiersBodyType( LARVAE_MONSTER, TRUE );
						//MercPtrs[ gusSelectedSoldier ]->usAttackingWeapon = TANK_CANNON;
						//LocateSoldier( gusSelectedSoldier, FALSE );
						//EVENT_FireSoldierWeapon( MercPtrs[ gusSelectedSoldier ], sMapPos );
					}
					else
					{
						HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem3 );
					}
				}
				else if( fCtrl ) //toggle between the different npc debug modes
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						CreateBloodCat();
					}
				}
				else
					ChangeCurrentSquad( 2 );

				break;

			case '4':
				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						ChangeSoldiersBodyType( CRIPPLECIV, TRUE );
					}
					else
					{
						HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem4 );
					}
				}
				else
					ChangeCurrentSquad( 3 );

				//ChangeSoldiersBodyType( BLOODCAT, FALSE );
				break;

			case '5':
				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						ChangeSoldiersBodyType( YAM_MONSTER, TRUE );
					}
					else
					{
						HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem5 );
					}
				}
				else
				{
					ChangeCurrentSquad( 4 );
					
					if (is_networked)
						grid_display();//hayden
				}
				break;

			case '6':
				if( fAlt )
				{
					HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem6 );
				}
				else
				{
					ChangeCurrentSquad( 5 );
				}
				break;

			case '7':
				if( fAlt )
				{
					HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem7 );
				}
				else
				{
					ChangeCurrentSquad( 6 );
				}
				break;

			case '8':
				if( fAlt )
				{
					HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem8 );
				}
				else
				{
					ChangeCurrentSquad( 7 );
				}
				break;

			case '9':
				if( fAlt )
				{
					HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem9 );
				}
				else
				{
					ChangeCurrentSquad( 8 );
				}
				break;

			case '0':
				if( fAlt )
				{
					// used for calculating item chance to get through in cheat mode
					HandleTacticalTakeInvItem( gGameExternalOptions.iQuickItem0 );
				}
				else
				{
					ChangeCurrentSquad( 9 );
				}
				break;

			case '!':
				break;

			case '@':
				break;

			case '#':
				break;

			case '$':
				if(fCtrl && gGameExternalOptions.fEnableInventoryPoolQ)//dnl ch75 021113
					DisplaySectorItemsInfo();
				else
				{
					// Flugente: trait skill selection menu. Yes, screw squad 13				
					INT32 usMapPos;
					if ( GetMouseMapPos( &usMapPos ) )
					{
						TraitsMenu(usMapPos);
					}
				}
				break;

			case '%':
				break;

			case '^':
				break;

			case '&':
				break;

			case '*':
				if ( fCtrl )
				{
					if ( gTacticalStatus.uiFlags & RED_ITEM_GLOW_ON	)
					{
						gTacticalStatus.uiFlags &= (~RED_ITEM_GLOW_ON );
					}
					else
					{
						gTacticalStatus.uiFlags |= RED_ITEM_GLOW_ON;
					}
				}
				break;

			case '(':
				break;

			case ')':
				break;

			case 'a':

				if ( fCtrl && fAlt )
				{
					// Flugente: fortification
					// set details on how to place fortification nodes
					// loop over all structure constructs, and select those entries that are possible in this map
					std::vector<std::pair<INT16, STR16> > dropdownvector_1 = GetCurrentSectorAllowedFortificationTileSetVector( giCurrentTilesetID );

					DropDownTemplate<DROPDOWNNR_MSGBOX_1>::getInstance( ).SetEntries( dropdownvector_1 );

					if ( !dropdownvector_1.empty() )
					{
						BOOLEAN found = FALSE;
						for ( std::vector<std::pair<INT16, STR16> >::iterator it = dropdownvector_1.begin( ); it != dropdownvector_1.end(); ++it )
						{
							if ( (*it).first == gCurrentFortificationStructure )
							{
								found = TRUE;
								break;
							}
						}

						if ( !found )
							gCurrentFortificationStructure = dropdownvector_1[0].first;
					}

					DropDownTemplate<DROPDOWNNR_MSGBOX_1>::getInstance( ).SetSelectedEntryKey( gCurrentFortificationStructure );

					std::vector<std::pair<INT16, STR16> > dropdownvector_2 = GetTileSetIndexVector( gCurrentFortificationStructure );

					DropDownTemplate<DROPDOWNNR_MSGBOX_2>::getInstance( ).SetEntries( dropdownvector_2 );

					if ( !dropdownvector_2.empty( ) )
					{
						BOOLEAN found = FALSE;
						for ( std::vector<std::pair<INT16, STR16> >::iterator it = dropdownvector_2.begin( ); it != dropdownvector_2.end( ); ++it )
						{
							if ( (*it).first == guiMessageBoxImageIndex )
							{
								found = TRUE;
								break;
							}
						}

						if ( !found )
							guiMessageBoxImageIndex = dropdownvector_2[0].first;
					}

					DropDownTemplate<DROPDOWNNR_MSGBOX_2>::getInstance( ).SetSelectedEntryKey( guiMessageBoxImageIndex );

					if ( dropdownvector_1.empty( ) || dropdownvector_2.empty() )
					{
						CHAR16 text[100];
						swprintf( text, szFortificationText[4], giCurrentTilesetID, gTilesets[giCurrentTilesetID].zName );

						DoMessageBox( MSG_BOX_BASIC_STYLE, text, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
					}
					else
					{
						CHAR16 text[100];
						swprintf( text, szFortificationText[5], giCurrentTilesetID, gTilesets[giCurrentTilesetID].zName );

						VOBJECT_DESC VObjectDesc;
						VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

						sprintf( VObjectDesc.ImageFile, "TILESETS\\%d\\%s", giCurrentTilesetID, gStructureConstruct[gCurrentFortificationStructure].szTileSetName );
						
						// even if AddVideoObject fails, we still set MSG_BOX_FLAG_IMAGE, as we might 'repair' it while the box is open
						if ( !AddVideoObject( &VObjectDesc, &guiMessageBoxImage ) )
						{
							sprintf( VObjectDesc.ImageFile, "TILESETS\\0\\%s", gStructureConstruct[gCurrentFortificationStructure].szTileSetName );

							AddVideoObject( &VObjectDesc, &guiMessageBoxImage );
						}

						DoMessageBox( MSG_BOX_BASIC_STYLE, text, GAME_SCREEN, (MSG_BOX_FLAG_OK | MSG_BOX_FLAG_DROPDOWN_1 | MSG_BOX_FLAG_DROPDOWN_2 | MSG_BOX_FLAG_IMAGE), FortificationSettingCallBack, NULL );
					}
				}
				else if ( fCtrl )
				{
					// Flugente: fortification
					INT32 sGridNo;

					//Get the gridno the cursor is at
					GetMouseMapPos( &sGridNo );

					// a node to add a structure
					AddFortificationPlanNode( sGridNo, (INT8)gsInterfaceLevel, gCurrentFortificationStructure, gCurrentFortificationTileLibraryIndex, TRUE );
				}
				else if ( fAlt )
				{
#ifdef JA2TESTVERSION
					//	ToggleMercsNeverQuit();
					static UINT8 ubAmbientSound = 0;

					ubAmbientSound++;

					if ( ubAmbientSound >= NUM_STEADY_STATE_AMBIENCES )
					{
						ubAmbientSound = 1;
					}

					SetSteadyStateAmbience( ubAmbientSound );

#endif
				}
				else
				{
					BeginAutoBandage( );
				}
				break;

			case 'A':
				//CHRISL: Ammo Crate
				//MM: Ammo Box
				if (fCtrl)
					HandleTacticalAmmoCrates( AMMO_CRATE );
				else
					HandleTacticalAmmoCrates( AMMO_BOX );
				break;

			case 'b':

				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						*puiNewEvent = I_NEW_BADMERC;
					}
				}
				else if( fCtrl )
				{
					// Flugente: fortification
					INT32 sGridNo;

					//Get the gridno the cursor is at
					GetMouseMapPos( &sGridNo );

					// a node to delete a structure
					AddFortificationPlanNode( sGridNo, (INT8)gsInterfaceLevel, gCurrentFortificationStructure, gCurrentFortificationTileLibraryIndex, FALSE );
				}
				else
				{
					// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
					if ( ( gpItemPointer == NULL ) &&
						( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
					{
						SetBurstMode();
					}
				}
				break;

			case 'B':
				//Drop pack for all mercs on current map
				HandleTBDropBackpacks();
				break;
			case 'c':

				if ( fAlt && fCtrl )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						ToggleCliffDebug( );
					}
				}
				else if ( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						CreateNextCivType();
					}
				}
				else if( fCtrl )
				{
					HandleTacticalCoverMenu();
				}
				else
				{
					HandleStanceChangeFromUIKeys( ANIM_CROUCH );
				}
				break;

			case 'C':

				if ( fAlt )
					ToggleHostileTrapsView();	// added by Flugente
				else
					ToggleEnemyView();

				break;

			case 'd':
				if( gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT )
				{
					if( gTacticalStatus.ubCurrentTeam == gbPlayerNum )
					{
						// nothing in hand and the Done button for whichever panel we're in must be enabled
						if ( ( gpItemPointer == NULL ) && !gfDisableTacticalPanelButtons &&
							( ( ( gsCurInterfacePanel == SM_PANEL	) && ( ButtonList[ iSMPanelButtons[ SM_DONE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) ||
							( ( gsCurInterfacePanel == TEAM_PANEL ) && ( ButtonList[ iTEAMPanelButtons[ TEAM_DONE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) ) )
						{
							if( fAlt )
							{
								INT32 cnt;
								SOLDIERTYPE *pSoldier;

								if ( CHEATER_CHEAT_LEVEL( ) )
								{
									for ( pSoldier = MercPtrs[ gbPlayerNum ], cnt = 0; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
									{
										if ( pSoldier->bActive && pSoldier->stats.bLife > 0 )
										{
											// Get APs back...
											pSoldier->CalcNewActionPoints( );

											fInterfacePanelDirty = DIRTYLEVEL2;
										}
									}
								}
							} 
							else
							{
								//End turn only if in combat and it is the player's turn
								if ( fCtrl )
									gTacticalStatus.ubDisablePlayerInterrupts = TRUE;								
								
								*puiNewEvent = I_ENDTURN;
							}
						}
					}
				}

#ifdef JA2TESTVERSION
				else if( fCtrl )
				{
					AdvanceToNextDay();
				}
#endif
				// sevenfm: press 'd' in realtime to start turnbased
				else	
				{
					if ( (gTacticalStatus.uiFlags & INCOMBAT) || (NumEnemyInSector() != 0) )
					{
						EnterCombatMode( OUR_TEAM );
					}
				}
				break;

			case 'D':
				// SANDRO - changed from drop all to enable soldier tooltips
				if ( gGameSettings.fOptions[ TOPTION_ALLOW_SOLDIER_TOOLTIPS ] )
				{
					gGameSettings.fOptions[ TOPTION_ALLOW_SOLDIER_TOOLTIPS ] = FALSE;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SOLDIER_TOOLTIPS_OFF ] );
				}
				else
				{
					gGameSettings.fOptions[ TOPTION_ALLOW_SOLDIER_TOOLTIPS ] = TRUE;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SOLDIER_TOOLTIPS_ON ] );
				}
			//	if ( gGameSettings.fOptions[TOPTION_DROP_ALL] )
			//	{
			//		gGameSettings.fOptions[TOPTION_DROP_ALL] = FALSE;
			//		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_DROP_ALL_OFF ] );
			//	}
			//	else
			//	{
			//		gGameSettings.fOptions[TOPTION_DROP_ALL] = TRUE;
			//		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_DROP_ALL_ON ] );
			//	}
				break;

			case 'e':

				if( fAlt )
				{
					// used for override player's turn in network game
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						ToggleViewAllMercs();
						ToggleViewAllItems();
					}
				}
#ifdef JA2BETAVERSION
				else if( fCtrl )
				{
					ToggleMapEdgepoints();
				}
#endif
				else
				{
					SOLDIERTYPE *pSoldier;

					if ( gusSelectedSoldier != NOBODY )
					{
						pSoldier = MercPtrs[ gusSelectedSoldier ];

						if ( pSoldier->aiData.bOppCnt > 0 )
						{
							// Cycle....
							CycleVisibleEnemies( pSoldier );
						}
						else
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_ENEMIES_IN_SIGHT_STR ] );
						}
					}
				}
				break;

			case 'E':
				//CHRISL: drop all items
				if ( gusSelectedSoldier != NOBODY && !(gTacticalStatus.fEnemyInSector) )
				{
					SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];
					for(int i = BODYPOSFINAL; i<NUM_INV_SLOTS; i++)
					{
						if(pSoldier->inv[i].exists() == true)
						{
							AddItemToPool(pSoldier->sGridNo, &pSoldier->inv[i], 1, pSoldier->pathing.bLevel, 0, -1);
							//pSoldier->inv[i].initialize();
							DeleteObj(&pSoldier->inv[i]);
						}
					}
					fCharacterInfoPanelDirty = TRUE;
					fInterfacePanelDirty = DIRTYLEVEL2;
				}
				break;

			case 'f':
				if( fCtrl )
				{
					if ( INFORMATION_CHEAT_LEVEL( ) )
					{
						//Toggle Frame Rate Display
						gbFPSDisplay = !gbFPSDisplay;
						DisableFPSOverlay( (BOOLEAN)!gbFPSDisplay );
						if( !gbFPSDisplay )
							SetRenderFlags( RENDER_FLAG_FULL );
					}
				}
				else if( fAlt )
				{
					if ( gGameSettings.fOptions[ TOPTION_TRACKING_MODE ] )
					{
						gGameSettings.fOptions[ TOPTION_TRACKING_MODE ] = FALSE;

						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_TRACKING_MODE_OFF ] );
					}
					else
					{
						gGameSettings.fOptions[ TOPTION_TRACKING_MODE ] = TRUE;

						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_TRACKING_MODE_ON ] );
					}
				}
				else
				{
					INT32 usGridNo;

					//Get the gridno the cursor is at
					GetMouseMapPos( &usGridNo );

					// if the cursor location is valid					
					if ( !TileIsOutOfBounds(usGridNo) )
					{
						// if there is a selected soldier
						if ( gusSelectedSoldier != NOBODY )
						{
							//if the cursor is over someone
							if ( gfUIFullTargetFound )
							{
								//Display the range to the target
								DisplayRangeToTarget( MercPtrs[gusSelectedSoldier], MercPtrs[gusUIFullTargetID]->sGridNo );
							}
							else
							{
								//Display the range to the target
								DisplayRangeToTarget( MercPtrs[gusSelectedSoldier], usGridNo );
							}
						}

						CHAR16	zOutputString[512];
						swprintf( zOutputString, L"%s: %d", pUpdateMapInfoText[19], usGridNo);
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );

						// Flugente: print out structure tileset
						if ( gGameExternalOptions.fPrintStructureTileset )
						{
							STRUCTURE * pStruct = FindStructure( usGridNo, ( STRUCTURE_TYPE_DEFINED ) );
							if ( pStruct )
							{
								// if this is a multi-tile structure, be sure to use the base gridno
								if ( !(pStruct->fFlags & STRUCTURE_BASE_TILE) )
								{
									pStruct = FindBaseStructure( pStruct );
								}

								if ( pStruct )
								{
									LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

									if ( pNode )
									{
										UINT32 uiTileType = 0;
										if ( GetTileType( pNode->usIndex, &uiTileType ) )
										{
											std::string tilesestr = GetNameToTileSet( uiTileType );

											CHAR16 whcarthing[100];

											int nChars = MultiByteToWideChar( CP_ACP, 0, tilesestr.c_str(), -1, NULL, 0 );
											MultiByteToWideChar( CP_UTF8, 0, tilesestr.c_str( ), -1, whcarthing, nChars );

											swprintf( zOutputString, L"Tileset %d: Tilesetname: %s tileindex: %d", giCurrentTilesetID, whcarthing, pStruct->pDBStructureRef->pDBStructure->usStructureNumber );
											ScreenMsg( FONT_MCOLOR_LTGREEN, MSG_INTERFACE, zOutputString );
										}
									}
								}
							}
						}

						extern BOOLEAN InARoom( INT32 sGridNo, UINT16 *pusRoomNo );

						UINT16 usRoom;
						if ( InARoom( usGridNo, &usRoom ) )
						{
							swprintf( zOutputString, L"Room number: %d", usRoom );
							ScreenMsg( FONT_MCOLOR_LTGREEN, MSG_INTERFACE, zOutputString );
						}

						// display height
						swprintf( zOutputString, L"Floor height: %d", gpWorldLevelData[usGridNo].sHeight );
						ScreenMsg( FONT_MCOLOR_LTGREEN, MSG_INTERFACE, zOutputString );
					}
				}
				break;

			case 'F':
				if( fCtrl && UsingNewInventorySystem() == true )
				{
					HandleTBPickUpBackpacks();
				}
				SeperateItems();
				if( fCtrl )
					StackAndSort( TRUE );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pImpButtonText[11] );
				break;

			case 'G':
				if( fCtrl )
				{				
				}
				else if ( gGameSettings.fOptions[TOPTION_GL_BURST_CURSOR] )
				{
					gGameSettings.fOptions[TOPTION_GL_BURST_CURSOR] = FALSE;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_GL_BURST_CURSOR_OFF ] );
				}
				else
				{
					gGameSettings.fOptions[TOPTION_GL_BURST_CURSOR] = TRUE;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_GL_BURST_CURSOR_ON ] );
				}
				break;
			case 'g':
				if( fCtrl && fAlt )
				{
					HandleTBToggleFormation();
				}
				else if( fCtrl )
				{
					// Flugente: ask player whether he wants to activate cheats or not, but not in multiplayer
					if ( !is_networked )
					{
						if ( CHEATER_CHEAT_LEVEL() )
						{
							DoScreenIndependantMessageBox( pMessageStrings[MSG_PROMPT_CHEATS_DEACTIVATE], MSG_BOX_FLAG_YESNO, ActivateCheatsMessageBoxCallBack );
						}
						else
						{
							DoScreenIndependantMessageBox( pMessageStrings[MSG_PROMPT_CHEATS_ACTIVATE], MSG_BOX_FLAG_YESNO, ActivateCheatsMessageBoxCallBack );
						}
					}
				}
				else if ( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						*puiNewEvent = I_NEW_MERC;
					}
				}
				else
				{
					HandlePlayerTogglingLightEffects(TRUE);
				}
				break;

			case 'H':
				HandleTBSwapHands();
				break;

			case 'h':
				if ( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						if ( gfReportHitChances )
						{
							gfReportHitChances = FALSE;
						}
						else
						{
							gfReportHitChances = TRUE;
						}
					}
				}
				else if( fCtrl )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						*puiNewEvent = I_TESTHIT;
					}
				}
				else
				{
					ShouldTheHelpScreenComeUp( HELP_SCREEN_TACTICAL, TRUE );
				}
				break;
			case 'I':
				//CHRISL: This will create a large number of objects for checking overloading //dnl ch75 251013
				if(fAlt && fCtrl)
				{
					if(CHEATER_CHEAT_LEVEL())
					{
						for ( UINT16 i = FIRST_WEAPON; i<gMAXITEMS_READ; ++i )
						{
							if(i == 1580 || Item[i].ubWeight == 0 && !(i == 257 || i == 1006 || i == 1026 || i == 1183))//dnl!!! items 257, 1006, 1026, 1183 had weight 0 which need to be changed in xml
								continue;

							CreateItem(i, 100, &gTempObject);
							AddItemToPool(usMapPos, &gTempObject, VISIBLE , 0, WORLD_ITEM_REACHABLE, 0);
						}
					}
				}
				break;

			case 'i':
				if ( fCtrl && fAlt )
				{
					if ( gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] )
					{
						gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] = FALSE;
						ToggleItemGlow( FALSE );
					}
					else
					{
						gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] = TRUE;
						ToggleItemGlow( TRUE );
					}
				}
				else if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						CreateRandomItem();
					}
				}
				else if( fCtrl )
				{
					// free to use
				}
				else
				{
					// freed, use this only for a frequently-used new function
				}
				break;

			case 'j':
				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						gfNextFireJam	= TRUE;
					}
				}
				else if ( fCtrl )
				{
#ifdef JA2BETAVERSION
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						ToggleNPCRecordDisplay();
					}
#endif
				}
				else
				{
					SOLDIERTYPE *pjSoldier;
					if ( GetSoldier( &pjSoldier, gusSelectedSoldier ) )
					{
						// Flugente: robots do not climb
						if ( AM_A_ROBOT( pjSoldier ) )
							break;

						INT16							sAPCost;
						INT16							sBPCost;
						BOOLEAN	fNearHeigherLevel;
						BOOLEAN	fNearLowerLevel;
						INT8	bDirection;

						// Make sure the merc is not collapsed!
						if (!IsValidStance(pjSoldier, ANIM_CROUCH) )
						{
							if ( pjSoldier->bCollapsed && pjSoldier->bBreath < OKBREATH )
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, gzLateLocalizedString[ 4 ], pjSoldier->GetName() );
							}

							break;
						}

						// Climb on roof
						GetMercClimbDirection( pjSoldier->ubID, &fNearLowerLevel, &fNearHeigherLevel );

						if ( fNearLowerLevel )
						{
							// No climbing when wearing a backpack!
							if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
								//JMich.BackpackClimb
								&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
								&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB] );
								return;
							}

							if ( EnoughPoints( pjSoldier, GetAPsToClimbRoof( pjSoldier, TRUE ), GetBPsToClimbRoof( pjSoldier, TRUE ), FALSE )	)
							{
								pjSoldier->BeginSoldierClimbDownRoof( );
								return;
							}
						}

						if ( fNearHeigherLevel )
						{
							// No climbing when wearing a backpack!
							if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
								//JMich.BackpackClimb
								&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
								&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB] );
								return;
							}

							if ( EnoughPoints( pjSoldier, GetAPsToClimbRoof( pjSoldier, FALSE ), GetBPsToClimbRoof( pjSoldier, FALSE ), FALSE )	)
							{
								pjSoldier->BeginSoldierClimbUpRoof(	);
								return;
							}
						}

						// Jump over fence
						if ( FindFenceJumpDirection( pjSoldier, pjSoldier->sGridNo, pjSoldier->ubDirection, &bDirection ) )
						{
							if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
								//JMich.BackpackClimb
								&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
								&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
							{
								//Moa: no jumping whith backpack
								//sAPCost = GetAPsToJumpFence( pjSoldier, TRUE );
								//sBPCost = GetBPsToJumpFence( pjSoldier, TRUE );
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB] );
								break;
							}
							else
							{
								sAPCost = GetAPsToJumpFence( pjSoldier, FALSE );
								sBPCost = GetBPsToJumpFence( pjSoldier, FALSE );
							}

							if ( EnoughPoints( pjSoldier, sAPCost, sBPCost, FALSE )	)
							{
								pjSoldier->BeginSoldierClimbFence(	);
								return;
							}	
						}
						
						// Climb on walls
						if (gGameExternalOptions.fCanClimbOnWalls == TRUE)
						{ 
							// No climbing when wearing a backpack!
							if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
								//JMich.BackpackClimb
								&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
								&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB] );
								return;
							}

							if ( FindWallJumpDirection( pjSoldier, pjSoldier->sGridNo, pjSoldier->ubDirection, &bDirection ) )
							{
								if ( EnoughPoints( pjSoldier, GetAPsToJumpWall( pjSoldier, FALSE ), GetBPsToJumpWall( pjSoldier, FALSE ), FALSE )	)
								{
									pjSoldier->BeginSoldierClimbWall(  );
									return;
								}
							}
						}
					}
				}
				break;

			case 'J':
				// WANNE: Jump through window?
				if (gGameExternalOptions.fCanJumpThroughWindows == TRUE )
				{
					INT16		sAPCost;
					INT16		sBPCost;
					INT8	bDirection;
			       	SOLDIERTYPE *lSoldier;

                    if ( GetSoldier( &lSoldier, gusSelectedSoldier ) )
					{
						// Flugente: robots do not climb
						if ( AM_A_ROBOT( lSoldier ) )
							break;

			 			if ( FindWindowJumpDirection( lSoldier, lSoldier->sGridNo, lSoldier->ubDirection, &bDirection ) )
						{
							if ((UsingNewInventorySystem() == true) && lSoldier->inv[BPACKPOCKPOS].exists() == true
								//JMich.BackpackClimb
								&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)lSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[lSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
								&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[lSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
							{
								//Moa: no jumping with backpack
								//sAPCost = GetAPsToJumpThroughWindows( lSoldier, TRUE );
								//sBPCost = GetBPsToJumpThroughWindows( lSoldier, TRUE );
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB] );
								break;
							}
							else
							{
								sAPCost = GetAPsToJumpFence( lSoldier, FALSE );
								sBPCost = GetBPsToJumpFence( lSoldier, FALSE );
							}
							
							if (EnoughPoints(lSoldier, sAPCost, sBPCost, FALSE))
							{
								lSoldier->BeginSoldierClimbWindow(	);
							}
   	                    }
					}
				}
				break;

			case 'k':
				if( fAlt )
				{
					if ( fCtrl )
					{
						if ( CHEATER_CHEAT_LEVEL( ) )
						{
							// next shot by anybody is auto kill
							if (gfNextShotKills)
							{
								gfNextShotKills = FALSE;
							}
							else
							{
								gfNextShotKills = TRUE;
							}
						}
					}
					else
					{
						// used for kicking player out in networked game
						if ( CHEATER_CHEAT_LEVEL( ) )
						{
						}
					}
				}
				else if( fCtrl )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
					}
				}
				else
				{
					BeginKeyPanelFromKeyShortcut( );
				}
				break;

			case 'K':
				// Flugente: reworked this
				//CHRISL: Swap gunsling
				if ( gusSelectedSoldier != NOBODY )
				{
					SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];

					if ( fAlt )
						// switch to knife, or from knife to gun
						pSoldier->SwitchWeapons( TRUE );
					else if ( fCtrl )
						// switch to sidearm, or from sidearm to non-sidearm gun
						pSoldier->SwitchWeapons( FALSE, TRUE );
					else
						// switch to and from gunsling
						pSoldier->SwitchWeapons( );
				}
				break;

			case 'l':
				if(fCtrl && fAlt)
				{
#ifdef JA2BETAVERSION
				gfDisplayStrategicAILogs ^= TRUE;
				if (gfDisplayStrategicAILogs)
				{
					ScreenMsg(FONT_LTKHAKI, MSG_INTERFACE, L"Strategic AI Log visually enabled.");
				}
				else
				{
					ScreenMsg(FONT_LTKHAKI, MSG_INTERFACE, L"Strategic AI Log visually disabled.");
				}
				break;
#endif
				}
				else if (fAlt )
				{
					// used for quickloading
				}
				else if (fCtrl)
				{
					// used for loading game
				}
				else
				{
					// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
					if ( ( gpItemPointer == NULL ) &&
						( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
					{
						*puiNewEvent = LC_CHANGE_TO_LOOK;
					}
				}
				break;

			case 'L':
				HandleTacticalTransformFlashlight();
				break;

			case 'm':
				if( fAlt )
				{
					if ( INFORMATION_CHEAT_LEVEL( ) )
					{
						*puiNewEvent = I_LEVELNODEDEBUG;
						CountLevelNodes();
					}
				}
				else if( fCtrl )
				{
					if ( INFORMATION_CHEAT_LEVEL( ) )
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Memory Used: %d + %d = %d vs: %d", guiMemTotal, giMemUsedInSurfaces, ( guiMemTotal + giMemUsedInSurfaces), ( giStartingMemValue - MemGetFree() ) );
					}
				}
				else
				{
					// nothing in hand and the Map Screen button for whichever panel we're in must be enabled
					if ( ( gpItemPointer == NULL ) && !gfDisableTacticalPanelButtons &&
						( ( ( gsCurInterfacePanel == SM_PANEL	) && ( ButtonList[ iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) ||
						( ( gsCurInterfacePanel == TEAM_PANEL ) && ( ButtonList[ iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) ) )
					{
						// go to Map screen
						if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
						{
							GoToMapScreenFromTactical();
						}
					}
				}
				break;

			case 'M':
				if( fAlt )
				{

				}
				else if( fCtrl )
				{
					// Merge all items
					StackAndSort( FALSE );
				}
				else
				{
					HandleTacticalMoveItems();
				}
				break;

				// swap sungoggles and nightgoggles / put on gas masks
			case 'N':
				SOLDIERTYPE	*pTeamSoldier;
				INT8		bLoop;

				// emergency command: everybody in this sector puts on gasmasks
				if ( fAlt )
				{
					for ( bLoop = gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier = MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++ )
					{
						if ( !AM_A_ROBOT( pTeamSoldier ) && pTeamSoldier->stats.bLife >= OKLIFE && (pTeamSoldier->sSectorX == gWorldSectorX) && (pTeamSoldier->sSectorY == gWorldSectorY) && (pTeamSoldier->bSectorZ == gbWorldSectorZ) )
						{
							WearGasMaskIfAvailable( pTeamSoldier );
						}
					}

					// redraw panels
					fCharacterInfoPanelDirty = TRUE;
					fTeamPanelDirty = TRUE;
					fInterfacePanelDirty = DIRTYLEVEL2;
				}
				// HEADROCK HAM B2.8: Added call for CTRL-SHIFT-N to switch all soldiers to day/night.
				else if ( fCtrl )
				{
					BOOLEAN fToNightVision;

					for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
					{
						if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
						{
							if ( pTeamSoldier->inv[HEAD1POS].exists() || pTeamSoldier->inv[HEAD2POS].exists() )
							{
								fToNightVision = ( Item[pTeamSoldier->inv[HEAD1POS].usItem].brightlightvisionrangebonus > 0 || Item[pTeamSoldier->inv[HEAD2POS].usItem].brightlightvisionrangebonus > 0 );
								break;
							}
							if ( bLoop == gTacticalStatus.Team[gbPlayerNum].bLastID )
							{
								// Default to night or day based on game hour
								fToNightVision = NightTime();
							}
						}
					}
					for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
					{
						// HEADROCK HAM 3.5: When this INI setting is enabled, ALL mercs in the current sector will do a goggle swap.
						if (gGameExternalOptions.fGoggleSwapAffectsAllMercsInSector)
						{
							if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->sSectorX == gWorldSectorX && pTeamSoldier->sSectorY == gWorldSectorY && pTeamSoldier->bSectorZ == gbWorldSectorZ && !AM_A_ROBOT( pTeamSoldier ) )
							{
								SwapGogglesUniformly(pTeamSoldier, fToNightVision);
							}
						}
						else
						{
							if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
							{
								SwapGogglesUniformly(pTeamSoldier, fToNightVision);
							}
						}
					}
				}
				else // Shift-N
				{
					HandleTBSwapGoogles();
				}
				break;

			case 'n':
				if( fAlt )
				{
					static UINT16 gQuoteNum = 0;

					if ( INFORMATION_CHEAT_LEVEL( ) )
					{
						if( gfUIFullTargetFound )
						{
							TacticalCharacterDialogue( MercPtrs[ gusUIFullTargetID ], gQuoteNum );
							gQuoteNum++;
						}
					}
				}
				else if( fCtrl )
				{
#if 0
					if ( INFORMATION_CHEAT_LEVEL( ) )
					{
						if( gfUIShowCurIntTile ^= TRUE )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Turning Enhanced mouse detection ON." );
							gubIntTileCheckFlags	= INTILE_CHECK_FULL;
						}
						else
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Turning Enhanced mouse detection OFF." );
							gubIntTileCheckFlags	= INTILE_CHECK_SELECTIVE;
						}
					}
#endif
				}
				else
					if( !CycleSoldierFindStack( usMapPos ) )// Are we over a merc stack?
						CycleIntTileFindStack( usMapPos ); // If not, now check if we are over a struct stack
				break;

			case 'o':

				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						gStrategicStatus.usPlayerKills += NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
						ObliterateSector();
					}
				}
				else if(fCtrl)
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						CreatePlayerControlledMonster();
					}
				}
				else
				{
					// nothing in hand and the Options Screen button for whichever panel we're in must be enabled
					if ( ( gpItemPointer == NULL ) && !gfDisableTacticalPanelButtons &&
						( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ OPTIONS_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
					{
						if( !fDisableMapInterfaceDueToBattle )
						{
							// go to Options screen
							guiPreviousOptionScreen = GAME_SCREEN;
							LeaveTacticalScreen( OPTIONS_SCREEN );
						}
					}
				}
				break;

			case 'O':
				HandleTacticalTransformScope();
				break;

			case 'p':

#ifdef JA2BETAVERSION
				if( fAlt )
					ToggleRealTimeConfirm();
				else
#endif

#ifdef JA2TESTVERSION
					if( fCtrl )
					{
						// CTRL-P: Display player's highest progress percentage
						DumpSectorDifficultyInfo();
					}
					else
#endif
						HandleStanceChangeFromUIKeys( ANIM_PRONE );
				break;

			case 'P':
				break;

			case 'q':
				if( fCtrl )
				{
					// used for cheat mode functions
					HandleTBSwapHands( );												
				}
				else if ( fAlt )
				{
					// used for cheat mode functions
					HandleTBSwapGunsling( );
				}
				else
				{
					if ( gGameSettings.fOptions[TOPTION_GL_HIGH_ANGLE] )
					{
						gGameSettings.fOptions[TOPTION_GL_HIGH_ANGLE] = FALSE;
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_GL_LOW_ANGLE ] );
					}
					else
					{
						gGameSettings.fOptions[TOPTION_GL_HIGH_ANGLE] = TRUE;
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_GL_HIGH_ANGLE ] );
					}
				}
				break;
			
			case 'Q':
				HandleTacticalDropItem( HANDPOS );
				break;

				// Make auto reload with magazines from sector inventory
			case 'R':
				HandleTBReloadAll();

				break;
			case 'r':
				if( gusSelectedSoldier != NOBODY )
				{
					if( fAlt ) //reload selected merc's weapon
					{
						if ( CHEATER_CHEAT_LEVEL( ) )
						{
							ReloadWeapon( MercPtrs[ gusSelectedSoldier ], MercPtrs[ gusSelectedSoldier ]->ubAttackingHand );
						}
						else
							HandleTBReload();
					}
					else if( fCtrl )
					{
						if ( INFORMATION_CHEAT_LEVEL( ) )
						{
						}
					}
					else
					{
						HandleTBSoldierRun();
					}
				}
				break;

			case 'S':
				StackAndSort( TRUE );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pImpButtonText[11] );
				break;

			case 's':

				if( fCtrl )
				{
					if ( fAlt )
						RemoveCharacterFromSquads(MercPtrs[gusSelectedSoldier]);

					else if( !fDisableMapInterfaceDueToBattle && !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) && !is_networked)
					{
						// WANNE: Do not allow saving via the save screen when it is not our turn,
						// because there is an explit when you close the save window without saving, you can move your merc even it is not your turn
						// IF UI HAS LOCKED, ONLY ALLOW EXIT!
						if ( gfDisableRegionActive || gfUserTurnRegionActive )
						{
							continue;
						}

						//if the game CAN be saved
						if( CanGameBeSaved() )
						{
							gfSaveGame = TRUE;
							gfCameDirectlyFromGame = TRUE;

							guiPreviousOptionScreen = GAME_SCREEN;
							LeaveTacticalScreen( SAVE_LOAD_SCREEN );
						}
						else
						{
							//Display a message saying the player cant save now
							switch( gGameOptions.ubIronManMode )
							{
								case 2:
									DoMapMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__SOFT_IRON_MAN_CANT_SAVE_NOW ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL );
									break;
								case 3:
									CHAR16 zTemp[320];
									swprintf( zTemp, zNewTacticalMessages[ TCTL_MSG__EXTREME_IRON_MAN_CANT_SAVE_NOW ], gGameExternalOptions.ubExtremeIronManSavingHour);
									DoMapMessageBox( MSG_BOX_BASIC_STYLE, zTemp, GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL );
									break;
								default:
									DoMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__IRON_MAN_CANT_SAVE_NOW ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL);
									break;
							}
						}
					}
				}
				else
					if( fAlt )
					{
						if( !fDisableMapInterfaceDueToBattle && !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )&& !is_networked )
						{
							//if the game CAN be saved
							if( CanGameBeSaved() )
							{
								guiPreviousOptionScreen = GAME_SCREEN;
								//guiPreviousOptionScreen = guiCurrentScreen;
								DoQuickSave();
							}
							else
							{
								//Display a message saying the player cant save now
								switch( gGameOptions.ubIronManMode )
								{
									case 2:
										DoMapMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__SOFT_IRON_MAN_CANT_SAVE_NOW ], GAME_SCREEN, MSG_BOX_FLAG_OK, NULL );
										break;
									case 3:
										CHAR16 zTemp[320];
										swprintf( zTemp, zNewTacticalMessages[ TCTL_MSG__EXTREME_IRON_MAN_CANT_SAVE_NOW ], gGameExternalOptions.ubExtremeIronManSavingHour);
										DoMapMessageBox( MSG_BOX_BASIC_STYLE, zTemp, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL );
										break;
									default:
										DoMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__IRON_MAN_CANT_SAVE_NOW ], GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
										break;
								}
							}
						}
					}
					else if( gusSelectedSoldier != NOBODY )
					{
						gfPlotNewMovement = TRUE;
						HandleStanceChangeFromUIKeys( ANIM_STAND );
					}
					break;

			case 't':

				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						TeleportSelectedSoldier();
					}
				}
				else if( fCtrl && !is_networked)// ary-05/05/2009 : add forced turn mode
				{
					ToggleTurnMode();
				}
				else
					ToggleTreeTops();
				break;

			case 'T':	
				if ( fCtrl && fShift && fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						TestCapture( );

						//EnterCombatMode( gbPlayerNum );
					}
				}
				else if ( fCtrl && fShift )
				{
					//SaveGame( SAVE__TIMED_AUTOSAVE_SLOT1, L"Auto Save 1" );
					swprintf( zString, L"%s %d",pMessageStrings[ 90 ],SAVE__TIMED_AUTOSAVE_SLOT1);
					DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT1,zString);
				}
				else if ( fAlt && fShift )
				{	
					//resort Team by ubID
					SortSquadByID(MercPtrs[gusSelectedSoldier]->bTeam);
				}
				else
				{	
					HandleTacticalTransformItem();
				}
				break;

			case 'u':

				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						RefreshSoldier();
					}
				}
				else if( fCtrl )
				{
					if ( CHEATER_CHEAT_LEVEL( ) && gusSelectedSoldier != NOBODY )
					{
						INT32 cnt;
						SOLDIERTYPE *pSoldier;

						for ( pSoldier = MercPtrs[ gbPlayerNum ], cnt = 0; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
						{
							if ( pSoldier->bActive && pSoldier->stats.bLife > 0 )
							{
								// Get breath back
								pSoldier->bBreath								= pSoldier->bBreathMax;

								// Get life back
								pSoldier->stats.bLife									= pSoldier->stats.bLifeMax;
								pSoldier->bBleeding							= 0;

								// SANDRO - erase insta-healable injury 
								pSoldier->iHealableInjury = 0; 

								fInterfacePanelDirty = DIRTYLEVEL2;
							}
						}
					}
				}

				else if ( gusSelectedSoldier != NOBODY )
					*puiNewEvent = M_CHANGE_TO_ACTION;
				break;

			case 'U':
				break;

			case 'v':
				if( fAlt )
				{
#ifdef JA2TESTVERSION
					if( gfDoVideoScroll ^= TRUE )
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Video Scroll ON"	);
					else
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Video Scroll OFF"	);
#endif
				}
				else if( fCtrl )
				{
#ifdef SGP_VIDEO_DEBUGGING
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"VObjects:	%d", guiVObjectSize );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"VSurfaces:	%d", guiVSurfaceSize );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"SGPVideoDump.txt updated..." );
					PerformVideoInfoDumpIntoFile( "SGPVideoDump.txt", TRUE );
#else
					HandleTacticalInventoryMenu();
#endif
				}
				else
					DisplayGameSettings( );

				break;

			case 'V':
				if( fAlt )
					ToggleTrapNetworkView();	// added by Flugente
				else
					ToggleMercView();

				break;

			case 'w':
				if ( fCtrl && fAlt )
				{
					ToggleWireFrame();
				}
				else if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						// cycle item forward in primary hand
						if ( InItemDescriptionBox( ) )
						{
							// Swap item in description panel...
							CycleItemDescriptionItem( ITEMDESC_START_X, ITEMDESC_START_Y);

						}
						else
						{
							CycleSelectedMercsItem();
						}
					}
				}
				else if( fCtrl )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						if ( gusSelectedSoldier != NOBODY )
						{
							CreateItem( FLAMETHROWER, 100, &( MercPtrs[ gusSelectedSoldier ]->inv[ HANDPOS ]) );
						}
					}
				}
				else
				{
					// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
					if ( ( gpItemPointer == NULL ) &&
						( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
					{
						*puiNewEvent = LC_CHANGE_TO_LOOK;
					}
				}
				break;

			case 'W':
				if( fAlt )
				{
					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						// cycle item backward in primary hand
						if ( InItemDescriptionBox( ) )
						{
							// Swap item in description panel...
							CycleItemDescriptionItem( ITEMDESC_START_X, ITEMDESC_START_Y);

						}
						else
						{
							CycleSelectedMercsItem();
						}
					}
				}
				break;

			case 'x':

				if ( !fCtrl && !fAlt )
				{
					// Exchange places...
					SOLDIERTYPE *pSoldier1, *pSoldier2;

					//Check if we have a good selected guy
					if ( gusSelectedSoldier != NOBODY )
					{
						pSoldier1 = MercPtrs[ gusSelectedSoldier ];

						BOOLEAN fFoundGoodTarget = FALSE;
						if ( gfUIFullTargetFound )
						{
							// Get soldier...
							pSoldier2 = MercPtrs[ gusUIFullTargetID ];

							// Check if both OK....
							if ( pSoldier1->stats.bLife >= OKLIFE && pSoldier2->ubID != gusSelectedSoldier )
							{
								if ( pSoldier2->stats.bLife >= OKLIFE )
								{
									if (CanSoldierReachGridNoInGivenTileLimit( pSoldier1, pSoldier2->sGridNo, 1, (INT8)gsInterfaceLevel ) )
									{
										// Exclude enemies....
										if ( !pSoldier2->aiData.bNeutral && (pSoldier2->bSide != gbPlayerNum ) )
										{

										}
										else
										{
											if ( CanExchangePlaces( pSoldier1, pSoldier2, TRUE ) )
											{
												// All's good!
												SwapMercPositions( pSoldier1, pSoldier2 );

												DeductPoints( pSoldier1, APBPConstants[AP_EXCHANGE_PLACES], 0 );
												DeductPoints( pSoldier2, APBPConstants[AP_EXCHANGE_PLACES], 0 );
											}
										}
										fFoundGoodTarget = TRUE;
									}
								}
							}
						}
						if( !fFoundGoodTarget )
						{
							HandleStanceChangeFromUIKeys( ANIM_PRONE );
						}
					}
				}
				else if ( fCtrl )	// The_Bob - real time sneaking, 01-06-09
				{	
					HandleTBEnterTurnbased();					
				}
				else if ( fAlt )
				{
					// used for exit game
				}
				break;
						// The_Bob - real time sneaking, 01-06-09
			case 'X':	// shift-ctrl-x: toggle real time sneaking
				if ( fCtrl )
				{
					HandleTBToggleSneak();
				}
				break;

			case 'y':
				if( fAlt )
				{
					OBJECTTYPE		Object;
					SOLDIERTYPE *pSoldier;

					if ( CHEATER_CHEAT_LEVEL( ) )
					{
						QuickCreateProfileMerc( CIV_TEAM, MARIA ); //Ira

						// Recruit!
						RecruitEPC( MARIA );
					
						//Heinz: 28.02.09 BUGFIX: giving G41 to Maria only in cheat mode
						// Create object and set
						CreateItem( G41, 100, &Object );

						pSoldier = FindSoldierByProfileID( MARIA, FALSE );

						AutoPlaceObject( pSoldier, &Object, FALSE );
					}
				}
				else
				{
					// used for opening chatbox in networked game
					if ( INFORMATION_CHEAT_LEVEL( ) )
					{
						*puiNewEvent = I_LOSDEBUG;
					}
				}
				break;

			case 'Y':
				HandleTacticalTransformLaser();
				break;

			case 'z':
				if( fCtrl && fAlt )
				{
					if ( INFORMATION_CHEAT_LEVEL( ) )
					{
						ToggleZBuffer();
					}
				}
				else if ( fAlt )
				{
					HandleTBToggleStealthAll();
				}
				else if ( fCtrl )
				{
					// used for "lock" / "unlock" the mouse cursor in windowed mode
				}
				else
				{
					HandleTBToggleStealth();
				}
				break;

			case 'Z':
				break;

			}
		}
	}
}


void HandleTalkingMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent )
{
	// CHECK ESC KEYS HERE....
	if ( pInputEvent->usEvent == KEY_UP )
	{
		if ( pInputEvent->usParam == ESC )
		{
			//Handle esc in talking menu
			if ( HandleTalkingMenuEscape( TRUE, TRUE ) )
			{
				*puiNewEvent = A_CHANGE_TO_MOVE;
			}
		}
		else if ( pInputEvent->usParam == BACKSPACE )
		{
			HandleTalkingMenuBackspace();
		}

	}
}

void HandleSectorExitMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent )
{
	// CHECK ESC KEYS HERE....
	if ( ( pInputEvent->usEvent == KEY_UP )&& ( pInputEvent->usParam == ESC ))
	{
		//Handle esc in talking menu
		RemoveSectorExitMenu( FALSE );

		*puiNewEvent = A_CHANGE_TO_MOVE;
	}
}


void HandleOpenDoorMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent )
{
	// CHECK ESC KEYS HERE....
	if ( ( pInputEvent->usEvent == KEY_UP )&& ( pInputEvent->usParam == ESC ))
	{
		//Handle esc in talking menu
		CancelOpenDoorMenu( );
		HandleOpenDoorMenu( );
		*puiNewEvent = A_CHANGE_TO_MOVE;
	}
}


void HandleMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent )
{
	// CHECK ESC KEYS HERE....
	if ( ( pInputEvent->usEvent == KEY_UP )&& ( pInputEvent->usParam == ESC ))
	{
		//Handle esc in talking menu
		CancelMovementMenu( );

		*puiNewEvent = A_CHANGE_TO_MOVE;
	}
}

void HandleItemMenuKeys( InputAtom *pInputEvent, UINT32 *puiNewEvent )
{
	// CHECK ESC KEYS HERE....
	if ( ( pInputEvent->usEvent == KEY_UP )&& ( pInputEvent->usParam == ESC ))
	{
		//Handle esc in talking menu
		RemoveItemPickupMenu( );
		*puiNewEvent = A_CHANGE_TO_MOVE;
	}
}

BOOLEAN HandleCheckForExitArrowsInput( BOOLEAN fAdjustConfirm )
{
	INT32 usMapPos;

	// If not in move mode, return!
	if ( gCurrentUIMode != MOVE_MODE )
	{
		return( FALSE );
	}

	if ( gusSelectedSoldier == NOBODY )
	{
		return( FALSE );
	}

	// ATE: Remove confirm for exit arrows...
	fAdjustConfirm = TRUE;
	gfUIConfirmExitArrows = TRUE;


	//Return right away, saying that we are in this mode, don't do any normal stuff!
	if ( guiCurrentUICursor == NOEXIT_EAST_UICURSOR || guiCurrentUICursor == NOEXIT_WEST_UICURSOR
		|| guiCurrentUICursor == NOEXIT_NORTH_UICURSOR || guiCurrentUICursor == NOEXIT_SOUTH_UICURSOR ||
		guiCurrentUICursor == NOEXIT_GRID_UICURSOR )
	{
		// Yeah, but add a message....
		if( gfInvalidTraversal )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANNOT_LEAVE_SECTOR_FROM_SIDE_STR ] );
			gfInvalidTraversal = FALSE;
		}
		else if( gfRobotWithoutControllerAttemptingTraversal )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, gzLateLocalizedString[ 1 ] );
			gfRobotWithoutControllerAttemptingTraversal = FALSE;
		}
		else if( gfLoneEPCAttemptingTraversal )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, pExitingSectorHelpText[ EXIT_GUI_ESCORTED_CHARACTERS_CANT_LEAVE_SECTOR_ALONE_STR ], MercPtrs[ gusSelectedSoldier ]->GetName() );
			gfLoneEPCAttemptingTraversal = FALSE;
		}
		else if( gubLoneMercAttemptingToAbandonEPCs )
		{
			CHAR16 str[256];
			if( gubLoneMercAttemptingToAbandonEPCs == 1 )
			{ //Use the singular version of the string
				if( gMercProfiles[ MercPtrs[ gusSelectedSoldier ]->ubProfile ].bSex == MALE )
				{ //male singular
					swprintf( str, pExitingSectorHelpText[ EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_MALE_SINGULAR ], MercPtrs[ gusSelectedSoldier ]->GetName(),
						MercPtrs[ gbPotentiallyAbandonedEPCSlotID ]->GetName() );
				}
				else
				{ //female singular
					swprintf( str, pExitingSectorHelpText[ EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_FEMALE_SINGULAR ], MercPtrs[ gusSelectedSoldier ]->GetName(),
						MercPtrs[ gbPotentiallyAbandonedEPCSlotID ]->GetName() );
				}
			}
			else
			{ //Use the plural version of the string
				if( gMercProfiles[ MercPtrs[ gusSelectedSoldier ]->ubProfile ].bSex == MALE )
				{ //male plural
					swprintf( str, pExitingSectorHelpText[ EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_MALE_PLURAL ], MercPtrs[ gusSelectedSoldier ]->GetName() );
				}
				else
				{ //female plural
					swprintf( str, pExitingSectorHelpText[ EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_FEMALE_PLURAL ], MercPtrs[ gusSelectedSoldier ]->GetName() );
				}
			}
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, str );
			gubLoneMercAttemptingToAbandonEPCs = FALSE;
		}
		else if( !gGameExternalOptions.fGridExitInTurnBased && ( gTacticalStatus.uiFlags & TURNBASED ) && ( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANNOT_LEAVE_IN_TURN_MODE_STR ] );
		}			
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ MERC_IS_TOO_FAR_AWAY_STR ], MercPtrs[ gusSelectedSoldier ]->GetName() );
		}

		return( TRUE );
	}

	// Check if we want to exit!
	if ( guiCurrentUICursor == EXIT_GRID_UICURSOR || guiCurrentUICursor == CONFIRM_EXIT_GRID_UICURSOR	)
	{
		if ( fAdjustConfirm )
		{
			if ( !gfUIConfirmExitArrows )
			{
				gfUIConfirmExitArrows = TRUE;
			}
			else
			{
				if ( !GetMouseMapPos( &usMapPos ) )
				{
					return( FALSE );
				}

				// Goto next sector
				//SimulateMouseMovement( gusMouseXPos - 5, gusMouseYPos );
				InitSectorExitMenu( DIRECTION_EXITGRID, usMapPos );
			}
		}
		return( TRUE );
	}

	// Check if we want to exit!
	if ( guiCurrentUICursor == EXIT_EAST_UICURSOR || guiCurrentUICursor == CONFIRM_EXIT_EAST_UICURSOR	)
	{
		if ( fAdjustConfirm )
		{
			if ( !gfUIConfirmExitArrows )
			{
				gfUIConfirmExitArrows = TRUE;
			}
			else
			{
				// Goto next sector
				//SimulateMouseMovement( gusMouseXPos - 5, gusMouseYPos );
				InitSectorExitMenu( EAST, 0 );
			}
		}
		return( TRUE );
	}
	if ( guiCurrentUICursor == EXIT_WEST_UICURSOR || guiCurrentUICursor == CONFIRM_EXIT_WEST_UICURSOR	)
	{
		if ( fAdjustConfirm )
		{
			if ( !gfUIConfirmExitArrows )
			{
				gfUIConfirmExitArrows = TRUE;
			}
			else
			{
				// Goto next sector
				//SimulateMouseMovement( gusMouseXPos + 5, gusMouseYPos );
				InitSectorExitMenu( WEST, 0);
			}
		}
		return( TRUE );
	}
	if ( guiCurrentUICursor == EXIT_NORTH_UICURSOR || guiCurrentUICursor == CONFIRM_EXIT_NORTH_UICURSOR )
	{
		if ( fAdjustConfirm )
		{
			if ( !gfUIConfirmExitArrows )
			{
				gfUIConfirmExitArrows = TRUE;
			}
			else
			{
				// Goto next sector
				//SimulateMouseMovement( gusMouseXPos, gusMouseYPos + 5 );
				InitSectorExitMenu( NORTH , 0);
			}
		}
		return( TRUE );
	}
	if ( guiCurrentUICursor == EXIT_SOUTH_UICURSOR || guiCurrentUICursor == CONFIRM_EXIT_SOUTH_UICURSOR )
	{
		if ( fAdjustConfirm )
		{
			if ( !gfUIConfirmExitArrows )
			{
				gfUIConfirmExitArrows = TRUE;
			}
			else
			{
				// Goto next sector
				//SimulateMouseMovement( gusMouseXPos, gusMouseYPos - 5);
				InitSectorExitMenu( SOUTH, 0 );
			}
		}
		return( TRUE );
	}
	return( FALSE );
}


//Simple function implementations called by keyboard input

void CreateRandomItem()
{
	INT32 usMapPos;
	if ( GetMouseMapPos( &usMapPos ) )
	{
		CreateItem( (UINT16) (Random( 35 ) + 1), 100, &gTempObject );
		AddItemToPool( usMapPos, &gTempObject, -1 , 0, 0, 0 );
	}
}

void MakeSelectedSoldierTired()
{
	// Key to make guy get tired!
	SOLDIERTYPE				*pSoldier;
	INT32 usMapPos;
	if ( GetMouseMapPos( &usMapPos ) )
	{
		CreateItem( TNT, 100, &gTempObject );
		AddItemToPool( usMapPos, &gTempObject, -1, 0, 0, 0 );
	}

	// CHECK IF WE'RE ON A GUY ( EITHER SELECTED, OURS, OR THEIRS
	if ( gfUIFullTargetFound )
	{
		// Get Soldier
		GetSoldier( &pSoldier, gusUIFullTargetID );

		//FatigueCharacter( pSoldier );

		fInterfacePanelDirty = DIRTYLEVEL2;
	}
}

void ToggleRealTime( UINT32 *puiNewEvent )
{
	if ( gTacticalStatus.uiFlags & TURNBASED )
	{
		// Change to real-time
		gTacticalStatus.uiFlags &= (~TURNBASED);
		gTacticalStatus.uiFlags |= REALTIME;

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Switching to Realtime." );
	}
	else if ( gTacticalStatus.uiFlags & REALTIME )
	{
		// Change to turn-based
		gTacticalStatus.uiFlags |= TURNBASED;
		gTacticalStatus.uiFlags &=( ~REALTIME );

		*puiNewEvent = M_ON_TERRAIN;

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Switching to Turnbased." );
	}

	// Plot new path!
	gfPlotNewMovement = TRUE;
}

void ToggleViewAllMercs()
{
	// Set option to show all mercs
	if ( gTacticalStatus.uiFlags&SHOW_ALL_MERCS )
	{
		gTacticalStatus.uiFlags&= (~SHOW_ALL_MERCS );
	}
	else
	{
		gTacticalStatus.uiFlags|= SHOW_ALL_MERCS;
	}

	// RE-RENDER
	SetRenderFlags(RENDER_FLAG_FULL);
}

void ToggleViewAllItems()
{
	// Set option to show all mercs
	if( gTacticalStatus.uiFlags & SHOW_ALL_ITEMS )
	{
		gTacticalStatus.uiFlags &= ~SHOW_ALL_ITEMS ;
	}
	else
	{
		gTacticalStatus.uiFlags |= SHOW_ALL_ITEMS;
	}

	if ( gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] )
	{
		ToggleItemGlow( TRUE );
	}
	else
	{
		ToggleItemGlow( FALSE );
	}

	// RE-RENDER
	SetRenderFlags(RENDER_FLAG_FULL);
}

void TestExplosion()
{
	INT32 usMapPos;
	if ( GetMouseMapPos( &usMapPos ) )
	{
		EXPLOSION_PARAMS	ExpParams ;
		ExpParams.uiFlags			= 0;
		ExpParams.ubOwner			= NOBODY;
		ExpParams.ubTypeID		= STUN_BLAST;
		ExpParams.sGridNo			= usMapPos;

		GenerateExplosion( &ExpParams );

		PlayJA2Sample( EXPLOSION_1, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
	}
}

void CycleSelectedMercsItem()
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CycleSelectedMercsItem"));
	INT16 usOldItem;
	SOLDIERTYPE *pSoldier;
	// Cycle selected guy's item...
	if( gusSelectedSoldier != NOBODY )
	{
		// Get soldier...
		pSoldier = MercPtrs[ gusSelectedSoldier ];

		UINT16 usOldHandItem = pSoldier->inv[HANDPOS].usItem;

		// Cycle item....
		usOldItem = CycleItems(pSoldier->inv[ HANDPOS ].usItem);

		CreateItem( (UINT16)usOldItem, 100, &( pSoldier->inv[ HANDPOS ]) );

		pSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldHandItem, pSoldier->inv[HANDPOS].usItem );

		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
}

void ToggleWireFrame()
{
	if ( gGameSettings.fOptions[ TOPTION_TOGGLE_WIREFRAME ] )
	{
		gGameSettings.fOptions[ TOPTION_TOGGLE_WIREFRAME ] = FALSE;

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_WIREFRAMES_REMOVED ] );
	}
	else
	{
		gGameSettings.fOptions[ TOPTION_TOGGLE_WIREFRAME ] = TRUE;

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_WIREFRAMES_ADDED ] );
	}

	SetRenderFlags(RENDER_FLAG_FULL );
}

void RefreshSoldier()
{
	SOLDIERTYPE *pSoldier;
	INT32 usMapPos;
	// CHECK IF WE'RE ON A GUY ( EITHER SELECTED, OURS, OR THEIRS
	if ( gfUIFullTargetFound )
	{
		// Get Soldier
		GetSoldier( &pSoldier, gusUIFullTargetID );

		pSoldier->ReviveSoldier( );

	}

	if ( GetMouseMapPos( &usMapPos ) )
		sprintf( gDebugStr, "%d %d %d %d %d %d %d %d",
		gubWorldMovementCosts[ usMapPos ][ 0 ][gsInterfaceLevel],
		gubWorldMovementCosts[ usMapPos ][ 1 ][gsInterfaceLevel],
		gubWorldMovementCosts[ usMapPos ][ 2 ][gsInterfaceLevel],
		gubWorldMovementCosts[ usMapPos ][ 3 ][gsInterfaceLevel],
		gubWorldMovementCosts[ usMapPos ][ 4 ][gsInterfaceLevel],
		gubWorldMovementCosts[ usMapPos ][ 5 ][gsInterfaceLevel],
		gubWorldMovementCosts[ usMapPos ][ 6 ][gsInterfaceLevel],
		gubWorldMovementCosts[ usMapPos ][ 7 ][gsInterfaceLevel] );
}

void ChangeSoldiersBodyType( UINT8 ubBodyType, BOOLEAN fCreateNewPalette )
{
	SOLDIERTYPE *pSoldier;
	if( gusSelectedSoldier != NOBODY )
	{
		if( GetSoldier( &pSoldier, gusSelectedSoldier ) )
		{
			pSoldier->ubBodyType = ubBodyType;
			pSoldier->EVENT_InitNewSoldierAnim( STANDING, 0 , TRUE );
			//SetSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );
			if( fCreateNewPalette )
			{
				pSoldier->CreateSoldierPalettes( );

				switch( ubBodyType )
				{
				case ADULTFEMALEMONSTER:
				case AM_MONSTER:
				case YAF_MONSTER:
				case YAM_MONSTER:
				case LARVAE_MONSTER:
				case INFANT_MONSTER:
				case QUEENMONSTER:

					pSoldier->flags.uiStatusFlags |= SOLDIER_MONSTER;
					AssignCreatureInventory( pSoldier );
					CreateItem( CREATURE_YOUNG_MALE_SPIT,		100, &(pSoldier->inv[HANDPOS]) );

					break;

				case TANK_NW:
				case TANK_NE:
				case COMBAT_JEEP:

					pSoldier->flags.uiStatusFlags |= SOLDIER_VEHICLE;
					//pSoldier->inv[ HANDPOS ].usItem = TANK_CANNON;

					pSoldier->inv[ HANDPOS ].usItem = MINIMI;
					pSoldier->bVehicleID = (INT8)AddVehicleToList( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ, HUMMER );

					break;
				}

			}
		}
	}
}

void TeleportSelectedSoldier()
{
	SOLDIERTYPE *pSoldier;
	INT32 usMapPos;
	// CHECK IF WE'RE ON A GUY ( EITHER SELECTED, OURS, OR THEIRS
	if( GetSoldier( &pSoldier, gusSelectedSoldier ) )
	{
		if ( GetMouseMapPos( &usMapPos ) )
		{
			// Check level first....
			if ( gsInterfaceLevel == 0 )
			{
				pSoldier->SetSoldierHeight( 0 );
				TeleportSoldier( pSoldier, usMapPos, FALSE );
				pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );
			}
			else
			{
				// Is there a roof?
				if ( FindStructure( usMapPos, STRUCTURE_ROOF ) != NULL )
				{
					pSoldier->SetSoldierHeight( 50.0 );

					TeleportSoldier( pSoldier, usMapPos, TRUE );
					pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );
				}
			}
		}
	}
}

void ToggleTurnMode()// ary-05/05/2009 : add forced turn mode
{
	if ( !gGameSettings.fOptions[ TOPTION_TOGGLE_TURN_MODE ] )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_FORCED_TURN_MODE ] );
		gGameSettings.fOptions[ TOPTION_TOGGLE_TURN_MODE ] = TRUE;
		EnterCombatMode( OUR_TEAM ); // ary-05/05/2009 : randomize ? i'm leaving this for now due to "player makes the call, player gets dibs"
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_NORMAL_TURN_MODE ] );
		gGameSettings.fOptions[ TOPTION_TOGGLE_TURN_MODE ] = FALSE;
	}
}

void ToggleTreeTops()
{
	if ( gGameSettings.fOptions[ TOPTION_TOGGLE_TREE_TOPS ] )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ REMOVING_TREETOPS_STR ] );
		WorldHideTrees( );
		gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ SHOWING_TREETOPS_STR ]);
		WorldShowTrees( );
		gTacticalStatus.uiFlags &= (~NOHIDE_REDUNDENCY);
	}
	gGameSettings.fOptions[ TOPTION_TOGGLE_TREE_TOPS ] = !gGameSettings.fOptions[ TOPTION_TOGGLE_TREE_TOPS ];

	// FOR THE NEXT RENDER LOOP, RE-EVALUATE REDUNDENT TILES
	InvalidateWorldRedundency( );
}

void ToggleZBuffer()
{
	// Set option to show all mercs
	if ( gTacticalStatus.uiFlags&SHOW_Z_BUFFER )
	{
		gTacticalStatus.uiFlags&= (~SHOW_Z_BUFFER );
		SetRenderFlags(SHOW_Z_BUFFER);
	}
	else
	{
		gTacticalStatus.uiFlags|= SHOW_Z_BUFFER;
	}
}

void TogglePlanningMode()
{
	SOLDIERTYPE *pSoldier;
	INT32 usMapPos;
	// DO ONLY IN TURNED BASED!
	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		// CANCEL FROM PLANNING MODE!
		if ( InUIPlanMode( ) )
		{
			EndUIPlan( );
		}
		else if ( GetMouseMapPos( &usMapPos ) )
		{
			switch( gCurrentUIMode )
			{
			case MOVE_MODE:
				if ( gusSelectedSoldier != NOBODY )
				{
					GetSoldier( &pSoldier, gusSelectedSoldier );
					BeginUIPlan( pSoldier );
					AddUIPlan( usMapPos, UIPLAN_ACTION_MOVETO );
				}
				break;
			case ACTION_MODE:
				if ( gusSelectedSoldier != NOBODY )
				{
					GetSoldier( &pSoldier, gusSelectedSoldier );
					BeginUIPlan( pSoldier );
					AddUIPlan( usMapPos, UIPLAN_ACTION_FIRE );
				}
				break;
			}
		}
	}
}

void SetBurstMode()
{
	if ( gusSelectedSoldier != NOBODY )
	{
		ChangeWeaponMode( MercPtrs[ gusSelectedSoldier ] );
	}
}

void SetScopeMode( INT32 usMapPos )
{
	if ( gusSelectedSoldier != NOBODY )
	{
		if ( GetMouseMapPos( &usMapPos ))
			ChangeScopeMode( MercPtrs[ gusSelectedSoldier ], usMapPos );
		else
			ChangeScopeMode( MercPtrs[ gusSelectedSoldier ], NOWHERE );

		// reevaluate sight
		ManLooksForOtherTeams( MercPtrs[ gusSelectedSoldier ] );
	}
}

void ObliterateSector()
{
	INT32 cnt;
	SOLDIERTYPE *pTSoldier;

	// Kill everybody!
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bLastID + 1;

#ifdef JA2BETAVERSION
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Obliterating Sector!" );
#endif

	for ( pTSoldier = MercPtrs[ cnt ]; cnt < MAX_NUM_SOLDIERS; pTSoldier++, cnt++ )
	{
		if ( pTSoldier->bActive && !pTSoldier->aiData.bNeutral && (pTSoldier->bSide != gbPlayerNum ) )
		{
			//	ANITILE_PARAMS	AniParams;
			//		memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
			//		AniParams.sGridNo							= pTSoldier->sGridNo;
			//		AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
			//	AniParams.usTileType				= FIRSTEXPLOSION;
			//	AniParams.usTileIndex					= FIRSTEXPLOSION1;
			//	AniParams.sDelay							= 80;
			//	AniParams.sStartFrame					= 0;
			//	AniParams.uiFlags							= ANITILE_FORWARD;

			//	CreateAnimationTile( &AniParams );
			//PlayJA2Sample( EXPLOSION_1, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );

			pTSoldier->EVENT_SoldierGotHit( 1, 400, 0, pTSoldier->ubDirection, 320, NOBODY , FIRE_WEAPON_NO_SPECIAL, pTSoldier->bAimShotLocation, 0, NOWHERE );
		}
	}
}

void RandomizeMercProfile()
{
	SOLDIERTYPE *pSoldier;
	// Get selected soldier
	if	( GetSoldier( &pSoldier, gusSelectedSoldier ) )
	{
		// Change guy!
		ForceSoldierProfileID( pSoldier, (UINT8)Random( 30 ) );

		// Dirty interface
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
}

void CreateNextCivType()
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	INT32 usMapPos;
	static						INT8 ubBodyType = FATCIV;

	// Get Grid Corrdinates of mouse
	if ( GetMouseMapPos( &usMapPos ) )
	{
		INT8							iNewIndex;

		MercCreateStruct.ubProfile		= NO_PROFILE;
		MercCreateStruct.sSectorX		= gWorldSectorX;
		MercCreateStruct.sSectorY		= gWorldSectorY;
		MercCreateStruct.bSectorZ		= gbWorldSectorZ;
		MercCreateStruct.ubBodyType		= ubBodyType;
		MercCreateStruct.ubDirection	= SOUTH;

		++ubBodyType;

		if ( ubBodyType > KIDCIV )
		{
			ubBodyType = FATCIV;
		}

		MercCreateStruct.bTeam					= CIV_TEAM;
		MercCreateStruct.sInsertionGridNo		= usMapPos;
		RandomizeNewSoldierStats( &MercCreateStruct );

		if ( TacticalCreateSoldier( &MercCreateStruct, (UINT8 *)&iNewIndex ) )
		{
			AddSoldierToSector( iNewIndex );

			// So we can see them!
			AllTeamsLookForAll(NO_INTERRUPTS);
		}
	}
}

void ToggleCliffDebug()
{
	// Set option to show all mercs
	if ( gTacticalStatus.uiFlags&DEBUGCLIFFS )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Cliff debug OFF." );

		gTacticalStatus.uiFlags&= (~DEBUGCLIFFS );
		SetRenderFlags(RENDER_FLAG_FULL);
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Cliff debug ON." );

		gTacticalStatus.uiFlags|= DEBUGCLIFFS;
	}
}

void CreateCow()
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	INT32 usMapPos;

	// Get Grid Corrdinates of mouse
	if ( GetMouseMapPos( &usMapPos ) )
	{
		INT8							iNewIndex;

		MercCreateStruct.ubProfile		= NO_PROFILE;
		MercCreateStruct.sSectorX		= gWorldSectorX;
		MercCreateStruct.sSectorY		= gWorldSectorY;
		MercCreateStruct.bSectorZ		= gbWorldSectorZ;
		MercCreateStruct.ubBodyType		= COW;
		//MercCreateStruct.bTeam				= SOLDIER_CREATE_AUTO_TEAM;
		MercCreateStruct.bTeam			= CIV_TEAM;
		MercCreateStruct.sInsertionGridNo		= usMapPos;
		RandomizeNewSoldierStats( &MercCreateStruct );

		if ( TacticalCreateSoldier( &MercCreateStruct, (UINT8 *)&iNewIndex ) )
		{
			AddSoldierToSector( iNewIndex );

			// So we can see them!
			AllTeamsLookForAll(NO_INTERRUPTS);
		}
	}
}

void CreateBloodCat()
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	INT32 usMapPos;

	// Get Grid Corrdinates of mouse
	if ( GetMouseMapPos( &usMapPos ) )
	{
		INT8							iNewIndex;

		MercCreateStruct.ubProfile		= NO_PROFILE;
		MercCreateStruct.sSectorX		= gWorldSectorX;
		MercCreateStruct.sSectorY		= gWorldSectorY;
		MercCreateStruct.bSectorZ		= gbWorldSectorZ;
		MercCreateStruct.ubBodyType		= BLOODCAT;
		//MercCreateStruct.bTeam				= SOLDIER_CREATE_AUTO_TEAM;
		MercCreateStruct.bTeam			= CREATURE_TEAM;
		MercCreateStruct.sInsertionGridNo		= usMapPos;
		RandomizeNewSoldierStats( &MercCreateStruct );

		if ( TacticalCreateSoldier( &MercCreateStruct, (UINT8 *)&iNewIndex ) )
		{
			AddSoldierToSector( iNewIndex );

			// So we can see them!
			AllTeamsLookForAll(NO_INTERRUPTS);
		}
	}
}

void CreatePlayerControlledCow()
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	INT32 usMapPos;

	// Get Grid Corrdinates of mouse
	if ( GetMouseMapPos( &usMapPos ) )
	{
		INT8							iNewIndex;

		MercCreateStruct.ubProfile		= 12;
		MercCreateStruct.sSectorX		= gWorldSectorX;
		MercCreateStruct.sSectorY		= gWorldSectorY;
		MercCreateStruct.bSectorZ		= gbWorldSectorZ;
		MercCreateStruct.ubBodyType		= COW;
		MercCreateStruct.sInsertionGridNo		= usMapPos;
		MercCreateStruct.bTeam			= SOLDIER_CREATE_AUTO_TEAM;
		MercCreateStruct.fPlayerMerc	= TRUE;

		RandomizeNewSoldierStats( &MercCreateStruct );

		if ( TacticalCreateSoldier( &MercCreateStruct, (UINT8 *)&iNewIndex ) )
		{
			AddSoldierToSector( iNewIndex );

			// So we can see them!
			AllTeamsLookForAll(NO_INTERRUPTS);
		}
	}
}

void ToggleRealTimeConfirm()
{

}

/*
void GrenadeTest1()
{
	// Get mousexy
	INT16 sX, sY;
	if ( GetMouseXY( &sX, &sY ) )
	{
		OBJECTTYPE		Object;
		Object.usItem = MUSTARD_GRENADE;
		Object[0]->data.objectStatus = 100;
		Object.ubNumberOfObjects = 1;
		CreatePhysicalObject( &Object, 60,	(FLOAT)(sX * CELL_X_SIZE), (FLOAT)(sY * CELL_Y_SIZE ), 256, -20, 20, 158, NOBODY, THROW_ARM_ITEM, 0, FALSE );
	}
}

void GrenadeTest2()
{
	// Get mousexy
	INT16 sX, sY;
	if ( GetMouseXY( &sX, &sY ) )
	{
		OBJECTTYPE		Object;
		Object.usItem = HAND_GRENADE;
		Object[0]->data.objectStatus = 100;
		Object.ubNumberOfObjects = 1;
		CreatePhysicalObject( &Object, 60,	(FLOAT)(sX * CELL_X_SIZE), (FLOAT)(sY * CELL_Y_SIZE ), 256, 0, -30, 158, NOBODY, THROW_ARM_ITEM, 0, FALSE );
	}
}

void GrenadeTest3()
{
	// Get mousexy
	INT16 sX, sY;
	if ( GetMouseXY( &sX, &sY ) )
	{
		OBJECTTYPE		Object;
		Object.usItem = HAND_GRENADE;
		Object[0]->data.objectStatus = 100;
		Object.ubNumberOfObjects = 1;
		CreatePhysicalObject( &Object, 60,	(FLOAT)(sX * CELL_X_SIZE), (FLOAT)(sY * CELL_Y_SIZE ), 256, -10, 10, 158, NOBODY, THROW_ARM_ITEM, 0, FALSE );
	}
}
*/
void CreatePlayerControlledMonster()
{
	INT32 usMapPos;
	if ( GetMouseMapPos( &usMapPos ) )
	{
		SOLDIERCREATE_STRUCT		MercCreateStruct;
		INT8							iNewIndex;

		MercCreateStruct.ubProfile		= NO_PROFILE;
		MercCreateStruct.sSectorX			= gWorldSectorX;
		MercCreateStruct.sSectorY			= gWorldSectorY;
		MercCreateStruct.bSectorZ			= gbWorldSectorZ;

		//Note:	only gets called if Alt and/or Ctrl isn't pressed!
		if ( _KeyDown( INSERT ) )
			MercCreateStruct.ubBodyType		= QUEENMONSTER;
		//MercCreateStruct.ubBodyType		= LARVAE_MONSTER;
		else
			MercCreateStruct.ubBodyType		= ADULTFEMALEMONSTER;
		MercCreateStruct.bTeam				= SOLDIER_CREATE_AUTO_TEAM;
		MercCreateStruct.sInsertionGridNo		= usMapPos;
		RandomizeNewSoldierStats( &MercCreateStruct );

		if ( TacticalCreateSoldier( &MercCreateStruct, (UINT8 *)&iNewIndex ) )
		{
			AddSoldierToSector( iNewIndex );
		}
	}
}


INT8 CheckForAndHandleHandleVehicleInteractiveClick( SOLDIERTYPE *pSoldier, UINT16 usMapPos, BOOLEAN fMovementMode, UINT8 ubSeatIndex )
{
	// Look for an item pool
	INT32 sActionGridNo;
	UINT8							ubDirection;
	SOLDIERTYPE		*pTSoldier;
	INT16							sAPCost = 0;

	if ( gfUIFullTargetFound	)
	{
		pTSoldier = MercPtrs[ gusUIFullTargetID ];

		// anv: added condition - make sure we won't put vehicle in another vehicle
		if ( OK_ENTERABLE_VEHICLE( pTSoldier ) && pTSoldier->bVisible != -1 && OKUseVehicle( pTSoldier->ubProfile ) && !( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER | SOLDIER_VEHICLE ) ) )
		//if ( OK_ENTERABLE_VEHICLE( pTSoldier ) && pTSoldier->bVisible != -1 && OKUseVehicle( pTSoldier->ubProfile ) )
		{
			if ( ( GetNumberInVehicle( pTSoldier->bVehicleID ) == 0 ) || !fMovementMode )
			{
				// Find a gridno closest to sweetspot...
				sActionGridNo = FindGridNoFromSweetSpotWithStructDataFromSoldier( pSoldier, pSoldier->usUIMovementMode, 5, &ubDirection, 0, pTSoldier, TRUE );
				
				if (!TileIsOutOfBounds(sActionGridNo))
				{
					// Calculate AP costs...
					//sAPCost = GetAPsToBeginFirstAid( pSoldier );
					sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

					if ( EnoughPoints( pSoldier, sAPCost, 0, TRUE ) )
					{
						pSoldier->DoMercBattleSound( BATTLE_SOUND_OK1 );

						// CHECK IF WE ARE AT THIS GRIDNO NOW
						if ( pSoldier->sGridNo != sActionGridNo )
						{
							// SEND PENDING ACTION
							pSoldier->aiData.ubPendingAction = MERC_ENTER_VEHICLE;
							pSoldier->aiData.sPendingActionData2	= pTSoldier->sGridNo;
							pSoldier->aiData.bPendingActionData3	= ubDirection;
							pSoldier->aiData.uiPendingActionData4	= ubSeatIndex;
							pSoldier->aiData.ubPendingActionAnimCount = 0;

							// WALK UP TO DEST FIRST
							pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, 3 , pSoldier->flags.fNoAPToFinishMove );
						}
						else
						{
							pSoldier->EVENT_SoldierEnterVehicle( pTSoldier->sGridNo, ubDirection, ubSeatIndex );
						}

						// OK, set UI
						SetUIBusy( pSoldier->ubID );
						//guiPendingOverrideEvent = A_CHANGE_TO_MOVE;

						return( -1 );
					}
				}
			}
		}
	}

	return( 0 );
}

void HandleHandCursorClick( INT32 usMapPos, UINT32 *puiNewEvent )
{
	SOLDIERTYPE *pSoldier;
	LEVELNODE					*pIntTile;
	INT32 sIntTileGridNo;
	INT32 sActionGridNo;
	UINT8							ubDirection;
	INT16							sAPCost;
	INT32 sAdjustedGridNo;
	STRUCTURE					*pStructure = NULL;
	ITEM_POOL					*pItemPool;
	BOOLEAN						fIgnoreItems = FALSE;
	
	if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
	{
		// If we are out of breath, no cursor...
		if ( pSoldier->bBreath < OKBREATH && pSoldier->bCollapsed )
		{
			return;
		}

		if ( CheckForAndHandleHandleVehicleInteractiveClick( pSoldier, usMapPos, FALSE, 0 ) == -1 )
		{
			return;
		}

		if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
		{
			return;
		}

		// Check if we are on a merc... if so.. steal!
		if ( gfUIFullTargetFound )
		{
			// Flugente: allow stealing if the other guy is an enemy, OR if we are on the same team
			if ( (( guiUIFullTargetFlags & ENEMY_MERC ) && !( guiUIFullTargetFlags & UNCONSCIOUS_MERC )) || (AllowedToStealFromTeamMate(pSoldier->ubID, (UINT8)gusUIFullTargetID) && guiUIFullTargetFlags & OWNED_MERC) )
			{
				sActionGridNo =	FindAdjacentGridEx( pSoldier, MercPtrs[ gusUIFullTargetID ]->sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
				if ( sActionGridNo == -1 )
				{
					sActionGridNo = sAdjustedGridNo;
				}

				// Steal!
				sAPCost = GetAPsToStealItem( pSoldier, MercPtrs[ gusUIFullTargetID ], sActionGridNo ); // SANDRO - added target variable

				if ( EnoughPoints( pSoldier, sAPCost, 0, TRUE ) )
				{
					MercStealFromMerc( pSoldier, MercPtrs[ gusUIFullTargetID ] );

					*puiNewEvent = A_CHANGE_TO_MOVE;

					return;
				}
				else
				{
					return;
				}
			}
		}

		// Default action gridno to mouse....
		sActionGridNo = usMapPos;

		// If we are over an interactive struct, adjust gridno to this....
		pIntTile = ConditionalGetCurInteractiveTileGridNoAndStructure( &sIntTileGridNo , &pStructure, FALSE );
		if ( pIntTile != NULL )
		{
			sActionGridNo = sIntTileGridNo;

			//if ( pStructure->fFlags & ( STRUCTURE_SWITCH | STRUCTURE_ANYDOOR ) )
			if ( pStructure->fFlags & ( STRUCTURE_SWITCH ) )
			{
				fIgnoreItems = TRUE;
			}

			if ( pStructure->fFlags & ( STRUCTURE_ANYDOOR ) && sActionGridNo != usMapPos )
			{
				fIgnoreItems = TRUE;
			}
		}

		// Check if we are over an item pool
		// ATE: Ignore items will be set if over a switch interactive tile...
		if ( GetItemPool( sActionGridNo, &pItemPool, pSoldier->pathing.bLevel ) && ITEMPOOL_VISIBLE( pItemPool ) && !fIgnoreItems )
		{
			if ( AM_AN_EPC( pSoldier ) )
			{
				// Display message
				// ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ EPC_CANNOT_DO_THAT ] );
			}
			else if ( UIOkForItemPickup( pSoldier, sActionGridNo ) )
			{
				INT8 bZLevel = GetLargestZLevelOfItemPool( pItemPool );

				SoldierPickupItem( pSoldier, pItemPool->iItemIndex, sActionGridNo, bZLevel );

				*puiNewEvent = A_CHANGE_TO_MOVE;

			}
		}
		else
		{
			if ( pIntTile != NULL && !( pStructure->fFlags & STRUCTURE_HASITEMONTOP ) )
			{
				sActionGridNo =	FindAdjacentGridEx( pSoldier, sIntTileGridNo, &ubDirection, NULL, FALSE, TRUE );
				if ( sActionGridNo == -1 )
				{
					sActionGridNo = sIntTileGridNo;
				}

				// If this is not the same tile as ours, check if we can get to dest!
				if ( sActionGridNo != pSoldier->sGridNo && gsCurrentActionPoints == 0 )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
				}
				else
				{
					if ( SelectedMercCanAffordMove(	)	)
					{
						*puiNewEvent = C_MOVE_MERC;
					}
				}
			}
			else
			{
				// ATE: Here, the poor player wants to search something that does not exist...
				// Why should we not let them make fools of themselves....?
				if ( AM_AN_EPC( pSoldier ) )
				{
					// Display message
					// ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ EPC_CANNOT_DO_THAT ] );
				}
				else
				{
					// Check morale, if < threashold, refuse...
					if ( pSoldier->aiData.bMorale < 30 )
					{
						TacticalCharacterDialogue( pSoldier, QUOTE_REFUSING_ORDER );
					}
					else
					{
						if ( gsCurrentActionPoints == 0 )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
						}
						else
						{
							SoldierPickupItem( pSoldier, NOTHING, sActionGridNo, 0 );
							*puiNewEvent = A_CHANGE_TO_MOVE;
						}
					}
				}
			}
		}
	}
}

void HandleHandCursorRightClick( INT32 usMapPos, UINT32 *puiNewEvent )
{
	SOLDIERTYPE *pSoldier;
	SOLDIERTYPE *pTSoldier;

	if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
	{
		// If we are out of breath, no cursor...
		if( pSoldier->bBreath < OKBREATH && pSoldier->bCollapsed )
		{
			return;
		}

		if( gfUIFullTargetFound )
		{
			pTSoldier = MercPtrs[ gusUIFullTargetID ];
			if( OK_ENTERABLE_VEHICLE( pTSoldier ) && pTSoldier->bVisible != -1 && OKUseVehicle( pTSoldier->ubProfile ) )
			{
				// anv: if we are passengers, only show menu when clicking on vehicle we're in
				if( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
				{
					if( pSoldier->iVehicleId == pTSoldier->bVehicleID )
					{
						VehicleMenu(usMapPos, pSoldier, pTSoldier);
					}
				}
				else
				{
					VehicleMenu(usMapPos, pSoldier, pTSoldier);
				}
				return;
			}
		}
	}
}

extern BOOLEAN AnyItemsVisibleOnLevel( ITEM_POOL *pItemPool, INT8 bZLevel );


void ExchangeMessageBoxCallBack( UINT8 bExitValue )
{
	if ( bExitValue == MSG_BOX_RETURN_YES )
	{
		SwapMercPositions( gpExchangeSoldier1, gpExchangeSoldier2 );
	}
}


INT8 HandleMoveModeInteractiveClick( INT32 usMapPos, UINT32 *puiNewEvent )
{
	// Look for an item pool
	ITEM_POOL					*pItemPool;
	BOOLEAN						fContinue = TRUE;
	SOLDIERTYPE				*pSoldier;
	LEVELNODE					*pIntTile;
	INT32 sIntTileGridNo;
	INT32 sActionGridNo;
	UINT8							ubDirection;
	INT8							bReturnCode = 0;
	INT8							bZLevel;
	STRUCTURE					*pStructure = NULL;

	if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
	{
		// If we are out of breath, no cursor...
		// if ( pSoldier->bBreath < OKBREATH )
		//{
		//	return( -1 );
		//}

		// ATE: If we are a vehicle, no moving!
		if ( !gGameExternalOptions.fAllowDrivingVehiclesInTactical && pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ VEHICLE_CANT_MOVE_IN_TACTICAL ] );
			return( -3 );
		}

		//if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
		if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_PASSENGER ) )
		{
			// anv: there's some weird bug with moving invisible passengers after loading game, that should solve it
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ VEHICLE_CANT_MOVE_IN_TACTICAL ] );
			// actually, you know what, let's switch to vehicle using the occasion
			HandleLocateSelectMerc( GetDriver(MercPtrs[ pSoldier->ubID ]->iVehicleId)->ubID, 0 );
			return( -3 );
		}

		if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER ) )
		{
			pSoldier = GetSoldierStructureForVehicle( MercPtrs[ pSoldier->ubID ]->iVehicleId );
		}

		// OK, check for height differences.....
		if ( gpWorldLevelData[ usMapPos ].sHeight != gpWorldLevelData[ pSoldier->sGridNo ].sHeight )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANT_GET_THERE ] );
			return( -1 );
		}

		// See if we are over a vehicle, and walk up to it and enter....
		if ( CheckForAndHandleHandleVehicleInteractiveClick( pSoldier, usMapPos, TRUE, 0 ) == -1 )
		{
			return( -1 );
		}

		// Check if we are over a civillian....
		if ( gfUIFullTargetFound )
		{
			if ( ValidQuickExchangePosition( ) )
			{
				// Check if we can...
				if ( CanExchangePlaces( pSoldier, MercPtrs[ gusUIFullTargetID ], TRUE ) )
				{
					gpExchangeSoldier1 = pSoldier;
					gpExchangeSoldier2 = MercPtrs[ gusUIFullTargetID ];

					// Do message box...
					//DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ EXCHANGE_PLACES_REQUESTER ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, ExchangeMessageBoxCallBack, NULL );
					SwapMercPositions( gpExchangeSoldier1, gpExchangeSoldier2 );
				}
			}
			return( -3 );
		}

		pIntTile = GetCurInteractiveTileGridNoAndStructure( &sIntTileGridNo, &pStructure );

		if ( pIntTile != NULL )
		{
			bReturnCode = -3;

			// Check if we are over an item pool, take precedence over that.....
			// EXCEPT FOR SWITCHES!
			if ( GetItemPool( sIntTileGridNo, &pItemPool, pSoldier->pathing.bLevel ) && !( pStructure->fFlags & ( STRUCTURE_SWITCH | STRUCTURE_ANYDOOR ) ) )
			{
				if ( AM_AN_EPC( pSoldier ) )
				{
					// Display message
					// ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ EPC_CANNOT_DO_THAT ] );
					bReturnCode = -1;
				}
				else if ( UIOkForItemPickup( pSoldier, sIntTileGridNo ) )
				{
					bZLevel = GetLargestZLevelOfItemPool( pItemPool );

					if ( AnyItemsVisibleOnLevel( pItemPool, bZLevel ) )
					{
						fContinue = FALSE;

						SetUIBusy( pSoldier->ubID );

						if ( ( gTacticalStatus.uiFlags & INCOMBAT ) && ( gTacticalStatus.uiFlags & TURNBASED ) )
						{
							//puiNewEvent = C_WAIT_FOR_CONFIRM;
							SoldierPickupItem( pSoldier, pItemPool->iItemIndex, sIntTileGridNo, bZLevel );
						}
						else
						{
							BeginDisplayTimedCursor( OKHANDCURSOR_UICURSOR, 300 );

							SoldierPickupItem( pSoldier, pItemPool->iItemIndex, sIntTileGridNo, bZLevel );
						}
					}
				}
			}

			if ( fContinue )
			{
				sActionGridNo =	FindAdjacentGridEx( MercPtrs[ gusSelectedSoldier ], sIntTileGridNo, &ubDirection, NULL, FALSE, TRUE );
				if ( sActionGridNo == -1 )
				{
					sActionGridNo = sIntTileGridNo;
				}

				// If this is not the same tile as ours, check if we can get to dest!
				if ( sActionGridNo != MercPtrs[ gusSelectedSoldier ]->sGridNo && gsCurrentActionPoints == 0 )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
					bReturnCode = -1;
				}
				else
				{
					bReturnCode = -2;
				}
			}
		}
	}

	return( bReturnCode );
}


BOOLEAN HandleUIReloading( SOLDIERTYPE *pSoldier )
{
	INT16 bAPs = 0;

	// CHECK OUR CURRENT CURSOR...

	// Do we have the ammo to reload?
	if ( guiCurrentUICursor == GOOD_RELOAD_UICURSOR )
	{
		// Check APs to reload...
		bAPs = GetAPsToAutoReload( pSoldier );

		if ( EnoughPoints( pSoldier, bAPs, 0,TRUE ) )
		{
			// OK, we have some ammo we can reload.... reload now!
			if ( !AutoReload( pSoldier ) )
			{
				// Do we say we could not reload gun...?
			}

			// ATE: Re-examine cursor info!
			gfUIForceReExamineCursorData = TRUE;
		}
		return( TRUE );
	}

	if ( guiCurrentUICursor == BAD_RELOAD_UICURSOR )
	{
		// OK, we have been told to reload but have no ammo...
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"No ammo to reload." );
		if ( Random( 3 ) == 0 )
		{
			TacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_AMMO );
		}
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN ConfirmActionCancel( INT32 usMapPos, INT32 usOldMapPos )
{
	// OK, most times we want to leave confirm mode if our
	// gridno is different... but if we are in the grenade throw
	// confirm UI, we want a bigger radius...
	//if ( InAimCubeUI( ) )
	//{
	// Calculate distence between both gridnos.....
	//	if ( GetRangeFromGridNoDiff( GetInAimCubeUIGridNo( ), usOldMapPos ) > 1 )
	//if ( sMapPos != usOldMapPos )
	//	{
	//		return( TRUE );
	//	}
	//
	//else
	{
		if ( usMapPos != usOldMapPos )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

void ChangeCurrentSquad( INT32 iSquad )
{
	// only allow if nothing in hand and the Change Squad button for whichever panel we're in must be enabled
	if ( ( gpItemPointer == NULL ) && !gfDisableTacticalPanelButtons &&
		( ( gsCurInterfacePanel != TEAM_PANEL ) || ( ButtonList[ iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
	{
		INT32 cnt, cnt2, iCurrentSquad;
		INT32 cnt3 = -1;

		// Buggler: dynamic tactical squad hotkey assignments only take into account squads present in sector
		//find the actual squad ID present in tactical map
		for( cnt = 0 ; cnt < NUMBER_OF_SQUADS; cnt++ )
		{
			for( cnt2 = 0; cnt2 < NUMBER_OF_SOLDIERS_PER_SQUAD; cnt2++ )
			{
				if ( Squad[ cnt ][ cnt2 ] != NULL && Squad[ cnt ][ cnt2 ]->bInSector && OK_INTERRUPT_MERC( Squad[ cnt ][ cnt2 ] ) && OK_CONTROLLABLE_MERC( Squad[ cnt ][ cnt2 ] ) )
				{
					cnt3++;
					break;
				}
			}

			if ( iSquad == cnt3 )
				break;
		}
		
		if ( IsSquadOnCurrentTacticalMap( cnt ) )
		{
			//resort the squad in case the order has, for any reason, changed
			SortSquadByID( (INT8)cnt );

			iCurrentSquad = CurrentSquad( );
			
			if ( cnt != iCurrentSquad )
			{		
				//set active squad
				SetCurrentSquad( cnt, FALSE );

				if ( gGameExternalOptions.fUseXMLSquadNames && cnt < ON_DUTY )
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE_STRING ], SquadNames[cnt].squadname );
				else
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE ], ( cnt + 1 ) );
				
				ErasePath( TRUE );
				gfPlotNewMovement = TRUE;
			}
		}
	}
}

void HandleSelectMercSlot( UINT8 ubPanelSlot, INT8 bCode )
{
	UINT8 ubID;

	if ( GetPlayerIDFromInterfaceTeamSlot( ubPanelSlot, &ubID ) )
	{
		HandleLocateSelectMerc( ubID, bCode );

		ErasePath( TRUE );
		gfPlotNewMovement = TRUE;
	}
}

void TestMeanWhile( INT32 iID )
{
	MEANWHILE_DEFINITION MeanwhileDef;
	INT32	cnt;
	SOLDIERTYPE *pSoldier;

	MeanwhileDef.sSectorX = gModSettings.ubMeanwhilePalaceSectorX; //3
	MeanwhileDef.sSectorY = gModSettings.ubMeanwhilePalaceSectorY; //16
	MeanwhileDef.ubNPCNumber = QUEEN;
	MeanwhileDef.usTriggerEvent = 0;
	MeanwhileDef.ubMeanwhileID = (UINT8)iID;

	if ( iID == INTERROGATION )
	{
		MeanwhileDef.sSectorX = gModSettings.ubMeanwhileInterrogatePOWSectorX; //7
		MeanwhileDef.sSectorY = gModSettings.ubMeanwhileInterrogatePOWSectorY; //14

		// Loop through our mercs and set gridnos once some found.....
		// look for all mercs on the same team,
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
		{
			// Are we a POW in this sector?
			if ( pSoldier->bActive && pSoldier->bInSector )
			{

				ChangeSoldiersAssignment( pSoldier, ASSIGNMENT_POW );

				pSoldier->sSectorX = gModSettings.ubMeanwhileInterrogatePOWSectorX; //7
				pSoldier->sSectorY = gModSettings.ubMeanwhileInterrogatePOWSectorY; //14
			}
		}
	}

	ScheduleMeanwhileEvent( &MeanwhileDef, 10 );
}

void EscapeUILock( )
{
	//UNLOCK UI
	UnSetUIBusy( (UINT8)gusSelectedSoldier );

	// Decrease global busy	counter...
	gTacticalStatus.ubAttackBusyCount = 0;
#ifdef DEBUG_ATTACKBUSY
	OutputDebugString( "Resetting attack busy due to escape of UI lock.\n");
#endif
	guiPendingOverrideEvent = LU_ENDUILOCK;
	UIHandleLUIEndLock( NULL );
}

#ifdef JA2BETAVERSION
#include "Map Edgepoints.h"
void ToggleMapEdgepoints()
{
#ifdef JA2EDITOR
	static BOOLEAN fToggleEdgepointDisplay = FALSE;
	if( fToggleEdgepointDisplay ^= TRUE )
	{ //Show edgepoints
		ShowMapEdgepoints();
	}
	else
	{ //Hide edgepoints
		HideMapEdgepoints();
	}
	SetRenderFlags( RENDER_FLAG_FULL );
#endif
}
#endif

#ifdef JA2BETAVERSION
BOOLEAN gfMercsNeverQuit = FALSE;
void ToggleMercsNeverQuit()
{
	if( gfMercsNeverQuit )
	{
		gfMercsNeverQuit = FALSE;
		ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Merc contract expiring enabled." );
	}
	else
	{
		gfMercsNeverQuit ^= TRUE;
		ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Merc contract expiring disabled." );
	}
}
#endif

// silversurfer: This function handles switching between prone, crouching and standing stance. Running is handled in function HandleTBSoldierRun().
void HandleStanceChangeFromUIKeys( UINT8 ubAnimHeight )
{
	// If we have multiple guys selected, make all change stance!
	SOLDIERTYPE *		pSoldier;
	INT32						cnt;

	if ( gTacticalStatus.fAtLeastOneGuyOnMultiSelect && !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// OK, loop through all guys who are 'multi-selected' and
		// check if our currently selected guy is amoung the
		// lucky few.. if not, change to a guy who is...
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++ )
		{
			if ( pSoldier->bActive && pSoldier->bInSector )
			{
				if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED )
				{
					// silversurfer: If we decide to stand up or press "s" again while we are standing we should reset movement to walk mode.
					// If we want to run we have to press "r" which is handled elsewhere.
					if ( ubAnimHeight == ANIM_STAND )
					{
						if ( pSoldier->usUIMovementMode != WALKING && pSoldier->usUIMovementMode != RUNNING && pSoldier->usUIMovementMode != WALKING_WEAPON_RDY && pSoldier->usUIMovementMode != WALKING_DUAL_RDY && pSoldier->usUIMovementMode != WALKING_ALTERNATIVE_RDY )
						{
							UIHandleSoldierStanceChange( pSoldier->ubID, ANIM_STAND );
							pSoldier->flags.fUIMovementFast = 0;
						}
						else
						{
							pSoldier->flags.fUIMovementFast = 0;
							pSoldier->usUIMovementMode = WALKING;
							gfPlotNewMovement = TRUE;
						}
					}
					else
						UIHandleSoldierStanceChange( pSoldier->ubID, ubAnimHeight );
				}
			}
		}
	}
	else
	{
		if( gusSelectedSoldier != NOBODY )
		{
			pSoldier = MercPtrs[ (UINT8)gusSelectedSoldier ];

			// silversurfer: If we decide to stand up or press "s" again while we are standing we should reset movement to walk mode.
			// If we want to run we have to press "r" which is handled elsewhere.
			if ( ubAnimHeight == ANIM_STAND )
			{
				if ( pSoldier->usUIMovementMode != WALKING && pSoldier->usUIMovementMode != RUNNING && pSoldier->usUIMovementMode != WALKING_WEAPON_RDY && pSoldier->usUIMovementMode != WALKING_DUAL_RDY && pSoldier->usUIMovementMode != WALKING_ALTERNATIVE_RDY )
				{
					UIHandleSoldierStanceChange( pSoldier->ubID, ANIM_STAND );
					pSoldier->flags.fUIMovementFast = 0;
				}
				else
				{
					pSoldier->flags.fUIMovementFast = 0;
					pSoldier->usUIMovementMode = WALKING;
					gfPlotNewMovement = TRUE;
				}
			}
			else
				UIHandleSoldierStanceChange( pSoldier->ubID, ubAnimHeight );
		}
	}
}

void ToggleStealthMode( SOLDIERTYPE *pSoldier )
{
	// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
	if ( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ giSMStealthButton ]->uiFlags & BUTTON_ENABLED ) )
	{	
		if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			return;
		}

		// ATE: Toggle stealth
		if ( gpSMCurrentMerc != NULL && pSoldier->ubID == gpSMCurrentMerc->ubID )
		{
			gfUIStanceDifferent = TRUE;
		}

		pSoldier->bStealthMode = ! pSoldier->bStealthMode;
		gfPlotNewMovement	= TRUE;
		fInterfacePanelDirty = DIRTYLEVEL2;

		if ( pSoldier->bStealthMode )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_MERC_ON_STEALTHMODE ], pSoldier->GetName() );
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_MERC_OFF_STEALTHMODE ], pSoldier->GetName() );
		}
	}
}


void HandleStealthChangeFromUIKeys(	)
{
	// If we have multiple guys selected, make all change stance!
	SOLDIERTYPE *		pSoldier;
	INT32						cnt;

	if ( gTacticalStatus.fAtLeastOneGuyOnMultiSelect && !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// OK, loop through all guys who are 'multi-selected' and
		// check if our currently selected guy is amoung the
		// lucky few.. if not, change to a guy who is...
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++ )
		{
			if ( pSoldier->bActive && !AM_A_ROBOT( pSoldier ) && pSoldier->bInSector )
			{
				if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED )
				{
					ToggleStealthMode( pSoldier );
				}
			}
		}
	}
	else
	{
		if( gusSelectedSoldier != NOBODY )
		{
			if ( !AM_A_ROBOT( MercPtrs[ gusSelectedSoldier ] ) )
			{
				ToggleStealthMode( MercPtrs[ gusSelectedSoldier ] );
			}
		}
	}
}



void TestCapture( )
{
	INT32 cnt;
	SOLDIERTYPE				*pSoldier;
	UINT32					uiNumChosen = 0;

	//StartQuest( QUEST_HELD_IN_ALMA, gWorldSectorX, gWorldSectorY );
	//EndQuest( QUEST_HELD_IN_ALMA, gWorldSectorX, gWorldSectorY );

	BeginCaptureSquence( );

	gStrategicStatus.uiFlags &= (~STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE );

	// loop through soldiers and pick 3 lucky ones....
	for ( cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID, pSoldier=MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; cnt++, pSoldier++ )
	{
		if ( pSoldier->stats.bLife >= OKLIFE && pSoldier->bActive && pSoldier->bInSector )
		{
			if ( uiNumChosen < 3 )
			{
				EnemyCapturesPlayerSoldier( pSoldier );

				// Remove them from tectical....
				pSoldier->RemoveSoldierFromGridNo( );

				uiNumChosen++;
			}
		}
	}

	EndCaptureSequence( );
}


void PopupAssignmentMenuInTactical( SOLDIERTYPE *pSoldier )
{
	// do something
	fShowAssignmentMenu = TRUE;
	CreateDestroyAssignmentPopUpBoxes( );
	SetTacticalPopUpAssignmentBoxXY( );
	DetermineBoxPositions( );
	DetermineWhichAssignmentMenusCanBeShown( );
	fFirstClickInAssignmentScreenMask = TRUE;
	gfIgnoreScrolling = TRUE;
}

//lal
void PopupMilitiaControlMenu( SOLDIERTYPE *pSoldier )
{
	// do something
	fShowMilitiaControlMenu = TRUE;
	CreateDestroyMilitiaControlPopUpBoxes( );
	SetTacticalPopUpMilitiaControlBoxXY( pSoldier );
	DetermineMilitiaControlBoxPositions();
	DetermineWhichMilitiaControlMenusCanBeShown( );
	fFirstClickInMilitiaControlScreenMask = TRUE;
	gfIgnoreScrolling = TRUE;
}

INT32 PickPocket(MERCPROFILESTRUCT *pProfile, UINT8 ppStart, UINT8 ppStop, UINT16 usItem, UINT8 iNumber, UINT8 * cap);

bool BadGoggles(SOLDIERTYPE *pTeamSoldier)
{
    // WDS - Smart goggle switching
	// NOTE: Investigate using GetItemVisionRangeBonus from Items.cpp???
	if (!gGameExternalOptions.smartGoggleSwitch)
		return false;
	else
	{
		// Look through the head slots and find any sort of goggle.  Check if it is bad for this time of day.
		// silversurfer: also check if we are underground, sun goggles and night vision bonus are useless underground
		for (int headSlot = HEAD1POS; headSlot <= HEAD2POS; ++headSlot)
		{
			// check if we are below ground
			if ( pTeamSoldier->bSectorZ > 0 )
			{
				// we don't want a cave vision penalty
				if ( Item[pTeamSoldier->inv[headSlot].usItem].cavevisionrangebonus < 0 )
					return true;
			}
			
			// if we are above ground
			else
			{
				// at night we don't want a night vision penalty
				if ( NightTime() )
				{
					if ( Item[pTeamSoldier->inv[headSlot].usItem].nightvisionrangebonus < 0 )
						return true;
				}
				// at daytime we don't want a day vision penalty
				// also if we have a bright light penalty make sure that is doesn't exceed the day vision bonus
				else if ( Item[pTeamSoldier->inv[headSlot].usItem].dayvisionrangebonus < 0 || 
					( Item[pTeamSoldier->inv[headSlot].usItem].dayvisionrangebonus + Item[pTeamSoldier->inv[headSlot].usItem].brightlightvisionrangebonus ) < 0 )
					return true;
			}
		}
	}
	return false;
}

void SwapGoggles(SOLDIERTYPE *pTeamSoldier)
{
    // WDS - Smart goggle switching
	// NOTE: Investigate using GetItemVisionRangeBonus from Items.cpp???
	// Look through the head slots and find any sort of goggle or an empty spot
	OBJECTTYPE * pGoggles = 0;
	int slotToUse = -1;
	INT8 swapSlot = -1;
	BOOLEAN isAttach = FALSE;

	for (int headSlot = HEAD1POS; headSlot <= HEAD2POS; ++headSlot) 
	{
		if ( (Item[pTeamSoldier->inv[headSlot].usItem].brightlightvisionrangebonus != 0) ) 
		{
			slotToUse = headSlot;
			break;
		} 
		if ( (Item[pTeamSoldier->inv[headSlot].usItem].dayvisionrangebonus != 0) ) 
		{
			slotToUse = headSlot;
			break;
		} 
		else if ( (Item[pTeamSoldier->inv[headSlot].usItem].nightvisionrangebonus != 0) ) 
		{
			slotToUse = headSlot;
			break;
		} 
		else if ( (Item[pTeamSoldier->inv[headSlot].usItem].cavevisionrangebonus != 0) ) 
		{
			slotToUse = headSlot;
			break;
		} 
		else if (pTeamSoldier->inv[headSlot].exists() == false) 
		{
			slotToUse = headSlot;
		}
	}
	
	if (slotToUse == -1) 
	{
		// No place to swap in a new goggle, give up
		return;
	}

	// Find the best goggles for the current time of day anywhere in inventory
	// silversurfer: also check if underground
	if (DayTime() && pTeamSoldier->bSectorZ == 0) 
	{
		pGoggles = FindSunGogglesInInv( pTeamSoldier, &swapSlot, &isAttach, TRUE );
	} 
	else 
	{
		pGoggles = FindNightGogglesInInv( pTeamSoldier, &swapSlot, &isAttach, TRUE );
	}

	if (pGoggles)
	{
		// We need to check that pGoggles is compatible with whatever is in the other face slot
		int otherFaceSlot = (slotToUse == HEAD1POS?HEAD2POS:HEAD1POS);
		if(pTeamSoldier->inv[otherFaceSlot].exists() == true && !CompatibleFaceItem(pGoggles->usItem, pTeamSoldier->inv[otherFaceSlot].usItem))
			pGoggles = NULL;
	}

	if (pGoggles) 
	{
		// Now either swap or equip the best one that was found
		if (pTeamSoldier->inv[slotToUse].exists()) 
		{
			if(CanItemFitInPosition(pTeamSoldier, &pTeamSoldier->inv[slotToUse], swapSlot, TRUE))
				SwapObjs( pTeamSoldier, slotToUse, pGoggles, TRUE );
			else if((pTeamSoldier->inv[HELMETPOS].exists()== true && pTeamSoldier->inv[HELMETPOS].AttachObject(pTeamSoldier, &pTeamSoldier->inv[slotToUse], FALSE)) ||
				AutoPlaceObject(pTeamSoldier, &pTeamSoldier->inv[slotToUse], FALSE))
				pGoggles->MoveThisObjectTo(pTeamSoldier->inv[slotToUse], 1, pTeamSoldier, slotToUse);
			else
			{
				CHAR16	zTemp[ 100 ];
				swprintf( zTemp, Message[ STR_CANNOT_ATTACH_ANY_SLOT ], ItemNames[ pTeamSoldier->inv[slotToUse].usItem ] );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, zTemp );
			}
		} 
		else 
		{
			pGoggles->MoveThisObjectTo(pTeamSoldier->inv[slotToUse], 1, pTeamSoldier, slotToUse);
		}
	} 
	else 
	{
		// No goggles to equip, should the current ones be unequiped?
		if (pTeamSoldier->inv[slotToUse].exists()) 
		{
			if ((DayTime() && Item[pTeamSoldier->inv[slotToUse].usItem].nightvisionrangebonus > 0 && pTeamSoldier->bSectorZ == 0) ||
			    ((!DayTime() || pTeamSoldier->bSectorZ > 0) && (Item[pTeamSoldier->inv[slotToUse].usItem].brightlightvisionrangebonus > 0))) 
			{
				// It's day and we're wearing night goggles (or vice-versa), find a place to stash them
				if (pTeamSoldier->inv[ HELMETPOS ].exists()) 
				{
					if (pTeamSoldier->inv[ HELMETPOS ].AttachObject( NULL, &pTeamSoldier->inv[slotToUse] )) 
					{
						// It worked!
					} 
					else 
					{
						/* //If we're here we already know we can't attach, don't we?
						// Try dumping it anywhere in inventory because it doesn't attach to the helmet
						if (NASValidAttachment( pTeamSoldier->inv[slotToUse].usItem, pTeamSoldier->inv[HELMETPOS].usItem ) &&
							pTeamSoldier->inv[slotToUse][0]->attachments.size() < MAX_ATTACHMENTS)
						{
							pTeamSoldier->inv[HELMETPOS].AttachObject( pTeamSoldier, &pTeamSoldier->inv[slotToUse], FALSE, 0 );
						}
						else
						*/
						//{
							// Remove sungoggles.
							PlaceInAnyPocket(pTeamSoldier, &pTeamSoldier->inv[slotToUse], FALSE);
						//}
					}
				} 
				else 
				{
					/* //again useless?
					// Try dumping it anywhere in inventory given there's no helemt
					if (NASValidAttachment( pTeamSoldier->inv[slotToUse].usItem, pTeamSoldier->inv[HELMETPOS].usItem ) &&
						pTeamSoldier->inv[slotToUse][0]->attachments.size() < MAX_ATTACHMENTS)
					{
						pTeamSoldier->inv[HELMETPOS].AttachObject( pTeamSoldier, &pTeamSoldier->inv[slotToUse], FALSE, 0 );
					}
					else
					*/
					//{
						// Remove sungoggles.
						PlaceInAnyPocket(pTeamSoldier, &pTeamSoldier->inv[slotToUse], FALSE);
					//}
				}
			}
		}
	}

	fCharacterInfoPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;
	pTeamSoldier->DeleteSoldierLight( );
	pTeamSoldier->PositionSoldierLight( );
}

// HEADROCK HAM B2.8: This function does the same as SwapGoggles, with a twist. It changes all worn goggles in the
// team uniformly, so that everyone is wearing either nightvision or dayvision, regardless of what they were wearing
// before. The check is based on the headgear of the first character in the group (he'll switch, and everyone else
// will switch to match him.

void SwapGogglesUniformly(SOLDIERTYPE *pTeamSoldier, BOOLEAN fToNightVision)
{
	/* CHRISL - Adjusted this option to allow the game to search through Helmet attachments
		as well as inventory positions.
	*/
	OBJECTTYPE * pObj;
	OBJECTTYPE * pGoggles = NULL;
	INT8		bSlot1;
	int			bestBonus;
	bool		itemFound = false;
	INT8 swapSlot = -1;
	BOOLEAN isAttach = FALSE;

	//CHRISL: Before doing anything, we should look at both head slots to see if either slot has some sort of goggles
	for(bSlot1 = HEAD1POS; bSlot1 <= HEAD2POS; bSlot1++)
	{
		if(Item[pTeamSoldier->inv[bSlot1].usItem].brightlightvisionrangebonus > 0)
			itemFound = true;
		if(Item[pTeamSoldier->inv[bSlot1].usItem].nightvisionrangebonus > 0)
			itemFound = true;
	}
	//2 head slots
	for (bSlot1 = HEAD1POS; bSlot1 <= HEAD2POS; bSlot1++)
	{
		// if wearing sungoggles
		if ( Item[pTeamSoldier->inv[bSlot1].usItem].brightlightvisionrangebonus > 0	)
		{
			if (fToNightVision == TRUE) // Only if we want to switch to nightvision!
			{
				itemFound = true;
				bestBonus = 0;
				pGoggles = FindNightGogglesInInv( pTeamSoldier, &swapSlot, &isAttach );
				//search for better goggles on the helmet
				if (pGoggles)
				{
					bestBonus = Item[pGoggles->usItem].nightvisionrangebonus;
				}
				//search helmet and vest
				for(UINT8 gear = HELMETPOS; gear <= VESTPOS; gear++)
				{
					pObj = &(pTeamSoldier->inv[gear]);
					for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
					{
						if ( Item[ iter->usItem ].nightvisionrangebonus > bestBonus && Item[ iter->usItem ].usItemClass == IC_FACE && iter->exists())
						{
							pGoggles = &(*iter);
							bestBonus = Item[ iter->usItem ].nightvisionrangebonus;
						}
					}
				}
				if ( pGoggles )
				{
					SwapObjs( pTeamSoldier, bSlot1, pGoggles, TRUE );
					break;
				}
				else if (Item[pTeamSoldier->inv[bSlot1].usItem].nightvisionrangebonus <= 0)
				{	//WarmSteel - just try and see if it works
					//if (NASValidAttachment( pTeamSoldier->inv[bSlot1].usItem, pTeamSoldier->inv[HELMETPOS].usItem ) &&
					//	pTeamSoldier->inv[bSlot1][0]->attachments.size() < MAX_ATTACHMENTS)
					if ( pTeamSoldier->inv[HELMETPOS].AttachObject( pTeamSoldier, &pTeamSoldier->inv[bSlot1], FALSE, 0 ) )
					{
						break;
					}
					else
					{
						// Remove nightgoggles.
						PlaceInAnyPocket(pTeamSoldier, &pTeamSoldier->inv[bSlot1], FALSE);
						break;
					}
				}
			}
		}
		// else if wearing NVGs
		else if(Item[pTeamSoldier->inv[bSlot1].usItem].nightvisionrangebonus > 0)
		{
			if (fToNightVision == FALSE) // Only if we want to switch to dayvision!
			{
				itemFound = true;
				bestBonus = 0;
				pGoggles = FindSunGogglesInInv( pTeamSoldier, &swapSlot, &isAttach );
				//search for better goggles on the helmet
				if (pGoggles)
				{
					bestBonus = Item[pGoggles->usItem].brightlightvisionrangebonus;
				}
				//search helmet and vest
				for(UINT8 gear = HELMETPOS; gear <= VESTPOS; gear++)
				{
					pObj = &(pTeamSoldier->inv[gear]);
					for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
					{
						if ( Item[ iter->usItem ].brightlightvisionrangebonus > bestBonus && Item[ iter->usItem ].usItemClass == IC_FACE && iter->exists())
						{
							pGoggles = &(*iter);
							bestBonus = Item[ iter->usItem ].brightlightvisionrangebonus;
						}
					}
				}
				if ( pGoggles )
				{
					SwapObjs( pTeamSoldier, bSlot1, pGoggles, TRUE );
					break;
				}
				else if (Item[pTeamSoldier->inv[bSlot1].usItem].brightlightvisionrangebonus <= 0)
				{	//WarmSteel - try
					//if (NASValidAttachment( pTeamSoldier->inv[bSlot1].usItem, pTeamSoldier->inv[HELMETPOS].usItem ) &&
					//	pTeamSoldier->inv[bSlot1][0]->attachments.size() < MAX_ATTACHMENTS)
					if ( pTeamSoldier->inv[HELMETPOS].AttachObject( pTeamSoldier, &pTeamSoldier->inv[bSlot1], FALSE, 0 ) )
					{
						break;
					}
					else
					{
						// Remove nightgoggles.
						PlaceInAnyPocket(pTeamSoldier, &pTeamSoldier->inv[bSlot1], FALSE);
						break;
					}
				}
			}
		}
		// else if not wearing anything and no goggles found
		else if(itemFound == false && pTeamSoldier->inv[bSlot1].exists() == false)
		{
			bestBonus = 0;
			// search helmet and vest for goggles of some kind
			for(UINT8 gear = HELMETPOS; gear <= VESTPOS; gear++)
			{
				pObj = &(pTeamSoldier->inv[gear]);
				for(attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
				{
					if(iter->exists()){
						if(fToNightVision == FALSE && Item[iter->usItem].brightlightvisionrangebonus > bestBonus && Item[iter->usItem].usItemClass == IC_FACE)
						{
							pGoggles = &(*iter);
							bestBonus = Item[iter->usItem].brightlightvisionrangebonus;
						}
						else if(fToNightVision == TRUE && Item[iter->usItem].nightvisionrangebonus > bestBonus && Item[iter->usItem].usItemClass == IC_FACE)
						{
							pGoggles = &(*iter);
							bestBonus = Item[iter->usItem].nightvisionrangebonus;
						}
					}
				}
				if(pGoggles)
				{
					pGoggles->MoveThisObjectTo(pTeamSoldier->inv[bSlot1], 1, pTeamSoldier, bSlot1);
					pObj->RemoveAttachment(pGoggles);
					break;
				}
			}
			if(pTeamSoldier->inv[bSlot1].exists() == false)
			{
				if(fToNightVision == FALSE)
					pGoggles = FindSunGogglesInInv( pTeamSoldier, &swapSlot, &isAttach );
				else
					pGoggles = FindNightGogglesInInv( pTeamSoldier, &swapSlot, &isAttach );
				if(pGoggles)
				{
					SwapObjs( pTeamSoldier, bSlot1, pGoggles, TRUE );
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	fCharacterInfoPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;
	pTeamSoldier->DeleteSoldierLight( );
	pTeamSoldier->PositionSoldierLight( );
}

void SeperateItems()
{
	if ( !(gTacticalStatus.fEnemyInSector) )
	{
		HandleAllReachAbleItemsInTheSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		for ( UINT32 uiLoop = 0; uiLoop < guiNumWorldItems; uiLoop++ ) //for all items in sector
		{
			if ( (gWorldItems[ uiLoop ].bVisible == TRUE) && (gWorldItems[ uiLoop ].fExists) && (gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_ARMED_BOMB) )//item exists, is reachable, is visible and is not trapped
			{
				for (int x = 0; x < gWorldItems[ uiLoop ].object.ubNumberOfObjects; ++x) {
					if (( Item[ gWorldItems[ uiLoop ].object.usItem ].usItemClass == IC_GUN ) && (gGameExternalOptions.gfShiftFUnloadWeapons == TRUE) )//item is a gun and unloading is allowed
					{
						//Remove magazine
						if ( (gWorldItems[ uiLoop ].object[x]->data.gun.usGunAmmoItem != NONE) && (gWorldItems[ uiLoop ].object[x]->data.gun.ubGunShotsLeft > 0) )
						{
							CreateAmmo(gWorldItems[ uiLoop ].object[x]->data.gun.usGunAmmoItem, &gTempObject, gWorldItems[ uiLoop ].object[x]->data.gun.ubGunShotsLeft);
							gWorldItems[ uiLoop ].object[x]->data.gun.ubGunShotsLeft = 0;
							gWorldItems[ uiLoop ].object[x]->data.gun.usGunAmmoItem = NONE;
							// HEADROCK HAM 3.5: Clear ammo type
							gWorldItems[ uiLoop ].object[x]->data.gun.ubGunAmmoType = NONE;

							// put it on the ground
							AddItemToPool( gWorldItems[ uiLoop ].sGridNo, &gTempObject, 1, gWorldItems[ uiLoop ].ubLevel, WORLD_ITEM_REACHABLE , -1 );
						}
					}

					//remove attachments
					//WarmSteel - This causes a crash when unloading attachments, then sorting, then selling all items in the sector in the map screen.
					//It is likely to be caused here because when i accidentally broke this part the bug didn't occur.
					//TODO : fix this.
					if ( gGameExternalOptions.gfShiftFRemoveAttachments == TRUE )
					{
						//CHRISL: We run into a problem here because GetFreeWorldItemIndex, which gets called 
						//	from AddItemToPool, resets gWorldItems when it increases it's size.  This means
						//	iter loses it's relationship which causes a CTD if we use this hotkey and there
						//	aren't enough open WorldItems to accomodate all the attachments we're seperating.
						UINT8 size = 0, cnt = 0, uiLoopCnt = 0;
						// uiLoopCnt is an extra failsafe.  I think I've already managed to eliminate the
						//	infinite loop, but just in case, we'll use uiLoopCnt to force a break after a
						//	certain point.

						// Iterate backwards through attachments in order to detach grenades from underbarrel
						// launchers before detaching the launchers (and their grenade slots) themselves.
						// Since the size of the attachmentList remains constant under NAS but decrements by one
						// under OAS, recheck the list size every iteration in order to calculate an rindex.
						while((size = gWorldItems[uiLoop].object[x]->attachments.size()) != cnt)
						{
							gTempObject = *gWorldItems[uiLoop].object[x]->GetAttachmentAtIndex(size - 1 - cnt);

							//WarmSteel - This actually still works with NAS, be it by accident
							if (gWorldItems[ uiLoop ].object.RemoveAttachment(&gTempObject,0,x))
							{
								AddItemToPool( gWorldItems[ uiLoop ].sGridNo, &gTempObject, 1, gWorldItems[ uiLoop ].ubLevel, WORLD_ITEM_REACHABLE , -1 );
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ ATTACHMENT_REMOVED ] );
							}
							else
							{
								cnt++;
							}

							uiLoopCnt ++;
							if(uiLoopCnt > 100)
								break;
						}
					}
					
					// Bob: also empty out LBE items in sector inventory
					if (gWorldItems[uiLoop].object.IsActiveLBE(x)) {
						LBENODE * lbePtr = gWorldItems[uiLoop].object.GetLBEPointer(x);

						for (auto lbeInvIter = lbePtr->inv.begin(); lbeInvIter != lbePtr->inv.end(); lbeInvIter++) {
							OBJECTTYPE * LBEStack = lbeInvIter._Ptr;
							
							if (LBEStack->usItem != 0 && LBEStack->exists()) {
								AddItemToPool(gWorldItems[uiLoop].sGridNo, LBEStack, 1, gWorldItems[uiLoop].ubLevel, WORLD_ITEM_REACHABLE, -1);
							}
						}
						DestroyLBE(&(gWorldItems[uiLoop].object), x);
					}
				
				}
			}
		}
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pImpButtonText[11] );
	}
}

void StackAndSort( BOOLEAN fRestrictToAmmo )
{
	if (! ( gTacticalStatus.fEnemyInSector ) )
	{
		HandleAllReachAbleItemsInTheSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

		for ( UINT32 uiLoop = 0; uiLoop < guiNumWorldItems; uiLoop++ )
		{
			if ( (gWorldItems[ uiLoop ].bVisible == TRUE) && (gWorldItems[ uiLoop ].fExists) && (gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_ARMED_BOMB) )//item exists, is reachable, is visible and is not trapped
			{
				//start by putting all identical items into one large pile regardless of stacking limits so we can merge them
				WORLDITEM dummyItem = gWorldItems[uiLoop];
				RemoveItemFromPool(gWorldItems[uiLoop].sGridNo,(uiLoop),gWorldItems[uiLoop].ubLevel);
				RemoveItemFromWorld(uiLoop);
				for(UINT32 i = uiLoop+1; i < guiNumWorldItems; i++)
				{
					if(dummyItem.object.usItem == gWorldItems[i].object.usItem && gWorldItems[i].bVisible == TRUE && gWorldItems[i].fExists && (gWorldItems[i].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[i].usFlags & WORLD_ITEM_ARMED_BOMB))
					{
						dummyItem.object.AddObjectsToStack(gWorldItems[i].object,-1,NULL,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT,false);
						if(gWorldItems[i].object.exists() == false && gWorldItems[i].fExists)
						{
							RemoveItemFromPool(gWorldItems[i].sGridNo,i,gWorldItems[i].ubLevel);
							RemoveItemFromWorld(i);
						}
					}
				}

				//merge items in stack
				if(fRestrictToAmmo == FALSE || Item[dummyItem.object.usItem].usItemClass == IC_AMMO)
					CleanUpStack(&dummyItem.object, NULL);

				//return item to world
				AddItemToPool(dummyItem.sGridNo, &dummyItem.object, dummyItem.bVisible, dummyItem.ubLevel, dummyItem.usFlags, dummyItem.bRenderZHeightAboveLevel, dummyItem.soldierID);

				//find out how many items can be put in a big slot
				UINT8 ubSlotLimit = ItemSlotLimit( &gWorldItems[ uiLoop ].object, STACK_SIZE_LIMIT );

/*				//if we still have some space
				INT32 i = 0;
				while ( gWorldItems[ uiLoop ].object.ubNumberOfObjects < ubSlotLimit )
				{
					i++;
					//if the next item is the same
					if ( gWorldItems[ uiLoop ].object.usItem == gWorldItems[ uiLoop + i ].object.usItem )
					{
						// Clean up ammo stacks prior to trying to merge them
						if(fRestrictToAmmo == FALSE || Item[gWorldItems[uiLoop].object.usItem].usItemClass == IC_AMMO)
						{
							CleanUpStack(&gWorldItems[uiLoop].object, NULL);
							CleanUpStack(&gWorldItems[uiLoop+i].object, NULL);
						}
						INT8 ubObjCount = ubSlotLimit - gWorldItems[ uiLoop ].object.ubNumberOfObjects;
						INT8 bPointsToMove = __min( ubObjCount, gWorldItems[ uiLoop + i ].object.ubNumberOfObjects );

						gWorldItems[ uiLoop ].object.AddObjectsToStack( (gWorldItems[ uiLoop + i ].object), bPointsToMove);
						if(fRestrictToAmmo == FALSE || Item[gWorldItems[uiLoop].object.usItem].usItemClass == IC_AMMO)
							CleanUpStack(&gWorldItems[uiLoop].object, NULL);
						//CHRISL: After adding the object to the stack, we need to delete the old object from
						//	both gWorldItems and the ItemPool otherwise we cause a CTD.
						if(gWorldItems[uiLoop+i].object.exists() == false && gWorldItems[ uiLoop ].fExists)
						{
							RemoveItemFromPool(gWorldItems[uiLoop+i].sGridNo,(uiLoop+i),gWorldItems[uiLoop+i].ubLevel);
							RemoveItemFromWorld(uiLoop+i);
						}
					}
					//CHRISL: by changing this to a condition, we can use this hotkey immediately after the
					//	SHITFT+F hotkey.  Otherwise, if a stackable item ends up at the end of the inventory
					//	list, we won't be able to add it to the initial stack.
					else if(uiLoop+i == guiNumWorldItems)
					{
						break;
					}
				}*/
			}
		}
		NotifySoldiersToLookforItems( );
		//HandleAllReachAbleItemsInTheSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
	}
}



void	QueryTBMButton( UINT32 *puiNewEvent )
{
	INT32	sMapPos;

		if (!GetMouseMapPos( &sMapPos ) )
			return;
		
		if (gViewportRegion.ButtonState & MSYS_MIDDLE_BUTTON) // MID MOUSE BUTTON
		{
			if ( !fMiddleButtonDown )
			{
				fMiddleButtonDown = TRUE;
				//RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );
			}
		}
		else if ( fMiddleButtonDown )
		{
		if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
			{
			if(gGameExternalOptions.bAlternateMouseCommands)
				HandleAltMouseTBMButton( puiNewEvent );
			else
				HandleMouseTBMButton( puiNewEvent );
				}
			fMiddleButtonDown = FALSE;
			// Reset counter
			//RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );
		}
}//void	QueryTBMButton( UINT32 *puiNewEvent )

void	QueryTBWheel( UINT32 *puiNewEvent )
{
	SOLDIERTYPE	*pSoldier;
	INT32		usMapPos=0;

	gViewportRegion.WheelState = gViewportRegion.WheelState * ( gGameSettings.fOptions[TOPTION_INVERT_WHEEL] ? -1 : 1 );
	// stub
	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		if ( gViewportRegion.WheelState != 0 )
		{
			//printf("wheel %d\n", gViewportRegion.WheelState);
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"wheel %d", gViewportRegion.WheelState);
			if ( gpItemPointer == NULL )
			{
				// ATE:
				if ( gusSelectedSoldier != NOBODY )
				{
					// Switch on UI mode
					switch( gCurrentUIMode )
					{

						case MOVE_MODE:
						case CONFIRM_MOVE_MODE:
						case HANDCURSOR_MODE:
							if(gGameExternalOptions.bAlternateMouseCommands)
								HandleAltMouseTBWheel();
										else
								HandleMouseTBWheel();
						case IDLE_MODE:
						case ACTION_MODE:
						case LOCKOURTURN_UI_MODE:
						case LOOKCURSOR_MODE:
						case LOCKUI_MODE:
						case TALKCURSOR_MODE:
						case MENU_MODE:						
							break;
						case CONFIRM_ACTION_MODE:
								if ( GetSoldier( &pSoldier, gusSelectedSoldier ) )
								{
									if(	gGameExternalOptions.bAimedBurstEnabled )
										HandleWheelAdjustCursor( pSoldier, usMapPos, -gViewportRegion.WheelState, brstmode );
									else
										HandleWheelAdjustCursorWOAB( pSoldier, usMapPos, -gViewportRegion.WheelState);
								}
								break;
					}
				}
			}
			ResetWheelState( &gViewportRegion );
		}
	}
}

void QueryTBX1Button( UINT32 *puiNewEvent  )
{
	INT32	sMapPos;
	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		if (!GetMouseMapPos( &sMapPos ) )
			return;
		
		if (gViewportRegion.ButtonState & MSYS_X1_BUTTON) // MID MOUSE BUTTON
		{
			if ( !fX1ButtonDown )
			{
				fX1ButtonDown = TRUE;
				//RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );
			}
		}
		else
		{
			if ( fX1ButtonDown )
			{
			//*puiNewEvent = LC_LOOK;
			fX1ButtonDown = FALSE;

				if(gGameExternalOptions.bAlternateMouseCommands)
					HandleAltMouseTBX1Button( puiNewEvent );
				else
					HandleMouseTBX1Button( puiNewEvent );
			}
		}
	}
}

void QueryTBX2Button( UINT32 *puiNewEvent  )
{
	INT32	sMapPos;
	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
			{
		if (!GetMouseMapPos( &sMapPos ) )
			return;
		
		if (gViewportRegion.ButtonState & MSYS_X2_BUTTON) // MID MOUSE BUTTON
		{
			if ( !fX2ButtonDown )
			{
				fX2ButtonDown = TRUE;
				//RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );
			}
		}
		else if ( fX2ButtonDown )
			{
			fX2ButtonDown = FALSE;
			if(gGameExternalOptions.bAlternateMouseCommands)
				HandleAltMouseTBX2Button( puiNewEvent );
			else
				HandleMouseTBX2Button( puiNewEvent );
		}
	}
}

//silversurfer: this allows swapping of merc portraits in tactical screen using CTRL+LEFTARROW / CTRL+RIGHTARROW
void SwapMercPortraits ( SOLDIERTYPE *pSoldier, INT8 bDirection )
{
	// only swap mercs when the Team Panel is active
	if ( gsCurInterfacePanel != TEAM_PANEL )
		return;

	// if someone is talking don't swap!
	if ( gpCurrentTalkingFace != NULL )
		return;

	UINT8 ubSourceMerc = (UINT8)gusSelectedSoldier;
	UINT8 ubTargetMerc;
	INT32 iSourceFace;
	INT32 iTargetFace;
	UINT8 ubGroupID = pSoldier->ubGroupID;
	INT8 bOldPosition = GetTeamSlotFromPlayerID ( MercPtrs[ ubSourceMerc ]->ubID );
	INT8 bNewPosition = bOldPosition + bDirection;
	SOLDIERTYPE TempMenptr = Menptr[ ubSourceMerc ];

	// anv: vehicle passengers are swapped differently
	if( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
	{
		SOLDIERTYPE *pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );
		if( pVehicle != NULL && bNewPosition < gNewVehicle[ pVehicleList[ pVehicle->bVehicleID ].ubVehicleType ].iNewSeatingCapacities )
		{
			if( SwapVehicleSeat( pVehicle, pSoldier, bNewPosition ) )
			{
				RebuildCurrentSquad( );
				// refresh interface
				fCharacterInfoPanelDirty = TRUE;
				fTeamPanelDirty = TRUE;
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}
		return;
	}

	// check if new position is occupied by another merc? we won't replace an empty slot
	if ( gTeamPanel[ bNewPosition ].fOccupied && gTeamPanel[ bNewPosition ].ubID != NOBODY )
	{
		ubTargetMerc = gTeamPanel[ bNewPosition ].ubID;

		// Hey, you're dead. I don't want to swap with you.
		if ( MercPtrs[ubTargetMerc]->stats.bLife <= 0 )
		{
			RebuildCurrentSquad( );
			return;
		}

		// store face indexes
		iSourceFace = MercPtrs[ ubSourceMerc ]->iFaceIndex;
		iTargetFace = MercPtrs[ ubTargetMerc ]->iFaceIndex;
		FACETYPE TempFace = gFacesData[ iSourceFace ];

		// swap the data
		Menptr[ ubSourceMerc ] = Menptr[ ubTargetMerc ];
		Menptr[ ubTargetMerc ] = TempMenptr; 
		// also swap face data, otherwise face gear, opp count etc won't update
		gFacesData[ iSourceFace ] = gFacesData[ iTargetFace ];
		gFacesData[ iTargetFace ] = TempFace;

		// update IDs in the data so they match array index again
		Menptr[ ubSourceMerc ].ubID = ubSourceMerc;
		Menptr[ ubTargetMerc ].ubID = ubTargetMerc;
		gFacesData[ iSourceFace ].iID = iSourceFace;
		gFacesData[ iTargetFace ].iID = iTargetFace;

		// update soldier ID so they match merc index again
		gFacesData[ iSourceFace ].ubSoldierID = ubSourceMerc;
		gFacesData[ iTargetFace ].ubSoldierID = ubTargetMerc;

		// update face index in merc data
		Menptr[ ubSourceMerc ].iFaceIndex = iSourceFace;
		Menptr[ ubTargetMerc ].iFaceIndex = iTargetFace;

		// update group info
		RemovePlayerFromGroup( ubGroupID, MercPtrs[ ubSourceMerc ] );
		RemovePlayerFromGroup( ubGroupID, MercPtrs[ ubTargetMerc ] );
		AddPlayerToGroup( ubGroupID, MercPtrs[ ubSourceMerc ] );
		AddPlayerToGroup( ubGroupID, MercPtrs[ ubTargetMerc ] );
		SortSquadByID( MercPtrs[ ubSourceMerc ]->bAssignment );
		RebuildCurrentSquad( );

		// don't forget to renew selection of merc
		gusSelectedSoldier = ubTargetMerc;

		// refresh interface
		fCharacterInfoPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fInterfacePanelDirty = DIRTYLEVEL2;
	}
}


// sevenfm: new mouse commands
void HandleAltMouseTBWheel( void )
{
	if (!(gTacticalStatus.uiFlags & ENGAGED_IN_CONV) &&
		((gsCurInterfacePanel != SM_PANEL) || (ButtonList[iSMPanelButtons[NEXTMERC_BUTTON]]->uiFlags & BUTTON_ENABLED)))
	{
		if (gViewportRegion.WheelState > 0)	// wheel up
		{
			if (_KeyDown(ALT))
			if (_KeyDown(SHIFT))
			{
				if (_KeyDown(CTRL))			// SHIFT+CTRL+ALT
					;
				else							// SHIFT+ALT
					;
			}
			else if (_KeyDown(CTRL))		// CTRL+ALT
				HandleTBPickUpBackpacks();
			else								// ALT
				HandleTBGotoHigherStance();
			else if (_KeyDown(CTRL))
			{
				if (_KeyDown(SHIFT))				// SHIFT+CTRL
					;
				else								// CTRL
					HandleTBLocatePrevMerc();
			}
			else if (_KeyDown(SHIFT))			// SHIFT
				HandleTBCycleThroughVisibleEnemiesBackward();
			else
				;
		}
		else										// wheel down
		{
			if (_KeyDown(ALT))
			if (_KeyDown(SHIFT))
			{
				if (_KeyDown(CTRL))			// SHIFT+CTRL+ALT
					;
				else							// SHIFT+ALT
					;
			}
			else if (_KeyDown(CTRL))		// CTRL+ALT
				HandleTBDropBackpacks();
			else								// ALT
				HandleTBGotoLowerStance();
			else if (_KeyDown(CTRL))
			{
				if (_KeyDown(SHIFT))				// SHIFT+CTRL
					;
				else								// CTRL
					HandleTBLocateNextMerc();
			}
			else if (_KeyDown(SHIFT))			// SHIFT
				HandleTBCycleThroughVisibleEnemies();
			else
				;
		}
	}
}

void HandleAltMouseTBMButton(UINT32 *puiNewEvent)
{
	INT32 usMapPos;
	GetMouseMapPos(&usMapPos);

	if (_KeyDown(ALT))
	if (_KeyDown(SHIFT))
	{
		if (_KeyDown(CTRL))			// SHIFT+CTRL+ALT
			;
		else							// SHIFT+ALT
			;// reserved
	}
	else if (_KeyDown(CTRL))		// CTRL+ALT
		HandleTacticalReload();
	else								// ALT
		HandleTBToggleFireMode();
	else if (_KeyDown(CTRL))
	{
		if (_KeyDown(SHIFT))				// SHIFT+CTRL
			;
		else								// CTRL
			SetScopeMode(usMapPos);
	}
	else if (_KeyDown(SHIFT))			// SHIFT
		HandleTacticalTransformItem();
	else									// Button
		HandleTBLook(puiNewEvent);
}

void HandleAltMouseTBX1Button(UINT32 *puiNewEvent)
{
	if (_KeyDown(ALT))
	if (_KeyDown(SHIFT))
	{
		if (_KeyDown(CTRL))		// SHIFT+CTRL+ALT
			;
		else						// SHIFT+ALT
			;
	}
	else if (_KeyDown(CTRL))	// CTRL+ALT
		ToggleTrapNetworkView();
	else							// ALT
		HandleTBSwapGunsling();
	else if (_KeyDown(CTRL))
	{
		if (_KeyDown(SHIFT))			// SHIFT+CTRL
			;
		else							// CTRL
			HandleTBSwapHands();
	}
	else if (_KeyDown(SHIFT))		// SHIFT
		;
	else								// Button
		HandleTBShowLOS();
}

void HandleAltMouseTBX2Button(UINT32 *puiNewEvent)
{
	if (_KeyDown(ALT))
	if (_KeyDown(SHIFT))
	{
		if (_KeyDown(CTRL))		// SHIFT+CTRL+ALT
			;
		else						// SHIFT+ALT
			;
	}
	else if (_KeyDown(CTRL))	// CTRL+ALT
		ToggleHostileTrapsView();
	else							// ALT
		HandleTBToggleFormation();
	else if (_KeyDown(CTRL))
	{
		if (_KeyDown(SHIFT))			// SHIFT+CTRL
			;
		else							// CTRL
			HandleTBJump();
	}
	else if (_KeyDown(SHIFT))		// SHIFT
		HandleTBJumpThroughWindow();
	else								// Button
		HandleTBShowCover();
}

// sevenfm: these functions keep original mouse code functionality
void HandleMouseTBWheel( void )
{
	UINT8		bID;

	if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )	&&
		( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ NEXTMERC_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
	{
		if ( gViewportRegion.WheelState > 0 )
		{
			//change stance ->DOWN
			if ( _KeyDown( ALT ) )
			{	if ( (gusSelectedSoldier != NOBODY) && ( gpItemPointer == NULL ) )
				GotoLowerStance(MercPtrs[ gusSelectedSoldier ]);
				return;
			}
			if ( gusSelectedSoldier != NOBODY )
			{ 
				bID = FindPrevActiveAndAliveMerc( MercPtrs[ gusSelectedSoldier ], TRUE, TRUE );
				HandleLocateSelectMerc( bID, LOCATEANDSELECT_MERC );
				// Center to guy....
				LocateSoldier( gusSelectedSoldier, SETLOCATOR );
			}
		}
							else
							{

			//change stance ->UP
			if ( _KeyDown( ALT ) )
			{	if ( (gusSelectedSoldier != NOBODY) && ( gpItemPointer == NULL ) )
					GotoHeigherStance( MercPtrs[ gusSelectedSoldier ] );
				return;
							}

			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"wheel %d", gViewportRegion.WheelState);
			if ( gusSelectedSoldier != NOBODY )
			{ //Select next merc
				bID = FindNextMercInTeamPanel( MercPtrs[ gusSelectedSoldier ], FALSE, FALSE );
				HandleLocateSelectMerc( bID, LOCATEANDSELECT_MERC );
				// Center to guy....
				LocateSoldier( gusSelectedSoldier, SETLOCATOR );
			}
	}
	//*puiNewEvent = M_ON_TERRAIN; ????????????????
}
}

void HandleMouseTBMButton(  UINT32 *puiNewEvent )
{
		if ( _KeyDown( ALT ) )
							{
			// toggle fire mode
			if ( ( gpItemPointer == NULL ) &&
				( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
				SetBurstMode();
							}
		else
		{
			HandleTBLook( puiNewEvent );
						}
}

void HandleMouseTBX1Button( UINT32 *puiNewEvent )
{			
		if ( !_KeyDown( ALT ) && !_KeyDown( SHIFT ))
		{
			UIHandleChangeLevel( NULL );
					}
		else if( _KeyDown( SHIFT ) )
		{
			HandleTBJumpThroughWindow();
				}
			else if (_KeyDown( ALT ) )
			{
			HandleTBJump();
		}	
}
void HandleMouseTBX2Button( UINT32 *puiNewEvent )
{
	if ( _KeyDown( ALT ) )
		AutoReload( MercPtrs[ gusSelectedSoldier ] );
	else
		HandleTBToggleStealthAll();
}

// sevenfm: common TurnBased functionality
void HandleTBLook( UINT32 *puiNewEvent )
{
	//*puiNewEvent = LC_LOOK;
	if(gCurrentUIMode == CONFIRM_ACTION_MODE)
		brstmode=(brstmode==1)?0:1;
	else
	{
		ErasePath( TRUE );
		*puiNewEvent = LC_ON_TERRAIN;
	}
}
void HandleTBJump( void )
{
				SOLDIERTYPE *pjSoldier;
				if ( GetSoldier( &pjSoldier, gusSelectedSoldier ) )
				{
					INT16							sAPCost;
					INT16							sBPCost;
					BOOLEAN	fNearHeigherLevel;
					BOOLEAN	fNearLowerLevel;
					INT8	bDirection;

					// Make sure the merc is not collapsed!
					if (!IsValidStance(pjSoldier, ANIM_CROUCH) )
					{
						if ( pjSoldier->bCollapsed && pjSoldier->bBreath < OKBREATH )
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, gzLateLocalizedString[ 4 ], pjSoldier->GetName() );
						return;
					}

					// Climb on roofs
					GetMercClimbDirection( pjSoldier->ubID, &fNearLowerLevel, &fNearHeigherLevel );

					if ( fNearLowerLevel )
					{
						// CHRISL: Turn off manual jumping while wearing a backpack
						if (UsingNewInventorySystem() == true && pjSoldier->inv[BPACKPOCKPOS].exists() == true
							//JMich.BackpackClimb
							&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
							&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))

							return;

						if ( EnoughPoints( pjSoldier, GetAPsToClimbRoof( pjSoldier, TRUE ), GetBPsToClimbRoof( pjSoldier, TRUE ), FALSE )	)
						{
							pjSoldier->BeginSoldierClimbDownRoof( );
						}
					}
					
					if ( fNearHeigherLevel )
					{
						// No climbing when wearing a backpack!
						if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
							//JMich.BackpackClimb
							&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
							&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))

							return;

						if ( EnoughPoints( pjSoldier, GetAPsToClimbRoof( pjSoldier, FALSE ), GetBPsToClimbRoof( pjSoldier, FALSE ), FALSE )	)
						{
							pjSoldier->BeginSoldierClimbUpRoof(	);
						}
					}
					
					// Jump over fence
					if ( FindFenceJumpDirection( pjSoldier, pjSoldier->sGridNo, pjSoldier->ubDirection, &bDirection ) )
					{
						if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
							//JMich.BackpackClimb
							&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
							&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))

						{
							sAPCost = GetAPsToJumpFence( pjSoldier, TRUE );
							sBPCost = GetBPsToJumpFence( pjSoldier, TRUE );
						}
						else
						{
							sAPCost = GetAPsToJumpFence( pjSoldier, FALSE );
							sBPCost = GetBPsToJumpFence( pjSoldier, FALSE );
						}

						if ( EnoughPoints( pjSoldier, sAPCost, sBPCost, FALSE )	)
						{
							pjSoldier->BeginSoldierClimbFence(	);
						}
					}

					// Climb on walls
					if (gGameExternalOptions.fCanClimbOnWalls == TRUE)
					{ 
						if ( FindWallJumpDirection( pjSoldier, pjSoldier->sGridNo, pjSoldier->ubDirection, &bDirection ) )
						{
							// No climbing when wearing a backpack!
							if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
								//JMich.BackpackClimb
								&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
								&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))

								return;

							if ( EnoughPoints( pjSoldier, GetAPsToJumpWall( pjSoldier, FALSE ), GetBPsToJumpWall( pjSoldier, FALSE ), FALSE )	)
							{
								pjSoldier->BeginSoldierClimbWall(  );
							}
						}
					}
				}
}
void HandleTBJumpThroughWindow( void ){
	// WANNE: Jump through window?
	if (gGameExternalOptions.fCanJumpThroughWindows == TRUE )
	{
		INT16		sAPCost;
		INT16		sBPCost;
		SOLDIERTYPE *pjSoldier;
		INT8	bDirection;

		if ( GetSoldier( &pjSoldier, gusSelectedSoldier ) )
	{
			if ( FindWindowJumpDirection( pjSoldier, pjSoldier->sGridNo, pjSoldier->ubDirection, &bDirection ) )
		{
				if ((UsingNewInventorySystem() == true) && pjSoldier->inv[BPACKPOCKPOS].exists() == true
					//JMich.BackpackClimb
					&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
					&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))

			{
					sAPCost = GetAPsToJumpThroughWindows( pjSoldier, TRUE );
					sBPCost = GetBPsToJumpThroughWindows( pjSoldier, TRUE );
			}
				else
				{
					sAPCost = GetAPsToJumpFence( pjSoldier, FALSE );
					sBPCost = GetBPsToJumpFence( pjSoldier, FALSE );
		}
				if (EnoughPoints(pjSoldier, sAPCost, sBPCost, FALSE))
		{
					pjSoldier->BeginSoldierClimbWindow();
				}
			}
		}
	}
}
void HandleTBToggleStealthAll( void )
{
			// Toggle squad's stealth mode.....
			// For each guy on squad...
				SOLDIERTYPE				*pTeamSoldier;
				INT8					bLoop;
				BOOLEAN					fStealthOn = FALSE;

				// Check if at least one guy is on stealth....
				for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
				{
					if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) )
					{
						if ( pTeamSoldier->bStealthMode )
							fStealthOn = TRUE;
						}
					}

				fStealthOn = !fStealthOn;

				for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
				{
					if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
					{
						if ( gpSMCurrentMerc != NULL && bLoop == gpSMCurrentMerc->ubID )
							gfUIStanceDifferent = TRUE;
						pTeamSoldier->bStealthMode = fStealthOn;
					}
				}

				fInterfacePanelDirty = DIRTYLEVEL2;

				// OK, display message
				if ( fStealthOn )
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ON_STEALTHMODE ] );
				else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_OFF_STEALTHMODE ] );
}
void HandleTBToggleFireMode( void )
{
	// toggle fire mode
	if ( ( gpItemPointer == NULL ) &&
		( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
		SetBurstMode();
}
void HandleTBSelectAllMercs( void )
{
	SOLDIERTYPE *		pSoldier;
	INT32						cnt;

	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++ )
				{

		// Check if this guy is OK to control....
		if ( OK_CONTROLLABLE_MERC( pSoldier ) && !( pSoldier->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_PASSENGER | SOLDIER_DRIVER ) ) )
		{
			pSoldier->flags.uiStatusFlags |= SOLDIER_MULTI_SELECTED;
				}
			}
	EndMultiSoldierSelection( TRUE );
}
void HandleTBCycleThroughKnownEnemies( void )
{
	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
	//	if ( (InputEvent.usEvent == KEY_DOWN ) && InputEvent.usParam == ENTER	)
			CycleThroughKnownEnemies( );
		}
}
void HandleTBCycleThroughKnownEnemiesBackward( void )
{
	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
	//	if ( (InputEvent.usEvent == KEY_DOWN ) && InputEvent.usParam == ENTER	)
			CycleThroughKnownEnemies( TRUE );
	}
}
void HandleTBCycleThroughVisibleEnemies( void )
{
	SOLDIERTYPE *pSoldier;
		
	if ( gusSelectedSoldier != NOBODY )
	{
		pSoldier = MercPtrs[ gusSelectedSoldier ];

		if ( pSoldier->aiData.bOppCnt > 0 )
		{
			// Cycle....
			CycleVisibleEnemies( pSoldier );
	}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_ENEMIES_IN_SIGHT_STR ] );
		}
	}
}
void HandleTBCycleThroughVisibleEnemiesBackward( void )
{
	SOLDIERTYPE *pSoldier;
	
	if ( gusSelectedSoldier != NOBODY )
	{
		pSoldier = MercPtrs[ gusSelectedSoldier ];

		if ( pSoldier->aiData.bOppCnt > 0 )
		{
			// Cycle....
			CycleVisibleEnemiesBackward( pSoldier );
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_ENEMIES_IN_SIGHT_STR ] );
		}
	}
}
void HandleTBLocateSoldier( void )
{
	if ( gusSelectedSoldier != NOBODY )
	{
		LocateSoldier( gusSelectedSoldier, 10 );
	}
}
void HandleTBChangeLevel( void )
{
	if ( gpItemPointer == NULL ) 
		UIHandleChangeLevel( NULL );
}
void HandleTBSwapHands( void )
{
	// swap primary & secondary hand
	if ( gusSelectedSoldier != NOBODY && !AM_A_ROBOT( MercPtrs[ gusSelectedSoldier ] ))
	{
		SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];
		UINT16 usOldHandItem = pSoldier->inv[HANDPOS].usItem;
		SwapHandItems( pSoldier );
		pSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldHandItem, pSoldier->inv[HANDPOS].usItem );

		fCharacterInfoPanelDirty = TRUE;
		fInterfacePanelDirty = DIRTYLEVEL2;
	}
}
void HandleTBSwapGunsling( void )
{
	SOLDIERTYPE *pSoldier;

	if ( gusSelectedSoldier != NOBODY && UsingNewInventorySystem() == true)
	{
		pSoldier = MercPtrs[ gusSelectedSoldier ];
		pSoldier->SwitchWeapons( );
	}
}
void HandleTBSwapKnife( void )
{
	if ( gusSelectedSoldier != NOBODY && UsingNewInventorySystem() == true)
	{
		SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];
		pSoldier->SwitchWeapons( TRUE );
	}
}
void HandleTBSwapSidearm( void )
{
	if ( gusSelectedSoldier != NOBODY && UsingNewInventorySystem() == true)
	{
		SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];
		pSoldier->SwitchWeapons( FALSE, TRUE );
	}
}
void HandleTBSwapGoogles( void )
{
	SOLDIERTYPE	*pTeamSoldier;
	INT8		bLoop;

	for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
	{
		// HEADROCK HAM 3.5: When this INI setting is enabled, ALL mercs in the current sector will do a goggle swap.
		if (gGameExternalOptions.fGoggleSwapAffectsAllMercsInSector)
		{
			if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->sSectorX == gWorldSectorX && pTeamSoldier->sSectorY == gWorldSectorY && pTeamSoldier->bSectorZ == gbWorldSectorZ && !AM_A_ROBOT( pTeamSoldier ) )
			{
				SwapGoggles(pTeamSoldier);
			}
		}
		else
		{
			if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
			{
				SwapGoggles(pTeamSoldier);
			}
		}
	}
}
void HandleTBToggleFormation( void )
{
	// Flugente: toggle formation
	if ( gGameSettings.fOptions[TOPTION_MERCENARY_FORMATIONS] )
	{
		gGameSettings.fOptions[TOPTION_MERCENARY_FORMATIONS] = FALSE;
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_FORMATIONS_OFF ] );
	}
	else
	{
		gGameSettings.fOptions[TOPTION_MERCENARY_FORMATIONS] = TRUE;
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_FORMATIONS_ON ] );
	}
}
void HandleTBEnterTurnbased( void )
{
	if (!is_networked)
	{
		// ctrl-x: enter turn based while sneaking - check if RT sneak is on, iw we're not already in combat and if we actually see any enemies
		if (gGameSettings.fOptions[TOPTION_ALLOW_REAL_TIME_SNEAK]) // changed by SANDRO
		{
			BOOLEAN fSneakingInRealTime = true;

			if( gTacticalStatus.uiFlags & INCOMBAT )
			{	// Don't allow this in combat
				if (!gGameExternalOptions.fQuietRealTimeSneak)
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_RTM_IN_COMBAT_ALREADY]);
				fSneakingInRealTime = false;
			}

			if( WeSeeNoOne() )
			{	// Don't allow this if no enemies are seen - we have the forced turn mode for that
				if (!gGameExternalOptions.fQuietRealTimeSneak)
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_RTM_NO_ENEMIES]);
				fSneakingInRealTime = false;
			}
							
			if (fSneakingInRealTime)
				EnterCombatMode(OUR_TEAM);
		}
	}
}
void HandleTBToggleSneak( void )
{
	if (!is_networked)
	{
		// SANDRO - changed the real time sneak switch to be in the preferences
		if (gGameSettings.fOptions[TOPTION_ALLOW_REAL_TIME_SNEAK])
		{
			gGameSettings.fOptions[TOPTION_ALLOW_REAL_TIME_SNEAK] = false;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_RTM_SNEAKING_OFF]);
							
			if( !WeSeeNoOne() )	// if we're sneaking up on someone, enter turn-based
				EnterCombatMode(OUR_TEAM);
		}
		else
		{
			gGameSettings.fOptions[TOPTION_ALLOW_REAL_TIME_SNEAK] = true;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_RTM_SNEAKING_ON]);
		}
	}
}
void HandleTBToggleStealth( void )
{
	// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
	if ( ( gpItemPointer == NULL ) )
	{
		HandleStealthChangeFromUIKeys( );
	}
}
void HandleTBReload( void )
{
	AutoReload( MercPtrs[ gusSelectedSoldier ] );
}
void HandleTBReloadAll( void )
{
				if (! ( gTacticalStatus.fEnemyInSector ) )
				{
					SOLDIERTYPE	*pTeamSoldier;
					UINT8		bLoop;
					UINT16		bullets;		
					OBJECTTYPE *pGun, *pAmmo, *pAmmoMags;

					// Search for soldier
					for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
					{
						if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
						{
							// Search for gun in soldier inventory
							UINT32 invsize = pTeamSoldier->inv.size();
							for (UINT32 bLoop2 = 0; bLoop2 < invsize; ++bLoop2)
							{
								if ( (Item[pTeamSoldier->inv[bLoop2].usItem].usItemClass & (IC_GUN | IC_LAUNCHER)) )
								{
									pGun	= &(pTeamSoldier->inv[bLoop2]);

									UINT16 gunmagsize = GetMagSize( pGun );

									//if magazine is not full
									if ( (*pGun)[0]->data.gun.ubGunShotsLeft < gunmagsize )
									{
										// Search for ammo in sector
										for ( UINT32 uiLoop = 0; uiLoop < guiNumWorldItems; ++uiLoop )
										{
											if ( (gWorldItems[ uiLoop ].bVisible == TRUE) && (gWorldItems[ uiLoop ].fExists) && (gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_ARMED_BOMB) )//item exists, is reachable, is visible and is not trapped
											{
												if ( ( Item[ gWorldItems[ uiLoop ].object.usItem ].usItemClass & IC_AMMO ) ) // the item is ammo
												{
													pAmmo = &( gWorldItems[ uiLoop ].object );

													if ( CompatibleAmmoForGun( pAmmo, pGun ) ) // can use the ammo with this gun
													{
														// same ammo type in gun and magazine
														if ( Magazine[Item[(*pGun)[0]->data.gun.usGunAmmoItem].ubClassIndex].ubAmmoType == Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType )
														{
															ReloadGun( pTeamSoldier, pGun, pAmmo );
														}

														if ((*pAmmo)[0]->data.ubShotsLeft == 0)
														{
															RemoveItemFromPool( gWorldItems[ uiLoop ].sGridNo, uiLoop, gWorldItems[ uiLoop ].ubLevel );
														}

														// if gun is full, we can stop this
														if ( (*pGun)[0]->data.gun.ubGunShotsLeft >= gunmagsize )
															break;
													}
												}
											}
										}
									}
									//CHRISL: if not enough ammo in sector, reload using ammo carried in inventory
									if ( (*pGun)[0]->data.gun.ubGunShotsLeft < gunmagsize )
									{
										AutoReload( pTeamSoldier );
									}
								}
							}
						}
					}

					//MM: loop thru the soldiers again (lazy copy/paste :p).  could do it all at once, but then there may not be enough ammo from the world items to fill everyone's guns first
					for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
					{
						if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
						{
							// Search for ammo in soldier inventory
							UINT32 invsize = pTeamSoldier->inv.size();
							for (UINT32 bLoop2 = 0; bLoop2 < invsize; ++bLoop2)
							{
								if ( Item[pTeamSoldier->inv[bLoop2].usItem].usItemClass & IC_AMMO )
								{
									pAmmoMags = &(pTeamSoldier->inv[bLoop2]);

									for ( UINT16 stackMag = 0; stackMag < (*pAmmoMags).ubNumberOfObjects; stackMag++ )
									{
										if ( (*pAmmoMags)[stackMag]->data.ubShotsLeft < Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubMagSize )
										{
											// Search for ammo in sector
											for ( UINT32 uiLoop = 0; uiLoop < guiNumWorldItems; ++uiLoop )
											{
												if ( (gWorldItems[ uiLoop ].bVisible == TRUE) && (gWorldItems[ uiLoop ].fExists) && (gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_ARMED_BOMB) )//item exists, is reachable, is visible and is not trapped
												{
													if ( ( Item[ gWorldItems[ uiLoop ].object.usItem ].usItemClass & IC_AMMO ) ) // the item is ammo
													{
														pAmmo = &( gWorldItems[ uiLoop ].object );

														if ( Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubCalibre == Magazine[Item[pAmmo->usItem].ubClassIndex].ubCalibre ) // same calibre
														{
															// same ammo type
															if ( Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubAmmoType == Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType )
															{
																bullets = Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubMagSize - (*pAmmoMags)[stackMag]->data.ubShotsLeft;
																
																if ((*pAmmo)[0]->data.ubShotsLeft < bullets)
																	bullets = (*pAmmo)[0]->data.ubShotsLeft;

																(*pAmmoMags)[stackMag]->data.ubShotsLeft += bullets;
																(*pAmmo)[0]->data.ubShotsLeft -= bullets;

																fCharacterInfoPanelDirty = TRUE;
																fInterfacePanelDirty = DIRTYLEVEL2;
															}

															if ((*pAmmo)[0]->data.ubShotsLeft == 0)
															{
																RemoveItemFromPool( gWorldItems[ uiLoop ].sGridNo, uiLoop, gWorldItems[ uiLoop ].ubLevel );
															}
														}
													}
												}
											}

											//MM: if magazines still are partly empty, look through inventory for boxes and crates
											if ( (*pAmmoMags)[stackMag]->data.ubShotsLeft < Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubMagSize )
											{
												UINT32 invsize = pTeamSoldier->inv.size();
												for (UINT32 uiLoop = 0; uiLoop < invsize; ++uiLoop)
												{
													if ( (Item[pTeamSoldier->inv[uiLoop].usItem].usItemClass & IC_AMMO) && Magazine[Item[pTeamSoldier->inv[uiLoop].usItem].ubClassIndex].ubMagType >= AMMO_BOX )
													{
														pAmmo = &(pTeamSoldier->inv[uiLoop]);

														if ( Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubCalibre == Magazine[Item[pAmmo->usItem].ubClassIndex].ubCalibre ) // same calibre
														{
															// same ammo type
															if ( Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubAmmoType == Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType )
															{
																bullets = Magazine[Item[pAmmoMags->usItem].ubClassIndex].ubMagSize - (*pAmmoMags)[stackMag]->data.ubShotsLeft;
																
																if ((*pAmmo)[0]->data.ubShotsLeft < bullets)
																	bullets = (*pAmmo)[0]->data.ubShotsLeft;

																(*pAmmoMags)[stackMag]->data.ubShotsLeft += bullets;
																(*pAmmo)[0]->data.ubShotsLeft -= bullets;

																fCharacterInfoPanelDirty = TRUE;
																fInterfacePanelDirty = DIRTYLEVEL2;
															}

															if ((*pAmmo)[0]->data.ubShotsLeft == 0)
																DeleteObj(pAmmo);
														}
													}
												}
											}
										}
									}

									if (IsWeaponAttached(pGun, IC_GUN))
									{
										OBJECTTYPE *pGun2 = FindAttachedWeapon(pGun, IC_GUN);

										UINT16 gunmagsize2 = GetMagSize( pGun2 );

										if ( (*pGun2)[0]->data.gun.ubGunShotsLeft < gunmagsize2 )
										{
											// Search for ammo in sector
											for ( UINT32 uiLoop = 0; uiLoop < guiNumWorldItems; ++uiLoop )
											{
												if ( (gWorldItems[ uiLoop ].bVisible == TRUE) && (gWorldItems[ uiLoop ].fExists) && (gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_ARMED_BOMB) )//item exists, is reachable, is visible and is not trapped
												{
													if ( ( Item[ gWorldItems[ uiLoop ].object.usItem ].usItemClass & IC_AMMO ) ) // the item is ammo
													{
														pAmmo = &( gWorldItems[ uiLoop ].object );

														if ( CompatibleAmmoForGun( pAmmo, pGun2 ) ) // can use the ammo with this gun
														{
															// same ammo type in gun and magazine
															if ( Magazine[Item[(*pGun2)[0]->data.gun.usGunAmmoItem].ubClassIndex].ubAmmoType == Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType )
															{
																ReloadGun( pTeamSoldier, pGun2, pAmmo );
															}

															if ((*pAmmo)[0]->data.ubShotsLeft == 0)
															{
																RemoveItemFromPool( gWorldItems[ uiLoop ].sGridNo, uiLoop, gWorldItems[ uiLoop ].ubLevel );
															}

															// if gun is full, we can stop this
															if ( (*pGun2)[0]->data.gun.ubGunShotsLeft >= gunmagsize2 )
																break;
														}
													}
												}
											}
										}

										//CHRISL: if not enough ammo in sector, reload using ammo carried in inventory
										if ( (*pGun2)[0]->data.gun.ubGunShotsLeft < gunmagsize2 )
										{
											AutoReload( pTeamSoldier );
										}
									}
								}
							}
						}
					}
				}
				else
				{
					SOLDIERTYPE	*pTeamSoldier;
					UINT8		bLoop;
					OBJECTTYPE *pGun, *pAmmo;

					for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++bLoop, pTeamSoldier++)
					{
						if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
						{
							if ( (Item[pTeamSoldier->inv[HANDPOS].usItem].usItemClass & (IC_GUN | IC_LAUNCHER) ) )
							{
								if ( ( gTacticalStatus.uiFlags & INCOMBAT ) )
								{
									// Flugente: check for underbarrel weapons and use that object if necessary
									pGun = pTeamSoldier->GetUsedWeapon( &(pTeamSoldier->inv[HANDPOS]) );

									//magazine is not full
									if ( (*pGun)[0]->data.gun.ubGunShotsLeft < GetMagSize( pGun )	)
									{
										AutoReload( pTeamSoldier );
									}
								}
								else
								{
									// Search for gun in soldier inventory
									UINT32 invsize = pTeamSoldier->inv.size();
									for (UINT32 bLoop2 = 0; bLoop2 < invsize; ++bLoop2)
									{
										if ( (Item[pTeamSoldier->inv[bLoop2].usItem].usItemClass & (IC_GUN | IC_LAUNCHER)) )
										{
											pGun	= &(pTeamSoldier->inv[bLoop2]);

											UINT16 gunmagsize = GetMagSize( pGun );

											//if magazine is not full
											if ( (*pGun)[0]->data.gun.ubGunShotsLeft < gunmagsize )
											{
												// Search for ammo in soldier inventory
												for ( UINT32 uiLoop = 0; uiLoop < invsize; ++uiLoop )
												{
													if ( (Item[pTeamSoldier->inv[uiLoop].usItem].usItemClass & IC_AMMO ) ) // the item is ammo
													{
														pAmmo = &(pTeamSoldier->inv[uiLoop]);

														if ( CompatibleAmmoForGun( pAmmo, pGun ) ) // can use the ammo with this gun
														{
															// same ammo type in gun and magazine
															if ( Magazine[Item[(*pGun)[0]->data.gun.usGunAmmoItem].ubClassIndex].ubAmmoType == Magazine[Item[pAmmo->usItem].ubClassIndex].ubAmmoType )
															{
																ReloadGun( pTeamSoldier, pGun, pAmmo );

																fCharacterInfoPanelDirty = TRUE;
																fInterfacePanelDirty = DIRTYLEVEL2;

																// if gun is full, we can stop this
																if ( (*pGun)[0]->data.gun.ubGunShotsLeft >= gunmagsize )
																	break;
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
}

void HandleTBShowCover( void )
{
	ToggleEnemyView();
}
void HandleTBShowMines( void )
{
	ToggleHostileTrapsView();
}
void HandleTBShowLOS( void )
{
	ToggleMercView();
}

void HandleTBToggleTrapNetworkView( void )
{
	ToggleTrapNetworkView();
}


void HandleTBGotoHigherStance( void )
{
	if ( (gusSelectedSoldier != NOBODY) && ( gpItemPointer == NULL ) )
		GotoHeigherStance(MercPtrs[ gusSelectedSoldier ]);
}
void HandleTBGotoLowerStance( void )
{
	if ( (gusSelectedSoldier != NOBODY) && ( gpItemPointer == NULL ) )
		GotoLowerStance( MercPtrs[ gusSelectedSoldier ] );
}
void HandleTBLocateNextMerc( void )
{
	UINT8	bID;
	if ( gusSelectedSoldier != NOBODY )
	{ //Select next merc
		bID = FindNextMercInTeamPanel( MercPtrs[ gusSelectedSoldier ], FALSE, FALSE );
		HandleLocateSelectMerc( bID, LOCATEANDSELECT_MERC );
		// Center to guy....
		LocateSoldier( gusSelectedSoldier, SETLOCATOR );
	}
}
void HandleTBLocatePrevMerc( void )
{
	UINT8	bID;
	if ( gusSelectedSoldier != NOBODY )
	{ 
		bID = FindPrevActiveAndAliveMerc( MercPtrs[ gusSelectedSoldier ], TRUE, TRUE );
		HandleLocateSelectMerc( bID, LOCATEANDSELECT_MERC );
		// Center to guy....
		LocateSoldier( gusSelectedSoldier, SETLOCATOR );
	}
}

void HandleTBDropBackpacks( void )
{
	//if( UsingNewInventorySystem() && gusSelectedSoldier != NOBODY )
	if( UsingNewInventorySystem() )
	{
		//SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];
		SOLDIERTYPE	*pTeamSoldier;
		UINT8		ubLoop;

		INT16	sAPCost = APBPConstants[AP_BACK_PACK];
		INT32	iBPCost = APBPConstants[BP_BACK_PACK];

		for( ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++ )
		{
			pTeamSoldier=MercPtrs[ ubLoop ];

			if( OK_CONTROLLABLE_MERC( pTeamSoldier ) &&
				OK_INTERRUPT_MERC( pTeamSoldier ) &&
				!AM_A_ROBOT( pTeamSoldier ) &&
				EnoughPoints(pTeamSoldier, sAPCost, iBPCost, FALSE) &&
				//pTeamSoldier->bAssignment == pSoldier->bAssignment &&
				pTeamSoldier->bAssignment < ON_DUTY &&
				pTeamSoldier->inv[BPACKPOCKPOS].exists() &&
				!pTeamSoldier->flags.DropPackFlag )
			{				
				if( ChangeDropPackStatus(pTeamSoldier, TRUE) )
				{
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_SOLDIER_DROP], pTeamSoldier->GetName());
				}
			}
		}

		fCharacterInfoPanelDirty = TRUE;
		fInterfacePanelDirty = DIRTYLEVEL2;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_ALL_DROP] );
	}
}

void HandleTBPickUpBackpacks( void )
{
	//if( UsingNewInventorySystem() && gusSelectedSoldier != NOBODY )
	if( UsingNewInventorySystem() )
	{
		//SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];
		SOLDIERTYPE	*pTeamSoldier;
		UINT8		ubLoop;

		INT16	sAPCost = APBPConstants[AP_BACK_PACK];
		INT32	iBPCost = APBPConstants[BP_BACK_PACK];

		for( ubLoop = gTacticalStatus.Team[gbPlayerNum].bFirstID; ubLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; ubLoop++ )
		{
			pTeamSoldier=MercPtrs[ubLoop];

			if( OK_CONTROLLABLE_MERC( pTeamSoldier ) &&
				OK_INTERRUPT_MERC( pTeamSoldier ) &&
				!AM_A_ROBOT( pTeamSoldier ) &&
				EnoughPoints(pTeamSoldier, sAPCost, iBPCost, FALSE) &&
				//pTeamSoldier->bAssignment == pSoldier->bAssignment &&
				pTeamSoldier->bAssignment < ON_DUTY &&
				!pTeamSoldier->inv[BPACKPOCKPOS].exists() &&
				pTeamSoldier->flags.DropPackFlag )
			{				
				if( ChangeDropPackStatus(pTeamSoldier, FALSE) )
				{
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_SOLDIER_PICKUP], pTeamSoldier->GetName());
				}
			}
		}

		fCharacterInfoPanelDirty = TRUE;
		fInterfacePanelDirty = DIRTYLEVEL2;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_ALL_PICKUP] );
	}
}

void HandleTBSoldierRun( void )
{
	SOLDIERTYPE *pSoldier = MercPtrs[ gusSelectedSoldier ];
	if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER ) )
	{
		pSoldier = GetSoldierStructureForVehicle( pSoldier->iVehicleId );
	}
	if ( !pSoldier->MercInWater() && !(pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT ) )
	{
		//change selected merc to run
		if ( pSoldier->usUIMovementMode != WALKING 
		   && pSoldier->usUIMovementMode != RUNNING
			&& pSoldier->usUIMovementMode != WALKING_WEAPON_RDY
			 && pSoldier->usUIMovementMode != WALKING_DUAL_RDY
			  && pSoldier->usUIMovementMode != WALKING_ALTERNATIVE_RDY )
		{
			UIHandleSoldierStanceChange( (UINT8)pSoldier->ubID, ANIM_STAND );
			pSoldier->flags.fUIMovementFast = 1;
		}
		else
		{
			pSoldier->flags.fUIMovementFast = 1;
			pSoldier->usUIMovementMode = RUNNING;
			gfPlotNewMovement = TRUE;
		}
	}
}

void HandleTacticalAmmoCrates( UINT8 magType )
{
	if(gTacticalStatus.fEnemyInSector)
		return;

	INT32		crateItem;
	INT32		worldItem;
	//UINT8		magType;
	bool		mergeSuccessful, ammoPresent;
	int loopCount = 0;
	OBJECTTYPE	newCrate;
	ammoPresent = true;

	while (ammoPresent && loopCount <= 10)
	{
		//look through all sector items for ammo.
		for(unsigned int wItem = 0; wItem < guiNumWorldItems; wItem++)
		{
			crateItem = 0;
			mergeSuccessful = false;
			if(Item[gWorldItems[wItem].object.usItem].usItemClass == IC_AMMO && gWorldItems[wItem].bVisible == TRUE && gWorldItems[wItem].fExists && (gWorldItems[wItem].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[wItem].usFlags & WORLD_ITEM_ARMED_BOMB))
			{
				worldItem = gWorldItems[wItem].object.usItem;
				//we don't want to do anything if the world item is already an ammo crate/box
				if(Magazine[Item[worldItem].ubClassIndex].ubMagType == magType)
					continue;

				//we have a valid, ammo item.  Look through Items.xml and see if we have an ammo crate for
				//	this ammo type
				for (UINT32 iLoop = 0; iLoop < gMAXITEMS_READ; ++iLoop)
				{
					//if ammo crate && calibers match && Ammo Types match
					if(Item[iLoop].usItemClass == IC_AMMO && Magazine[Item[iLoop].ubClassIndex].ubMagType == magType && Magazine[Item[iLoop].ubClassIndex].ubCalibre == Magazine[Item[worldItem].ubClassIndex].ubCalibre && Magazine[Item[iLoop].ubClassIndex].ubAmmoType == Magazine[Item[worldItem].ubClassIndex].ubAmmoType)
					{
						crateItem = iLoop;
						break;
					}
				}

				//if we found a crateItem in the list, we first want to see if we already have an item created
				if(crateItem != 0)
				{
					//look through world items first
					for(unsigned int loop=0; loop < guiNumWorldItems; ++loop)
					{
						if(gWorldItems[loop].object.usItem == crateItem)
						{
							DistributeStatus(&gWorldItems[wItem].object, &gWorldItems[loop].object, Magazine[Item[crateItem].ubClassIndex].ubMagSize);
							if(gWorldItems[wItem].object.ubNumberOfObjects < 1)
							{
								mergeSuccessful = true;
								RemoveItemFromPool(gWorldItems[wItem].sGridNo,(wItem),gWorldItems[wItem].ubLevel);
								RemoveItemFromWorld(wItem);
								break;
							}
						}

					}
					//no crates in sector inventory.  search merc inventories
					if(!mergeSuccessful)
					{
						for(int loop=0; loop<(int)OUR_TEAM_SIZE_NO_VEHICLE; ++loop)
						{
							if(MercPtrs[loop]->bActive && MercPtrs[loop]->bInSector)
							{
								UINT32 invsize = MercPtrs[loop]->inv.size();
								for(UINT32 pocket=0; pocket < invsize; ++pocket)
								{
									if(MercPtrs[loop]->inv[pocket].usItem == crateItem)
									{
										DistributeStatus(&gWorldItems[wItem].object, &MercPtrs[loop]->inv[pocket], Magazine[Item[crateItem].ubClassIndex].ubMagSize);
										if(gWorldItems[wItem].object.ubNumberOfObjects < 1)
										{
											mergeSuccessful = true;
											RemoveItemFromPool(gWorldItems[wItem].sGridNo,(wItem),gWorldItems[wItem].ubLevel);
											RemoveItemFromWorld(wItem);
											break;
										}
									}
								}
								if(mergeSuccessful == true)
									break;
							}
						}
					}
					//no crates in merc inventory.  create a new sector item
					if(!mergeSuccessful)
					{
						CreateAmmo(crateItem, &newCrate, 0);
						DistributeStatus(&gWorldItems[wItem].object, &newCrate, Magazine[Item[crateItem].ubClassIndex].ubMagSize);
						AddItemToPool(gWorldItems[wItem].sGridNo, &newCrate, 1, gWorldItems[wItem].ubLevel, gWorldItems[wItem].usFlags, gWorldItems[wItem].bRenderZHeightAboveLevel);
						if(gWorldItems[wItem].object.ubNumberOfObjects < 1)
						{
							mergeSuccessful = true;
							RemoveItemFromPool(gWorldItems[wItem].sGridNo,(wItem),gWorldItems[wItem].ubLevel);
							RemoveItemFromWorld(wItem);
						}
					}
				}
			}
		}

		// if we added to / created a box/crate, then we're fine to reset this
		if ( mergeSuccessful )
			loopCount = 0;

		//MM: loop through ammo multiple times, as boxes and crates may take a few passes to fill
		ammoPresent = false;
		for(unsigned int wItem = 0; wItem < guiNumWorldItems; ++wItem)
		{
			if(Item[gWorldItems[wItem].object.usItem].usItemClass == IC_AMMO && gWorldItems[wItem].bVisible == TRUE && gWorldItems[wItem].fExists && (gWorldItems[wItem].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[wItem].usFlags & WORLD_ITEM_ARMED_BOMB))
			{
				worldItem = gWorldItems[wItem].object.usItem;
				if(Magazine[Item[worldItem].ubClassIndex].ubMagType == magType)
					continue;

				loopCount++;
				ammoPresent = true;
				break;
			}
		}

		StackAndSort(TRUE);
	}
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pImpButtonText[11] );
}

void HandleTacticalInventoryMenu( void )
{
	for( INT32 cnt = 0; cnt < TACTICAL_INVENTORY_DIALOG_NUM; ++cnt)
	{
		wcscpy( gzUserDefinedButton[cnt], szTacticalInventoryDialogString[cnt+1] );		
		gzUserDefinedButtonColor[cnt] = 0;
	}

	gzUserDefinedButtonColor[0] = FONT_MCOLOR_LTGRAY;
	gzUserDefinedButtonColor[1] = FONT_MCOLOR_LTGRAY;
	gzUserDefinedButtonColor[2] = FONT_MCOLOR_LTGRAY;
	gzUserDefinedButtonColor[3] = FONT_MCOLOR_LTGRAY;
	gzUserDefinedButtonColor[4] = FONT_ORANGE;
	gzUserDefinedButtonColor[5] = FONT_ORANGE;
	gzUserDefinedButtonColor[6] = FONT_ORANGE;
	gzUserDefinedButtonColor[7] = FONT_ORANGE;

	gzUserDefinedButtonColor[8] = FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[9] = FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[10] = FONT_MCOLOR_LTYELLOW;
	gzUserDefinedButtonColor[11] = FONT_MCOLOR_LTYELLOW;

	DoMessageBox( MSG_BOX_BASIC_STYLE,  szTacticalInventoryDialogString[0], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS, TacticalInventoryMessageBoxCallBack, NULL );
}

void TacticalInventoryMessageBoxCallBack( UINT8 ubExitValue )
{
	switch( ubExitValue )
	{
	case 1:		// switch NVG
		HandleTBSwapGoogles();
		break;
	case 2:		// reload ALL
		HandleTBReloadAll();
		break;
	case 3:		// move items (Shift+m)
		HandleTacticalMoveItems();
		break;
	case 4:		
		break;
	case 5:		// sort items
		StackAndSort( TRUE );
		break;
	case 6:		// merge items
		StackAndSort( FALSE );
		break;
	case 7:		// remove att
		SeperateItems();
		break;
	case 8:		// organize items
		SeperateItems();
		StackAndSort( FALSE );
		break;
	case 9:		// ammo crates
		HandleTacticalAmmoCrates( AMMO_CRATE );		
		break;
	case 10:	// ammo boxes
		HandleTacticalAmmoCrates( AMMO_BOX );
		break;
	case 11:	// drop backpacks
		HandleTBDropBackpacks();
		break;
	case 12:	// pickup backpacks
		HandleTBPickUpBackpacks();
		break;
	case 13:
		break;
	case 14:
		break;
	case 15:
		break;
	case 16:
		break;
	}
}

void FortificationSettingCallBack( UINT8 ubExitValue )
{
	gCurrentFortificationStructure			= (UINT8)(DropDownTemplate<DROPDOWNNR_MSGBOX_1>::getInstance( ).GetSelectedEntryKey( ));
	gCurrentFortificationTileLibraryIndex	= (UINT8)(DropDownTemplate<DROPDOWNNR_MSGBOX_2>::getInstance( ).GetSelectedEntryKey( ));
}

void HandleTacticalCoverMenu( void )
{
	for( INT32 cnt = 0; cnt < TACTICAL_COVER_DIALOG_NUM; ++cnt)
	{
		wcscpy( gzUserDefinedButton[cnt], szTacticalCoverDialogString[cnt+1] );		
		gzUserDefinedButtonColor[cnt] = 0;
	}
	gzUserDefinedButtonColor[0] = 0;
	gzUserDefinedButtonColor[1] = (gubDrawMode != COVER_DRAW_ENEMY_VIEW) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[2] = (gubDrawMode != COVER_DRAW_MERC_VIEW) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[3] = 0;
	gzUserDefinedButtonColor[4] = gDisplayEnemyRoles ? FONT_MCOLOR_LTGREEN : FONT_LTRED;
	gzUserDefinedButtonColor[5] = (gubDrawMode != DRAW_MODE_FORTIFY) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[6] = (gubDrawMode != DRAW_MODE_TRACKER_SMELL) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[7] = (gubDrawMode != DRAW_MODE_WEAPONDATA) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[8] = (gubDrawMode != MINES_DRAW_PLAYERTEAM_NETWORKS) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[9] = (gubDrawMode != MINES_DRAW_NETWORKCOLOURING) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[10] = (gubDrawMode != MINES_DRAW_DETECT_ENEMY) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[11] = 0;
	gzUserDefinedButtonColor[12] = (gubDrawMode != MINES_DRAW_NET_A) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[13] = (gubDrawMode != MINES_DRAW_NET_B) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[14] = (gubDrawMode != MINES_DRAW_NET_C) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;
	gzUserDefinedButtonColor[15] = (gubDrawMode != MINES_DRAW_NET_D) ? FONT_LTRED : FONT_MCOLOR_LTGREEN;

	DoMessageBox( MSG_BOX_BASIC_STYLE,  szTacticalCoverDialogString[0], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS, TacticalCoverMessageBoxCallBack, NULL );
}

void TacticalCoverMessageBoxCallBack( UINT8 ubExitValue )
{
	switch( ubExitValue )
	{
	case 1:
		gubDrawMode = DRAW_MODE_OFF;
		break;
	case 2:
		gubDrawMode = COVER_DRAW_ENEMY_VIEW;
		break;
	case 3:
		gubDrawMode = COVER_DRAW_MERC_VIEW;
		break;
	case 4:
		break;
	case 5:
		gDisplayEnemyRoles = !gDisplayEnemyRoles;
		break;
	case 6:		
		if ( gubDrawMode == DRAW_MODE_FORTIFY )
			gubDrawMode = DRAW_MODE_OFF;
		else
			gubDrawMode = DRAW_MODE_FORTIFY;
		break;
	case 7:
		if ( gubDrawMode == DRAW_MODE_TRACKER_SMELL )
			gubDrawMode = DRAW_MODE_OFF;
		else
			gubDrawMode = DRAW_MODE_TRACKER_SMELL;
		break;
	case 8:
		if ( gubDrawMode == DRAW_MODE_WEAPONDATA )
			gubDrawMode = DRAW_MODE_OFF;
		else
			gubDrawMode = DRAW_MODE_WEAPONDATA;
		break;
	case 9:
		gubDrawMode = MINES_DRAW_PLAYERTEAM_NETWORKS;
		break;
	case 10:
		gubDrawMode = MINES_DRAW_NETWORKCOLOURING;
		break;
	case 11:
		gubDrawMode = MINES_DRAW_DETECT_ENEMY;
		break;
	case 12:
		break;
	case 13:
		gubDrawMode = MINES_DRAW_NET_A;
		break;
	case 14:
		gubDrawMode = MINES_DRAW_NET_B;
		break;
	case 15:
		gubDrawMode = MINES_DRAW_NET_C;
		break;
	case 16:
		gubDrawMode = MINES_DRAW_NET_D;
		break;
	}

	DisplayCover( TRUE );

	if( ubExitValue <= TACTICAL_COVER_DIALOG_NUM )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szTacticalCoverDialogPrintString[ubExitValue-1]);
}

void ActivateCheatsMessageBoxCallBack( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		if ( CHEATER_CHEAT_LEVEL( ) )
		{
			RESET_CHEAT_LEVEL( );
		}
		else
		{
			ACTIVATE_CHEAT_LEVEL( );

			// ATE; We're done.... start cheat mode....
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_CHEAT_LEVEL_TWO] );
			SetHistoryFact( HISTORY_CHEAT_ENABLED, 0, GetWorldTotalMin( ), -1, -1 );
		}
	}
}

void HandleTacticalTransformItem(void)
{
	UINT16 usItem;
	TransformInfoStruct *pTransformation;

	SOLDIERTYPE* pSoldier;
	if (!GetSoldier(&pSoldier, gusSelectedSoldier))
		return;

	if (!pSoldier->inv[HANDPOS].exists())
		return;

	// ATE: Don't do this if in a fire animation.....
	if (gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_FIRE)
		return;

	// don't do this when EDB is active
	if (gpItemDescObject)
		return;

	usItem = pSoldier->inv[HANDPOS].usItem;

	if (FindTransformation(usItem, &pTransformation))
	{
		if (pTransformation->usAPCost > 0 && gTacticalStatus.uiFlags & INCOMBAT && gTacticalStatus.uiFlags & TURNBASED)
			ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s (%d AP)", pTransformation->szMenuRowText, pTransformation->usAPCost);
		else
			ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pTransformation->szMenuRowText);

		// let's pretend we are calling this from EDB
		gpItemDescObject = &pSoldier->inv[HANDPOS];

		gpItemDescObject->TransformObject(pSoldier, 0, pTransformation, NULL);

		// cleanup
		gpItemDescObject = NULL;

		DirtyMercPanelInterface(pSoldier, DIRTYLEVEL2);
		gfUIForceReExamineCursorData = TRUE;
	}
}

BOOLEAN FindTransformation( UINT16 usItem, TransformInfoStruct **pTransformation )
{
	// find transformation
	for ( UINT32 x = 0; x < gMAXITEMS_READ; ++x )
	{
		if ( Transform[x].usItem == (UINT16)-1 )
		{
			break;
		}
		if ( Transform[x].usItem == usItem )
		{
			*pTransformation = &Transform[x];
			return TRUE;
		}
	}
	return FALSE;
}

void HandleTacticalMoveItems( void )
{
	// Move all sector items to current mercs feet
	if ( !(gTacticalStatus.fEnemyInSector) )
	{
		HandleAllReachAbleItemsInTheSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

		SOLDIERTYPE *pSoldier;
		if ( GetSoldier( &pSoldier, gusSelectedSoldier ) )
		{

			for ( UINT32 uiLoop = 0; uiLoop < guiNumWorldItems; ++uiLoop ) //for all items in sector
			{
				if ( (gWorldItems[ uiLoop ].bVisible == TRUE) && (gWorldItems[ uiLoop ].fExists) && (gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_REACHABLE) && !(gWorldItems[ uiLoop ].usFlags & WORLD_ITEM_ARMED_BOMB) && (gWorldItems[ uiLoop ].sGridNo != pSoldier->sGridNo) )//item exists and is reachable and is not already on soldiers tile
				{
					MoveItemPools(gWorldItems[ uiLoop ].sGridNo, pSoldier->sGridNo, gWorldItems[ uiLoop ].ubLevel, pSoldier->pathing.bLevel);
				}
			}

			NotifySoldiersToLookforItems( );
		}
	}
}

void HandleTacticalStoreInvItem( void )
{
	SOLDIERTYPE *pSoldier;
	UINT8 ubStoreSlot = 0;
	UINT16 APTotalCost = 0;

	if( gusSelectedSoldier != NOBODY )
		pSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;

	if ( !UsingNewInventorySystem() )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEMS_ONLY_NIV ] );
		return;
	}
	
	if( !pSoldier->inv[HANDPOS].exists() )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEMS_NO_ITEM_IN_HAND ] );
		return;
	}

	if( pSoldier->usQuickItemId == pSoldier->inv[HANDPOS].usItem &&
		( CanItemFitInPosition(pSoldier, &pSoldier->inv[HANDPOS], pSoldier->ubQuickItemSlot, FALSE) && !pSoldier->inv[pSoldier->ubQuickItemSlot].exists() ) )
		ubStoreSlot = pSoldier->ubQuickItemSlot;

	// find slot to store HANDPOS in
	for(UINT8 i = GUNSLINGPOCKPOS; i < NUM_INV_SLOTS && ubStoreSlot == 0; ++i)
	{
		// take first slot that hand item would fit
		if ( ( CanItemFitInPosition(pSoldier, &pSoldier->inv[HANDPOS], i, FALSE) && !pSoldier->inv[i].exists() ) )
		{
			ubStoreSlot = i;
			break;
		}
	}

	if( ubStoreSlot == 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEMS_NOWHERE_TO_PLACE ] );
		return;
	}
	
	// calculate needed APs
	if ( UsingInventoryCostsAPSystem() )
	{		
		APTotalCost += GetInvMovementCost(&pSoldier->inv[HANDPOS], HANDPOS, ubStoreSlot  );
		if( !EnoughPoints( pSoldier, APTotalCost, 0, TRUE ) )
			return;

		// move item from HANDPOS to found ubStoreSlot
		DeductPoints( pSoldier, APTotalCost, 0 );
	}

	pSoldier->inv[HANDPOS].MoveThisObjectTo(pSoldier->inv[ubStoreSlot], 1, pSoldier);
	
	// swap hands
	if( pSoldier->inv[SECONDHANDPOS].exists() )
		SwapHandItems( pSoldier );

	// update interface
	fCharacterInfoPanelDirty = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;
	pSoldier->HandleFlashLights();
}

void HandleTacticalTakeInvItem( INT32 iType )
{
	SOLDIERTYPE *pSoldier;
	UINT8 ubItemSlot = HANDPOS;
	UINT16 APTotalCost = 0;

	if( iType == 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEM_NO_DEFINED_ITEM ] );
		return;
	}

	if( gusSelectedSoldier != NOBODY )
		pSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;

	if ( !UsingNewInventorySystem() )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEMS_ONLY_NIV ] );
		return;
	}

	// check if item is already in main hand
	if ( pSoldier->inv[HANDPOS].exists() &&		
		( iType > 0 ? ( pSoldier->inv[HANDPOS].usItem == iType ) : ( InvItemType( pSoldier->inv[HANDPOS].usItem ) == iType ) ) )
		return;

	if( pSoldier->inv[HANDPOS].exists() && pSoldier->inv[SECONDHANDPOS].exists() )
	{
		// try to free HANDPOS for new item
		HandleTacticalStoreInvItem();
	}

	// both hands are full
	if( pSoldier->inv[HANDPOS].exists() && pSoldier->inv[SECONDHANDPOS].exists() )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEM_NO_FREE_HAND ] );
		return;
	}

	// search for type of item
	for(UINT8 i = GUNSLINGPOCKPOS; i < NUM_INV_SLOTS; ++i)
	{
		if ( pSoldier->inv[i].exists() &&
			!Item[ pSoldier->inv[i].usItem ].twohanded &&
			( iType > 0 ? ( pSoldier->inv[i].usItem == iType ) : ( InvItemType( pSoldier->inv[i].usItem ) == iType ) ) )
		{
			ubItemSlot = i;
			break;
		}
	}
	// if not found item
	if( ubItemSlot == HANDPOS )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEM_NOT_FOUND ]);
		return;
	}

	// remember item slot and id
	pSoldier->usQuickItemId = pSoldier->inv[ubItemSlot].usItem;
	pSoldier->ubQuickItemSlot = ubItemSlot; 	

	if ( UsingInventoryCostsAPSystem() )
	{		
		APTotalCost += GetInvMovementCost(&pSoldier->inv[ubItemSlot], ubItemSlot, HANDPOS);
		if( !EnoughPoints( pSoldier, APTotalCost, 0, TRUE ) )
			return;
	}

	if( !CanItemFitInPosition( pSoldier, &pSoldier->inv[ubItemSlot], HANDPOS, FALSE ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ QUICK_ITEM_CANNOT_TAKE ] );
		return;
	}

	// if item in main hand exists -> move it to second hand
	if( pSoldier->inv[HANDPOS].exists() )
		HandleTBSwapHands();

	// move item to handpos
	if ( UsingInventoryCostsAPSystem() )
		DeductPoints( pSoldier, APTotalCost, 0 );
	pSoldier->inv[ubItemSlot].MoveThisObjectTo(pSoldier->inv[HANDPOS], 1, pSoldier);

	// update interface
	fCharacterInfoPanelDirty = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;
	pSoldier->HandleFlashLights();
}

INT32 InvItemType( UINT16 usItem )
{
	if( usItem == XRAY_DEVICE )
		return INV_ITEM_TYPE_XRAY;
	if( Item[ usItem ].firstaidkit )
		return INV_ITEM_TYPE_FIRSTAID;
	if( Item[ usItem ].usItemClass == IC_BLADE )
		return INV_ITEM_TYPE_MELEE;
	if( Item[ usItem ].usItemClass == IC_PUNCH && !HasItemFlag( usItem, TASER ) )
		return INV_ITEM_TYPE_HTH;
	if( HasItemFlag( usItem, HANDCUFFS ) )
	//if( Item[ usItem ].usItemClass == IC_MISC && Item[ usItem ].usItemFlag & HANDCUFFS )
		return INV_ITEM_TYPE_HANDCUFFS;
	//Item[ uiItem ].usSpotting
	if( Item[ usItem ].usItemClass == IC_MISC && Item[ usItem ].dayvisionrangebonus )
		return INV_ITEM_TYPE_BINOCULARS;
	if( HasItemFlag( usItem, TASER ) )
		return INV_ITEM_TYPE_TASER;
	if( Item[ usItem ].usItemClass == IC_GUN && !Item[ usItem ].twohanded && Weapon[Item[ usItem ].ubClassIndex].ubWeaponType <= GUN_SMG )
		return INV_ITEM_TYPE_SIDEARM;
	if( Item[ usItem ].usItemClass == IC_MISC && Item[ usItem ].metaldetector )
		return INV_ITEM_TYPE_METALDETECTOR;
		
	return INV_ITEM_TYPE_UNKNOWN;
}

void HandleTacticalDropItem( UINT8 ubSlot )
{
	SOLDIERTYPE *pSoldier;
	UINT16 APTotalCost = 0;
	
	if( gusSelectedSoldier != NOBODY )
		pSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;

	if( pSoldier->inv[ ubSlot ].exists() )
	{		
		if ( UsingInventoryCostsAPSystem() )
		{		
			APTotalCost += GetInvMovementCost(&pSoldier->inv[ubSlot], ubSlot, HANDPOS);
			APTotalCost += GetBasicAPsToPickupItem( pSoldier );
			if( !EnoughPoints( pSoldier, APTotalCost, 0, TRUE ) )
				return;
			DeductPoints( pSoldier, APTotalCost, 0 );
		}

		SoldierDropItem( pSoldier, &pSoldier->inv[ ubSlot ] );
		DeleteObj( &(pSoldier->inv[ ubSlot ]) );
	}
}

void HandleTacticalTakeItem( void )
{
	SOLDIERTYPE *pSoldier;
	
	if( gusSelectedSoldier != NOBODY )
		pSoldier = MercPtrs[ gusSelectedSoldier ];
	else
		return;

	if( gpItemPointer == NULL )
	{
		//not holding anything
		if( pSoldier->inv[HANDPOS].exists() )
		{
			pSoldier->inv[HANDPOS].MoveThisObjectTo(gItemPointer);
			gpItemPointer = &gItemPointer;
		}
	}

	// update interface
	fCharacterInfoPanelDirty = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;
	pSoldier->HandleFlashLights();

	// Set mouse
	if( gpItemPointer->exists() )
	{
		guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
		gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

		MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
		MSYS_SetCurrentCursor( EXTERN_CURSOR );

		fMapInventoryItem=TRUE;
		fTeamPanelDirty=TRUE;
	}
}

void HandleTacticalReload()
{
	SOLDIERTYPE *pSoldier;
	INT16 bAPs = 0;

	if (gusSelectedSoldier != NOBODY)
		pSoldier = MercPtrs[gusSelectedSoldier];
	else
		return;

	// Do we have the ammo to reload?
	// Check APs to reload...
	bAPs = GetAPsToAutoReload(pSoldier);

	if (EnoughPoints(pSoldier, bAPs, 0, TRUE))
	{
		// OK, we have some ammo we can reload.... reload now!
		if (!AutoReload(pSoldier))
		{
			// Do we say we could not reload gun...?
			pSoldier->DoMercBattleSound(BATTLE_SOUND_CURSE1);
			//TacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_AMMO );
		}

		// ATE: Re-examine cursor info!
		gfUIForceReExamineCursorData = TRUE;
	}
}

extern OBJECTTYPE	*gpItemDescOrigAttachmentObject;

void HandleTacticalTransformScope(void)
{
	UINT16 usItem;
	TransformInfoStruct *pTransformation;
	OBJECTTYPE* pObj;

	SOLDIERTYPE* pSoldier;
	if (!GetSoldier(&pSoldier, gusSelectedSoldier))
		return;

	if (!pSoldier->inv[HANDPOS].exists())
		return;

	// ATE: Don't do this if in a fire animation.....
	if (gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_FIRE)
		return;

	// don't do this when EDB is active
	if (gpItemDescObject)
		return;

	usItem = pSoldier->inv[HANDPOS].usItem;
	//GetMouseMapPos(&usGridNo);

	// check attachments
	attachmentList::iterator iter;
	pObj = &(pSoldier->inv[HANDPOS]);

	// check all attachments, search for available transformations
	for (iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
	{
		if (iter->exists() && Item[iter->usItem].usItemClass == IC_MISC && Item[iter->usItem].attachmentclass & AC_SCOPE)
		{
			usItem = Item[iter->usItem].uiIndex;

			// check flashlight transformation
			if (FindScopeTransformation(usItem, &pTransformation) &&
				pTransformation->usResult[0] != 0 &&
				Item[usItem].scopemagfactor != Item[pTransformation->usResult[0]].scopemagfactor)
			{
				if (pTransformation->usAPCost > 0 && gTacticalStatus.uiFlags & INCOMBAT && gTacticalStatus.uiFlags & TURNBASED)
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s (%d AP)", pTransformation->szMenuRowText, pTransformation->usAPCost);
				else
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pTransformation->szMenuRowText);

				// let's pretend we are calling this from EDB
				gpItemDescObject = pObj;
				gpItemDescOrigAttachmentObject = &(*iter);

				gpItemDescOrigAttachmentObject->TransformObject(pSoldier, 0, pTransformation, gpItemDescObject);

				// cleanup
				gpItemDescObject = NULL;
				gpItemDescOrigAttachmentObject = NULL;

				pSoldier->aiData.bShownAimTime = 0;
				DirtyMercPanelInterface(pSoldier, DIRTYLEVEL2);
				gfUIForceReExamineCursorData = TRUE;

				break;
			}
		}
	}
}

void HandleTacticalTransformLaser(void)
{
	UINT16 usItem;
	TransformInfoStruct *pTransformation;
	OBJECTTYPE* pObj;

	SOLDIERTYPE* pSoldier;
	if (!GetSoldier(&pSoldier, gusSelectedSoldier))
		return;

	if (!pSoldier->inv[HANDPOS].exists())
		return;

	// ATE: Don't do this if in a fire animation.....
	if (gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_FIRE)
		return;

	// don't do this when EDB is active
	if (gpItemDescObject)
		return;

	usItem = pSoldier->inv[HANDPOS].usItem;

	// check attachments
	attachmentList::iterator iter;
	pObj = &(pSoldier->inv[HANDPOS]);

	// check all attachments, search for available transformations
	for (iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
	{
		if (iter->exists() && Item[iter->usItem].usItemClass == IC_MISC && Item[iter->usItem].attachmentclass & AC_LASER)
		{
			usItem = Item[iter->usItem].uiIndex;

			// check flashlight transformation
			if (FindLaserTransformation(usItem, &pTransformation) &&
				pTransformation->usResult[0] != 0 &&
				Item[usItem].bestlaserrange != Item[pTransformation->usResult[0]].bestlaserrange)
			{
				if (pTransformation->usAPCost > 0 && gTacticalStatus.uiFlags & INCOMBAT && gTacticalStatus.uiFlags & TURNBASED)
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s (%d AP)", pTransformation->szMenuRowText, pTransformation->usAPCost);
				else
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pTransformation->szMenuRowText);

				// let's pretend we are calling this from EDB
				gpItemDescObject = pObj;
				gpItemDescOrigAttachmentObject = &(*iter);

				gpItemDescOrigAttachmentObject->TransformObject(pSoldier, 0, pTransformation, gpItemDescObject);

				// cleanup
				gpItemDescObject = NULL;
				gpItemDescOrigAttachmentObject = NULL;

				DirtyMercPanelInterface(pSoldier, DIRTYLEVEL2);
				gfUIForceReExamineCursorData = TRUE;

				break;
			}
		}
	}
}

void HandleTacticalTransformFlashlight(void)
{
	UINT16 usItem;
	TransformInfoStruct *pTransformation;
	OBJECTTYPE* pObj;

	SOLDIERTYPE* pSoldier;
	if (!GetSoldier(&pSoldier, gusSelectedSoldier))
		return;

	if (!pSoldier->inv[HANDPOS].exists())
		return;

	// ATE: Don't do this if in a fire animation.....
	if (gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_FIRE)
		return;

	// don't do this when EDB is active
	if (gpItemDescObject)
		return;

	usItem = pSoldier->inv[HANDPOS].usItem;

	// first try a transformation of item
	if (FindTransformation(usItem, &pTransformation) &&
		pTransformation->usResult[0] != 0 &&
		Item[usItem].usFlashLightRange != Item[pTransformation->usResult[0]].usFlashLightRange)
	{
		if (pTransformation->usAPCost > 0 && gTacticalStatus.uiFlags & INCOMBAT && gTacticalStatus.uiFlags & TURNBASED)
			ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s (%d AP)", pTransformation->szMenuRowText, pTransformation->usAPCost);
		else
			ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pTransformation->szMenuRowText);

		// let's pretend we are calling this from EDB
		gpItemDescObject = &pSoldier->inv[HANDPOS];

		gpItemDescObject->TransformObject(pSoldier, 0, pTransformation, NULL);

		// cleanup
		gpItemDescObject = NULL;

		DirtyMercPanelInterface(pSoldier, DIRTYLEVEL2);
		gfUIForceReExamineCursorData = TRUE;

		return;
	}

	// check attachments
	attachmentList::iterator iter;
	pObj = &(pSoldier->inv[HANDPOS]);

	// check all attachments, search for available transformations
	for (iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter)
	{
		if (iter->exists() && Item[iter->usItem].usItemClass == IC_MISC)
		{
			usItem = Item[iter->usItem].uiIndex;

			// check flashlight transformation
			if (FindTransformation(usItem, &pTransformation) &&
				pTransformation->usResult[0] != 0 &&
				Item[usItem].usFlashLightRange != Item[pTransformation->usResult[0]].usFlashLightRange)
			{
				if (pTransformation->usAPCost > 0 && gTacticalStatus.uiFlags & INCOMBAT && gTacticalStatus.uiFlags & TURNBASED)
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s (%d AP)", pTransformation->szMenuRowText, pTransformation->usAPCost);
				else
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pTransformation->szMenuRowText);

				// let's pretend we are calling this from EDB
				gpItemDescObject = pObj;
				gpItemDescOrigAttachmentObject = &(*iter);

				gpItemDescOrigAttachmentObject->TransformObject(pSoldier, 0, pTransformation, gpItemDescObject);

				// cleanup
				gpItemDescObject = NULL;
				gpItemDescOrigAttachmentObject = NULL;

				DirtyMercPanelInterface(pSoldier, DIRTYLEVEL2);
				gfUIForceReExamineCursorData = TRUE;

				break;
			}
		}
	}
}

BOOLEAN FindLaserTransformation(UINT16 usItem, TransformInfoStruct **pTransformation)
{
	// find transformation
	for (INT32 x = 0; x < MAXITEMS; x++)
	{
		if (Transform[x].usItem == (UINT16)-1)
		{
			break;
		}
		if (Transform[x].usItem == usItem &&
			Transform[x].usResult[0] != 0 &&
			Item[Transform[x].usResult[0]].usItemClass & IC_MISC &&
			Item[Transform[x].usResult[0]].attachmentclass & AC_LASER)
		{
			*pTransformation = &Transform[x];
			return TRUE;
		}
	}
	return FALSE;
}

BOOLEAN FindScopeTransformation(UINT16 usItem, TransformInfoStruct **pTransformation)
{
	INT32 iFoundIndex;
	FLOAT scope;

	// search for higher magnification first
	iFoundIndex = MAXITEMS;
	scope = Item[usItem].scopemagfactor;
	for (INT32 x = 0; x < MAXITEMS; x++)
	{
		if (Transform[x].usItem == (UINT16)-1)
		{
			break;
		}

		if (Transform[x].usItem == usItem &&
			Transform[x].usResult[0] != 0 &&
			Item[Transform[x].usResult[0]].usItemClass & IC_MISC &&
			Item[Transform[x].usResult[0]].attachmentclass & AC_SCOPE &&
			Item[Transform[x].usResult[0]].scopemagfactor > Item[usItem].scopemagfactor &&
			(iFoundIndex == MAXITEMS || Item[Transform[x].usResult[0]].scopemagfactor < scope))
		{
			scope = Item[Transform[x].usResult[0]].scopemagfactor;
			iFoundIndex = x;
		}
	}
	if (iFoundIndex != MAXITEMS)
	{
		*pTransformation = &Transform[iFoundIndex];
		return TRUE;
	}

	// search for lowest magnification
	iFoundIndex = MAXITEMS;
	scope = Item[usItem].scopemagfactor;
	for (INT32 x = 0; x < MAXITEMS; x++)
	{
		if (Transform[x].usItem == (UINT16)-1)
		{
			break;
		}

		if (Transform[x].usItem == usItem &&
			Transform[x].usResult[0] != 0 &&
			Item[Transform[x].usResult[0]].usItemClass & IC_MISC &&
			Item[Transform[x].usResult[0]].attachmentclass & AC_SCOPE &&
			Item[Transform[x].usResult[0]].scopemagfactor < scope)
		{
			scope = Item[Transform[x].usResult[0]].scopemagfactor;
			iFoundIndex = x;
		}
	}
	if (iFoundIndex != MAXITEMS)
	{
		*pTransformation = &Transform[iFoundIndex];
		return TRUE;
	}

	// search for any transformation
	for (INT32 x = 0; x < MAXITEMS; x++)
	{
		if (Transform[x].usItem == (UINT16)-1)
		{
			break;
		}

		if (Transform[x].usItem == usItem &&
			Transform[x].usResult[0] != 0 &&
			Item[Transform[x].usResult[0]].usItemClass & IC_MISC &&
			Item[Transform[x].usResult[0]].attachmentclass & AC_SCOPE)
		{
			*pTransformation = &Transform[x];
			return TRUE;
		}
	}

	return FALSE;
}
