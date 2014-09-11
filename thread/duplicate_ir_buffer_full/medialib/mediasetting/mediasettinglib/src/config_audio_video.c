/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/	


 /**
 * 
 * <b>FUNCTION: </b>
 *
 *			processConfigPreview(const struct RUA *pRUA,
 *								const struct SContentInfo *content,
 *								const struct rmcci *drm,
 *								const struct SMRUAOutputConfig *settings,
 *								const struct SAdjustment *adjustments,
 *								struct SMRUAConfig *configuration)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *					const struct RUA *pRUA,
 *					const struct SContentInfo *content,
 *					const struct rmcci *drm,
 *					const struct SMRUAOutputConfig *settings,
 *					const struct SAdjustment *adjustments,
 *					struct SMRUAConfig *configuration)
 * @return 
 *      bool
*/
RMbool  isSetHDBitStream = FALSE;
RMbool BDMain_startup = FALSE;
RMbool isNeedToSet = TRUE;
RMuint8	uiTVMode = TVSYS_PAL;

bool process_config_preview(const struct RUA *pRUA,
						  const struct SContentInfo *content,
						  const struct rmcci *drm,
						  const struct SMRUAOutputConfig *settings,
						  const struct SAdjustment *adjustments,
						  struct SMRUAConfig *configuration)
{
	
	//***************** this is code for FPT probject 
	// it kill preview play function
	// you can delete this code when write your app
	
	//if(onActualSize) {
	//	printf("\n\n\n\n\n----- process_config_preview changed, it kill preview play function  --- \n\n\n\n");
	//	change_actual_size(pRUA,content,drm,settings,adjustments,configuration);
	//}
	
	//if(onFramerateSync != 0) {
	//	printf("\n\n\n\n\n----- process_config_preview changed, it kill preview play function  --- \n\n\n\n");
	//	change_framerate_sync(pRUA,content,drm,settings,adjustments,configuration);
	//	
	//}
	
	//******************************************************************

	int i;
	const struct SVideoOutConfigModule *pSettings = &settings->video.common.composite;
	struct SVideoOutConfigModule *pConfiguration = &configuration->output.video.common.composite;

	check_hotplug_hdmi_cable();	
	if (ampGlobals->shared->configuration.output.video.common.hdmi.status == CS_Connected || ampGlobals->shared->configuration.output.video.common.hdmi.status == CS_ConnectedSecure)
	{
		GUI_INFO("====================== output.video.common.hdmi.status =  CS_Connected ===================\n");
		pSettings = &settings->video.common.hdmi;
		pConfiguration = &configuration->output.video.common.hdmi;

		GUI_INFO("--------------- hotplug %d -----------------\n",(int)isHDMIHotPlug);
		if(isHDMIHotPlug)
		{
			isHDMIHotPlug = FALSE;

			isResetResolution = TRUE;
			if(ValueOfResolution > get_index_of_resolution(pSettings->display.preferred_signal))
			{	
				GUI_INFO("--------------- Reset HDMI ---------------\n");
				ValueOfResolution = TVMODE_NONE;
			}
			else
			{
				GUI_INFO("----------------- HOLD OLD HDMI ---------------------------\n");
			}

		} else 
		if(PermissionResetResolution && !BDMain_startup)
		{
#ifdef USER_KNOWS_BEST_HDMI_OUT // force hdmi signal output
			if(*PermissionResetResolution == USER_IS_PERMIT_TO_SET_RESOLUTION  /*&& PermissionForceSignal*/)
			{
				GUI_INFO("------------------- INTO HDMI -------------------------\n");
				switch(ValueOfResolution) {
				case TVMODE_480I_576I:
					GUI_INFO("======================== TVMODE_480I_576I ===========================\n");
					pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
					break;
				case TVMODE_480P_576P:
					GUI_INFO("======================== TVMODE_480P_576P ===========================\n");
					pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_EDTV);
					break;
				case TVMODE_720P:
					GUI_INFO("======================== TVMODE_720P ===========================\n");
					pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_720p);
					break;
				case TVMODE_1080I:
					GUI_INFO("======================== TVMODE_1080I ===========================\n");
					pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080i);
					break;
				case TVMODE_1080P:
					GUI_INFO("======================== TVMODE_1080P ===========================\n");
					pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080p);
					//last_hdtv_signal = DSOS_1080p;
					break;
				case TVMODE_1080P24:
					GUI_INFO("======================== TVMODE_1080P24 ===========================\n");
					pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080p24);
					//last_hdtv_signal = DSOS_1080p24;
					break;
				}
				//isResetResolution =FALSE ; // do not redraw BG_LAYER
			} else 
#endif /* USER_KNOWS_BEST_HDMI_OUT */
			if(*PermissionResetResolution == AUTO_DETECT_HDMI)
			{
				pConfiguration->display.signal = pSettings->display.preferred_signal;//ampGlobals->shared->configuration.output.video.common.hdmi.display.preferred_signal;
			}
		}
	}
	else if  (configuration->output.video.common.component.status == CS_Connected || configuration->output.video.common.component.status == CS_ConnectedSecure)
	{
		GUI_INFO("=============================== output.video.common.component.status  ==============================\n");
		pSettings = &settings->video.common.component;
		pConfiguration = &configuration->output.video.common.component;
	}


#ifdef SOURCE_DIRECT_HDMI_OUT
	if(pIsAutoDetectSourceForTvMode && *pIsAutoDetectSourceForTvMode == 2) // auto detect source direct for TV mode
	{
		set_source_direct_for_tvmode(content,settings,configuration,true,true);	
	}
#endif /* SOURCE_DIRECT_HDMI_OUT */


	for (i = 0; i < mediaCnt; i++)
	{
#ifndef NO_RED_SUPPORT
		struct SMediaSession *media = &mediaTab[i];

		if (media->running && media->msRT && media->msRT->mediaSpace == MEDIA_SPACE_DVD)
		{
			struct SDVDCommand cmd = { { sizeof(cmd) } };
			cmd.cmd = DVDCmd_Video_Mode_Change;

			switch (media->status.dvd.video)
			{
				case RM_DVD_VIDEO_16_9__4_3_PANSCAN:
				case RM_DVD_VIDEO_16_9__4_3_LETTERBOX:
				case RM_DVD_VIDEO_4_3__4_3_RECT:
					if (pConfiguration->display.aspectRatio == TVAR_16x9) // navigator is in 4:3 mode but the display is 16:9
						cmd.param1.videoMode = RM_DVD_VIDEO_4_3__16_9_ZOOM_OFF;

					break;

				case RM_DVD_VIDEO_4_3__16_9_ZOOM_ON:
				case RM_DVD_VIDEO_4_3__16_9_ZOOM_OFF:
				case RM_DVD_VIDEO_16_9__16_9_RECT:
					if (pConfiguration->display.aspectRatio == TVAR_4x3) // navigator is in 16:9 mode but the display is 4:3
						cmd.param1.videoMode = pConfiguration->arMatch == ARM_PanScan ? RM_DVD_VIDEO_16_9__4_3_PANSCAN : RM_DVD_VIDEO_16_9__4_3_LETTERBOX;

					break;
			}

			if (cmd.param1.videoMode) // a change is needed
			{
				if (media->status.dvd.permissions & 0x01000000)
				{
					// Video_Mode_Change command is prohibited, we'll have to change the display instead

					if (pConfiguration->display.aspectRatio == TVAR_4x3)
					{
						// 16:9 configurations are supported for all outputs, in all resolutions
						pConfiguration->display.aspectRatio = TVAR_16x9;
						pConfiguration->arMatch = pSettings->arMatch == ARM_No ? ARM_No : ARM_SidePanel;
					}
					else
					{
						// HDMI doesn't support 4:3 in hi-def modes (720, 1080)
						if (pSettings->display.aspectRatio != TVAR_Unknown && pSettings->display.aspectRatio != TVAR_4x3)
						{
							char* chosen_signal;

							// force a video mode change - we'll use EDTV because it works in 4:3 mode (or SDTV if EDTV is unavailable)
							if (pSettings->display.signal & TV_SIGNAL_TO_BITMASK(TVS_EDTV))
							{
								pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_EDTV);
								chosen_signal = "EDTV";
							}
							else
							{
								pConfiguration->display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
								chosen_signal = "SDTV";
							}

							// SD modes support only YUV601
							pConfiguration->display.colorSpace = CS_YUV_601;

							D_INFO("\033[31m[configPreview] DVD navigator wants a 4:3 display; switching output to %s !\033[0m\n",
								   chosen_signal);
						}
						pConfiguration->display.aspectRatio = TVAR_4x3;
						pConfiguration->arMatch = pSettings->arMatch == ARM_PanScan ? ARM_PanScan : ARM_LetterBox;
					}
				}
				else
					media->amp->PostPresentationCmd(media->amp, (struct SCommand *) &cmd);
			}
		}
#endif
	}
	if (showcfginfo)
		return ConfigPreview(pRUA, content, drm, settings, adjustments, configuration);
	else
		return true;
}
 /**
 * 
 * <b>FUNCTION: </b>
 *
 *			processConfigConfirm(const struct RUA *pRUA,
 *								const struct SContentInfo *content,
 *								const struct SMRUAConfig *configuration)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *				const struct RUA *pRUA,
 *				const struct SContentInfo *content,
 *				const struct SMRUAConfig *configuration
 * @return 
 *      void
*/
bool process_config_confirm(const struct RUA *pRUA,
				  		  const struct SContentInfo *content,
				  		  const struct SMRUAConfig *configuration)
{
	GUI_INFO("=================  processConfigConfirm >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	//printf("hdmi status = %d\n", configuration->output.video.common.hdmi.status);
	printf("hdmi status = %d\n", ampGlobals->shared->configuration.output.video.common.hdmi.status);
	if (ampGlobals->shared->configuration.output.video.common.hdmi.status == CS_Connected || ampGlobals->shared->configuration.output.video.common.hdmi.status == CS_ConnectedSecure)
	{
		if (isDolbyChannel == TRUE) {
			GUI_INFO("====================== SetVolumeNoMSPForDolby ===================\n");
			SetVolumeNoMSPForDolby(m_uiVolume);
		}
		else {
		GUI_INFO("====================== SetVolumeNoMSP ===================\n");
			SetVolumeNoMSP(m_uiVolume);
		}
	}
	if (ampGlobals->shared->configuration.output.video.common.hdmi.status == CS_Disconnected)
	{
/*
		//thinh add
		if (runGettime == 0) {
			pthread_create( &threadGettime, NULL, get_time_thread_proc, NULL);
			runGettime = 1;
		}
		//thinh end
*/
		if (isDolbyChannel == TRUE) {
			SetVolumeNoMSPForDolby(m_uiVolume);
		}
		else {
		//SetVolumeNoMSP(m_uiVolume);
		}
	}

	if (showcfginfo)
		return ConfigConfirm(pRUA, content, configuration);
	else
		return true;
}
 /**
 * 
 * <b>FUNCTION: </b>
 *
 * SetSourceDirectForTVMode(const struct SContentInfo *content,
 *							const struct SMRUAOutputConfig *settings,
 *							struct SMRUAConfig *configuration,bool is_hdmi,bool is_component)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *       const struct SContentInfo *content,
 *							const struct SMRUAOutputConfig *settings,
 *							struct SMRUAConfig *configuration,bool is_hdmi,bool is_component
 * @return 
 *      void
*/
void set_source_direct_for_tvmode(const struct SContentInfo *content,
							  const struct SMRUAOutputConfig *settings,
							  struct SMRUAConfig *configuration,bool is_hdmi,bool is_component)
{
#if 1
	if(amp)
	{
			int i = 0;
			// enable for 'content direct' video mode
			for (i=0; i</*configuration->video.decoderCnt*/RMCFG_VIDEO_DECODER_CNT; i++)
			{
				if ( ((int)configuration->video.decoder[i].decoderID > 0) && (content[configuration->video.decoder[i].contentIndex].format.formatValid) )
				{
					// select TV standard
					if (content[configuration->video.decoder[i].contentIndex].format.format.image.rateN)
					{
						int rateM = content[configuration->video.decoder[i].contentIndex].format.format.image.rateM;
						int rateN = content[configuration->video.decoder[i].contentIndex].format.format.image.rateN;

						if ((rateM * 1001 == rateN * 30000 || rateM == rateN * 30 || rateM * 1001 == rateN * 60000 || rateM == rateN * 60) &&
							(settings->video.common.hdmi.display.system & TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60)) != 0)
						{
							printf("\n\n\nForced HDTV60 mode for %5.2fHz content !!!\n\n\n", (float)rateM / rateN);
							if(is_hdmi)
							{
								configuration->output.video.common.hdmi.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60);
							}
							if(is_component)
							{
								configuration->output.video.common.component.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60);
							}	
							configuration->output.video.common.composite.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_NTSC);
							configuration->output.video.common.composite.display.signal = TV_SIGNAL_TO_BITMASK(TVS_NTSC);
						}
						else if ((rateM == rateN * 25 || rateM == rateN * 50) &&
								 (settings->video.common.hdmi.display.system & TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50)) != 0)
						{
							printf("\n\n\nForced HDTV60 mode for %5.2fHz content !!!\n\n\n", (float)rateM / rateN);
							if(is_hdmi)
							{
								configuration->output.video.common.hdmi.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50);
							}
							if(is_component)
							{
								configuration->output.video.common.component.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50);
							}
							configuration->output.video.common.composite.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_PAL);
							configuration->output.video.common.composite.display.signal = TV_SIGNAL_TO_BITMASK(TVS_PAL);
						}
					}

					// select resolution
					if (content[configuration->video.decoder[i].contentIndex].format.format.image.height < 720)
					{
						if (content[configuration->video.decoder[i].contentIndex].format.format.image.interlaced)
						{
			    			if ((configuration->output.video.common.hdmi.status != CS_Disconnected && configuration->output.video.common.hdmi.display.signal != TV_SIGNAL_TO_BITMASK(TVS_SDTV)) ||
								configuration->output.video.common.component.display.signal != TV_SIGNAL_TO_BITMASK(TVS_SDTV))
			    			{
			    				printf("\n\n\nForced SDTV mode for interlaced SD format!!!\n\n\n");
								if(is_hdmi)
								{
									//configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
									configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_EDTV);
									configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_601;									
								}
			    				if(is_component)
								{
									configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
									configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
								}
								goto ResetResolutionForGui;
			    			}
						}
						else
						{
							if ((configuration->output.video.common.hdmi.status != CS_Disconnected && configuration->output.video.common.hdmi.display.signal != TV_SIGNAL_TO_BITMASK(TVS_EDTV)) ||
								configuration->output.video.common.component.display.signal != TV_SIGNAL_TO_BITMASK(TVS_EDTV))
			    			{
			    				printf("\n\n\nForced EDTV mode for progressive SD format!!!\n\n\n");
								if(is_hdmi)
								{
									configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_EDTV);
									configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_601;
								}
			    				else if (is_component)
								{
									configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_EDTV);
									configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
								}
								goto ResetResolutionForGui;
			    			}
						}
					}
					else if (!content[configuration->video.decoder[i].contentIndex].format.format.image.interlaced &&
		    				 (settings->video.common.hdmi.display.signal & TV_SIGNAL_TO_BITMASK(TVS_1080p)) &&
		    				 content[configuration->video.decoder[i].contentIndex].format.format.image.rateN &&
		    		 			(content[configuration->video.decoder[i].contentIndex].format.format.image.rateM /
		    		 			 content[configuration->video.decoder[i].contentIndex].format.format.image.rateN > 30))
					{
		    			if (configuration->output.video.common.component.display.signal != TV_SIGNAL_TO_BITMASK(TVS_1080p))
		    			{
		    				printf("\n\n\nForced 1080p mode for progressive HD format!!!\n\n\n");
							if(is_hdmi)
							{
								configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080p);
								configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_709;
							}
		    				if(is_component)
							{
								configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
								configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
							}
							goto ResetResolutionForGui;
		    			}
					}
					else if (!content[configuration->video.decoder[i].contentIndex].format.format.image.interlaced &&
		    				 (settings->video.common.hdmi.display.signal & TV_SIGNAL_TO_BITMASK(TVS_1080p24)) &&
		    				 content[configuration->video.decoder[i].contentIndex].format.format.image.rateN &&
		    		 			(content[configuration->video.decoder[i].contentIndex].format.format.image.rateM /
		    		 			 content[configuration->video.decoder[i].contentIndex].format.format.image.rateN < 30))
					{
		    			if (configuration->output.video.common.component.display.signal != TV_SIGNAL_TO_BITMASK(TVS_1080p24))
		    			{
		    				printf("\n\n\nForced 1080p24 mode for film HD format!!!\n\n\n");
							if(is_hdmi)
							{
								configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080p24);
								configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_709;
							}
							if(is_component)
							{
								configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
								configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
							}
							goto ResetResolutionForGui;
		    			}
					}
#if 0
					else if (!content[configuration->video.decoder[i].contentIndex].format.format.image.interlaced &&
						(settings->video.common.hdmi.display.signal & TV_SIGNAL_TO_BITMASK(TVS_720p)) &&
						content[configuration->video.decoder[i].contentIndex].format.format.image.rateN &&
						(content[configuration->video.decoder[i].contentIndex].format.format.image.rateM /
						content[configuration->video.decoder[i].contentIndex].format.format.image.rateN > 30))
					{
						printf("\n\n\nForced 720p mode for interlaced HD format!!!\n\n\n");
						if(is_hdmi)
						{
							configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_720p);
							configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_709;
						}
						if(is_component)
						{
							configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_720p);
							configuration->output.video.common.component.display.colorSpace = CS_YUV_709;
						}
						goto ResetResolutionForGui;
					}
#endif
					else
					{
		    			if (configuration->output.video.common.component.display.signal != TV_SIGNAL_TO_BITMASK(TVS_1080i))
		    			{
		    				printf("\n\n\nForced 1080i mode for interlaced HD format!!!\n\n\n");
							if(is_hdmi)
							{
								configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080i);
								configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_709;
							}
							if(is_component)
							{
								configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080i);
								configuration->output.video.common.component.display.colorSpace = CS_YUV_709;
							}
							goto ResetResolutionForGui;
		    			}
					}
				}
			}
	}
#endif
	return;
ResetResolutionForGui:
	{
		DFBScreenOutputSignals tempSignal = DSOS_NONE;
		tempSignal = is_hdmi ? configuration->output.video.common.hdmi.display.signal : is_component ? configuration->output.video.common.component.display.signal : DSOS_NONE;
		if(tempSignal != DSOS_NONE )
		{
			if(pCurrentResolution)
			{
				RMuint8 tempIndexResolution= get_index_of_resolution(tempSignal);
				if( *pCurrentResolution != tempIndexResolution)
				{
					GUI_INFO("=============  Resolution is change in source direct for TV mode =======================\n");
					*pCurrentResolution = tempIndexResolution; // update Value of resolution on GUI.
					last_hdtv_signal = tempSignal; // updater signal for resolution
					get_signal_resolution(last_hdtv_signal); // get value of resolution.
				}
	
			}
		}
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * RMstatus set_hdtv_resolution(DFBScreenOutputSignals signal)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *       DFBScreenOutputSignals signal
 * @return 
 *      RMstatus
*/
RMstatus set_hdtv_resolution(DFBScreenOutputSignals signal)
{
	RMstatus status = RM_OK;
	DFBScreenEncoderConfig enc_cfg;
	DFBScreenOutputConfig out_cfg;
	bool hdmi = ampGlobals->shared->settings.video.common.hdmi.status != CS_Disconnected;
	bool component = ampGlobals->shared->settings.video.common.component.status != CS_Disconnected;
	bool analog = ampGlobals->shared->settings.video.common.composite.status != CS_Disconnected;

	// TEMPORARY - hack around RUA NTSC Zoom problem
	if (signal == DSOS_NONE || signal == last_hdtv_signal)
		return RM_ERROR;		// don't mute anymore
	// end TEMPORARY
	//D_INFO("\n\n   set_hdtv_resolution(DFBScreenOutputSignals signal) ! \n\n"); 
	if (hdmi)
	{
		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG);
		enc_cfg.out_signals = signal;
		enc_cfg.delayed_config = true;
		if (screen->SetEncoderConfiguration(screen, 0, &enc_cfg) != DFB_OK)
		{
			D_ERROR("SetEncoderConfiguration(0) failed!\n");
			component = false;
			analog = false;
		}
		else
		{
			out_cfg.flags = (DFBScreenOutputConfigFlags)(DSOCONF_SIGNALS | DSOCONF_DELAYED_CONFIG);
			out_cfg.out_signals = signal;
			out_cfg.delayed_config = component || analog;
			if (screen->SetOutputConfiguration(screen,  0, &out_cfg) != DFB_OK)
			{
				D_ERROR("SetOutputConfiguration(0) failed!\n");
				component = false;
				analog = false;
			}
		}
	}

	if (component)
	{
		if (signal & (DSOS_1080p | DSOS_1080p24))
		{
			// not component standard signals, use the next best
			if (hdmi && analog)
				// only 2 independent output timings available, use the analog one
				signal = DSOS_SDTV;
			else
				// independent output timing available, use the best signal
				signal = DSOS_1080i;
		}
		else if (signal == DSOS_EDID)
			signal = DSOS_1080i;

		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG);
		enc_cfg.out_signals = signal;
		enc_cfg.delayed_config = true;
		if (screen->SetEncoderConfiguration(screen, 1, &enc_cfg) != DFB_OK)
		{
			D_ERROR("SetEncoderConfiguration(1) failed!\n");
			analog = false;
		}
		else
		{
			out_cfg.flags = (DFBScreenOutputConfigFlags)(DSOCONF_SIGNALS | DSOCONF_DELAYED_CONFIG);
			out_cfg.out_signals = signal;
			out_cfg.delayed_config = analog;
			if (screen->SetOutputConfiguration(screen,  1, &out_cfg) != DFB_OK)
			{
				D_ERROR("SetOutputConfiguration(1) failed!\n");
				analog = false;
			}
		}
	}

	if (analog)
	{
		screen->GetEncoderConfiguration(screen, 2, &enc_cfg);		// for TV standard

		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG);
#ifdef RMCFG_TV_STD_PAL
		enc_cfg.out_signals = DSOS_PAL;
#elif defined RMCFG_TV_STD_SECAM
		enc_cfg.out_signals = DSOS_SECAM;
#elif defined RMCFG_TV_STD_NTSC_J // Japan
		enc_cfg.out_signals = DSOS_NTSC_J;
#elif defined RMCFG_TV_STD_NTSC       
		enc_cfg.out_signals = DSOS_NTSC;
#else 
		enc_cfg.out_signals = DSOS_NONE;
#endif
		enc_cfg.delayed_config = true;
		if (screen->SetEncoderConfiguration(screen, 2, &enc_cfg) != DFB_OK)
		{
			D_ERROR("SetEncoderConfiguration(2) failed!\n");
		}
		else
		{
			out_cfg.flags = (DFBScreenOutputConfigFlags)(DSOCONF_SIGNALS | DSOCONF_DELAYED_CONFIG);
			out_cfg.out_signals = enc_cfg.out_signals;
			out_cfg.delayed_config = false;
			if (screen->SetOutputConfiguration(screen,  2, &out_cfg) != DFB_OK)
			{
				D_ERROR("SetOutputConfiguration(2) failed!\n");
			}
		}
	}
		
	if (signal != DSOS_NONE)
	{
		out_cfg.flags = DSOCONF_SIGNALS;
		if ((hdmi && (screen->GetOutputConfiguration(screen, 0, &out_cfg) == DFB_OK)) ||
			(component && (screen->GetOutputConfiguration(screen, 1, &out_cfg) == DFB_OK)) ||
			(analog && (screen->GetOutputConfiguration(screen, 2, &out_cfg) == DFB_OK)))
		{
			unsigned i;
			DirectFBScreenOutputSignalsNames(signalNamesTab);
			
			signal = out_cfg.out_signals;
			for (i=0; i<sizeof(signalNamesTab)/sizeof(signalNamesTab[0]); i++)
				if (signalNamesTab[i].signal == signal)
				{
					D_INFO("New nominal output signal: %s\n", signalNamesTab[i].name);
					break;
				}
		}
		else
		{
			D_ERROR("GetOutputConfiguration failed on all encoders!\n");
		}
		last_hdtv_signal = signal;
		
		get_signal_resolution(signal);
		/*
		 *	Tri hide these line, so we don't need use background layer.
		 */
#if 0
		if (useBackground)		// if background is enabled, reconfigure it
		{
			
			if  ( ( !mediaTab[ampIndex].amp) ||
				( mediaTab[ampIndex].status.generic.mediaSpace == MEDIA_SPACE_LINEAR_MEDIA || 
				mediaTab[ampIndex].status.generic.mediaSpace == MEDIA_SPACE_CDDA))
			{	
				enable_background_layer();
				
			}// defaults to (xres,yres)
		}
#endif
	}
	return status ;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *				SetOutpuMode (RMuint8 index)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *				RMuint8 index
 * @return 
 *				RMstatus
*/
RMstatus set_output_mode (RMuint8 index)
{
	RMstatus status;
	useBackground = TRUE;
	switch(index) {
	case TVMODE_480I_576I: // 480i/576i
		status = set_hdtv_resolution(DSOS_SDTV);
		break;
	case TVMODE_480P_576P: // 480p/576p
		status = set_hdtv_resolution(DSOS_EDTV);
		break;
	case TVMODE_720P: // 720p
		status = set_hdtv_resolution(DSOS_720p);
		break;
	case TVMODE_1080I: // 1080i
		status = set_hdtv_resolution(DSOS_1080i);
		break;
	case TVMODE_1080P: // 1080p 	
		status = set_hdtv_resolution(DSOS_1080p);
		break;
	case TVMODE_1080P24: // 1080p24 				
		status = set_hdtv_resolution(DSOS_1080p24);
		break;
	case TVMODE_AUTO: // auto 				
		status = set_hdtv_resolution(DSOS_EDID);
		break;
		
	default:
		status = set_hdtv_resolution(DSOS_EDTV);
		break;
	}
	return status ;
}


/**
 * 
 * <b>FUNCTION: </b>
 *
 *	set_output_mode_for_PAL (RMuint8 index)
 *
 * <b>DESCRIPTION: </b>
 * 
 * set output mode for PAL
 * 
 * @param
 *				RMuint8 index
 * @return 
 *				RMstatus
*/
RMstatus set_output_mode_for_PAL (RMuint8 index)
{
	RMstatus status;
	useBackground = TRUE;
	switch(index) {
	case TVMODE_480I_576I: // 480i/576i
		status = set_hdtv_resolution_standard_tv(DSOS_SDTV, true);
		break;
	case TVMODE_480P_576P: // 480p/576p
		status = set_hdtv_resolution_standard_tv(DSOS_EDTV, true);
		break;
	case TVMODE_720P: // 720p
		status = set_hdtv_resolution_standard_tv(DSOS_720p, true);
		break;
	case TVMODE_1080I: // 1080i
		status = set_hdtv_resolution_standard_tv(DSOS_1080i, true);
		break;
	case TVMODE_1080P: // 1080p 	
		status = set_hdtv_resolution_standard_tv(DSOS_1080p, true);
		break;
	case TVMODE_1080P24: // 1080p24 				
		status = set_hdtv_resolution_standard_tv(DSOS_1080p24, true);
		break;
	case TVMODE_AUTO: // auto 
		status = set_hdtv_resolution_standard_tv(DSOS_EDID, true);			
		break;
		
	default:
		status = set_hdtv_resolution_standard_tv(DSOS_EDID, true);
		break;
	}
	return status ;
}



/**
 * 
 * <b>FUNCTION: </b>
 *
 * set_output_mode_for_NTSC (RMuint8 index)
 *
 * <b>DESCRIPTION: </b>
 * 
 * set output mofe for NTSC
 * 
 * @param
 *				RMuint8 index
 * @return 
 *				RMstatus
*/
RMstatus set_output_mode_for_NTSC (RMuint8 index)
{
	RMstatus status;
	useBackground = TRUE;
	switch(index) {
	case TVMODE_480I_576I: // 480i/576i
		status = set_hdtv_resolution_standard_tv(DSOS_SDTV, false);
		break;
	case TVMODE_480P_576P: // 480p/576p
		status = set_hdtv_resolution_standard_tv(DSOS_EDTV, false);
		break;
	case TVMODE_720P: // 720p
		status = set_hdtv_resolution_standard_tv(DSOS_720p, false);
		break;
	case TVMODE_1080I: // 1080i
		status = set_hdtv_resolution_standard_tv(DSOS_1080i, false);
		break;
	case TVMODE_1080P: // 1080p 	
		status = set_hdtv_resolution_standard_tv(DSOS_1080p, false);
		break;
	case TVMODE_1080P24: // 1080p24 				
		status = set_hdtv_resolution_standard_tv(DSOS_1080p24, false);
		break;
	case TVMODE_AUTO: // auto 
		status = set_hdtv_resolution_standard_tv(DSOS_EDID, false);			
		break;
		
	default:
		status = set_hdtv_resolution_standard_tv(DSOS_EDID, false);
		break;
	}
	return status ;
}



/**
 * 
 * <b>FUNCTION: </b>
 *
 * get_signal_resolution(DFBScreenOutputSignals signal)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *       DFBScreenOutputSignals signal
 * @return 
 *      DFBResult
*/
DFBResult get_signal_resolution(DFBScreenOutputSignals signal)
{
	switch (signal)
	{
	case DSOS_1080i:
	case DSOS_1080p:
	case DSOS_1080p24:
		out_xres = 1920;
		out_yres = 1080;
		return DFB_OK;
		
	case DSOS_720p:
		out_xres = 1280;
		out_yres = 720;
		return DFB_OK;
		
	case DSOS_SDTV:
	case DSOS_EDTV:
		out_xres = 720;
#if defined(RMCFG_TV_STD_NTSC) || defined(RMCFG_TV_STD_NTSC_J)
		out_yres = 480;
#else
		out_yres = 576;
#endif /* NTSC or NTSC_J */
		return DFB_OK;
		
	default:
		out_xres = xres;
		out_yres = yres;
		return DFB_FAILURE;
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * getResolution(RMuint32 index)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *       RMuint32 index
 * @return 
 *      DFBScreenOutputSignals
*/
DFBScreenOutputSignals get_resolution(RMuint32 index)
{
	
	switch(index) 
	{
	case TVMODE_480I_576I:
		return DSOS_SDTV ;
	case TVMODE_480P_576P:
		return DSOS_EDTV ;
	case TVMODE_720P:
		return DSOS_720p ;
	case TVMODE_1080I:
		return DSOS_1080i ;
	case TVMODE_1080P:
		return DSOS_1080p ;
	case TVMODE_1080P24:
		return DSOS_1080p24 ;
	}
	return DSOS_SDTV ;
	
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * get_index_of_resolution(DFBScreenOutputSignals signal)
 *
 * <b>DESCRIPTION: </b>
 * 
 * intialize directfb
 * 
 * @param
 *       DFBScreenOutputSignals signal
 * @return 
 *      RMuint16
*/
RMuint16 get_index_of_resolution(DFBScreenOutputSignals signal)
{
	RMuint16 temp = 1;
	switch(signal) 
	{
	case DSOS_SDTV:
		temp = TVMODE_480I_576I ;
		break;
	case DSOS_EDTV:
		temp = TVMODE_480P_576P ;
		break;
	case DSOS_720p:
		temp = TVMODE_720P ;
		break;
	case DSOS_1080i:
		temp = TVMODE_1080I ;
		break;
	case DSOS_1080p:
		temp = TVMODE_1080P ;
		break;
	case DSOS_1080p24:
		temp = TVMODE_1080P24 ;
		break;
	default:
		temp = TVMODE_1080I;
		break;
	}
	return temp ;
}
/*************************************************************************************
 *
 *
 *
 *                          Video Config
 *
 *
 *
 *************************************************************************************/
/**
 * 
 * <b>FUNCTION: </b>
 *
 * CheckHDMIOunt(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       void
 * @return 
 *      bool
*/
bool check_hdmi_signal(void)
{
    if (ampGlobals->shared->settings.video.common.hdmi.status != CS_Disconnected)
    {
        return true;
    }
    else 
    {
        return false;
    }
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * CheckHDMIOunt(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       void
 * @return 
 *      bool
*/
void check_hotplug_hdmi_cable(void)
{
	if(hdmi_out_status != ampGlobals->shared->configuration.output.video.common.hdmi.status)
	{
		isHDMIHotPlug = TRUE;
		hdmi_out_status = ampGlobals->shared->configuration.output.video.common.hdmi.status ;
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * getResolution(RMuint32 index)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *       RMuint32 index
 * @return 
 *      DFBScreenOutputSignals
*/
void set_aspect_ratio(RMuint16 as_type)
{
	GUI_INFO("[(%s)%s] Aspect Ratio: %d\n", __FILE__, __FUNCTION__, as_type);
	
	/* NOTE: don't use "TVAR_Unknown" (HDMI auto-detect) for HDMI's display-aspect ratio.
	* It will cause problems with losing the background image, since with auto-detection
	* there may be a mismatch between the component AR and HDMI AR.
	*/
	switch(as_type) {
	case ARM_PanScan:
	/* Intended display is 4:3; performs pan-scan on 16:9 
	* *DVD* content with correct aspect ratio. Non-DVD content
	* will be cropped on left and right sides, since pan-scan
	* metadata is not available.
		*/
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = 
			pCrtVideoCfg->common.composite.arMatch = ARM_PanScan;
		
		pCrtVideoCfg->common.hdmi.display.aspectRatio = 
			pCrtVideoCfg->common.component.display.aspectRatio = 
			pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_4x3;
		break;
	case ARM_LetterBox:
	/* Intended display is 4:3; adds letter-box to 16:9 content
	* to keep correct aspect ratio. 
		*/
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = 
			pCrtVideoCfg->common.composite.arMatch = ARM_LetterBox;
		
		pCrtVideoCfg->common.hdmi.display.aspectRatio = 
			pCrtVideoCfg->common.component.display.aspectRatio = 
			pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_4x3;
		printf("\n\033[31mset_aspect_ratio: as_type= %d = 4:3 ARM_LetterBox\n\033[0m", as_type);
		break;
	case ARM_SidePanel:
	/* Intended display is 16:9; adds side panels to 4:3 content
	* to keep correct aspect ratio. 
		*/
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = ARM_SidePanel;
		pCrtVideoCfg->common.composite.arMatch = ARM_NotSet;
		
		pCrtVideoCfg->common.hdmi.display.aspectRatio = 
			pCrtVideoCfg->common.component.display.aspectRatio = TVAR_16x9;
		pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_4x3;
		printf("\n\033[31mset_aspect_ratio: as_type= %d - 16:9 ARM_SidePanel\n\033[0m", as_type);
		break;
	case ARM_No:
	/* Intended display is full screen
		*/
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = ARM_No;
		pCrtVideoCfg->common.composite.arMatch = ARM_NotSet;
		
		pCrtVideoCfg->common.hdmi.display.aspectRatio = 
			pCrtVideoCfg->common.component.display.aspectRatio = TVAR_16x9;
		pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_4x3;
		break;
#if 0 //not used
	case ARM_Crop: 
	/* Intended display is 16:9; crop top and bottom of 4:3 
	* content. This will remove letter-box from letter-boxed 4:3
	* content and keep the correct aspect ratio. However, 
	* if 4:3 content is not letter-boxed then top and bottom 
	* content will be cropped and not be visible! 
		*/
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = ARM_Crop;
		pCrtVideoCfg->common.composite.arMatch = ARM_NotSet;
		
		pCrtVideoCfg->common.hdmi.display.aspectRatio = 
			pCrtVideoCfg->common.component.display.aspectRatio = TVAR_16x9;
		pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_4x3;
		break;
#endif /* not used */
	default:
		APP_ASSERT(0); //should not get here
		break;
	}
	
	cfgamp->SetConfiguration(cfgamp, (struct SVideoOutConfig*)pCrtVideoCfg, NULL);
}
/*************************************************************************************
 *
 *
 *
 *                          Audio Config
 *
 *
 *
 *************************************************************************************/
/**
 * 
 * <b>FUNCTION: </b>
 *
 *		SetSpeakerConfig(RMuint16 L, RMuint16 C, RMuint16 R, RMuint16 SL, 
 *						RMuint16 SR, RMuint16 LEF, RMuint16 BL, RMuint16 BR , RMbool SetConfig)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *       RMuint16 L, RMuint16 C, RMuint16 R, RMuint16 SL, 
 *						RMuint16 SR, RMuint16 LEF, RMuint16 BL, RMuint16 BR , RMbool SetConfig
 * @return 
 *      void
*/
void set_speaker_config(RMuint16 L, RMuint16 C, RMuint16 R, RMuint16 SL, RMuint16 SR, RMuint16 LEF, RMuint16 BL, RMuint16 BR , RMbool SetConfig)
{

	// Find Max value of Speaker
	RMint8 n = 8;
	RMuint16 maxtemp;
	RMuint16 speaker[n];
	RMuint16 value0fSpeaker = 1129 ;
	int i = 0;
	// assign value for speaker
	speaker[0] = L;
	speaker[1] = C;
	speaker[2] = R;
	speaker[3] = SL;
	speaker[4] = SR;
	speaker[5] = LEF;
	speaker[6] = BL;
	speaker[7] = BR;
	maxtemp = speaker[0];
	for (i=0;i<n;i++)
	{
		if(speaker[i] > maxtemp)
		{
			maxtemp =speaker[i];
		}
	}
	
	speaker[0] = (RMuint16)(((maxtemp - L ) / value0fSpeaker )*10);
	speaker[1] = (RMuint16)(((maxtemp - C ) / value0fSpeaker )*10);
	speaker[2] = (RMuint16)(((maxtemp - R ) / value0fSpeaker )*10);
	speaker[3] = (RMuint16)(((maxtemp - SL ) / value0fSpeaker )*10);
	speaker[4] = (RMuint16)(((maxtemp - SR ) / value0fSpeaker )*10);
	speaker[5] = (RMuint16)(((maxtemp - LEF ) / value0fSpeaker )*10);
	speaker[6] = (RMuint16)(((maxtemp - BL ) / value0fSpeaker )*10);
	speaker[7] = (RMuint16)(((maxtemp - BR ) / value0fSpeaker )*10);
	
	/////////////////////////////////////////////////////////////
	// L : 0 | C : 1 | R : 2 | SL : 3 | SR : 4 | LFE : 5 | BL : 6 | BR : 7 |
	///////////////////////////////////////////////////////////////
	// set delay audio for HDMI audio
	pCrtAudioCfg->common.hdmi.output.delay[0] = speaker[0] ;
	pCrtAudioCfg->common.hdmi.output.delay[1] = speaker[1] ;
	pCrtAudioCfg->common.hdmi.output.delay[2] = speaker[2] ;
	pCrtAudioCfg->common.hdmi.output.delay[3] = speaker[3] ;
	pCrtAudioCfg->common.hdmi.output.delay[4] = speaker[4] ;
	pCrtAudioCfg->common.hdmi.output.delay[5] = 0;//speaker[5] ;
	pCrtAudioCfg->common.hdmi.output.delay[6] = speaker[6] ;
	pCrtAudioCfg->common.hdmi.output.delay[7] = speaker[7] ;

	
	// set delay audio for multichannel

	pCrtAudioCfg->common.multiChannel.output.delay[0] = speaker[0] ;
	pCrtAudioCfg->common.multiChannel.output.delay[1] = speaker[1] ;
	pCrtAudioCfg->common.multiChannel.output.delay[2] = speaker[2] ;
	pCrtAudioCfg->common.multiChannel.output.delay[3] = speaker[3] ;
	pCrtAudioCfg->common.multiChannel.output.delay[4] = speaker[4] ;
	pCrtAudioCfg->common.multiChannel.output.delay[5] = 0;//speaker[5] ;
	pCrtAudioCfg->common.multiChannel.output.delay[6] = speaker[6] ;
	pCrtAudioCfg->common.multiChannel.output.delay[7] = speaker[7] ;

	// set delay audio for spdif
	pCrtAudioCfg->common.spdif.output.delay[0] = speaker[0] ;
	pCrtAudioCfg->common.spdif.output.delay[1] = speaker[1] ;
	pCrtAudioCfg->common.spdif.output.delay[2] = speaker[2] ;
	pCrtAudioCfg->common.spdif.output.delay[3] = speaker[3] ;
	pCrtAudioCfg->common.spdif.output.delay[4] = speaker[4] ;
	pCrtAudioCfg->common.spdif.output.delay[5] = 0;//speaker[5] ;
	pCrtAudioCfg->common.spdif.output.delay[6] = speaker[6] ;
	pCrtAudioCfg->common.spdif.output.delay[7] = speaker[7] ;
	if(SetConfig)
		cfgamp->SetConfiguration(cfgamp, NULL, (struct SAudioOutConfig*)pCrtAudioCfg);
	GUI_INFO("----------- Set configuraton for audio delay ---------------\n");
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * set_digital_audio(RMuint16 index, RMbool SetConfig)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *     RMuint16 index, RMbool SetConfig
 * @return 
 *      RMbool
*/
RMbool set_digital_audio(RMuint16 index, RMbool SetConfig)
{
	switch(index) {
	case PCM_OUT_HDMI:
		if(pCrtAudioCfg->common.hdmi.bitstream.preserveOriginalBitstream == 0 && 
		   pCrtAudioCfg->common.hdmi.bitstream.formats == audioConfigTab[2].common.hdmi.bitstream.formats	)
		{
			break;
		}
		pCrtAudioCfg->common.hdmi.bitstream.preserveOriginalBitstream = 0;//audioConfigTab[2].common.hdmi;
		pCrtAudioCfg->common.hdmi.bitstream.formats = audioConfigTab[2].common.hdmi.bitstream.formats;

		goto SetAudioConfig;
		break;
	case BIT_STREAM_OUT_HDMI:
		//pCrtAudioCfg->common.hdmi = audioConfigTab[0].common.hdmi;
		if(pCrtAudioCfg->common.hdmi.bitstream.preserveOriginalBitstream == 1 &&
		   pCrtAudioCfg->common.hdmi.bitstream.formats == audioConfigTab[0].common.hdmi.bitstream.formats)
		{
			break;
		}
		pCrtAudioCfg->common.hdmi.bitstream.preserveOriginalBitstream = 1;
		pCrtAudioCfg->common.hdmi.bitstream.formats = audioConfigTab[0].common.hdmi.bitstream.formats;
		goto SetAudioConfig;
		break;
	case PCM_OUT_SPDIF:
		//pCrtAudioCfg->common.spdif = audioConfigTab[2].common.spdif;
		if(pCrtAudioCfg->common.spdif.bitstream.preserveOriginalBitstream == 0 &&
		   pCrtAudioCfg->common.spdif.bitstream.formats == audioConfigTab[2].common.spdif.bitstream.formats)
		{
			break;
		}
		pCrtAudioCfg->common.spdif.bitstream.preserveOriginalBitstream = 0;//audioConfigTab[2].common.spdif;
		pCrtAudioCfg->common.spdif.bitstream.formats = audioConfigTab[2].common.spdif.bitstream.formats;
		goto SetAudioConfig;
		break;
	case BIT_STREAM_OUT_SPDIF:
		//pCrtAudioCfg->common.spdif = audioConfigTab[0].common.spdif;
		//pCrtAudioCfg->common.spdif.bitstream.pcmChannels = CCf_LCRBlBr;
		if(pCrtAudioCfg->common.spdif.bitstream.preserveOriginalBitstream == 1 &&
		   pCrtAudioCfg->common.spdif.bitstream.formats == audioConfigTab[0].common.spdif.bitstream.formats)
		{
			break;
		}
		pCrtAudioCfg->common.spdif.bitstream.preserveOriginalBitstream = 1;
		pCrtAudioCfg->common.spdif.bitstream.formats = audioConfigTab[0].common.spdif.bitstream.formats; 
		goto SetAudioConfig;
		break;
	}
	return true;
SetAudioConfig:
    if(SetConfig)
		cfgamp->SetConfiguration(cfgamp, NULL, (struct SAudioOutConfig*)pCrtAudioCfg);
	return true;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * reset_digital_audio(RMuint16 index, RMbool SetConfig)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *     RMuint16 index, RMbool SetConfig
 * @return 
 *      RMbool
*/
void reset_digital_audio(RMbool isConvertToPCM)
{
	if(isConvertToPCM)
	{
		// set audio for HDMI
		if(pvalueAudioOuput_HDMI)
		{		
			if(*pvalueAudioOuput_HDMI == AUDIO_BITSTREAM )
			{
					GUI_INFO("==============   Convert BITSTREAM TO PCM TO OUT HDMI ===================\n");
					if( pCrtAudioCfg->common.hdmi.bitstream.formats != audioConfigTab[2].common.hdmi.bitstream.formats )
					set_digital_audio(PCM_OUT_HDMI,FALSE);
			}
		}
		// set audio for spdif 
		if(pvalueAudioOuput_SPDIF)
		{
			if(*pvalueAudioOuput_SPDIF == AUDIO_BITSTREAM)
			{
				GUI_INFO("==============   Convert BITSTREAM TO PCM TO OUT SPDIF ===================\n");
				if(pCrtAudioCfg->common.spdif.bitstream.formats != audioConfigTab[2].common.spdif.bitstream.formats)
					set_digital_audio(PCM_OUT_SPDIF,TRUE); 
			}
		}
	}
	else
	{
		// set audio for HDMI
		if(pvalueAudioOuput_HDMI)
		{		
			if(*pvalueAudioOuput_HDMI == AUDIO_BITSTREAM )
			{
				GUI_INFO("==============   Convert PCM TO  BITSTREAM  TO OUT HDMI ===================\n");
				set_digital_audio(BIT_STREAM_OUT_HDMI,FALSE);
			}
		}
		// set audio for spdif 
		if(pvalueAudioOuput_SPDIF)
		{
			if(*pvalueAudioOuput_SPDIF == AUDIO_BITSTREAM)
			{
				GUI_INFO("==============   Convert PCM TO  BITSTREAM  TO OUT SPDIF ===================\n");
				set_digital_audio(BIT_STREAM_OUT_SPDIF,TRUE); 
			}
		}
	}
}
void set_pcm_down_sampling(RMbool isOn)
{
	if(isOn )
	{
		pCrtAudioCfg->common.spdif.bitstream.pcmRate = 48000;
		pCrtAudioCfg->common.spdif.bitstream.formatParamTab[0].samplingRates = ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000); //PCM
	}
	else
	{
		pCrtAudioCfg->common.spdif.bitstream.pcmRate = 96000;
		pCrtAudioCfg->common.spdif.bitstream.formatParamTab[0].samplingRates = ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000) | ASRATE_TO_BITMASK(FREQ_96000); //PCM
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * setDynamicCompression(RMuint16 DynamicCompresssion)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *     RMuint16 DynamicCompresssion
 * @return 
 *      void
*/
void set_dynamic_compression(RMuint16 DynamicCompresssion)
{
	GUI_INFO("[(%s)%s] DynamicCompresssion: %d\n", __FILE__, __FUNCTION__, DynamicCompresssion);
	// Set Dolby Dynamic Range Control levels
	if(DynamicCompresssion == DRC_HIGH )
	{
		// "High" cut and boost levels
		GUI_INFO("-----------  DRC_HIGH ---------------------\n");
		pCrtAudioCfg->common.drc.mode = DRC_Custom; 
		pCrtAudioCfg->common.drc.dialogRef = -31 ;
		pCrtAudioCfg->common.drc.boost = 0x10000000;
		pCrtAudioCfg->common.drc.cut = 0x10000000;
		pCrtAudioCfg->common.drc.partComp = 0x10000000;
	}
	else if(DynamicCompresssion == DRC_MID )
	{
		// "Medium" cut and boost levels 
		GUI_INFO("-----------  DRC_MID ---------------------\n");
		pCrtAudioCfg->common.drc.mode = DRC_Custom; // Min
		pCrtAudioCfg->common.drc.dialogRef = -31;
		pCrtAudioCfg->common.drc.boost = 0x02000000;
		pCrtAudioCfg->common.drc.cut = 0x02000000;
		pCrtAudioCfg->common.drc.partComp = 0x10000000;
	}
	else if(DynamicCompresssion == DRC_OFF)
	{
		// No cut or boost applied
		GUI_INFO("-----------  DRC_OFF ---------------------\n");
		pCrtAudioCfg->common.drc.mode = DRC_Custom;
		pCrtAudioCfg->common.drc.dialogRef = -31;
		pCrtAudioCfg->common.drc.boost = 0;
		pCrtAudioCfg->common.drc.cut = 0;
		pCrtAudioCfg->common.drc.partComp = 0x10000000;
	}
	else if(DynamicCompresssion == DRC_AUTO)
	{
		// DRC parameters in the stream are enforced
		GUI_INFO("-----------  DRC_AUTO ---------------------\n");
		pCrtAudioCfg->common.drc.mode = DRC_Auto;
		pCrtAudioCfg->common.drc.dialogRef = -31;
		pCrtAudioCfg->common.drc.boost = 0x10000000;
		pCrtAudioCfg->common.drc.cut = 0x10000000;
		pCrtAudioCfg->common.drc.partComp = 0x10000000;
	}
	else {
		APP_ASSERT(0); //should not get here
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetHDMIConnected
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *    const bool *isConnected
 * @return 
 *      void
*/

void set_HDMI_Connected(const bool isConnected)
{
	//printf("----------- set_HDMI_Connected-------------------\n");
	// discconnected hdmi audio, AVR couldn't decode audio from hdmi cable
	// but stereo can receive audio normally.
	unsigned short tempStatus = isConnected ? CS_Unknown: CS_Disconnected;
	pCrtAudioCfg->common.hdmi.bitstream.status = tempStatus;
	// commit the changing of audio to global aduio configuration.
	cfgamp->SetConfiguration(cfgamp, NULL, (struct SAudioOutConfig*)pCrtAudioCfg);
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * void setAccAudio(RMuint16 AAC)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *     const unsigned long value ,const unsigned short executeAudioConfig 
 * @return 
 *      void
*/
void set_PassThrough_Downmix_Audio(const unsigned long value, const unsigned short executeAudioConfig )
{
	//formats of HDMi and SPDIF, they are the same,
	// so using one comparision of the value 
	bool isValue = pCrtAudioCfg->common.hdmi.bitstream.formats & value;
	//printf("*value [0x0%lx]<-----------------\n\n",value);
	long tempvalue =  (executeAudioConfig == PASSTHROUGH )? /*on*/ value : /*off*/~(value);
	//printf("hdmi.bitstream.formats [0x0%lx]<-----------------\n\n",pCrtAudioCfg->common.hdmi.bitstream.formats);
	switch(executeAudioConfig)
	{
	case DOWNMIX:
		if(isValue){
		//Downmix, audio will down to PCM 2 channel.
			//printf("----------- AUDIO_DIGITAL_DOWNMIX-------------------\n");
			pCrtAudioCfg->common.hdmi.bitstream.formats &= tempvalue; 
			pCrtAudioCfg->common.spdif.bitstream.formats &= tempvalue;
		}
		break;
	case PASSTHROUGH:
		if(!isValue){
			// PassThrough audio direct to AVR.
			//printf("----------- AUDIO_DIGITAL_PASSTHROUGH-------------------\n");
			//printf("tempvalue [0x0%lx]<-----------------\n\n",tempvalue);
			pCrtAudioCfg->common.hdmi.bitstream.formats |= tempvalue; 
			pCrtAudioCfg->common.spdif.bitstream.formats |= tempvalue;
		}
		break;
	}
	pCrtAudioCfg->common.hdmi.bitstream.preserveOriginalBitstream  =
	pCrtAudioCfg->common.hdmi.bitstream.preserveOriginalQuality   =
	pCrtAudioCfg->common.spdif.bitstream.preserveOriginalBitstream  =
	pCrtAudioCfg->common.spdif.bitstream.preserveOriginalQuality  = 1;
	cfgamp->SetConfiguration(cfgamp, NULL, (struct SAudioOutConfig*)pCrtAudioCfg);
 
	//printf("Debug Audio\n\n");
	//printf("\t\thdmi.formats=[0x0%lx]\n\n",pCrtAudioCfg->common.hdmi.bitstream.formats);
	//printf("\t\tMTYPE_ELEM_PCM=[0x0%lx]\n",AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_PCM));
	//printf("\t\tMTYPE_ELEM_AC3=[0x0%lx]\n",AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_AC3));
	//printf("\t\tMTYPE_ELEM_DTS=[0x0%lx]\n",AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_DTS));
	//printf("\t\tMTYPE_ELEM_WMAPRO=[0x0%lx]\n",AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_WMAPRO));
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * void SetSurroundMode(RMuint16 mode)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *     RMuint16 mode
 * @return 
 *      void
*/
void set_surround_mode(RMuint16 mode)
{
	switch(mode) {
	case SURROUND_MODE_OFF:
		printf("---------  SURROUND_MODE_OFF -----------------------\n");
		pCrtAudioCfg->common.stereo.surroundMode = SRND_OFF;
		/* Apply to spdif also, since DCCHD may override with spdif
		   setting on the same audio engine
		*/ 
		pCrtAudioCfg->common.spdif.output.surroundMode = SRND_OFF; 
		break;
	case SURROUND_MODE_ON:
		printf("---------  SURROUND_MODE_ON -----------------------\n");
		pCrtAudioCfg->common.stereo.surroundMode = SRND_ON;
		/* Apply to spdif also, since DCCHD may override with spdif
		   setting on the same audio engine
		*/ 
		pCrtAudioCfg->common.spdif.output.surroundMode = SRND_ON; 
		break;
	default:
		printf("---------  SURROUND_MODE_Default -----------------------\n");
		pCrtAudioCfg->common.stereo.surroundMode = SRND_Default;
		/* Apply to spdif also, since DCCHD may override with spdif
		   setting on the same audio engine
		*/ 
		pCrtAudioCfg->common.spdif.output.surroundMode = SRND_Default; 
		break;
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetVolumeForSpeaker(enum SpeakerOut speaker)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *     enum SpeakerOut speaker
 * @return 
 *      void
*/
/* Test-tone setting of 0dB resulted in measured output of -22dB which is greater than the Dolby spec 
 * of -30dB. Therefore, we need to use the -8dB volume setting for test-tone. 
 */
#define TEST_TONE_VOLUME	0x06597fa9  //-8dB
#define FULL_SCALE_VOLUME	0x10000000  //0dB
void set_volume_for_speaker(enum SpeakerOut speaker)
{
	///< channel volume (L, C, R, Sl, Sr, Lfe, Bl, Br for HDMI/multichannel; L, R for SPDIF) for sound field balancing
	RMuint32 L, C, R, Sl, Sr, Lfe, Bl, Br;
	
	
	// Default to zero
	L = C = R = Sl = Sr = Lfe = Bl = Br = 0;
	
	// mute other audio connectors:HDMI,S/PDIF,2ch downmix 
	pCrtAudioCfg->common.hdmi.output.volume[0] = 0x0;
	pCrtAudioCfg->common.hdmi.output.volume[1] = 0x0;
	pCrtAudioCfg->common.hdmi.output.volume[2] = 0x0;
	pCrtAudioCfg->common.hdmi.output.volume[3] = 0x0;
	pCrtAudioCfg->common.hdmi.output.volume[4] = 0x0;
	pCrtAudioCfg->common.hdmi.output.volume[5] = 0x0;
	pCrtAudioCfg->common.hdmi.output.volume[6] = 0x0;
	pCrtAudioCfg->common.hdmi.output.volume[7] = 0x0;
	
	// S/PDIF only 2ch.."volume[2]" is *not* a typo 
	pCrtAudioCfg->common.spdif.output.volume[0] = 0x0;
	pCrtAudioCfg->common.spdif.output.volume[2] = 0x0;
	
	pCrtAudioCfg->common.stereo.volume[0] = 0x0;
	pCrtAudioCfg->common.stereo.volume[1] = 0x0;
	
	switch(speaker) {
	case Speaker_C:
		C = TEST_TONE_VOLUME;
		break;
	case Speaker_L:
		L = TEST_TONE_VOLUME;
		break;
	case Speaker_R:
		R = TEST_TONE_VOLUME;
		break;
	case Speaker_LR:
		L = R = TEST_TONE_VOLUME;
		break;
	case Speaker_SL:
		Sl = TEST_TONE_VOLUME;
		break;
	case Speaker_SR:
		Sr = TEST_TONE_VOLUME;
		break;
	case Speaker_SLSR:
		Sl = Sr = TEST_TONE_VOLUME;
		break;
	case Speaker_BL:
		Bl = TEST_TONE_VOLUME;
		break;
	case Speaker_BR:
		Br = TEST_TONE_VOLUME;
		break;
	case Speaker_BLBR:
		Bl = Br = TEST_TONE_VOLUME;
		break;
	case Speaker_LFE:
		Lfe = TEST_TONE_VOLUME;
		break;
	case Speaker_LCRSLSRLEFBLBR:
		// reset all volumes to full-scale
		C = L = R = Sl = Sr = Bl = Br = Lfe = FULL_SCALE_VOLUME;
		
		// HDMI,S/PDIF,2ch downmix 
		pCrtAudioCfg->common.hdmi.output.volume[0] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.hdmi.output.volume[1] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.hdmi.output.volume[2] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.hdmi.output.volume[3] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.hdmi.output.volume[4] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.hdmi.output.volume[5] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.hdmi.output.volume[6] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.hdmi.output.volume[7] = FULL_SCALE_VOLUME;
		
		// S/PDIF only 2ch.."volume[2]" is *not* a typo 
		pCrtAudioCfg->common.spdif.output.volume[0] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.spdif.output.volume[2] = FULL_SCALE_VOLUME;
		
		pCrtAudioCfg->common.stereo.volume[0] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.stereo.volume[1] = FULL_SCALE_VOLUME;
		
		break;
	default:
		break;
	}
	
	// For multiChannel
	pCrtAudioCfg->common.multiChannel.output.volume[0] = L ;
	pCrtAudioCfg->common.multiChannel.output.volume[1] = C ;
	pCrtAudioCfg->common.multiChannel.output.volume[2] = R ;
	pCrtAudioCfg->common.multiChannel.output.volume[3] = Sl ;
	pCrtAudioCfg->common.multiChannel.output.volume[4] = Sr ;
	pCrtAudioCfg->common.multiChannel.output.volume[5] = Lfe ;
	pCrtAudioCfg->common.multiChannel.output.volume[6] = Bl ;
	pCrtAudioCfg->common.multiChannel.output.volume[7] = Br ;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetEchannelConfig(enum EChannelConfig speaker)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *    enum EChannelConfig speaker
 * @return 
 *      RMbool
*/
RMbool set_echannel_config(enum EChannelConfig speaker)
{
	pCrtAudioCfg->common.multiChannel.output.speakers = speaker;
	pCrtAudioCfg->common.multiChannel.preserveOriginalQuality = 0;
	return true;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetBassMode(enum EBassMode bassMode, RMbool isSetConfig)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *    enum EBassMode bassMode, RMbool isSetConfig
 * @return 
 *      void
*/
void  set_bass_mode(enum EBassMode bassMode, RMbool isSetConfig)
{
	pCrtAudioCfg->common.multiChannel.output.bassMode = bassMode;
	if(isSetConfig)
	{
		set_effect_on_audio();
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetBassMode(enum EBassMode bassMode, RMbool isSetConfig)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *    enum EBassMode bassMode, RMbool isSetConfig
 * @return 
 *      void
*/
RMbool set_lfe_bassmode_on_audio(RMbool lfe, RMuint32 speaker_size)
{
	enum EBassMode bass_mode;

	// Bass management depends on speaker size and SW existence
	if(lfe )
	{
		// Sub-woofer enabled..assume exists
		pCrtAudioCfg->common.multiChannel.output.lfe = 1;
		if (speaker_size == 3) {
			//set to Small Speakers..send low-freq to SW
			bass_mode = BM_all_2_lfe;
			GUI_INFO("[(%s)%s] Bass Mode:BM_all_2_lfe\n", __FILE__, __FUNCTION__);
		} else if (speaker_size == 2) {
			//set to Large LR speakers..send CLsRs->LFE
			bass_mode = BM_CLsRs_2_lfe;
			GUI_INFO("[(%s)%s] Bass Mode:BM_CLsRs_2_lfe\n", __FILE__, __FUNCTION__);
 		} else {
			//set to Large Speakers..no bass-management
			bass_mode = BM_None;
			GUI_INFO("[(%s)%s] Bass Mode:BM_None\n", __FILE__, __FUNCTION__);
		}
	}
	else
	{
		// Sub-woofer disabled..assume none
		pCrtAudioCfg->common.multiChannel.output.lfe = 0;
		if (speaker_size == 3) {
			/* set to Small Speakers..send low-freq to SW anyways
			 * to prevent speaker damage
			 */
			bass_mode = BM_all_2_lfe;
			GUI_INFO("[(%s)%s] Bass Mode:BM_all_2_lfe\n", __FILE__, __FUNCTION__);
		} else if (speaker_size == 2) {
			//set to Large LR speakers..send CLsRs->LR
			bass_mode = BM_CLsRs_2_LR;
			GUI_INFO("[(%s)%s] Bass Mode:BM_CLsRs_2_LR\n", __FILE__, __FUNCTION__);
		} else {
			//set to Large Speakers..no bass-management
			bass_mode = BM_None;
			GUI_INFO("[(%s)%s] Bass Mode:BM_None\n", __FILE__, __FUNCTION__);
		}
	}
	set_bass_mode(bass_mode, FALSE);

	return true;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * setEffectOnAudio(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *     void
 * @return 
 *      void
*/
void set_effect_on_audio(void)
{
	cfgamp->SetConfiguration(cfgamp, NULL, (struct SAudioOutConfig*)pCrtAudioCfg);
}

void set_TV_mode(UINT32 tv_mode)
{
	GUI_INFO("[(%s)%s] set_TV_mode: %d\n", __FILE__, __FUNCTION__, tv_mode);
	bool is_hdmi = ampGlobals->shared->settings.video.common.hdmi.status != CS_Disconnected;
	bool is_component = ampGlobals->shared->settings.video.common.component.status != CS_Disconnected;
	switch(tv_mode) {
		case TVSYS_PAL:		
			if(is_hdmi)
			{
				pCrtVideoCfg->common.hdmi.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50);
			}
			if(is_component)
			{
				pCrtVideoCfg->common.component.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50);
			}
			pCrtVideoCfg->common.composite.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_PAL);
			pCrtVideoCfg->common.composite.display.signal = TV_SIGNAL_TO_BITMASK(TVS_PAL);
			uiTVMode = TVSYS_PAL;
			break;
		case TVSYS_NTSC:					
			if(is_hdmi)
			{
				pCrtVideoCfg->common.hdmi.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60);
			}
			if(is_component)
			{
				pCrtVideoCfg->common.component.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60);
			}	
			pCrtVideoCfg->common.composite.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_NTSC);
			pCrtVideoCfg->common.composite.display.signal = TV_SIGNAL_TO_BITMASK(TVS_NTSC);
			uiTVMode = TVSYS_NTSC;
			break;		
		default:
			APP_ASSERT(0); //should not get here
			break;
	}	
	cfgamp->SetConfiguration(cfgamp, (struct SVideoOutConfig*)pCrtVideoCfg, NULL);
}

void update_encoder_configuration(char *aspect_ratio)
{
	//DFBResult err;
	static int adjustmentCurrEncoder = 0;
	DFBScreenEncoderConfig cfg_enc;

	int j=0;
	for (j=0; j<3; ++j)
	{
		screen->GetEncoderConfiguration(screen, (adjustmentCurrEncoder + j) % 3, &cfg_enc);
		if (cfg_enc.out_signals != DSOS_NONE)
		{
			D_INFO("Using encoder %d...\n", (adjustmentCurrEncoder + j) % 3);
			break;
		}
	}

	//cfg_enc.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG);
	//cfg_enc.delayed_config = true;

	if (!strcmp(aspect_ratio,"not_set")) {
			cfg_enc.aspect_match = DARM_NOT_SET;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_NOT_SET \n");
	} else if (!strcmp(aspect_ratio,"auto")) {
			cfg_enc.aspect_match = DARM_AUTO;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_AUTO \n");
	} else if (!strcmp(aspect_ratio,"letter_box")) {
			cfg_enc.aspect_match = DARM_LETTER_BOX;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_LETTER_BOX \n");
	} else if (!strcmp(aspect_ratio,"pan_scan")) {
			cfg_enc.aspect_match = DARM_PAN_SCAN;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_PAN_SCAN \n");
	} else if (!strcmp(aspect_ratio,"side_panel")) {
			cfg_enc.aspect_match = DARM_SIDE_PANEL;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_SIDE_PANEL \n");
	} else if (!strcmp(aspect_ratio,"crop")) {
			cfg_enc.aspect_match = DARM_CROP;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_CROP \n");
	} else if (!strcmp(aspect_ratio,"zoom")) {
			cfg_enc.aspect_match = DARM_ZOOM;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_ZOOM \n");
	} else if (!strcmp(aspect_ratio,"panorama")) {
			cfg_enc.aspect_match = DARM_PANORAMA;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_PANORAMA \n");
	} else if (!strcmp(aspect_ratio,"fullscreen")) {
			cfg_enc.aspect_match = DARM_NONE;
			D_INFO(">>>>>>>>>>>>>>> AR MATCH: armatch setting - cfg_enc.aspect_match = DARM_NONE = fullscreen \n");
	} else {
		D_INFO(">>>>>>>>>>>>>>> AR MATCH:unrecognized armatch setting !\n");
		return;
	}       
	
	if (cfg_enc.flags & DSECONF_ASPECT_MATCH)
	{
		D_INFO(">>>>>>>>>>>>>>> AR MATCH:force immediate reconfig !\n");
		cfg_enc.delayed_config = false;		// force immediate reconfig
		//DFBCHECK(screen->SetEncoderConfiguration(screen, 0, &cfg_enc));
		if (screen->SetEncoderConfiguration(screen, 0, &cfg_enc) != DFB_OK)
		{
			D_ERROR("SetEncoderConfiguration(0) failed!\n");
		}
	}
	
}
// control close and open for spdif and analog
// disconnect is to true, all of them is disconnected, and they are connected for any cases else
void set_connecting_for_spdif_and_analog(RMbool disconnect)
{
	if(disconnect)
	{
		pCrtAudioCfg->common.spdif.bitstream.status = CS_Disconnected;
		pCrtAudioCfg->common.multiChannel.status = CS_Disconnected;
		pCrtAudioCfg->common.stereo.status = CS_Disconnected;
	}
	else
	{
#if RMCFG_ANALOG_ENGINES == 0
		pCrtAudioCfg->common.spdif.bitstream.status = CS_Disconnected;
		pCrtAudioCfg->common.multiChannel.status = CS_Disconnected;
		pCrtAudioCfg->common.stereo.status = CS_Disconnected;
#else
		pCrtAudioCfg->common.spdif.bitstream.status = CS_Connected;
		pCrtAudioCfg->common.multiChannel.status = CS_Connected;
		pCrtAudioCfg->common.stereo.status = CS_Connected;
#endif
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * set_connect_audio_analog_stereo();
 *
 * <b>DESCRIPTION: </b>
 * 
 * set audio output :analog stereo
 *
 * @param
 *     RMuint16 connect,RMuint16 surround_mode,RMuint16 bass_mode,RMuint16 volume,RMbool isEffect
 * @return 
 *      void
*/
//luunguyen add: analog audio output: stereo  
void set_connect_audio_analog_stereo(RMuint16 connect,enum ESurroundMode surroundMode,enum EBassMode bassMode,RMuint16 volume,RMbool isEffect){
	GUI_INFO("\n===========> set_connect_audio_analog_stereo <============");
	GUI_INFO("\n=========> connect : %d <=======",connect);
	GUI_INFO("\n=========> surroundMode: %d <=======",surroundMode);
	GUI_INFO("\n=========> bassMode: %d <=======",bassMode);
	GUI_INFO("\n=========> volume: %d <=======",volume);
	//set connect
	if(connect == 1){
		GUI_INFO("\n========> connect - : CS_Connected<========");
		pCrtAudioCfg->common.stereo.status = CS_Connected;	
	}else{
		GUI_INFO("\n========> connect - : CS_Disconnected<========");
		pCrtAudioCfg->common.stereo.status = CS_Disconnected;
	}
	
	//set suuround mode
	pCrtAudioCfg->common.stereo.surroundMode	= surroundMode;
	/*if(surround_mode == 1){
		GUI_INFO("\n========> surround_mode - : SRND_ON<========\n");
		pCrtAudioCfg->common.stereo.surroundMode	= SRND_ON;
	}else if(surround_mode == 2){
		GUI_INFO("\n========> surround_mode - : SRND_OFF<========\n");
		pCrtAudioCfg->common.stereo.surroundMode	= SRND_OFF;
	}else{
		GUI_INFO("\n========> surround_mode - : SRND_Default<========\n");
		pCrtAudioCfg->common.stereo.surroundMode	= SRND_Default;
	}*/
	
	//set bass
	pCrtAudioCfg->common.stereo.bassMode 			= bassMode;
	
	//setvolume
	if(volume == 1){
	
	}else if(volume == 2){
	
	}else{
		GUI_INFO("\n========> volume - :Default<========\n");
		pCrtAudioCfg->common.stereo.volume[0] = FULL_SCALE_VOLUME;
		pCrtAudioCfg->common.stereo.volume[1] = FULL_SCALE_VOLUME;
	}
	
	if(isEffect){
		set_effect_on_audio();
	}
}
bool change_framerate_sync(const struct RUA *pRUA,
						  const struct SContentInfo *content,
						  const struct rmcci *drm,
						  const struct SMRUAOutputConfig *settings,
						  const struct SAdjustment *adjustments,
						  struct SMRUAConfig *configuration)
{
	printf("\n\n\n ----  change_framerate_sync ---- \n\n");
		const struct SVideoOutConfigModule *pSettings = &settings->video.common.composite;
		struct SVideoOutConfigModule *pConfiguration = &configuration->output.video.common.composite;
		int is_output = 0; // 0 : composite ; 1 : component; 2 : HDMI
		int onFramerateSync = 3;

		if (configuration->output.video.common.hdmi.status == CS_Connected || configuration->output.video.common.hdmi.status == CS_ConnectedSecure)
		{
			pSettings = &settings->video.common.hdmi;
			pConfiguration = &configuration->output.video.common.hdmi;
			is_output = 2;
		}
		else if (configuration->output.video.common.component.status == CS_Connected || configuration->output.video.common.component.status == CS_ConnectedSecure)
		{
			pSettings = &settings->video.common.component;
			pConfiguration = &configuration->output.video.common.component;
			is_output = 1; // warming
		}

		if(content->format.formatValid) {   // .. and we know the content framerate
			if (content->format.format.image.rateM == content->format.format.image.rateN * 30 )//&&	// .. and the content is 30 fps/60i fps
			{
				if(is_output == 2) { // HDMI or Component is active
					if((pConfiguration->display.system & TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50)) != 0 || (pConfiguration->display.signal & TV_SIGNAL_TO_BITMASK(TVS_1080p24)) != 0) { // .. and the output is not 60Hz
						pConfiguration->display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60);
					}
				}else{  // composite is active
					if((pConfiguration->display.system & TV_SYSTEM_TO_BITMASK(TVSYS_NTSC)) != 0) {  
						pConfiguration->display.system = TV_SYSTEM_TO_BITMASK(TVSYS_PAL);
						pConfiguration->display.signal = TV_SYSTEM_TO_BITMASK(TVS_PAL);
					}
				}
			}else{
				if (content->format.format.image.rateM == content->format.format.image.rateN * 25 )//&&	// .. and the content is 25 fps/50 fps
				{
					if(onFramerateSync != 2) { // MOD : NTSC/PAL , NTSC/PAL/24Hz
						if(is_output == 2) { // HDMI or component is active
							if((pConfiguration->display.system & TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60)) != 0 || (pConfiguration->display.signal & TV_SIGNAL_TO_BITMASK(TVS_1080p24)) != 0) { // .. and the output is not 60Hz
								pConfiguration->display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50);
							}
						}else{ //composite is active
							if((pConfiguration->display.system & TV_SYSTEM_TO_BITMASK(TVSYS_NTSC)) != 0) {  
								pConfiguration->display.system = TV_SYSTEM_TO_BITMASK(TVSYS_PAL);
								pConfiguration->display.signal = TV_SYSTEM_TO_BITMASK(TVS_PAL);
							}
						}
					}
				}else{ // .. and the content is 24 fps
					if(onFramerateSync != 1) { // MOD : NTSC/24Hz , NTSC/PAL/24Hz
						if(is_output == 2) { // HDMI or Component is active
							if((pConfiguration->display.system & TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50)) != 0 || (pConfiguration->display.system & TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60)) != 0) { // .. and the output is not 24Hz
								pConfiguration->display.signal = TV_SYSTEM_TO_BITMASK(TVS_1080p24);
							}
						}
					}
				}
			}
		}

		if (showcfginfo){
			return ConfigPreview(pRUA, content, drm, settings, adjustments, configuration);
		}
		else{
			return true;
		}
			
}


bool change_actual_size(const struct RUA *pRUA,
						  const struct SContentInfo *content,
						  const struct rmcci *drm,
						  const struct SMRUAOutputConfig *settings,
						  const struct SAdjustment *adjustments,
						  struct SMRUAConfig *configuration)
{
		printf("\n\n\n\n ---- change_actual_size: hght =  %d ---- \n\n\n\n",content->format.format.image.height);

		if(content->format.format.image.height > 0 && content->format.format.image.height < 720 && content->format.format.image.width < 1280) {
			printf("\n\n\n\n ---- change_actual_size  sucess---- \n\n\n\n");
			int xScreen = (1280 - content->format.format.image.width)/2 ;
			int yScreen = (720 - content->format.format.image.height)/2;
			amp_table[ampIndex].layer->SetScreenRectangle(amp_table[ampIndex].layer, xScreen, yScreen, content->format.format.image.width, content->format.format.image.height);
		}
		return true;
			
}


// code C for video setting project FPT


RMstatus set_output_mode_have_standard_tv (RMuint8 index) 
{
	GUI_INFO("\t\t===================> set_output_mode_have_standard_tv <====================\n");
	RMstatus status;
	useBackground = TRUE;
	switch(index) {
	case TVMODE_FPT_AUTO: // AUTO
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_AUTO **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_1080i,false);
		break;
	case TVMODE_FPT_NTSC: // NTSC
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_NTSC **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_720p,false);
		break;
	case TVMODE_FPT_PAL: // PAL
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_PAL **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_720p,true);
		break;
	case TVMODE_FPT_720P_60HZ: // 720P_60HZ
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_720P_60HZ **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_720p,false);
		break;
	case TVMODE_FPT_720P_50HZ: // 720P_50HZ 	
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_720P_50HZ **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_720p,true);
		break;
	case TVMODE_FPT_1080I_60HZ: // 1080I_60HZ 				
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_1080I_60HZ **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_1080i,false);
		break;
	case TVMODE_FPT_1080I_50HZ: // 1080I_50HZ 				
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_1080I_50HZ **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_1080i,true);
		break;
	case TVMODE_FPT_1080P_60HZ: // 1080P_60HZ 				
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_1080P_60HZ **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_1080p,false);
		break;
	case TVMODE_FPT_1080P_50HZ: // 1080P_50HZ 				
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_1080P_50HZ **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_1080p,true);
		break;
	case TVMODE_FPT_1080P_24HZ: // 1080p24 				
		printf("\n\n\n ********* set_output_mode_have_standard_tv = TVMODE_FPT_1080P_24HZ **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_1080p24,true);
		break;
	default:
		printf("\n\n\n ********* set_output_mode_have_standard_tv = default **************\n\n\\n");
		status = set_hdtv_resolution_standard_tv(DSOS_1080i,false);
		break;
	}
	return status ;
}


RMstatus set_hdtv_resolution_standard_tv(DFBScreenOutputSignals signal, bool bType) 
{
	GUI_INFO("\t\t===================> set_hdtv_resolution_standard_tv <====================\n");

	RMstatus status = RM_OK;
	DFBScreenEncoderConfig enc_cfg;
	DFBScreenOutputConfig out_cfg;
	bool hdmi = ampGlobals->shared->settings.video.common.hdmi.status != CS_Disconnected;
	bool component = ampGlobals->shared->settings.video.common.component.status != CS_Disconnected;
	bool analog = ampGlobals->shared->settings.video.common.composite.status != CS_Disconnected;

	// TEMPORARY - hack around RUA NTSC Zoom problem
	if (signal == DSOS_NONE ){
		return RM_ERROR;		// don't mute anymore
	}
	// end TEMPORARY
	//D_INFO("\n\n   set_hdtv_resolution(DFBScreenOutputSignals signal) ! \n\n"); 
	if (hdmi)
	{
		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG);
		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG | DSECONF_TV_STANDARD);
		if (bType) {
			enc_cfg.tv_standard = DSETV_HDTV50;		
		}else{
			enc_cfg.tv_standard = DSETV_HDTV60;			
		}
		enc_cfg.out_signals = signal;
		enc_cfg.delayed_config = true;
		if (screen->SetEncoderConfiguration(screen, 0, &enc_cfg) != DFB_OK)
		{
			D_ERROR("SetEncoderConfiguration(0) failed!\n");
			component = false;
			analog = false;
		}
		else
		{
			out_cfg.flags = (DFBScreenOutputConfigFlags)(DSOCONF_SIGNALS | DSOCONF_DELAYED_CONFIG);
			out_cfg.out_signals = signal;
			out_cfg.delayed_config = component || analog;
			if (screen->SetOutputConfiguration(screen,  0, &out_cfg) != DFB_OK)
			{
				D_ERROR("SetOutputConfiguration(0) failed!\n");
				component = false;
				analog = false;
			}
		}
	}

	if (component)
	{
		if (signal & (DSOS_1080p | DSOS_1080p24))
		{
			// not component standard signals, use the next best
			if (hdmi && analog)
				// only 2 independent output timings available, use the analog one
				signal = DSOS_SDTV;
			else
				// independent output timing available, use the best signal
				signal = DSOS_1080i;
		}
		else if (signal == DSOS_EDID)
			signal = DSOS_1080i;

		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG);
		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG | DSECONF_TV_STANDARD);
		if (bType) {			
			enc_cfg.tv_standard = DSETV_HDTV50;//PAL System			
		}else{
			enc_cfg.tv_standard = DSETV_HDTV60;//NTSC System			
		}
		enc_cfg.out_signals = signal;
		enc_cfg.delayed_config = true;
		if (screen->SetEncoderConfiguration(screen, 1, &enc_cfg) != DFB_OK)
		{
			D_ERROR("SetEncoderConfiguration(1) failed!\n");
			analog = false;
		}
		else
		{
			out_cfg.flags = (DFBScreenOutputConfigFlags)(DSOCONF_SIGNALS | DSOCONF_DELAYED_CONFIG);
			out_cfg.out_signals = signal;
			out_cfg.delayed_config = analog;
			if (screen->SetOutputConfiguration(screen,  1, &out_cfg) != DFB_OK)
			{
				D_ERROR("SetOutputConfiguration(1) failed!\n");
				analog = false;
			}
		}
	}

	if (analog)
	{
		screen->GetEncoderConfiguration(screen, 2, &enc_cfg);		// for TV standard

		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG);
		enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_OUT_SIGNALS | DSECONF_DELAYED_CONFIG | DSECONF_TV_STANDARD);
		if (bType) {
			enc_cfg.tv_standard = DSETV_PAL;			
		}else{
			enc_cfg.tv_standard = DSETV_NTSC;		
		}
		enc_cfg.out_signals = signal ? (enc_cfg.tv_standard==DSETV_NTSC ? DSOS_NTSC : DSOS_PAL) : DSOS_NONE;
		enc_cfg.delayed_config = true;
		if (screen->SetEncoderConfiguration(screen, 2, &enc_cfg) != DFB_OK)
		{
			D_ERROR("SetEncoderConfiguration(2) failed! 1\n");
		}
		else
		{
			out_cfg.flags = (DFBScreenOutputConfigFlags)(DSOCONF_SIGNALS | DSOCONF_DELAYED_CONFIG);
			out_cfg.out_signals = enc_cfg.out_signals;
			out_cfg.delayed_config = false;
			if (screen->SetOutputConfiguration(screen,  2, &out_cfg) != DFB_OK)
			{
				D_ERROR("SetOutputConfiguration(2) failed! 2\n");
			}
		}
	}
		
	if (signal != DSOS_NONE)
	{
		out_cfg.flags = DSOCONF_SIGNALS;
		if ((hdmi && (screen->GetOutputConfiguration(screen, 0, &out_cfg) == DFB_OK)) ||
			(component && (screen->GetOutputConfiguration(screen, 1, &out_cfg) == DFB_OK)) ||
			(analog && (screen->GetOutputConfiguration(screen, 2, &out_cfg) == DFB_OK)))
		{
			unsigned i;
			DirectFBScreenOutputSignalsNames(signalNamesTab);
			
			signal = out_cfg.out_signals;
			for (i=0; i<sizeof(signalNamesTab)/sizeof(signalNamesTab[0]); i++)
				if (signalNamesTab[i].signal == signal)
				{
					D_INFO("New nominal output signal: %s\n", signalNamesTab[i].name);
					break;
				}
		}
		else
		{
			D_ERROR("GetOutputConfiguration failed on all encoders!\n");
		}
		last_hdtv_signal = signal;
		
		//get_signal_resolution(signal);
		/*
		 *	Tri hide these line, so we don't need use background layer.
		 */
#if 0
		if (useBackground)		// if background is enabled, reconfigure it
		{
			
			if  ( ( !mediaTab[ampIndex].amp) ||
				( mediaTab[ampIndex].status.generic.mediaSpace == MEDIA_SPACE_LINEAR_MEDIA || 
				mediaTab[ampIndex].status.generic.mediaSpace == MEDIA_SPACE_CDDA))
			{	
				enable_background_layer();
				
			}// defaults to (xres,yres)
		}
#endif
	}
	return status ;
}


void set_TVType_FPT(RMuint16 as_type) 
{
	switch(as_type) {
	case 0: // 4:3
		printf(" \n--- 1111111set_TVType : 4:3  ---\n");
		pCrtVideoCfg->common.hdmi.arMatch =
		pCrtVideoCfg->common.component.arMatch = ARM_SidePanel;
		pCrtVideoCfg->common.composite.arMatch = ARM_NotSet;

		pCrtVideoCfg->common.hdmi.display.aspectRatio =
		pCrtVideoCfg->common.component.display.aspectRatio = TVAR_16x9;
		pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_4x3;

		break;
	case 1: // 16:9
		printf(" \n--- 11111111set_TVType : 16:9  ---\n");
		pCrtVideoCfg->common.hdmi.display.aspectRatio = 
		pCrtVideoCfg->common.component.display.aspectRatio = 
		pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_16x9;
								

		pCrtVideoCfg->common.hdmi.arMatch =
		pCrtVideoCfg->common.component.arMatch =
		pCrtVideoCfg->common.composite.arMatch = ARM_No;

		break;
	case 2: // 16: 10
		printf(" \n--- set_TVType_FPT : 16:10  ---\n");
		pCrtVideoCfg->common.hdmi.display.aspectRatio = 
			pCrtVideoCfg->common.component.display.aspectRatio =
			pCrtVideoCfg->common.composite.display.aspectRatio = TVAR_16x9;
		break;

	default:
		APP_ASSERT(0); //should not get here
		break;
	}
	
	cfgamp->SetConfiguration(cfgamp, (struct SVideoOutConfig*)pCrtVideoCfg, NULL);
}

void set_VideoZoom_FPT(RMuint16 as_type) 
{
	switch(as_type) {
	case 0:  //actual size
		printf(" \n--- set_VideoZoom_FPT : actual size  ---\n");
		onActualSize = true;
		
		break;
	case 1: //Fit to screen
		printf(" \n--- set_VideoZoom_FPT : Fit to screen  ---\n");
		onActualSize = false;
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = 
			pCrtVideoCfg->common.composite.arMatch = ARM_SidePanel;
	
		break;
	case 2: //crop
		printf(" \n--- set_VideoZoom_FPT : crop  ---\n");
		onActualSize = false;
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = 
			pCrtVideoCfg->common.composite.arMatch = ARM_Crop;
	
		break;
	case 3: // fullscreen
		printf(" \n--- set_VideoZoom_FPT : fullscreen  ---\n");
		onActualSize = false;
		pCrtVideoCfg->common.hdmi.arMatch = 
			pCrtVideoCfg->common.component.arMatch = 
			pCrtVideoCfg->common.composite.arMatch = ARM_Panorama;
	
		break;

	default:
		APP_ASSERT(0); //should not get here
		break;
	}
	
	cfgamp->SetConfiguration(cfgamp, (struct SVideoOutConfig*)pCrtVideoCfg, NULL);
}

void set_color_space_FPT(RMuint16 as_type)
{
	switch(as_type) {
	case 0: //sRGB(16-225)
		printf(" \n--- set_color_space_FPT : sRGB(16-235)  ---\n");
		pCrtVideoCfg->common.hdmi.display.colorSpace = CS_RGB_16_235;
		pCrtVideoCfg->common.component.display.colorSpace = 
			pCrtVideoCfg->common.composite.display.colorSpace = CS_Default; 
		//pCrtVideoCfg->common.hdmi.display.colorSpace = CS_RGB_16_235;
		break;
	case 1: //sRGB(0-225)
		printf(" \n--- set_color_space_FPT : sRGB(0-225)  ---\n");
		//pCrtVideoCfg->common.hdmi.display.colorSpace = CS_RGB_0_255;
		pCrtVideoCfg->common.hdmi.display.colorSpace = CS_RGB_0_255;
		pCrtVideoCfg->common.component.display.colorSpace = 
			pCrtVideoCfg->common.composite.display.colorSpace = CS_Default; 
		break;
	default:
		APP_ASSERT(0); //should not get here
		break;
	}
	cfgamp->SetConfiguration(cfgamp, (struct SVideoOutConfig*)pCrtVideoCfg, NULL);
}

void set_Framerate_sync(int _type){
	//onActualSize = true;
	onFramerateSync = _type;
}



char *mediaType2StringForFPTProject(TMediaType type)
{
        char *text = NULL;
        switch (type)
        {
                case MTYPE_ELEM_NONE     : // 0x0000  ///< no elementary data exists or the format is irrelevant
                        text = "none";
                        break;
                case MTYPE_ELEM_UNKNOWN  : // 0x00FF  ///< elementary data exists, is relevant but the format is unknown
                        text =  "unknown";
                        break;

                case MTYPE_ELEM_MPEG1    : // 0x0011
                        text = "MPEG1";// "MPEG1 video";
                        break;

                case MTYPE_ELEM_MPEG2    : // 0x0012
                        text = "MPEG2";// "MPEG2 video";
                        break;

                case MTYPE_ELEM_MPEG4    : // 0x0014
                        text = "MPEG4";//"MPEG4 video";
                        break;

                case MTYPE_ELEM_AVC      : // 0x001A
                        text = "AVC";//"MPEG4 AVC video";
                        break;

                case MTYPE_ELEM_VC1      : // 0x0020
                        text = "VC1";// "VC1 video";
                        break;

                case MTYPE_ELEM_DIVX3    : // 0x0033
                        text = "DIVX3";// "DIVX 3 video";
                        break;

                case MTYPE_ELEM_DIVX4    : // 0x0034
                        text = "DIVX4"; //"DIVX 4 video";
                        break;

//        case MTYPE_ELEM_MPA      : // 0x0040      ///< any MPEG audio
//            text = "MPEG audio";
//            break;

                case MTYPE_ELEM_MP1      : // 0x0041
                        text = "MPEG1";// "MPEG1 Layer 1-2 audio";
                        break;

//        case MTYPE_ELEM_MP2      : // 0x0042
//            text = "MPEG1 Layer 2 audio";
//            break;

                case MTYPE_ELEM_MP3      : // 0x0043
                        text = "MPEG1";// "MPEG1 Layer 3 (MP3) audio";
                        break;

                case MTYPE_ELEM_MP2MC    : // 0x0044
                        text = "MPEG2";// "MPEG2 (MC) audio";
                        break;

                case MTYPE_ELEM_AAC      : // 0x0045
                        text = "AAC";// "MPEG2 AAC audio";
                        break;

                case MTYPE_ELEM_AACP     : // 0x0046
                        text = "AAC";// "MPEG2 AAC+ audio";
                        break;

                case MTYPE_ELEM_AC3      : // 0x0053
                        text = "AC3";// "Dolby Digital (AC3) audio";
                        break;

                case MTYPE_ELEM_DDP      : // 0x0054
                        text = "DDP";// "Dolby Digital Plus audio";
                        break;

                case MTYPE_ELEM_DLLS     : // 0x0055
                        text = "?";//"? audio";
                        break;

                case MTYPE_ELEM_DTS      : // 0x0060
                        text = "DTS";// "DTS audio";
                        break;

                case MTYPE_ELEM_DTSHD    : // 0x0061
                        text = "DTS-HD";// "DTS-HD audio";
                        break;

                case MTYPE_ELEM_PCM      : // 0x0070
                        text = "PCM";// "PCM audio";
                        break;

                case MTYPE_ELEM_WMA      : // 0x0080
                        text = "WMA";
                        break;

                case MTYPE_ELEM_WMAPRO   : // 0x0081
                        text = "WMA-PRO";
                        break;

                case MTYPE_ELEM_SPU      : // 0x0090
                        text = "SPU";// "SD graphic subtitles";
                        break;

                case MTYPE_ELEM_PG    : // 0x0091
                    text = "PG";// "HD graphic subtitles";
                        break;

                case MTYPE_ELEM_IG       : // 0x00A0
                        text = "IG";// "Interactive graphics";
                        break;

                case MTYPE_ELEM_TS       : // 0x00A2
                        text = "TS";// "Text subtitles";
                        break;

                case MTYPE_ELEM_JPEG     : // 0x00B0
                        text = "JPEG";// "JPEG graphics";
                        break;

                case MTYPE_ELEM_GIF      : // 0x00B1
                        text = "GIF"; //"GIF graphics";
                        break;

                case MTYPE_ELEM_PNG      : // 0x00B2
                        text = "PNG";// "PNG graphics";
                        break;

                case MTYPE_ELEM_BMP      : // 0x00B3
                        text = "BMP";// "BMP graphics";
                        break;

                case MTYPE_ELEM_TIFF     : // 0x00B4
                        text = "TIFF";// "TIFF graphics";
                        break;

                case MTYPE_ELEM_PIXMAP   : // 0x00BF  ///< uncompressed pixel map
                        text = "RAW";// "RAW graphics";
                        break;

                case MTYPE_ELEM_ASCII    : // 0x00C1
                        text = "ASCII";// "ASCII text";
                        break;

                case MTYPE_ELEM_FONT   : // 0x00C2  ///< TrueType font data
                        text = "Font data";
                        break;

                case MTYPE_ELEM_VIDEO    : // 0x00F1  ///< any video type
                        text = "Video";
                        break;

                case MTYPE_ELEM_AUDIO    : // 0x00F2  ///< any audio type
                        text = "Audio";
                        break;

                case MTYPE_ELEM_GRAPHICS : // 0x00F3  ///< any graphics type
                        text = "Graphics";
                        break;

                case MTYPE_CONT_ASF      :
                        text = "Asf";
                        break;

                case MTYPE_CONT_AVI      :
                        text = "Avi";
                        break;

                default:
                        text = "invalid value";
                        break;
        }
        return text;
}




