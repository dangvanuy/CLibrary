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
	//DFBCHECK(gdfb->CreateInputEventBuffer( gdfb, DICAPS_KEYS, DFB_TRUE, &keybuffer ));
	//DFBCHECK(keybuffer->CreateFileDescriptor(keybuffer, &keyfd));

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

	// find out the size of the primary surface
	DFBCHECK(layer->GetConfiguration(layer, &dlcfg));

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
	DFBDisplayLayerConfig dlcfg;
	GUI_INFO("--------------- init_osd_layer -----------------------------\n");
	if (!layer)
	{
		// set up the graphic layer
		DFBCHECK(gdfb->GetDisplayLayer(gdfb, DLID_PRIMARY, &layer));
		DFBCHECK(layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE));
		DFBCHECK(layer->SetBackgroundMode(layer, DLBM_DONTCARE));
		DFBCHECK(layer->EnableCursor(layer, 0));		
	}
	
	dlcfg.flags       = (DFBDisplayLayerConfigFlags)(DLCONF_BUFFERMODE | DLCONF_PIXELFORMAT);
	dlcfg.buffermode  = DLBM_FRONTONLY ;
	if(osd16bit)
	{
		dlcfg.pixelformat = DSPF_ARGB4444;//DSPF_ARGB;
	}
	else
		dlcfg.pixelformat = DSPF_ARGB;//DSPF_ARGB;
	if (layer->SetConfiguration(layer, &dlcfg) != DFB_OK)
	{
		D_ERROR("Could not set OSD in %d bpp mode!!!\n", osd16bit ? 16 : 32);
		return false;
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


