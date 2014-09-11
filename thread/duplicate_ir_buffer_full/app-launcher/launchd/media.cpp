/*****************************************
 Copyright  2005
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**

*/

//#include "include/generalmedia.h"

extern "C" {
#include <divine.h>
}


#ifdef REAL_TIME_THREADS
#include <sched.h>
#endif

#ifdef DO_DMALLOC
#include <dmalloc.h>
#endif

#include "media.h"

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
// storage for nominal deinterlacing mode while restrictions exists due to  the VCR scaler being unavailable; -1 means VCR is available
DFBDeinterlacingMode				 nominal_deint_mode = (DFBDeinterlacingMode)-1;	
// index of the AMP that has the focus curently
int 										   ampIndex ;	
// AMP that has the focus curently
IAdvancedMediaProvider							   *amp = NULL;	
// event file descriptor of the AMP that has the focus curently	
int						 ampfd;		
// number of amps - first amps entries in amp_table
int												   amps;						
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
#include "init_directfb.c"
#include "gui.c"

/////////////////////////////////////// end /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


