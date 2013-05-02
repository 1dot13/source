	#include "Map Screen Interface.h"
	#include "popupbox.h"
	#include "interface control.h"
	#include "renderworld.h"
	#include "Text.h"
	#include "utilities.h"
	#include "Interface.h"
	#include "GameSettings.h"
	#include "Input.h"

	#include "Console.h"
	#include "lwstring.h"
	#include "message.h"
	#include "mousesystem.h"

	#include "Lua Interpreter.h"
	#include "popup_callback.h"
	#include "popup_class.h"

  //////////////////////////////////////////////////////////////////
 //	functions and variable declarations for binding the class with C callbacks
//////////////////////////////////////////////////////////////////

vector<PopupIndex>	gPopupRegionIndex;
vector<POPUP*>		gPopupIndex;	
UINT32			gPopupRegionIndexCounter = 0;
UINT32			gPopupIndexCounter = 0;
UINT32			POPUP::nextid = 0;

UINT32 POPUP::uiPOPUPBORDERS = 0;
UINT32 POPUP::uiPOPUPTEX = 0;


///////////////////////
// Lua vars
/////////////////

BOOLEAN gfPopupInit = FALSE;

POPUP * gPopup;	// single popup

///////////////////////
// Lua Popup functions
/////////////////

void initPopupIndex(void)
{
	/*
	int i;
	for (i=0; i<MAX_POPUPS; i++)
		{
		gPopupIndex[i] = NULL;
		}
	*/	
}

void reinitPopupIndex(void)
{
/*
	int i;
	for (i=0; i<MAX_POPUPS; i++)
		{
		if (gPopupIndex[i] != NULL)
			{
				gPopupIndex[i]->~POPUP();
				gPopupIndex[i] = NULL;
			}
		}
*/
}

POPUP * findPopupInIndex( UINT32 cID )
{
	for(std::vector<POPUP*>::iterator cPopup=gPopupIndex.begin(); cPopup != gPopupIndex.end(); ++cPopup)
		if ((*cPopup)->id == cID){
			return (*cPopup);
		}

		#ifdef JA2TESTVERSION
			CHAR8 debugStr[120];
			wsprintf(debugStr,"Failed to find popup id: %i \n",cID);
			OutputDebugString( debugStr );
		#endif

	return NULL;
}

void rebuildPopupIndex(void)
{
	/*UINT8 i,j;
	POPUP*		TempPopupIndex[MAX_POPUPS];
	//cout << "cleaning index"<<endl;
	for (i = 0; i < gPopupIndexCounter; i++)
	{
		TempPopupIndex[i] = gPopupIndex[i];
	}
	j=0; // actual  index

	for (i = 0; i < gPopupIndexCounter; i++)
	{
		if (TempPopupIndex[i] != NULL)
		{
			gPopupIndex[j] = TempPopupIndex[i];
			j++;
		}
	}
	// We only have j elements worth keeping.
	gPopupIndexCounter = j;*/
}

  //////////////////////////////////////////////////////////////////
 //	POPUP_OPTION methods
//////////////////////////////////////////////////////////////////

POPUP_OPTION::POPUP_OPTION(void)
{
	this->name = L"unnamed option";

	this->action = 0;
	this->avail = 0;
	this->hover = 0;

	// set highlight color
	this->color_highlight = FONT_WHITE;

	// unhighlighted color
	this->color_foreground = FONT_LTGREEN;

	// background color
	this->color_background = FONT_BLACK;

	// shaded color..for darkened text
	this->color_shade = FONT_GRAY7 ;
}

POPUP_OPTION::~POPUP_OPTION(void)
{
	if(this->action)	delete(this->action);
	if(this->avail)		delete(this->avail);
	if(this->hover)		delete(this->hover);
}


POPUP_OPTION::POPUP_OPTION(std::wstring *newName, popupCallback * newFunction)
{
	this->name = *newName;

	this->action = newFunction;
	this->avail = 0;
	this->hover = 0;

	// set highlight color
	this->color_highlight = FONT_WHITE;

	// unhighlighted color
	this->color_foreground = FONT_LTGREEN;

	// background color
	this->color_background = FONT_BLACK;

	// shaded color..for darkened text
	this->color_shade = FONT_GRAY7 ;
}

BOOLEAN POPUP_OPTION::setName( std::wstring * newName )
{
	this->name = *newName;
	return TRUE;
}

BOOLEAN POPUP_OPTION::setAction( popupCallback *fun )
{
	this->action = fun;

	return TRUE;
}

BOOLEAN POPUP_OPTION::setAvail( popupCallback *fun )
{
	this->avail = fun;

	return TRUE;
}

BOOLEAN POPUP_OPTION::setHover( popupCallback *fun )
{
	this->hover = fun;

	return TRUE;
}


BOOLEAN POPUP_OPTION::checkAvailability()
{
	if (this->avail != 0)
		return ( this->avail->call() == true );
	else if ( this->action == 0 )	// options with no action are always shaded
		return false;
	else
		return true;
}

BOOLEAN POPUP_OPTION::runHoverCallback(MOUSE_REGION * pRegion)
{
	// TODO: pass the mouse region data to the function (if it can accept it)
	if (this->hover != 0)
		return ( this->hover->call() == true );
	else
		return true;
}

BOOLEAN POPUP_OPTION::run()
{
	// check if it's aaliable at this time
	if ( this->action != 0 && this->checkAvailability() )
	{
		this->action->call();

		return TRUE;
	}
	else
		return FALSE;
}

BOOLEAN POPUP_OPTION::forceRun()
{
	if (this->action != 0)
		this->action->call();
	else
		return FALSE;

	return TRUE;
}

  //////////////////////////////////////////////////////////////////
 //	POPUP_SUB_POPUP_OPTION methods
//////////////////////////////////////////////////////////////////

// constructor
POPUP_SUB_POPUP_OPTION::POPUP_SUB_POPUP_OPTION(void) : POPUP_OPTION(new wstring(L"Unnamed subPopup"),NULL)	//TODO: possible memmory leak!
{
	this->parent = NULL;
	this->initSubPopup();
}

POPUP_SUB_POPUP_OPTION::POPUP_SUB_POPUP_OPTION(wstring* name) : POPUP_OPTION(name, NULL)
{
	this->parent = NULL;
	this->initSubPopup();
}

POPUP_SUB_POPUP_OPTION::POPUP_SUB_POPUP_OPTION(wstring* newName, const POPUP * parent) : POPUP_OPTION(newName, NULL)
{
	this->parent = parent;
	this->initSubPopup();
}

// destructor
POPUP_SUB_POPUP_OPTION::~POPUP_SUB_POPUP_OPTION(void)
{

}

void POPUP_SUB_POPUP_OPTION::showPopup()
{
	// check if it's available at this time, if there is an avail function set
	if ( !this->AvailabilityFunctionSet() || this->checkAvailability() )
	{
		if (!this->customPositionSet || this->customRule == POPUP_POSITION_RELATIVE) {
			this->positionSubPopup();
		}

		this->subPopup->show();
	}
}

void POPUP_SUB_POPUP_OPTION::hidePopup()
{
	this->subPopup->hide();
}

void POPUP_SUB_POPUP_OPTION::positionSubPopup()
{
	INT16 x,y;

	if( this->customPositionSet )
	{
		x = this->parent->X + this->parent->Dimensions.iLeft + this->parent->Dimensions.iRight + this->customX;
		y = this->parent->Y + this->customY;
	} else {
		x = this->parent->X + this->parent->Dimensions.iLeft + this->parent->Dimensions.iRight + 10;
		y = this->parent->Y + 10;
	}

	this->subPopup->setPosition(x,y);
}

BOOLEAN POPUP_SUB_POPUP_OPTION::run()
{
	//cout << "running function " << utf8string::as_utf8(this->name.c_str()).c_str() << endl;
	this->showPopup();

	return TRUE;
}

BOOLEAN POPUP_SUB_POPUP_OPTION::setPopupPosition(UINT16 x,	UINT16 y, INT8 positioningRule){
	this->customPositionSet = true;

	if (positioningRule != POPUP_POSITION_RELATIVE){
		return this->subPopup->setPosition(x,y,positioningRule);
	} else {
		this->customX = x;
		this->customY = y;
		this->customRule = positioningRule;
		return this->subPopup->setPosition(this->parent->X + this->parent->Dimensions.iRight + x, this->parent->Y + y,POPUP_POSITION_TOP_LEFT);
	}
}




static void shadeOpenSubPopup( POPUP_SUB_POPUP_OPTION * opt ){
	// make the option yellow while its open
	SetCurrentBox( opt->parent->getBoxId() );
	SetStringForeground( opt->stringHandle, FONT_YELLOW);
}

extern  void RenderTeamRegionBackground();
static void unShadeOpenSubPopup( POPUP_SUB_POPUP_OPTION * opt ){
	// restore the option's color
	SetCurrentBox( opt->parent->getBoxId() );
	SetStringForeground( opt->stringHandle, FONT_LTGREEN);

	// if in mapscreen, redraw the UI
	if( guiCurrentScreen == MAP_SCREEN ){
		fTeamPanelDirty = TRUE;
		fMapPanelDirty = TRUE;
		RenderTeamRegionBackground();
	}

}

void POPUP_SUB_POPUP_OPTION::initSubPopup()
{
	this->subPopup = new POPUP( (CHAR*) wstring(this->name).c_str() );
	this->subPopup->setCallback(POPUP_CALLBACK_SHOW,new popupCallbackFunction<void,POPUP_SUB_POPUP_OPTION*>( &shadeOpenSubPopup, this ));
	this->subPopup->setCallback(POPUP_CALLBACK_HIDE,new popupCallbackFunction<void,POPUP_SUB_POPUP_OPTION*>( &unShadeOpenSubPopup, this ));

	this->customPositionSet = false;

	// set highlight color
	this->color_highlight = FONT_WHITE;

	// unhighlighted color
	this->color_foreground = FONT_LTGREEN;

	// background color
	this->color_background = FONT_BLACK;

	// shaded color..for darkened text
	this->color_shade = FONT_GRAY7 ;
}

void POPUP_SUB_POPUP_OPTION::destroySubPopup()
{
	this->subPopup->~POPUP();
}

  //////////////////////////////////////////////////////////////////
 //	POPUP methods
//////////////////////////////////////////////////////////////////

// constructors
POPUP::POPUP(void)
{
	strcpy((char*) this->name, (char*) "unNamed popup");
	this->optionCount = 0;

	this->addToIndex();
	this->setInitialValues();
}


POPUP::POPUP(CHAR* name)
{
	strcpy((char*) this->name, (char*) name);
	this->optionCount = 0;

	this->addToIndex();
	this->setInitialValues();

	#ifdef JA2TESTVERSION
		CHAR8 debugStr[120];
		wsprintf(debugStr,"Created popup: %s \n",this->name);
		OutputDebugString( debugStr );
	#endif
}

// destructor

POPUP::~POPUP(void)
{
	if(this->PopupVisible)
		this->hide();

	for (UINT16 i = 0; i<this->options.size(); i++)
	{
		this->options[i]->~POPUP_OPTION();
	}

	for (UINT16 i = 0; i<this->subPopupOptions.size(); i++)
	{
		this->subPopupOptions[i]->subPopup->hide();
		this->subPopupOptions[i]->~POPUP_SUB_POPUP_OPTION();
	}

	#ifdef JA2TESTVERSION
		CHAR8 debugStr[120];
		wsprintf(debugStr,"Destroyed popup: %s \n",this->name);
		OutputDebugString( debugStr );
	#endif

	this->removeFromIndex();

	if (this->initCallback) this->initCallback->~popupCallback();
	if (this->ShowCallback) this->ShowCallback->~popupCallback();
	if (this->HideCallback) this->HideCallback->~popupCallback();

	if (this->EndCallback) {
		this->EndCallback->call();
		this->EndCallback->~popupCallback();
	}
}

// callback handlers

BOOLEAN POPUP::setCallback(UINT8 type, popupCallback * callback){
	if(callback == NULL) return FALSE;

	switch(type){
		case POPUP_CALLBACK_INIT: 
			if (this->initCallback) this->initCallback->~popupCallback();
			this->initCallback = callback;
			break;
		case POPUP_CALLBACK_END:  
			if (this->EndCallback) this->EndCallback->~popupCallback();
			this->EndCallback = callback;
			break;
		case POPUP_CALLBACK_SHOW: 
			if (this->ShowCallback) this->ShowCallback->~popupCallback();
			this->ShowCallback = callback;
			break;
		case POPUP_CALLBACK_HIDE: 
			if (this->HideCallback) this->HideCallback->~popupCallback();
			this->HideCallback = callback;
			break;

		default: return false;
	}

	return true;
}
		
BOOLEAN POPUP::isCallbackSet(UINT8 type){

	switch(type){
		case POPUP_CALLBACK_INIT: if (this->initCallback) return true; break;
		case POPUP_CALLBACK_END:  if (this->EndCallback) return true; break;
		case POPUP_CALLBACK_SHOW: if (this->ShowCallback) return true; break;
		case POPUP_CALLBACK_HIDE: if (this->HideCallback) return true; break;

		default: return false;
	}

	return false;
}

// popup index handlers

BOOLEAN POPUP::addToIndex()
{
	/*
	if (gPopupIndexCounter == MAX_POPUPS) 
	{
		rebuildPopupIndex(); // if index full, try to rebuild it before giving up
		if (gPopupIndexCounter == MAX_POPUPS) 
			return FALSE;
	}
	*/

	// assign the ID and increment the counter.
	this->id = this->nextid;
	this->nextid++;

	//gPopupIndex[gPopupIndexCounter] = this;
	gPopupIndex.push_back( this );
	gPopupIndexCounter++;

	return TRUE;
}

BOOLEAN POPUP::removeFromIndex()
{

	for(std::vector<POPUP*>::iterator cPopup=gPopupIndex.begin(); cPopup != gPopupIndex.end(); ++cPopup)
		if ((*cPopup)->id == this->id){
			gPopupIndex.erase(cPopup);
			return TRUE;
		}

	#ifdef JA2TESTVERSION
		CHAR8 debugStr[120];
		wsprintf(debugStr,"Failed to remove popup from index: %i \n",this->id);
		OutputDebugString( debugStr );
	#endif

	return FALSE;
}

// set defaults

void POPUP::setInitialValues(void)
{
	this->Position.iX=20;
	this->Position.iY=20;

	this->OrigPosition.iX = 0	;
	this->OrigPosition.iY = 0 ;

	this->Dimensions.iLeft=0;
	this->Dimensions.iTop=0;
	this->Dimensions.iBottom=100;
	this->Dimensions.iRight=120;

	this->BoxesX =	20;
	this->BoxesY =	20;

		// the x,y position of the pop up in tactical
	this->X =	20;
	this->Y =	20;

	this->positioningRule = POPUP_POSITION_TOP_LEFT;

	//int i = 0;
	//for (i=0; i<POPUP_MAX_OPTIONS; i++)
	//{
	//	this->options[i] = NULL;
	//	//this->MenuRegion[i] = NULL;
	//}

	this->PopupVisible = FALSE;
	this->FirstClickInScreenMask = FALSE;
	this->BoxPersist = FALSE;

	this->subPopupOptionCount = 0;
	this->optionCount = 0;

	this->initCallback = NULL;
	this->EndCallback = NULL;
	this->ShowCallback = NULL;
	this->HideCallback = NULL;
}

// setup functions

POPUP_OPTION *  POPUP::addOption(wstring * name, popupCallback* action)
{	
	if (this->optionCount < POPUP_MAX_OPTIONS)
	{
		this->options.push_back( new POPUP_OPTION ( name, action ) );
		this->optionCount++;
		return this->options.back();
	}
	else
		return NULL;
}
/*
INT16 POPUP::findFreeOptionIndex()
{
	int i;
	for (i=0; i<POPUP_MAX_OPTIONS; i++)
		if ( this->options[i] == NULL )
			return i;

	return -1;
}
*/
BOOLEAN POPUP::addOption(POPUP_OPTION &option)
{
	if (this->optionCount < POPUP_MAX_OPTIONS)
	{	
		this->options.push_back( &option );
		this->optionCount++;
		return TRUE;
	}
	else
		return FALSE;
	
}

POPUP_OPTION * POPUP::getOption(UINT16 n)
{
	if (n < this->options.size() && this->options[n])
		return this->options[n];
	
	return NULL;
}

POPUP * POPUP::addSubMenuOption(wstring * name)
{
	if (this->subPopupOptionCount < POPUP_MAX_SUB_POPUPS)
	{
		this->subPopupOptions.push_back( new POPUP_SUB_POPUP_OPTION ( name, this ) );
		this->subPopupOptionCount++;
		return this->subPopupOptions.back()->subPopup;
	}
	else
		return NULL;
}

BOOL POPUP::addSubMenuOption( POPUP_SUB_POPUP_OPTION* sub )
{
	if (this->subPopupOptionCount < POPUP_MAX_SUB_POPUPS)
	{
		sub->parent = this;
		this->subPopupOptions.push_back( sub );
		this->subPopupOptionCount++;
		return TRUE;
	}
	else
		return FALSE;
}

POPUP_SUB_POPUP_OPTION * POPUP::getSubPopupOption(UINT8 n)
{
	if (n < this->subPopupOptions.size() && this->subPopupOptions[n])
		return this->subPopupOptions[n];
	else
		return NULL;
}

void POPUP::recalculateWidth(void){

	this->Dimensions.iRight = this->getCurrentWidth();

}

INT16 POPUP::getCurrentWidth(void){
	INT16 longestString = 0, longestStringTmp;

	CHAR16 sString[ 128 ];

	if (this->subPopupOptionCount > 0)

		for(std::vector<POPUP_SUB_POPUP_OPTION*>::iterator cOption=this->subPopupOptions.begin(); cOption != this->subPopupOptions.end(); ++cOption)
		{
			swprintf( sString, (*cOption)->name.c_str() );	  
			longestStringTmp = StringPixLength(	sString, MAP_SCREEN_FONT);
			if( longestString < longestStringTmp ) longestString = longestStringTmp;
		}

	if (this->optionCount > 0)
		for(std::vector<POPUP_OPTION*>::iterator cOption=this->options.begin(); cOption != this->options.end(); ++cOption)
		{
			swprintf( sString, (*cOption)->name.c_str() );	  
			longestStringTmp = StringPixLength(	sString, MAP_SCREEN_FONT);
			if( longestString < longestStringTmp ) longestString = longestStringTmp;
		}

	return longestString + 10; // 10+ marginLeft(6) + marginRight(4)
}

INT16 POPUP::getCurrentHeight(void){

	// linespace = 2; margin = 6+4
	return ( this->optionCount + this->subPopupOptionCount ) * ( GetFontHeight(MAP_SCREEN_FONT) + 2 ) +10;

}

SGPRect POPUP::getBoxDimensions(){
	SGPRect tmp;

	GetBoxSize( this->boxId, &tmp );

	return tmp;
}

SGPPoint POPUP::getBoxPosition(){
	SGPPoint tmp;

	GetBoxPosition( this->boxId, &tmp );

	return tmp;
}

BOOLEAN POPUP::setPosition(UINT16 x, UINT16 y, UINT8 positioningRule )
{
	//TODO: check if the desired position is valid.

	// why the hell are there three sets of box positions ?
	this->Position.iX=x;
	this->Position.iY=y;

	this->BoxesX =	x;
	this->BoxesY =	y;

	this->X =	x;
	this->Y =	y;

	// make that four
	this->OrigPosition.iX = x ;
	this->OrigPosition.iY = y ;

	this->positioningRule = positioningRule;

	#ifdef JA2TESTVERSION
		CHAR8 debugStr[120];
		wsprintf(debugStr,"Positioning popup %i at (x/y) %i / %i ; positioning rule: %i \n",this->id, x, y, positioningRule);
		OutputDebugString( debugStr );
	#endif

	return TRUE;
}

//////////////////////////////////
// usage functions

BOOLEAN POPUP::show()
{

	if (this->PopupVisible) {
		// this->RebuildBox();
		return false;
	}

	if (this->ShowCallback) this->ShowCallback->call();

	// can't show an empty popup. 
	if ( (this->optionCount + this->subPopupOptionCount) == 0) return FALSE;

	// set the box to visible
	this->PopupVisible = TRUE;
	// should take care of creating the box
	this->CreatePopUpBoxes();
	// set the position before creating the screen mask and mouse regions
	this->DetermineBoxPositions();
	// create all the GUI areas
	this->CreateScreenMask();
	this->CreateMouseRegions();
	// make SURE the regions have been repositioned
	this->CheckAndUpdateTacticalPopUpPositions();
	// shade unavaliable options
	this->HandleShadingOfLines();
	
	#ifdef JA2TESTVERSION
		CHAR8 debugStr[120];
		wsprintf(debugStr,"Showing popup id: %i \n",this->boxId);
		OutputDebugString( debugStr );
	#endif

	// showtime !
	ShowBox(this->boxId);

	return TRUE;
}

void POPUP::hideAfter(){
	this->hideAfterRun = TRUE;
}

BOOLEAN POPUP::hide()
{
	if (!this->PopupVisible) {

		#ifdef JA2TESTVERSION
			//  __debugbreak();
		#else
			// Assert(false);
		#endif

		return false;
	}


	for (UINT16 i = 0; i<this->subPopupOptions.size(); i++)
	{
		this->subPopupOptions[i]->subPopup->hide();
	}

	#ifdef JA2TESTVERSION
		CHAR8 debugStr[120];
		wsprintf(debugStr,"Hiding popup id: %i \n",this->boxId);
		OutputDebugString( debugStr );
	#endif

	this->DestroyMouseRegions();	
	this->DestroyScreenMask();

	HideBox(this->boxId);
	RemoveBox(this->boxId);

	this->DestroyPopUpBoxes();

	this->boxId = -1;
	this->PopupVisible = FALSE;

	if (this->HideCallback) this->HideCallback->call();

	return TRUE;
}

BOOLEAN POPUP::callOption(int optIndex)
{
	this->hideAfterRun = FALSE;

	if (	this->optionCount > 0 
	&&		optIndex < POPUP_MAX_OPTIONS
	&&		optIndex < optionCount
	&&		optIndex >= 0)
	{	
		BOOLEAN callbackStatus = this->options[optIndex]->run();
		
		if(!this->hideAfterRun)
		{
			return callbackStatus;
		} else
		{
			return this->hide();
		}
	}
	else
		return FALSE;
	
}

BOOLEAN POPUP::forceDraw(){

	if (!this->PopupVisible) {
	// set the box to visible
	this->PopupVisible = TRUE;
	// should take care of creating the box
	this->CreatePopUpBoxes();
	// set the position before creating the screen mask and mouse regions
	this->DetermineBoxPositions();
	// create all the GUI areas
	this->CreateScreenMask();
	this->CreateMouseRegions();
	// make SURE the regions have been repositioned
	this->CheckAndUpdateTacticalPopUpPositions();
	// shade unavaliable options
	this->HandleShadingOfLines();
	

	// showtime !
	ShowBox(this->boxId);
	} else {
		this->RebuildBox();
	}

	DisplayOnePopupBox(this->boxId, FRAME_BUFFER);
	ForceUpDateOfBox(this->boxId);

	return true;
}

////////////////////////////
// private utility functions

//////
// external flag access functions
//////

BOOLEAN POPUP::TeamPanelDirty(void)
{
	extern BOOLEAN fTeamPanelDirty;
	return fTeamPanelDirty;
}
BOOLEAN POPUP::CharacterInfoPanelDirty(void)
{
	extern BOOLEAN fCharacterInfoPanelDirty;
	return fCharacterInfoPanelDirty;
}
BOOLEAN POPUP::MapScreenBottomDirty(void)
{
	extern BOOLEAN fMapScreenBottomDirty;
	return fMapScreenBottomDirty;
}
BOOLEAN POPUP::RenderPBInterface(void)
{
	extern BOOLEAN gfRenderPBInterface ;
	return gfRenderPBInterface ;
}


void POPUP::setTeamPanelDirty(BOOLEAN flag )
{
	extern BOOLEAN fTeamPanelDirty;
	fTeamPanelDirty = flag;
}
void POPUP::setCharacterInfoPanelDirty(BOOLEAN flag )
{
	extern BOOLEAN fCharacterInfoPanelDirty;
	fCharacterInfoPanelDirty = flag;
}
void POPUP::setMapScreenBottomDirty(BOOLEAN flag )
{
	extern BOOLEAN fMapScreenBottomDirty;
	fMapScreenBottomDirty = flag;
}
void POPUP::setRenderPBInterface(BOOLEAN flag )
{
	extern BOOLEAN gfRenderPBInterface ;
	gfRenderPBInterface = flag;
}



//////
// internal box setup methods
//////

// rewrites all the text lines in the box and adjusts
// the box's size and mouse regions to potential changes
// less expensive then hide();show();
void POPUP::RebuildBox( void )
{	if ( this->boxId != -1 && this->id < POPUP::nextid )
	{	
		// make sure we're working with the right popup!
		SetCurrentBox( this->boxId );

		// remove all the 'old' text from the box
		RemoveAllCurrentBoxStrings();

		// add the sub popups (they go first)
		this->AddSubPopupStrings();

		// add the options
		this->AddOptionStrings();

		// set fonts and what not
		this->UpdateTextProperties();

		// adjust the mouse regions (might've changed substantially)
		this->AdjustMouseRegions();

		// shade/unshade accordingly
		this->HandleShadingOfLines();
	} else if ( this->id > POPUP::nextid ) {
		// chances are buggered up data will have impossible boxid

		#ifdef JA2TESTVERSION
			 __debugbreak();
		#else
			Assert(false);
		#endif
	}
}

BOOLEAN POPUP::CreateBox()
{
	// this->Position.iX = this->OrigPosition.iX;

	if( giBoxY != 0 )
	{
		this->Position.iY = giBoxY;
	}

	// create basic box
	CreatePopUpBox(&this->boxId, this->Dimensions, this->Position, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

	// which buffer will box render to
	SetBoxBuffer(this->boxId, FRAME_BUFFER);

	// border type?
	SetBorderType(this->boxId,POPUP::uiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(this->boxId, POPUP::uiPOPUPTEX);

	// margin sizes
	SetMargins(this->boxId, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(this->boxId, 2);

	// set current box to this one
	SetCurrentBox( this->boxId );


	// add the sub popups (they go first)
		
	this->AddSubPopupStrings();

	// add the options
		
	this->AddOptionStrings();

	// set fonts and what not

	this->UpdateTextProperties();

	//UpdateMapScreenthis->Positions();

	return TRUE;
}

void POPUP::AddSubPopupStrings()
{
	UINT32 hStringHandle;
	CHAR16 sString[ 128 ];

	if (this->subPopupOptionCount > 0)

		for(std::vector<POPUP_SUB_POPUP_OPTION*>::iterator cOption=this->subPopupOptions.begin(); cOption != this->subPopupOptions.end(); ++cOption)
		{
			swprintf( sString, (*cOption)->name.c_str() );

			AddMonoString((UINT32 *)&hStringHandle, sString );
	  
			// make sure it is unhighlighted
			UnHighLightLine(hStringHandle);
			// update options string handle for setting colors
			(*cOption)->stringHandle = hStringHandle;
		}
}

void POPUP::AddOptionStrings()
{
	UINT32 hStringHandle;
	CHAR16 sString[ 128 ];

	if (this->optionCount > 0)
		for(std::vector<POPUP_OPTION*>::iterator cOption=this->options.begin(); cOption != this->options.end(); ++cOption)
		{
			swprintf( sString, (*cOption)->name.c_str() );

			AddMonoString((UINT32 *)&hStringHandle, sString );
	  
			// make sure it is unhighlighted
			UnHighLightLine(hStringHandle);
			// update options string handle for setting colors
			(*cOption)->stringHandle = hStringHandle;
		}
}


void POPUP::UpdateTextProperties()
{

	// set font type
	SetBoxFont(this->boxId, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(this->boxId, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(this->boxId, FONT_LTGREEN);

	// background color
	SetBoxBackground(this->boxId, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( this->boxId, FONT_GRAY7 );

	// alt shaded color
	SetBoxSecondaryShade( this->boxId, FONT_YELLOW );


	if (this->subPopupOptionCount > 0)
		for(std::vector<POPUP_SUB_POPUP_OPTION*>::iterator cOption=this->subPopupOptions.begin(); cOption != this->subPopupOptions.end(); ++cOption)
		{
			SetStringForeground( (*cOption)->stringHandle, (*cOption)->color_foreground);
			SetStringBackground( (*cOption)->stringHandle, (*cOption)->color_background);
			SetStringHighLight( (*cOption)->stringHandle, (*cOption)->color_highlight);
			SetStringShade( (*cOption)->stringHandle, (*cOption)->color_shade);
		}


	if (this->optionCount > 0)
		for(std::vector<POPUP_OPTION*>::iterator cOption=this->options.begin(); cOption != this->options.end(); ++cOption)
		{
			SetStringForeground( (*cOption)->stringHandle, (*cOption)->color_foreground);
			SetStringBackground( (*cOption)->stringHandle, (*cOption)->color_background);
			SetStringHighLight( (*cOption)->stringHandle, (*cOption)->color_highlight);
			SetStringShade( (*cOption)->stringHandle, (*cOption)->color_shade);
		}


	 // resize box to text
	ResizeBoxToText( this->boxId );
}

BOOLEAN POPUP::CreateDestroyPopUpBoxes( void )
{
	static BOOLEAN fCreated = FALSE;
	BOOLEAN returnStatus = TRUE;

	if( ( this->PopupVisible == TRUE ) && ( fCreated == FALSE ) )
	{
		returnStatus = this->CreatePopUpBoxes();
		fCreated = TRUE;
	}
	else if( ( this->PopupVisible  == FALSE ) && ( fCreated == TRUE ) )
	{
		returnStatus = this->DestroyPopUpBoxes();
		fCreated = FALSE;		
	}


	return( returnStatus );
}

BOOLEAN POPUP::CreatePopUpBoxes()
{
	VSURFACE_DESC		vs_desc;
	VOBJECT_DESC    VObjectDesc; 

	// load textures
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\popup.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &POPUP::uiPOPUPBORDERS));

	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
	strcpy(vs_desc.ImageFile, "INTERFACE\\popupbackground.pcx");
	CHECKF(AddVideoSurface(&vs_desc, &POPUP::uiPOPUPTEX));

	// does some other stff
	this->CreateBox();
	
	return TRUE;
}

BOOLEAN POPUP::DestroyPopUpBoxes()
{
	if ( gPopupIndex.size() < 2 ){
		DeleteVideoObjectFromIndex(POPUP::uiPOPUPBORDERS);
		DeleteVideoSurfaceFromIndex(POPUP::uiPOPUPTEX);
	}

	gfIgnoreScrolling = FALSE;
	return TRUE;
}

void POPUP::CreateDestroyScreenMask( void )
{
	static BOOLEAN fCreated = FALSE;

	// not created, create
	if( ( fCreated == FALSE ) )
	{
		this->CreateScreenMask();
		fCreated = TRUE;
	}
	else if( ( fCreated == TRUE ) )
	{
		this->DestroyScreenMask();
		fCreated = FALSE;
	}

	return;
}

void POPUP::CreateScreenMask( void )
{
/*	void (POPUP::*ScreenMaskBtnCallbackMethod) (MOUSE_REGION *, INT32 );
	ScreenMaskBtnCallbackMethod = &POPUP::ScreenMaskBtnCallback;
*/
	// will create a screen mask to catch mouse input to disable LPC menus
	

	MSYS_DefineRegion( 	&this->ScreenMaskRegion, 
						0, 
						0, 
						SCREEN_WIDTH, 
						SCREEN_HEIGHT, 
						POPUP_SCREEN_MASK_PRIORITY ,
						MSYS_NO_CURSOR, 
						MSYS_NO_CALLBACK, 
						popupMaskCallback );

	// if in tactical...
	if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN) )
		MSYS_ChangeRegionCursor(  &this->ScreenMaskRegion, 0 );

	if (!registerPopupRegion( this->ScreenMaskRegion.IDNumber, this->id)){
		#ifdef JA2TESTVERSION
			CHAR8 debugStr[120];
			wsprintf(debugStr,"Failed to register screen mask for box id: %i \n",this->boxId);
			OutputDebugString( debugStr );
		#endif
	}

	
	return;
}

void POPUP::DestroyScreenMask( void )
{
	unregisterPopupRegion(this->ScreenMaskRegion.IDNumber);	// unregister it before removing it
																// otherwise the ID might get lost somewhere
	MSYS_RemoveRegion(  &this->ScreenMaskRegion );
	return;
}



void POPUP::CreateDestroyMouseRegions( void )
{
	static BOOLEAN fCreated = FALSE;

	if( ( this->PopupVisible == TRUE ) && ( fCreated == FALSE ) )
	{
		this->CreateMouseRegions();
		fCreated = TRUE;
	}
	else if( ( this->PopupVisible == FALSE ) && ( fCreated == TRUE ) )
	{
		this->DestroyMouseRegions();
		fCreated = FALSE;
	}
}

void POPUP::CreateMouseRegions( void )
{
	UINT32 iCounter = 0;
	UINT32 iTotal = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition={0,0};
	INT32 iBoxWidth = 0;
	SGPRect pDimensions={0,0,0,0};
	static BOOLEAN fShowRemoveMenu = FALSE;

	gfIgnoreScrolling = FALSE;

	if( ( guiCurrentScreen == MAP_SCREEN ) ) 
	{
	  SetBoxPosition( this->boxId, this->Position );
	}

	// grab height of font
	iFontHeight = GetLineSpace( this->boxId ) + GetFontHeight( GetBoxFont( this->boxId ) ); 

	// get x.y position of box
	GetBoxPosition( this->boxId, &pPosition);

	// grab box x and y position
	iBoxXPosition = pPosition.iX;
	iBoxYPosition = pPosition.iY;

	// get dimensions..mostly for width
	GetBoxSize( this->boxId, &pDimensions );

	// get width
	iBoxWidth = pDimensions.iRight;

	SetCurrentBox( this->boxId );
	
	// define regions for sub popups 
	for( iCounter = 0; iCounter < this->subPopupOptionCount ; iCounter++ )
	{
		// add mouse region for each line of text..and set user data
		
		MSYS_DefineRegion(	&this->MenuRegion[ iTotal ], 	
							( INT16 )( iBoxXPosition ), 
							( INT16 )( iBoxYPosition + GetTopMarginSize( this->boxId ) + 
							( iFontHeight ) * iTotal ), 
							( INT16 )( iBoxXPosition + iBoxWidth ), 
							( INT16 )( iBoxYPosition + GetTopMarginSize( this->boxId ) + 
							( iFontHeight ) * ( iTotal + 1 ) ), 
							POPUP_OPTION_PRIORITY ,
							MSYS_NO_CURSOR, 
							popupMouseMoveCallback, 
							popupMouseClickCallback );
	
		MSYS_SetRegionUserData( &this->MenuRegion[ iTotal ], 0, iCounter );
		MSYS_SetRegionUserData( &this->MenuRegion[ iTotal ], 1, REGION_SUB_POPUP );
		MSYS_SetRegionUserData( &this->MenuRegion[ iTotal ], 2, iTotal );
	
		if (!registerPopupRegion( this->MenuRegion[ iTotal ].IDNumber, this->id)){}
	
		iTotal++;
	}

	// define regions for options 
	for( iCounter = 0; iCounter < this->optionCount ; iCounter++ )
	{
		// add mouse region for each line of text..and set user data
		
		MSYS_DefineRegion(	&this->MenuRegion[ iTotal ], 	
							( INT16 )( iBoxXPosition ), 
							( INT16 )( iBoxYPosition + GetTopMarginSize( this->boxId ) + ( iFontHeight ) * iTotal ), 
							( INT16 )( iBoxXPosition + iBoxWidth ), 
							( INT16 )( iBoxYPosition + GetTopMarginSize( this->boxId ) + ( iFontHeight ) * ( iTotal + 1 ) ), 
							POPUP_OPTION_PRIORITY ,
							MSYS_NO_CURSOR, 
							popupMouseMoveCallback, 
							popupMouseClickCallback );
	
		MSYS_SetRegionUserData( &this->MenuRegion[ iTotal ], 0, iCounter );
		MSYS_SetRegionUserData( &this->MenuRegion[ iTotal ], 1, REGION_OPTION );
		MSYS_SetRegionUserData( &this->MenuRegion[ iTotal ], 2, iTotal );
	
		if (!registerPopupRegion( this->MenuRegion[ iTotal ].IDNumber, this->id)){}

		iTotal++;
	}



	// unhighlight all strings in box
	UnHighLightBox( this->boxId );
	this->CheckAndUpdateTacticalPopUpPositions();
}

void POPUP::DestroyMouseRegions( void )
{
	UINT32 iCounter = 0;
	
	for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( this->boxId ); iCounter++ )
	{
		unregisterPopupRegion(this->MenuRegion[ iCounter ].IDNumber);
		MSYS_RemoveRegion( &this->MenuRegion[ iCounter ] );
	}

	SetRenderFlags( RENDER_FLAG_FULL );
}

void POPUP::AdjustMouseRegions( void )
{
	UINT32 iCounter = 0;
	UINT32 iTotal = 0;

	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	INT32 iBoxWidth = 0;

	SGPPoint pPosition={0,0};
	SGPRect pDimensions={0,0,0,0};

	// grab height of font
	iFontHeight = GetLineSpace( this->boxId ) + GetFontHeight( GetBoxFont( this->boxId ) ); 

	// get x.y position of box
	GetBoxPosition( this->boxId, &pPosition);

	// grab box x and y position
	iBoxXPosition = pPosition.iX;
	iBoxYPosition = pPosition.iY;

	// get dimensions..mostly for width
	GetBoxSize( this->boxId, &pDimensions );

	// get width
	iBoxWidth = pDimensions.iRight;

	// adjust regions for sub popups 
	for( iCounter = 0; iCounter < this->subPopupOptionCount ; iCounter++ )
	{
		this->MenuRegion[ iTotal ].RegionTopLeftX = ( INT16 )( iBoxXPosition );
		this->MenuRegion[ iTotal ].RegionTopLeftY = ( INT16 )( iBoxYPosition 
												+ GetTopMarginSize( this->boxId ) 
												+ ( iFontHeight ) * iTotal );

		this->MenuRegion[ iTotal ].RegionBottomRightX = ( INT16 )( iBoxXPosition + iBoxWidth );
		this->MenuRegion[ iTotal ].RegionBottomRightY = ( INT16 )( iBoxYPosition 
												+ GetTopMarginSize( this->boxId ) 
												+ ( iFontHeight ) * ( iTotal + 1 ) );
		iTotal++;
	}

	// adjust regions for options 
	for( iCounter = 0; iCounter < this->optionCount ; iCounter++ )
	{
		this->MenuRegion[ iTotal ].RegionTopLeftX = ( INT16 )( iBoxXPosition );
		this->MenuRegion[ iTotal ].RegionTopLeftY = ( INT16 )( iBoxYPosition 
												+ GetTopMarginSize( this->boxId ) 
												+ ( iFontHeight ) * iTotal );

		this->MenuRegion[ iTotal ].RegionBottomRightX = ( INT16 )( iBoxXPosition + iBoxWidth );
		this->MenuRegion[ iTotal ].RegionBottomRightY = ( INT16 )( iBoxYPosition 
												+ GetTopMarginSize( this->boxId ) 
												+ ( iFontHeight ) * ( iTotal + 1 ) );
		iTotal++;
	}
}

void POPUP::ScreenMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for screen mask region
	
	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) || ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) )
	{
		if( this->FirstClickInScreenMask == TRUE )
		{
			this->FirstClickInScreenMask = FALSE;
			return;
		}

		// button event, stop showing menus
		if (!this->BoxPersist)
		{
			this->hide();
		}

		// update mapscreen
		setTeamPanelDirty(TRUE);
		setCharacterInfoPanelDirty(TRUE);
		setMapScreenBottomDirty(TRUE);
		setRenderPBInterface(TRUE);

		SetRenderFlags( RENDER_FLAG_FULL );
	}
	return;
}

void POPUP::MenuBtnCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for LPC region
	INT32 iValue = -1;
	INT32 iType = 0;
	INT32 iTotal = 0;
	SOLDIERTYPE * pSoldier = NULL;
	UINT8 ubVolume = 10;

	// sanity check #1
	if (!this || this->boxId < 0 || this->id > POPUP::nextid){
		#ifdef JA2TESTVERSION
			 __debugbreak();
		#else
			Assert(false);
		#endif
	}

	iValue = MSYS_GetRegionUserData( pRegion, 0 );
	iType  = MSYS_GetRegionUserData( pRegion, 1 );
	iTotal  = MSYS_GetRegionUserData( pRegion, 2 );

	switch (iType)
	{
	case REGION_OPTION:	
		if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
		{
			UnHighLightBox( this->boxId );

			if (iValue < POPUP_MAX_OPTIONS
			&&	iValue < this->optionCount 
			&&	iValue >= 0
			&&	this->options[iValue] != NULL)
			{
				this->options[iValue]->run();	// run the option's callback

				// sanity check #2
				// if this popup was fine in check #1 but is broken now, chances are 
				// we got deleted (or just plain broken) by the callback. 
				if (!this || this->id > POPUP::nextid){
					#ifdef JA2TESTVERSION
						 __debugbreak();
					#else
						Assert(false);
					#endif
				}


				this->RebuildBox();				// rebuild the box so that the changes can take effect

				if( GetBoxShadeFlag( this->boxId, iTotal ) == FALSE )	// highlight the line again, if appropriate
					HighLightBoxLine( this->boxId, iTotal );
			}
		}
		break;
	
	case REGION_SUB_POPUP:	
		if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
		{
			UnHighLightBox( this->boxId );

			if (iValue < POPUP_MAX_SUB_POPUPS
				&&	iValue < this->subPopupOptionCount 
				&&	iValue >= 0
				&&	this->subPopupOptions[iValue] != NULL)
			{
				this->recalculateWidth();
				this->subPopupOptions[iValue]->run();
			}
		}
		break;
	}
}

void POPUP::MenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for LPC region
	INT32 iTotal = -1;
	INT32 iType = 0;
	INT32 iValue = 0;

	if (!this || this->boxId < 0){
		__debugbreak();
	}
	
	iTotal = MSYS_GetRegionUserData( pRegion, 2 );	// set this to the total value of all mouse regions
	iType  = MSYS_GetRegionUserData( pRegion, 1 );
	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string
		if( GetBoxShadeFlag( this->boxId, iTotal ) == FALSE )
		{
			// get the string line handle
			HighLightBoxLine( this->boxId, iTotal );
			if ( iType == REGION_OPTION && this->options[iValue]->HoverFunctionSet() )	// if it has been set...
				this->options[iValue]->runHoverCallback(pRegion);	// run the custom mouse callback
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( this->boxId );
	}
	else if (iReason & MSYS_CALLBACK_REASON_MOVE)
	{
		// if the hover callback is set then also react to mouse movement within the region (only when available)
		if( iType == REGION_OPTION && this->options[iValue]->HoverFunctionSet() && GetBoxShadeFlag( this->boxId, iTotal ) == FALSE )
		{
			this->options[iValue]->runHoverCallback(pRegion);	// run the custom mouse callback
			
			if ( (pRegion->ButtonState & MSYS_LEFT_BUTTON) || (pRegion->ButtonState & MSYS_RIGHT_BUTTON) )
			{	// if drag detedted,rebuild the boxstrings
				this->RebuildBox();
			}
		}
	}
}

void POPUP::HandleShadingOfLines( void )
{
	// check if valid
	if( ( this->PopupVisible == FALSE ) || ( this->boxId == - 1 ) )
	{
		return;
	}
	
	UINT8 i = 0;
	// subPopups with no options are shaded
	for(std::vector<POPUP_SUB_POPUP_OPTION*>::iterator cOption=this->subPopupOptions.begin(); cOption != this->subPopupOptions.end(); ++cOption)
	{
		if ( (*cOption)->subPopup->optionCount + (*cOption)->subPopup->subPopupOptionCount > 0 )
			UnShadeStringInBox( this->boxId, i);	
		else																
			ShadeStringInBox( this->boxId, i);
		i++;
	}

	i = 0;
	// shade strings past the this->subPopupOptionCount offset
	for(std::vector<POPUP_OPTION*>::iterator cOption=this->options.begin(); cOption != this->options.end(); ++cOption)
	{
		if ( (*cOption)->checkAvailability() )
			UnShadeStringInBox( this->boxId, i+this->subPopupOptionCount);	
		else																
			ShadeStringInBox( this->boxId, i+this->subPopupOptionCount);
		i++;
	}
		
	return;
}

void POPUP::DetermineBoxPositions( void )
{
	// depending on how many boxes there are, reposition as needed
	SGPPoint pPoint;
	SGPPoint pNewPoint;
	SGPRect pDimensions;

	if( ( this->PopupVisible == FALSE ) || ( this->boxId == -1 ) )
	{
		return;
	}

	// get an idea about what size the box ought to be
	pPoint.iX = 0;
	pPoint.iY = 0;

	SetBoxPosition( this->boxId, pPoint );
	GetBoxSize( this->boxId, &pDimensions );

	INT32 boxWidth = pDimensions.iRight;
	INT32 boxHeight = pDimensions.iBottom;

	/*
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		GetBoxPosition( this->boxId, &pPoint );
		this->BoxesX = ( INT16 )pPoint.iX;
		this->BoxesY = ( INT16 )pPoint.iY;
	}
	*/

	switch (this->positioningRule){
		case POPUP_POSITION_TOP_LEFT:

			pPoint.iX = this->OrigPosition.iX;
			pPoint.iY = this->OrigPosition.iY;

			break;

		case POPUP_POSITION_TOP_RIGHT:

			pPoint.iX = this->OrigPosition.iX - boxWidth;
			pPoint.iY = this->OrigPosition.iY;

			break;

		case POPUP_POSITION_BOTTOM_RIGHT:

			pPoint.iX = this->OrigPosition.iX - boxWidth;
			pPoint.iY = this->OrigPosition.iY - boxHeight;

			break;

		case POPUP_POSITION_BOTTOM_LEFT:

			pPoint.iX = this->OrigPosition.iX;
			pPoint.iY = this->OrigPosition.iY - boxHeight;

			break;

		default:
			#ifdef JA2TESTVERSION
				CHAR8 debugStr[120];
				wsprintf(debugStr,"Incorrect positioning rule for box id: %i \n",this->boxId);
				OutputDebugString( debugStr );
			#endif

			// assume default
			pPoint.iX = this->OrigPosition.iX;
			pPoint.iY = this->OrigPosition.iY;

			return;
	}

	// set all the other positions to match this one
	this->Position = pPoint;
	this->BoxesX = pPoint.iX;
	this->BoxesY = pPoint.iY;
	this->X= pPoint.iX;
	this->Y= pPoint.iY;

	SetBoxPosition( this->boxId, pPoint );
	GetBoxSize( this->boxId, &pDimensions );

	// hang it right beside the LPC/EPC box menu
	pNewPoint.iX = pPoint.iX + pDimensions.iRight;
	pNewPoint.iY = pPoint.iY;

	return;
}

void POPUP::CheckAndUpdateTacticalPopUpPositions( void )
{
	SGPRect pDimensions2;
	SGPPoint pPoint;	

	if( this->PopupVisible == FALSE )
	{
		return;
	}

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	GetBoxSize( this->boxId, &pDimensions2 );

	if( this->BoxesX + pDimensions2.iRight  >= SCREEN_WIDTH ) 
	{
		this->BoxesX = ( INT16 ) ( SCREEN_WIDTH - 1 - ( pDimensions2.iRight  ) );
		SetRenderFlags( RENDER_FLAG_FULL );
	}

	if( this->BoxesY + pDimensions2.iBottom  >= SCREEN_HEIGHT-120 )
	{
		this->BoxesY = ( INT16 )( SCREEN_HEIGHT - 121 - ( pDimensions2.iBottom  ) );
		SetRenderFlags( RENDER_FLAG_FULL );
	}

	pPoint.iX = this->BoxesX;
	pPoint.iY = this->BoxesY;

	SetBoxPosition( this->boxId, pPoint );

	this->RepositionMouseRegions( );
}

void POPUP::RepositionMouseRegions( void )
{
	INT16 sDeltaX, sDeltaY;
	INT32 iCounter = 0;

	if( this->PopupVisible == TRUE )
	{
		sDeltaX = this->BoxesX - this->MenuRegion[ 0 ].RegionTopLeftX;
		sDeltaY = ( INT16 ) ( this->BoxesY - this->MenuRegion[ 0 ].RegionTopLeftY + GetTopMarginSize( this->boxId ) );

		// find the delta from the old to the new, and alter values accordingly
		for( iCounter = 0; iCounter < ( INT32 )GetNumberOfLinesOfTextInBox( this->boxId ); iCounter++ )
		{
			this->MenuRegion[ iCounter ].RegionTopLeftX += sDeltaX;
			this->MenuRegion[ iCounter ].RegionTopLeftY += sDeltaY;

			this->MenuRegion[ iCounter ].RegionBottomRightX += sDeltaX;
			this->MenuRegion[ iCounter ].RegionBottomRightY += sDeltaY;
		}

		gfPausedTacticalRenderFlags = RENDER_FLAG_FULL;
	}
}

void POPUP::PositionCursorForBox(void)
{
	// position cursor over y of on duty in tactical assignments
	SGPPoint pPosition;
	SGPRect pDimensions;
	INT32 iFontHeight;

	// get x.y position of box
	GetBoxPosition( this->boxId, &pPosition);

	// get dimensions..mostly for width
	GetBoxSize( this->boxId, &pDimensions );

	iFontHeight = GetLineSpace( this->boxId ) + GetFontHeight( GetBoxFont( this->boxId ) ); 
	
	if( gGameSettings.fOptions[ TOPTION_DONT_MOVE_MOUSE ] == FALSE )
	{
		SimulateMouseMovement( pPosition.iX + pDimensions.iRight - 6, pPosition.iY + ( iFontHeight / 2 ) + 2 );
	}
}

  //////////////////////////////////////////////////////////////////
 //	Functions for binding POPUP class with MSYS callbacks
//////////////////////////////////////////////////////////////////

PopupIndex * findMouseRegionInIndex(UINT16 regionId)
{
	for(std::vector<PopupIndex>::iterator cPopupIndex=gPopupRegionIndex.begin(); cPopupIndex != gPopupRegionIndex.end(); ++cPopupIndex)
		if ((*cPopupIndex).regionId == regionId){
			return &(*cPopupIndex);
		}

		#ifdef JA2TESTVERSION
			CHAR8 debugStr[120];
			wsprintf(debugStr,"Failed to find callback to mouse region id: %i \n",regionId);
			OutputDebugString( debugStr );
		#endif

	return NULL;
}

BOOLEAN registerPopupRegion(UINT16 rID, UINT32 cID)
{
	/*
	if (gPopupRegionIndexCounter == MAX_REGIONS_IN_INDEX)
	{	// If we can't fit in any more regions, try to rebuild the index before giving up
		rebuildPopupRegionIndex();	  // since there may be some unregistered regions
		if (gPopupRegionIndexCounter == MAX_REGIONS_IN_INDEX) 
			return FALSE;	// if index still full, return false
	}
	*/

	PopupIndex * pi = new PopupIndex;
	pi->regionId = rID;
	pi->classId = cID;

	gPopupRegionIndex.push_back(*pi);
	gPopupRegionIndexCounter++;

	return TRUE;
}

BOOLEAN unregisterPopupRegion(UINT16 regionId)
{
	for(std::vector<PopupIndex>::iterator cPopupIndex=gPopupRegionIndex.begin(); cPopupIndex != gPopupRegionIndex.end(); ++cPopupIndex)
		if ((*cPopupIndex).regionId == regionId){
			gPopupRegionIndex.erase(cPopupIndex);
			return TRUE;
		}

		#ifdef JA2TESTVERSION
			CHAR8 debugStr[120];
			wsprintf(debugStr,"Failed to erase mouse region index: %i \n",regionId);
			OutputDebugString( debugStr );
		#endif
	return FALSE;
}

void rebuildPopupRegionIndex(void)
{/*
	UINT8 i,j;
	PopupIndex	TempPopupIndex[MAX_REGIONS_IN_INDEX];
	// cout << "cleaning index"<<endl;
	for (i = 0; i < gPopupRegionIndexCounter; i++)
	{
		TempPopupIndex[i].classId = gPopupRegionIndex[i].classId;
		TempPopupIndex[i].regionId = gPopupRegionIndex[i].regionId;
	}
	j=0; // actual region index
	for (i = 0; i < gPopupRegionIndexCounter; i++)
	{
		if (TempPopupIndex[i].classId < MAX_POPUPS)
		{
			gPopupRegionIndex[j].classId = TempPopupIndex[i].classId;
			gPopupRegionIndex[j].regionId = TempPopupIndex[i].regionId;
			j++;
		}
	}
	// We only have j elements worth keeping.
	gPopupRegionIndexCounter = j;*/
}

void popupMaskCallback(MOUSE_REGION *pRegion, INT32 iReason)
{
	UINT32 cID = findMouseRegionInIndex(pRegion->IDNumber)->classId;
	if (cID < 0) return;

	POPUP * p = findPopupInIndex(cID);
	if (p) { p->ScreenMaskBtnCallback(pRegion, iReason); }
	
}

void popupMouseMoveCallback(MOUSE_REGION *pRegion, INT32 iReason)
{
	UINT32 cID = findMouseRegionInIndex(pRegion->IDNumber)->classId;
	if (cID < 0) return;

	POPUP * p = findPopupInIndex(cID);
	if (p) p->MenuMvtCallBack(pRegion, iReason);
}

void popupMouseClickCallback(MOUSE_REGION *pRegion, INT32 iReason)
{
	UINT32 cID = findMouseRegionInIndex(pRegion->IDNumber)->classId;
	if (cID < 0) return;

	POPUP * p = findPopupInIndex(cID);
	if (p) {
		p->MenuBtnCallBack(pRegion, iReason);
	}
}
