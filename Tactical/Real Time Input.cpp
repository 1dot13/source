#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "debug.h"
#include "math.h"
#include "jascreens.h"
#include "pathai.h"
//#include "Soldier Control.h"
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
#include "Interactive Tiles.h"
#include "spread burst.h"
#include "world items.h"
#include "interface items.h"
#include "physics.h"
#include "ui cursors.h"
#include "strategicmap.h"
#include "soldier profile.h"
#include "soldier create.h"
#include "soldier add.h"
#include "dialogue control.h"
#include "interface dialogue.h"
#include "interactive tiles.h"
#include "messageboxscreen.h"
#include "gameloop.h"
#include "spread burst.h"
#include "explosion control.h"
#include "message.h"
#include "Strategic Exit GUI.h"
#include "Assignments.h"
#include "Map Screen Interface.h"
#include "renderworld.h"
#include	"GameSettings.h"
#include "english.h"
#include "text.h"
#include "soldier macros.h"
#include "render dirty.h"
#include "Militia Control.h"
#include "render dirty.h"
#include "Militia Control.h"
///***dddd
#include "Squads.h"
#include "Interface Panels.h"
#include "Soldier functions.h"
#include "SaveLoadMap.h"
#include "worlddat.h" //for gtileset
#include "Debug Control.h" //for livelog

#include "SkillMenu.h"						// sevenfm: need this for TraitsMenu

#include "Vehicles.h"	// anv: for switching from soldier to vehicle

#endif


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


extern UIKEYBOARD_HOOK					gUIKeyboardHook;
extern BOOLEAN	fRightButtonDown;
extern BOOLEAN	fLeftButtonDown;
extern BOOLEAN fIgnoreLeftUp;
extern UINT32	guiCurrentEvent;
extern UINT32 guiCurrentUICursor;
extern void DetermineWhichAssignmentMenusCanBeShown( void );
extern void DetermineWhichMilitiaControlMenusCanBeShown( void );
extern BOOLEAN gfIgnoreOnSelectedGuy;
extern INT32 gsOverItemsGridNo;
extern INT16 gsOverItemsLevel;
extern UINT32	guiUITargetSoldierId;
extern BOOLEAN	gfUIShowExitSouth;
//***ddd
extern BOOLEAN	fMiddleButtonDown;
extern BOOLEAN	fX1ButtonDown;
extern BOOLEAN	fX2ButtonDown;
extern SOLDIERTYPE *gpSMCurrentMerc;


BOOLEAN	gfStartLookingForRubberBanding	= FALSE;
UINT16	gusRubberBandX									= 0;
UINT16	gusRubberBandY									= 0;


BOOLEAN	gfBeginBurstSpreadTracking = FALSE;

BOOLEAN	gfRTClickLeftHoldIntercepted = FALSE;
BOOLEAN	gfRTHaveClickedRightWhileLeftDown = FALSE;

extern BOOLEAN ValidQuickExchangePosition( );

extern INT32		iSMPanelButtons[ NUM_SM_BUTTONS ];
extern INT32		iTEAMPanelButtons[ NUM_TEAM_BUTTONS ];
extern INT32		giSMStealthButton;

void	QueryRTMButton( UINT32 *puiNewEvent );
void	QueryRTWheels( UINT32 *puiNewEvent );
void	QueryRTX1Button( UINT32 *puiNewEvent );
void	QueryRTX2Button( UINT32 *puiNewEvent );

// sevenfm: new mouse commands
void HandleAltMouseRTWheel( void );
void HandleAltMouseRTMButton( UINT32 *puiNewEvent );
void HandleAltMouseRTX1Button( UINT32 *puiNewEvent );
void HandleAltMouseRTX2Button( UINT32 *puiNewEvent );

// sevenfm: original mouse commands functionality
void HandleMouseRTWheel( void );
void HandleMouseRTMButton( UINT32 *puiNewEvent );
void HandleMouseRTX1Button( UINT32 *puiNewEvent );
void HandleMouseRTX2Button( UINT32 *puiNewEvent );

// sevenfm: common functionality
void HandleRTToggleFireMode( void );
void HandleRTJumpThroughWindow( void );
void HandleRTJump( void );
void HandleRTLook( UINT32 *puiNewEvent );
void HandleRTLocateSoldier( void );

extern void SetScopeMode( INT32 usMapPos );

extern void HandleStealthChangeFromUIKeys( );
extern void HandleTBReloadAll( void );
extern void HandleTBEnterTurnbased( void );
extern void HandleTBToggleSneak( void );
extern void HandleTBSoldierRun( void );
extern void HandleTBPickUpBackpacks( void );
extern void HandleTBDropBackpacks( void );
extern void HandleTBLocatePrevMerc( void );
extern void HandleTBLocateNextMerc( void );
extern void HandleTBSwapGoogles( void );
extern void HandleTBSwapSidearm( void );
extern void HandleTBSwapKnife( void );
extern void HandleTBSwapGunsling( void );
extern void HandleTBSwapHands( void );
extern void HandleTBChangeLevel( void );
extern void HandleTBCycleThroughKnownEnemies( void );
extern void HandleTBGotoHigherStance( void );
extern void HandleTBCycleThroughVisibleEnemies( void );
extern void HandleTBCycleThroughKnownEnemiesBackward( void );
extern void HandleTBGotoLowerStance( void );
extern void HandleTBCycleThroughVisibleEnemiesBackward( void );
extern void HandleTBLocateSoldier( void );
extern void HandleTBToggleFormation( void );
extern void HandleTBToggleTrapNetworkView( void );
extern void HandleTBShowMines( void );
extern void HandleTBToggleStealthAll( void );
extern void HandleTBShowLOS( void );
extern void HandleTBShowCover( void );
extern void HandleTBReload( void );
extern void HandleTBToggleStealth( void );

void	GetRTMouseButtonInput( UINT32 *puiNewEvent )
{
	QueryRTLeftButton( puiNewEvent );
	QueryRTRightButton( puiNewEvent );
	QueryRTMButton( puiNewEvent );
	QueryRTWheels( puiNewEvent );
	QueryRTX1Button( puiNewEvent  );
	QueryRTX2Button( puiNewEvent  );
}

void	QueryRTLeftButton( UINT32 *puiNewEvent )
{
	UINT16	usSoldierIndex;
	SOLDIERTYPE *pSoldier;
	SOLDIERTYPE *pVehicle;
	UINT32	uiMercFlags;
	static	UINT32 uiSingleClickTime;
	INT32 usMapPos;
	static BOOLEAN	fDoubleClickIntercepted = FALSE;
	static BOOLEAN	fValidDoubleClickPossible = FALSE;
	static BOOLEAN	fCanCheckForSpeechAdvance = FALSE;
	static INT32		sMoveClickGridNo					= 0;
	UINT16 usSubjectSoldier = gusSelectedSoldier;

	// LEFT MOUSE BUTTON
	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		if (!GetMouseMapPos( &usMapPos ) && !gfUIShowExitSouth )
		{
			return;
		}

		if ( gusSelectedSoldier != NOBODY )
		{
			if ( MercPtrs[ gusSelectedSoldier ]->pTempObject != NULL )
			{
				return;
			}
			if ( MercPtrs[ gusSelectedSoldier ]->flags.uiStatusFlags & SOLDIER_DRIVER )
			{
				pVehicle = GetSoldierStructureForVehicle( MercPtrs[ usSubjectSoldier ]->iVehicleId );
				usSubjectSoldier = pVehicle->ubID;
			}
		}

		if ( gViewportRegion.ButtonState & MSYS_LEFT_BUTTON )
		{
			if ( !fLeftButtonDown )
			{
				fLeftButtonDown = TRUE;
				gfRTHaveClickedRightWhileLeftDown = FALSE;
				RESETCOUNTER( LMOUSECLICK_DELAY_COUNTER );
				RESETCOUNTER( RUBBER_BAND_START_DELAY );

				if ( giUIMessageOverlay == -1 )
				{
					if ( gpItemPointer == NULL )
					{
						switch( gCurrentUIMode )
						{
						case ACTION_MODE:

							if ( gusSelectedSoldier != NOBODY )
							{
								if(	GetSoldier( &pSoldier, gusSelectedSoldier ) && gpItemPointer == NULL )
								{
									// OK, check for needing ammo
									if ( HandleUIReloading( pSoldier ) )
									{
										gfRTClickLeftHoldIntercepted = TRUE;
										//fLeftButtonDown				= FALSE;
									}
									else
									{
										if ( pSoldier->bDoBurst )
										{
											pSoldier->sStartGridNo = usMapPos;
											ResetBurstLocations( );
											*puiNewEvent = A_CHANGE_TO_CONFIM_ACTION;
										}
										else
										{
											gfRTClickLeftHoldIntercepted = TRUE;

											if ( UIMouseOnValidAttackLocation( pSoldier )	)
											{
												// OK< going into confirm will call a function that will automatically move
												// us to shoot in most vases ( grenades need a confirm mode regardless )
												*puiNewEvent = A_CHANGE_TO_CONFIM_ACTION;
												//*puiNewEvent = CA_MERC_SHOOT;
												PauseRT( FALSE );
											}
										}
									}
								}
							}
							break;


						case MOVE_MODE:

							gfUICanBeginAllMoveCycle = TRUE;

							if ( !HandleCheckForExitArrowsInput( FALSE ) && gpItemPointer == NULL )
							{
								if ( gfUIFullTargetFound && ( guiUIFullTargetFlags & OWNED_MERC )	)
								{
									// Reset , if this guy is selected merc, reset any multi selections...
									if ( gusUIFullTargetID == gusSelectedSoldier )
									{
										ResetMultiSelection( );
									}
								}
								else
								{
									INT8 bReturnCode;

									bReturnCode = HandleMoveModeInteractiveClick( usMapPos, puiNewEvent );

									if ( bReturnCode == -1 )
									{
										//gfRTClickLeftHoldIntercepted = TRUE;
									}
									else if ( bReturnCode == -2 )
									{
										//if ( gGameSettings.fOptions[ TOPTION_RTCONFIRM ] )
										//{
										//	*puiNewEvent = C_WAIT_FOR_CONFIRM;
										//	gfPlotNewMovement = TRUE;
										//}/
										//else
									}
									else if ( bReturnCode == -3 )
									{
										gfRTClickLeftHoldIntercepted = TRUE;
									}
									else if ( bReturnCode == 0 )
									{
										{
											{
												BOOLEAN fResult;

												if ( usSubjectSoldier != NOBODY )
												{
													if ( ( fResult = UIOKMoveDestination( MercPtrs[ usSubjectSoldier ], usMapPos ) ) == 1 )
													{
														if ( gsCurrentActionPoints != 0 )
														{
															// We're on terrain in which we can walk, walk
															// If we're on terrain,
															if ( !gGameSettings.fOptions[ TOPTION_RTCONFIRM ]	)
															{
																*puiNewEvent = C_WAIT_FOR_CONFIRM;
																gfPlotNewMovement = TRUE;
															}
														}
													}
												}
											}
										}
									}
								}
								//gfRTClickLeftHoldIntercepted = TRUE;
							}
							else
							{
								gfRTClickLeftHoldIntercepted = TRUE;
								fIgnoreLeftUp = TRUE;
							}

							break;
						}
					}
				}
				if ( gfUIWaitingForUserSpeechAdvance )
				{
					fCanCheckForSpeechAdvance = TRUE;
				}

			}

			if ( gpItemPointer == NULL )
			{
				if ( giUIMessageOverlay == -1 && !gfRTHaveClickedRightWhileLeftDown )
				{
					// HERE FOR CLICK-DRAG CLICK
					switch( gCurrentUIMode )
					{

					case MOVE_MODE:
					case CONFIRM_MOVE_MODE:

						// First check if we clicked on a guy, if so, make selected if it's ours
						if(	FindSoldierFromMouse( &usSoldierIndex, &uiMercFlags ) )
						{
							// Select guy
							if ( (uiMercFlags & SELECTED_MERC) && !( uiMercFlags & UNCONSCIOUS_MERC ) && !( MercPtrs[ usSoldierIndex ]->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
							{
								*puiNewEvent = M_CHANGE_TO_ADJPOS_MODE;
							}
						}
						else
						{
							//if ( COUNTERDONE( RUBBER_BAND_START_DELAY )	)
							{
								// OK, change to rubber banding mode..
								// Have we started this yet?
								if ( !gfStartLookingForRubberBanding && !gRubberBandActive )
								{
									gfStartLookingForRubberBanding	= TRUE;
									gusRubberBandX									= gusMouseXPos;
									gusRubberBandY									= gusMouseYPos;
								}
								else
								{
									// Have we moved....?
									if ( abs( gusMouseXPos - gusRubberBandX ) > 10 || abs( gusMouseYPos - gusRubberBandY ) > 10 )
									{
										gfStartLookingForRubberBanding = FALSE;

										// Stop scrolling:
										gfIgnoreScrolling = TRUE;

										// Anchor cursor....
										RestrictMouseToXYXY( 0, 0, gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y );

										// OK, settup anchor....
										gRubberBandRect.iLeft = gusRubberBandX;
										gRubberBandRect.iTop	= gusRubberBandY;

										gRubberBandActive			= TRUE;

										// ATE: If we have stopped scrolling.....
										if ( gfScrollInertia != FALSE	)
										{
											SetRenderFlags( RENDER_FLAG_FULL | RENDER_FLAG_CHECKZ );

											// Restore Interface!
											RestoreInterface( );

											// Delete Topmost blitters saved areas
											DeleteVideoOverlaysArea( );

											gfScrollInertia = FALSE;
										}

										*puiNewEvent = RB_ON_TERRAIN;
										return;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
		else
		{
			if ( fLeftButtonDown	)
			{
				if ( !fIgnoreLeftUp )
				{
					// set flag for handling single clicks
					// OK , FOR DOUBLE CLICKS - TAKE TIME STAMP & RECORD EVENT
					if ( ( GetJA2Clock() - uiSingleClickTime ) < 300 )
					{
						// CHECK HERE FOR DOUBLE CLICK EVENTS
						if ( fValidDoubleClickPossible )
						{
							if ( gpItemPointer == NULL )
							{
								fDoubleClickIntercepted = TRUE;

								// First check if we clicked on a guy, if so, make selected if it's ours
								if( usSubjectSoldier != NOBODY )
								{
									// Set movement mode
									// OK, only change this if we are stationary!
									//if ( gAnimControl[ MercPtrs[ gusSelectedSoldier ]->usAnimState ].uiFlags & ANIM_STATIONARY )
									//if ( MercPtrs[ gusSelectedSoldier ]->usAnimState == WALKING )
									{
										MercPtrs[ usSubjectSoldier ]->flags.fUIMovementFast = TRUE;
										*puiNewEvent = C_MOVE_MERC;
									}
								}
							}
						}

					}

					// Capture time!
					uiSingleClickTime = GetJA2Clock();

					fValidDoubleClickPossible = FALSE;

					if ( !fDoubleClickIntercepted )
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
						if ( COUNTERDONE( LMOUSECLICK_DELAY_COUNTER ) && gpItemPointer != NULL )
						{
							// LEFT CLICK-HOLD EVENT
							// Switch on UI mode
							switch( gCurrentUIMode )
							{
							case CONFIRM_ACTION_MODE:
							case ACTION_MODE:

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
										gfRTClickLeftHoldIntercepted = TRUE;
									}
								}
								break;

							}
						}
						//else
						{
							//LEFT CLICK NORMAL EVENT
							// Switch on UI mode
							if ( !gfRTClickLeftHoldIntercepted )
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
											if ( gfUIWaitingForUserSpeechAdvance && fCanCheckForSpeechAdvance )
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
													if(	FindSoldierFromMouse( &usSoldierIndex, &uiMercFlags ) )
													{
														// Select guy
														if ( uiMercFlags & OWNED_MERC )
														{
															*puiNewEvent = I_SELECT_MERC;
														}

													}
													break;

												case HANDCURSOR_MODE:

													HandleHandCursorClick( usMapPos, puiNewEvent );
													break;

												case ACTION_MODE:

													//*puiNewEvent = A_CHANGE_TO_CONFIM_ACTION;
													//if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
													//{
													//	pSoldier->sStartGridNo = sMapPos;
													//	ResetBurstLocations( );
													//}
													*puiNewEvent = CA_MERC_SHOOT;
													break;

												case CONFIRM_MOVE_MODE:

													if ( usSubjectSoldier != NOBODY )
													{
														if ( MercPtrs[ usSubjectSoldier ]->usAnimState != RUNNING )
														{
															*puiNewEvent = C_MOVE_MERC;
														}
														else
														{
															MercPtrs[ usSubjectSoldier ]->flags.fUIMovementFast = 2;
															*puiNewEvent = C_MOVE_MERC;
														}
													}

													//*puiNewEvent = C_MOVE_MERC;

													//if ( gGameSettings.fOptions[ TOPTION_RTCONFIRM ] )
													{
														fValidDoubleClickPossible = TRUE;
													}
													break;

												case CONFIRM_ACTION_MODE:

													// Check if we are stationary
													//if ( AimCubeUIClick( ) )
													//{
													//	if ( gusSelectedSoldier != NOBODY )
													//	{
													//		if ( !( gAnimControl[ MercPtrs[ gusSelectedSoldier ]->usAnimState ].uiFlags & ANIM_STATIONARY ) )
													//		{

													//gUITargetShotWaiting	= TRUE;
													//gsUITargetShotGridNo	= sMapPos;
													//		}
													//		else
													//		{
													//	*puiNewEvent = CA_MERC_SHOOT;
													//		}
													*puiNewEvent = CA_MERC_SHOOT;
													//	}
													//}
													break;

												case MOVE_MODE:

													if ( !HandleCheckForExitArrowsInput( FALSE ) && gpItemPointer == NULL )
													{
														// First check if we clicked on a guy, if so, make selected if it's ours
														if ( gfUIFullTargetFound && ( guiUIFullTargetFlags & OWNED_MERC )	)
														{
															if ( !( guiUIFullTargetFlags & UNCONSCIOUS_MERC ) )
															{
																// Select guy
																if(	GetSoldier( &pSoldier, gusUIFullTargetID ) && gpItemPointer == NULL )
																{
																	if( pSoldier->bAssignment >= ON_DUTY && !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
																	{
																		PopupAssignmentMenuInTactical( pSoldier );
																	}
																	else
																	{
																		if ( !_KeyDown( ALT ) )
																		{
																			ResetMultiSelection( );
																			*puiNewEvent = I_SELECT_MERC;
																		}
																		else
																		{
																			if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED )
																			{
																				pSoldier->flags.uiStatusFlags &= (~SOLDIER_MULTI_SELECTED );
																			}
																			else
																			{
																				pSoldier->flags.uiStatusFlags |= (SOLDIER_MULTI_SELECTED );
																				// Say Confimation...
																				if( !gGameSettings.fOptions[ TOPTION_MUTE_CONFIRMATIONS ] )
																					pSoldier->DoMercBattleSound( BATTLE_SOUND_ATTN1 );

																				// OK, if we have a selected guy.. make him part too....
																				if ( gusSelectedSoldier != NOBODY )
																				{
																					MercPtrs[ gusSelectedSoldier ]->flags.uiStatusFlags |= (SOLDIER_MULTI_SELECTED );
																				}
																			}

																			gfIgnoreOnSelectedGuy = TRUE;


																			EndMultiSoldierSelection( FALSE );
																		}
																	}
																}
																else
																{
																	if ( !_KeyDown( ALT ) )
																	{
																		ResetMultiSelection( );
																		*puiNewEvent = I_SELECT_MERC;
																	}
																	else
																	{
																		if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTI_SELECTED )
																		{
																			pSoldier->flags.uiStatusFlags &= (~SOLDIER_MULTI_SELECTED );
																		}
																		else
																		{
																			pSoldier->flags.uiStatusFlags |= (SOLDIER_MULTI_SELECTED );
																			// Say Confimation...
																			if( !gGameSettings.fOptions[ TOPTION_MUTE_CONFIRMATIONS ] )
																				pSoldier->DoMercBattleSound( BATTLE_SOUND_ATTN1 );
																		}

																		// OK, if we have a selected guy.. make him part too....
																		if ( gusSelectedSoldier != NOBODY )
																		{
																			MercPtrs[ gusSelectedSoldier ]->flags.uiStatusFlags |= (SOLDIER_MULTI_SELECTED );
																		}

																		gfIgnoreOnSelectedGuy = TRUE;

																		EndMultiSoldierSelection( FALSE );
																	}
																}
															}
															gfRTClickLeftHoldIntercepted = TRUE;
														}
														else
														{
															INT8 bReturnCode;

															bReturnCode = HandleMoveModeInteractiveClick( usMapPos, puiNewEvent );

															if ( bReturnCode == -1 )
															{
																gfRTClickLeftHoldIntercepted = TRUE;
															}
															else if ( bReturnCode == -2 )
															{
																//if ( gGameSettings.fOptions[ TOPTION_RTCONFIRM ] )
																//{
																//	*puiNewEvent = C_WAIT_FOR_CONFIRM;
																//	gfPlotNewMovement = TRUE;
																//}/
																//else
																{
																			  INT32 sIntTileGridNo;

																	if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
																	{
																		BeginDisplayTimedCursor( GetInteractiveTileCursor( guiCurrentUICursor, TRUE ), 300 );

																		if ( pSoldier->usAnimState != RUNNING )
																		{
																			*puiNewEvent = C_MOVE_MERC;
																		}
																		else
																		{
																			if ( GetCurInteractiveTileGridNo( &sIntTileGridNo ) != NULL )
																			{
																				pSoldier->flags.fUIMovementFast = TRUE;
																				*puiNewEvent = C_MOVE_MERC;
																			}
																		}
																		fValidDoubleClickPossible = TRUE;
																	}
																}
															}
															else if ( bReturnCode == 0 )
															{
																if(	GetSoldier( &pSoldier, usSubjectSoldier ) )
																{
																	// First check if we clicked on a guy, if so, make selected if it's ours
																	if(	FindSoldierFromMouse( &usSoldierIndex, &uiMercFlags ) && ( uiMercFlags & OWNED_MERC ) )
																	{
																		// Select guy
																		*puiNewEvent = I_SELECT_MERC;
																		gfRTClickLeftHoldIntercepted = TRUE;
																	}
																	else
																	{
																		//if ( FindBestPath( pSoldier, sMapPos, pSoldier->pathing.bLevel, pSoldier->usUIMovementMode, NO_COPYROUTE, 0 ) == 0 )
																		if (gsCurrentActionPoints == 0 && !gfUIAllMoveOn && !gTacticalStatus.fAtLeastOneGuyOnMultiSelect )
																		{
																			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
																			gfRTClickLeftHoldIntercepted = TRUE;
																		}
																		else
																		{
																			BOOLEAN fResult;

																			if ( usSubjectSoldier != NOBODY )
																			{
																				if ( ( fResult = UIOKMoveDestination( MercPtrs[ usSubjectSoldier ], usMapPos ) ) == 1 )
																				{
																					if ( gfUIAllMoveOn )
																					{
																						// ATE: Select everybody in squad and make move!
																						{
#if 0
																							SOLDIERTYPE *		pTeamSoldier;
																							INT32						cnt;
																							SOLDIERTYPE			*pFirstSoldier = NULL;

																							// OK, loop through all guys who are 'multi-selected' and
																							// check if our currently selected guy is amoung the
																							// lucky few.. if not, change to a guy who is...
																							cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
																							for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pTeamSoldier++ )
																							{
																								// Default turn off
																								pTeamSoldier->flags.uiStatusFlags &= (~SOLDIER_MULTI_SELECTED );

																								// If controllable
																								if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == MercPtrs[ gusSelectedSoldier ]->bAssignment )
																								{
																									pTeamSoldier->flags.uiStatusFlags |= SOLDIER_MULTI_SELECTED;
																								}
																							}
																							EndMultiSoldierSelection( FALSE );
#endif

																							// Make move!
																							*puiNewEvent = C_MOVE_MERC;

																							fValidDoubleClickPossible = TRUE;
																						}
																					}
																					else
																					{
																						// We're on terrain in which we can walk, walk
																						// If we're on terrain,
																						if ( gGameSettings.fOptions[ TOPTION_RTCONFIRM ]	)
																						{
																							*puiNewEvent = C_WAIT_FOR_CONFIRM;
																							gfPlotNewMovement = TRUE;
																						}
																						else
																						{
																							*puiNewEvent = C_MOVE_MERC;
																							fValidDoubleClickPossible = TRUE;
																						}
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
																						//		sMoveClickGridNo					= sMapPos;

																						//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANT_MOVE_THERE_STR ] );

																						//		*puiNewEvent					= M_CHANGE_TO_HANDMODE;
																						//		gsOverItemsGridNo				= sMapPos;
																						//		gsOverItemsLevel				= gsInterfaceLevel;
																						//	}
																						//	else
																						//	{
																						//		sMoveClickGridNo = 0;
																						//		*puiNewEvent = M_CHANGE_TO_HANDMODE;
																						//	}
																					}
																					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Invalid move destination." );

																					// Goto hand cursor mode...
																					//*puiNewEvent = M_CHANGE_TO_HANDMODE;

																					gfRTClickLeftHoldIntercepted = TRUE;
																				}
																			}
																		}
																	}
																}
															}
														}
														//gfRTClickLeftHoldIntercepted = TRUE;
													}
													else
													{
														gfRTClickLeftHoldIntercepted = TRUE;
													}


#if 0
													fDone = FALSE;
													if(	GetSoldier( &pSoldier, gusUIFullTargetID ) && gpItemPointer == NULL )
													{
														if( ( guiUIFullTargetFlags & OWNED_MERC ) && ( guiUIFullTargetFlags & VISIBLE_MERC ) && !( guiUIFullTargetFlags & DEAD_MERC ) &&( pSoldier->bAssignment >= ON_DUTY )&&!( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
														{
															fShowAssignmentMenu = TRUE;
															gfRTClickLeftHoldIntercepted = TRUE;
															CreateDestroyAssignmentPopUpBoxes( );
															SetTacticalPopUpAssignmentBoxXY( );
															DetermineBoxPositions( );
															DetermineWhichAssignmentMenusCanBeShown( );
															fFirstClickInAssignmentScreenMask = TRUE;
															gfIgnoreScrolling = TRUE;

															fDone = TRUE;
														}
														else
														{
															fShowAssignmentMenu = FALSE;
															CreateDestroyAssignmentPopUpBoxes( );
															DetermineWhichAssignmentMenusCanBeShown( );
														}
													}

													if( fDone == TRUE )
													{
														break;
													}
#endif
													break;


												case LOOKCURSOR_MODE:
													// If we cannot actually do anything, return to movement mode
													*puiNewEvent = LC_LOOK;
													break;

												case JUMPOVER_MODE:

													*puiNewEvent = JP_JUMP;
													break;

												case TALKCURSOR_MODE:

													PauseRT( FALSE );

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

												case RUBBERBAND_MODE:

													EndRubberBanding( );
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
				}

				// Reset flag
				fLeftButtonDown = FALSE;
				fIgnoreLeftUp	= FALSE;
				gfRTClickLeftHoldIntercepted = FALSE;
				fDoubleClickIntercepted = FALSE;
				fCanCheckForSpeechAdvance = FALSE;
				gfStartLookingForRubberBanding = FALSE;

				// Reset counter
				RESETCOUNTER( LMOUSECLICK_DELAY_COUNTER );

			}

		}

	}
	else
	{
		// Set mouse down to false
		//fLeftButtonDown = FALSE;

		//fCanCheckForSpeechAdvance = FALSE;

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

void	QueryRTRightButton( UINT32 *puiNewEvent )
{
	static BOOLEAN	fClickHoldIntercepted = FALSE;
	static BOOLEAN	fClickIntercepted = FALSE;
	static UINT32		uiSingleClickTime;
	static BOOLEAN	fDoubleClickIntercepted = FALSE;
	static BOOLEAN	fValidDoubleClickPossible = FALSE;

	SOLDIERTYPE *pSoldier;
	INT32 usMapPos;

	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		if (!GetMouseMapPos( &usMapPos ) )
		{
			return;
		}

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
				//fIgnoreLeftUp = TRUE;
				gfRTHaveClickedRightWhileLeftDown = TRUE;

				if ( gpItemPointer == NULL )
				{

					// ATE:
					if ( gusSelectedSoldier != NOBODY )
					{
						switch( gCurrentUIMode )
						{
						case CONFIRM_MOVE_MODE:
						case MOVE_MODE:

							if ( !gfUIAllMoveOn )
							{
								fValidDoubleClickPossible	= TRUE;

								// OK, our first right-click is an all-cycle
								if ( gfUICanBeginAllMoveCycle )
								{
									// ATE: Here, check if we can do this....
									if ( !UIOKMoveDestination( MercPtrs[ gusSelectedSoldier ], usMapPos ) )
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANT_MOVE_THERE_STR ] );
										gfRTClickLeftHoldIntercepted = TRUE;
									}
									//else if ( gsCurrentActionPoints == 0 )
									//{
									//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
									//	gfRTClickLeftHoldIntercepted = TRUE;
									//}
									else
									{
										*puiNewEvent = M_CYCLE_MOVE_ALL;
									}
								}
								fClickHoldIntercepted = TRUE;
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
				// IF HERE, DO A CLICK-HOLD IF IN INTERVAL
				if ( COUNTERDONE( RMOUSECLICK_DELAY_COUNTER ) && !fClickHoldIntercepted )
				{
					if ( gpItemPointer == NULL )
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

							if ( GetSoldier( &pSoldier, gusSelectedSoldier ) )
							{
								if ( ( guiUIFullTargetFlags & OWNED_MERC ) && ( guiUIFullTargetFlags & VISIBLE_MERC ) && !( guiUIFullTargetFlags & DEAD_MERC )&&!( pSoldier ?	pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE : 0 ) )
								{
									//if( pSoldier->bAssignment >= ON_DUTY )
									{
										PopupAssignmentMenuInTactical( pSoldier );
										fClickHoldIntercepted = TRUE;
									}
									break;
								}
								else
								{
									fShowAssignmentMenu = FALSE;
									CreateDestroyAssignmentPopUpBoxes( );
									DetermineWhichAssignmentMenusCanBeShown( );

									fShowMilitiaControlMenu = FALSE;
									CreateDestroyMilitiaControlPopUpBoxes( );
									DetermineWhichMilitiaControlMenusCanBeShown( );
								}

								// ATE:
								if ( !fClickHoldIntercepted )
								{
									*puiNewEvent = U_MOVEMENT_MENU;
									fClickHoldIntercepted = TRUE;
								}
								break;
							}
						}

						if ( gCurrentUIMode == ACTION_MODE || gCurrentUIMode == TALKCURSOR_MODE )
						{
							PauseRT( FALSE );
						}
					}
				}

			}

		}
		else
		{
			if ( fRightButtonDown )
			{
				// OK , FOR DOUBLE CLICKS - TAKE TIME STAMP & RECORD EVENT
				if ( ( GetJA2Clock() - uiSingleClickTime ) < 300 )
				{
					// CHECK HERE FOR DOUBLE CLICK EVENTS
					if ( fValidDoubleClickPossible )
					{
						fDoubleClickIntercepted = TRUE;

						// Do stuff....
						// OK, check if left button down...
						if ( fLeftButtonDown )
						{
							if ( gpItemPointer == NULL )
							{
								if ( !fClickIntercepted && !fClickHoldIntercepted )
								{
									// ATE:
									if ( gusSelectedSoldier != NOBODY )
									{
										//fIgnoreLeftUp = TRUE;
										switch( gCurrentUIMode )
										{
										case CONFIRM_MOVE_MODE:
										case MOVE_MODE:

											if ( gfUIAllMoveOn )
											{
												// OK, now we wish to run!
												gfUIAllMoveOn = 2;
											}
										}
									}

								}
							}
						}
					}
				}

				// Capture time!
				uiSingleClickTime = GetJA2Clock();

				fValidDoubleClickPossible = TRUE;

				if ( !fDoubleClickIntercepted )
				{
					// CHECK COMBINATIONS
					if ( fLeftButtonDown )
					{
						if ( gpItemPointer == NULL )
						{
							if ( !fClickHoldIntercepted && !fClickIntercepted )
							{
								// ATE:
								if ( gusSelectedSoldier != NOBODY )
								{
									//fIgnoreLeftUp = TRUE;
									switch( gCurrentUIMode )
									{
									case CONFIRM_MOVE_MODE:
									case MOVE_MODE:

										if ( gfUIAllMoveOn )
										{
											gfUIAllMoveOn = FALSE;
											gfUICanBeginAllMoveCycle = TRUE;
										}

									}
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
									else
									{
										// Switch on UI mode
										switch( gCurrentUIMode )
										{
										case IDLE_MODE:

											break;

										case CONFIRM_MOVE_MODE:
										case MOVE_MODE:
										case TALKCURSOR_MODE:

											// We have here a change to action mode
											*puiNewEvent = M_CHANGE_TO_ACTION;
											fClickIntercepted = TRUE;
											break;

										case ACTION_MODE:

											// We have here a change to move mode
											*puiNewEvent = A_END_ACTION;
											fClickIntercepted = TRUE;
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

										case HANDCURSOR_MODE:
											// If we cannot actually do anything, return to movement mode
											*puiNewEvent = A_CHANGE_TO_MOVE;
											break;

										case LOOKCURSOR_MODE:

											// If we cannot actually do anything, return to movement mode
											*puiNewEvent = A_CHANGE_TO_MOVE;
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
				fDoubleClickIntercepted = FALSE;


				// Reset counter
				RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );

			}

		}
	}

}


extern BOOLEAN ConfirmActionCancel( INT32 usMapPos, INT32 usOldMapPos );

extern BOOLEAN	gUIActionModeChangeDueToMouseOver;


void GetRTMousePositionInput( UINT32 *puiNewEvent )
{
	INT32 usMapPos;
	static INT32			usOldMapPos = 0;
	static UINT32			uiMoveTargetSoldierId = NOBODY;
	SOLDIERTYPE								*pSoldier;
	static BOOLEAN		fOnValidGuy = FALSE;


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
		case RUBBERBAND_MODE:

			// ATE: Make sure!
			if ( gRubberBandActive == FALSE )
			{
				*puiNewEvent = M_ON_TERRAIN;
			}
			else
			{
				*puiNewEvent = RB_ON_TERRAIN;
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

		case LOCKUI_MODE:
			*puiNewEvent = LU_ON_TERRAIN;
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

			if ( HandleOpenDoorMenu( ) )
			{
				*puiNewEvent = A_CHANGE_TO_MOVE;
			}
			break;

		case HANDCURSOR_MODE:

			*puiNewEvent = HC_ON_TERRAIN;
			break;

		case MOVE_MODE:

			if ( usMapPos != usOldMapPos )
			{
				// Set off ALL move....
				gfUIAllMoveOn = FALSE;
			}

			uiMoveTargetSoldierId = NOBODY;

			// Check for being on terrain
			if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
			{
				UINT16 usItem;
				UINT8					ubItemCursor;

				// Alrighty, check what's in our hands = if a 'friendly thing', like med kit, look for our own guys
				usItem = pSoldier->inv[HANDPOS].usItem;

				// get cursor for item
				ubItemCursor	=	GetActionModeCursor( pSoldier );

				//
				if ( IsValidJumpLocation( pSoldier, usMapPos, TRUE ) )
				{
					*puiNewEvent = JP_ON_TERRAIN;
					gsJumpOverGridNo = usMapPos;
					return;
				}
				else
				{
					if( gfUIFullTargetFound )
					{
						if ( IsValidTalkableNPC( (UINT8)gusUIFullTargetID, FALSE, FALSE, FALSE ) && !_KeyDown( SHIFT ) && !AM_AN_EPC( pSoldier ) && MercPtrs[ gusUIFullTargetID ]->bTeam != ENEMY_TEAM && !ValidQuickExchangePosition( ) )
						{
							uiMoveTargetSoldierId = gusUIFullTargetID;
							*puiNewEvent = T_CHANGE_TO_TALKING;
							return;
						}
						else if ( ubItemCursor == AIDCURS )
						{
							// IF it's an ememy, goto confirm action mode
							if ( ( guiUIFullTargetFlags & OWNED_MERC ) && ( guiUIFullTargetFlags & VISIBLE_MERC ) && !( guiUIFullTargetFlags & DEAD_MERC ) )
							{
								//uiMoveTargetSoldierId = gusUIFullTargetID;
								//*puiNewEvent = A_ON_TERRAIN;
								//return;
							}
						}
						else
						{
							// IF it's an ememy, goto confirm action mode
							if ( ( guiUIFullTargetFlags & ENEMY_MERC ) && ( guiUIFullTargetFlags & VISIBLE_MERC ) && !( guiUIFullTargetFlags & DEAD_MERC ) )
							{
								uiMoveTargetSoldierId = gusUIFullTargetID;
								*puiNewEvent = A_ON_TERRAIN;
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

		case CONFIRM_MOVE_MODE:

			if ( usMapPos != usOldMapPos )
			{
				// Switch event out of confirm mode
				// Set off ALL move....
				gfUIAllMoveOn = FALSE;

				*puiNewEvent = A_CHANGE_TO_MOVE;
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
			break;

		}

		//if ( gCurrentUIMode != CONFIRM_ACTION_MODE )
		//{
		//	if(	GetSoldier( &pSoldier, gusSelectedSoldier ) )
		//	{
		// Change refine value back to 1
		///		pSoldier->aiData.bShownAimTime = REFINE_AIM_1;
		//	}
		//}

		usOldMapPos = usMapPos;

	}
}

//***13.02.2008*** постановка в тактике мешков с песком
//usSubIndex может быть от 1 до 10
//void PlaceSandbag (UINT16 usSubIndex)
//{
//	INT16		sGridNo;
//	UINT16		usIndex, cnt;
//	INT8		bOverTerrainType;
//	UINT32		uiType;
//	//SECTORINFO	*pSector;
//
//	/*if( (gTacticalStatus.uiFlags & INCOMBAT) && (gTacticalStatus.uiFlags & TURNBASED) )
//	{
//		return;
//	}
//
//	if( gWorldSectorX != -1 && gWorldSectorY != -1 && gWorldSectorX != 0 && gWorldSectorY != 0 && 
//		NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) > 0 )
//	{
//		return;
//	}
//
//	if( gbWorldSectorZ > 0 || gsInterfaceLevel > 0)
//	{
//		return;
//	}
//
//	for(uiType = 0; uiType < giNumberOfTileTypes; uiType++)
//	{
//		if( !gTilesets[ giCurrentTilesetID].TileSurfaceFilenames[ uiType ][0] )
//		{
//			if( !_strnicmp(gTilesets[ 0 ].TileSurfaceFilenames[ uiType ], "sandbag.sti", 11) )
//				break;
//		}
//		else
//		{
//			if( !_strnicmp(gTilesets[ giCurrentTilesetID ].TileSurfaceFilenames[ uiType ], "sandbag.sti", 11) )
//				break;
//		}
//	}
//
//	if(uiType >= giNumberOfTileTypes)
//		return;*/
//
//	if( GetMouseMapPos( &sGridNo ) )
//	{
//		//if( InARoom( sGridNo, NULL ) )
//		//	return;
//
//		//bOverTerrainType = GetTerrainType( sGridNo );
//		//if( bOverTerrainType == MED_WATER || bOverTerrainType == DEEP_WATER || bOverTerrainType == LOW_WATER )
//		//	return;
//
//		//pSector = &SectorInfo[ SECTOR(gWorldSectorX, gWorldSectorY) ];
//		//if( pSector->bUSUSED <= 0 ) //проверяем число доступных мешков
//		//{
//		//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"No Sandbags");
//		//}
//usIndex =1;
//		//GetTileIndexFromTypeSubIndex( SECONDLARGEEXPDEBRIS, usSubIndex, &usIndex ); //только для 0 тайлсета
//GetTileIndexFromTypeSubIndex( 150, usSubIndex, &usIndex ); //только для 0 тайлсета
//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"No Sandbags");
//		//GetTileIndexFromTypeSubIndex( SECONDLARGEEXPDEBRIS, usSubIndex, &usIndex );
//		ApplyMapChangesToMapTempFile( TRUE );
//		AddStructToHead( sGridNo, usIndex);
//		//if( pSector->bUSUSED <= 0 || !AddStructToHead( sGridNo, usIndex ) )
//		//{
//			//перебор для удаления всех видов мешков
//		//	for(cnt = 1; cnt <= 10; cnt++)
//		//	{
//		//		GetTileIndexFromTypeSubIndex( uiType, cnt, &usIndex );
//		//		if(RemoveStruct( sGridNo, usIndex ))
//		//		{
//		//			RecompileLocalMovementCosts(sGridNo);
//		//			pSector->bUSUSED += 1;
//		//			break;
//		//		}
//		//	}
//		//}
//		//else
//		//{
//			RecompileLocalMovementCosts(sGridNo);
////			pSector->bUSUSED -= 1;
//		//}
//		ApplyMapChangesToMapTempFile( FALSE );
//		SetRenderFlags( RENDER_FLAG_FULL );
//	}
//
//}
//
//
//

void	QueryRTMButton( UINT32 *puiNewEvent )
{
	INT32	sMapPos;

	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
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

			if(gGameExternalOptions.bAlternateMouseCommands)
				HandleAltMouseRTMButton( puiNewEvent );
			else
				HandleMouseRTMButton( puiNewEvent );

			fMiddleButtonDown = FALSE;
			// Reset counter
			//RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );
		}
	}
}//void	QueryRTMButton( UINT32 *puiNewEvent )


void	QueryRTWheels( UINT32 *puiNewEvent )
{
	INT32		sMapPos=0;

	gViewportRegion.WheelState = gViewportRegion.WheelState * ( gGameSettings.fOptions[TOPTION_INVERT_WHEEL] ? -1 : 1 );
	if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		if (!GetMouseMapPos( &sMapPos ) )
			return;
		if ( gViewportRegion.WheelState != 0 )
		{
//			printf("wheel %d\n", gViewportRegion.WheelState);MessageBeep(0x00000040L);
			if ( gpItemPointer == NULL )
			{
				if ( gusSelectedSoldier != NOBODY )
				{
					// Switch on UI mode
					switch( gCurrentUIMode )
					{
						case IDLE_MODE:
						case MOVE_MODE:
						case HANDCURSOR_MODE:
							if(gGameExternalOptions.bAlternateMouseCommands)
								HandleAltMouseRTWheel();
							else
								HandleMouseRTWheel();
							break;
						case ACTION_MODE:
						case CONFIRM_MOVE_MODE:						
						case LOOKCURSOR_MODE:
						case TALKCURSOR_MODE:
						case MENU_MODE:
						break;
						case CONFIRM_ACTION_MODE: //стрелять здесь. но до сюда не доходит ;)
								break;
					}//switch
				}//if ( gusSelectedSoldier != NOBODY )
			}//if ( gpItemPointer == NULL 
			ResetWheelState( &gViewportRegion );
		}//if ( gViewportRegion.WheelState != 0 )
	}
}//void	QueryRTWheels( UINT32 *puiNewEvent )



void QueryRTX1Button( UINT32 *puiNewEvent  )
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
			}
		}
		else if ( fX1ButtonDown )
		{
			fX1ButtonDown = FALSE;

			if(gGameExternalOptions.bAlternateMouseCommands)
				HandleAltMouseRTX1Button( puiNewEvent );
			else
				HandleMouseRTX1Button( puiNewEvent );
		}
	}
}
void QueryRTX2Button( UINT32 *puiNewEvent )
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
			}
		}
		else if ( fX2ButtonDown )
		{
			fX2ButtonDown = FALSE;
			if(gGameExternalOptions.bAlternateMouseCommands)
				HandleAltMouseRTX2Button( puiNewEvent );
			else
				HandleMouseRTX2Button( puiNewEvent );
		}
	}
}

// sevenfm: new mouse commands
void HandleAltMouseRTWheel( void )
{
	if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )	&&
		( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ NEXTMERC_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
	{
		if ( gViewportRegion.WheelState > 0 )	// wheel up
		{
			if ( _KeyDown( ALT ) )				
				if( _KeyDown( SHIFT ) )	
				{
					if( _KeyDown( CTRL ) )			// SHIFT+CTRL+ALT
						HandleTBPickUpBackpacks();
					else							// SHIFT+ALT
						CycleThroughKnownEnemies(); 
				}
				else if ( _KeyDown( CTRL ) )		// CTRL+ALT
					HandleTBSwapGoogles();
				else								// ALT
					HandleTBGotoHigherStance();
			else if( _KeyDown( CTRL ) )
			{
				if( _KeyDown( SHIFT ) )				// SHIFT+CTRL
					HandleTBSwapGunsling();
				else								// CTRL
					HandleTBSoldierRun();
			}
			else if( _KeyDown( SHIFT ) )			// SHIFT
				HandleTBCycleThroughVisibleEnemies();
			else									
				HandleTBLocatePrevMerc();
		}
		else										// wheel down
		{
			if ( _KeyDown( ALT ) )				
				if( _KeyDown( SHIFT ) )	
				{
					if( _KeyDown( CTRL ) )			// SHIFT+CTRL+ALT
						HandleTBDropBackpacks();
					else							// SHIFT+ALT
						CycleThroughKnownEnemies( TRUE );
				}
				else if ( _KeyDown( CTRL ) )		// CTRL+ALT
					HandleTBSwapSidearm();
				else								// ALT
					HandleTBGotoLowerStance();
			else if( _KeyDown( CTRL ) )
			{
				if( _KeyDown( SHIFT ) )				// SHIFT+CTRL
					HandleTBSwapKnife();
				else								// CTRL
					HandleTBSwapHands();
			}
			else if( _KeyDown( SHIFT ) )			// SHIFT
				HandleTBCycleThroughVisibleEnemiesBackward();
			else									
				HandleTBLocateNextMerc();
		}
	}
}
void HandleAltMouseRTMButton( UINT32 *puiNewEvent )
{
	INT32 usMapPos;
	GetMouseMapPos( &usMapPos );

	if ( _KeyDown( ALT ) )				
		if( _KeyDown( SHIFT ) )	
		{
			if( _KeyDown( CTRL ) )			// SHIFT+CTRL+ALT
				HandleRTJumpThroughWindow();
			else							// SHIFT+ALT
				;// reserved
		}
		else if ( _KeyDown( CTRL ) )		// CTRL+ALT
			HandleTBChangeLevel();
		else								// ALT
			HandleRTToggleFireMode();
	else if( _KeyDown( CTRL ) )
	{
		if( _KeyDown( SHIFT ) )				// SHIFT+CTRL
			HandleRTJump();
		else								// CTRL
			SetScopeMode( usMapPos );
	}
	else if( _KeyDown( SHIFT ) )			// SHIFT
		HandleTBLocateSoldier();
	else									// Button
		HandleRTLook( puiNewEvent );
	
}
void HandleAltMouseRTX1Button( UINT32 *puiNewEvent )
{
	if ( _KeyDown( ALT ) )				
		if( _KeyDown( SHIFT ) )	
		{
			if( _KeyDown( CTRL ) )		// SHIFT+CTRL+ALT
				HandleTBToggleSneak();
			else						// SHIFT+ALT
				HandleTBToggleFormation();
		}
		else if ( _KeyDown( CTRL ) )	// CTRL+ALT
			HandleTBSwapGoogles();
		else							// ALT
			HandleTBSwapKnife();
	else if( _KeyDown( CTRL ) )
	{
		if( _KeyDown( SHIFT ) )			// SHIFT+CTRL
			HandleTBEnterTurnbased();
		else							// CTRL
			HandleTBSwapSidearm();
	}
	else if( _KeyDown( SHIFT ) )		// SHIFT
		HandleTBSwapGunsling();
	else								// Button
		HandleTBSwapHands();
	
}
void HandleAltMouseRTX2Button( UINT32 *puiNewEvent )
{
	if ( _KeyDown( ALT ) )				
		if( _KeyDown( SHIFT ) )	
		{
			if( _KeyDown( CTRL ) )		// SHIFT+CTRL+ALT
				HandleTBToggleTrapNetworkView();
			else						// SHIFT+ALT
				HandleTBReloadAll();
		}
		else if ( _KeyDown( CTRL ) )	// CTRL+ALT
			HandleTBShowMines();
		else							// ALT
			HandleTBToggleStealthAll();
	else if( _KeyDown( CTRL ) )
	{
		if( _KeyDown( SHIFT ) )			// SHIFT+CTRL
			HandleTBShowLOS();
		else							// CTRL
			HandleTBShowCover();
	}
	else if( _KeyDown( SHIFT ) )		// SHIFT
		HandleTBReload();
	else								// Button
		HandleTBToggleStealth();	
}

// sevenfm: original mouse commands functionality
void HandleMouseRTWheel( void )
{
	UINT8		bID;
								// nothing in hand and either not in SM panel, or the matching button is enabled if we are in SM panel
								if ( !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )	&&
									( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ NEXTMERC_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
								{
										if ( gViewportRegion.WheelState > 0 ) //колесо от себя
										{
											//change stance ->DOWN
											if ( _KeyDown( ALT ) )
			{	
				if ( (gusSelectedSoldier != NOBODY) && ( gpItemPointer == NULL ) )
												GotoLowerStance(MercPtrs[ gusSelectedSoldier ]);
				return;
											}

											if ( gusSelectedSoldier != NOBODY )
											{ //Select prev merc
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
			{	
				if ( (gusSelectedSoldier != NOBODY) && ( gpItemPointer == NULL ) )
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
void HandleMouseRTMButton( UINT32 *puiNewEvent )
{
	if ( _KeyDown( ALT ) )
	{
		// toggle fire mode
		if ( ( gpItemPointer == NULL ) && ( gusSelectedSoldier != NOBODY ) &&
			( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
			ChangeWeaponMode( MercPtrs[ gusSelectedSoldier ] );
			}
	else
		*puiNewEvent = LC_LOOK;
}
void HandleMouseRTX1Button( UINT32 *puiNewEvent )
{
			if ( !_KeyDown( ALT ) && !_KeyDown( SHIFT ))
			{
					UIHandleChangeLevel( NULL );
			}
			else if( _KeyDown( SHIFT ) )
			{
				// WANNE: Jump through window?
				if (gGameExternalOptions.fCanJumpThroughWindows == TRUE )
				{
					INT8	bDirection;
			       	SOLDIERTYPE *lSoldier;

                    if ( GetSoldier( &lSoldier, gusSelectedSoldier ) )
					{
			 			if ( FindWindowJumpDirection( lSoldier, lSoldier->sGridNo, lSoldier->ubDirection, &bDirection ) )
						{
							lSoldier->BeginSoldierClimbWindow(	);
   	                    }
					}
				}
			}
			else if (_KeyDown( ALT ) )
			{
				// Climb on roofs
				SOLDIERTYPE *pjSoldier;
				if ( GetSoldier( &pjSoldier, gusSelectedSoldier ) )
				{
					BOOLEAN	fNearHeigherLevel;
					BOOLEAN	fNearLowerLevel;
					INT8	bDirection;
						// CHRISL: Turn off manual jumping while wearing a backpack
					if (UsingNewInventorySystem() == true && pjSoldier->inv[BPACKPOCKPOS].exists() == true
						//JMich.BackpackClimb
						&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
						&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))

							return;

					// Make sure the merc is not collapsed!
					if (!IsValidStance(pjSoldier, ANIM_CROUCH) )
					{
						if ( pjSoldier->bCollapsed && pjSoldier->bBreath < OKBREATH )
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, gzLateLocalizedString[ 4 ], pjSoldier->name );
						return;
					}

					GetMercClimbDirection( pjSoldier->ubID, &fNearLowerLevel, &fNearHeigherLevel );

					if ( fNearLowerLevel )
						pjSoldier->BeginSoldierClimbDownRoof( );
					
					if ( fNearHeigherLevel )
						pjSoldier->BeginSoldierClimbUpRoof(	);
					
					if ( FindFenceJumpDirection( pjSoldier, pjSoldier->sGridNo, pjSoldier->ubDirection, &bDirection ) )
						pjSoldier->BeginSoldierClimbFence(	);
				}
			}
}

void HandleMouseRTX2Button( UINT32 *puiNewEvent )
{
			if ( _KeyDown( ALT ) )
				AutoReload( MercPtrs[ gusSelectedSoldier ] );
			else
			// Toggle squad's stealth mode.....
			// For each guy on squad...
			{
				SOLDIERTYPE				*pTeamSoldier;
				INT8					bLoop;
				BOOLEAN					fStealthOn = FALSE;

				// Check if at least one guy is on stealth....
				for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
				{
					if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) )
					{
						if ( pTeamSoldier->bStealthMode )
						{
							fStealthOn = TRUE;
						}
					}
				}

				fStealthOn = !fStealthOn;

				for (bLoop=gTacticalStatus.Team[gbPlayerNum].bFirstID, pTeamSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[gbPlayerNum].bLastID; bLoop++, pTeamSoldier++)
				{
					if ( OK_CONTROLLABLE_MERC( pTeamSoldier ) && pTeamSoldier->bAssignment == CurrentSquad( ) && !AM_A_ROBOT( pTeamSoldier ) )
					{
						if ( gpSMCurrentMerc != NULL && bLoop == gpSMCurrentMerc->ubID )
						{
							gfUIStanceDifferent = TRUE;
						}

						pTeamSoldier->bStealthMode = fStealthOn;
					}
				}

				fInterfacePanelDirty = DIRTYLEVEL2;

				// OK, display message
				if ( fStealthOn )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ON_STEALTHMODE ] );
				}
				else
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_OFF_STEALTHMODE ] );
				}
			}
}

// sevenfm: common functionality
void HandleRTLook( UINT32 *puiNewEvent )
{
	*puiNewEvent = LC_LOOK;
}
void HandleRTJump( void )
{
	// Climb on roofs
	SOLDIERTYPE *pjSoldier;
	if ( GetSoldier( &pjSoldier, gusSelectedSoldier ) )
	{
		BOOLEAN	fNearHeigherLevel;
		BOOLEAN	fNearLowerLevel;
		INT8	bDirection;
		// CHRISL: Turn off manual jumping while wearing a backpack
		if (UsingNewInventorySystem() == true && pjSoldier->inv[BPACKPOCKPOS].exists() == true
			//JMich.BackpackClimb
			&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pjSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pjSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
			&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pjSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))

				return;

		// Make sure the merc is not collapsed!
		if (!IsValidStance(pjSoldier, ANIM_CROUCH) )
		{
			if ( pjSoldier->bCollapsed && pjSoldier->bBreath < OKBREATH )
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, gzLateLocalizedString[ 4 ], pjSoldier->name );
			return;
		}

		GetMercClimbDirection( pjSoldier->ubID, &fNearLowerLevel, &fNearHeigherLevel );

		if ( fNearLowerLevel )
			pjSoldier->BeginSoldierClimbDownRoof( );
		
		if ( fNearHeigherLevel )
			pjSoldier->BeginSoldierClimbUpRoof(	);
					
		if ( FindFenceJumpDirection( pjSoldier, pjSoldier->sGridNo, pjSoldier->ubDirection, &bDirection ) )
			pjSoldier->BeginSoldierClimbFence(	);
	}
}
void HandleRTJumpThroughWindow( void )
{
	if (gGameExternalOptions.fCanJumpThroughWindows == TRUE )
	{
		INT8	bDirection;
     	SOLDIERTYPE *lSoldier;

        if ( GetSoldier( &lSoldier, gusSelectedSoldier ) )
		{
			if ( FindWindowJumpDirection( lSoldier, lSoldier->sGridNo, lSoldier->ubDirection, &bDirection ) )
			{
				lSoldier->BeginSoldierClimbWindow(	);
            }
		}
	}
}


void HandleRTToggleFireMode( void )
{
	// toggle fire mode
	if ( ( gpItemPointer == NULL ) && ( gusSelectedSoldier != NOBODY ) &&
		( ( gsCurInterfacePanel != SM_PANEL ) || ( ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
		ChangeWeaponMode( MercPtrs[ gusSelectedSoldier ] );
}
void HandleRTLocateSoldier( void )
{
	if ( gusSelectedSoldier != NOBODY )
	{
		LocateSoldier( gusSelectedSoldier, 10 );
	}
}
