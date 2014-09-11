/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/	


 /**
 * 
 * <b>FUNCTION: </b>
 *
 * global_init(int *argc, char **argv[])
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *       int argc, char *argv[]
 *
 *
*/
void global_init(int *argc, char **argv[])
{
	DFBResult err;
	DFBDisplayLayerConfig dlcfg;
	DFBScreenEncoderConfig enc_cfg;
	DFBScreenOutputSignals signal = DSOS_NONE;
	int i;
	//
	// global initialziations
	//
	//srand((long)time(0));
	DFBCHECK(DirectFBInit( argc, argv ));
	DFBCHECK(DirectFBCreate( &gdfb ));
	DFBCHECK(gdfb->SetCooperativeLevel( gdfb, DFSCL_NORMAL ));

	DFBCHECK(gdfb->GetScreen(gdfb, 0, &screen));
	
	DFBCHECK(gdfb->CreateInputEventBuffer( gdfb, DICAPS_KEYS, DFB_TRUE, &keybuffer ));
	DFBCHECK(keybuffer->CreateFileDescriptor(keybuffer, &keyfd));


	// first initialize the OSD layer in 32bit mode, so that we can blit stuff to the background layer;
	// will change the 16 bpp after that if needed

	init_osd_layer(false);
	
	//init_osd_second_layer(FALSE);
	// get MainVideo layer
	DFBCHECK(gdfb->GetDisplayLayer(gdfb, EM86LAYER_MAINVIDEO, &layer_mv));
	DFBCHECK(layer_mv->SetCooperativeLevel(layer_mv, DLSCL_EXCLUSIVE));

	// get best enabled encoder signal (to compute BG layer resolution)
	for (i=0; i<3; ++i)
	{
		DFBCHECK(screen->GetEncoderConfiguration(screen, i, &enc_cfg));
		if (enc_cfg.out_signals & ~DSOS_EDID)
		{
			signal = (DFBScreenOutputSignals)(enc_cfg.out_signals & ~DSOS_EDID);
			break;
		}
	}
	last_hdtv_signal = signal;

	// find out the size of the primary surface
	DFBCHECK(layer->GetConfiguration(layer, &dlcfg));
	xres = dlcfg.width;
	yres = dlcfg.height;
	// make sure we remember the signal resolution
	get_signal_resolution(signal);	// defaults to (xres,yres)

	set_app_call_back();

	/*
	 * init media
	 */
	//memset(&mediaTab, 0, sizeof(mediaTab));
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * global_uninit(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release directfb
 * 
 * @param
 *       int argc, char *argv[]
 *
 *
*/
void global_uninit(void)
{
	// close the configuration AMP
	if( cfgamp )
	{
		cfgamp->Release( cfgamp );
		cfgamp = NULL;
	}
	disable_background_layer();
	disable_sv_layer();
	
	if (layer_mv)
	{
		layer_mv->Release(layer_mv);
		layer_mv = NULL;
	}
	global_uninit_osd_resources();
	if (layer_so)
	{
		layer_so->Release(layer_so);
		layer_so = NULL;
	}
	if(screen)
	{
		screen->Release( screen );
		screen = NULL;
	}
	if(keybuffer)
	{
		keybuffer->Release( keybuffer );
		keybuffer = NULL;
	}
	if(gdfb)
	{
		gdfb->Release( gdfb );
		gdfb = NULL;
	}
#ifdef DO_DMALLOC
	dmalloc_shutdown();
#endif
	
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * global_init_osd_resources(bool osd16)
 *
 * <b>DESCRIPTION: </b>
 * 
 * create window and surface in osd layer
 * 
 * @param
 *       bool osd16
 *
 *
*/
void global_init_osd_resources(bool osd16)
{	
	DFBResult err;
	//DFBSurfaceDescription sdsc;
	DFBWindowDescription desc;
	if( window_osd1 && surface_osd1 )
	{
		return;
	}
	if(0)//(playInWindow)
	{
		DFBWindowOptions windowOption = DWOP_GHOST | DWOP_INDESTRUCTIBLE | DWOP_SCALE ;
		desc.flags = (DFBWindowDescriptionFlags)(DWDESC_WIDTH | DWDESC_HEIGHT |
												 DWDESC_POSX | DWDESC_POSY |
			                                     DWDESC_CAPS | DWDESC_SURFACE_CAPS | DWDESC_PIXELFORMAT);
		desc.caps = DWCAPS_NODECORATION;
		desc.posx = 0;
		desc.posy = 0;
		desc.width = WIDTH_OF_PRIMARY;//WIDTH_OF_GUI;
		desc.height = HEIGHT_OF_GUI_PRIMARY;//HEIGHT_OF_GUI;
		desc.surface_caps = DSCAPS_VIDEOONLY;
		desc.pixelformat  = DSPF_ARGB;
		if (layer->CreateWindow(layer, &desc, &window_osd1) != DFB_OK)
		{
			GUI_INFO("=========> Could not create window on the primary layer!!! <=========\n");
		}
		GUI_INFO("=====================> USe Window for FLASHLITE <===============================\n");
		window_osd1->SetOptions(window_osd1, windowOption);
	}
	else
	{
		desc.flags  = (DFBWindowDescriptionFlags)(DWDESC_CAPS | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_POSX | DWDESC_POSY | DWDESC_SURFACE_CAPS | DWDESC_PIXELFORMAT);
		desc.caps   = 0;//DWCAPS_ALPHACHANNEL;
		desc.posx = 0;
		desc.posy = 0;
		desc.width = WIDTH_OF_PRIMARY;
		desc.height = HEIGHT_OF_GUI_PRIMARY;
		desc.surface_caps = DSCAPS_VIDEOONLY;
		desc.pixelformat  = DSPF_ARGB;//DSPF_ARGB4444;//DSPF_ARGB;
		DFBCHECK(layer->CreateWindow( layer, &desc, &window_osd1 ) );
		DFBCHECK(window_osd1->SetOptions(window_osd1, DWOP_GHOST));
		DFBCHECK(window_osd1->GetSurface( window_osd1, &surface_osd1 ) );
		DFBCHECK(layer->SetBackgroundColor( layer, 0xFF, 0xFF, 0xFF, 0x00 ));
		DFBCHECK(layer->SetBackgroundMode(layer, DLBM_COLOR));
	}
	window_osd1->SetOpacity( window_osd1, 0xFF);
	
	return ;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * init_osd_layer(bool osd16bit)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release osd layer
 * 
 * @param
 *    bool osd16bit
 * @return
 *    bool
*/
bool init_osd_layer(bool osd16bit)
{
	DFBResult err;
	GUI_INFO("--------------- init_osd_layer -----------------------------\n");
	if (!layer)
	{
		// set up the graphic layer
#ifdef MLB_SUPPORT
		DFBCHECK(gdfb->GetDisplayLayer(gdfb, DLID_PRIMARY, &layer));
#elif  FLASH_TEST_SUPPORT
		DFBCHECK(gdfb->GetDisplayLayer(gdfb, DLID_PRIMARY, &layer));
#else // stagecraft
		#ifdef NO_DTV_SUPPORT
			DFBCHECK(gdfb->GetDisplayLayer(gdfb, DLID_PRIMARY, &layer));
			layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);
		#else
		    // now, we need use EM86LAYER_SECOSD to DTV can show subtitle
			// but when use EM86LAYER_SECOSD, flashlite play the .png files not smooth
		DFBCHECK(gdfb->GetDisplayLayer(gdfb, EM86LAYER_SECOSD, &layer));
			layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);
			DFBScreenMixerConfig mixcfg;
			if (screen) {
				DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
				if (!DFB_DISPLAYLAYER_IDS_HAVE(mixcfg.layers, EM86LAYER_SECOSD))
				{
					// first enable the layer; otherwise we'll freeze the hardware when doing operations on the scaler (e.g. set its surface)
					printf("\n\n\nEnabling the Secondary OSD layer\n");
					mixcfg.flags = DSMCONF_LAYERS;
					DFB_DISPLAYLAYER_IDS_ADD(mixcfg.layers, EM86LAYER_SECOSD);
					while (screen->SetMixerConfiguration(screen, 0, &mixcfg) != DFB_OK)
					{
						printf("\n\n\nCan't enable the Secondary OSD layer now; probably used for stretch blits\n\n");
						usleep(10000);
					}
				}
			}
		#endif
#endif // #ifdef MLB_SUPPORT
	}

	DFBCHECK(layer->GetSurface( layer, &primary));
	DFBCHECK(primary->SetDrawingFlags( primary, DSDRAW_NOFX ));

    return true ;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * global_uninit_osd_resources(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release osd layer
 * 
 * @param
 * 
 *
 *
*/
void global_uninit_osd_resources(void)
{
	#ifndef MLB_SUPPORT
	DFBResult err;	
	if (screen)
	{
		DFBScreenMixerConfig mixcfg;
		DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
		mixcfg.flags = (DFBScreenMixerConfigFlags)(DSMCONF_LAYERS);
		DFB_DISPLAYLAYER_IDS_REMOVE(mixcfg.layers, EM86LAYER_SECOSD);     
		
		DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
	}		
	#endif  // #ifndef MLB_SUPPORT	

	if( surface_osd1 )
	{
		surface_osd1->Release(surface_osd1);
		surface_osd1 = NULL ;
	}
	
	if( window_osd1 )
	{
		window_osd1->Release(window_osd1);
		window_osd1 = NULL; 
	}
	if( surface_osd2 )
	{
		surface_osd2->Release(surface_osd2);
		surface_osd2 = NULL ;
	}

	if( window_osd2 )
	{
		window_osd2->Release(window_osd2);
		window_osd2 = NULL;
	}
	if (primary)
	{
		primary->Release(primary);
		primary = NULL;
	}
	if(layer)
	{
		layer->Release(layer);
		layer = NULL ;
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * init_osd_second_layer(bool osd16bit)
 *
 * <b>DESCRIPTION: </b>
 * 
 * create second osd layey
 * 
 * @param
 *		bool osd16bit
 * @return 
 *      bool
*/
bool init_osd_second_layer(bool osd16bit)
{
	DFBResult err;
	DFBScreenMixerConfig mixcfg;
	DFBWindowDescription desc;
	GUI_INFO("===============> init_osd_second_layer <====================\n");
	
	/*DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
	if (!DFB_DISPLAYLAYER_IDS_HAVE(mixcfg.layers, EM86LAYER_SECOSD))
	{
		// first enable the layer; otherwise we'll freeze the hardware when doing operations on the scaler (e.g. set its surface)
		printf("\n\n\nEnabling the Secondary OSD layer\n");
		mixcfg.flags = DSMCONF_LAYERS;
		DFB_DISPLAYLAYER_IDS_ADD(mixcfg.layers, EM86LAYER_SECOSD);
		while (screen->SetMixerConfiguration(screen, 0, &mixcfg) != DFB_OK)
		{
			printf("\n\n\nCan't enable the Secondary OSD layer now; probably used for stretch blits\n\n");
			usleep(10000);
		}
		
	}*/
	
	if (!layer_so)
	{
		DFBDisplayLayerConfig lcfg;
		printf("\tSetting up GUI on Secondary OSD layer\n");
		DFBCHECK(gdfb->GetDisplayLayer(gdfb, EM86LAYER_SECOSD, &layer_so));
		DFBCHECK(layer_so->SetCooperativeLevel(layer_so, DLSCL_EXCLUSIVE));
		DFBCHECK(layer_so->EnableCursor(layer_so, 0));
		
		lcfg.flags = DLCONF_BUFFERMODE | DLCONF_PIXELFORMAT;
		lcfg.buffermode = DLBM_FRONTONLY/*DLBM_FRONTONLY DLBM_BACKVIDEO*/;
		lcfg.pixelformat = DSPF_ARGB;//DSPF_ARGB4444;//DSPF_ARGB;
		DFBCHECK(layer_so->SetConfiguration(layer_so, &lcfg));
		// update the screen size, it might have been changed by BD-J
		DFBCHECK(layer->GetConfiguration(layer, &lcfg));	
		//			DFBCHECK(layer_so->SetScreenRectangle(layer_so, 320 * lcfg.width / 1920, 0, 1280 * lcfg.width / 1920, 360 * lcfg.height / 1080));
		// resize it starting with the source, in order to avoid maxing out the 
		// scaler bandwidth by forcing the scaling of the entire source into a small destination (bug CT-6776)		
		//DFBCHECK(layer_so->SetSourceRectangle(layer_so, 0, 0, xres, 550));
		//DFBCHECK(layer_so->SetScreenRectangle(layer_so, 0, 0, lcfg.width, 550*lcfg.height/yres/*lcfg.height*4/ 6*/));
		DFBCHECK(layer_so->SetSourceRectangle(layer_so, 0, 0, WIDTH_OF_GUI, HEIGHT_OF_GUI));
		DFBCHECK(layer_so->SetScreenRectangle(layer_so, 0, 0, lcfg.width, lcfg.height));
		
		GUI_INFO("---------------   width of second osd  = %d ---------------\n" ,lcfg.width);
		GUI_INFO("--------------- height of second osd = %d ------------------\n" ,lcfg.height);	
		DFBCHECK(layer_so->SetBackgroundColor( layer_so, 0xFF, 0xFF, 0xFF, 0x00 ));			
		//DFBCHECK(layer_so->SetBackgroundMode(layer_so, DLBM_COLOR));
		// clear the background and keep it that way
		//DFBCHECK(layer_so->SetBackgroundColor(layer_so, 0, 0, 0, 0));
		DFBCHECK(layer_so->SetBackgroundMode(layer_so, DLBM_COLOR));
		if(playInWindow)
		{
			DFBWindowOptions windowOption = DWOP_GHOST | DWOP_INDESTRUCTIBLE | DWOP_SCALE ;
			desc.flags = (DFBWindowDescriptionFlags)(DWDESC_WIDTH | DWDESC_HEIGHT |
				DWDESC_POSX | DWDESC_POSY |
				DWDESC_CAPS | DWDESC_SURFACE_CAPS | DWDESC_PIXELFORMAT);
			desc.caps = DWCAPS_NODECORATION;
			desc.posx = 0;
			desc.posy = 0;
			desc.width = WIDTH_OF_GUI;
			desc.height = HEIGHT_OF_GUI;
			desc.surface_caps = DSCAPS_VIDEOONLY;
			desc.pixelformat  = DSPF_ARGB;
			if (layer_so->CreateWindow(layer_so, &desc, &window_osd1) != DFB_OK)
			{
				GUI_INFO("=========> Could not create window on the primary layer!!! <=========\n");
			}
			GUI_INFO("=====================> USe Window for FLASHLITE <===============================\n");
			window_osd1->SetOptions(window_osd1, windowOption);
		}
		else
		{
			desc.flags  = (DFBWindowDescriptionFlags)(DWDESC_CAPS | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_POSX | DWDESC_POSY | DWDESC_SURFACE_CAPS | DWDESC_PIXELFORMAT);
			desc.flags  = (DFBWindowDescriptionFlags)(DWDESC_CAPS | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_POSX | DWDESC_POSY | DWDESC_SURFACE_CAPS | DWDESC_PIXELFORMAT);
			desc.caps   = 0;//DWCAPS_ALPHACHANNEL;
			desc.posx = 0;
			desc.posy = 0;
			desc.width = WIDTH_OF_GUI;
			desc.height = HEIGHT_OF_GUI;
			desc.surface_caps = DSCAPS_VIDEOONLY;//DSCAPS_VIDEOONLY;
			desc.pixelformat  = DSPF_ARGB;//DSPF_RGB24 ;//DSPF_ARGB4444;//DSPF_ARGB;//DSPF_ARGB;//DSPF_ARGB4444;
			DFBCHECK(layer_so->CreateWindow( layer_so, &desc, &window_osd1 ) );
			DFBCHECK(window_osd1->SetOptions(window_osd1, DWOP_GHOST));
			DFBCHECK(window_osd1->GetSurface( window_osd1, &surface_osd1 ) );
		}
		
		window_osd1->SetOpacity( window_osd1, 0xFF);
		
	}
	DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
	if (!DFB_DISPLAYLAYER_IDS_HAVE(mixcfg.layers, EM86LAYER_SECOSD))
	{
		// first enable the layer; otherwise we'll freeze the hardware when doing operations on the scaler (e.g. set its surface)
		printf("\n\n\nEnabling the Secondary OSD layer\n");
		mixcfg.flags = DSMCONF_LAYERS;
		DFB_DISPLAYLAYER_IDS_ADD(mixcfg.layers, EM86LAYER_SECOSD);
		while (screen->SetMixerConfiguration(screen, 0, &mixcfg) != DFB_OK)
		{
			printf("\n\n\nCan't enable the Secondary OSD layer now; probably used for stretch blits\n\n");
			usleep(10000);
		}

	}
	return true ;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * enable_background_layer(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * create background layer
 * 
 * @param
 *		void
 * @return 
 *      void
*/
void enable_background_layer(void)
{
	DFBResult err;
	DFBDisplayLayerConfig dlcfg;
	static int oldxres = 0;
	static int oldyres = 0;
	//if ((out_xres == oldxres) && (out_xres == oldyres))
	//	return;
	printf("--------------- enable_background_layer ---------------\n");
	if ( bg )
	{
		bg->Release(bg);
		bg = NULL;
	}
	
	if (screen)
	{
		DFBScreenMixerConfig mixcfg;
		DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
		
		if (!DFB_DISPLAYLAYER_IDS_HAVE(mixcfg.layers, EM86LAYER_BKGND))
		{
			mixcfg.flags = (DFBScreenMixerConfigFlags)(DSMCONF_LAYERS);
			DFB_DISPLAYLAYER_IDS_ADD(mixcfg.layers, EM86LAYER_BKGND);     // enable Background	
			DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));			
		}
	}
	
	if ( !layer_bg )
	{
		DFBCHECK(gdfb->GetDisplayLayer(gdfb, EM86LAYER_BKGND, &layer_bg));
		printf("------------- GetDisplayLayerBackground ------------------ \n");
		DFBCHECK(layer_bg->SetCooperativeLevel(layer_bg, DLSCL_ADMINISTRATIVE)); // Must set admin first, otherwise level remains EXCLUSIVE but state may not be realized
		DFBCHECK(layer_bg->SetCooperativeLevel(layer_bg, DLSCL_EXCLUSIVE));	
	}
	//DFBCHECK(layer_bg->SetCooperativeLevel(layer_bg, DLSCL_ADMINISTRATIVE)); // Must set admin first, otherwise level remains EXCLUSIVE but state may not be realized
	//DFBCHECK(layer_bg->SetCooperativeLevel(layer_bg, DLSCL_EXCLUSIVE));
	dlcfg.flags   = (DFBDisplayLayerConfigFlags)(DLCONF_WIDTH | DLCONF_HEIGHT );
	dlcfg.width   = out_xres;
	dlcfg.height  = out_yres;
	printf("x =%d         , y= %d\n" , out_xres ,out_yres);
	DFBCHECK(layer_bg->SetConfiguration(layer_bg, &dlcfg));
	
	if( !bg )
	{
		if(0)
		{	
			if( !window_bg )
			{
				DFBWindowDescription desc;
				
				desc.flags  = (DFBWindowDescriptionFlags)(DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_POSX | DWDESC_POSY );
				desc.posx = 0;
				desc.posy = 0;
				desc.width = out_xres;
				desc.height = out_yres;
				DFBCHECK(layer_bg->CreateWindow( layer_bg, &desc, &window_bg ) );
			}
			else
			{
				window_bg->Resize(window_bg ,out_xres , out_yres);
			}
			
			DFBCHECK(window_bg->GetSurface( window_bg, &bg ) );
			window_bg->SetOpacity( window_bg, 0x00);
		}
		else if(1)
		{
			DFBCHECK(layer_bg->GetSurface(layer_bg, &bg));
		}
	}
	useBackground = TRUE ;
	draw_image_or_text(bg, fileNameImageBackGround);
	
	//if(window_bg)
	//	window_bg->SetOpacity( window_bg, 0xFF);
	printf("<<<<<<<<<<<<<<<<<<<<<  size of picture = %d  >>>>>>>>>>>>>>>>>>>>>>>>>\n", sizeof(bg));
	oldxres = out_xres;
	oldyres = out_yres;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * disable_background_layer(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release background layer
 * 
 * @param
 *		void
 * @return 
 *      void
*/
void disable_background_layer(void)
{
	DFBResult err;
    DFBScreenMixerConfig mixcfg;
	
	//	D_ERROR("\n\n\n\n\n\ndisable_background_layer\n\n\n\n\n\n");
	if ( bg )
	{
		bg->Release(bg);
		bg = NULL;
	}
	if( window_bg )
	{
		window_bg->Release(window_bg);
		window_bg  = NULL;
	}
    if ( layer_bg )
    {
        layer_bg->Release(layer_bg);
        layer_bg = NULL;
    }
	
	if(screen)
	{
		DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
		if (DFB_DISPLAYLAYER_IDS_HAVE(mixcfg.layers, EM86LAYER_BKGND))
		{
			GUI_INFO("------------- disable_background_layer -------------------\n");
			mixcfg.flags = (DFBScreenMixerConfigFlags)(DSMCONF_LAYERS);
			DFB_DISPLAYLAYER_IDS_REMOVE(mixcfg.layers, EM86LAYER_BKGND);     // disable Background
			DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
		}
		
	}
	useBackground = FALSE ;
	
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * disable_secosd_gui(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release second osd layer
 * 
 * @param
 *		void
 * @return 
 *      RMbool
*/
RMbool disable_secosd_gui(void)
{
	if (gui_on_secosd)
	{	
		if (gui_ref_count)
		{
			DFBResult err;
			DFBScreenMixerConfig mixcfg;
			DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
			if (DFB_DISPLAYLAYER_IDS_HAVE(mixcfg.layers, EM86LAYER_SECOSD))
			{
				if( surface_osd1 )
				{
					surface_osd1->Release(surface_osd1);
					surface_osd1 = NULL ;
					GUI_INFO("=================> release surface on second osd <=================\n");
				}
				if( window_osd1 )
				{
					window_osd1->Release(window_osd1);
					window_osd1 = NULL; 
					printf("=================> release window on second osd <===================\n");
				}
				if(layer_so)
				{
					layer_so->Release(layer_so);
					layer_so = NULL ;
					printf("=======> release layer second osd <=============\n");
				}
				printf("\n\n\nDisabling the Secondary OSD layer\n\n\n");
				mixcfg.flags = DSMCONF_LAYERS;
				DFB_DISPLAYLAYER_IDS_REMOVE(mixcfg.layers, EM86LAYER_SECOSD);
				DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
			}

			gui_ref_count = 0;
		}
		return true;
		
	}
	/*else
	{
		printf("disable_gui    nogui_on_secosd >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		// pull up tent from Primary OSD
		//printf("\tPulling down GUI on Primary OSD layer\n\n\n");
		if( surface_osd1 )
		{
			surface_osd1->Release(surface_osd1);
			surface_osd1 = NULL ;
		}
		
		if( window_osd1 )
		{
			window_osd1->Release(window_osd1);
			window_osd1 = NULL; 
		}
		gui_ref_count = 0;
	}*/
	return false;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * enable_secosd_gui(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * enable second osd layer
 * 
 * @param
 *		void
 * @return 
 *      RMbool
*/
bool enable_secosd_gui(void)
{
	if ( !gui_on_secosd )
	{																		
		printf("------------- enable_secosd_gui -------------------\n");
		if (gui_ref_count)
		{
			//disable_gui();
			if( surface_osd1 )
			{
				surface_osd1->Release(surface_osd1);
				surface_osd1 = NULL ;
				printf("--------------- surface_osd1 = NULL    -----------------\n");
			}
			
			if( window_osd1 )
			{
				window_osd1->Release(window_osd1);
				window_osd1 = NULL; 
			}
			gui_ref_count = 0;
			gui_on_secosd = true;
			
			return true; // gui needs refreshing
		}
		
		gui_on_secosd = true;
	}
	
	return false;	// gui does not need refreshing
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * disable_gui(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release second osd layer or surface and window osd layer
 * 
 * @param
 *		void
 * @return 
 *      RMbool
*/
void disable_gui(void)
{
	if (gui_on_secosd)
	{
		GUI_INFO("--------------- disable_gui  on second osd ---------------------------\n");
		disable_secosd_gui();
	}
	else
	{
		//DFBResult err;
		GUI_INFO("----------------- disable_gui on osd ------------------------------\n");
		// pull up tent from Primary OSD
		//printf("\tPulling down GUI on Primary OSD layer\n\n\n");
#if 1
		if( surface_osd1 )
		{
			surface_osd1->Release(surface_osd1);
			surface_osd1 = NULL ;
		}
		
		if( window_osd1 )
		{
			window_osd1->Release(window_osd1);
			window_osd1 = NULL; 
		}
		gui_ref_count = 0;
#else		
		primary->Release( primary);
		
		DFBCHECK(layer->GetSurface( layer, &primary));
		
		DFBCHECK(primary->SetDrawingFlags( primary, DSDRAW_NOFX ));
#endif
		
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * enable_gui(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * create second osd layer or surface and window osd layer
 * 
 * @param
 *		void
 * @return 
 *      RMbool
*/
void enable_gui(void)
{
	if (gui_on_secosd)
	{
		GUI_INFO("----------------- enable_gui on second osd -------------------------\n");
		if (!gui_ref_count)
		{
			init_osd_second_layer(false);
		}
		
	}
	else
	{
		GUI_INFO("--------------- enable_gui on osd -----------------------------\n");
		if (!gui_ref_count)
		{
#if 1
			global_init_osd_resources(false);
#endif
		}
		
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * enable_sv_layer(int width, int height)
 *
 * <b>DESCRIPTION: </b>
 * 
 * create second video layer
 * 
 * @param
 *		int width, int height
 * @return 
 *      void
*/
void enable_sv_layer(int width, int height)
{
	DFBResult err;
	DFBDisplayLayerConfig dlcfg;
	
	if (sv)
	{
		sv->Release(sv);
		sv = NULL;
	}
	
	if (!layer_sv)
	{
		DFBCHECK(gdfb->GetDisplayLayer(gdfb, EM86LAYER_SECVIDEO, &layer_sv));
		DFBCHECK(layer_sv->SetCooperativeLevel(layer_sv, DLSCL_EXCLUSIVE));
	}
	
	// put the layer in surface mode and set its size
	dlcfg.flags = (DFBDisplayLayerConfigFlags)(DLCONF_WIDTH | DLCONF_HEIGHT | DLCONF_SOURCE);
	dlcfg.width = width;
	dlcfg.height = height;
	dlcfg.source = DLSID_SURFACE;
	DFBCHECK(layer_sv->SetConfiguration(layer_sv, &dlcfg));
	
	DFBCHECK(layer_sv->GetSurface(layer_sv, &sv));
	
	if (screen)
	{
			DFBScreenMixerConfig mixcfg;
			DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
			mixcfg.flags = (DFBScreenMixerConfigFlags)(DSMCONF_LAYERS);
			DFB_DISPLAYLAYER_IDS_ADD(mixcfg.layers, EM86LAYER_SECVIDEO);     // disable SecVideo  
			DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * disable_sv_layer(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release second video layer
 * 
 * @param
 *		int width, int height
 * @return 
 *      void
*/
void disable_sv_layer(void)
{
    DFBResult err;
	DFBDisplayLayerConfig dlcfg;
	
	if (sv)
	{
		sv->Release(sv);
		sv = NULL;
	}
	
    if (layer_sv)
    {
		// put the layer in video mode to destroy its surface
		dlcfg.flags = (DFBDisplayLayerConfigFlags)(DLCONF_SOURCE);
		dlcfg.source = 2;
		DFBCHECK(layer_sv->SetConfiguration(layer_sv, &dlcfg));
		
		layer_sv->Release(layer_sv);
        layer_sv = NULL;
    }
	GUI_INFO("====================> release second video <===================\n");
	if (screen)
	{
		DFBScreenMixerConfig mixcfg;
		DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
		
		mixcfg.flags = (DFBScreenMixerConfigFlags)(DSMCONF_LAYERS);
		DFB_DISPLAYLAYER_IDS_REMOVE(mixcfg.layers, EM86LAYER_SECVIDEO);     // disable SecVideo
		
		DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
	}	
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetPriorityForApp(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *		void
 * @return 
 *      void
*/
void set_priority_for_app(void)
{	
	// since we handle the input, we must have high priority
	struct sched_param spar;
	int policy;
	if (!pthread_getschedparam(pthread_self(), &policy, &spar))
	{
		D_INFO("amp_test initial settings: pid = %d, policy = %s, priority %d\n",
			getpid(), policy == SCHED_RR ? "RR" : (policy == SCHED_FIFO ? "FIFO" : "OTHER"), spar.sched_priority);
	}
#if REAL_TIME_THREADS_DEFINED
	
#ifndef PRIORITY_BOOST
#define PRIORITY_BOOST	0
#endif
	
	policy = REAL_TIME_THREADS_DEFINED ? SCHED_RR : SCHED_OTHER;
    spar.sched_priority = (sched_get_priority_max(policy) - sched_get_priority_min(policy))/2 + PRIORITY_BOOST;
	D_INFO("amp_test requested settings: policy = %s, priority %d\n",
		policy == SCHED_RR ? "RR" : (policy == SCHED_FIFO ? "FIFO" : "OTHER"), spar.sched_priority);
	
    if (!pthread_setschedparam(pthread_self(), REAL_TIME_THREADS_DEFINED ? SCHED_RR : SCHED_OTHER, &spar))
    {
		pthread_getschedparam(pthread_self(), &policy, &spar);
		D_INFO("amp_test final settings: pid = %d, policy = %s, priority %d\n",
			getpid(), policy == SCHED_RR ? "RR" : (policy == SCHED_FIFO ? "FIFO" : "OTHER"), spar.sched_priority);
    }
    else
    {
		D_ERROR("Could not change amp_test's priority settings!\n");
    }
#endif
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *		handle_priority_for_app(int action)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *		void
 * @return 
 *      void
*/
void handle_priority_for_app(int action)
{
#if REAL_TIME_THREADS_DEFINED
	
	static bool firstTime = true;
	static struct sched_param spar;
	static int temp_priority;
	if(action == BACKUP_PRIORITY_AND_SET_PRIORITY)
	{
		if (firstTime)
		{
			// get the default parameters
			int policy;
			
			if (!pthread_getschedparam(pthread_self(), &policy, &spar))
			{
				// initial settings acquired successfuly
				firstTime =false;
				
				// set the temporary priority just below DCC-HD
#ifndef PRIORITY_BOOST
#define PRIORITY_BOOST	0
#endif
				temp_priority = (sched_get_priority_max(SCHED_RR) - sched_get_priority_min(SCHED_RR))/2 + PRIORITY_BOOST - 1;
				
			}
			else
			{
				D_ERROR("Could not acquire priority before info display!\n");
			}
		}
		
		if (!firstTime)
		{
			struct sched_param lspar;
			
			lspar.sched_priority = temp_priority;
			if (pthread_setschedparam(pthread_self(), SCHED_RR, &lspar))
			{
				D_ERROR("Could not lower priority for info display!\n");
			}
		}
	}
	else if(action == BACKUP_PRIORITY_AND_SET_PRIORITY_FOR_CDDA)
	{
		if (firstTime)
		{
			// get the default parameters
			int policy;
			
			if (!pthread_getschedparam(pthread_self(), &policy, &spar))
			{
				// initial settings acquired successfuly
				firstTime =false;
				
				// set the temporary priority just below DCC-HD
#ifndef PRIORITY_BOOST
#define PRIORITY_BOOST	0
#endif
				temp_priority = 20;//(sched_get_priority_max(SCHED_RR) - sched_get_priority_min(SCHED_RR))/2 + PRIORITY_BOOST - 1;
				
			}
			else
			{
				D_ERROR("Could not acquire priority before info display!\n");
			}
		}
		
		if (!firstTime)
		{
			struct sched_param lspar;
			
			lspar.sched_priority = temp_priority;
			if (pthread_setschedparam(pthread_self(), SCHED_RR, &lspar))
			{
				D_ERROR("Could not lower priority for info display!\n");
			}
		}
	}
	else if (action == RESTORE_PRIORITY)
	{
#if REAL_TIME_THREADS_DEFINED
		if (!firstTime)
		{
			if (pthread_setschedparam(pthread_self(), SCHED_RR, &spar))
			{
				D_ERROR("Could not restore priority after info display!\n");
			}
		}
#endif
	}
		
#endif	

}
