	
#ifndef AMPTEST_DEFINE_H
#define AMPTEST_DEFINE_H




//enum { BAR_VOLUME = 0 , BAR_CONTRAST , BAR_BRIGHTNESS , BAR_HUE , BAR_SATURATION };

#define BASIC_COMMANDS	\
	{(int) Cmd_Adjust, (int *) &adjustment.value.volume, (int *) &adjustment.type, NULL, DIET_KEYPRESS, DIKS_VOLUME_UP, "System volume", 29, "Enter volume (0-32, neutral = 29), then press ENTER", ADJ_VOLUME, NULL, 0, NULL}, \
    {(int) Cmd_Adjust, (int *) &adjustment.value.mute, (int *) &adjustment.type, NULL, DIET_KEYPRESS, DIKS_MUTE, "System mute", 0, "Enter 1 for Mute ON, 0 for Mute OFF, then press ENTER", ADJ_MUTE, NULL, 0, NULL}, \
    {(int) Cmd_SetConfig, (int *) &newAudioConfigIndex, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Audio cfg", 0, "Enter audio configuration index, then press ENTER", 0, NULL, 0, NULL}, \
    {(int) Cmd_SetConfig, (int *) &newVideoConfigIndex, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Video cfg", 0, "Enter video configuration index, then press ENTER", 0, NULL, 0, NULL}, \
    {(int) Cmd_SetConfig, (int *) &newDRMConfigIndex, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "DRM cfg", 0, "Enter video configuration index, then press ENTER", 0, NULL, 0, NULL}, \
    {(int) Cmd_SetCallbacks, (int *) &ampCommand.generic.control.callback.configPreview, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Cfg preview", 1, "Enter 0 to disable, 1 to enable Config Preview, then press ENTER", 0, NULL, 0, NULL}

struct SMediaSpaceCmdDefinition lpbCmdList[] =
{
	BASIC_COMMANDS,

	{(int) Cmd_Adjust, (int *) &adjustment.value.volume, (int *) &adjustment.type, NULL, DIET_KEYPRESS, DIKS_VOLUME_UP, "Volume (PCMX only)", 29, "Enter Volume (0-32, neutral = 29), then press ENTER", ADJ_MIXER_VOLUME_MAIN, NULL, 0, NULL},
	{(int) Cmd_Adjust, (int *) &adjustment.value.pan.x, (int *) &adjustment.value.pan.y, (struct SMediaTime *) &adjustment.type, DIET_KEYPRESS, DIKS_VOLUME_UP, "Pan (PCMX only)", 15, "Enter X position (0-30, center = 15), then press ENTER", 15, "Enter Y position (0-30, center = 15), then press ENTER", ADJ_MIXER_PAN_MAIN, NULL},
	{(int) Cmd_WakeUpOnPTS, (int *) &ampCommand.generic.control.notification.pts, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "Notify PTS", 0, NULL, 0, NULL, 0, NULL},

	{(int) LPBCmd_PLAY, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Play", 0, NULL, 0, NULL, 0, NULL},
	{(int) LPBCmd_PAUSE_ON, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause", 0, NULL, 0, NULL, 0, NULL},
	{(int) LPBCmd_PAUSE_OFF, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Resume", 0, NULL, 0, NULL, 0, NULL},
	{(int) LPBCmd_REBUFFER, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Rebuffer", 0, NULL, 0, NULL, 0, NULL},
	{(int) LPBCmd_STOP, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, NULL, 0, NULL, 0, NULL},
	{(int) LPBCmd_PLAY_TIME, (int *) &ampCommand.lpb.param1.seekMode, NULL, (struct SMediaTime *)&ampCommand.lpb.param2.time, DIET_KEYPRESS, DIKS_LIST, "Seek to time", 0, NULL, 0, NULL, 0, NULL},
	{(int) LPBCmd_SCAN_FORWARD, (int *) &ampCommand.lpb.param2.speed, NULL, NULL, DIET_KEYPRESS, DIKS_FASTFORWARD, "Fast Forward (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_FAST_FORWARD, (int *) &ampCommand.lpb.param2.speed, NULL, NULL, DIET_KEYPRESS, DIKS_SLOW, "Slow Forward", 512, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_SCAN_BACKWARD, (int *) &ampCommand.lpb.param2.speed, NULL, NULL, DIET_KEYPRESS, DIKS_REWIND, "Rewind (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_STEP, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_SLOW, "Step forward", 0, NULL, 0, NULL, 0, NULL},
	{(int) LPBCmd_SELECT_VIDEO_STREAM, (int *) &ampCommand.lpb.param1.streamIndex, NULL, NULL, DIET_KEYPRESS, DIKS_TITLE, "Select Video", 0, "Enter Stream Index, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_SELECT_AUDIO_STREAM, (int *) &ampCommand.lpb.param1.streamIndex, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Select Audio", 0, "Enter Stream Index, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_GET_STREAM_INFO, (int *) &ampCommand.lpb.param1.streamIndex, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Get Stream Info", 0, "Enter Stream Index, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_GET_STREAM_SET_INFO, (int *) &ampCommand.lpb.param1.streamSetIndex, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Get Stream Set Info", 0, "Enter Stream Set Index, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_SELECT_STREAM_SET, (int *) &ampCommand.lpb.param1.streamSetIndex, NULL, NULL, DIET_KEYPRESS, DIKS_SELECT, "Select Stream Set", 0, "Enter Stream Set Index, then press ENTER", 0, NULL, 0, NULL},
	{(int) LPBCmd_SELECT_SUBTITLE_STREAM, (int *) &ampCommand.lpb.param1.streamIndex, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "Select Subtitle", 0, "Enter Stream Index, then press ENTER", 0, NULL, 0, NULL},
};

#ifndef NO_RED_SUPPORT
struct SMediaSpaceCmdDefinition cddaCmdList[] =
{
	BASIC_COMMANDS,
    {(int) Cmd_WakeUpOnPTS, (int *) &ampCommand.generic.control.notification.pts, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "Notify PTS", 0, NULL, 0, NULL, 0, NULL}, \
	
	{(int) CDDACmd_Play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Play", 0, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Pause, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause", 0, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Resume", 0, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Stop, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Seek, (int *) &ampCommand.cdda.param1.track, NULL, (struct SMediaTime *)&ampCommand.cdda.param2.time, DIET_KEYPRESS, DIKS_LIST, "Seek to time", 0, "Enter Track number, then press ENTER", 0, NULL, 0, NULL},
	{(int) CDDACmd_NextTrack, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Track", 0, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_PrevTrack, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Previous Track", 0, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_FastForward, (int *) &ampCommand.cdda.param1.speed, NULL, NULL, DIET_KEYPRESS, DIKS_FASTFORWARD, "Fast forward", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) CDDACmd_Rewind, (int *) &ampCommand.cdda.param1.speed, NULL, NULL, DIET_KEYPRESS, DIKS_REWIND, "Fast rewind", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) CDDACmd_Repeat, (int *) &ampCommand.cdda.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Track", CDDA_REPEAT_TRACK, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Repeat, (int *) &ampCommand.cdda.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Disk", CDDA_REPEAT_DISC, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Repeat, (int *) &ampCommand.cdda.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Random", CDDA_REPEAT_RANDOM, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Repeat, (int *) &ampCommand.cdda.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat OFF", CDDA_REPEAT_NONE, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Repeat, (int *) &ampCommand.cdda.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_AB, "Repeat AB", CDDA_REPEAT_AB, NULL, 0, NULL, 0, NULL},
	{(int) CDDACmd_Repeat, (int *) &ampCommand.cdda.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat AB", CDDA_REPEAT_AB, NULL, 0, NULL, 0, NULL}, // for remotes without the A-B key
	
};


struct SMediaSpaceCmdDefinition dvdCmdList[] =
{
	BASIC_COMMANDS,
    {(int) Cmd_WakeUpOnPTS, (int *) &ampCommand.generic.control.notification.pts, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "Notify PTS", 0, NULL, 0, NULL, 0, NULL}, \
	
	{(int) DVDCmd_Play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Start/Resume", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Still_Off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Still OFF", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Pause_On, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause ON", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Pause_Off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Pause OFF", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Stop, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Forward_Scan, (int *) &ampCommand.dvd.param1.speed, (int *) &ampCommand.dvd.param2.selector, NULL, DIET_KEYPRESS, DIKS_FASTFORWARD, "Fast Forward (I-Frames Only)", 10240, "Enter forward speed in nominal speed/1024 units, then press ENTER", PLAY_MODE_KEY, NULL, 0, NULL},
	{(int) DVDCmd_Forward_Scan, (int *) &ampCommand.dvd.param1.speed, (int *) &ampCommand.dvd.param2.selector, NULL, DIET_KEYPRESS, DIKS_SLOW, "Slow Forward", 512, "Enter forward speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Backward_Scan, (int *) &ampCommand.dvd.param1.speed, (int *) &ampCommand.dvd.param2.selector, NULL, DIET_KEYPRESS, DIKS_REWIND, "Rewind (I-Frames Only)", 10240, "Enter reverse speed in nominal speed/1024 units, then press ENTER", PLAY_MODE_KEY, NULL, 0, NULL},
	{(int) DVDCmd_Upper_Button_Select, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_UP, "Button UP", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Lower_Button_Select, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_DOWN, "Button DOWN", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Left_Button_Select, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_LEFT, "Button LEFT", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Right_Button_Select, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_RIGHT, "Button RIGHT", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Button_Activate, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_ENTER, "Button ACTIVATE", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_0, "Numeric Button SELECT AND ACTIVATE", 0, "Enter button number, then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_1, "Button 1 SELECT AND ACTIVATE", 1, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_2, "Button 2 SELECT AND ACTIVATE", 2, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_3, "Button 3 SELECT AND ACTIVATE", 3, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_4, "Button 4 SELECT AND ACTIVATE", 4, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_5, "Button 5 SELECT AND ACTIVATE", 5, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_6, "Button 6 SELECT AND ACTIVATE", 6, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "Button 7 SELECT AND ACTIVATE", 7, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_8, "Button 8 SELECT AND ACTIVATE", 8, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Numeric_Button_Select_And_Activate, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "Button 9 SELECT AND ACTIVATE", 9, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Menu_Call, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Title ", RM_DVD_MENU_ID_TITLE, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Menu_Call, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Root ", RM_DVD_MENU_ID_ROOT, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Menu_Call, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Subtitle ", RM_DVD_MENU_ID_SUBPICTURE, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Menu_Call, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Language ", RM_DVD_MENU_ID_AUDIO, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Menu_Call, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Angle ", RM_DVD_MENU_ID_ANGLE, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Menu_Call, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Chapter ", RM_DVD_MENU_ID_PTT, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Resume, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Resume", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Query_Bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Save", 0, NULL, 0, NULL, 0, NULL},	// uses DVDCmd_Query_BookmarkSize and DVDCmd_Query_Bookmark to set a bookmark for the current position
	{(int) DVDCmd_Resume_Bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Restore", 0, NULL, 0, NULL, 0, NULL}, // resumes playback from the last bookmark set
	{(int) DVDCmd_GoUp, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Go Up", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Title_Play, (int *) &ampCommand.dvd.param1.selector, (int *) &ampCommand.dvd.param3.endAction, NULL, DIET_KEYPRESS, DIKS_TITLE, "Title play", 0, "Enter a Title number, then press ENTER", 0, "Enter end action (0=default, 1=Stop), then press ENTER", 0, NULL},
	{(int) DVDCmd_PTT_Play, (int *) &ampCommand.dvd.param1.selector, (int *) &ampCommand.dvd.param2.selector, (struct SMediaTime *) &ampCommand.dvd.param3.endAction, DIET_KEYPRESS, DIKS_TITLE, "PTT play", 0, "Enter a Title number, then press ENTER", 0, "Enter a PTT number, then press ENTER", 0, "Enter end action (0=default, 1=Stop), then press ENTER"},
	{(int) DVDCmd_Time_Play, (int *) &ampCommand.dvd.param1.selector, NULL, (struct SMediaTime *) &ampCommand.dvd.param2.time, DIET_KEYPRESS, DIKS_LIST, "Time play", 0, "Enter a Title number then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Time_Search, NULL, NULL, (struct SMediaTime *)&ampCommand.dvd.param2.time, DIET_KEYPRESS, DIKS_LIST, "Time search", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_PTT_Search, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "PTT search", 0, "Enter a PTT number, then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_NextPG_Search, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next PG", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_PrevPG_Search, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Prev PG", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_TopPG_Search, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Crt PG", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Repeat, (int *) &ampCommand.dvd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_AB, "Repeat AB", DVD_REPEAT_AB, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Repeat, (int *) &ampCommand.dvd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat AB", DVD_REPEAT_AB, NULL, 0, NULL, 0, NULL}, // for remotes without the A-B key
	{(int) DVDCmd_Repeat, (int *) &ampCommand.dvd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Title", DVD_REPEAT_TITLE, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Repeat, (int *) &ampCommand.dvd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat PTT", DVD_REPEAT_CHAPTER, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Repeat, (int *) &ampCommand.dvd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat OFF", DVD_REPEAT_NONE, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Step, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Step", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Backward_Step, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Backward Step", 0, NULL, 0, NULL, 0, NULL},
	{(int) DVDCmd_Audio_Stream_Change, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Audio select", 0, "Enter a stream number (0 to 7), then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Subpicture_Stream_Change, (int *) &ampCommand.dvd.param1.selector, (int *) &ampCommand.dvd.param2.spuDisplay, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "Subtitle select", 0, "Enter a stream number (0 to 31), then press ENTER", 0, "Enter display status (0/1), then press ENTER", 0, NULL},
	{(int) DVDCmd_Angle_Change, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_ANGLE, "Angle select", 0, "Enter an angle number (1 to 9), then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Menu_Language_Select, (int *) &ampCommand.dvd.param1.language, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Menu language select", 0, "Enter a language code, then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Parental_Level_Select, (int *) &ampCommand.dvd.param1.level, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Parental level select", 0, "Enter a parental level, then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Parental_Country_Select, (int *) &ampCommand.dvd.param1.country, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Country select", 0, "Enter a country code, then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Video_Mode_Change, (int *) &ampCommand.dvd.param1.videoMode, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Video mode select", 1, "Enter a video mode (1: direct, 2: crop, 3: bands, 4: wide, 5: pan scan, 6: letter box), then press ENTER", 0, NULL, 0, NULL},
	{(int) DVDCmd_Config_Audio_Language, (int *) &ampCommand.dvd.param1.iniLCD.langCode, (int *) &ampCommand.dvd.param1.iniLCD.langCodeExt, NULL, DIET_KEYPRESS, DIKS_SETUP, "Initial audio language", 0, "Enter language code, then press ENTER", 0, "Enter extension code, then press ENTER", 0, NULL},
	{(int) DVDCmd_Config_Subpicture_Language, (int *) &ampCommand.dvd.param1.iniLCD.langCode, (int *) &ampCommand.dvd.param1.iniLCD.langCodeExt, NULL, DIET_KEYPRESS, DIKS_SETUP, "Initial spu language", 0, "Enter language code, then press ENTER", 0, "Enter extension code, then press ENTER", 0, NULL},
	//    {(int) DVDCmd_Config_Region_Code, (int *) &ampCommand.dvd.param1.region, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Region setup", 0, "Enter a region code, then press ENTER", NULL, NULL},
	{(int) DVDCmd_QueryTitle, (int *) &ampCommand.dvd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Query Title", 0, "Enter a Title number, then press ENTER", 0, NULL, 0, NULL},	// retrieves info on current title
	{(int) DVDCmd_QueryTXTDT, (int *) &ampCommand.dvd.param2.selector, (int *) &ampCommand.dvd.param1.language, NULL, DIET_KEYPRESS, DIKS_INFO, "Query Title Info", 0, "Enter a Title number, then press ENTER", 0, "Enter a language code, then press ENTER", 0, NULL}, // retrieves the TXTDT info for title number number (1-99) specified in SDVDCommand::param1::selector in language SDVDCommand::param2::selector
	{(int) DVDCmd_QuerySPRMs, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Query SPRs", 0, NULL, 0, NULL, 0, NULL},	// retrieves the SPR registers
	{(int) DVDCmd_QueryGPRMs, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Query GPRs", 0, NULL, 0, NULL, 0, NULL},	// retrieves  the GPR registers
	
	
};

struct SMediaSpaceCmdDefinition vrCmdList[] =
{
	BASIC_COMMANDS,
    {(int) Cmd_WakeUpOnPTS, (int *) &ampCommand.generic.control.notification.pts, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "Notify PTS", 0, NULL, 0, NULL, 0, NULL}, \
	
	{(int) VRCmd_Play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Play", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_PG_Set_Play, (int *) &ampCommand.vr.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Start", 0, "Enter end action (0=default, 1=Stop)", 0, NULL, 0, NULL},
	{(int) VRCmd_Pause_On, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause ON", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Pause_Off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Pause OFF", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Stop, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Forward_Scan, (int *) &ampCommand.vr.param1.speed, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_FASTFORWARD,  "Fast Forward (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", LPBPLAY_MODE_KEY, NULL, 0, NULL},
	{(int) VRCmd_Forward_Scan, (int *) &ampCommand.vr.param1.speed, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_SLOW, "Slow Forward", 512, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) VRCmd_Backward_Scan, (int *) &ampCommand.vr.param1.speed, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_REWIND, "Rewind (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", LPBPLAY_MODE_KEY, NULL, 0, NULL},
	{(int) VRCmd_PG_Play, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param3.endAction, NULL, DIET_KEYPRESS, DIKS_TITLE, "PG play", 0, "Enter a PG number, then press ENTER", 0, "Enter end action (0=default, 1=Stop)", 0, NULL},
	{(int) VRCmd_PL_Play, (int *) &ampCommand.vr.param1.selector,(int *) &ampCommand.vr.param3.endAction, NULL, DIET_KEYPRESS, DIKS_TITLE, "PL play", 0, "Enter a PL number, then press ENTER", 0, "Enter end action (0=default, 1=Stop)", 0, NULL},
	{(int) VRCmd_PG_Time_Play, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param3.endAction, (struct SMediaTime *) &ampCommand.vr.param2.time, DIET_KEYPRESS, DIKS_LIST, "PG time play", 0, "Enter a PG number, then press ENTER", 0, "Enter end action (0=default, 1=Stop)", 0, NULL},
	{(int) VRCmd_PL_Time_Play, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param3.endAction, (struct SMediaTime *) &ampCommand.vr.param2.time, DIET_KEYPRESS, DIKS_LIST, "PL time play", 0, "Enter a PL number, then press ENTER", 0, "Enter end action (0=default, 1=Stop)", 0, NULL},
	{(int) VRCmd_PG_EP_Play, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param2.selector, (struct SMediaTime *) &ampCommand.vr.param3.endAction, DIET_KEYPRESS, DIKS_LIST, "PG EP play", 0, "Enter a PG number, then press ENTER", 0, "Enter a EP number, then press ENTER", 0, "Enter end action (0=default, 1=Stop)"},
	{(int) VRCmd_PL_EP_Play, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param2.selector, (struct SMediaTime *) &ampCommand.vr.param3.endAction, DIET_KEYPRESS, DIKS_LIST, "PL EP play", 0, "Enter a PL number, then press ENTER", 0, "Enter a EP number, then press ENTER", 0, "Enter end action (0=default, 1=Stop)"},
	{(int) VRCmd_Forward_Skip, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next EP", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Backward_Skip, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Previous EP", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Still_Off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Still OFF", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Step, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Step", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Resume, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Resume disc", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Audio_Select, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Audio select", 0, "Enter a stream number (0-1), then press ENTER", 0, "Enter a channel number (0-3), then press ENTER", 0, NULL},
	{(int) VRCmd_Sub_picture_On, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "Subpicture ON", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Sub_picture_Off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "Subpicture OFF", 0, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Goto_Marker, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Show disc icon", 1, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Goto_Marker, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Show PG icon (after Query PG)", 2, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Goto_Marker, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Show PL icon (after Query PL)", 3, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Query_PG, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Query PG", 0, "Enter a PG number, then press ENTER", 0, NULL, 0, NULL},
	{(int) VRCmd_Query_PL, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Query PL", 0, "Enter a PL number, then press ENTER", 0, NULL, 0, NULL},
	{(int) VRCmd_Query_PG_Cell, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Query PG Cell", 0, "Enter a PG number, then press ENTER", 0, "Enter a Cell number, then press ENTER", 0, NULL},
	{(int) VRCmd_Query_PL_Cell, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Query PL Cell", 0, "Enter a PL number, then press ENTER", 0, "Enter a Cell number, then press ENTER", 0, NULL},
	{(int) VRCmd_Query_PG_EP_Name, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Query PG EP name", 0, "Enter a PG number, then press ENTER", 0, "Enter a EP number, then press ENTER", 0, NULL},
	{(int) VRCmd_Query_PL_EP_Name, (int *) &ampCommand.vr.param1.selector, (int *) &ampCommand.vr.param2.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Query PL EP name", 0, "Enter a PL number, then press ENTER", 0, "Enter a EP number, then press ENTER", 0, NULL},
	{(int) VRCmd_Query_Info, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Query info", 0, "Enter an info item number, then press ENTER", 0, NULL, 0, NULL},
	{(int) VRCmd_Manual_Still, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Manual Still Picture Playback", 0, "Select 0 for Automatic and 1 for Manual playback of Still Pictures", 0, NULL, 0, NULL},
	{(int) VRCmd_Repeat, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat OFF", VR_REPEAT_NONE, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Repeat, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Disc", VR_REPEAT, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Repeat, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Item", VR_REPEAT_ITEM, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Repeat, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat A-B", VR_REPEAT_AB, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Repeat, (int *) &ampCommand.vr.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat EP", VR_REPEAT_EP, NULL, 0, NULL, 0, NULL},
	{(int) VRCmd_Query_Bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Save", 0, NULL, 0, NULL, 0, NULL},	// uses VRCmd_Query_BookmarkSize and VRCmd_Query_Bookmark to set a bookmark for the current position
	{(int) VRCmd_Resume_Bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Restore", 0, NULL, 0, NULL, 0, NULL}, // resumes playback from the last bookmark set
	

};
#ifndef NO_VCD_SUPPORT	
struct SMediaSpaceCmdDefinition vcdCmdList[] =
{
	BASIC_COMMANDS,
	{(int) VCDCmd_Play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Play", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Pause, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Resume", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Stop, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Next, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Track", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Previous, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Previous Track", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_DefaultSelection, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_SELECT, "Default Selection", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Return, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Return", 0, NULL, 0, NULL, 0, NULL},	
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_ENTER, "NUMERIC SELECTION Button", 0, "Enter button number, then press ENTER", 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_1, "Button 1 NUMERIC SELECTION", 1, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_2, "Button 2 NUMERIC SELECTION", 2, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_3, "Button 3 NUMERIC SELECTION", 3, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_4, "Button 4 NUMERIC SELECTION", 4, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_5, "Button 5 NUMERIC SELECTION", 5, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_6, "Button 6 NUMERIC SELECTION", 6, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "Button 7 NUMERIC SELECTION", 7, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_8, "Button 8 NUMERIC SELECTION", 8, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_NumericSelection, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "Button 9 NUMERIC SELECTION", 9, NULL, 0, NULL, 0, NULL},	
	{(int) VCDCmd_Step, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Step", 0, NULL, 0, NULL, 0, NULL}, 
	{(int) VCDCmd_FastForward, (int *) &ampCommand.vcd.param1.speed, (int *) &ampCommand.vcd.param2.selector, NULL, DIET_KEYPRESS, DIKS_FASTFORWARD, "Fast forward (I-Frames Only)", 2048, "Enter speed in nominal speed/1024 units, then press ENTER", PLAY_MODE_KEY, NULL, 0, NULL},	
	{(int) VCDCmd_FastForward, (int *) &ampCommand.vcd.param1.speed, (int *) &ampCommand.vcd.param2.selector, NULL, DIET_KEYPRESS, DIKS_SLOW, "Slow Forward", 512, "Enter forward speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) VCDCmd_Rewind, (int *) &ampCommand.vcd.param1.speed, (int *) &ampCommand.vcd.param2.selector, NULL, DIET_KEYPRESS, DIKS_REWIND, "Fast rewind (I-Frames Only)", 2048, "Enter speed in nominal speed/1024 units, then press ENTER", PLAY_MODE_KEY, NULL, 0, NULL},
	{(int) VCDCmd_Audio_Stream_Change, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Audio select", 0, "Enter a stream number (0 or 1), then press ENTER", 0, NULL, 0, NULL},
	{(int) VCDCmd_Repeat, (int *) &ampCommand.vcd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Entry", VCD_REPEAT_MODE_ENTRY, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Repeat, (int *) &ampCommand.vcd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Track", VCD_REPEAT_MODE_TRACK, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Repeat, (int *) &ampCommand.vcd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat Disc", VCD_REPEAT_MODE_DISC, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_Repeat, (int *) &ampCommand.vcd.param1.repeat, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat OFF", VCD_REPEAT_MODE_NONE, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_PBCControl, (int *) &ampCommand.vcd.param1.enable, NULL, NULL, DIET_KEYPRESS, DIKS_PROGRAM, "Playback Control", 1, "Select 1 for PBC or 0 for linear playback", 0, NULL, 0, NULL},
	{(int) VCDCmd_TimeSearch,  NULL, NULL, (struct SMediaTime *)&ampCommand.vcd.param2.time, DIET_KEYPRESS, DIKS_LIST, "Time search", 0, NULL, 0, NULL, 0, NULL},
	{(int) VCDCmd_TrackSearch, (int *) &ampCommand.vcd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "Track search", 0, "Enter a track number, then press ENTER", 0, NULL, 0, NULL},	
};
#endif  // #ifndef NO_VCD_SUPPORT	

#endif
////////////////////////////////////////////// End Red Package ///////////////////////////////////////////////////////////////

#ifndef NO_BLUE_SUPPORT

#define BD_COMMON_CMDS \
{(int) Cmd_Adjust, (int *) &adjustment.value.volume, (int *) &adjustment.type, NULL, DIET_KEYPRESS, DIKS_VOLUME_UP, "Main audio volume", 29, "Enter volume (0-32, neutral = 29), then press ENTER", ADJ_MIXER_VOLUME_MAIN, NULL, 0, NULL}, \
{(int) Cmd_Adjust, (int *) &adjustment.value.autoAdjust, (int *) &adjustment.type, NULL, DIET_KEYPRESS, DIKS_VOLUME_UP, "Mixing mode", 0, "Enter source of mixing coefficients (0=application, 1=metadata), then press ENTER", ADJ_MIXER_MODE, NULL, 0, NULL}, \
{(int) Cmd_Adjust, (int *) &adjustment.value.volume, (int *) &adjustment.type, NULL, DIET_KEYPRESS, DIKS_VOLUME_UP, "2nd audio volume", 29, "Enter volume (0-32, neutral = 29), then press ENTER", ADJ_MIXER_VOLUME_SECONDARY, NULL, 0, NULL}, \
{(int) Cmd_Adjust, (int *) &adjustment.type, (int *) &adjustment.value.pan.x, (struct SMediaTime *) &adjustment.value.pan.y, DIET_KEYPRESS, DIKS_VOLUME_UP, "2nd audio pan", ADJ_MIXER_PAN_SECONDARY, NULL, 15, "Enter X position (0-30, center = 15), then press ENTER", 15, "Enter Y position (0-30, center = 15), then press ENTER"}, \
{(int) BRDCmd_get_meta_data_value, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_VOLUME_DOWN, "Get metadata", 0, NULL, 0, NULL, 0, NULL}, \
{(int) Cmd_WakeUpOnPTS, (int *) &ampCommand.generic.control.notification.pts, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "Notify PTS", 0, NULL, 0, NULL, 0, NULL}

#define BD_USER_EVENTS \
	{(int) BRDCmd_play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Play", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_pause_on, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause ON", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_pause_off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Pause OFF", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_step, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_SLOW, "Step", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_step_back, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_SLOW, "Step back", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_still_on, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Still ON", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_still_off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Still OFF", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_first_play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "FirstPlay", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_stop, (int *) &ampCommand.brd.param1.stopMode, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, "Enter stopMode, then press ENTER: (0-BLANK, 1-LAST FRAME)", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_forward_play, (int *) &ampCommand.brd.param1.speed, (int *) &ampCommand.brd.param2.selector, NULL, DIET_KEYPRESS, DIKS_FASTFORWARD,  "Fast Forward (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", LPBPLAY_MODE_KEY, NULL, 0, NULL}, \
	{(int) BRDCmd_forward_play, (int *) &ampCommand.brd.param1.speed, (int *) &ampCommand.brd.param2.selector, NULL, DIET_KEYPRESS, DIKS_SLOW, "Slow Forward", 512, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_backward_play, (int *) &ampCommand.brd.param1.speed, (int *) &ampCommand.brd.param2.selector, NULL, DIET_KEYPRESS, DIKS_REWIND, "Rewind (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", LPBPLAY_MODE_KEY, NULL, 0, NULL}, \
	{(int) BRDCmd_resume, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Resume", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_0, "Button 0 SELECT AND ACTIVATE", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_1, "Button 1 SELECT AND ACTIVATE", 1, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_2, "Button 2 SELECT AND ACTIVATE", 2, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_3, "Button 3 SELECT AND ACTIVATE", 3, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_4, "Button 4 SELECT AND ACTIVATE", 4, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_5, "Button 5 SELECT AND ACTIVATE", 5, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_6, "Button 6 SELECT AND ACTIVATE", 6, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "Button 7 SELECT AND ACTIVATE", 7, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_8, "Button 8 SELECT AND ACTIVATE", 8, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "Button 9 SELECT AND ACTIVATE", 9, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button_and_activate, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SELECT, "Button 10+ SELECT AND ACTIVATE", 0, "Enter button number, then press ENTER", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_select_button, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SELECT, "Button SELECT", 0, "Enter button number, then press ENTER", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_menu_call, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Top Menu ", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_title_search, (int *) &ampCommand.brd.param1.selector, (int *)&ampCommand.brd.param3.endAction, NULL, DIET_KEYPRESS, DIKS_TITLE, "Title search", 0, "Enter a Title number, then press ENTER", 0, "Enter end action (0=default, 1=Stop, 2=Repeat), then press ENTER", 0, NULL}, \
	{(int) BRDCmd_chapter_search, (int *) &ampCommand.brd.param1.selector, (int *)&ampCommand.brd.param3.endAction, NULL, DIET_KEYPRESS, DIKS_TITLE, "Chapter search", 0, "Enter a Chapter number, then press ENTER", 0, "Enter end action (0=default, 1=Stop, 2=Repeat), then press ENTER", 0, NULL}, \
	{(int) BRDCmd_time_search, NULL, NULL, (struct SMediaTime *)&ampCommand.brd.param2.time, DIET_KEYPRESS, DIKS_LIST, "Time search", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_skip_to_next_point, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Point", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_skip_back_to_previous_point, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Prev Point", 0, NULL, 0, NULL, 0, NULL}, \
	{(int) BRDCmd_primary_audio_stream_number_change, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Audio select", 0, "Enter a stream number, then press ENTER", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_PG_textST_stream_number_change, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "ST select", 0, "Enter stream number, then press ENTER", 0, NULL /*"Enter display status (0/1), then press ENTER"*/, 0, NULL}, \
	{(int) BRDCmd_PG_textST_enable_disable_flag, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "ST on/off", 0, "Select action: 1= enable, 0=disable, then press Enter", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_PiP_PG_textST_stream_number_change, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "PiP ST select", 0, "Enter stream number, then press ENTER", 0, NULL /*"Enter display status (0/1), then press ENTER"*/, 0, NULL}, \
	{(int) BRDCmd_secondary_video_stream_number_change, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_TITLE, "2nd Video select", 0, "Enter stream number, then press ENTER", 0, NULL	/*"Enter display status (0/1), then press ENTER"*/, 0, NULL}, \
	{(int) BRDCmd_secondary_video_enable_disable_flag, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_TITLE, "2nd Video on/off", 0, "Select action: 1= enable, 0=disable, then press Enter", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_secondary_audio_stream_number_change, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "2nd Audio select", 0, "Enter stream number, then press ENTER", 0, NULL /*"Enter display status (0/1), then press ENTER"*/, 0, NULL}, \
	{(int) BRDCmd_secondary_audio_enable_disable_flag, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "2nd Audio on/off", 0, "Select action: 1= enable, 0=disable, then press Enter", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_angle_number_change, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_ANGLE, "Angle select", 0, "Enter an angle number, then press ENTER", 0, NULL, 0, NULL}, \
	{(int) BRDCmd_set_ts_style, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "Text ST style select", 0, "Enter a Style number, then press ENTER", 0, NULL, 0, NULL}




struct SMediaSpaceCmdDefinition brdCmdList[] =
{
	BASIC_COMMANDS,

	// HDMV-specific user operations
	{(int) BRDCmd_popup_on, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Popup ON", 0, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_popup_off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Popup OFF", 0, NULL, 0, NULL, 0, NULL},
		
	BD_COMMON_CMDS,
		
	// navigation commands handled separately in HDMV and BD-J modes because of autorepeat reasons (only BD-J keys support autorepeat)
	{(int) BRDCmd_move_up_selected_button, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_UP, "Button UP", 0, NULL, 0, NULL, 0, NULL}, 
	{(int) BRDCmd_move_down_selected_button, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_DOWN, "Button DOWN", 0, NULL, 0, NULL, 0, NULL}, 
	{(int) BRDCmd_move_left_selected_button, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_LEFT, "Button LEFT", 0, NULL, 0, NULL, 0, NULL}, 
	{(int) BRDCmd_move_right_selected_button, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_RIGHT, "Button RIGHT", 0, NULL, 0, NULL, 0, NULL}, 
	{(int) BRDCmd_activate_button, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_ENTER, "Button ACTIVATE", 0, NULL, 0, NULL, 0, NULL}, 
	
	// user events shared with BD-J
	BD_USER_EVENTS,
	
	// BD-J/custom commands
	{(int) BRDCmd_play_pl_playitem, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param1.selector, NULL, DIET_KEYPRESS, DIKS_LIST, "Play PL PI", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, NULL},
	{(int) BRDCmd_play_pl_mark, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param1.selector, NULL, DIET_KEYPRESS, DIKS_LIST, "Play PL Mark", 0, "Enter PL number, then press ENTER", 0, "Enter Mark number, then press ENTER", 0, NULL},
	{(int) BRDCmd_play_pl_time, (int *) &ampCommand.brd.param3.playListID, NULL, (struct SMediaTime *)&ampCommand.brd.param2.time, DIET_KEYPRESS, DIKS_LIST, "Play PL Time", 0, "Enter PL number, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_skip_to_next_mark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Mark", 0, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_skip_back_to_previous_mark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Prev Mark", 0, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_menu_language, (int *) &ampCommand.brd.param1.language, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Set Menu Lang", 0, "Enter a language code, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_audio_language, (int *) &ampCommand.brd.param1.language, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Set Audio Lang", 0, "Enter a language code, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_subtitle_language, (int *) &ampCommand.brd.param1.language, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Set Subtitle Lang", 0, "Enter a language code, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_gpr, (int *) &ampCommand.brd.param1.selector, (int *) &ampCommand.brd.param2.regValue, NULL, DIET_KEYPRESS, DIKS_SETUP, "Set GPR", 0, "Enter GPR index, then press ENTER", 0, "Enter GPR value, then press ENTER", 0, NULL},
	{(int) BRDCmd_set_psr, (int *) &ampCommand.brd.param1.selector, (int *) &ampCommand.brd.param2.regValue, NULL, DIET_KEYPRESS, DIKS_SETUP, "Set PSR", 0, "Enter PSR index, then press ENTER", 0, "Enter PSR value, then press ENTER", 0, NULL},
	{(int) BRDCmd_get_psr, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Get PSR", 0, "Enter PSR index, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_get_gpr, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SETUP, "Get GPR", 0, "Enter GPR index, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_get_title_info, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Get Title Info", 0, "Enter Title number, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_get_pl_id, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Get PL ID", 0, "Enter Title number, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_get_pl_info, (int *) &ampCommand.brd.param3.playListID, NULL, NULL, DIET_KEYPRESS, DIKS_INFO, "Get PL Info", 0, "Enter PL number, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_get_pi_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param1.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Get PI Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, NULL},
	{(int) BRDCmd_get_mark_info , (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param1.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Get Mark Info", 0, "Enter PL number, then press ENTER", 0, "Enter Mark number, then press ENTER", 0, NULL},
	{(int) BRDCmd_get_chapter_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param1.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Get Chapter Info", 0, "Enter PL number, then press ENTER", 0, "Enter Chapter number, then press ENTER", 0, NULL},
	{(int) BRDCmd_get_video_stream_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param2.selector, (struct SMediaTime *) &ampCommand.brd.param1.selector, DIET_KEYPRESS, DIKS_EDITOR, "Get Video Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, "Enter stream number, then press ENTER"},
	{(int) BRDCmd_get_audio_stream_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param2.selector, (struct SMediaTime *) &ampCommand.brd.param1.selector, DIET_KEYPRESS, DIKS_EDITOR, "Get Audio Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, "Enter stream number, then press ENTER"},
	{(int) BRDCmd_get_pg_ts_stream_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param2.selector, (struct SMediaTime *) &ampCommand.brd.param1.selector, DIET_KEYPRESS, DIKS_EDITOR, "Get ST Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, "Enter stream number, then press ENTER"},
	{(int) BRDCmd_get_ig_stream_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param2.selector, (struct SMediaTime *) &ampCommand.brd.param1.selector, DIET_KEYPRESS, DIKS_EDITOR, "Get IG Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, "Enter stream number, then press ENTER"},
	{(int) BRDCmd_get_sec_video_stream_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param2.selector, (struct SMediaTime *) &ampCommand.brd.param1.selector, DIET_KEYPRESS, DIKS_EDITOR, "Get 2nd Video Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, "Enter stream number, then press ENTER"},
	{(int) BRDCmd_get_sec_audio_stream_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param2.selector, (struct SMediaTime *) &ampCommand.brd.param1.selector, DIET_KEYPRESS, DIKS_EDITOR, "Get 2nd Audio Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, "Enter stream number, then press ENTER"},
	{(int) BRDCmd_get_PiP_PG_textST_stream_info, (int *) &ampCommand.brd.param3.playListID, (int *) &ampCommand.brd.param2.selector, (struct SMediaTime *) &ampCommand.brd.param1.selector, DIET_KEYPRESS, DIKS_EDITOR, "Get PiP ST Info", 0, "Enter PL number, then press ENTER", 0, "Enter PI number, then press ENTER", 0, "Enter stream number, then press ENTER"},
	{(int) BRDCmd_set_PiP_PG_textST_valid_flag, (int *) &ampCommand.brd.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_SUBTITLE, "Validate PiP Subtitle", 0, "Select action: 1= valid, 0=invalid, then press Enter", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_title_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat title ON", PIEA_Repeat, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_title_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat title OFF", PIEA_Default, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_chapter_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat chapter ON", PIEA_Repeat, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_chapter_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat chapter OFF", PIEA_Default, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_title_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Set title end action", 0, "Enter end action (0=default, 1=Stop, 2=Repeat), then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_chapter_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Set chapter end action", 0, "Enter end action (0=default, 1=Stop, 2=Repeat), then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_query_bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Save", 0, NULL, 0, NULL, 0, NULL}, // uses BRDCmd_Query_BookmarkSize and BRDCmd_Query_Bookmark to set a bookmark for the current position
	{(int) BRDCmd_resume_bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Restore", 0, NULL, 0, NULL, 0, NULL}, // resumes playback from the last bookmark set
	
};

struct SMediaSpaceCmdDefinition bdjCmdList[] =
{
	BASIC_COMMANDS,
		
	// BDJ-specific user operations
	{(int) BRDCmd_popup_on, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_MENU, "Popup ON/OFF", 0, NULL, 0, NULL, 0, NULL},
	
	BD_COMMON_CMDS,
		
	// BDJ single key events
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_0, "0", BDJ_VK_SINGLEKEY | BDJ_VK_0, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_1, "1", BDJ_VK_SINGLEKEY | BDJ_VK_1, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_2, "2", BDJ_VK_SINGLEKEY | BDJ_VK_2, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_2, "A", BDJ_VK_SINGLEKEY | BDJ_VK_A, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_2, "B", BDJ_VK_SINGLEKEY | BDJ_VK_B, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_2, "C", BDJ_VK_SINGLEKEY | BDJ_VK_C, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_3, "3", BDJ_VK_SINGLEKEY | BDJ_VK_3, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_3, "D", BDJ_VK_SINGLEKEY | BDJ_VK_D, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_3, "E", BDJ_VK_SINGLEKEY | BDJ_VK_E, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_3, "F", BDJ_VK_SINGLEKEY | BDJ_VK_F, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_4, "4", BDJ_VK_SINGLEKEY | BDJ_VK_4, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_4, "G", BDJ_VK_SINGLEKEY | BDJ_VK_G, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_4, "H", BDJ_VK_SINGLEKEY | BDJ_VK_H, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_4, "I", BDJ_VK_SINGLEKEY | BDJ_VK_I, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_5, "5", BDJ_VK_SINGLEKEY | BDJ_VK_5, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_5, "J", BDJ_VK_SINGLEKEY | BDJ_VK_J, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_5, "K", BDJ_VK_SINGLEKEY | BDJ_VK_K, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_5, "L", BDJ_VK_SINGLEKEY | BDJ_VK_L, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_6, "6", BDJ_VK_SINGLEKEY | BDJ_VK_6, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_6, "M", BDJ_VK_SINGLEKEY | BDJ_VK_M, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_6, "N", BDJ_VK_SINGLEKEY | BDJ_VK_N, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_6, "O", BDJ_VK_SINGLEKEY | BDJ_VK_O, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "7", BDJ_VK_SINGLEKEY | BDJ_VK_7, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "P", BDJ_VK_SINGLEKEY | BDJ_VK_P, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "Q", BDJ_VK_SINGLEKEY | BDJ_VK_Q, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "R", BDJ_VK_SINGLEKEY | BDJ_VK_R, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_7, "S", BDJ_VK_SINGLEKEY | BDJ_VK_S, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_8, "8", BDJ_VK_SINGLEKEY | BDJ_VK_8, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_8, "T", BDJ_VK_SINGLEKEY | BDJ_VK_T, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_8, "U", BDJ_VK_SINGLEKEY | BDJ_VK_U, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_8, "V", BDJ_VK_SINGLEKEY | BDJ_VK_V, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "9", BDJ_VK_SINGLEKEY | BDJ_VK_9, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "W", BDJ_VK_SINGLEKEY | BDJ_VK_W, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "X", BDJ_VK_SINGLEKEY | BDJ_VK_X, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "Y", BDJ_VK_SINGLEKEY | BDJ_VK_Y, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_9, "Z", BDJ_VK_SINGLEKEY | BDJ_VK_Z, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_0, "Color 0 (Red)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_0, NULL, 0, NULL, 0, NULL}, // for remotes without color keys
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_RED, "Color 0 (Red)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_0, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_0, "Color 1 (Green)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_1, NULL, 0, NULL, 0, NULL}, // for remotes without color keys
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_GREEN, "Color 1 (Green)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_1, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_0, "Color 2 (Yellow)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_2, NULL, 0, NULL, 0, NULL}, // for remotes without color keys
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_YELLOW, "Color 2 (Yellow)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_2, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_0, "Color 3 (Blue)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_3, NULL, 0, NULL, 0, NULL}, // for remotes without color keys
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_BLUE, "Color 3 (Blue)", BDJ_VK_SINGLEKEY | BDJ_VK_COLORED_KEY_3, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_CLEAR, "Cancel", BDJ_VK_SINGLEKEY | BDJ_VK_CANCEL, NULL, 0, NULL, 0, NULL},
	
	// navigation commands handled separately in HDMV and BD-J modes because of autorepeat reasons (only BD-J keys support autorepeat)
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_UP, "Button UP", BDJ_VK_KEYPRESSED | BDJ_VK_SINGLEKEY | BDJ_VK_UP, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_DOWN, "Button DOWN", BDJ_VK_KEYPRESSED | BDJ_VK_SINGLEKEY | BDJ_VK_DOWN, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_LEFT, "Button LEFT", BDJ_VK_KEYPRESSED | BDJ_VK_SINGLEKEY | BDJ_VK_LEFT, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_CURSOR_RIGHT, "Button RIGHT", BDJ_VK_KEYPRESSED | BDJ_VK_SINGLEKEY | BDJ_VK_RIGHT, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYPRESS, DIKS_ENTER, "Button ACTIVATE", BDJ_VK_KEYPRESSED | BDJ_VK_SINGLEKEY | BDJ_VK_ENTER, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYRELEASE, DIKS_CURSOR_UP, "Button UP", BDJ_VK_KEYRELEASED | BDJ_VK_UP, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYRELEASE, DIKS_CURSOR_DOWN, "Button DOWN", BDJ_VK_KEYRELEASED | BDJ_VK_DOWN, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYRELEASE, DIKS_CURSOR_LEFT, "Button LEFT", BDJ_VK_KEYRELEASED | BDJ_VK_LEFT, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYRELEASE, DIKS_CURSOR_RIGHT, "Button RIGHT", BDJ_VK_KEYRELEASED | BDJ_VK_RIGHT, NULL, 0, NULL, 0, NULL},
	{(int) BDJCmd_key_event, (int *)&ampCommand.bdj.param2.selector, NULL, NULL, DIET_KEYRELEASE, DIKS_ENTER, "Button ACTIVATE", BDJ_VK_KEYRELEASED | BDJ_VK_ENTER, NULL, 0, NULL, 0, NULL},
	
	// user events shared with HDMV
	BD_USER_EVENTS,
	
	// commands normally used by BD-J but needed by config GUI
	{(int) BRDCmd_set_menu_language, (int *) &ampCommand.brd.param1.language, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Set Menu Lang", 0, "Enter a language code, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_audio_language, (int *) &ampCommand.brd.param1.language, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Set Audio Lang", 0, "Enter a language code, then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_subtitle_language, (int *) &ampCommand.brd.param1.language, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Set Subtitle Lang", 0, "Enter a language code, then press ENTER", 0, NULL, 0, NULL},
	
	// custom commands shared with HDMV
	{(int) BRDCmd_set_title_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat title ON", PIEA_Repeat, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_title_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat title OFF", PIEA_Default, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_chapter_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat chapter ON", PIEA_Repeat, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_chapter_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_RESTART, "Repeat chapter OFF", PIEA_Default, NULL, 0, NULL, 0, NULL},
	{(int) BRDCmd_set_title_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Set title end action", 0, "Enter end action (0=default, 1=Stop, 2=Repeat), then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_set_chapter_end_action, (int *)&ampCommand.brd.param3.endAction, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Set chapter end action", 0, "Enter end action (0=default, 1=Stop, 2=Repeat), then press ENTER", 0, NULL, 0, NULL},
	{(int) BRDCmd_query_bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Save", 0, NULL, 0, NULL, 0, NULL}, // uses BRDCmd_Query_BookmarkSize and BRDCmd_Query_Bookmark to set a bookmark for the current position
	{(int) BRDCmd_resume_bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Restore", 0, NULL, 0, NULL, 0, NULL}, // resumes playback from the last bookmark set
	
};


struct SMediaSpaceCmdDefinition bdreCmdList[] =
{
	BASIC_COMMANDS,
    {(int) Cmd_WakeUpOnPTS, (int *) &ampCommand.generic.control.notification.pts, NULL, NULL, DIET_KEYPRESS, DIKS_LIST, "Notify PTS", 0, NULL, 0, NULL, 0, NULL}, \

	{(int) BDRECmd_play, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Play", 0, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_play_pl_time, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param1.selector, NULL, DIET_KEYPRESS, DIKS_TITLE, "Play PL", 0, "Enter a PlayList number, then press ENTER", 0, "Enter 4 digit PIN (10000 - no PIN), then press ENTER", 0, NULL},
	{(int) BDRECmd_stop, (int *) &ampCommand.brd.param1.stopMode, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, "Enter stopMode, then press ENTER: (0-BLANK, 1-LAST FRAME)", 0, NULL, 0, NULL},
	{(int) BDRECmd_pause_on, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause ON", 0, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_pause_off, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause OFF", 0, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_step_fwd, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_FORWARD, "Step", 0, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_step_rev, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACK, "Step back", 0, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_forward_play, (int *) &ampCommand.bdre.param1.speed, (int *) &ampCommand.bdre.param2.selector, NULL, DIET_KEYPRESS, DIKS_FASTFORWARD,  "Scan Forward (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", PLAY_MODE_KEY, NULL, 0, NULL},
	{(int) BDRECmd_forward_play, (int *) &ampCommand.bdre.param1.speed, (int *) &ampCommand.bdre.param2.selector, NULL, DIET_KEYPRESS, DIKS_FORWARD,  "Fast Forward (smooth)", 2048, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) BDRECmd_backward_play, (int *) &ampCommand.bdre.param1.speed, (int *) &ampCommand.bdre.param2.selector, NULL, DIET_KEYPRESS, DIKS_REWIND, "Scan Rewind (I-Frames Only)", 10240, "Enter speed in nominal speed/1024 units, then press ENTER", PLAY_MODE_KEY, NULL, 0, NULL},
	{(int) BDRECmd_backward_play, (int *) &ampCommand.bdre.param1.speed, (int *) &ampCommand.bdre.param2.selector, NULL, DIET_KEYPRESS, DIKS_BACK, "Fast Rewind (smooth)", 2048, "Enter speed in nominal speed/1024 units, then press ENTER", 0, NULL, 0, NULL},
	{(int) BDRECmd_audio_change, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_LANGUAGE, "Audio Change", 0, "Enter a Stream number, then press ENTER", 0, NULL, 0, NULL},
	{(int) BDRECmd_view_change, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_ANGLE, "View Change", 0, "Enter a Stream number, then press ENTER", 0, NULL, 0, NULL},
	{(int) BDRECmd_play_pl_mark, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param1.selector, (struct SMediaTime *) &ampCommand.bdre.param2.time, DIET_KEYPRESS, DIKS_LIST, "Play PL Mark", 0, "Enter a PlayList number, then press ENTER", 10000, "Enter 4 digit PIN (10000 - no PIN), then press ENTER", 0, "Enter Mark ID, then press ENTER"},
	{(int) BDRECmd_play_pl_time, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param1.selector, (struct SMediaTime *) &ampCommand.bdre.param2.time, DIET_KEYPRESS, DIKS_LIST, "Play PL Time", 0, "Enter a PlayList number, then press ENTER", 0, "Enter 4 digit PIN (10000 - no PIN), then press ENTER", 0, NULL},
	{(int) BDRECmd_skip_to_next_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Chapter", BDRE_MARK_CHAPTER, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_next_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Bookmark", BDRE_MARK_FAVORITE, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_next_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Resume Mark", BDRE_MARK_RESUME, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_next_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Skip-Start", BDRE_MARK_SKIP_START, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_next_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_NEXT, "Next Point", DCCHD_BDRE_MARK_ANY, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_previous_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Prev Chapter", BDRE_MARK_CHAPTER, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_previous_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Prev Bookmark", BDRE_MARK_FAVORITE, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_previous_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Resume Mark", BDRE_MARK_RESUME, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_previous_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Prev Skip-Start", BDRE_MARK_SKIP_START, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_skip_to_previous_mark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_PREVIOUS, "Prev Point", DCCHD_BDRE_MARK_ANY, NULL, 0, NULL, 0, NULL},
	{(int) BDRECmd_get_pl_info, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param1.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Get PL Info", 0, "Enter a PlayList number, then press ENTER", 10000, "Enter 4 digit PIN (10000 - no PIN)", 0, NULL},
	{(int) BDRECmd_get_pi_info, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param2.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Get PI Info", 0, "Enter a PlayList number, then press ENTER", 0, "Enter PlayItem number, then press ENTER", 0, NULL},
	{(int) BDRECmd_get_pl_mark_info, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param2.selector, NULL, DIET_KEYPRESS, DIKS_INFO, "Get PL Mark Info", 0, "Enter a PlayList ID, then press ENTER", 0, "Enter Mark index, then press ENTER", 0, NULL},
	{(int) BDRECmd_get_clip_mark_info, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param2.selector, (struct SMediaTime *) &ampCommand.bdre.param1.selector, DIET_KEYPRESS, DIKS_INFO, "Get Clip Mark Info", 0, "Enter a PlayList number, then press ENTER", 0, "Enter a Clip ID, then press ENTER", 0, "Enter Mark index, then press ENTER"},
	// NOTE: ampCommand.bdre.param2.selector is used as a temporary variable, it will be overwritten with the address of the thumbnail buffer
	{(int) BDRECmd_get_thumbnail, (int *) &ampCommand.bdre.param3.playListID, (int *) &ampCommand.bdre.param1.selector, (struct SMediaTime *) &ampCommand.bdre.param2.selector, DIET_KEYPRESS, DIKS_INFO, "Get Thumbnail", 0, "Enter a PlayList number, then press ENTER", 0, "Enter a Thumbnail index, then press ENTER", 0, "Enter thumbnail type (0 - menu, 1 - PlayList mark, 2 - Clip Mark), then press ENTER"},
	{(int) BDRECmd_query_bookmark, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Save", 0, NULL, 0, NULL, 0, NULL},	// uses BDRECmd_query_bookmark_size and BDRECmd_query_bookmark to set a bookmark for the current position
	{(int) BDRECmd_resume_bookmark, (int *) &ampCommand.bdre.param1.selector, NULL, NULL, DIET_KEYPRESS, DIKS_BACKSPACE, "Restore", 0, "Enter 4 digit PIN (10000 - no PIN), then press ENTER", 0, NULL, 0, NULL}, // resumes playback from the last bookmark set
};
#endif
/////////////////////////////////////////// End Blue Package /////////////////////////////////////////////////////////
#ifndef NO_DTV_SUPPORT
struct SMediaSpaceCmdDefinition dtvCmdList[] =
{
	BASIC_COMMANDS,

	{(int) DTVCmd_PLAY, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PLAY, "Play", 0, NULL, 0, NULL, 0, NULL},
	{(int) DTVCmd_STOP, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_STOP, "Stop", 0, NULL, 0, NULL, 0, NULL},

	{(int) DTVCmd_PAUSE_ON, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Pause", 0, NULL, 0, NULL, 0, NULL},
	{(int) DTVCmd_PAUSE_OFF, NULL, NULL, NULL, DIET_KEYPRESS, DIKS_PAUSE, "Resume", 0, NULL, 0, NULL, 0, NULL},
	{(int) DTVCmd_SET_SECTCBF,(int *) &ampCommand.dtv.param1.SecDesc,(int *) &ampCommand.dtv.param2.fSecCallback,NULL,DIET_KEYPRESS,DIKS_SELECT,"PSI mon",0,NULL,0,NULL,0,NULL},
	{(int) DTVCmd_SET_DSMCCCBF,(int *) &ampCommand.dtv.param1.SecDesc,(int *) &ampCommand.dtv.param2.fSecCallback,NULL,DIET_KEYPRESS,DIKS_PREVIOUS,"DSMCC",0,"Pid Number :",0,NULL,0,NULL},
	{(int) DTVCmd_SET_ECMKEYS,(int *) &ampCommand.dtv.param1.Pid,(int *) &ampCommand.dtv.param2.CipherKey,NULL,DIET_KEYPRESS,DIKS_NEXT,"EcmKeys",0,NULL,0,NULL,0,NULL},
	{(int) DTVCmd_START_FILTS,(int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS,DIKS_RESTART,"Start Sect/PES", 0, "Stop:0 Start:1", 0, NULL, 0, NULL},
	{(int) DTVCmd_STOP_FILTS,(int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS,DIKS_RESTART,"Stop Sect/PES", 0, NULL, 0, NULL, 0, NULL},
	{(int) DTVCmd_SET_PESCBF,(int *) &ampCommand.dtv.param1.PesDesc,(int *) &ampCommand.dtv.param2.fPesCallback,NULL,DIET_KEYPRESS,DIKS_SLOW,"PES mon",0,NULL,0,NULL,0,NULL},
	{(int) DTVCmd_SET_STCDRIFT, (int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS, DIKS_SUBTITLE, "set STC drift mode", 0, "Disable = 0: Enable = 1", 0, NULL, 0, NULL},
	{(int) DTVCmd_GET_VCXO_ADJ, (int *) &ampCommand.dtv.param1.Adj,NULL,NULL,DIET_KEYPRESS, DIKS_ANGLE, "get STC speed", 0, "get:0 set:1", 0, NULL, 0, NULL},
	{(int) DTVCmd_SET_VCXO_ADJ, (int *) &ampCommand.dtv.param2.Input,NULL,NULL,DIET_KEYPRESS, DIKS_ANGLE, "set STC speed", 0, "M=1000000000, N =?", 0, NULL, 0, NULL},
	{(int) DTVCmd_STEP, NULL,NULL,NULL,DIET_KEYPRESS, DIKS_MENU, "Step", 0, NULL, 0, NULL, 0, NULL},
    {(int) DTVCmd_SEEKPOS, (int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS, DIKS_MENU, "Seek position", 0, "miliseconds:", 0, NULL, 0, NULL},
    {(int) DTVCmd_SET_INDEX_FILE, (int *) &ampCommand.dtv.param1.FileName,NULL,NULL,DIET_KEYPRESS, DIKS_MENU, "Set index file",(int)"SAVE/Save.idx" , NULL, 0, NULL, 0, NULL},
	{(int) DTVCmd_SET_AVSOURCE, (int *) &ampCommand.dtv.param1.PesDesc,(int *) &ampCommand.dtv.param2.PesDesc,(struct SMediaTime *) &ampCommand.dtv.param3.Pid,DIET_KEYPRESS, DIKS_REWIND, "set av source", 0, NULL, 0, NULL, 0, "Press [Enter]"},
	{(int) DTVCmd_FASTFWD, (int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS, DIKS_LIST, "FastFwd", 2, "Speed :", 0, NULL, 0, NULL},
    {(int) DTVCmd_FASTRWD, (int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS, DIKS_LIST, "FastRwd", 2, "Speed :", 0, NULL, 0, NULL},
	{(int) DTVCmd_SET_DOWNMIXTABLE, (int *) &ampCommand.dtv.param2.Input,NULL,NULL,DIET_KEYPRESS, DIKS_CLEAR, "Downmix table", 0, "Disable = 0 : Enable = 1", 0, NULL, 0, NULL},
	{(int) DTVCmd_SET_CHANNEL_UP, (int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS, DIKS_CURSOR_UP, "Channel Up", 0, NULL, 0, NULL, 0, NULL},
    {(int) DTVCmd_SET_CHANNEL_DN, (int *) &ampCommand.dtv.param1.Input,NULL,NULL,DIET_KEYPRESS, DIKS_CURSOR_DOWN, "Channel Down", 0, NULL, 0, NULL, 0, NULL},
    {(int) DTVCmd_SET_FORMATMONCBF, (int *) &ampCommand.dtv.param1.fFormatCallback,NULL,NULL,DIET_KEYPRESS, DIKS_TITLE, "set format change callback", 0, NULL, 0, NULL, 0, NULL},
	{(int) DTVCmd_SET_PARTIALTSCBF,(int *) &ampCommand.dtv.param1.TsPids,(int *)&ampCommand.dtv.param2.PesDesc,(struct SMediaTime *)&ampCommand.dtv.param3.fPTSCallback,DIET_KEYPRESS, DIKS_EDITOR, "Partial TS mon", 0, NULL, 0, NULL, 0, "Press [Enter]"},
    {(int) DTVCmd_SET_TUNER, (int *) &ampCommand.dtv.param1.Input,(int *) &ampCommand.dtv.param2.Input,(struct SMediaTime *) &ampCommand.dtv.param3.Input,DIET_KEYPRESS, DIKS_LANGUAGE, "Tuner setting", 0, "Frequency:", 2, "Model:", 12, "Mode:"},
};
#endif // DTV_SUPPORT
#endif /*AMPTEST_DEFINE_H*/
