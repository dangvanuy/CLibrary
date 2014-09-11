/******************************************************************************
 File name: 	debug.h
 Copyright (c) 2007 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
 Version	1.0
 Date		25-Feb-2008
******************************************************************************/
/**
* @file   debug.h
* @brief  Macro for debug
*/

/** 
$Log: debug.h,v $
Revision 1.1.1.1  2013/02/20 09:35:12  liempham
Imported sources

Revision 1.2  2013/01/24 01:02:24  hdo
BugId: 38635
Description:
 - Import dtv apis into sigmaobjectextension
SEND_COMMENT_BUGZILLA

Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
add new

Revision 1.1  2011/04/25 04:35:05  liempham

BugId: 31699 add sc_sdk module
SEND_COMMENT_BUGZILLA

Revision 1.2  2010/07/22 02:36:36  trihuynh
BugID : None
	Add detection disc function.

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

Revision 1.1.1.1  2010/01/18 08:09:17  cvsadmin
The import ZENO_FlashLite_SMP865x_ZenoFramework
*/

#ifndef __ZENO_DEBUG_H__
#define __ZENO_DEBUG_H__

#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif
	void debug_printf(char *fmt, ...);    /* print a message */
#ifdef __cplusplus
}
#endif

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
   /*
    * if in debug mode, print message; otherwise do nothing.
    *
    * Note: paramter list for DMSG must be enclosed in double parenthesis!
    * (outer parenthesis are for the macro, inner ones for debug_printf() )
    */

	#ifndef DPRINTF
	#define DPRINTF(str, args...)   fprintf(stderr, str, ##args)  /* debug output*/
	#endif

	#define ZDMSG(args)  do { fprintf(stderr, "[%s:%d]", __FILE__, __LINE__); debug_printf args; } while(0);
	#define ZERROR(args) do{ fprintf(stderr, "[%s:%d]", __FILE__, __LINE__); debug_printf args; } while(0);
	#define ZINFO(args)  do { fprintf(stderr, "[%s:%d]", __FILE__, __LINE__); debug_printf args; } while(0);

	#define GETDBG { printf("[%s:%d:%s] ", __FILE__, __LINE__, __FUNCTION__); getchar(); }

	#define MAINAPP_ASSERT(x)			assert(x)

#else    /* not DEBUG */
	
	#ifndef DPRINTF
	#define DPRINTF(str, args...)	/* no debug output*/
	#endif

	#define ZDMSG(args)    /* don't compile anything for ZDMSG */
	#define ZERROR(args)    /* don't compile anything for ZDMSG */
	#define ZINFO(args)    /* don't compile anything for ZDMSG */
	#define GETDBG
	#define MAINAPP_ASSERT(x)
	
#endif
#endif //__ZENO_DEBUG_H__
