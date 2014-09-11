/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

#ifndef __GENERAL_MEDIA_H__
#define __GENERAL_MEDIA_H__

#include <ctype.h>
#include <sys/resource.h>

#include <rmdef/rmdef.h>
#include <emhwlib/include/emhwlib.h>
#include <rua/include/rua.h>
//#include <mdefs.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <directfb.h>
#include <directfb_keynames.h>
#include <directfb_version.h>
#include <direct/messages.h>
#include <direct/thread.h>
#include <direct/util.h>
#include <directfb_strings.h> 
//#include <misc/conf.h> 
#include <time.h>     /* for `clock()'   */
#include <stdio.h>    /* for `D_INFO()' */
#include <stdlib.h>   /* for `rand()'    */
#include <unistd.h>   /* for `sleep()'   */
#include <math.h>     /* for `sqrt()'    */
#include <assert.h>	  /* for assert()	*/
#include <wchar.h>     /* for `sqrt()'    */
#include <stddef.h>   /* for `offsetof()' */
#include <ctype.h>
#include "util.h"
#include <sys/vfs.h>
//RUA
#include <rua/include/rua.h>
#include <rua/include/rua_property.h>
#include <dcc/include/dcc.h>
#include <dcc/src/dcc_common.h>
#include <rmlibcw/include/rmlibcw.h>
typedef unsigned short UINT16;
#include <advancedmediaprovider.h>

#include <cdefs_lpb.h>
#ifndef NO_RED_SUPPORT
		#include <cdefs_cdda.h>
		#include <cdefs_dvd.h>
		#include <cdefs_vr.h>
#ifndef NO_VCD_SUPPORT	
		#include <cdefs_vcd.h>
#endif // #ifndef NO_VCD_SUPPORT	
	
#endif

#ifndef NO_BLUE_SUPPORT
        #include <cdefs_brd.h>
        #include <cdefs_bdre.h>
        #include <cdefs_bdj.h>
#endif

#ifndef NO_DTV_SUPPORT
#ifndef RM_PSM_Context
struct RM_PSM_Context {
    RMint32 validPSMContexts;
    RMint32 currentActivePSMContext;
    RMuint32 selectedPSMContextMask;
    RMuint32 keyflags;
};
#endif
#include <cdefs_dtv.h>
#include "cdefs_cap.h"
#endif // DTV_SUPPORT
#include <globals.h>
#include "mediaplay_define.h"
#include "media_def.h"
#include "media.h"

#endif /*__GENERAL_MEDIA_H__*/
