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
	$Log: zenogpio.cpp,v $
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
#include "zenogpio.h"

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
CGpio::CGpio() {
	mbIsLEDOn = TRUE;
	mpLLAD = NULL;
	mpGBus = NULL;
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
CGpio::~CGpio()
{
	if(mpGBus != NULL) {
		gbus_close((gbus*)mpGBus);
	}

	if(mpLLAD != NULL) {
	       llad_close(mpLLAD);
	}
}

/**
 *
 */
void CGpio::WriteGpio(enum GPIO_type pin, RMuint8 ishigh)
{
	if(mpLLAD == NULL || mpGBus == NULL)
	{
		if( (s = mum_init( 0, &mpLLAD, &mpGBus)) != RM_OK )
		{
			MPRINT(("main !mum_init FAILED %d\ncheck your 86xx driver\n", s));
			return;
		}
		else {
			MPRINT(("init mpLLAD && mpGBus OK\n"));
		}
	}
	else {
		//MPRINT(("mpLLAD != NULL && mpGBus != NULL\n"));
	}

	memset(&mGpioConfig, 0 , sizeof(GPIO_CONFIG));	
	
	mGpioConfig.pGBus = mpGBus;
	mGpioConfig.nGpioCfg = GpioCfg_Gpio;
	mum_gpioSet(&mGpioConfig, pin, ishigh);
}

/**
 *
 */
void CGpio::ReadGpio(enum GPIO_type pin, RMuint8 *pData)
{
	if(mpLLAD == NULL || mpGBus == NULL)
	{
		if( (s = mum_init( 0, &mpLLAD, &mpGBus)) != RM_OK )
		{
			MPRINT(("main !mum_init FAILED %d\ncheck your 86xx driver\n", s));
			return;
		}
		else {
			MPRINT(("init mpLLAD && mpGBus OK\n"));
		}
	}
	else {
		//MPRINT(("mpLLAD != NULL && mpGBus != NULL\n"));
	}

	memset(&mGpioConfig, 0 , sizeof(GPIO_CONFIG));	
	
	mGpioConfig.pGBus = mpGBus;
	mGpioConfig.nGpioCfg = GpioCfg_Gpio;
	mum_gpioGet(&mGpioConfig, pin, pData);
}

/**
 * Set LED
 * set ON if 'flag' is TRUE. Otherwise set OFF
 */
void CGpio::SetLED(RMbool flag)
{
	if(flag == TRUE) {
		if(!mbIsLEDOn) {
			WriteGpio(GPIO_14, 0); // set ON
			mbIsLEDOn = !mbIsLEDOn;
		}
	}
	else {
		if(mbIsLEDOn) {
			WriteGpio(GPIO_14, 1); // set OFF
			mbIsLEDOn = !mbIsLEDOn;
		}
	}
}

/**
 *
 */
void CGpio::WriteGpioDir(RMuint32 value)
{
	if(mpLLAD == NULL || mpGBus == NULL)
	{
		if( (s = mum_init( 0, &mpLLAD, &mpGBus)) != RM_OK )
		{
			MPRINT(("main !mum_init FAILED %d\ncheck your 86xx driver\n", s));
			return;
		}
		else {
			MPRINT(("init mpLLAD && mpGBus OK\n"));
		}
	}
	else {
		//MPRINT(("mpLLAD != NULL && mpGBus != NULL\n"));
	}

	gbus_write_uint32((gbus*)mpGBus, REG_BASE_system_block + SYS_gpio_dir, value);
}
