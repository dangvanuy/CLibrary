/*****************************************
 Copyright  2005
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/
#include "guiconfig.c"



IDirectFBFont           *font 			= NULL;
IDirectFBFont           *font_header 	= NULL;

/* The adjustments (Setup) window */
IDirectFBWindow         *adj_window 	= NULL;
IDirectFBSurface        *adj 			= NULL;

static const int		adj_width 		= 1500;
static const int 		osdBPP = 32;


char 					font_filename[1024] = "";
bool 					wnd2Buf;
bool 					opaqueSetupMenu;	
int 					keyfd;
int 					xres;
int 					yres;
int 					fontheight;
int 					fontheaderheight; //for Header text
int 					fontascender;
int 					fontdescender;




/**
* 
* <b>FUNCTION: </b>
*
* uninit_osd_layer()
*
* <b>DESCRIPTION: </b>
* 
* Uninitialize all varaiables related to OSD Layer
* 
* @param No
*   
* @return No
*
*/
void uninit_osd_layer(void)
{
	if (primary)
	{
		primary->Release(primary);
		primary = NULL;
	}

	if (layer)
	{
		layer->Release(layer);
		layer = NULL;
	}
}


/**
* 
* <b>FUNCTION: </b>
*
* init_gui()
*
* <b>DESCRIPTION: </b>
* 
* Initialize gui
* 
* @param No
*   
* @return No
*
*/
void init_gui(void)
{
	DFBDisplayLayerConfig dlcfg;
	DFBResult err;
	DFBWindowDescription wdsc;
    
	// find out the size of the primary surface
	DFBCHECK(layer->GetConfiguration(layer, &dlcfg));
	xres = dlcfg.width;
	yres = dlcfg.height;

	// set up font
	if (FONTNAME[0] != '/')
	{
		char *dfbDir = getenv("DCCHD_INSTALL_DIR");
		if (dfbDir)
		{
			strcpy(font_filename, dfbDir);
			strcat(font_filename, "/share/directfb-examples/fonts/");
		}
	}
	strcat(font_filename, FONTNAME);

	DFBFontDescription desc;

	// Setup font for Header text
	desc.flags = DFDESC_HEIGHT;
	desc.height = yres/15;
	printf("\n font_filename = %s",font_filename);
    DFBCHECK(gdfb->CreateFont( gdfb, font_filename, &desc, &font_header ));
	DFBCHECK(font_header->GetHeight( font_header, &fontheaderheight ));
	DFBCHECK(font_header->GetAscender( font_header, &fontascender ));
	DFBCHECK(font_header->GetDescender( font_header, &fontdescender ));
	if (fontheaderheight < fontdescender + fontascender)
		fontheaderheight = fontdescender + fontascender;

	// Setup font for Normal text
	desc.flags = DFDESC_HEIGHT;
	desc.height = yres/28;
    DFBCHECK(gdfb->CreateFont( gdfb, font_filename, &desc, &font ));
	DFBCHECK(font->GetHeight( font, &fontheight ));
	DFBCHECK(font->GetAscender( font, &fontascender ));
	DFBCHECK(font->GetDescender( font, &fontdescender ));
	if (fontheight < fontdescender + fontascender)
		fontheight = fontdescender + fontascender;


	//For forcing hide GUI
	DFBCHECK(layer->GetSurface( layer, &primary));
	DFBCHECK(primary->SetDrawingFlags( primary, DSDRAW_NOFX ));
	// clear the background and keep it that way
	DFBCHECK(layer->SetBackgroundColor( layer, 0xFF, 0xFF, 0xFF, 0x00 ));
	DFBCHECK(layer->SetBackgroundMode(layer, DLBM_COLOR));
	//End of forcing hide GUI

    wdsc.flags  = (DFBWindowDescriptionFlags)(DWDESC_CAPS | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_POSX | DWDESC_POSY | DWDESC_SURFACE_CAPS | DWDESC_PIXELFORMAT);
	wdsc.caps   = (DFBWindowCapabilities)((opaqueSetupMenu ? 0 : DWCAPS_ALPHACHANNEL) | (wnd2Buf ? DWCAPS_DOUBLEBUFFER : 0));
	wdsc.posx   = 0;
	wdsc.posy   = 0;
	wdsc.width  = 1280;
	wdsc.height = 720;
	wdsc.surface_caps = DSCAPS_VIDEOONLY;
	wdsc.pixelformat  = osdBPP == 8 ? DSPF_LUT8 : osdBPP == 16 ? DSPF_ARGB4444 : DSPF_ARGB;
	DFBCHECK(layer->CreateWindow(layer, &wdsc, &adj_window));
	DFBCHECK(adj_window->GetSurface(adj_window, &adj));
	DFBCHECK(adj_window->SetOpacity(adj_window, 0));   // the window is hidden until the setup menu is activated
	DFBCHECK(adj->SetFont(adj, font));
	
}


/**
* 
* <b>FUNCTION: </b>
*
* unInitGUI()
*
* <b>DESCRIPTION: </b>
* 
* Uninitialize all varaiables related to GUI
* 
* @param No
*   
* @return No
*
*/
void uninit_gui(void)
{	
	if (adj)
	{
		adj->Release(adj);
		adj = NULL;
	}

	if (adj_window)
	{
		adj_window->Release(adj_window);
		adj_window = NULL;
	}

	if(font)
	{
		font->Release(font);
		font = NULL;
	}

	if(font_header)
	{
		font_header->Release(font_header);
		font_header = NULL;
	}
}




/**
* 
* <b>FUNCTION: </b>
*
* show_test_items()
*
* <b>DESCRIPTION: </b>
* 
* Show the test items
* 
* @param No
*   
* @return No
*
*/
void show_test_items(void)
{
	DFBResult err;
	DFBColor color_text, color_status;
	int i;
	
	color_text.a = 0xff;
	color_text.r = 0xff;
	color_text.g = 0xff;
	color_text.b = 0x00;

	DFBCHECK(adj->SetFont(adj, font));
	
	for (i = 0; i < NUM_TEST_ITEM; i++) 
	{
		
		adj->SetColor(adj, color_text.r, color_text.g, color_text.b, color_text.a);	
		adj->DrawString(adj, (char*)s_ArrTestItem[i].str, -1, s_ArrTestItem[i].x1, s_ArrTestItem[i].y1, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
       // adj->DrawString(adj, (char*)s_ArrTestAllItem[i].str, -1, s_ArrTestAllItem[i].x1, s_ArrTestAllItem[i].y1, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));

		get_color_for_status_text(s_ArrTestItem[i].status, (int *)&color_status.a, (int *)&color_status.r, (int *)&color_status.g, (int *)&color_status.b);
		adj->SetColor(adj, color_status.r, color_status.g, color_status.b, color_status.a);	
		adj->DrawString(adj, (char*)statusText[s_ArrTestItem[i].status], -1, s_ArrTestItem[i].x2, s_ArrTestItem[i].y2, (DFBSurfaceTextFlags)(DSTF_CENTER | DSTF_TOP));
       // adj->DrawString(adj, (char*)statusText[s_ArrTestAllItem[i].status], -1, s_ArrTestAllItem[i].x2, s_ArrTestAllItem[i].y2, (DFBSurfaceTextFlags)(DSTF_CENTER | DSTF_TOP));
    }
}

/**
* 
* <b>FUNCTION: </b>
*
* show_menu()
*
* <b>DESCRIPTION: </b>
* 
* Show menu
* 
* @param No
*   
* @return No
*
*/
void show_menu(void)
{
	DFBResult err;
	int i;

	//draw background
	for (i = 0; i < 1; i++) {
		adj->SetColor(adj, s_ArrRectangle[i].color_r, s_ArrRectangle[i].color_g, s_ArrRectangle[i].color_b, s_ArrRectangle[i].color_a);	
		adj->FillRectangle(adj, s_ArrRectangle[i].x, s_ArrRectangle[i].y, s_ArrRectangle[i].w, s_ArrRectangle[i].h);
	}
	for (i = 0; i < 1; i++) {
		if (s_ArrMenuText[i].font == FONT_HEADER) {
			DFBCHECK(adj->SetFont(adj, font_header));
		}
		else{
			DFBCHECK(adj->SetFont(adj, font));
		}
		adj->SetColor(adj, s_ArrMenuText[i].color_r, s_ArrMenuText[i].color_g, s_ArrMenuText[i].color_b, s_ArrMenuText[i].color_a);	
		adj->DrawString(adj, (char*)s_ArrMenuText[i].str, -1, s_ArrMenuText[i].x, s_ArrMenuText[i].y, (DFBSurfaceTextFlags)(DSTF_CENTER | DSTF_TOP));			
	}
	//show_test_items();
	adj->Flip(adj, NULL, DSFLIP_NONE);
}
void show_menu_front_display(void)
{
    show_message(MESSAGE_TEST_FRONT_DISPLAY);
}
void show_menu_config_front_display(void)
{
    show_confirm_message(MESSAGE_CONFIRM_FRONT_DISPLAY);
}
void show_menu_front_key(RMuint8 key)
{
    s_ArrMessageText[MESSAGE_KEY1_FAILED].x=500;
    s_ArrMessageText[MESSAGE_KEY1_FAILED].y=343;

    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x=740;
    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y=343;
	switch (key) {    
	default:
		show_message_with_key_return(MESSAGE_TEST_FRONT_KEY_POWER);
		break;
	}

    s_ArrMessageText[MESSAGE_KEY1_FAILED].x=620;
    s_ArrMessageText[MESSAGE_KEY1_FAILED].y=363;

    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x=815;
    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y=363;
    show_menu_top_front_key();


}

void show_menu_tunner_and_output(void)
{
    show_message(MESSAGE_TEST_TUNER_OUTPUT);
}
void show_menu_start_playing_channel(void)
{
    show_message(MESSAGE_START_PLAYING_CHANNEL);
}
void show_menu_start_playing_next_channel(void)
{
    show_message(MESSAGE_START_PLAYING_NEXT_CHANNEL);
}

void show_menu_press_up_channel(void)
{
    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x=670;
    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y=343;
    s_ArrBgMessageWithKeyReturn[0].w=650;
    s_ArrBgMessageWithKeyReturn[1].w=640;
    s_ArrBgMessageWithKeyReturn[2].x=580;
	//show_message(PLAYING_CHANNEL);
    //show_message(MESSAGE_PRESS_UP_CHANNEL);
	////////////////////////////

	DFBResult err;
	int i;

	uninit_gui();
	uninit_osd_layer();

	init_osd_layer(false);
	init_gui();

	/// redraw channel name
	//x  = 320 + 800; y = 260 -250; w = 600-500; h =150 - 100 ; 
	adj->SetColor(adj, s_ArrBgConfirmMessage[0].color_r, s_ArrBgConfirmMessage[0].color_g, s_ArrBgConfirmMessage[0].color_b, s_ArrBgConfirmMessage[0].color_a);	
	adj->FillRectangle(adj, 1120, 10, 100, 50);
	
	//x = 325 + 800, y = 265 -250, w = 590 - 500, h = 140 - 100;
	adj->SetColor(adj, s_ArrBgConfirmMessage[1].color_r, s_ArrBgConfirmMessage[1].color_g, s_ArrBgConfirmMessage[1].color_b, s_ArrBgConfirmMessage[1].color_a);	
	adj->FillRectangle(adj, 1125, 15, 90, 40);

	//x = 325 + 800 = 1125 + 45 = 1170
	//y = 265 - 250 = 15 + 35 = 40
	//draw text
	DFBCHECK(adj->SetFont(adj, font));	
	adj->SetColor(adj, s_ArrMessageText[PLAYING_CHANNEL_01].color_r, s_ArrMessageText[PLAYING_CHANNEL_01].color_g, s_ArrMessageText[PLAYING_CHANNEL_01].color_b, s_ArrMessageText[PLAYING_CHANNEL_01].color_a);	
	adj->DrawString(adj, s_ArrMessageText[PLAYING_CHANNEL_01].str, -1, s_ArrMessageText[PLAYING_CHANNEL_01].x,s_ArrMessageText[PLAYING_CHANNEL_01].y, DSTF_BOTTOMCENTER);			
	
	/// 
	//draw background 
	for (i = 0; i < NUM_RECTANGLE_BGMESSAGEWITHKEYRETURN; i++) {
		adj->SetColor(adj, s_ArrBgMessageWithKeyReturn[i].color_r, s_ArrBgMessageWithKeyReturn[i].color_g, s_ArrBgMessageWithKeyReturn[i].color_b, s_ArrBgMessageWithKeyReturn[i].color_a);	
		adj->FillRectangle(adj, s_ArrBgMessageWithKeyReturn[i].x, s_ArrBgMessageWithKeyReturn[i].y, s_ArrBgMessageWithKeyReturn[i].w, s_ArrBgMessageWithKeyReturn[i].h);
	}
	//draw text
	DFBCHECK(adj->SetFont(adj, font));
	adj->SetColor(adj, s_ArrMessageText[MESSAGE_PRESS_UP_CHANNEL].color_r, s_ArrMessageText[MESSAGE_PRESS_UP_CHANNEL].color_g, s_ArrMessageText[MESSAGE_PRESS_UP_CHANNEL].color_b, s_ArrMessageText[MESSAGE_PRESS_UP_CHANNEL].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_PRESS_UP_CHANNEL].str, -1, s_ArrMessageText[MESSAGE_PRESS_UP_CHANNEL].x, s_ArrMessageText[MESSAGE_PRESS_UP_CHANNEL].y, DSTF_BOTTOMCENTER);			
    
    adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_r, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_g, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_b, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].str, -1, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y, DSTF_BOTTOMCENTER);

	adj_window->SetOpacity(adj_window, 0xFF);   // show window
	s_ArrBgMessageWithKeyReturn[0].w=550;
    s_ArrBgMessageWithKeyReturn[1].w=540;
    s_ArrBgMessageWithKeyReturn[2].x=530;
    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x=815;
    s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y=363;
}
void show_menu_press_up_confirm(void)
{
    show_confirm_message(MESSAGE_CONFIRM_PRESS_UP);
}
void show_menu_update_production_firmware(void)
{
    show_confirm_message_update_fw(MESSAGE_CONFIRM_UPDATE_PROD_FW);
}
void show_menu_not_find_production_firmware(void)
{
    show_message(MESSAGE_NOT_FIND_PROD_FW);
}
void show_menu_red(void)
{        
         s_ArrRectangle[0].color_a=0xff;
         s_ArrRectangle[0].color_r=0xff;
         s_ArrRectangle[0].color_g=0x00;
         s_ArrRectangle[0].color_b=0x00;

         s_ArrMenuText[3].color_a=0xff;
         s_ArrMenuText[3].color_r=0xff;
         s_ArrMenuText[3].color_g=0x00;
         s_ArrMenuText[3].color_b=0x00;

         s_ArrMenuText[4].color_a=0xff;
         s_ArrMenuText[4].color_r=0xff;
         s_ArrMenuText[4].color_g=0x00;
         s_ArrMenuText[4].color_b=0x00;
         show_menu();
}
void show_menu_blue(void)
{        
         s_ArrRectangle[0].color_a=0xff;
         s_ArrRectangle[0].color_r=0x00;
         s_ArrRectangle[0].color_g=0x7a;
         s_ArrRectangle[0].color_b=0xc5; 

         s_ArrMenuText[3].color_a=0xff;
         s_ArrMenuText[3].color_r=0xff;
         s_ArrMenuText[3].color_g=0xff;
         s_ArrMenuText[3].color_b=0xff;

         s_ArrMenuText[4].color_a=0xff;
         s_ArrMenuText[4].color_r=0xff;
         s_ArrMenuText[4].color_g=0xff;
         s_ArrMenuText[4].color_b=0xff;
         show_menu();
}
/**
* 
* <b>FUNCTION: </b>
*
* show_window()
*
* <b>DESCRIPTION: </b>
* 
* Show window
* 
* @param No
*   
* @return No
*
*/
void show_window(void)
{
	adj_window->SetOpacity(adj_window, 0xFF);
}


/**
* 
* <b>FUNCTION: </b>
*
* hide_window()
*
* <b>DESCRIPTION: </b>
* 
* hide window
* 
* @param No
*   
* @return No
*
*/
void hide_window(void)
{
	adj_window->SetOpacity(adj_window, 0x00);
}


/**
* 
* <b>FUNCTION: </b>
*
* show_message()
*
* <b>DESCRIPTION: </b>
* 
* show message
* 
* @param No
*   
* @return No
*
*/
void show_message(int message_id)
{
	DFBResult err;
	int i;

	uninit_gui();
	uninit_osd_layer();

	init_osd_layer(false);
	init_gui();

	
	//draw background 
	for (i = 0; i < NUM_RECTANGLE_BGMESSAGE; i++) {
		adj->SetColor(adj, s_ArrBgMessage[i].color_r, s_ArrBgMessage[i].color_g, s_ArrBgMessage[i].color_b, s_ArrBgMessage[i].color_a);	
		adj->FillRectangle(adj, s_ArrBgMessage[i].x, s_ArrBgMessage[i].y, s_ArrBgMessage[i].w, s_ArrBgMessage[i].h);
	}
	//draw text
	DFBCHECK(adj->SetFont(adj, font));
	adj->SetColor(adj, s_ArrMessageText[message_id].color_r, s_ArrMessageText[message_id].color_g, s_ArrMessageText[message_id].color_b, s_ArrMessageText[message_id].color_a);	
	adj->DrawString(adj, s_ArrMessageText[message_id].str, -1, s_ArrMessageText[message_id].x, s_ArrMessageText[message_id].y, DSTF_BOTTOMCENTER);			
    
	adj_window->SetOpacity(adj_window, 0xFF);   // show window
}

void show_message_with_key_return(int message_id)
{
	DFBResult err;
	int i;

	uninit_gui();
	uninit_osd_layer();

	init_osd_layer(false);
	init_gui();

	
	//draw background 
	for (i = 0; i < NUM_RECTANGLE_BGFRONTKEYMESSAGE; i++) {
		adj->SetColor(adj, s_ArrBgFrontKeyMessage[i].color_r, s_ArrBgFrontKeyMessage[i].color_g, s_ArrBgFrontKeyMessage[i].color_b, s_ArrBgFrontKeyMessage[i].color_a);	
		adj->FillRectangle(adj, s_ArrBgFrontKeyMessage[i].x, s_ArrBgFrontKeyMessage[i].y, s_ArrBgFrontKeyMessage[i].w, s_ArrBgFrontKeyMessage[i].h);
	}
	//draw text
	DFBCHECK(adj->SetFont(adj, font));
	adj->SetColor(adj, s_ArrMessageText[message_id].color_r, s_ArrMessageText[message_id].color_g, s_ArrMessageText[message_id].color_b, s_ArrMessageText[message_id].color_a);	
	adj->DrawString(adj, s_ArrMessageText[message_id].str, -1, s_ArrMessageText[message_id].x, s_ArrMessageText[message_id].y, DSTF_BOTTOMCENTER);			
 
    adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_r, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_g, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_b, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEY1_FAILED].str, -1, s_ArrMessageText[MESSAGE_KEY1_FAILED].x, s_ArrMessageText[MESSAGE_KEY1_FAILED].y, DSTF_BOTTOMCENTER);
	   
    adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_r, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_g, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_b, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].str, -1, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y, DSTF_BOTTOMCENTER);

	adj_window->SetOpacity(adj_window, 0xFF);   // show window
}

/**
* 
* <b>FUNCTION: </b>
*
* show_confirm_message()
*
* <b>DESCRIPTION: </b>
* 
* show confirm message
* 
* @param No
*   
* @return No
*
*/

void show_confirm_message(int message_id)
{
	DFBResult err;
	int i;

	uninit_gui();
	uninit_osd_layer();

	init_osd_layer(false);
	init_gui();

	
	//draw background 
	for (i = 0; i < NUM_RECTANGLE_BGCONFIRMMESSAGE; i++) {
		adj->SetColor(adj, s_ArrBgConfirmMessage[i].color_r, s_ArrBgConfirmMessage[i].color_g, s_ArrBgConfirmMessage[i].color_b, s_ArrBgConfirmMessage[i].color_a);	
		adj->FillRectangle(adj, s_ArrBgConfirmMessage[i].x, s_ArrBgConfirmMessage[i].y, s_ArrBgConfirmMessage[i].w, s_ArrBgConfirmMessage[i].h);
	}
	//draw text
	DFBCHECK(adj->SetFont(adj, font));
	adj->SetColor(adj, s_ArrMessageText[message_id].color_r, s_ArrMessageText[message_id].color_g, s_ArrMessageText[message_id].color_b, s_ArrMessageText[message_id].color_a);	
	adj->DrawString(adj, s_ArrMessageText[message_id].str, -1, s_ArrMessageText[message_id].x, s_ArrMessageText[message_id].y, DSTF_BOTTOMCENTER);			
    
	adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEY0_PASSED].color_r, s_ArrMessageText[MESSAGE_KEY0_PASSED].color_g, s_ArrMessageText[MESSAGE_KEY0_PASSED].color_b, s_ArrMessageText[MESSAGE_KEY0_PASSED].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEY0_PASSED].str, -1, s_ArrMessageText[MESSAGE_KEY0_PASSED].x, s_ArrMessageText[MESSAGE_KEY0_PASSED].y, DSTF_BOTTOMCENTER);			

	adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_r, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_g, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_b, s_ArrMessageText[MESSAGE_KEY1_FAILED].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEY1_FAILED].str, -1, s_ArrMessageText[MESSAGE_KEY1_FAILED].x, s_ArrMessageText[MESSAGE_KEY1_FAILED].y, DSTF_BOTTOMCENTER);			

    adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_r, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_g, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_b, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].str, -1, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y, DSTF_BOTTOMCENTER);


	adj_window->SetOpacity(adj_window, 0xFF);   // show window
}

void show_confirm_message_update_fw(int message_id)
{
	DFBResult err;
	int i;

	uninit_gui();
	uninit_osd_layer();

	init_osd_layer(false);
	init_gui();

	
	//draw background 
	for (i = 0; i < NUM_RECTANGLE_BGCONFIRMMESSAGE; i++) {
		adj->SetColor(adj, s_ArrBgConfirmMessage[i].color_r, s_ArrBgConfirmMessage[i].color_g, s_ArrBgConfirmMessage[i].color_b, s_ArrBgConfirmMessage[i].color_a);	
		adj->FillRectangle(adj, s_ArrBgConfirmMessage[i].x, s_ArrBgConfirmMessage[i].y, s_ArrBgConfirmMessage[i].w, s_ArrBgConfirmMessage[i].h);
	}
	//draw text
	DFBCHECK(adj->SetFont(adj, font));
	adj->SetColor(adj, s_ArrMessageText[message_id].color_r, s_ArrMessageText[message_id].color_g, s_ArrMessageText[message_id].color_b, s_ArrMessageText[message_id].color_a);	
	adj->DrawString(adj, s_ArrMessageText[message_id].str, -1, s_ArrMessageText[message_id].x, s_ArrMessageText[message_id].y, DSTF_BOTTOMCENTER);			
    
	adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEY0_YES].color_r, s_ArrMessageText[MESSAGE_KEY0_YES].color_g, s_ArrMessageText[MESSAGE_KEY0_YES].color_b, s_ArrMessageText[MESSAGE_KEY0_YES].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEY0_YES].str, -1, s_ArrMessageText[MESSAGE_KEY0_YES].x, s_ArrMessageText[MESSAGE_KEY0_YES].y, DSTF_BOTTOMCENTER);			

	adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEY1_NO].color_r, s_ArrMessageText[MESSAGE_KEY1_NO].color_g, s_ArrMessageText[MESSAGE_KEY1_NO].color_b, s_ArrMessageText[MESSAGE_KEY1_NO].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEY1_NO].str, -1, s_ArrMessageText[MESSAGE_KEY1_NO].x, s_ArrMessageText[MESSAGE_KEY1_NO].y, DSTF_BOTTOMCENTER);			

    adj->SetColor(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_r, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_g, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_b, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].color_a);	
	adj->DrawString(adj, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].str, -1, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].x, s_ArrMessageText[MESSAGE_KEYRETURN_EXIT].y, DSTF_BOTTOMCENTER);


	adj_window->SetOpacity(adj_window, 0xFF);   // show window
}


void show_menu_top_front_key(void)
{
    //DFBResult err;
    int i;
	//draw background
	for (i = 0; i < NUM_MenuTopFrontKey; i++) {
		
        adj->SetColor(adj, s_ArrMenuTopFrontKey[i].color_r, s_ArrMenuTopFrontKey[i].color_g, s_ArrMenuTopFrontKey[i].color_b, s_ArrMenuTopFrontKey[i].color_a);
        adj->FillRectangle(adj, s_ArrMenuTopFrontKey[i].x, s_ArrMenuTopFrontKey[i].y, s_ArrMenuTopFrontKey[i].w, s_ArrMenuTopFrontKey[i].h);
	}
			
	show_menu_items_top_front_key();
	adj->Flip(adj, NULL, DSFLIP_NONE);

}

void show_menu_items_top_front_key(void)
{
	DFBResult err;
	DFBColor color_text, color_status;
	int i;
	
	color_text.a = 0xff;
	color_text.r = 0xff;
	color_text.g = 0xff;
	color_text.b = 0x00;

	DFBCHECK(adj->SetFont(adj, font));
	
	for (i = 0; i < NUM_MENUTopFrontKey_TEXT; i++) 
	{
		
		adj->SetColor(adj, color_text.r, color_text.g, color_text.b, color_text.a);	
		adj->DrawString(adj, (char*)s_ArrItemsMenuTopFrontKey[i].str, -1, s_ArrItemsMenuTopFrontKey[i].x1, s_ArrItemsMenuTopFrontKey[i].y1, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
       // adj->DrawString(adj, (char*)s_ArrTestAllItem[i].str, -1, s_ArrTestAllItem[i].x1, s_ArrTestAllItem[i].y1, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));

		get_color_for_status_text(s_ArrItemsMenuTopFrontKey[i].status, (int *)&color_status.a, (int *)&color_status.r, (int *)&color_status.g, (int *)&color_status.b);
		adj->SetColor(adj, color_status.r, color_status.g, color_status.b, color_status.a);	
		adj->DrawString(adj, (char*)statusText[s_ArrItemsMenuTopFrontKey[i].status], -1, s_ArrItemsMenuTopFrontKey[i].x2, s_ArrItemsMenuTopFrontKey[i].y2, (DFBSurfaceTextFlags)(DSTF_CENTER | DSTF_TOP));
       // adj->DrawString(adj, (char*)statusText[s_ArrTestAllItem[i].status], -1, s_ArrTestAllItem[i].x2, s_ArrTestAllItem[i].y2, (DFBSurfaceTextFlags)(DSTF_CENTER | DSTF_TOP));
    }
}

/**
* 
* <b>FUNCTION: </b>
*
* show_confirm_message()
*
* <b>DESCRIPTION: </b>
* 
* get script file name
* 
* @param int item_id
*   
* @return 
*
*/
char *get_script_file_name(int item_id)
{
	return (char*)s_ArrTestItem[item_id].scriptfile;
}

void set_test_pass_status(int item)
{
	s_ArrTestItem[item].status = STATUS_PASSED;
}

void set_test_failed_status(int item)
{
	s_ArrTestItem[item].status = STATUS_FAILED;
}

void set_menu_top_front_key_pass_status(int item)
{
	s_ArrItemsMenuTopFrontKey[item].status = STATUS_PASSED;
}

void set_menu_top_front_key_failed_status(int item)
{
	s_ArrItemsMenuTopFrontKey[item].status = STATUS_FAILED;
}

int check_test_items_status()
{
	int i;
	for (i = 0; i < NUM_TEST_ITEM; i++) 
	{
		if (s_ArrTestItem[i].status == STATUS_FAILED)
		{
			return FALSE;
		}
    }
	return TRUE;
}

void show_channel_playing(int chan)
{
	if (chan == 1) {
		chan = PLAYING_CHANNEL_01;
	}else{
		chan = PLAYING_CHANNEL_02;
	}

	DFBResult err;
//	int i;

	uninit_gui();
	uninit_osd_layer();

	init_osd_layer(false);
	init_gui();

	
	//draw background 
	//x  = 320 + 800; y = 260 -250; w = 600-500; h =150 - 100 ; 
	adj->SetColor(adj, s_ArrBgConfirmMessage[0].color_r, s_ArrBgConfirmMessage[0].color_g, s_ArrBgConfirmMessage[0].color_b, s_ArrBgConfirmMessage[0].color_a);	
	adj->FillRectangle(adj, 1120, 10, 100, 50);
		
	//x = 325 + 800, y = 265 -250, w = 590 - 500, h = 140 - 100;
	adj->SetColor(adj, s_ArrBgConfirmMessage[1].color_r, s_ArrBgConfirmMessage[1].color_g, s_ArrBgConfirmMessage[1].color_b, s_ArrBgConfirmMessage[1].color_a);	
	adj->FillRectangle(adj, 1125, 15, 90, 40);

	//draw text
	DFBCHECK(adj->SetFont(adj, font));	
	adj->SetColor(adj, s_ArrMessageText[chan].color_r, s_ArrMessageText[chan].color_g, s_ArrMessageText[chan].color_b, s_ArrMessageText[chan].color_a);	
	adj->DrawString(adj, s_ArrMessageText[chan].str, -1, s_ArrMessageText[chan].x,s_ArrMessageText[chan].y, DSTF_BOTTOMCENTER);			
	adj_window->SetOpacity(adj_window, 0xFF);   // show window
	printf("\nShow mess: %s",s_ArrMessageText[chan].str);
}

void show_menu_get_key(void)
{
    show_confirm_message_update_fw(MESSAGE_CONFIRM_GET_KEY);
}

void show_message_starting_get_key(void)
{
    show_message(MESSAGE_STARTING_GET_KEY);
}

void show_message_get_key_successful(void)
{
    show_message(MESSAGE_GET_KEY_SUCCESSFUL);
}

void show_message_get_key_failed(void)
{
    show_message(MESSAGE_GET_KEY_FAILED);
}

void change_get_key_text(void)
{
    strcpy(s_ArrMenuText[3].str, "Get keys is finished"); 
	s_ArrMenuText[3].x = 447;
}

/////////////////////////////////////// end /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


