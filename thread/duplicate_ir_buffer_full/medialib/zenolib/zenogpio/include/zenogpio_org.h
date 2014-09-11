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
	$Log: zenogpio_org.h,v $
	Revision 1.1.1.1  2013/02/20 09:35:12  liempham
	Imported sources

	Revision 1.1  2013/01/24 01:02:25  hdo
	BugId: 38635
	Description:
	 - Import dtv apis into sigmaobjectextension
	SEND_COMMENT_BUGZILLA

	Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
	add new
	
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
//#include "../rmdef/rmdef.h"
#include "rmdef/rmdef.h"
#include "emhwlib_hal/include/emhwlib_registers.h"
#include "llad/include/gbus.h"
//#include "../helper/helper.h"
enum UART_type{
	UART_RX = 0,
		UART_CTS,
		UART_DSR,
		UART_DCD,
		UART_TXD,
		UART_RTS,
		UART_DTR
};

enum GPIO_type{
	GPIO_0 = 0,GPIO_1,GPIO_2,GPIO_3,GPIO_4,GPIO_5,GPIO_6,GPIO_7,
	GPIO_8,GPIO_9,GPIO_10,GPIO_11,GPIO_12,GPIO_13,GPIO_14,GPIO_15,
	GPIO_16,GPIO_17,GPIO_18,GPIO_19,GPIO_20,GPIO_21,GPIO_22,GPIO_23,
	GPIO_24,GPIO_25,GPIO_26,GPIO_27,GPIO_28,GPIO_29,GPIO_30,GPIO_31
};

class CGpio
{
	public:
		struct llad *pLLAD;
		void* pGBus;

		CGpio();
		~CGpio();
		//void Write_Gpio(enum GPIOId_type pin,RMbool val);
		void WriteGpioUart1(enum UART_type pin, RMuint8 ishigh);
		void WriteGpioUart0(RMuint8 pin, RMuint8 ishigh);
		void WriteGpio(RMuint8 pin, RMuint8 ishigh);
		void ReadGpioUart0(RMuint32* pData);
		void ReadGpioUart1(RMuint8 pin, RMuint8* pData);
		void ReadGpio(RMuint32* pData);
		void WriteGpioDir(RMuint32 value);
		void WriteGpioUart0Dir(RMuint32 value); 	
		void Write_Gpio(enum GPIO_type pin, RMuint8 ishigh);
};

#endif

