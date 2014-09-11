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
* @file   zenocommand.h
* @brief  Zeno command header
*/

/**
$Log: zenocommand.h,v $
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

#ifndef __ZENOCOMMAND_H__
#define __ZENOCOMMAND_H__

#include <stdio.h>
#include <string.h>
#include "rmdef/rmdef.h"

#define T_COMMAND_NAME_LEN 64
#define ZENO_MAX_COMMAND_LIST 16

class ICommand
{
public:
	virtual ~ICommand() {};
	virtual void Execute(RMuint32 uiIn, RMuint32 &uiOut) = 0;
	virtual char* GetName() {return (char*)"IC:Unknown";};
protected:
	ICommand() {};
};

template <class TReceiver> class CSimpleCommand: public ICommand
{
public:
	typedef void (TReceiver::*Action)(RMuint32, RMuint32&);
	
	CSimpleCommand(TReceiver* r, Action a, char* pName)
	{
		m_Receiver = r; m_Action = a; 
		memset(m_acName, 0, sizeof(m_acName));
		if(pName) 
		{
			if(strlen(pName) >= T_COMMAND_NAME_LEN)
			{
				strncpy(m_acName, pName, T_COMMAND_NAME_LEN - 1);
			}
			else
			{
				strncpy(m_acName, pName, strlen(pName));
			}
		}
		else
		{
			strncpy(m_acName, "CS:Unknown", T_COMMAND_NAME_LEN - 1);
		}
	};

	virtual ~CSimpleCommand() {};
	virtual void Execute(RMuint32 uiIn, RMuint32 &uiOut) { (m_Receiver->*m_Action)(uiIn, uiOut); };
	virtual char* GetName() { return m_acName; };
private:
	char m_acName[T_COMMAND_NAME_LEN];
	Action m_Action;
	TReceiver* m_Receiver;
};

class CZenoCommandList
{
public:
	CZenoCommandList();
	virtual ~CZenoCommandList();

	RMint32 Add(ICommand*);
	RMint32 Remove(RMuint32);
	void 	Invoke(RMuint32, RMuint32, RMuint32&);
	void 	Invoke(RMuint32, RMuint32, RMuint32, RMuint32&);
	RMint32 Focus(RMuint32);

	RMint32 AddMain(ICommand*);
	RMint32 RemoveMain(RMuint32);
	void 	InvokeMain(RMuint32, RMuint32, RMuint32, RMuint32&);
	RMint32 GetCurrentFocus();

private:
	RMuint32 	m_uiPreFocusID[ZENO_MAX_COMMAND_LIST];
	RMuint32	m_uiNumPreFocus;
	RMuint32 	m_uiCurFocusID;
	RMuint32 	m_uiTotal;
	ICommand* 	m_apCmds[ZENO_MAX_COMMAND_LIST];
	ICommand* 	m_pMainCmd;
};

#endif //__ZENOCOMMAND_H__

