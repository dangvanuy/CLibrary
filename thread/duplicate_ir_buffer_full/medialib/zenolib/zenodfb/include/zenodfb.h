/******************************************************************************
 File name: 	zenodfb.h
 Copyright (c) 2007 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
 Version
 Date
******************************************************************************/
/**
* @file   zenodfb.h
* @brief  zeno directfb header
*/

/**
$Log: zenodfb.h,v $
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

Revision 1.3  2010/01/19 09:27:21  khanhngo
BugID: none

Revision 1.2  2010/01/19 09:21:41  khanhngo
BugID: none

 */

#ifndef _INIT_DIRECTFB_H__
#define _INIT_DIRECTFB_H__
void	InitDFB(int argc, char *argv[]);
void	DeInitDFB(void);

#endif
