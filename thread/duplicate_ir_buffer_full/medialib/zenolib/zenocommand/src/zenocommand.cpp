/******************************************************************************
 File name: 	zenocommand.cpp
 Copyright (c) 2009 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose: implement command pattern, apply for handling multi-windows 
 Note:	
 Author: Phu Nguyen.
 Version	1.0
 Date		25-July-2009
******************************************************************************/
/**
$Log: zenocommand.cpp,v $
Revision 1.1.1.1  2013/02/20 09:35:12  liempham
Imported sources

Revision 1.2  2013/01/24 01:02:24  hdo
BugId: 38635
Description:
 - Import dtv apis into sigmaobjectextension
SEND_COMMENT_BUGZILLA

Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
add new

Revision 1.1  2011/04/25 06:40:12  liempham

BugId: 31699 add flags : NO_DISC_SUPPORT, NO_RED_SUPPORT, NO_BLUE_SUPPORT

Revision 1.1.1.1  2010/07/19 02:47:56  cvsadmin
add new

*/

#include "debug.h"
#include "zenocommand.h"

/**
* 
* <b>FUNCTION: </b>
*
* CZenoCommandList()
*
* <b>DESCRIPTION: </b>
* 
* Constructor
* 
* @param	No
*		
* @return	No
*
*/
CZenoCommandList::CZenoCommandList()
{
	DPRINTF("+++ \n");
	for (int i = 0; i < ZENO_MAX_COMMAND_LIST; i++) {
		m_apCmds[i] = NULL;
	}
	//memset(m_apCmds, 0, sizeof(m_apCmds));
	m_uiTotal = 0;
	m_uiCurFocusID = 0;
	//m_uiPreFocusID = 0;
	m_uiNumPreFocus = 0;
	memset(m_uiPreFocusID, 0, sizeof(m_uiPreFocusID));
	m_pMainCmd	= NULL;
}

/**
* <b>FUNCTION: </b>
*
* ~CZenoCommandList()
*
* <b>DESCRIPTION: </b>
* 
* Destructor
* 
* @param	No
*		
* @return	No
*
*/
CZenoCommandList::~CZenoCommandList()
{
	RMuint32 i;
	DPRINTF("+++ total %ld \n", m_uiTotal );
	//release command list.
	for (i = 0; i < ZENO_MAX_COMMAND_LIST; i++) {
		if (m_apCmds[i]) {
			delete (m_apCmds[i]);
			m_apCmds[i] = NULL;
		}
	}
	m_uiTotal = 0;
	m_uiCurFocusID = 0;
	//m_uiPreFocusID = 0;
	memset(m_uiPreFocusID, 0, sizeof(m_uiPreFocusID));
	m_uiNumPreFocus = 0;
	if (m_pMainCmd) {
		delete (m_pMainCmd);
		m_pMainCmd = NULL;
	}
}

#define CMD_PRINTF_INFO() \
	{ \
		printf("-------------------------------------- \n"); \
		if (m_pMainCmd) { \
			printf("[%s] ", m_pMainCmd->GetName()); \
		} \
		for (RMuint32 i = 0; i < ZENO_MAX_COMMAND_LIST; i ++) \
		{ \
			if (m_apCmds[i] != NULL) { \
				if (i == m_uiCurFocusID) { \
					printf("*"); \
				} \
				printf("(%lu:%s) ",i, m_apCmds[i]->GetName()); \
			} \
		} \
		printf("\n"); \
		printf("-------------------------------------- \n"); \
	}

/**
* <b>FUNCTION: </b>
*
* Add(ICommand *pCmd)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   pFunPtrEx pF, a function pointer
*
* @return
*   -1 if error, Otherwise return handle id of function pointer
*    in list.
*
*/
RMint32 CZenoCommandList::Add(ICommand* pCmd)
{
	RMuint32 i = 0;
	RMint32 ret = -1;
	DPRINTF("+++ total %ld \n", m_uiTotal );
	if(m_uiTotal >= ZENO_MAX_COMMAND_LIST)
	{
		DPRINTF(" List command is full ---> total %ld \n", m_uiTotal );
		return ret;
	}
	if (pCmd) 
	{
		for (i = 0; i < ZENO_MAX_COMMAND_LIST; i ++) 
		{
			if (m_apCmds[i] == NULL)
			{
				m_apCmds[i] = pCmd;
				m_uiTotal ++;
				ret = i;
				break;
			}
		}
	}
	CMD_PRINTF_INFO();
	if (ret >= 0) {
		DPRINTF("Add successful i: %lu  \n", ret);
	}
	else {
		DPRINTF("Add failure \n");
	}

	return ret;
}

/**
* <b>FUNCTION: </b>
*
* Remove(RMuint32 uiCmdID)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   RMuint32 uiCmdID
*		
* @return
*   RM_OK if sucess, otherwise return RM_ERROR
*
*/
RMint32 CZenoCommandList::Remove(RMuint32 uiCmdID)
{
	DPRINTF("+++ total %ld, CmdID: %ld \n", m_uiTotal, uiCmdID);
	if ((uiCmdID >= ZENO_MAX_COMMAND_LIST) || !m_apCmds[uiCmdID]) {
		DPRINTF(" Invalid arguments  \n");
		return RM_ERROR;
	}
	//release 
	if (m_apCmds[uiCmdID]) {
		delete (m_apCmds[uiCmdID]);
		m_apCmds[uiCmdID] = NULL;
		m_uiTotal --;
	}
	if (m_uiCurFocusID == uiCmdID) {
		//m_uiCurFocusID = 0;
		while (m_apCmds[m_uiCurFocusID] == NULL && m_uiNumPreFocus > 0)
		{
			m_uiCurFocusID = m_uiPreFocusID[m_uiNumPreFocus - 1];
			m_uiPreFocusID[m_uiNumPreFocus - 1] = 0;
			m_uiNumPreFocus --;
		}
		if (m_apCmds[m_uiCurFocusID] == NULL){
			m_uiCurFocusID = 0;
		}
	}
	CMD_PRINTF_INFO();
	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* Invoke(RMuint32 uiKey, RMuint32 uiIdle, RMuint32 &uiOut)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   RMuint32 uiKey, RMuint32 uiIdle, RMuint32 &uiOut
*		
* @return	No
*
*/
void CZenoCommandList::Invoke(RMuint32 uiKey, RMuint32 uiIdle, RMuint32 &uiOut)
{
//	DPRINTF("+++ TotalCmd %ld, Key: 0x%lX, Idle: 0x%lX, m_uiCurFocusID: %ld  \n", m_uiTotal, uiKey, uiIdle,m_uiCurFocusID);
	RMuint32 i;

	if (m_apCmds[m_uiCurFocusID]) {
		if (uiKey != 0) {
			DPRINTF("call Focus Cmd ID: %ld, uiKey: 0x%lX, idle: 0x%lX\n", m_uiCurFocusID, uiKey, uiIdle);
			CMD_PRINTF_INFO();
		}
        m_apCmds[m_uiCurFocusID]->Execute(uiKey, uiOut);
	}
	for (i = 0; i < ZENO_MAX_COMMAND_LIST; i ++) {
		if (m_apCmds[i]) {
			if (i != m_uiCurFocusID) {
//				ZERROR1("call i: %ld, uiKey: %ld \n", i, uiIdle);
				m_apCmds[i]->Execute(uiIdle, uiOut);
			}
		}
	}
}

/**
* <b>FUNCTION: </b>
*
* Invoke(RMuint32 uiCmdID, RMuint32 uiKey, RMuint32 uiIdle,
* RMuint32 &uiOut)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   RMuint32 uiCmdID, RMuint32 uiKey, RMuint32 uiIdle, RMuint32
*   &uiOut
*		
* @return	No
*
*/
void CZenoCommandList::Invoke(RMuint32 uiCmdID, RMuint32 uiKey, RMuint32 uiIdle, RMuint32 &uiOut)
{
//	DPRINTF("+++ TotalCmd %ld, CmdID: %ld, Key: 0x%lX, Idle: 0x%lX, m_uiCurFocusID: %ld  \n", m_uiTotal, uiCmdID, uiKey, uiIdle,m_uiCurFocusID);
	if ((uiCmdID >= ZENO_MAX_COMMAND_LIST) || !m_apCmds[uiCmdID]) {
		DPRINTF(" Invalid arguments  \n");
		return;
	}
	RMuint32 uiTemp = m_uiCurFocusID; //in case change forcus while executing command.
	{
		if (m_apCmds[uiCmdID]) {
			if (uiKey != uiIdle) {
				DPRINTF("uiCmdID: %ld, m_uiCurFocusID: %ld \n", uiCmdID, m_uiCurFocusID);
			}			
			if (uiCmdID == uiTemp) {
				m_apCmds[uiCmdID]->Execute(uiKey, uiOut);
			}
			else {
				m_apCmds[uiCmdID]->Execute(uiIdle, uiOut);
			}
		}
	}
}

/**
* <b>FUNCTION: </b>
*
* Focus(RMuint32 iCmdID)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   RMuint32 iCmdID
*		
* @return
*   RM_OK if success, otherwise return RM_ERROR
*
*/
RMint32 CZenoCommandList::Focus(RMuint32 iCmdID)
{
	DPRINTF("+++ total %ld, CmdID: %ld \n", m_uiTotal, iCmdID);
	if ((iCmdID >= ZENO_MAX_COMMAND_LIST) || !m_apCmds[iCmdID]) {
		DPRINTF("error, cmd is not exist \n");
		return RM_ERROR;
	}
	//add
//	m_uiPreFocusID = m_uiCurFocusID;
	m_uiPreFocusID[m_uiNumPreFocus] = m_uiCurFocusID;
	m_uiNumPreFocus ++;

	m_uiCurFocusID = iCmdID;
	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* AddMain(ICommand *pCmd)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   ICommand *pCmd
*		
* @return
*   ZENO_MAX_COMMAND_LIST if success, otherwise return -1
*
*/
RMint32 CZenoCommandList::AddMain(ICommand* pCmd)
{
	DPRINTF("+++ total %ld \n", m_uiTotal );
	if (pCmd) 
	{
		if (!m_pMainCmd)
		{
			m_pMainCmd = pCmd;
			DPRINTF("Add successful id: %d  \n", ZENO_MAX_COMMAND_LIST);
			return ZENO_MAX_COMMAND_LIST;
		}
	}
	DPRINTF("Add failure \n");
	return -1;
}

/**
* <b>FUNCTION: </b>
*
* ReMoveMain(RMuint32 uiCmdID)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   RMuint32 uiCmdID
*		
* @return
*   RM_OK if success, otherwise return RM_ERROR
*
*/
RMint32 CZenoCommandList::RemoveMain(RMuint32 uiCmdID)
{
	DPRINTF("+++ total %ld \n", m_uiTotal );
	if (uiCmdID != ZENO_MAX_COMMAND_LIST || !m_pMainCmd) {
		DPRINTF(" Invalid arguments  \n");
		return RM_ERROR;
	}
	//release 
	delete (m_pMainCmd);
	m_pMainCmd = NULL;

	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* InvokeMain(RMuint32 uiCmdID, RMuint32 uiKey, RMuint32 uiIdle, RMuint32 &uiOut)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   RMuint32 uiCmdID, RMuint32 uiKey, RMuint32 uiIdle, RMuint32 &uiOut
*		
* @return   No
*
*/
void CZenoCommandList::InvokeMain(RMuint32 uiCmdID, RMuint32 uiKey, RMuint32 uiIdle, RMuint32 &uiOut)
{
	if (uiKey != 0) {
		DPRINTF("+++ TotalCmd %ld, CmdID: %ld, Key: 0x%lX, Idle: 0x%lX \n", m_uiTotal, uiCmdID, uiKey, uiIdle);
		CMD_PRINTF_INFO();
	}
	if (uiCmdID != ZENO_MAX_COMMAND_LIST || !m_pMainCmd) {
		//console_error(" Invalid arguments  \n");
		return;
	}
	m_pMainCmd->Execute(uiKey, uiOut);
}
/**
* <b>FUNCTION: </b>
*
* GetCurrentFocus()
*
* <b>DESCRIPTION: </b>
* Get Current Focus
* @param
*   No
*		
* @return   No
*
*/
RMint32 CZenoCommandList::GetCurrentFocus()
{
	return m_uiCurFocusID;
}
// ------------------------------------------ END ---------------------------------------

