/******************************************************************************
 File name: 	zenoapp.cpp
 Copyright (c) 2007 KENVI, Corp. All Rights Reserved
 Proprietary and Confidential
 Purpose:		
 Note:			
 Version:
 Date:	Jan 12, 2010
******************************************************************************/
/**
$Log: zenoapp.cpp,v $
Revision 1.1.1.1  2013/02/20 09:35:12  liempham
Imported sources

Revision 1.2  2013/01/24 01:02:24  hdo
BugId: 38635
Description:
 - Import dtv apis into sigmaobjectextension
SEND_COMMENT_BUGZILLA

Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
add new

Revision 1.2  2011/04/28 04:45:03  liempham

BugId: 31699 update sc_sdk module - support customer can build their extension together
SEND_COMMENT_BUGZILLA

Revision 1.22  2011/01/21 06:56:04  haitran
add standby

Revision 1.21  2011/01/05 09:09:30  minhdo
fix bug : DLNA

Revision 1.20  2010/12/24 06:20:02  nguyennguyen
Return Value To Play Disc flow by

Revision 1.19  2010/12/22 09:08:20  hiennguyen
Create DTV.fla

Revision 1.18  2010/12/20 06:56:29  nguyennguyen
IdeKey_ShowBarAudio

Revision 1.17  2010/11/24 03:30:41  trihuynh
BugId :None
	remove error message.

Revision 1.16  2010/11/24 02:35:15  trihuynh
BugId : None
	miss "/ *".

Revision 1.15  2010/11/24 02:07:20  trihuynh
BugId : None
	Memory leak after go out usb stick many times;

Revision 1.14  2010/11/22 06:38:12  cuongnguyen
Add Slideshow.

Revision 1.13  2010/10/19 06:42:24  trihuynh
BugId : None
	Fix : Control second Video is exactly.

Revision 1.12  2010/10/18 07:54:19  trihuynh
BugID : None
	Move fd_set	 set into "while" loop.

Revision 1.11  2010/10/18 03:32:10  trihuynh
BugId : 30390
	Fix : playback freezes when do angle change in title 7.

Revision 1.10  2010/10/05 07:02:24  trihuynh
BugId:None
	Implement Handle VCD by c++ lib.

Revision 1.9  2010/09/24 06:32:14  trihuynh
BugId : None
	Add setting menu,audio,subtitle before playing bdj disc.

Revision 1.8  2010/09/23 01:11:26  trihuynh
BugID : None
	Control switching between internal usb and external usb while playing bdj disc.

Revision 1.7  2010/08/09 03:09:28  trihuynh
BugId : None
	Port BDFramework from 3.7.2 --> 3.9.0.

Revision 1.6  2010/08/04 02:57:13  trihuynh
BugId : None
	Add NetFlix library.

Revision 1.5  2010/07/22 10:06:43  trihuynh
BugId : None
	fix can not updateMedia event.

Revision 1.4  2010/07/22 02:36:36  trihuynh
BugID : None
	Add detection disc function.

Revision 1.3  2010/07/20 02:35:14  trihuynh
Big Id :
	+ Add some keys to get status of media during playing
	+ Getting exactly information during playing.

Revision 1.2  2010/07/19 08:59:21  trihuynh
BugId :
	Add SLowFWD, Pause, Sep, Angle, Audio, Subttile function for DVD.

Revision 1.1.1.1  2010/07/19 02:47:56  cvsadmin
add new

Revision 1.1.1.1  2010/05/06 01:52:41  cvsadmin
The update zeno Z5D1-SD-0706-700-2.0X.01

Revision 1.1.1.1  2010/04/07 02:52:56  cvsadmin
The import new Z5D1-SD-0706-700-2.0X.00

Revision 1.5  2010/03/10 04:27:55  chautran
Repeate key

Revision 1.4  2010/02/06 11:27:53  chautran
Add pictures adjust

Revision 1.3  2010/02/06 06:21:40  chautran
Add mute key

Revision 1.2  2010/02/05 08:27:01  chautran
Edit volume, sys process data between C & Flash

Revision 1.1.1.1  2010/01/21 09:32:43  cvsadmin
Then import new Z5D1-SD-0705-700-2.0X.00

Revision 1.1.1.1  2010/01/21 06:02:35  cvsadmin
The import new ZenoFramework

Revision 1.2  2010/01/19 07:24:28  khanhngo
BugID: none

 */
//#include "flashlite/include/includestagecraft.h"
#include "zeno.h"
//#include <mcheck.h>
extern int						keyfd ; 
extern int                      ampfd;
extern int 						ampIndex; 
extern bool					    gui_on_secosd;
extern int						gui_ref_count;
extern IAdvancedMediaProvider   *amp;
extern fd_set					set;


char                            MEDIA_PATH[MAX_LENGTH_STRING];
DFBEvent						*g_pEvt;
RMbool							isProcessHotPlugEvent = FALSE;
/**
* 
* <b>FUNCTION: </b>
*
* Run(int argc, char *argv[]
*
* <b>DESCRIPTION: </b>
* 
* Loop here to get key event until key event is POWER_OFF
* 
* @param
*       int argc, char *argv[]
* @return	0
*
*/

RMint32 CZenoApp::Run(int argc, char *argv[])
{
#if 1
    //fd_set	 set;
	//mtrace(); /* This starts memory tracing. 
	/*This has to be done before we do a 'malloc' or we allocate memory.  */

	/*
	nRun = 1;
	MyControlAppIndex::SetAppIndex(BLURAY_APP);
	OnInit();
restartapp:	
	InitializeStagecraftLib(MyControlAppIndex::GetAppIndex());

	while(!MyControlAppIndex::isExitApp())
	{	
		DFBEvent evt;
		g_pEvt = & evt;
		fd_set	 set;
        FD_ZERO(&set);
        FD_SET(keyfd, &set);
		if (hotplug_fd != -1)
		{
			FD_SET (hotplug_fd, &set);
		}
			
		if (amp)
			FD_SET (ampfd, &set);

		//struct timeval timeout = {0, 1};
		struct timeval timeout = {1, 0};
		
        select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		
        if(FD_ISSET(keyfd, &set))
        {
            read(keyfd, &evt, sizeof(evt));
			if((int)evt.input.key_symbol == ZENO_EVENT_KEY_POWER){
                if(evt.clazz == DFEC_INPUT && evt.input.type == DIET_KEYPRESS)
                {
                    if(MyControlAppIndex::GetAppIndex() == INTERNET_APP) {
                        MyControlAppIndex::SetAppIndex(BLURAY_APP);
                        MyControlAppIndex::SetExitAppParameter(TRUE);
                    }
                    
                    OnKeyDown(evt.input.key_symbol);
                }   
            }
            else
                OnKeyDown(evt.input.key_symbol);            			
		}
		else
		{
			OnKeyDown(ZENO_EVENT_IDLE);
		}
		
		if(amp && ampfd >= 0 && FD_ISSET(ampfd, &set))
		{
			// AMP event, purge them all and then process the aggregated status
			fd_set set;
			struct timeval timeout = {0, 0};
			
			do
			{
				read(ampfd, &evt, sizeof(evt));
				
				FD_ZERO (&set);
				FD_SET (ampfd, &set);
				
				// select returns 0 if timeout, 1 if input available, -1 if error.
				select (FD_SETSIZE, &set, NULL, NULL, &timeout);
			}
			while(FD_ISSET(ampfd, &set));	
			process_media_events(ampIndex);
		}
		if ((hotplug_fd != -1) && FD_ISSET(hotplug_fd, &set))
		{
			isProcessHotPlugEvent = TRUE;
		}
#if 1
		if(GetStageWindowStatus() == kStatusComplete)
		{
			if(MyControlAppIndex::GetAppIndex() == NETFLIX_APP)
			{
				MyControlAppIndex::SetAppIndex(INTERNET_APP);
				MyControlAppIndex::SetExitAppParameter(TRUE);
			}
		}
#endif
	}
	
	// Process App
	UninitializeStagecraftLib();
	int appNumber = MyControlAppIndex::GetAppIndex();
	if(appNumber != TERMINAL_APP)
	{
		SwitchBetweenOsdLayerAndSecondOsdLayer(appNumber);
		MyControlAppIndex::SetExitAppParameter(FALSE);
		goto restartapp;
	}
	OnTerminate();
	DPRINTF("Quit running...\n");
	*/
#endif
	return 0;
}
/**
* <b>FUNCTION: </b>
*
* Terminate(RMuint32 nExitCode)
*
* <b>DESCRIPTION: </b>
* 
* Exit the application
* 
* @param	No
*		
* @return	No
*
*/
void CZenoApp::Terminate(RMint32 nExitCode)
{
	//MyControlAppIndex::SetAppIndex(TERMINAL_APP);
	//MyControlAppIndex::SetExitAppParameter(true);
	nRun = nExitCode;
}
/**
* <b>FUNCTION: </b>
*
* Terminate(RMuint32 nExitCode)
*
* <b>DESCRIPTION: </b>
* 
* Exit the application
* 
* @param	No
*		
* @return	No
*
*/
void CZenoApp::SwitchBetweenOsdLayerAndSecondOsdLayer(int appIndex)
{
	/*switch(appIndex) {
	case INTERNET_APP:
	case YOUTUBE_APP:
	case NETFLIX_APP:
	case SHOUTCAST_APP:
	case DTV_APP:
	case DLNA_APP:
		
		//	release second osd layer & start primary osd player
		
		if(!gui_on_secosd)
			return;
		GUI_INFO("=================> Release Second Osd <=====================\n");
		disable_gui();
		
		//	don't use second osd 
		
		gui_on_secosd = false ;
		
		//	create surface on osd layer
		
		enable_gui();
		gui_ref_count = 1 ;
		break;
	case BLURAY_APP:
		if(gui_on_secosd)
		{
			return;
		}
		
		//	release osd layer & start second osd player
		
		
		//	release surface on osd layer  
		
		disable_gui();
		
		//	use second osd layer to show gui
		
		gui_on_secosd = true;
		GUI_INFO("=================> init Second Osd  <=====================\n");
		enable_gui();
		gui_ref_count = 1; 
		break;
	}
	*/
}
/**
* <b>FUNCTION: </b>
*
* CZenoApp()
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
CZenoApp::CZenoApp()
{
	nRun = 0;
}

/**
* <b>FUNCTION: </b>
*
* ~CZenoApp()
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
CZenoApp::~CZenoApp()
{
}
