/******************************************************************************
*
* File name		: zenogpio.h
* Copyright (c) 2008, Kenvi.  All rights reserved.
* Created Date	: 21-May-2009
* Autho	r		: 
* Purpose		: 
* Note			: 
*
******************************************************************************/
/** 
	$Log: zenogpio.h,v $
	Revision 1.1.1.1  2013/02/20 09:35:12  liempham
	Imported sources

	Revision 1.2  2013/01/24 01:02:25  hdo
	BugId: 38635
	Description:
	 - Import dtv apis into sigmaobjectextension
	SEND_COMMENT_BUGZILLA

	Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
	add new
	
	Revision 1.2  2012/12/10 00:12:29  thinhhoang
	Update GPIO code
	
	Revision 1.1.1.1  2012/11/26 03:13:26  cvsadmin
	add new
	
	Revision 1.1.1.1  2012/11/26 01:22:28  cvsadmin
	add new
	
	Revision 1.1.1.1  2012/10/22 03:44:37  cvsadmin
	add new
	
	Revision 1.1.1.1  2012/09/26 07:37:16  cvsadmin
	add new
	
	Revision 1.1.1.1  2012/09/17 03:23:24  cvsadmin
	add new
	
	Revision 1.1.1.1  2012/08/09 02:12:15  cvsadmin
	add new
	
	Revision 1.1.1.1  2012/06/20 05:47:36  cvsadmin
	add new
	
	Revision 1.1.1.1  2012/05/21 08:43:59  cvsadmin
	add new
	
	Revision 1.2  2012/04/19 09:47:54  uydang
	*** empty log message ***
	
	Revision 1.2  2011/08/26 06:47:33  liempham

	BugId: 31699

	 - replace function sprintf(...) by function snprintf(...)

	SEND_COMMENT_BUGZILLA

	Revision 1.1  2011/04/25 04:35:05  liempham

	BugId: 31699 add sc_sdk module
	SEND_COMMENT_BUGZILLA

	Revision 1.1  2010/12/02 06:08:18  vuongtran
	VuongT add GPIO
	
	Revision 1.1  2010/08/18 08:50:33  minhdo
	add : VuongTran
	
	Revision 1.2  2010/06/11 07:23:54  tientran
	*** empty log message ***
	
	Revision 1.1  2010/06/11 06:02:51  thuytran
	add file
	cVS: ----------------------------------------------------------------------
	
*/
#ifndef _CPU_UART_GPIO_H
#define _CPU_UART_GPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rmdef/rmdef.h"
#include "emhwlib_hal/include/emhwlib_registers.h"
#include "llad/include/gbus.h"

RM_EXTERN_C_BLOCKSTART
#include "tuner/helper/helper.h"
RM_EXTERN_C_BLOCKEND

enum GPIO_type{
	GPIO_0 = 0,GPIO_1,GPIO_2,GPIO_3,GPIO_4,GPIO_5,GPIO_6,GPIO_7,
	GPIO_8,GPIO_9,GPIO_10,GPIO_11,GPIO_12,GPIO_13,GPIO_14,GPIO_15,
	GPIO_16,GPIO_17,GPIO_18,GPIO_19,GPIO_20,GPIO_21,GPIO_22,GPIO_23,
	GPIO_24,GPIO_25,GPIO_26,GPIO_27,GPIO_28,GPIO_29,GPIO_30,GPIO_31
};

class CGpio
{
private:
	RMbool mbIsLEDOn;
	struct llad* 	mpLLAD;
	RMvoid* 	mpGBus;
	RMstatus 	s;
	GPIO_CONFIG	mGpioConfig;

public:
	CGpio();
	~CGpio();

	void ReadGpio(enum GPIO_type pin, RMuint8 *pData);
	void WriteGpio(enum GPIO_type pin, RMuint8 ishigh);
	void WriteGpioDir(RMuint32 value);
	void SetLED(RMbool flag);
};

#endif
