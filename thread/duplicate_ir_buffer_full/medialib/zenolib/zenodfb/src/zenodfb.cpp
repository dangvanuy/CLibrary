/******************************************************************************
 File name: 	zenodfb.cpp
 Copyright (c) 2010 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
 Version:
 Date:
******************************************************************************/
/**
$Log: zenodfb.cpp,v $
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

*/
#include "generalzeno.h"
#include "zenodfb.h"

/**
* <b>FUNCTION: </b>
*
* InitDFB()
*
* <b>DESCRIPTION: </b>
* 
* @param	No
*   
* @return   No
*/
void InitDFB(int argc, char *argv[])
{
	global_init(&argc, &argv);
}

/**
* <b>FUNCTION: </b>
*
* DeInitDFB()
*
* <b>DESCRIPTION: </b>
* 
* @param    No
* 
* @return   No
*/
void DeInitDFB()
{
	global_uninit();
}