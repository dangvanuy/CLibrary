/***********************************************************************
  Module: ir_scancodes.h
  Copyright (c) Sigma Designs 2004.  All Rights Reserved

***********************************************************************/
/**
* @file   ir_scancodes.h
* @brief  Remote key event
*/

/**
$Log : $
*/

#ifndef _IR_SCANCODES_H_
#define _IR_SCANCODES_H_

/*
 *	 Using them in ACTIONSCRIPT
 */
#define ZENO_EVENT_KEY_ALT                       0x00000012 ///< used to dispatch Key.ALT
#define ZENO_EVENT_KEY_BACKSPACE                 0x00000008 ///< used to dispatch Key.BACKSPACE
#define ZENO_EVENT_KEY_CAPSLOCK                  0x00000014 ///< used to dispatch Key.CAPSLOCK
#define ZENO_EVENT_KEY_CONTROL                   0x00000011 ///< used to dispatch Key.CONTROL
#define ZENO_EVENT_KEY_INSERT                    0x0000002D ///< used to dispatch Key.INSERT
#define	ZENO_EVENT_KEY_DELETEKEY                 0x0000002E ///< used to dispatch Key.DELETEKEY
#define ZENO_EVENT_KEY_DOWN                      0x00000028 ///< used to dispatch Key.DOWN
#define ZENO_EVENT_KEY_END                       0x00000023 ///< used to dispatch Key.END
#define ZENO_EVENT_KEY_ENTER                     0x0000000D ///< used to dispatch Key.ENTER
#define ZENO_EVENT_KEY_ESCAPE                    0x0000001B ///< used to dispatch Key.ESCAPE
#define ZENO_EVENT_KEY_HOME                      0x00000024 ///< used to dispatch Key.HOME
#define ZENO_EVENT_KEY_LEFT                      0x00000025 ///< used to dispatch Key.LEFT
#define ZENO_EVENT_KEY_PGDN                      0x00000022 ///< used to dispatch Key.PGDN
#define ZENO_EVENT_KEY_PGUP                      0x00000021 ///< used to dispatch Key.PGUP
#define ZENO_EVENT_KEY_RIGHT                     0x00000027 ///< used to dispatch Key.RIGHT
#define ZENO_EVENT_KEY_SHIFT                     0x00000010 ///< used to dispatch Key.SHIFT
#define ZENO_EVENT_KEY_SPACE                     0x00000020 ///< used to dispatch Key.SPACE
#define ZENO_EVENT_KEY_TAB                       0x00000009 ///< used to dispatch Key.TAB
#define ZENO_EVENT_KEY_UP                        0x00000026 ///< used to dispatch Key.UP



#define ZENO_EVENT_KEY_VOLUME_UP                 0x01000001 ///< volume up
#define ZENO_EVENT_KEY_VOLUME_DOWN               0x01000002 ///< volume down
#define ZENO_EVENT_KEY_VOLUME_MUTE               0x01000003 ///< volume mute
#define ZENO_EVENT_KEY_CHANNEL_UP                0x01000004 ///< channel up
#define ZENO_EVENT_KEY_CHANNEL_DOWN              0x01000005 ///< channel down
#define ZENO_EVENT_KEY_RECORD                    0x01000006 ///< record item or engage record transport mode
#define ZENO_EVENT_KEY_PLAY                      0x01000007 ///< engage play transport mode
#define ZENO_EVENT_KEY_PAUSE                     0x01000008 ///< engage pause transport mode
#define ZENO_EVENT_KEY_STOP                      0x01000009 ///< engage stop transport mode
#define ZENO_EVENT_KEY_FAST_FORWARD              0x0100000A ///< engage fast-forward transport mode
#define ZENO_EVENT_KEY_REWIND                    0x0100000B ///< engage rewind transport mode
#define ZENO_EVENT_KEY_SKIP_FORWARD              0x0100000C ///< quick skip ahead (usually 30 seconds)
#define ZENO_EVENT_KEY_SKIP_BACKWARD             0x0100000D ///< quick skip backward (usually 7-10 seconds)
#define ZENO_EVENT_KEY_NEXT                      0x0100000E ///< skip to next track or chapter
#define ZENO_EVENT_KEY_PREVIOUS                  0x0100000F ///< skip to previous track or chapter
#define ZENO_EVENT_KEY_LIVE                      0x01000010 ///< return to live [position in broadcast]
#define ZENO_EVENT_KEY_LAST                      0x01000011 ///< watch last channel or show watched
#define ZENO_EVENT_KEY_MENU                      0x01000012 ///< engage menu
#define ZENO_EVENT_KEY_INFO                      0x01000013 ///< info button
#define ZENO_EVENT_KEY_GUIDE                     0x01000014 ///< engage program guide
#define ZENO_EVENT_KEY_EXIT                      0x01000015 ///< exits current application mode
#define ZENO_EVENT_KEY_BACK                      0x01000016 ///< return back to previous page in application
#define ZENO_EVENT_KEY_AUDIO                     0x01000017 ///< select the audio mode
#define ZENO_EVENT_KEY_SUBTITLE                  0x01000018 ///< toggle subtitles
#define ZENO_EVENT_KEY_DVR                       0x01000019 ///< engage dvr application mode
#define ZENO_EVENT_KEY_VOD                       0x0100001A ///< engage video on demand
#define ZENO_EVENT_KEY_INPUT                     0x0100001B ///< cycle input
#define ZENO_EVENT_KEY_SETUP                     0x0100001C ///< engage setup application or menu
#define ZENO_EVENT_KEY_HELP                      0x0100001D ///< engage help application or context-sensitive help
#define ZENO_EVENT_KEY_MASTER_SHELL              0x0100001E ///< engage "Master Shell" e.g. TiVo or other vendor button
#define ZENO_EVENT_KEY_RED                       0x0100001F ///< red function key button
#define ZENO_EVENT_KEY_GREEN                     0x01000020 ///< green function key button
#define ZENO_EVENT_KEY_YELLOW                    0x01000021 ///< yellow function key button
#define ZENO_EVENT_KEY_BLUE                      0x01000022 ///< blue function key button

/*
 *	 Using it in C++
 */
#define ZENO_EVENT_KEY_POWER                     DIKS_POWER //
#define ZENO_EVENT_IDLE							 0 //
//End of Add

#endif //
