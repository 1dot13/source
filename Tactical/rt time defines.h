#ifndef RT_DEFINES
#define RT_DEFINES



//silversurfer: now externalized to APBPConstants.ini
//#define		TB_BREATH_RECOVER_MODIFIER		1
//#define		TB_BREATH_DEDUCT_MODIFIER			1.25

#define		RT_NEXT_BLEED_MODIFIER							0.5
#define		RT_FIRST_AID_GAIN_MODIFIER					12


#ifndef NUM_SEC_IN_MIN
#define NUM_SEC_IN_MIN 60
#endif

#define		NUM_REAL_SEC_PER_TACTICAL_TURN						5000

#define		NUM_SEC_PER_STRATEGIC_TURN					( NUM_SEC_IN_MIN * 15 )	// Every fifteen minutes
#define		NUM_SEC_PER_TACTICAL_TURN					5
#define		RT_COMPRESSION_TACTICAL_TURN_MODIFIER			10										


#endif
