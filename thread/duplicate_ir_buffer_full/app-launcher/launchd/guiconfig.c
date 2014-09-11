/*****************************************
 Copyright  2005
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/
#include <stdio.h>
#include <stdlib.h>

#define TEXT_LEN 256
#define SHORT_TEXT_LEN 50





/******************************************************************************************************
	TEXT ON MENU
*******************************************************************************************************/
#define NUM_MENU_TEXT 7

typedef enum
{
	FONT_HEADER = 0,
	FONT_NORMAL,
}eFont;

typedef struct sText{
	char str[TEXT_LEN];
	int x;
	int y;
	int font;
	int color_a;
	int color_r;
	int color_g;
	int color_b;
}sText;

sText s_ArrMenuText[] = {
    #ifdef WITH_IPTV
    {"PRODUCTION TEST PROGRAM", 640, 32, FONT_HEADER, 0xff, 0xff, 0xff, 0x00},
    {"Auto Test", 640, 160, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Press Key 0 to test all the items in turn or Key [1~3] to select the test item", 640, 400, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Press Key 9 to start updating production firmware", 620, 645, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Items", 300, 140, FONT_NORMAL, 0xff, 0x00, 0x00, 0x00},
	{"Result", 1000, 140, FONT_NORMAL, 0xff, 0x00, 0x00, 0x00},
    #else

	{"LOADING ...", 640, 350, FONT_HEADER, 0xff, 0xff, 0xff, 0x00},
    {"Auto Test", 640, 160, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Press Key 0 to test all the items in turn or Key [1~3] to select the test item", 640, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Press Key 7 to start \"get keys\" process", 552, 595, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Press Key 9 to start updating production firmware", 620, 645, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Items", 300, 140, FONT_NORMAL, 0xff, 0x00, 0x00, 0x00},
	{"Result", 1000, 140, FONT_NORMAL, 0xff, 0x00, 0x00, 0x00},
    #endif

};


/******************************************************************************************************
	BACKGROUND OF MenuTopFrontKey 
*******************************************************************************************************/
#define NUM_MenuTopFrontKey 4
#define NUM_MENUTopFrontKey_TEXT 7

typedef struct MenuTopFrontKey{
	int x;
	int y;
	int w;
	int h;
	int color_a;
	int color_r;
	int color_g;
	int color_b;
}MenuTopFrontKey;

MenuTopFrontKey s_ArrMenuTopFrontKey[] = {
    {30, 15, 1215, 50,  0xff, 0xff, 0xff, 0xff},
    {35, 20, 1205, 40, 0xff, 0x66, 0x33, 0xcc},

    {30, 70, 1215, 50,  0xff, 0xff, 0xff, 0xff},
	{35, 75, 1205, 40, 0xff, 0x66, 0x33, 0xcc},
};



/******************************************************************************************************
	BACKGROUND OF MENU 
*******************************************************************************************************/
#define NUM_RECTANGLE 3


typedef struct sRectangle{
	int x;
	int y;
	int w;
	int h;
	int color_a;
	int color_r;
	int color_g;
	int color_b;
}sRectangle;

sRectangle s_ArrRectangle[] = {
	{0, 0, 1280, 720, 0xff, 0x00, 0x7a, 0xc5},
	{0, 120, 1280, 7, 0xff, 0x00, 0x00, 0x00},
	{0, 570, 1280, 7, 0xff, 0x00, 0x00, 0x00},
};

/******************************************************************************************************
	BACKGROUND OF MESSAGE 
*******************************************************************************************************/
#define NUM_RECTANGLE_BGMESSAGE 2
#define NUM_RECTANGLE_BGMESSAGEWITHKEYRETURN 3
#define NUM_RECTANGLE_BGFRONTKEYMESSAGE 4
#define NUM_RECTANGLE_BGCONFIRMMESSAGE 5


sRectangle s_ArrBgMessage[] = {
	{350, 260, 550, 130, 0xff, 0xff, 0xff, 0xff},
	{355, 265, 540, 120, 0xff, 0x00, 0x7a, 0xc5},
};

sRectangle s_ArrBgMessageWithKeyReturn[] = {
	{350, 260, 550, 130, 0xff, 0xff, 0xff, 0xff},
	{355, 265, 540, 120, 0xff, 0x00, 0x7a, 0xc5},
    {530, 320, 180, 40, 0xff, 0x00, 0x00, 0x66},
};

sRectangle s_ArrBgFrontKeyMessage[] = {
	{350, 260, 550, 130, 0xff, 0xff, 0xff, 0xff},
	{355, 265, 540, 120, 0xff, 0x00, 0x7a, 0xc5},
    {410, 320, 180, 40, 0xff, 0x00, 0x00, 0x66},
    {650, 320, 180, 40, 0xff, 0x00, 0x00, 0x66},
};

sRectangle s_ArrBgConfirmMessage[] = {
	{320, 260, 600, 150, 0xff, 0xff, 0xff, 0xff},
	{325, 265, 590, 140, 0xff, 0x00, 0x7a, 0xc5},
	{335, 340, 180, 40, 0xff, 0x00, 0x00, 0x66},
	{530, 340, 180, 40, 0xff, 0x00, 0x00, 0x66},
    {725, 340, 180, 40, 0xff, 0x00, 0x00, 0x66},
};

typedef enum
{
    MESSAGE_TEST_SECURITY_KEY = 0,
	MESSAGE_TEST_HDCP_KEY,
	MESSAGE_TEST_USB,
	MESSAGE_TEST_FRONT_DISPLAY,
    MESSAGE_TEST_FRONT_KEY_POWER,
    MESSAGE_TEST_FRONT_KEY_MENU,
    MESSAGE_TEST_FRONT_KEY_OK,
    MESSAGE_TEST_FRONT_KEY_CHAN_DW,
    MESSAGE_TEST_FRONT_KEY_CHAN_UP,
    MESSAGE_TEST_FRONT_KEY_VOL_DW,
    MESSAGE_TEST_FRONT_KEY_VOL_UP,
    MESSAGE_TEST_TUNER_OUTPUT,
    MESSAGE_START_PLAYING_CHANNEL,
    MESSAGE_PRESS_UP_CHANNEL,
    MESSAGE_CONFIRM_FRONT_DISPLAY,
    MESSAGE_CONFIRM_PRESS_UP,
	MESSAGE_CONFIRM_GET_KEY,
    MESSAGE_CONFIRM_UPDATE_PROD_FW,
	MESSAGE_STARTING_GET_KEY,
	MESSAGE_GET_KEY_SUCCESSFUL,
	MESSAGE_GET_KEY_FAILED,
	MESSAGE_NOT_FIND_PROD_FW,
    MESSAGE_KEY0_PASSED,
    MESSAGE_KEY1_FAILED,     
    MESSAGE_KEYRETURN_EXIT,    
	MESSAGE_START_PLAYING_NEXT_CHANNEL,
	PLAYING_CHANNEL,
	PLAYING_CHANNEL_01,
	PLAYING_CHANNEL_02,
    MESSAGE_KEY0_YES,
    MESSAGE_KEY1_NO, 
    #ifdef WITH_IPTV
    MESSAGE_TEST_NETWORK,
    MESSAGE_TEST_OUTPUT,   
    #endif 
     
	NUM_MESSAGE,
}eTestMessage;

sText s_ArrMessageText[] = {
    {"Start testing Security Key", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Start testing HDCP Key", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Start testing USB", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Start testing Front Display", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Please press key POWER", 625, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Please press key MENU", 625, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Please press key OK", 625, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Please press key CHAN-", 625, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Please press key CHAN+", 625, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Please press key VOL-", 625, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Please press key VOL+", 625, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Start testing tuner", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Start Playing channel 1", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Press Up Channel Key to see the next channel", 675, 300, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Do you see the number 88:88 on Front Display?", 625, 307, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Do you see two channels on all output?", 625, 307, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Do you want to start \"get keys\" process?", 625, 307, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Do you want to update production firmware?", 625, 307, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Starting \"get keys\" process. Please wait!", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Get keys successful", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Get keys failed", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Can not find the production firmware", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Key 0: Pass", 405, 363, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Key 1: Failed", 620, 363, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff}, 
    {"Key return: Exit", 815, 363, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff}, 
	{"Start Playing channel 2", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"Playing channel 1", 675, 300 + 15, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"CH 01", 1170, 40, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
	{"CH 02", 1170, 40, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Key 0: Yes", 405, 363, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Key 1: No", 620, 363, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff}, 
    #ifdef WITH_IPTV
    {"Start testing NETWORK", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    {"Start testing Output", 625, 324, FONT_NORMAL, 0xff, 0xff, 0xff, 0xff},
    #endif


};


/******************************************************************************************************
	TEST ITEMS 
*******************************************************************************************************/

typedef enum
{
	//TEST_SECURITY = 0,
	//TEST_HDCP,
	TEST_USB,
    #ifdef WITH_IPTV
    TEST_NETWORK,
    #endif
	TEST_FRONT_DISPLAY,
    TEST_FRONT_KEY,
    #ifdef WITH_IPTV
    TEST_OUTPUT,
    #else
    TEST_TUNER,
    #endif
    
	NUM_TEST_ITEM
}eTestItem;

typedef enum
{
	STATUS_UNTESTED = 0,
	STATUS_PASSED,
	STATUS_FAILED,
	STATUS_UNSUPPORTED,
	NUM_TEST_STATUS
}eTestStatus;


char statusText[NUM_TEST_STATUS][SHORT_TEXT_LEN] = {
	"Untested",
	"Passed",
	"Failed",
	"Unsupported"
};


typedef struct sTestItem{
	int type;
	char scriptfile[TEXT_LEN];
	char str[TEXT_LEN];
	int x1;
	int y1;
	int status;
	int x2;
	int y2;
	int messageid;
	int confirmid;
}sTestItem;

sTestItem s_ArrTestItem[] = {
    #ifdef WITH_IPTV
    {TEST_SECURITY, "Security_Test", "1. Assign and Verify Security Key", 200, 200, STATUS_UNTESTED, 1000, 200, MESSAGE_TEST_SECURITY_KEY, MESSAGE_TEST_SECURITY_KEY},
	{TEST_HDCP, "HDCP_Test", "2. Assign and Verify HDCP Key", 200, 250, STATUS_UNTESTED, 1000, 250, MESSAGE_TEST_HDCP_KEY, MESSAGE_TEST_HDCP_KEY},
	{TEST_USB, "USB_Test", "3. Test USB", 200, 300, STATUS_UNTESTED, 1000, 300, MESSAGE_TEST_USB, MESSAGE_TEST_USB},
    {TEST_NETWORK, "NETWORK_Test", "4. Test Network", 200, 350, STATUS_PASSED, 1000, 350, MESSAGE_TEST_NETWORK, MESSAGE_TEST_NETWORK},
    {TEST_FRONT_DISPLAY, "FrontDisplay_Test", "1. Test Front Display", 200, 450, STATUS_UNTESTED, 1000, 450, MESSAGE_TEST_FRONT_DISPLAY, MESSAGE_CONFIRM_FRONT_DISPLAY},
	{TEST_FRONT_KEY, "FrontKey_Test", "2. Test Front Key", 200, 500, STATUS_UNTESTED, 1000, 500, MESSAGE_TEST_FRONT_KEY_POWER, MESSAGE_TEST_FRONT_KEY_POWER},
	{TEST_OUTPUT, "Output_Test", "3. Test Outputs", 200, 550, STATUS_UNTESTED, 1000, 550, MESSAGE_TEST_OUTPUT, MESSAGE_TEST_OUTPUT},
    #else
	//{TEST_SECURITY, "Security_Test", "1. Assign and Verify Security Key", 200, 200, STATUS_PASSED, 1000, 200, MESSAGE_TEST_SECURITY_KEY, MESSAGE_TEST_SECURITY_KEY},
	//{TEST_HDCP, "HDCP_Test", "2. Assign and Verify HDCP Key", 200, 250, STATUS_PASSED, 1000, 250, MESSAGE_TEST_HDCP_KEY, MESSAGE_TEST_HDCP_KEY},
	{TEST_USB, "USB_Test", "1. Test USB", 200, 230, STATUS_PASSED, 1000, 230, MESSAGE_TEST_USB, MESSAGE_TEST_USB},
    {TEST_FRONT_DISPLAY, "FrontDisplay_Test", "1. Test Front Display", 200, 370, STATUS_UNTESTED, 1000, 370, MESSAGE_TEST_FRONT_DISPLAY, MESSAGE_CONFIRM_FRONT_DISPLAY},
	{TEST_FRONT_KEY, "FrontKey_Test", "2. Test Front Key", 200, 440, STATUS_UNTESTED, 1000, 440, MESSAGE_TEST_FRONT_KEY_POWER, MESSAGE_TEST_FRONT_KEY_POWER},
	{TEST_TUNER, "TunerAndOutput_Test", "3. Test Tuner and Outputs", 200, 510, STATUS_UNTESTED, 1000, 510, MESSAGE_TEST_TUNER_OUTPUT, MESSAGE_TEST_TUNER_OUTPUT},
    #endif
};

static void get_color_for_status_text(int status, int *a, int *r, int *g, int *b)
{
	switch(status) {
	case STATUS_UNTESTED:
	case STATUS_PASSED:
	case STATUS_UNSUPPORTED:
	case STATUS_FAILED:
		*a = 0xff;
		*r = 0xff;
		*g = 0xff;
		*b = 0xff;
		break;

	default:
		*a = 0xff;
		*r = 0xcc;
		*g = 0x00;
		*b = 0x00;
		break;
	}
}

/******************************************************************************************************
	Items MenuTop Test FrontKey 
*******************************************************************************************************/

typedef struct ItemsMenuTopFrontKey{
	char str[TEXT_LEN];
	int x1;
	int y1;
	int status;
	int x2;
	int y2;
}ItemsMenuTopFrontKey;

ItemsMenuTopFrontKey s_ArrItemsMenuTopFrontKey[] = {
	{"POWER:", 50, 25, STATUS_UNTESTED, 200, 25},
	{"MENU:", 340, 25, STATUS_UNTESTED, 475, 25},
	{"OK:", 635, 25, STATUS_UNTESTED, 735, 25},
    {"CHAN-:", 50, 80, STATUS_UNTESTED, 200, 80},
	{"CHAN+:", 340, 80, STATUS_UNTESTED, 490, 80},
	{"VOL-:", 635, 80, STATUS_UNTESTED, 750, 80},
    {"VOL+:", 900, 80, STATUS_UNTESTED, 1035, 80},
};













/////////////////////////////////////// end /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


