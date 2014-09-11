/******************************************************************************
 File name: 	ErrorCode.h
 Copyright (c) 2008 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
 Version	1.0
 Date		
******************************************************************************/
/**
* @file   Errorcode.h
* @brief  Define error message
*/

/**
$Log: ErrorCode.h,v $
Revision 1.1.1.1  2013/02/20 09:35:11  liempham
Imported sources

Revision 1.2  2013/01/24 01:02:24  hdo
BugId: 38635
Description:
 - Import dtv apis into sigmaobjectextension
SEND_COMMENT_BUGZILLA

Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
add new

Revision 1.1.1.1  2012/06/20 05:47:36  cvsadmin
add new

Revision 1.1.1.1  2012/05/21 08:43:59  cvsadmin
add new

Revision 1.1.1.1  2012/04/12 03:05:18  cvsadmin
add new

Revision 1.1.1.1  2012/02/17 01:11:17  cvsadmin
add new

Revision 1.1.1.1  2011/12/12 01:57:26  cvsadmin
add new

Revision 1.1.1.1  2011/10/26 02:06:54  cvsadmin
add new

Revision 1.1  2011/04/25 04:35:05  liempham

BugId: 31699 add sc_sdk module
SEND_COMMENT_BUGZILLA

Revision 1.2  2010/11/22 06:38:12  cuongnguyen
Add Slideshow.

Revision 1.1.1.1  2010/07/19 02:47:56  cvsadmin
add new

Revision 1.1.1.1  2010/05/06 01:52:41  cvsadmin
The update zeno Z5D1-SD-0706-700-2.0X.01

Revision 1.1.1.1  2010/04/07 02:52:57  cvsadmin
The import new Z5D1-SD-0706-700-2.0X.00

Revision 1.1.1.1  2010/01/21 09:32:43  cvsadmin
Then import new Z5D1-SD-0705-700-2.0X.00

Revision 1.1.1.1  2010/01/21 06:02:35  cvsadmin
The import new ZenoFramework

Revision 1.2  2010/01/20 09:51:52  khanhngo
BugID: none

*/

#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

#define ERR_MESSAGE(str, args...)  	   {printf("\n \033[31m [%s:%s:%d] => ",__FILE__, __FUNCTION__,__LINE__); printf(str, ##args); printf("\033[0m \n");}
#define WARNING_MESSAGE(str, args...)  {printf("\n \033[34m [%s:%s:%d] => ",__FILE__, __FUNCTION__,__LINE__); printf(str, ##args); printf("\033[0m \n");}

#define DFB_MESSAGE(x, str...) 									\
{                                                               \
	err = x;                                                  	\
	if (err != DFB_OK) {      									\
	ERR_MESSAGE(str);											\
	global_uninit();										    \
	DirectFBErrorFatal( #x, err );          	               	\
	}                                                         	\
}



/* SOME DEFINED ERRORS*/
#define ERR_INIT_PARAMETER			"ERROR: Init Parameter"
#define	ERR_INVALID_PARAMETER		"ERROR: Invalid Parameter"
#define	ERR_MEMORY_ALLOCATE			"ERROR: Allocate Memory"
#define	ERR_SYSTEM_CMD		 		"ERROR: System Cmd"

/*  overflow  */
#define ERR_OUT_OF_CHAN_NUM 		"ERROR: Out of Default Channel Number"
#define ERR_OUT_OF_NAME_LENGTH  	"ERROR: Out of Name Length"
#define	ERR_BUFFER_OVERFLOW			"ERROR: Buffer Overflow"

/* open & close provider */
#define ERR_OPEN_PROVIDER 			"ERROR: Open Provider"		
#define ERR_CLOSE_PROVIDER			"ERROR: Close Provider"	

/* scan & play channel */
#define ERR_SET_CHAN_INFO 			"ERROR: Set Channel Info"	
#define ERR_SET_PROG_INFO 			"ERROR: Set Program Info"

#define ERR_GET_PROG_INFO 			"ERROR: Get Program Info"	
#define ERR_GET_TUNER_INFO 			"ERROR: Get Tuner Info"	
#define ERR_GET_SCAN_INFO  			"ERROR: Get Scan Info"	
#define ERR_GET_VIDEO_STREAM_INFO	"ERROR: Get Video Stream Info"

#define ERR_TUNE_PROGRAM			"ERROR: Tune Program"
#define ERR_SAVE_CONFIG 			"ERROR: Save User Config"

/* mount, open, delete, play file*/
#define	ERR_MOUNT_STORAGE 			"ERROR: Mounting storage is failed"
#define	ERR_OPEN_PARTITION			"ERROR: Can not open /proc/partitions to check"
#define	ERR_OPEN_FILE		 		"ERROR: Can not open file"
#define	ERR_DELETE_FILE		 		"ERROR: Can not delete file"
#define	ERROR_PLAYING_FILE			"ERROR: Can not play file"

/* read, write file */
#define ERR_TOO_LARGE_SIZE			"ERROR: Cannot read. Size is too large"
#define ERR_SEEK_ADDRESS			"ERROR: Can't seek to address to read"
#define ERR_WRITING_DATA			"ERROR: Writing data error"

/* amptest */
#define	ERR_INIT_DIRECTFB	 		"ERROR: DirectFB don't init"
#define ERR_INIT_AMP				"ERROR: Init Amp"
#define ERR_START_AMP				"ERROR: Start Amp"
#define ERR_START_PRESENTATION		"ERROR: Unable to start presentation, abort!"
#define ERR_START_PRESENTATION_TO	"ERROR: StartPresentationTo"
#define ERR_LOAD_FILE				"ERROR: Load file"

#define ERR_RETURN_DFB_BUSY			"ERROR: The resource or device is busy"
#define ERR_RETURN_DFB_NOIMPL		"ERROR: Feature not implemented"
#define ERR_RETURN_DFB_UNSUPPORTED	"ERROR: The requested operation or an argument is (currently) not supported. "
#define ERR_RETURN_DFB_FAILURE		"ERROR: Return DFB_FAILURE - A general or unknown error occured"

#define ERR_TOO_MANY_VIDEO_LAYERS	"ERROR: Too many video layers"
#define ERR_TOO_MANY_AMPS			"ERROR: Too many AMPs"
#define ERR_TOO_MANY_PLAY_ITEMS		"ERROR: Too many play items"

#define ERR_CREATE_DIRECTFB				"ERROR: Create DirectFB"
#define ERR_CREATE_WINDOW				"ERROR: Create a window within a layer"
#define ERR_CREATE_SURFACE				"ERROR: Create Surface"
#define ERR_CREATE_IMAGE_PROVIDER		"ERROR: Can not Create Image Provider"
#define ERR_CREATE_FILE_DESCRIPTOR		"ERROR: Create a file descriptor for reading events"
#define ERR_CREATE_INPUT_BUFFER_EVENTS	"ERROR: Create an input buffer for key events"

#define ERR_RENDER_TO_SURFACE		"ERROR: Render to surface"
#define ERR_OPEN_DATA_BUFFER		"ERROR: Could not open data buffer for file!"
#define ERR_MEDIA_NOT_PREBUFFER		"ERROR: Media is NOT prebuffered!"
#define ERR_OPEN_MEDIA				"ERROR: Could not open media"
#define ERR_INVALID_AMP_REFERENCE	"ERROR: Invalid AMP reference"
#define ERR_NO_DESTINATION_SURFACE	"ERROR: No destination surface specified for image renderer, abort!"

#define ERR_STRETCH_BLIT			"ERROR: Stretch Blit"
#define ERR_GEOMETRIC_TRANS			"ERROR: Geometric transformations"

#define ERR_ENABLE_CURSOR			"ERROR: Enable Cursor"
#define ERR_FLIP_SURFACE			"ERROR: Flip/Update surface buffers"
#define ERR_PLIT_SURFACE			"ERROR: Blit an area from the source to this surface"
#define ERR_BLIT_SURFACE			"ERROR: Blit an area from the source to this surface"
#define ERR_LOCK_SURFACE			"ERROR: Lock surface"
#define ERR_UNLOCK_SURFACE			"ERROR: UnLock surface"
#define ERR_CLEAR_SURFACE			"ERROR: Clear Surface"

#define ERR_GET_ENCODER_CONFIG		"ERROR: Get current encoder configuration."
#define ERR_GET_LAYER_CONFIG		"ERROR: Get current layer configuration."
#define ERR_GET_OUTPUT_CONFIG		"ERROR: GetOutputConfiguration failed on all encoders!"
#define ERR_GET_MIXER_CONFIG		"ERROR: Get current mixer configuration!"

#define ERR_GET_DESTINATION			"ERROR: Surface destination not available, abort!"
#define ERR_GET_INTERFACE			"ERROR: GetInterface - Advanced Media Provider instantiation failed!"
#define ERR_GET_EVENT_BUFFER		"ERROR: Advanced Media Provider event buffer retrieval failed!"
#define ERR_GET_DISPLAY_LAYER		"ERROR: Get Display Layer - Retrieve an interface to a specific display layer"
#define ERR_GET_SCREEN				"ERROR: Get Screen - Retrieve an interface to a specific screen"
#define ERR_GET_SURFACE				"ERROR: Get Surface - Get an interface to layer's surface"
#define ERR_GET_SURFACE_DESCRIPTION	"ERROR: Get a surface description that best matches the video contained in the file"
#define ERR_GET_COLOR_ADJUSTMENT	"ERROR: Get Color Adjustment"
#define ERR_GET_PIXEL_FORMAT		"ERROR: Get the current pixel format."
#define ERR_GET_SIZE_WINDOW			"ERROR: Get the size of the window in pixels."

#define ERR_SET_LAYER_CONFIG		"ERROR: Set layer configuration."
#define ERR_SET_ENCODER_CONFIG		"ERROR: Set Encoder Configuration failed!"
#define ERR_SET_OUTPUT_CONFIG		"ERROR: SetOutputConfiguration failed on all encoders!"
#define ERR_SET_MIXER_CONFIG		"ERROR: Set mixer configuration.!"

#define ERR_SET_SCREEN_RECTANGLE	"ERROR: SetScreenRectangle - Set location on screen in pixels"
#define ERR_SET_SOURCE_RECTANGLE	"ERROR: Set the source rectangle."
#define ERR_SET_OPACITY				"ERROR: Set Opacity - Set global alpha factor"
#define ERR_SET_LEVEL				"ERROR: Set Level - Move the layer below or on top of others"
#define ERR_SET_COOPERTATIVE_LEVEL	"ERROR: Set cooperative level to get control over the layer"
#define ERR_SET_COLOR_ADJUSTMENT	"ERROR: Set Color Adjustment"
#define ERR_SET_BACKGROUND_MODE		"ERROR: Set Background Mode"
#define ERR_SET_BACKGROUND_COLOR	"ERROR: Set the color for a solid colored background"

#endif
