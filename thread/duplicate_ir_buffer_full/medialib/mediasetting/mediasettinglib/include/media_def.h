
#ifndef MEDIA_DEF_H
#define MEDIA_DEF_H

union UMSStatus
{
	struct SStatus      generic;
	struct SLPBStatus   lpb;
#ifndef NO_RED_SUPPORT
	struct SCDDAStatus  cdda;
	struct SDVDStatus   dvd;
	struct SVRStatus    vr;
#ifndef NO_VCD_SUPPORT	
	struct SVCDStatus   vcd;
#endif // #ifndef NO_VCD_SUPPORT		
	
#endif
#ifndef NO_BLUE_SUPPORT
	struct SBRDStatus   brd;
	struct SBDREStatus  bdre;
	struct SBRDStatus   bdj;
#endif
#ifndef NO_DTV_SUPPORT
	struct SDTVStatus dtv;
	struct SCAPStatus cap;
#endif // DTV_SUPPORT
};

union UMSCommand
{
	struct SCommand     generic;
	struct SLPBCommand  lpb;
#ifndef NO_RED_SUPPORT
	struct SCDDACommand cdda;
	struct SDVDCommand  dvd;
	struct SVRCommand   vr;
#ifndef NO_VCD_SUPPORT	
	struct SVCDCommand   vcd;
#endif // #ifndef NO_VCD_SUPPORT		
#endif
#ifndef NO_BLUE_SUPPORT
	struct SBRDCommand  brd;
	struct SBDRECommand bdre;
	struct SBRDCommand  bdj;
#endif
#ifndef NO_DTV_SUPPORT
		struct SDTVCommand dtv;
		struct SCAPCommand cap;
#endif // DTV_SUPPORT
};

union UMSResult
{
	struct SResult      generic;
	struct SLPBResult   lpb;
#ifndef NO_RED_SUPPORT
	struct SCDDAResult  cdda;
	struct SDVDResult   dvd;
	struct SVRResult    vr;
#ifndef NO_VCD_SUPPORT	
	struct SVCDResult   vcd;
#endif // #ifndef NO_VCD_SUPPORT		
#endif
#ifndef NO_BLUE_SUPPORT
	struct SBRDResult   brd;
	struct SBDREResult  bdre;
	struct SBRDResult   bdj;
#endif

#ifndef NO_DTV_SUPPORT
	struct SDTVResult dtv;
	struct SCAPResult cap;
#endif // DTV_SUPPORT
};


#ifndef NO_DTV_SUPPORT
typedef enum
{
	ACTION_NONE, ACTION_POST, ACTION_EXECUTE,
} action_t;
#endif // DTV_SUPPORT

/********************************************************************************************************
 *
 * MEDIA SPACE COMMAND DEFINITIONS
 *
 * Defines the functionality and UI parameters for media spaces commands
 * in a standard formthat is automatically handled by the framework
 *
 ********************************************************************************************************/

struct SMediaSpaceCmdDefinition
{
	int cmd;						// cmd id
	int *param1Ptr;					// pointer to the first command parameter
	int *param2Ptr;					// pointer to the 2nd command parameter
	struct SMediaTime *timePtr;		// pointer to the time command parameter (can be use dfor non-time values also, but then needs express handling)
	DFBInputEventType       selKeyType;       /* type of event */	
	DFBInputDeviceKeySymbol selKey;	// UI key that triggers the command
	char *featureName;				// feature name to be presented in the help screen and selection lists
	int   defParam1;				// default parameter 1 value, used for compile-time selections
	char *par1SelMsg;				// first parameter selection prompt
	int   defParam2;				// default parameter 2 value, used for compile-time selections
    char *par2SelMsg;				// 2nd parameter selection prompt
	int   defParam3;				// default parameter 3 value, used for compile-time selections
    char *par3SelMsg;				// 3rd parameter selection prompt (NULL for time selection)
};



/********************************************************************************************************
 *
 * MEDIA SPACE RUN-TME PARAMETERS DEFINITIONS
 *
 * Defines the run-time parameters associated with one media space presentation
 *
 ********************************************************************************************************/

struct SMediaSpaceRunTime
{
    int mediaSpace;
    char *mediaSpaceName;
	
	unsigned int cmdCnt;
    struct SMediaSpaceCmdDefinition *cmdList;
	
    int *pCmd;		// address of the command field
    
    int bookmarkSizeQueryCmd;
    int bookmarkSizeOffset;
    char **bookmarkBufPtr;
};

#endif /*MEDIA_DEF_H*/
