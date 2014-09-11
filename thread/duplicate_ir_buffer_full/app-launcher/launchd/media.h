
#ifndef AMPTEST_H_
#define AMPTEST_H_

  
 

#include <directfb.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <termios.h>
#include <unistd.h>

#include <linux/input.h>
#include <linux/keyboard.h>

#include <stdio.h>
#include <string.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdbool.h>

#include <directfb.h>
#include <directfb_keynames.h>
#include <directfb_util.h>

#include <direct/clock.h>
#include <direct/messages.h>

#include <sawman.h>

#include <tinyxml.h>

#include "rmdef/rmdef.h"
#include <advancedmediaprovider.h>
#ifdef	APP_DEBUG
#define LIB_DEBUG(str, args...)	fprintf(stderr, str, ##args)  //debug output
#define APP_ASSERT(x)			assert(x)
#else
#define LIB_DEBUG(str, args...)	// no debug output
#define APP_ASSERT(x)			
#endif

#ifdef ENABLE_MESSAGE_TO_DEBUG
#define GUI_INFO(str, args...)   fprintf(stderr, str, ##args)  //debug output
#else
#define GUI_INFO(str, args...)	// no debug output
#endif

#define SCHED_PRIO_DEFAULT 20
#define WIDTH_OF_GUI  1280
#define HEIGHT_OF_GUI 720

#define WIDTH_OF_PRIMARY  1280//1920
#define HEIGHT_OF_GUI_PRIMARY 720//1080

#define GUI_PREVIEW_THUMBNAIL_X 263
#define GUI_PREVIEW_THUMBNAIL_Y 220
#define GUI_PREVIEW_X  1300
#define GUI_PREVIEW_Y  220
#define GUI_PREVIEW_W  360
#define GUI_PREVIEW_H  240


#ifndef SPLIT_CHARACTER
#define SPLIT_CHARACTER "#"
#endif

#define MAX_LENGTH_STRING 512
#define MIN_LENGTH_STRING 4



#include "init_directfb.h"
#include "gui.h"



#endif /*AMPTEST_H_*/
