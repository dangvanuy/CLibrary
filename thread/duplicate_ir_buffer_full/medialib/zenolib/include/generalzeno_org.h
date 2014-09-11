
/**
* @file   zenodefine.h
* @brief  Define for zeno
*/

/**
$Log : $
*/


#ifndef __GENERAL_ZENO_H__
#define __GENERAL_ZENO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

//DCC_HD core
#ifdef	LOADER_HAVE_IDLE_TIME
#include "rmdiscapi/include/rmdiscapi.h"
#endif //  #ifdef	LOADER_HAVE_IDLE_TIME

#ifndef NO_RED_SUPPORT
#include "curacao/src/CCddaNavigator.h"
#include "curacao/src/CDvdNavigator.h"
#endif // #ifndef NO_RED_SUPPORT
#include "mono/src/CMonoNavigator.h"

#ifndef NO_BLUE_SUPPORT
#include "brd/src/CBRDPresEng.h"
#endif // #ifndef NO_BLUE_SUPPORT

#include "mono/src/CPCMPresEng.h"
#include "mono/src/CPCMXPresEng.h"
#include "mono/src/CXPEGPresEng.h"
#include "core/src/CBitstreamDecoder.h"
#include "mono/src/CSimplePBC.h"

#ifndef NO_RED_SUPPORT
#include "curacao/src/C86xxDecoder.h"
#endif // #ifndef NO_RED_SUPPORT

#include "src/MediaDetection.h"

RM_EXTERN_C_BLOCKSTART
#include <direct/thread.h>
#include <directfb_strings.h>
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
//dummy callback
#include "include/mediaconfig_dump.h"
#include "include/media.h"
#include "include/media_def.h"
#include "include/hotplug_handler.h"
#include "include/mediaplaylist.h"
RM_EXTERN_C_BLOCKEND

#include "debug.h"
#include "zenodefine.h"

#endif //__GENERAL_ZENO_H__

