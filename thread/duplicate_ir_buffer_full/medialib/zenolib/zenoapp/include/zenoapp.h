/******************************************************************************
 File name: 	zenoapp.h
 Copyright (c) 2007 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
 Version	1.0
 Date		Jan 13, 2010
******************************************************************************/
/**
* @file   zenoapp.h
* @brief  Zeno application header
*/

/**
$Log: zenoapp.h,v $
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

Revision 1.3  2010/08/09 03:09:27  trihuynh
BugId : None
	Port BDFramework from 3.7.2 --> 3.9.0.

Revision 1.2  2010/08/04 02:57:13  trihuynh
BugId : None
	Add NetFlix library.

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

Revision 1.3  2010/01/19 09:21:41  khanhngo
BugID: none

Revision 1.2  2010/01/19 07:24:28  khanhngo
BugID: none

 */

#ifndef __ZENOAPP_H__
#define __ZENOAPP_H__

#include <advancedmediaprovider.h>

typedef unsigned short UINT16;
class CZenoApp
{
	RMint32					nRun; /**< */
	
	void					SwitchBetweenOsdLayerAndSecondOsdLayer(int appIndex);
public:
	virtual void			OnInit() { };
	virtual void			OnKeyDown(RMuint32 nKey) {};
	virtual void			OnTerminate() {};
	virtual void			InitializeStagecraftLib(int appIndex){};
	virtual void			UninitializeStagecraftLib(){};
	virtual int				GetStageWindowStatus(){ return 0; };
  	
							CZenoApp();
	virtual					~CZenoApp();

	RMint32					Run(int argc, char *argv[]);
	void					Terminate(RMint32 nExitCode);
};

#endif
