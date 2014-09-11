
#ifndef __DTV_DEFINE_H__
#define __DTV_DEFINE_H__

#include <directfb.h>

#include <rmdef/rmdef.h>
#include <emhwlib/include/emhwlib_displaytypes.h>
#include <emhwlib/include/emhwlib.h>
#include <rua/include/rua.h>
#include <rua/include/rua_property.h>
#include <rmlibcw/include/rmcriticalsections.h>
#include <rmrtk/include/rmrtk.h>

#include "em86xx_private_defs.h"
#include "em86xx_private_mem.h"
#include "em86xx_private_mem_mngr.h"
#include "mem_mngr.h"
#include "em86xx_ruaevents.h"
#include "em86xx_userevents.h"

#include "../mediaplay_define.h"
//#include "../generalmedia.h"

#define MAX_TUNER 3

#ifndef STRING_HYPHEN
#define STRING_HYPHEN					"|"
#endif

#ifndef STRING_DOUBLE_HYPHEN
#define STRING_DOUBLE_HYPHEN			"||"
#endif

#ifndef STRING_TRIO_HYPHEN
#define STRING_TRIO_HYPHEN				"|||"
#endif

#define EPG_PIP_X      51//778//1330//1360
#define EPG_PIP_Y      38//50//140//106
#define EPG_PIP_WIDTH     395//424
#define EPG_PIP_HEIGHT     220//253
//#define GUI_ON_OSD_WIDTH			1920 	
//#define GUI_ON_OSD_HEIGHT			1080 	
//#define SEC_WINDOW_ON_OSD_WIDTH		1920	
//#define SEC_WINDOW_ON_OSD_HEIGHT	1080	
//#define SEC_WINDOW_ON_OSD_POSY		0
//#define SEC_WINDOW_ON_OSD_POSX		0
#ifdef WITH_CC
#define EDT_DATA    0x10            ///< flag in CPresEng::EEDType values for specifying embedded data extraction. If this flag is specified in the dataType parameter of the AllocEmbeddedSubstream() call, embedded data can be extracted through GetSubstreamData() calls.

#define EDT_SPU     0x20            ///< flag in CPresEng::EEDType values for specifying embedded data display on the SPU scaler

#define EDT_FORMAT_MASK     0x0F    ///< mask for extracting the embedded data format from CPresEng::EEDType values

/// Embedded data type - use #EDT_FORMAT_MASK to extract the format and #EDT_DATA and #EDT_SPU to extract processing mode
enum EEDType
{
	EDT_User_Data,                              ///< user data format as meta buffer payload
	EDT_CC_608,                                 ///< Closed Caption data in EIA-608 format sent directly to the (composite/SVideo) video output
	EDT_CC_708,                                 ///< Closed Caption data in EIA-708 format sent directly to the (composite/SVideo) video output
	EDT_CC_608_Data = EDT_CC_608 | EDT_DATA,    ///< Closed Caption data in EIA-608 format passed as meta buffer payload consisting of one or more MRUA #CCFifo_CCEntry_type structures
	EDT_CC_708_Data = EDT_CC_708 | EDT_DATA,    ///< Closed Caption data in EIA-708 format passed as meta buffer payload consisting of one or more MRUA #CCFifo_CCEntry_type structures
	EDT_CC_608_SPU = EDT_CC_608 | EDT_SPU,      ///< Closed Caption data in EIA-608 format decoded internally and displayed (on all video outputs) using the SPU scaler
	EDT_CC_708_SPU = EDT_CC_708 | EDT_SPU,      ///< Closed Caption data in EIA-708 format decoded internally and displayed (on all video outputs) using the SPU scaler
};
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
enum PIPType
{
	PIP_NONE = 0,
	PIP_BETWEEN_TUNERS,
	PIP_BETWEEN_PMTS,
	PIP_BETWEEN_PLAY_FILE_CHAN,
};

enum PIP_Show_Position
{
	PIP_SHOW_TOP_LEFT = 0,
	PIP_SHOW_TOP_RIGHT,
	PIP_SHOW_BOTTOM_LEFT,
	PIP_SHOW_BOTTOM_RIGHT,
};

enum Speaker
{
    SPEAKER_LEFT,
    SPEAKER_RIGHT,
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

typedef TKeyMode (*KeyCallbackFunction_t)(int ampIndex, DFBEvent *evt);
typedef void (*IdleProcess_t)(int ampIndex);



#endif //__DTV_DEFINE_H__
