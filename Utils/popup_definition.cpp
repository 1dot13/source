	#include "popup_class.h"
	#include "popup_callback.h"
	#include "sgp.h"

	#include "popup_definition.h"
	#include "Interface Items.h"

	// for getting psoldier
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Map.h"
	#include "Overhead.h"

//////////////////////////////////////
//	popupDef
//////////////////////////////////////

	popupDef::popupDef(){}

	popupDef::~popupDef(){
		for(std::vector<popupDefContent*>::iterator content=this->content.begin(); content != this->content.end(); ++content)
		{
			delete *content;
		}
	}

	BOOL popupDef::applyToBox(POPUP* popup){
		for(std::vector<popupDefContent*>::iterator content=this->content.begin(); content != this->content.end(); ++content)
		{
			if ( (*content)->addToBox( popup ) != TRUE ) return FALSE;
		}

		return TRUE;
	}

	BOOL popupDef::addOption(std::wstring* name, UINT16 callbackId, UINT16 availId){
		// TODO: check for vaid callbacl/avail ID
		
		this->content.push_back( new popupDefOption(name,callbackId,availId) );

		return TRUE;
	}

	popupDef * popupDef::addSubPopup(std::wstring* name){

		popupDefSubPopupOption * sub = new popupDefSubPopupOption(name);

		this->content.push_back( sub );
	
		return sub->getSubDef();
	}

	BOOL popupDef::addSubPopup(popupDefSubPopupOption* sub){
		// TODO: add check for max options
		this->content.push_back( sub );

		return TRUE;
	};

	BOOL popupDef::addGenerator(UINT16 id){
		// TODO: check for valid generator id

		this->content.push_back( new popupDefContentGenerator(id) );

		return TRUE;
	}


//////////////////////////////////////
//	popupDefContent
//////////////////////////////////////

	popupDefContent::popupDefContent(){}
	popupDefContent::~popupDefContent(){}


//////////////////////////////////////
//	popupDefOption helpers
//////////////////////////////////////

	static BOOL setPopupDefCallback( POPUP_OPTION * opt, UINT16 callbackId ){
	
		// TODO
		opt->setAction(NULL);

		return TRUE;

	}

	static BOOL setPopupDefAvail( POPUP_OPTION * opt, UINT16 callbackId ){
	
		// TODO
		opt->setAvail(NULL);

		return TRUE;

	}

//////////////////////////////////////
//	popupDefOption
//////////////////////////////////////
	/* defined in header file
	popupDefOption::popupDefOption(){}
	popupDefOption::popupDefOption( std::wstring* name, UINT16 callbackId, UINT16 availId ){}

	~popupDefOption::popupDefOption(){}
	*/
	BOOL popupDefOption::addToBox(POPUP * popup){
	
		POPUP_OPTION * opt = new POPUP_OPTION();

		opt->setName( this->name );
		if (	!setPopupDefCallback(opt, this->callbackId) 
			||	!setPopupDefAvail(opt, this->availId) )
		{
			delete opt;
			return false;
		}
		
	
		return popup->addOption(*opt);
	}


//////////////////////////////////////
//	popupDefSubPopupOption
//////////////////////////////////////
	/* defined in header file
	popupDefSubPopupOption::popupDefSubPopupOption(){}
	popupDefSubPopupOption::popupDefSubPopupOption( std::wstring* name ){}
	
	popupDefSubPopupOption::~popupDefSubPopupOption(){}
	*/
	BOOL popupDefSubPopupOption::addToBox(POPUP * popup){
	
		POPUP_SUB_POPUP_OPTION * sub = new POPUP_SUB_POPUP_OPTION( this->name );

		if( !this->content->applyToBox( sub->subPopup ) ){
			delete sub;
			return false;
		}

		return popup->addSubMenuOption(sub);	
	}
	

//////////////////////////////////////
//	popupDefContentGenerator helpers
//////////////////////////////////////

	/*
	void addArmorToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addLBEToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addWeaponsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addWeaponGroupsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addGrenadesToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addBombsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addFaceGearToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addAmmoToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );

	void addRifleGrenadesToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addRocketAmmoToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	void addDrugsToPocketPopup( SOLDIERTYPE *pSoldier, INT16 sPocket, POPUP* popup );
	*/

	static BOOL applyPopupContentGenerator( POPUP * popup, UINT16 generatorId ){

		SOLDIERTYPE	*pSoldier;
		GetSoldier( &pSoldier, gCharactersList[ bSelectedInfoChar ].usSolID );

		switch(generatorId){
		case popupGenerators::dummy:
			popup->addOption(new std::wstring( L"Dummy generator" ),NULL);
			break;

		case popupGenerators::addArmor:
			addArmorToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addLBE:
			addLBEToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addWeapons:
			addWeaponsToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addWeaponGroups:
			addWeaponGroupsToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addGrenades:
			addGrenadesToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addBombs:
			addBombsToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addFaceGear:
			addFaceGearToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addAmmo:
			addAmmoToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addRifleGrenades:
			addRifleGrenadesToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addRocketAmmo:
			addRocketAmmoToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addMisc:
			addMiscToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;
		case popupGenerators::addKits:
			addKitsToPocketPopup( pSoldier, gsPocketUnderCursor, popup );
			break;

		default:
			return FALSE;
		}

		return TRUE;
		
	}

//////////////////////////////////////
//	popupDefContentGenerator
//////////////////////////////////////
	/* defined in header file
	popupDefContentGenerator::popupDefContentGenerator(){}
	popupDefContentGenerator::popupDefContentGenerator( UINT16 generatorId ){}
	
	popupDefContentGenerator::~popupDefContentGenerator(){}
	*/

	BOOL popupDefContentGenerator::addToBox(POPUP * popup){
	
		return applyPopupContentGenerator( popup, this->generatorId );
	
	}
	