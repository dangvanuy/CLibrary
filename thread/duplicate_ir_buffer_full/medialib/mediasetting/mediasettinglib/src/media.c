/*****************************************
 Copyright  2005
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**

*/

#include "include/generalmedia.h"

// initialize the A/V configuration tables
#include "mediaconfig.c"
#include "include/mediaconfig_dump.h"
#include "include/mediaplaylist.h"
#include "include/ir.h"
#ifdef REAL_TIME_THREADS
#include <sched.h>
#endif

#ifdef DO_DMALLOC
#include <dmalloc.h>
#endif

#if (RMENDIANNESS == RMBIGENDIAN)
#define BYTE_1_SHIFT    24
#define BYTE_2_SHIFT    16
#define BYTE_3_SHIFT     8
#else
#define BYTE_1_SHIFT     0
#define BYTE_2_SHIFT     8
#define BYTE_3_SHIFT    16
#endif

#define DIRECTFB_VERSION (DIRECTFB_MAJOR_VERSION * 0x100 + DIRECTFB_MINOR_VERSION * 0x10 + DIRECTFB_MICRO_VERSION)

/**
* 
* <b>FUNCTION: </b>
*
* declare DirectFb
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
IDirectFB							*gdfb = NULL ;
IDirectFBScreen					  *screen = NULL;
IDirectFBEventBuffer		   *keybuffer = NULL;
IAdvancedMediaProvider            *cfgamp = NULL;

/**
* 
* <b>FUNCTION: </b>
*
* declare MainOsdLayer
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
IDirectFBDisplayLayer			   *layer = NULL;
IDirectFBWindow				 *window_osd1 = NULL;
IDirectFBSurface			*surface_osd1 = NULL;
IDirectFBSurface				 *primary = NULL;
/**
* 
* <b>FUNCTION: </b>
*
* declare SecondOsdLayer
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
// OSD second layer
IDirectFBDisplayLayer			*layer_so = NULL;
IDirectFBWindow         *window_osd2 = NULL;
IDirectFBSurface        *surface_osd2 = NULL;

/**
* 
* <b>FUNCTION: </b>
*
* declare MainVideoLayer
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
// Main video layer (for color adjustments)
IDirectFBDisplayLayer			*layer_mv = NULL;
/**
* 
* <b>FUNCTION: </b>
*
* declare SecondVideoLayer
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
//Secondary video layer and its surface (in case it's used for graphics)
IDirectFBDisplayLayer			*layer_sv = NULL;	  
IDirectFBSurface				      *sv = NULL;
/**
* 
* <b>FUNCTION: </b>
*
* declare backgroundLayer
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
// Background layer and its surface
IDirectFBDisplayLayer				*layer_bg = NULL;
IDirectFBWindow					   *window_bg = NULL;
IDirectFBSurface						  *bg = NULL;
/**
* 
* <b>FUNCTION: </b>
*
* declare Audio & Video structal configuration
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
DFBRectangle					m_src_rect;
DFBRectangle					m_zoom_rect;
DFBRectangle					m_layer_rect;

struct SMRUAAudioOutConfig		*pCrtAudioCfg = NULL;
struct SMRUAVideoOutConfig		*pCrtVideoCfg = NULL;
struct SMRUAAudioOutConfig		  CrtAudioCfg;
struct rmcci					  *pCrtDRMCfg;
struct SGlobals					  *ampGlobals = NULL;
/**
* 
* <b>FUNCTION: </b>
*
* declare some Audio & Video parameter relation configuration for them
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
RMbool						isResetAudio	= FALSE;
RMbool				 isResetAudioDigital    = FALSE;
RMuint8			 *pvalueAudioOuput_SPDIF	= NULL; // 0: PCM  ---  1: BitStream
RMuint8			  *pvalueAudioOuput_HDMI    = NULL;  // 0: PCM  ---  1: BitStream

RMbool						isDolbyChannel	= FALSE;


static unsigned int newVideoConfigIndex;
static unsigned int newAudioConfigIndex;
static unsigned int newDRMConfigIndex;
static unsigned int videoConfigIndex = 0;
static unsigned int audioConfigIndex = 0;
static int			  drmConfigIndex = 0;
/**
* 
* <b>FUNCTION: </b>
*
* declare some parameter to control pin for BDRE
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
#ifndef NO_BLUE_SUPPORT
//KhanhNgo add
static bool						noAuto = false;
bool							reqPIN = false;
char						  stPIN[5] = {0, };
#endif //#NO_BLUE_SUPPORT

/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to debug audio and video
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
RMbool showcfginfo = FALSE;
/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to save the last signal from app
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
DFBScreenOutputSignals last_hdtv_signal = DSOS_NONE;
/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to control showing information to user, only using them in BDROM
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
RMuint16				isShowLoading = NO_SHOW;
RMuint16				isShowWarning = NO_THING;
/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to control switch between osdlayer and second osdlayer
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
bool					gui_on_secosd = true;
int						gui_ref_count = 1;
/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to control level for DVD ( parental control)
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
#ifndef NO_RED_SUPPORT
RMbool						    isWrongRegion = FALSE;
RMbool					isShowPassWordConfirm = FALSE;
int						     parentalChangeOK = -1;
#endif //#ifndef NO_RED_SUPPORT

/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to control getting key from remote control 
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
int						keyfd ; 
/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to check HDMI out
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
RMbool							isHDMIHotPlug = FALSE ;
enum EConnectionStatus		  hdmi_out_status ;
/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to control resolution
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
RMbool							isTVPreferred = FALSE;
RMbool						isResetResolution = FALSE;
RMbool					PermissionForceSignal = FALSE;
RMuint8			   *PermissionResetResolution = NULL;
RMuint8						ValueOfResolution = 0;
RMuint8					  *pCurrentResolution = NULL;
RMuint8			*pIsAutoDetectSourceForTvMode = NULL;
/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to save event of remote control
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
DFBEvent							*media_evt = NULL ;
fd_set									   set; //Vuong adds

/**
* 
* <b>FUNCTION: </b>
*
* declare a parameter to save name of image( using background layer)
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
char								fileNameImageBackGround[40] ;
/**
* 
* <b>FUNCTION: </b>
*
* declare some parameters to media
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
bool								allowAdaptiveDexing = true;
RMbool										  isPostCmd = FALSE;
RMbool									   playInWindow = TRUE ;
// play preview
RMbool									isSetRecPreview = FALSE;
RMbool									  useBackground = FALSE ;
RMuint8										  isShowGui = NO_GUI;
// storage for nominal deinterlacing mode while restrictions exists due to  the VCR scaler being unavailable; -1 means VCR is available
DFBDeinterlacingMode				 nominal_deint_mode = (DFBDeinterlacingMode)-1;	
// index of the AMP that has the focus curently
int 										   ampIndex ;	//15/11/2011 Phathv rename to distinguish global and local variables ampIndex 
// AMP that has the focus curently
IAdvancedMediaProvider							   *amp = NULL;	
// event file descriptor of the AMP that has the focus curently	
int						 ampfd;		
// number of amps - first amps entries in amp_table
int												   amps;						
int												   xres;
int												   yres;
int											   out_xres;
int											   out_yres;
int												  xpos = 0;
int												  ypos = 0;

/**
* 
* <b>FUNCTION: </b>
*
* declare a amp_table struct
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
// AMP table
static struct
{
	IAdvancedMediaProvider  *amp;		// AMP pointer, valid when img_size is (0,0)
	DFBDimension			 img_size;	// image data, valid when amp is NULL
	int						 ampfd;		// AMP event file descriptor, always valid
	int						 layerID;	// display layer ID or -1 if media does not render video to DFB video layer
	IDirectFBDisplayLayer   *layer;		// display layer, valid (non-NULL) only for media with video rendered to one of the 2 video layers available in the system
	IDirectFBWindow         *window;	// display window, valid (non-NULL) only for media with video rendered to a graphic surface
	IDirectFBSurface        *surface;	// display surface, valid (non-NULL) only for media with video rendered to a graphic surface
	DFBRectangle			 src_area;	// displayed content area, valid only for media with video
	DFBRectangle			 dst_area;	// display area, valid inly for media with video
}
amp_table[MAX_AMP_INSTANCES] = {};
/**
* 
* <b>FUNCTION: </b>
*
* declare a struct SFrameCallbackParam
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
static struct SFrameCallbackParam
{
	DFBRectangle 		 area;
	IDirectFBSurface 	*surf;
}
amp_frame_callback_table[MAX_AMP_INSTANCES] = {};


union UMSCommand ampCommand;

struct SAdjustment adjustment;

#include "include/media_command.h"
#ifndef NO_RED_SUPPORT
struct SDVDInitialSettings dvd_settings =
{
	RM_DVD_REGION_FREE,
	RM_LANGUAGE_ENGLISH,
	RM_LANGUAGE_ENGLISH,
	RM_LANGUAGE_ENGLISH,
	RM_COUNTRY_UNITED_STATES,
	RM_DVD_PARENTAL_LEVEL_DISABLED,
	parental_level_change_callback,
	NULL
};
#endif
/*************************************************************************************/
#ifndef NO_BLUE_SUPPORT
// needed by MEDIA_SPACE_BDRE
static int selected_play_list_idx = 1;				// PlayList index
static unsigned char tn_buff[BDRE_TN_BUFF_SIZE];	// thumbnail picture buffer
static char *bdreCharSetName[256] = {"Invalid"};
#endif
/******************************************************************************/
/* macro for a safe call to DirectFB functions */
/* macro for a safe call to DirectFB functions */
#define DFBCHECK(x...) \
	do {                                                            \
			err = x;                                                  \
			if (err != DFB_OK) {                                      \
				fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ ); \
				global_uninit();										 \
				DirectFBErrorFatal( #x, err );                         \
			}                                                         \
	} while(0)


/********************************************************************************************************
 *
 * MEDIA SPACE RUN-TME PARAMETERS
 *
 ********************************************************************************************************/
struct SMediaSpaceRunTime lpbRT =
{
    MEDIA_SPACE_LINEAR_MEDIA, "File/Stream",
    sizeof(lpbCmdList)/sizeof(lpbCmdList[0]), lpbCmdList,

    (int *) &ampCommand.lpb.cmd, // command field's address
	0, 0, NULL,	// bookmark params
	
};
#ifndef NO_RED_SUPPORT
struct SMediaSpaceRunTime cddaRT =
{
    MEDIA_SPACE_CDDA, "CD-DA",

    sizeof(cddaCmdList)/sizeof(cddaCmdList[0]), cddaCmdList,

    (int *) &ampCommand.cdda.cmd, // command field's address
	0, 0, NULL,	// bookmark params
};
struct SMediaSpaceRunTime dvdRT =
{
    MEDIA_SPACE_DVD, "DVD",

    sizeof(dvdCmdList)/sizeof(dvdCmdList[0]), dvdCmdList,

    (int *) &ampCommand.dvd.cmd,	// command field's address
	DVDCmd_Query_BookmarkSize, offsetof(struct SDVDResult, info.size), &ampCommand.dvd.param1.bookmarkBuf,
	
};
struct SMediaSpaceRunTime vrRT =
{
    MEDIA_SPACE_DVD_VR, "DVD-VR",

    sizeof(vrCmdList)/sizeof(vrCmdList[0]), vrCmdList,

    (int *) &ampCommand.vr.cmd, // command field's address
	VRCmd_Query_BookmarkSize, offsetof(struct SVRResult, info.size), &ampCommand.vr.param1.bookmarkBuf,
	
};
#ifndef NO_VCD_SUPPORT	
static struct SMediaSpaceRunTime vcdRT =
{
	MEDIA_SPACE_VCD, "VCD",
		
		sizeof(vcdCmdList)/sizeof(vcdCmdList[0]), vcdCmdList,
		
		(int *) &ampCommand.vcd.cmd,    // command field's address
		
		0, 0, NULL,     // bookmark params
};
#endif // NO_VCD_SUPPORT	

#endif
#ifndef NO_BLUE_SUPPORT
struct SMediaSpaceRunTime brdRT =
{
	MEDIA_SPACE_BLU_RAY, "AVCHD",
	sizeof(brdCmdList)/sizeof(brdCmdList[0]), brdCmdList,
	(int *) &ampCommand.brd.cmd,
	BRDCmd_query_bookmark_size, offsetof(struct SBRDResult, info.size), &ampCommand.brd.param1.bookmarkBuf,
	
};
struct SMediaSpaceRunTime bdreRT =
{
	MEDIA_SPACE_BDRE, "BD-RE",
		
		sizeof(bdreCmdList)/sizeof(bdreCmdList[0]), bdreCmdList,
		
		(int *) &ampCommand.bdre.cmd,
		
		BDRECmd_query_bookmark_size, offsetof(struct SBDREResult, info.size), &ampCommand.bdre.param2.bookmarkBuf,
};
static struct SMediaSpaceRunTime bdjRT =
{
    MEDIA_SPACE_BDJ, "BD-ROM",
		
		sizeof(bdjCmdList)/sizeof(bdjCmdList[0]), bdjCmdList,
		
		(int *) &ampCommand.bdj.cmd,
		
		BRDCmd_query_bookmark_size, offsetof(struct SBRDResult, info.size), &ampCommand.bdj.param1.bookmarkBuf,
};
#endif
#ifndef NO_DTV_SUPPORT
	static struct SMediaSpaceRunTime dtvRT =
	{
		MEDIA_SPACE_DTV, "DTV",

		sizeof(dtvCmdList)/sizeof(dtvCmdList[0]), dtvCmdList,

		(int *) &ampCommand.dtv.cmd, // command field's address

		0, 0, NULL,	// bookmark params
	};
#endif //DTV_SUPPORT
// Template table indexed by media space ID
struct SMediaSpaceRunTime *msRTTab[] =
{
		NULL,			// 0 - invalid
		&lpbRT,
#ifndef NO_RED_SUPPORT
		&cddaRT,
#else
		NULL,
#endif
#ifndef NO_VCD_SUPPORT	
#ifndef NO_RED_SUPPORT
        &vcdRT,                  // VCD
#else
		NULL,
#endif
#else
        NULL,		// VCD
#endif // NO_VCD_SUPPORT
        NULL,		
#ifndef NO_RED_SUPPORT
		&dvdRT,
#else
		NULL,
#endif
		NULL,
#ifndef NO_RED_SUPPORT
		&vrRT,
#else
		NULL,
#endif
		NULL,
#ifndef NO_BLUE_SUPPORT
		&brdRT,
		&bdreRT,
		&bdjRT,
#else
		NULL,
		NULL,
		NULL,
#endif
#ifndef NO_DTV_SUPPORT
	&dtvRT,
#else
	NULL,
#endif
};
/********************************************************************************************************
 *
 *  END MEDIA SPACE RUN-TME PARAMETERS
 *
 ********************************************************************************************************/
// number of concurrent media sessions
static int mediaCnt = 0;
/**
* 
* <b>FUNCTION: </b>
*
* declare a struct SMediaSession
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
// AMP related variables, per object
// AMP related variables, per object
static struct SMediaSession
{
	bool                             noAutoPlay;
	struct SMediaFormat              format;
	struct SMediaFormat              cFormat;
	char                             url[512];
	char                             cUrl[512];
	void                            *openParam;
	bool                             useDataBuffer;
	bool                             useStreamingDataBuffer;
	long                                                     streamMultiplier;
	IDirectFBDataBuffer             *dataBuffer;
	IDirectFBDataBuffer             *cDataBuffer;
	int                              fileToStream;
	DirectThread                    *dataBufferFeeder;
	bool                             stopStreaming;
	int                              next;                  // index of
	
	IAdvancedMediaProvider          *amp;
	int                              ampID;
	IDirectFBEventBuffer            *ampEvent;
	DFBDimension                     img_size;              // img and amp can't both be valid at the same time
	bool                             useImg;                // true when we need to use img instead of amp
#if DIRECTFB_VERSION < 0x140
	DFBSurfaceGeometricTransform     imgXform;              // optional transformation in the image case
#endif
	
	DFBRectangle                     dest;                  // OSD ares destination (valid only for ampID >= 2)
	bool                             bdst;                  // true if dest area is in background layer, false if in OSD/secvideo
	bool                             vdst;                  // true if dest area is in secvideo layer, false if in OSD/background
	struct SMediaSpaceRunTime       *msRT;                  // media space run-time parameters
	union UMSStatus                  status;                // status info for this AMP instance
	union UMSResult                  result;                // result info for this AMP instance
	struct SMediaSpaceCmdDefinition *msCmd;                 // definition of the command currently being processed
	char                            *bookmarkBuf;
	int                              bookmarkSize;
	TAudioVolume                     volume;
	
#ifndef NO_BLUE_SUPPORT
	bool anyBdPsrSetFlag;
	bool bdPsrSetFlag[32];
	unsigned long bdPsr[32];
#endif
	
	enum
	{
		TSTS_NORMAL = 0,
			TSTS_BUSY,
			TSTS_USER_ERROR,
			TSTS_AMP_ERROR,
			TSTS_APP_ERROR,
	}
	tempStatus;                                             // temporary status
	
	char statusInfo[16384];
	char queryResult[16384];
	char helpInfo[16384];
	
	playlist_t playlist;
	
	// prefetching for PFS
	bool use_pfs;
	IAdvancedMediaProvider *amp_pfs;
	IDirectFBEventBuffer   *ampEvent_pfs;
	
	bool running; // can process command
	
	int repeatTitle;                // number of the title to be repeated (0 = no repeat)
	int repeatChapter;              // number of the chapter to be repeated (0 = no repeat)
	bool repeatIssued;              // flag to disable repeat commnds while the state transitions from STOPPED
	UINT64 targetPTS;               // target PTS for WakeUpOnPTS command (0 if none pending)
	
	IDirectFBImageProvider *dummy_img;
	
	struct SLPBOpenParams lpb_open_params;
#ifndef NO_DTV_SUPPORT
	int	isOpen;
#endif
}
mediaTab[3*MAX_AMP_INSTANCES];  // allow for sequences of at least 3 play items for each and every available AMP
/**
*
* <b>FUNCTION: </b>
*
* declare Slide Show variable
*
* <b>DESCRIPTION: </b>
*
*
*
* @param
*
*
*
*/
int uiWidth, uiHeight;
IDirectFBSurface 		*slidesurface = NULL;
IDirectFBSurface 		*tempsurface  = NULL;
DFBRectangle			rect_SlideShow;
int uiPreMode = 0, uiCurMode=0, usleep_time=100;


bool onActualSize = false; 
int onFramerateSync = 0 ; // 0 : off; 1:NTSC/PAL; 2:NTSC/24Hz; 3:NTSC/PAL/24Hz
bool flagEncodeDisplay = true; 

//const struct SContentInfo *contentFPT = NULL; 

/**
* 
* <b>FUNCTION: </b>
*
* include source code from somewhere
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/
#include "internalusb_handler.c"
#include "init_directfb.c"
#include "mediaprovider_handler.c"
#include "config_audio_video.c"


#ifndef NO_DTV_SUPPORT
#include "dtv/dtv_cmd.c"
#include "dtv/media_dtv.c"
#endif
/////////////////////////////////////// end /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


