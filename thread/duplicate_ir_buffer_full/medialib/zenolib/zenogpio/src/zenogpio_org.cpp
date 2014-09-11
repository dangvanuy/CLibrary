/******************************************************************************
*
* File name		: zenogpio.cpp
* Copyright (c) 2008, Kenvi.  All rights reserved.
* Created Date	: 21-May-2009
* Autho	r		: 
* Purpose		: 
* Note			: 
*
******************************************************************************/
/** 
	$Log: zenogpio_org.cpp,v $
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
#include "zenogpio.h"

RM_EXTERN_C_BLOCKSTART

#include "tuner/helper/helper.h"

RM_EXTERN_C_BLOCKEND

/**
* 
* <b>FUNCTION: </b>
*
* GPio()
*
* <b>DESCRIPTION: </b>
* 
* Constructor
* 
* @param	No
*       
* @return	No
*/
CGpio::CGpio() 
{
	RMuint32 chip_num = 0;
	RMascii device[40];
	sprintf(device, "%lu", chip_num);
	
	RMstatus s;
	if( (s = mum_init( 0, &pLLAD, &pGBus)) != RM_OK )
	{
		MPRINT(("main !mum_init FAILED %d\ncheck your 86xx driver\n", s));
		exit(s);
	}

}

/**
* <b>FUNCTION: </b>
*
* ~Gpio()
*
* <b>DESCRIPTION: </b>
* 
* Destructor
* 
* @param	No
*       
* @return	No
*/
CGpio::~CGpio() {}

/*
 *	RX  0
 *	CTS 1
 *	DSR 2
 *	DCD 3
 *	TXD 4
 *	RTS 5
 *	DTR 6

 */
void CGpio::WriteGpioUart1(enum UART_type pin, RMuint8 ishigh)
{
	
	struct llad *pLLAD;
	struct gbus *pGBus;
	RMuint32 chip_num = 0;
	RMascii device[40];
	sprintf(device, "%lu", chip_num);
	
	pLLAD = llad_open(device);
	pGBus = gbus_open(pLLAD);
	//Set CPU_UART_GPIO_MODE . Value : 0x7f7f : All pin are in the GPIO mode.
	gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART1_base + CPU_UART_GPIOMODE,0x7F7F);
	//Set CPU_UART_GPIO_DIR : direction of pin : 0 : input , 1 : output
	gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART1_base + CPU_UART_GPIODIR, ( 0x0100 | 1) << pin);
	if(ishigh)
		gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART1_base + CPU_UART_GPIODATA, (0x0100 |1) << pin);
	else
		gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART1_base + CPU_UART_GPIODATA, 0x0100 << pin);
		
	gbus_close(pGBus);
	llad_close(pLLAD);
	
	
}

void CGpio::WriteGpioUart0(RMuint8 pin, RMuint8 ishigh)
{
	
	struct llad *pLLAD;
	struct gbus *pGBus;
	RMuint32 chip_num = 0;
	RMascii device[40];
	sprintf(device, "%lu", chip_num);
	
	pLLAD = llad_open(device);
	pGBus = gbus_open(pLLAD);
	//Set CPU_UART_GPIO_MODE . Value : 0x7f7f : All pin are in the GPIO mode.
	//gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART0_base + CPU_UART_GPIOMODE,0x7F7F);
	//gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART0_base + CPU_UART_GPIOMODE,0x7F11);
	//Set CPU_UART_GPIO_DIR : direction of pin : 0 : input , 1 : output
	gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART0_base + CPU_UART_GPIODIR, ( 0x0100 | 1) << pin);
	if(ishigh)
		gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART0_base + CPU_UART_GPIODATA, (0x0100 |1) << pin);
	else
		gbus_write_uint32(pGBus, REG_BASE_cpu_block + CPU_UART0_base + CPU_UART_GPIODATA, 0x0100 << pin);
	
	gbus_close(pGBus);
	llad_close(pLLAD);
	
	
}


void CGpio::WriteGpio(RMuint8 pin, RMuint8 ishigh)
{
	GPIO_CONFIG	gpioConfig;
	
	
	memset(&gpioConfig, 0 , sizeof(GPIO_CONFIG));	
	
	gpioConfig.pGBus = pGBus;
	gpioConfig.nGpioCfg = GpioCfg_Gpio;
    mum_gpioSet(&gpioConfig, pin, ishigh);
  
}
/*
void CGpio::Write_Gpio(enum GPIO_type pin, RMuint8 ishigh)
{
	struct llad *pLLAD ;
	struct gbus *pGBus ;
	RMuint32 chip_num = 0;
	RMascii device[40];
	RMuint8 uiPin = pin;
    sprintf(device, "%lu", chip_num);

	pLLAD = llad_open(device);
	pGBus = gbus_open(pLLAD);

	if (uiPin>=GPIO_16) {
		uiPin -=16;		
		//directtion Out put (1)
		gbus_write_uint32(pGBus, 0x00020000 + 0x710c, ( 0x00010000 | 1) << uiPin);		
		if(ishigh)
			gbus_write_uint32(pGBus, 0x00020000 + 0x7110, (0x00010000 |1) << uiPin);
		else
			gbus_write_uint32(pGBus, 0x00020000 + 0x7110, 0x00010000 << uiPin);
	}else{
		//directtion
		gbus_write_uint32(pGBus, REG_BASE_system_block + SYS_gpio_dir, ( 0x00010000 | 1) << uiPin);
        if(ishigh)
			gbus_write_uint32(pGBus, REG_BASE_system_block + SYS_gpio_data, (0x00010000 |1) << uiPin);
		else
			gbus_write_uint32(pGBus, REG_BASE_system_block + SYS_gpio_data, 0x00010000 << uiPin);
	}
	
	gbus_close(pGBus);
	llad_close(pLLAD);

}*/

void CGpio::ReadGpioUart0(RMuint32* pData)
{
	*pData = gbus_read_uint32((gbus*)pGBus, REG_BASE_cpu_block + CPU_UART0_base + CPU_UART_GPIODATA);
	//mum_exit(pGBus, pLLAD);
}

void CGpio::ReadGpioUart1(RMuint8 pin, RMuint8* pData)
{
	
	struct llad *pLLAD;
	struct gbus *pGBus;
	RMuint32 chip_num = 0;
	RMascii device[40];
	sprintf(device, "%lu", chip_num);
	
	pLLAD = llad_open(device);
	pGBus = gbus_open(pLLAD);

	*pData = (gbus_read_uint32(pGBus, REG_BASE_cpu_block + CPU_UART1_base + CPU_UART_GPIODATA) & (1 << pin))?1:0;
	
	gbus_close(pGBus);
	llad_close(pLLAD);
	
}

void CGpio::ReadGpio(RMuint32* pData)
{
	*pData = gbus_read_uint32((gbus*)pGBus, REG_BASE_system_block + SYS_gpio_data);
	//mum_exit(pGBus, pLLAD);
}

void CGpio::WriteGpioDir(RMuint32 value)
{
	gbus_write_uint32((gbus*)pGBus, REG_BASE_system_block + SYS_gpio_dir, value);
}

void CGpio::WriteGpioUart0Dir(RMuint32 value)
{
	gbus_write_uint32((gbus*)pGBus, REG_BASE_cpu_block + CPU_UART0_base + CPU_UART_GPIODIR, value);
}

void CGpio::Write_Gpio(enum GPIO_type pin, RMuint8 ishigh)
{
	printf("\n[VuongT]-----------------Write_Gpio---------------------------------------\n");
    struct llad* 	pLLAD;
	RMvoid* 	pGBus;

	RMstatus s;
	GPIO_CONFIG	gpioConfig;
	
	
	if( (s = mum_init( 0, &pLLAD, &pGBus)) != RM_OK )
	{
		MPRINT(("main !mum_init FAILED %d\ncheck your 86xx driver\n", s));
		exit(s);
	}

	memset(&gpioConfig, 0 , sizeof(GPIO_CONFIG));	
	
	gpioConfig.pGBus = pGBus;
	gpioConfig.nGpioCfg = GpioCfg_Gpio;
	printf("\n[VuongT]----GPIO--0----------------------------------------------------\n");
    mum_gpioSet(&gpioConfig, pin, ishigh);
	printf("\n[VuongT]----GPIO--01----------------------------------------------------\n");
  
}
