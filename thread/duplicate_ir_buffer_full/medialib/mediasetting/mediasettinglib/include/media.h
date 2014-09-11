	
#ifndef AMPTEST_H_
#define AMPTEST_H_

  
 

#include <directfb.h>
#include "mediaplay_define.h"
#include "zenodefine.h"
#include "ErrorCode.h"

#ifdef	APP_DEBUG
#define LIB_DEBUG(str, args...)	fprintf(stderr, str, ##args)  //debug output
#define APP_ASSERT(x)			assert(x)
#else
#define LIB_DEBUG(str, args...)	// no debug output
#define APP_ASSERT(x)			
#endif

#ifdef ENABLE_MESSAGE_TO_DEBUG
#define GUI_INFO(str, args...)   fprintf(stderr, str, ##args)  //debug output
#else
#define GUI_INFO(str, args...)	// no debug output
#endif

#define SCHED_PRIO_DEFAULT 20
#define WIDTH_OF_GUI  1280
#define HEIGHT_OF_GUI 720

#define WIDTH_OF_PRIMARY  1280//1920
#define HEIGHT_OF_GUI_PRIMARY 720//1080

#define GUI_PREVIEW_THUMBNAIL_X 263
#define GUI_PREVIEW_THUMBNAIL_Y 220
#define GUI_PREVIEW_X  1300
#define GUI_PREVIEW_Y  220
#define GUI_PREVIEW_W  360
#define GUI_PREVIEW_H  240


#define  MAX_TRICKMODE_SF_VALUE 	1024//512
#define  MIN_TRICKMODE_SF_VALUE 	128

#define	 MIN_TRICKMODE_VALUE		1024
#define	 MAX_TRICKMODE_VALUE		16384//32768

#define	 TRICKMODE_2X_VALUE		2048
#define	 TRICKMODE_4X_VALUE		4096
#define	 TRICKMODE_8X_VALUE		8192					
#define	 TRICKMODE_16X_VALUE		16384	
#define	 TRICKMODE_32X_VALUE		32768

#define	 SLOW_1_2X_VALUE		512
#define	 SLOW_1_4X_VALUE		256
#define	 SLOW_1_8X_VALUE		128

#ifndef SPLIT_CHARACTER
#define SPLIT_CHARACTER "#"
#endif

#define MAX_LENGTH_STRING 512
#define MIN_LENGTH_STRING 4

enum { BAR_VOLUME = 0 , BAR_CONTRAST , BAR_BRIGHTNESS , BAR_HUE , BAR_SATURATION };
enum {BAR_DOWN = 0  , BAR_UP , BAR_DEFAULT };
enum { IMAGE_MAIN_SCREEN = 0 , IMAGE_MAIN_MENU , IMAGE_MAIN_SETUP };
enum
{
	ERROR_LEVEL = 0,
	ERROR_COUNTRY_CODE,
	ERROR_REGION_CODE
};

enum
{
	DRC_HIGH = 1,
	DRC_MID,  
	DRC_OFF,
	DRC_AUTO
}; //Dolby Dynamic Range Control levels

enum
{
	SURROUND_MODE_OFF = 1,
	SURROUND_MODE_ON  ,
	SURROUND_MODE_DEFAULT ,
};

enum 
{
	BACKUP_PRIORITY_AND_SET_PRIORITY = 0       ,
	BACKUP_PRIORITY_AND_SET_PRIORITY_FOR_CDDA  ,
	RESTORE_PRIORITY
};

enum
{
	PCM_OUT_HDMI = 0 ,
	BIT_STREAM_OUT_HDMI ,
	PCM_OUT_SPDIF , 
	BIT_STREAM_OUT_SPDIF
};
enum
{
    DOWNMIX,
    PASSTHROUGH 
};
enum
{
	LPCM_OUT_SPDF = 0 		,
	RAW_OUT_SPDF 			,
	LPCM_OUT_HDMI 			, 
	LPCM_MUILTI_OUT_HDMI	,
	RAW_OUT_HDMI
};
enum
{
	TVMODE_480I_576I = 1	,
	TVMODE_480P_576P		,
	TVMODE_720P				,
	TVMODE_1080I			,
	TVMODE_1080P			,
	TVMODE_1080P24			,
	TVMODE_AUTO				,      //trungkt add 2012jan05 Thu
	TVMODE_NONE				,
};

enum
{
	TVMODE_FPT_AUTO = 0	,
	TVMODE_FPT_NTSC		,
	TVMODE_FPT_PAL		,
	TVMODE_FPT_720P_60HZ		,
	TVMODE_FPT_720P_50HZ		,
	TVMODE_FPT_1080I_60HZ		,
	TVMODE_FPT_1080I_50HZ		,
	TVMODE_FPT_1080P_60HZ		,
	TVMODE_FPT_1080P_50HZ		,
	TVMODE_FPT_1080P_24HZ	
};

enum
{
TVMODE_HDMI_AUTO = 0	,
	TVMODE_480I_60HZ		,
	TVMODE_576I_50HZ		,
	TVMODE_480P_60HZ		,
	TVMODE_576P_50HZ		,
	TVMODE_720P_50HZ		,
	TVMODE_720P_60HZ		,
	TVMODE_1080I_50HZ		,
	TVMODE_1080I_60HZ		,
	TVMODE_1080P_50HZ		,
	TVMODE_1080P_60HZ	
};


enum
{
	DRIVE_NO_DISC = 1 ,
	DRIVE_UNKNOWN_DISC ,
	DRIVE_DISC_READY ,
	DRIVE_HAVE_DISC_NOT_READY ,
	TRAY_OPEN ,
};

enum
{
	NO_SHOW = 0      ,
	SHOW_LOADING     ,
	HIDE_LOADING     ,
	WAIT_TO_HIDE     ,	
	 
};

enum
{
	ROOT_MENU = 0   ,
	POPUP_MENU      ,
};

enum
{
	NO_THING = 0                                     ,
	PLAYBACK_CANCELLED_DUE_TO_SECURITY_VIOLATION_ONLY,
	PLAYBACK_CANCELLED_DUE_TO_UNSUPPORTED_DISC       ,   
	PLAYBACK_CANCELLED_DUE_TO_SECURITY_VIOLATION     ,
	PLAYBACK_CANCELLED_DUE_TO_INTERNAL_ERROR	     ,
	PLAYBACK_CANCELLED_DUE_TO_WRONG_REGION	         ,
	PLAYBACK_DELAYED_DUE_TO_SECURITY_VIOLATION       ,
	PLAYBACK_DELAYED_DUE_TO_IO_ERROR                 ,
	PLAYBACK_DELAYED_DUE_TO_INTERNAL_ERROR           ,
	PLAYBACK_IS_LOADING                              ,
	PLAYBACK_CHANGED_BY_IO_ERROR                     ,
	PLAYBACK_CHANGED_BY_INTERNAL_ERROR               ,
	WAIT_TO_HIDE_WARNING                             ,
	HIDE_WARNING                                     ,
};

enum
{
	
	EXTERNAL_STORAGE_REMOVED                     = 1,
	EXTERNAL_STORAGE_REMOVED_EJECT_DISC             ,
	EXTERNAL_STORAGE_REMOVED_PLUG_THE_USB_BACK      ,
	EXTERNAL_STORAGE_READ_ONLY                      ,
	EXTERNAL_STORAGE_READ_ONLY_PLUG_THE_USB_BACK    ,
	EXTERNAL_STORAGE_READY                          ,
	EXTERNAL_STORAGE_NOT_READY                      ,
	EXTERNAL_STORAGE_NOT_READY_PLUG_THE_USB_BACK    ,   
	HIDE_WARNING_HOTPLUG_MESSAGE                    ,
};


enum SpeakerOut
{
	Speaker_C = 0	,
	Speaker_L		,
	Speaker_R		,
	Speaker_LR		,
	Speaker_SL		,
	Speaker_SR		,
	Speaker_SLSR	,
	Speaker_BL		,
	Speaker_BR		,
	Speaker_BLBR	,
	Speaker_LFE		,
	Speaker_LCRSLSRLEFBLBR		,
};

enum
{
	AUTO_DETECT_HDMI = 1,
	USER_IS_PERMIT_TO_SET_RESOLUTION ,
};
enum
{
	AUDIO_PCM = 0  ,
		AUDIO_BITSTREAM ,
};

enum
{
	UPDATER_VALUE_NONE = 0 ,
	UPDATER_VALUE_NORMAL   , 
	UPDATER_VALUE_IMMEDIATELY   ,
};
typedef enum enumUserDirection
{
	UP_DIRECTION,
		DOWN_DIRECTION,
		LEFT_DIRECTION,
		RIGHT_DIRECTION,
}EUserDirection ;

enum
{
	SET_STATIC_IP = 1 ,
    SET_DYNAMIC_IP    ,
	SET_IP_DISABLE ,
};

#if 0
enum
{
	ICON_STOP 	= 1,
	ICON_REV 	   ,
	ICON_FWD 	   ,
	ICON_PLAY 	   ,
	ICON_PAUSE 	   ,
	ICON_PREV 	   ,
	ICON_NEXT	   ,
	ICON_SLOWFWD   ,
	ICON_REPEAT	   ,
	ICON_SHUFFLE   ,
	ICON_EJECK	   ,
	ICON_ERROR  = 16   ,
};
#endif
enum
{
	EVENT_PLAY_DISC  = 1 ,
	EVENT_GOTO_SETUP_MENU ,
	EVENT_PLAY_NETFLIX ,
	EVENT_PLAY_USB,
};

enum
{
	TITLE_FOCUS = 1 ,
	CHAPTER_FOCUS ,
	SEARCH_FOCUS ,
	CANCEL_FOCUS ,
};
enum
{
	HOUR_FOCUS = 1 ,
	MIN_FOCUS ,
	SECOND_FOCUS ,
	YES_FOCUS ,
	NO_FOCUS ,
};

enum
{
	DVD_ICON = 30,
	BRD_ICON ,
};

enum
{
	Features_NONE    = 0  ,
	Features_SUBTITLE  ,
	Features_AUDIO     ,
	Features_ANGLE     ,
	Features_SECOND_VIDEO ,
	Features_SECOND_AUDIO ,
	Features_TITLE_SEARCH ,
};
enum{
	ROOT_ICON  = 0   ,
	PICTURE_ICON = 2 ,
	AUDIO_ICON       ,
	VIDEO_ICON       ,
	FOLDER_ICON      ,
	UNKNOW_ICON      , 
		
};


enum MenuLanguage
{
	ITEM_LANGUAGE_ENGLISH = 0,
	ITEM_LANGUAGE_SPANISH,
	ITEM_LANGUAGE_FRENCH,
	ITEM_LANGUAGE_PORTUGUESE,
	ITEM_LANGUAGE_JAPANESE,
	ITEM_LANGUAGE_CHINESE,
	ITEM_LANGUAGE_CHINESE_COM,
	NUM_MENU_LANGUAGE
};

enum ShowFocusOPtion
{
	SHOW_NUMBER = 0, 
	CLEAN_ALL_NUMBER ,
};

enum
{
	ZOOM_OFF = 0 , 
		ZOOM_OUT ,
		ZOOM_IN ,
		ZOOM_MOVE_LEFT ,
		ZOOM_MOVE_RIGHT ,
		ZOOM_MOVE_UP ,
		ZOOM_MOVE_DOWN,
};
enum
{
	ZOOM_NONE = 0,
		ZOOM_2X,
		ZOOM_4X,
		ZOOM_8X,
		ZOOM_16X,
		NUM_ZOOM
};


enum
{
	OSD_ENABLE = 0,
		SEC_OSD_ENABLE,
		WAIT_LOAD_GUI,
		NO_GUI ,
};

enum
{
	ENUM_DIET_KEYRELEASE = 0 ,
	E_NUM_DIET_KEYPRESS       ,	
};

enum EXECUTION_SUBTTILE
{
		TURN_SUBTITLE_ON = 0, 
		TURN_SUBTITLE_OFF ,
		CHANGE_SUBTITLE ,
};

enum EXECUTION_SECONDVIDEO
{
	TURN_SECOND_VIDEO_ON = 0, 
	TURN_SECOND_VIDEO_OFF ,
	CHANGE_SECOND_VIDEO ,
};

enum EXECUTION_SECONDAUDIO
{
	TURN_SECOND_AUDIO_ON = 0, 
	TURN_SECOND_AUDIO_OFF ,
	CHANGE_SECOND_AUDIO ,
};

enum APP_NUMBER
{
	BLURAY_APP = 0, 
	INTERNET_APP  ,
		YOUTUBE_APP     ,
		NETFLIX_APP     ,
		SHOUTCAST_APP   ,
		TERMINAL_APP    ,
	HDPLAYER_APP  ,
	DTV_APP,
	DLNA_APP,
		
};

enum
{
	/*
	 *	subtitle value
	 */
	NUMBER_OF_PIP_PG_TEXTST_STREAMS_VALUE = 1 ,
	NUMBER_OF_PG_TS_STREAMS_VALUE   ,
	STREAM_NUMBER_VALUE ,
	PG_LANGUAGE_CODE ,
	/*
	 *	audio value
	 */
	STREAM_NUMBER    ,
	NUMBER_OF_PRIMARY_AUDIO_STREAMS ,
	AUDIO_LANGUAGE_CODE ,
	/*
	 *	angle value
	 */
	NUMBER_OF_ANGLES ,
	ANGLE_CRT ,
	/*
	 *	time value
	 */
	ELAPSEDTIME ,
	/*
	 *	current title value
	 */
	CRTTITLE_ACCESSTYPE ,
	NUMBER_OF_TITLES ,
	CRTTITLE_NUMBER , 
	/*
	 *	current chapter value
	 */
	 NUMBER_OF_CHAPTERS ,
	 CRTCHAPTER_NUMBER ,
	 /*
	  *	Second video
	  */
	 SEC_VIDEO_STREAM_NUMBER_VALUE ,
	 NUMBER_OF_SECONDARY_VIDEO_STREAMS ,
	 /*
	  *	Second Audio
	  */
	  SEC_AUDIO_STREAM_NUMBER_VALUE ,
	  NUMBER_OF_SECONDARY_AUDIO_STREAMS ,
	  /*
	   * speed of playing movie
	   */
	  MODE_SPEED ,
	  /*
	   * USB hotplug
	   */
	   USB_HOTPLUG ,
	   /*
	    *  audio type
		*/
		AUDIO_TYPE ,
		/*
		 * Show loading message in playing bdj 
		 */
		LOADING_MESSAGE ,
		/*
		 * Show security message in playing bdj 
		 */
		SECURITY_MESSAGE ,
};
enum SystemMode
{
	PAL_SYSTEM = 0,	
	NTSC_SYSTEM,
	NUM_SYSTEM_MODE
};
enum SlideShowMode
{
	SS_TOP = 1,
	SS_BOTTOM,
	SS_LEFT,
	SS_RIGHT,
	SS_TOPLEFT,
	SS_TOPRIGHT,
	SS_BOTTOMLEFT,
	SS_BOTTOMRIGHT,
	SS_TOPCENTER,
	SS_BOTTOMCENTER,
	SS_LEFTCENTER,
	SS_RIGHTCENTER,

	SS_TOP_BOTTOM,
	SS_LEFT_RIGHT,
	SS_TOPLEFT_BOTTOMRIGHT_HEIGHT,
	SS_TOPLEFT_BOTTOMRIGHT_WIDTH,
	SS_TOPRIGHT_BOTTOMLEFT_HEIGHT,
	SS_TOPRIGHT_BOTTOMLEFT_WIDTH,
	SS_TOPCENTER_BOTTOMCENTER,
	SS_LEFTCENTER_BOTTOMCENTER,
	SS_SPLIT_VERTICAL_OUT,
/*
	SS_SPLIT_VERTICAL_TOP_OUT,
	SS_SPLIT_VERTICAL_BOTTOM_OUT,
	SS_SPLIT_VERTICAL_CENTER_OUT,*/

	SS_SPLIT_HORIZONTAL_OUT,
/*
	SS_SPLIT_HORIZONTAL_LEFT_OUT,
	SS_SPLIT_HORIZONTAL_RIGHT_OUT,
	SS_SPLIT_HORIZONTAL_CENTER_OUT,*/

	SS_FOCUS,
	SS_4CENTERS,
	SS_4CORNERS,
	SS_BLINDS_VERTICAL,
	SS_BLINDS_HORIZONTAL,

	SS_ROTATE_CLOCKWISE,
	SS_MODE_NUM,

};
// for dlna
#ifdef DLNA_SUPPORT
//HoaLe DLNA add
typedef enum 
{
	DIR_DLNA_DEMO_FILETYPE_UNKNOWN = 0,
	DIR_DLNA_DEMO_FILETYPE_VIDEO,
	DIR_DLNA_DEMO_FILETYPE_AUDIO,
	DIR_DLNA_DEMO_FILETYPE_PICTURE,
}mpFileType_DLNA_DEMO;
#endif // DLNA_SUPPORT
/*
 *	idle time out for group key
 */
#define PLAY_PREVIEW_TIMEOUT 2
/*
 *	 0 -> 9
 */
#define NUMBER_TIMEOUT 4

/*
 *	 Rev , Fwd , Slow Fwd , Slow Fwd
 */
#define FWD_TIMEOUT 4
/*
 *	Play , Stop , Info , Pause
 */
#define PLAY_TIMEOUT 4
/*
 *	AB , Prev , Next , Repeat
 */
#define NEXT_TIMEOUT 4

/*
 *	invalid information
 */
#define INVALID_TIMEOUT 8

/*
 *	Advanced feature timeout
 */
#define ADVANCED_TIMEOUT 4
/*
 *	Time Search timeout
 */
#define TIMESEARCH_TIMEOUT 12
/*
 *	Time Out Zoom
 */
#define ZOOM_TIMEOUT 2
/*
 *	Advanced feature timeout
 */
#define DISC_TIMEOUT 4
// playlist functions

#include "internalusb_handler.h"
#include "init_directfb.h"
#include "mediaprovider_handler.h"
#include "config_audio_video.h"


#ifndef NO_DTV_SUPPORT
#include "dtv/dtv_cmd.h"
#include "dtv/media_dtv.h"
#endif //DTV_SUPPORT

#endif /*AMPTEST_H_*/
