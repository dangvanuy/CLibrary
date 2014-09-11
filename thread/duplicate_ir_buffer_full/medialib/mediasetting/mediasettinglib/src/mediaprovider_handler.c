/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/	

union UMSStatus            *m_upMyAppStatus = NULL;
union UMSResult            *m_upMyAppResult = NULL;
UINT64 m_pMediaTime = 0;
UINT64 m_pPresentationTime = 0;
bool isStartingAmp = false;
bool isPlayingMediaFile = false;
RMuint16 uiStreamIndex;		// Index of subtitle Internal
static RMuint8 				m_uiVolume = 70;
bool                      m_isMute = false;

#ifndef NO_DTV_SUPPORT
int pip_x = 0, pip_y = 0;				
bool isSetPiPRect = false;
#endif //NO_DTV_SUPPORT
// for dlna
#ifdef DLNA_SUPPORT
mpFileType_DLNA_DEMO m_eFileType = DIR_DLNA_DEMO_FILETYPE_UNKNOWN;
#endif // DLNA_SUPPORT

//picture preview slideshow
int x_preview;
int y_preview;
int h_preview;
int w_preview;

// for stream navigator
struct SLPBRebufferControl m_rebufferControl;
int m_rebufferControl_mode;

int m_stc_offset_ms ;
int m_prebuf_max ;
bool m_lpb_SeekSupport = true;
int m_lpb_indexMode = -1; // none set, use value default
// end 

char *m_mediaType;

/***********************************************
************************************************/
static RMuint32 VolumeTable[VOLUME_INDEX_0DB] =
{
		0x00000000,	// mute
		0x00100000, 0x0011f59c, 0x001428a4, 0x0016a09f, 0x001965ff, 0x001c823e, // -48dB ... -43dB
		0x00200000, 0x0023eb35, 0x00285145, 0x002d413c, 0x0032cbfc, 0x0039047b, // -42dB ... -37dB
		0x00400000, 0x0047d66a, 0x0050a28b, 0x005a8279, 0x006597fa, 0x007208f8, // -36dB ... -31dB
		0x00800000, 0x008facd6, 0x00a14518, 0x00b504f3, 0x00cb2ff5, 0x00e411f0, // -30dB ... -25dB
		0x01000000, 0x011f59ac, 0x01428a2f, 0x016a09e6, 0x01965fea, 0x01c823e0, // -24dB ... -19dB
		0x02000000, 0x023eb358, 0x0285145f, 0x02d413cd, 0x032cbfd5, 0x039047c1, // -18dB ... -13dB
		0x04000000, 0x047d66b1, 0x050a28be, 0x05a82799, 0x06597fa9, 0x07208f82, // -12dB ...  -7dB
		0x08000000, 0x08facd62, 0x0a14517d, 0x0b504f34, 0x0cb2ff53, 0x0e411f04, //  -6dB ...  -1dB
		0x10000000, 0x11f59ac4, 0x1428a2fa, 0x16a09e67, 0x1965fea6, 0x1c823e08, //   0dB ...   5dB
		0x20000000, 0x23eb3589, 0x285145f5, 0x2d413ccf, 0x32cbfd4c, 0x39047c10, //   6dB ...  11dB
		0x40000000, 0x47d66b10, 0x50a28be7, 0x5a82799a, 0x6597fa95, 0x7208f81d, //  12dB ...  17dB
		0x80000000, 0x8facd61c, 0xa14517c9, 0xb504f32f, 0xcb2ff523, 0xe411f032, //  18dB ...  23dB
		0xffffffff  // ~24dB

};


/**
 * 
 * <b>FUNCTION: </b>
 *
 * ParentalLevelChangeCallback(int level, void *context)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int level, void *context
 * @return 
 *		 int
*/
#ifndef NO_RED_SUPPORT
int parental_level_change_callback(int level, void *context)
{
	isShowPassWordConfirm = TRUE;
	parentalChangeOK = -1;
	GUI_INFO("=================> ParentalLevelChangeCallback <===================\n");
	while (parentalChangeOK == -1)
	{
		sleep(1);
	}
	return parentalChangeOK;
}
#endif
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			SetAppCallBack(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       void
 * @return 
 *		 void
*/
void set_app_call_back(void)
{
	DFBResult err;
	DFBAdvancedMediaProviderDescription desc = { 0 };
	DFBCHECK(gdfb->GetInterface(gdfb, "IAdvancedMediaProvider", NULL, (void*)(MAX_AMP_INSTANCES - 1), (void *)&cfgamp));	
	// set up the callbacks
	#ifndef MLB_SUPPORT
	cfgamp->SetApplicationCallbacks(cfgamp, (void *)&process_config_preview, (void *)&process_config_confirm , CMD_INVALID_CALLBACK, CMD_INVALID_CALLBACK);
	#endif // #ifndef MLB_SUPPORT
	cfgamp->GetDescription(cfgamp, &desc);	
	ampGlobals = (struct SGlobals *)desc.privateInfo;	
	//pCrtVideoCfg = (struct SMRUAVideoOutConfig *)&videoConfigTab[0];

	pCrtVideoCfg = (struct SMRUAVideoOutConfig *)&ampGlobals->shared->preferences.video;
	pCrtDRMCfg = (struct rmcci *)&ampGlobals->shared->restrictions;


	CrtAudioCfg = (struct SMRUAAudioOutConfig)audioConfigTab[0];
	pCrtAudioCfg = (struct SMRUAAudioOutConfig*)&CrtAudioCfg;
	// init status of hdmi
	hdmi_out_status = ampGlobals->shared->configuration.output.video.common.hdmi.status;
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * StartMediaPresentation(IDirectFB *dfb, int ampIndex, int *pEventFD, int *pLayerID,
 *							IAdvancedMediaProvider **ppAMP, DFBDimension *pImgSz)
 *
 * <b>DESCRIPTION: </b>
 * 
 * start media provider
 * 
 * @param
 *       IDirectFB *dfb, int ampIndex, int *pEventFD, int *pLayerID,
 *  IAdvancedMediaProvider **ppAMP, DFBDimension *pImgSz
 * @return 
 *		 DFBResult
*/
// Opens the AMP specified in the presIndex-th set of command line parameters
// Returns its event file descriptor and corresponding layer ID (-1 for surface playback or video-less AMP)
DFBResult start_media_presentation(IDirectFB *dfb, int ampIndex, int *pEventFD, int *pLayerID,
								 IAdvancedMediaProvider **ppAMP, DFBDimension *pImgSz)
{
	DFBResult err;
	TResultCode res;
	struct SMediaSession *media;
	IDirectFBImageProvider *img = NULL;
	bool alreadyOpened = FALSE;

	if (ampIndex >= mediaCnt)
		return DFB_LIMITEXCEEDED;

	media = &mediaTab[ampIndex];
	pImgSz->w = 0;
	pImgSz->h = 0;
	if (media->useImg)
	{
		media->ampEvent = NULL;
		media->amp = NULL;
	}
	else		// use AMP
	{
		if (media->use_pfs)
		{
			if (media->amp_pfs != NULL)
			{
				alreadyOpened = TRUE;
				D_INFO("PFS: Media is already prebuffered\n");
			}
			else
				D_INFO("PFS: Media is NOT prebuffered!!!\n");

			// swap the amp
			IAdvancedMediaProvider *amp = media->amp_pfs;
			media->amp_pfs = media->amp;
			media->amp = amp;

			// swap the event buffer
			IDirectFBEventBuffer   *amp_event = media->ampEvent_pfs;
			media->ampEvent_pfs = media->ampEvent;
			media->ampEvent = amp_event;

			// reset session-specific fields
			media->running = false;
			media->repeatTitle = 0;
			media->repeatChapter = 0;
		}
		
		if (!media->amp)
		{
			// create an Advanced Media Provider (tm)
			void *dummy; // just to avoid stupid compiler warning
			err = dfb->GetInterface(dfb, "IAdvancedMediaProvider", NULL, (void*)media->ampID, &dummy);
			if (err != DFB_OK)
			{
				D_ERROR(/*stderr,*/ "Advanced Media Provider instantiation failed!!!\n");

				return err;
			}
			media->amp = dummy;

			// reset session-specific fields
			media->running = false;
			media->repeatTitle = 0;
			media->repeatChapter = 0;
			memset(&media->status, 0, sizeof(media->status));

			// get the event buffer
			err = media->amp->GetEventBuffer(media->amp, &media->ampEvent);
			if (err != DFB_OK)
			{
				D_ERROR(/*stderr,*/ "Could not get the AMP event buffer!!!\n");
				end_media_presentation(ampIndex, true);

				return err;
			}
		}

		// get the event buffer
		// This is needed here for Reopen Media to work, because in that case media->amp is NOT released and, consequently,
		// no media->ampEvent would be created
		if (!media->ampEvent)
		{
			// get the event buffer
			err = media->amp->GetEventBuffer(media->amp, &media->ampEvent);
			if (err != DFB_OK)
			{
				D_ERROR(/*stderr,*/ "Could not get the AMP event buffer!!!\n");
				end_media_presentation(ampIndex, true);

				return err;
			}
		}
	}
	if (media->useDataBuffer)
	{
		DFBDataBufferDescription desc = { DBDESC_FILE , media->url,{ NULL, 0}};

		err = dfb->CreateDataBuffer( dfb, &desc, &media->dataBuffer );

		if (err != DFB_OK)
		{
			D_ERROR(/*stderr,*/ "Could not open data buffer for file %s!!!\n", media->url);
			end_media_presentation(ampIndex, true);

			return err;
		}

		goto _set_db;
	}
	else if (media->useStreamingDataBuffer)
	{
		char thread_name[64];

		media->fileToStream = open(media->url, O_RDONLY | O_LARGEFILE);
		if (media->fileToStream < 0)
		{
			D_ERROR(/*stderr,*/ "Could not open data buffer for file %s!!!\n", media->url);
			end_media_presentation(ampIndex, true);

			return DFB_FAILURE;
		}
		err = dfb->CreateDataBuffer( dfb, NULL, &media->dataBuffer );

		if (err != DFB_OK)
		{
			D_ERROR(/*stderr,*/ "Could not open data buffer for file %s!!!\n", media->url);
			end_media_presentation(ampIndex, true);

			return err;
		}

		sprintf(thread_name, "DataBufferFeeder_%d", ampIndex);
		media->dataBufferFeeder = direct_thread_create(DTT_CRITICAL, (DirectThreadMainFunc)data_buffer_thread, media, thread_name);

		_set_db:
		if (media->useImg)
		{
			err = media->dataBuffer->CreateImageProvider(media->dataBuffer, &img);
		}
		else
		{
			media->amp->SetDataBuffer(media->amp, media->dataBuffer, &media->format);
			err = media->amp->OpenMedia(media->amp, NULL, &media->format, media->openParam);
		}
	}
	else
	{
		if (media->useImg)
		{
			GUI_INFO("\n \033[35m[%s] [%d] \033[0m \n", __FUNCTION__, __LINE__);
			err = dfb->CreateImageProvider(dfb, media->url, &img);
		}
		else if (alreadyOpened)	// if already opened by prefetch_data
		{
			err = DFB_OK;
		}
		else
		{
			if (media->cFormat.mediaType != MTYPE_ELEM_NONE)
			{
				DFBDataBufferDescription desc = { DBDESC_FILE , media->cUrl,{ NULL, 0}};
				
				err = dfb->CreateDataBuffer( dfb, &desc, &media->cDataBuffer );
				
				if (err != DFB_OK)
				{
					D_ERROR(/*stderr,*/ "Could not open data buffer for file %s!!!\n", media->url);
					end_media_presentation(ampIndex, true);
					
					return err;
				}
				media->amp->SetDataBuffer(media->amp, media->cDataBuffer, &media->cFormat);
				
			}
			
			err = media->amp->OpenMedia(media->amp, media->url, &media->format, media->openParam);			
		}
	}

	if (err != DFB_OK)
	{
		GUI_INFO("\n \033[35m[%s] [%d] \033[0m \n", __FUNCTION__, __LINE__);
		D_ERROR(/*stderr,*/ "Could not open media %s!!!\n", media->url);
		end_media_presentation(ampIndex, true);

		return err;
	}

	if (media->useImg)
	{
		*pEventFD = -1;

		D_INFO("*********** Playing back an image ! *******************\n\n");
		if (media->dest.x >= 0)
		{
			DFBSurfaceDescription sdesc;
			DFBRectangle rect;
			int w, h;
			DVFrameCallback callback = NULL;
			void *ctx = NULL;
			IDirectFBSurface *surface = get_destination(ampIndex, media->bdst, media->vdst, &media->dest, &callback, &ctx);

			if (!surface)
			{
				D_ERROR("Surface destination not available, abort!!!\n");
				return DFB_FAILURE;
			}

			// size magic
			img->GetSurfaceDescription(img, &sdesc);

			if (media->dest.w >= sdesc.width && media->dest.h >= sdesc.height)
			{
				w = sdesc.width;
				h = sdesc.height;
			}
			else
			{
				if (media->dest.w * sdesc.height <= media->dest.h * sdesc.width)
				{
					w = media->dest.w;
					h = w * sdesc.height / sdesc.width;
				}
				else
				{
					h = media->dest.h;
					w = h * sdesc.width / sdesc.height;
				}
			}

			rect.x = media->dest.x + (media->dest.w - w) / 2;
			rect.y = media->dest.y + (media->dest.h - h) / 2;
			rect.w = w;
			rect.h = h;
			media->img_size.w = w;
			media->img_size.h = h;

			D_INFO("Rendering a %dx%d picture at (%d,%d,%d,%d); stored size=%dx%d\n", sdesc.width, sdesc.height, rect.x, rect.y, rect.w, rect.h,
				   media->img_size.w, media->img_size.h);

			surface->Clear(surface, 0, 0, 0, 255);		// opaque black
			err = img->RenderTo(img, surface, &rect);

			img->Release(img);
			surface->Release(surface);

			if (callback)
				callback(ctx);
		}
		else
		{
			D_ERROR("No destination surface specified for image renderer, abort!!!\n");
			err = DFB_FAILURE;		// we can only play back to a surface
		}
	}
	else	// AMP
	{
		// wait for the result
		D_INFO("Waiting for OpenMedia confirmation...\n");
		media->ampEvent->WaitForEvent(media->ampEvent);

		// consume the event
		{
			DFBEvent event;

			media->ampEvent->GetEvent(media->ampEvent, &event);
		}
		APP_ASSERT(media->ampEvent->HasEvent(media->ampEvent) != DFB_OK);

		// check the result
		media->status.generic.size = sizeof(union UMSStatus);
		media->status.generic.mediaSpace = MEDIA_SPACE_UNKNOWN;
		process_media_events(ampIndex); // upload the status and perform the standard processing

		res = media->status.generic.lastCmdResult;

		if (IS_ERROR(res))
		{
			D_ERROR(/*stderr,*/ "Could not open media %d from %s!!!\n", ampIndex, media->url);
			end_media_presentation(ampIndex, true);

			if (TRESULT_VALUE(res) == RESULT_BUSY)
				return DFB_BUSY;
			else if (TRESULT_VALUE(res) == RESULT_UNIMPLEMENTED)
			{
				isShowWarning = PLAYBACK_CANCELLED_DUE_TO_UNSUPPORTED_DISC;
				return DFB_NOIMPL;
			}
			else if (TRESULT_VALUE(res) == RESULT_PROTECTED) {
				// TODO: more specific message
				isShowWarning = PLAYBACK_CANCELLED_DUE_TO_SECURITY_VIOLATION_ONLY;
				return DFB_UNSUPPORTED;
			}
			else
			{
				isShowWarning = PLAYBACK_CANCELLED_DUE_TO_UNSUPPORTED_DISC;
				return DFB_FAILURE;
			}
				
		}

		// verify that is a supported media space
		if (((unsigned int)media->status.generic.mediaSpace >= sizeof(msRTTab)/sizeof(msRTTab[0])) ||
			(msRTTab[media->status.generic.mediaSpace] == MEDIA_SPACE_UNKNOWN))
		{
			D_ERROR("Unknown media space %d, abort!!!\n", media->status.generic.mediaSpace);
			end_media_presentation(ampIndex, true);
			return DFB_UNSUPPORTED;
		}

		//
		// set up the run-time parameters
		GUI_INFO("***************************************************************\n");
		GUI_INFO("\t media->status.generic.mediaSpace = %d \t \n", media->status.generic.mediaSpace);
		GUI_INFO("***************************************************************\n");
		media->msRT = msRTTab[(unsigned int)media->status.generic.mediaSpace];

		// prepare the command and result structures
		media->result.generic.mediaSpace = ampCommand.generic.mediaSpace = media->status.generic.mediaSpace;
	
		// not needed, it is set by the specific Navigator
		//media->result.generic.dataSize = sizeof(union UMSResult);

		// THIS IS WRONG !!!!!!!!!!!!!!!!!!!
		//ampCommand.generic.dataSize = sizeof(union UMSCommand);

		// command size IS media dependent,
		// otherwise bad things will happen when copying the command in CPlaybackControl::ExecuteCmd()


		switch (media->result.generic.mediaSpace)
		{
			case MEDIA_SPACE_LINEAR_MEDIA:
				ampCommand.generic.dataSize = sizeof(struct SLPBCommand);
				break;

#ifndef NO_DTV_SUPPORT				
			case MEDIA_SPACE_DTV:
				ampCommand.generic.dataSize = sizeof(struct SDTVCommand);
				break;
#endif // DTV_SUPPORT				
#ifndef NO_RED_SUPPORT
			case MEDIA_SPACE_CDDA:
				ampCommand.generic.dataSize = sizeof(struct SCDDACommand);
				break;
#ifndef NO_VCD_SUPPORT	
			case MEDIA_SPACE_VCD:
				ampCommand.generic.dataSize = sizeof(struct SVCDCommand);
				break;
#endif
			case MEDIA_SPACE_DVD:
				ampCommand.generic.dataSize = sizeof(struct SDVDCommand);
				break;

			case MEDIA_SPACE_DVD_VR:
				ampCommand.generic.dataSize = sizeof(struct SVRCommand);
				break;
#endif
#ifndef NO_BLUE_SUPPORT
			case MEDIA_SPACE_BDJ:
			case MEDIA_SPACE_BLU_RAY:
				ampCommand.generic.dataSize = sizeof(struct SBRDCommand);
				break;

			case MEDIA_SPACE_BDRE:
				ampCommand.generic.dataSize = sizeof(struct SBDRECommand);
				if(media->openParam)
					printf("media->openParam = %s\n", (char*)media->openParam);
				else
					printf("media->openParam == NULL\n");
				if (media->status.generic.changes & SBDRESTATUS_REQ_PIN)
				{
					D_ERROR("\n\nMedia %d from %s is protected BD-RE! Enter PIN.\n\n", ampIndex, media->url);
					set_reqpin_status(true);
					end_media_presentation(ampIndex, true);
					return DFB_FAILURE;
				}
				else if ((media->status.generic.flags & SBDRESTATUS_DIR) && (media->status.generic.changes & SBDRESTATUS_DIR))
				{
					if (media->status.bdre.bdavDir.resume_PlayList_file_id != 0xFFFFFFFF)
					{
						int idx;
						TBDREItemID *p_play_list_id = media->status.bdre.bdavDir.PlayListID;

						for (idx = 0; idx < media->status.bdre.bdavDir.number_of_PlayLists; idx++, p_play_list_id++ )
						{
							if (*p_play_list_id == media->status.bdre.bdavDir.resume_PlayList_file_id)
							{
								selected_play_list_idx = idx + 1;
								break;
							}
						}
					}
				}

				// this should be -tpar, i.e. the PIN
				if (media->openParam)
				{
					RMint32 pin;
					unsigned int ci;

					pin = atoi((const char *)media->openParam);

					// change the default displayed PIN with the user supplied one for the commands that need it
					for (ci=0; ci<media->msRT->cmdCnt; ci++)
					{
						switch (media->msRT->cmdList[ci].cmd)
						{
							case BDRECmd_play_pl_time:
							case BDRECmd_play_pl_mark:
							case BDRECmd_get_pl_info:
                                media->msRT->cmdList[ci].defParam2 = pin;
								break;
							case BDRECmd_resume_bookmark:
								media->msRT->cmdList[ci].defParam1 = pin;
								break;
							default:
								break;
						}
					}
				}

				break;
#endif
			default:
				D_ERROR("Unknown media space %d, abort!!!\n", media->result.generic.mediaSpace);
				end_media_presentation(ampIndex, true);
				return DFB_UNSUPPORTED;
		}


		D_INFO("*********** Starting a %s presentation *******************\n", media->msRT->mediaSpaceName);
		D_INFO("URL: %s\n\n", media->url);

		if (media->dest.x >= 0)
		{
			DVFrameCallback callback;
			void *ctx;
			IDirectFBSurface *surface = get_destination(ampIndex, media->bdst, media->vdst, &media->dest, &callback, &ctx);

			D_INFO("*********** Playback to surface! *******************\n\n");
			if (!surface)
			{
				D_ERROR("Surface destination not available, abort!!!\n");
				end_media_presentation(ampIndex, true);
				return DFB_FAILURE;
			}
			err = media->amp->StartPresentationTo(media->amp, surface, callback, ctx);
			surface->Release(surface);
		}
#ifndef NO_BLUE_SUPPORT
		else if (media->anyBdPsrSetFlag &&
			((media->result.generic.mediaSpace == MEDIA_SPACE_BLU_RAY) ||
			 (media->result.generic.mediaSpace == MEDIA_SPACE_BDJ)))
			err	= media->amp->StartPresentation(media->amp, DFB_FALSE);
#endif		
		else
			err	= media->amp->StartPresentation(media->amp, (DFBBoolean)!media->noAutoPlay);
		
	}

	if (err != DFB_OK)
	{
		D_ERROR("Unable to start presentation, abort!!!\n");
		end_media_presentation(ampIndex, true);
		return err;
	}

	if (!media->useImg)
	{
		// wait for the result
		media->ampEvent->WaitForEvent(media->ampEvent);
		// consume the event
		{
			DFBEvent event;
			//printf("Getting the AMP event...\n");
			media->ampEvent->GetEvent(media->ampEvent, &event);
		}
		//	// flush the event queue, in case more than one changes has occured since last time we've checked the status
		// 	while (media->ampEvent->HasEvent(media->ampEvent) == DFB_OK)
		// 	{
		// 		DFBEvent event;
		// 		printf("Getting the AMP event...\n");
		// 		media->ampEvent->GetEvent(media->ampEvent, &event);
		// 	}

		// get the result
		process_media_events(ampIndex); // upload the status and perform the standard processing

		res = media->status.generic.lastCmdResult;

		if (IS_ERROR(res))
		{
#ifndef NO_BLUE_SUPPORT
		_presentation_start_error:
#endif			
			D_ERROR(/*stderr,*/ "Could not start presentation: ERROR in navigator!!!\n");
			end_media_presentation(ampIndex, true);

			if (TRESULT_VALUE(res) == RESULT_BUSY)
				return DFB_BUSY;
			else if (TRESULT_VALUE(res) == RESULT_UNIMPLEMENTED)
				return DFB_NOIMPL;

			return DFB_FAILURE;
		}
#ifndef NO_BLUE_SUPPORT
		if (media->anyBdPsrSetFlag &&
			((media->result.generic.mediaSpace == MEDIA_SPACE_BLU_RAY) ||
			(media->result.generic.mediaSpace == MEDIA_SPACE_BDJ)))
		{
			unsigned i;
			
			for (i=0; i<sizeof(media->bdPsrSetFlag)/sizeof(media->bdPsrSetFlag[0]); i++)
			{
				if (media->bdPsrSetFlag[i])
				{
					ampCommand.brd.cmd = BRDCmd_set_psr;
					ampCommand.brd.param1.selector = i;
					ampCommand.brd.param2.regValue = media->bdPsr[i];
					if (media->amp->ExecutePresentationCmd(media->amp, &ampCommand.generic, &media->result.generic) == DFB_OK)
					{
						// consume and process the event
						DFBEvent event;
						GUI_INFO("----------------  psr = %d ---------------------\n",i);
						media->ampEvent->GetEvent(media->ampEvent, &event);
						process_media_events(ampIndex); // upload the status and perform the standard processing
						if (IS_ERROR(media->status.generic.lastCmdResult))
						{
								D_ERROR("The PSR%d=%#lx preset request failed (%ld)\n", i, media->bdPsr[i], media->status.generic.lastCmdResult);
						}
						else
							printf("PSR%d was successfully preset to %#lx\n", i, media->bdPsr[i]);
						
					}
					else
					{
						GUI_INFO("-----------------  psr = %d  fail -----------------\n",i);
						D_ERROR("Unable to preset PSR%d\n", i);
					}
				}
			}
			
			if (!media->noAutoPlay)
			{
				
				ampCommand.brd.cmd = BRDCmd_first_play | BRDCmd_not_maskable;
				if ((media->amp->ExecutePresentationCmd(media->amp, &ampCommand.generic, &media->result.generic) != DFB_OK) ||
					(IS_ERROR(media->status.generic.lastCmdResult)))
					goto _presentation_start_error;
			}
		}
#endif
		// retrieve the event file descriptor
		err = media->ampEvent->CreateFileDescriptor(media->ampEvent, pEventFD);
		if (err != DFB_OK)
		{
			D_ERROR("Can't get the AMP event buffer file descriptor (error %d)!!!\n", err);
			end_media_presentation(ampIndex, true);
			return err;
		}
	}

	media->running = true;

	// prepare the help info
	if (!media->useImg)
	{
		static DirectFBKeySymbolNames(DFBKeySymbolName);
		RMuint16 i, j;

		sprintf(media->helpInfo, "%cKeys for commands\n", CHAR_CODE_SECTION);

		// for every command in the list
		for (i=0; i<media->msRT->cmdCnt; i++)
		{
			// find the associated key's name
			for (j=0; j<sizeof(DFBKeySymbolName)/sizeof(DFBKeySymbolName[0]); j++)
				if (media->msRT->cmdList[i].selKey == DFBKeySymbolName[j].symbol)
				{
					// append the key name - feature name association
					strcat(media->helpInfo, "    ");
					strcat(media->helpInfo, DFBKeySymbolName[j].name);
					strcat(media->helpInfo, ": ");
					strcat(media->helpInfo, media->msRT->cmdList[i].featureName);
					strcat(media->helpInfo, "\n");
					break;
				}
		}

		sprintf(media->helpInfo + strlen(media->helpInfo), "%cCommands per key\n", CHAR_CODE_SECTION);

		// for every key name
		for (j=0; j<sizeof(DFBKeySymbolName)/sizeof(DFBKeySymbolName[0]); j++)
		{
			bool keyUsed = false;

			// find the associated commands
			for (i=0; i<media->msRT->cmdCnt; i++)
				if (media->msRT->cmdList[i].selKey == DFBKeySymbolName[j].symbol)
				{
					if (!keyUsed)
					{
						// add the key name
						sprintf(media->helpInfo + strlen(media->helpInfo), "%c%s: ", CHAR_CODE_TAB, DFBKeySymbolName[j].name);
						keyUsed = true;
					}
					else
						strcat(media->helpInfo, ",  ");

					// append the command name
					strcat(media->helpInfo, media->msRT->cmdList[i].featureName);
				}

			if (keyUsed)
				strcat(media->helpInfo, "\n");
		}

		// remove the trailing newline
		media->helpInfo[strlen(media->helpInfo)-1] = 0;

		if (media->use_pfs)
		{
			if (GetNextPlaylistItem(media->playlist, media->url, 512) > 0)
				prefetch_data(ampIndex);
			else
			{
				free_play_list(ampIndex);

				media->amp_pfs->Release(media->amp_pfs);
				media->amp_pfs = NULL;
				media->ampEvent_pfs->Release(media->ampEvent_pfs);
				media->ampEvent_pfs = NULL;
			}
		}
	}

	if (media->ampID == MAIN_VIDEO_AMP_ID)
		*pLayerID = EM86LAYER_MAINVIDEO;
	else if (media->ampID == SECONDARY_VIDEO_AMP_ID)
		*pLayerID = EM86LAYER_SECVIDEO;
	else
		*pLayerID = -1;

	*ppAMP = media->amp;
	*pImgSz = media->img_size;

	// set up adjustment structure
	adjustment.type = ADJ_MIXER_PAN_MAIN;

	newVideoConfigIndex = videoConfigIndex;
	newAudioConfigIndex = audioConfigIndex;
	newDRMConfigIndex = drmConfigIndex;

	// set up AMP volume
	//media->volume = 0x10000000;
	RMuint32 idx = 0 ;
	if (m_isMute == false) {
        idx =((m_uiVolume + 1)* MAX_VOLUME_TABLE)/MAX_UI_VOLUME;
        if(idx == MAX_VOLUME_TABLE)
            idx = MAX_VOLUME_TABLE - 1;
	}

	ampGlobals->shared->adjustments.value.volume = VolumeTable[idx];
	ampGlobals->shared->adjustments.type = ADJ_VOLUME;

	return DFB_OK;

}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * set_active_amp(int index, bool allow_src_rect_change, bool allow_screen_rect_change)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int index, bool allow_src_rect_change, bool allow_screen_rect_change
 * @return 
 *		void
*/
void set_active_amp(int index, bool allow_src_rect_change, bool allow_screen_rect_change)
{
	if (index < amps)
	{
		ampIndex = index;
		amp = amp_table[ampIndex].amp;
		ampfd = amp_table[ampIndex].ampfd;

		// make the active layer, if present, full screen and the other one, if present, PIP
		if ((!ampIndex) && amp_table[ampIndex].layer)
		{
			DFBResult err;
		    int i;
			DFBDisplayLayerConfig lcfg;
			DFBRectangle src_rect;
			DFBRectangle dst_rect = {xpos, ypos, xres, yres}; 

			src_rect.x = 0;
			src_rect.y = 0;
			if (amp_table[ampIndex].layer->GetConfiguration(amp_table[ampIndex].layer, &lcfg) == DFB_OK)
			{
				src_rect.w = lcfg.width;
				src_rect.h = lcfg.height;
			}
			else
			{
				src_rect.w = xres;
				src_rect.h = yres;
			}

			if(isSetRecPreview)
			{
				src_rect.w = src_rect.w*7/8; 
				src_rect.h = src_rect.h*7/8;
				src_rect.x = (src_rect.w - src_rect.w*7/8)/2;
				src_rect.y = (src_rect.h - src_rect.h*7/8)/2;
			}

            if (amp_table[ampIndex].layerID == EM86LAYER_SECVIDEO)
			{
				// full screen but behind (main video layer is always level -3)
				DFBCHECK(amp_table[ampIndex].layer->SetLevel(amp_table[ampIndex].layer, -4));

				// if it's in image mode, make sure the image looks nice by resizing the secvideo layer
				if (amp_table[ampIndex].img_size.w)
				{
					DFBSurfaceDescription sdesc;
					int surf_w = lcfg.width, surf_h = lcfg.height;
					int myxres, myyres;

					sdesc.width = amp_table[ampIndex].img_size.w;
					sdesc.height = amp_table[ampIndex].img_size.h;
					D_INFO("Image: %dx%d\n", sdesc.width, sdesc.height);

					if (xres <= surf_w && yres <= surf_h)
					{
						myxres = xres;
						myyres = yres;
					}
					else
					{
						if (xres * surf_h >= yres * surf_w)
						{
							myxres = surf_w;
							myyres = myxres * yres / xres;
						}
						else
						{
							myyres = surf_h;
							myxres = myyres * xres / yres;
						}
					}

					if (sdesc.width <= myxres && sdesc.height <= myyres)
					{
						src_rect.w = myxres;
						src_rect.h = myyres;
					}
					else
					{
						if (myxres * sdesc.height <= myyres * sdesc.width)
						{
						_l1:
							src_rect.w = sdesc.width;
							src_rect.h = src_rect.w * myyres / myxres;
							if (src_rect.h > surf_h)
								goto _l2;
						}
						else
						{
						_l2:
							src_rect.h = sdesc.height;
							src_rect.w = src_rect.h * myxres / myyres;
							if (src_rect.w > surf_w)
								goto _l1;
						}
					}

					src_rect.x = (surf_w - src_rect.w) / 2;
					src_rect.y = (surf_h - src_rect.h) / 2;

					D_INFO("Surface: %dx%d\n", surf_w, surf_h);
				}
			}
			amp_table[ampIndex].src_area = src_rect;
			amp_table[ampIndex].dst_area = dst_rect;
            D_INFO("Source: %d,%d,%d,%d\n", src_rect.x, src_rect.y, src_rect.w, src_rect.h);
			D_INFO("Dest: %d,%d,%d,%d\n", dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h);
			m_src_rect = src_rect;
            if (allow_screen_rect_change)
								DFBCHECK(amp_table[ampIndex].layer->SetScreenRectangle(amp_table[ampIndex].layer, dst_rect.x, dst_rect.y, 
																	   dst_rect.w, dst_rect.h));
			if (allow_src_rect_change)
					DFBCHECK(amp_table[ampIndex].layer->SetSourceRectangle(amp_table[ampIndex].layer, src_rect.x, src_rect.y, 
																	   src_rect.w, src_rect.h));
		    for (i=0; i<amps; i++)
		    {
						if ((i != ampIndex) && amp_table[i].layer)
		    	{
					dst_rect.x = 3*xres/4;
					dst_rect.y = 0;
					dst_rect.w = xres/4;
					dst_rect.h = yres/4;
					//quyenlu add - PIP
#ifndef NO_DTV_SUPPORT
					if (isSetPiPRect == true)
					{
						//dst_rect.x = pip_x;
						//dst_rect.y = pip_y;
					}
#endif //NO_DTV_SUPPORT
					//quyenlu end
					src_rect.x = 0;
					src_rect.y = 0;
					if (amp_table[i].layer->GetConfiguration(amp_table[i].layer, &lcfg) == DFB_OK)
					{
						src_rect.w = lcfg.width;
						src_rect.h = lcfg.height;
					}
					else
					{
						src_rect.w = xres;
						src_rect.h = yres;
					}

					amp_table[i].src_area = src_rect;
					amp_table[i].dst_area = dst_rect;

					if (allow_screen_rect_change)
						DFBCHECK(amp_table[i].layer->SetScreenRectangle(amp_table[i].layer, dst_rect.x, dst_rect.y, 
																		dst_rect.w, dst_rect.h));
					if (allow_src_rect_change)
						DFBCHECK(amp_table[i].layer->SetSourceRectangle(amp_table[i].layer, src_rect.x, src_rect.y, 
																		src_rect.w, src_rect.h));

					if (amp_table[i].layerID == EM86LAYER_SECVIDEO)
		            	// PIP but above (main video layer is always level -3)
						DFBCHECK(amp_table[i].layer->SetLevel(amp_table[i].layer, -2));
					break;
		    	}
		    }
		}
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * init_amp(int i)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int i
 * @return 
 *		DFBResult
*/
DFBResult init_amp(int i)
{
	GUI_INFO("\n===============> init_amp <=====================\n");
    DFBResult err;
    err = start_media_presentation(gdfb, i, &amp_table[i].ampfd, &amp_table[i].layerID, &amp_table[i].amp, &amp_table[i].img_size);
	GUI_INFO("\n===============> start_media_presentation <=====================\n");
	if (err != DFB_OK)
		return err;
    //End
	if (amp_table[i].layerID >= 0)	// set up the video layer
	{
		DFBScreenMixerConfig mixcfg;
		int j;
		
		// special handling for sec layer
		if (amp_table[i].layerID == EM86LAYER_SECVIDEO)
		{	
			// we have sec video (either a movie or an image surface);
			// make sure we don't use Adaptive deinterlacing (uses the secvideo scaler internally)
			DFBScreenEncoderConfig enc_cfg;
			
			// enable deinterlacing mode restrictions
			nominal_deint_mode = DDM_NONE;
			
			// verify the current mode
			screen->GetEncoderConfiguration(screen, 0, &enc_cfg);
			if ((enc_cfg.flags & DSECONF_DEINTERLACING_MODE) &&
				((enc_cfg.deint_mode == DDM_ADAPTIVE) || (enc_cfg.deint_mode == DDM_ADAPTIVE_TWO_FIELDS) || (enc_cfg.deint_mode == DDM_IVTC)))
			{
				// save the nominal deinterlacing mode
				nominal_deint_mode = enc_cfg.deint_mode;
				allowAdaptiveDexing = false;
				
				// change the deinterlacing mode to one that does not require the VCR scaler
				D_INFO("Secondary video layer is enabled; turning off Adaptive/IVTC deinterlacing !\n");
				enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_DEINTERLACING_MODE | DSECONF_DELAYED_CONFIG);
				enc_cfg.deint_mode = DDM_WEAVE;
				enc_cfg.delayed_config = false;
				DFBCHECK(screen->SetEncoderConfiguration(screen, 0, &enc_cfg));
			}
			
			// reuse the layer_sv pointer
			if (layer_sv)
			{
				amp_table[i].layer = layer_sv;
				layer_sv->AddRef(layer_sv);
			}
		}
		
		if (!amp_table[i].layer)	// may have been set above
		{
			DFBCHECK(gdfb->GetDisplayLayer(gdfb, amp_table[i].layerID, &amp_table[i].layer));
			DFBCHECK(amp_table[i].layer->SetCooperativeLevel(amp_table[i].layer, DLSCL_EXCLUSIVE));
		}
		
		// assume it's the 1st video layer, make it full screen
        amp_table[i].dst_area.x = xpos;
        amp_table[i].dst_area.y = ypos;
        amp_table[i].dst_area.w = xres;
        amp_table[i].dst_area.h = yres;
		
        // search for another existing AMP in the other video layer
        for (j=0; j<amps; j++)
			if ((j != i) &&
				amp_table[j].layer &&
				(((amp_table[i].layerID == EM86LAYER_MAINVIDEO) && (amp_table[j].layerID == EM86LAYER_SECVIDEO)) ||
				((amp_table[i].layerID == EM86LAYER_SECVIDEO) && (amp_table[j].layerID == EM86LAYER_MAINVIDEO))))
			{
				// found, this must be then the 2nd video layer, make it PIP
				amp_table[i].dst_area.x = 3*xres/4;
				amp_table[i].dst_area.y = 0;
				amp_table[i].dst_area.w = xres/4;
				amp_table[i].dst_area.h = yres/4;
			}
			
			//sleep(2); //wait for set Screen Rectangle
			
			// set the layer's screen position based on their Z order (1st - full screen, snd - upper right corner)
			DFBCHECK(amp_table[i].layer->SetScreenRectangle(amp_table[i].layer,
				amp_table[i].dst_area.x, amp_table[i].dst_area.y, amp_table[i].dst_area.w, amp_table[i].dst_area.h));
			
			// add the video layer to the mixer config
			DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
			mixcfg.flags = DSMCONF_LAYERS;			
			DFB_DISPLAYLAYER_IDS_ADD(mixcfg.layers, amp_table[i].layerID);
			// if one of the amps plays back to the background, enable the layer
			if (layer_bg)
				DFB_DISPLAYLAYER_IDS_ADD(mixcfg.layers, EM86LAYER_BKGND);
			
			DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
			
			D_INFO("******* Added layer %d at index %d with destination (%d,%d,%d,%d) ***************\n", amp_table[i].layerID, i,
				amp_table[i].dst_area.x, amp_table[i].dst_area.y, amp_table[i].dst_area.w, amp_table[i].dst_area.h);
	}
	else
	{
		amp_table[i].layer = NULL;
		
        amp_table[i].dst_area.x = -1;
        amp_table[i].dst_area.y = -1;
        amp_table[i].dst_area.w = -1;
        amp_table[i].dst_area.h = -1;
	}
	GUI_INFO("\n===============>end init_amp <=====================\n");
	return err;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * terminate_amp(int ampIndex, bool releaseAMP)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int ampIndex, bool releaseAMP
 * @return 
 *		void
*/
void terminate_amp(int ampIndex, bool releaseAMP)
{
    DFBResult err;
	end_media_presentation(ampIndex, releaseAMP);
	
	if (amp_table[ampIndex].layerID == EM86LAYER_SECVIDEO && amp_table[ampIndex].amp == NULL)
	{
		// release the secondary video surface if we're playing back an image
		// (useful for debugging allocations and needed in sequential sessions)
		disable_sv_layer();
	}

	amp = amp_table[ampIndex].amp = NULL;
	ampfd = amp_table[ampIndex].ampfd = -1;
	if (amp_table[ampIndex].layer)
	{
	
		// disable the layer output
		DFBScreenMixerConfig mixcfg;
		DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
		mixcfg.flags = (DFBScreenMixerConfigFlags)(DSMCONF_LAYERS);
		DFB_DISPLAYLAYER_IDS_REMOVE(mixcfg.layers, amp_table[ampIndex].layerID);
		DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
		// reset the layer 
		amp_table[ampIndex].layer->Release(amp_table[ampIndex].layer);
		amp_table[ampIndex].layer = NULL;
		
		if (amp_table[ampIndex].layerID == EM86LAYER_SECVIDEO)
		{
		
			DFBScreenEncoderConfig enc_cfg;
			
			// restore the nominal deinterlacing mode as the VCR scaler has become avialable
			screen->GetEncoderConfiguration(screen, 0, &enc_cfg);
			if ((enc_cfg.flags & DSECONF_DEINTERLACING_MODE) && (nominal_deint_mode != enc_cfg.deint_mode))
			{
				
				D_INFO("Secondary video layer is NOT enabled; turning ON Adaptive/IVTC deinterlacing !\n");
				allowAdaptiveDexing = true;
				enc_cfg.flags = (DFBScreenEncoderConfigFlags)(DSECONF_DEINTERLACING_MODE | DSECONF_DELAYED_CONFIG);
				enc_cfg.deint_mode = nominal_deint_mode;
				enc_cfg.delayed_config = false;
				DFBCHECK(screen->SetEncoderConfiguration(screen, 0, &enc_cfg));
			}
			
			// disable deinterlacing mode restrictions
			nominal_deint_mode = (DFBDeinterlacingMode)-1;
		}
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * FeedStreamedDataBuffer(struct SMediaSession *media)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       struct SMediaSession *media
 * @return 
 *		DFBResult
*/
static DFBResult FeedStreamedDataBuffer(struct SMediaSession *media)
{
	char buffer[32*1024];
	static bool zerosSent = true;
	size_t size;
	unsigned int buffered;
	
	if ((media->dataBuffer->GetLength(media->dataBuffer, &buffered) == DFB_OK) && (buffered > 128*1024))
	{
		usleep(100000);
		return DFB_OK;
	}
	
	if (!media->stopStreaming && (media->fileToStream >= 0))
		size = read(media->fileToStream, buffer, sizeof(buffer));
	else
		size = 0;
	
	if (size > 0)
	{
		zerosSent = false;
		return media->dataBuffer->PutData(media->dataBuffer, buffer, size);
	}
	else
	{
		// end of file reached, close it
		if (media->fileToStream >= 0)
		{
			close(media->fileToStream);
			media->fileToStream = -1;
			media->stopStreaming = false;
		}
		
		// for playlists, insert zeros between play items and wait for the user to advance to the next item
		if (media->playlist)
		{
			if (/*(media->format.mediaType == (MTYPE_ELEM_MPEG2 | MTYPE_APP_BDJ)) && */!zerosSent)
			{
				// insert 4 zeros between MPEG2 video frames to flush the hw
				static char dummy[] ={0, 0, 0, 0};
				
				zerosSent = true;
				return media->dataBuffer->PutData(media->dataBuffer, dummy, sizeof(dummy));
			}
			
			// wait for the next playlist item
			usleep(100000);
			
			// continue to wait for data, will reach EOF only at end of playlist
			return DFB_OK;
		}
	}
	
	return DFB_EOF;
	
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * data_buffer_thread(DirectThread *thread, void *arg)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       DirectThread *thread, void *arg
 * @return 
 *		 *void
*/
static bool stopStreamingThread = false;
void* data_buffer_thread(DirectThread *thread, void *arg)
{
	struct SMediaSession *media = (struct SMediaSession *)arg;
	
	while (!stopStreamingThread)
	{
		if (FeedStreamedDataBuffer(media) != DFB_OK)
			break;
	}
	media->dataBuffer->Finish(media->dataBuffer);
	
	media->dataBufferFeeder = NULL;
	
	return NULL;
	
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * prefetch_data(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int ampIndex
 * @return 
 *		 DFBResult
*/
DFBResult prefetch_data(int ampIndex)
{
	DFBResult res;
	struct SMediaSession *media = &mediaTab[ampIndex];
	
	if (media->amp_pfs == NULL)
	{
		res = gdfb->GetInterface(gdfb, "IAdvancedMediaProvider", "EM8630", (void*)media->ampID, (void**) &media->amp_pfs);
		if (res != DFB_OK)
		{
			D_ERROR("PFS: Advanced Media Provider instantiation failed!!!\n");
			return res;
		}
		
		res = media->amp_pfs->GetEventBuffer(media->amp_pfs, &media->ampEvent_pfs);
		if (res != DFB_OK)
		{
			D_ERROR("PFS: Advanced Media Provider event buffer retrieval failed!!!\n");
			return res;
		}
	}
	
	/*wchar_t w_url[512];
	
	  // convert to unicode
	  // (do not use mbstowcs as the input is not really multi-byte and will fail if unusual codes are used)
	  char *pChar = media->url;
	  wchar_t *pWChar = w_url;
	  
		do
		{
		*(pWChar++) = (wchar_t)*pChar;
		}
		while (*(pChar++) && (pWChar - w_url < sizeof(w_url) - 1));
	*pWChar = 0;*/
	
	//			mbstowcs(w_url, media->url, sizeof(w_url)/sizeof(wchar_t));
	D_INFO("PFS: Prefetching %s (AMP1: %p, AMP2: %p)\n", media->url, media->amp, media->amp_pfs);
	
	if ((res = media->amp_pfs->OpenMedia(media->amp_pfs, media->url, &media->format, media->openParam)) != DFB_OK)
		D_INFO("PFS: Prefetching failed (err: %i)!!!\n", res);
	
	return res;
	
	
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * allocAMPid(struct SMediaSession *media)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release media provider
 * 
 * @param
 *       struct SMediaSession *media
 * @return 
 *		 bool
*/
static bool allocAMPid(struct SMediaSession *media)
{
	if ((media->ampID < 0) && !media->useImg)
	{
		// must assign an AMP ID
		int j;
		int ampID;
		
		// set up the default AMP ID value: 0 for media with video output on layers, 2 otherwise
		if ((media->dest.x >= 0) ||														// blitted video output
			(media->format.mediaType == MTYPE_APP_CDA) ||									// audio-only format
			((GET_ELEMENTARY_MTYPE(media->format.mediaType) >= MTYPE_ELEM_AUDIO_FIRST) &&
			(GET_ELEMENTARY_MTYPE(media->format.mediaType) <= MTYPE_ELEM_AUDIO_LAST)))	// audio-only format
			// force no video layer
			ampID = 2;
		else
			// request video layer
			ampID = 0;
		
		// verify that the proposed ID has not been already assigned
		for (j=0; j<mediaCnt; j++)
			if (mediaTab[j].ampID == ampID)
			{
				// AMP ID already assigned, must try a new value
				
				if (ampID == 1)
					// running out of video layers, fatal error unless autodetection (it may end up with appropriate media)
					if (media->format.mediaType != MTYPE_APP_UNKNOWN)
					{
						GUI_INFO("FATAL ERROR: Too many video layers (%d or more)\n", ampID + 1);
						return false;
					}
					
					// bump up AMP ID and check again from start
					ampID++;
					j=-1;
			}
			
			if (ampID >= MAX_AMP_INSTANCES)
			{
				GUI_INFO("FATAL ERROR: Too many AMPs (%d or more)\n", ampID);
				return false;
			}
			
			media->ampID = ampID;
	}
	
	return true;
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * end_media_presentation(int ampIndex, bool releaseAMP)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release media provider
 * 
 * @param
 *       IDirectFB *dfb, int ampIndex, int *pEventFD, int *pLayerID,
 *  IAdvancedMediaProvider **ppAMP, DFBDimension *pImgSz
 * @return 
 *		 DFBResult
*/
// terminates the specified AMP object
void end_media_presentation(int ampIndex, bool releaseAMP)
{
	//GUI_INFO("\n===============> end_media_presentation <=====================\n");
	if (ampIndex < MAX_AMP_INSTANCES)
	{
		
		struct SMediaSession *media = &mediaTab[ampIndex];
		media->running = false;
		
		// terminate the input first to avoid deadlock when closing the media below
		if (media->dataBufferFeeder)
		{
			stopStreamingThread = true;
			while ((volatile DirectThread *)media->dataBufferFeeder)
				usleep(100000);
			stopStreamingThread = false;
		}
		
		if (media->amp)
		{
			//releaseAMP = true;
			if (releaseAMP)
			{
			
				media->amp->Release(media->amp);
				media->amp = NULL;
			
				if (media->amp_pfs != NULL)
				{
					media->amp_pfs->Release(media->amp_pfs);
					media->amp_pfs = NULL;
				}
				if (media->ampEvent_pfs != NULL)
				{
					media->ampEvent_pfs->Release(media->ampEvent_pfs);
					media->ampEvent_pfs = NULL;
				}
				//				dmalloc_shutdown();
				
			}
			else
				media->amp->CloseMedia(media->amp);
	
			
			if (media->ampEvent)
			{
				media->ampEvent->Release(media->ampEvent);
				media->ampEvent = NULL;
			}
		
			if (media->bookmarkBuf)
			{
		
				free(media->bookmarkBuf);
				media->bookmarkBuf = NULL;
			}
			//mediaCnt--;
			
		}
		
		if (media->dataBuffer)
		{
			media->dataBuffer->Release(media->dataBuffer);
			media->dataBuffer = NULL;
		}
		
		if (media->fileToStream >= 0)
		{
			close(media->fileToStream);
			media->fileToStream = -1;
		}
		if (media->cDataBuffer)
		{
			media->cDataBuffer->Release(media->cDataBuffer);
			media->cDataBuffer = NULL;
		}
		
		if (media->next)
		{
			
			// replace this AMP entry with the enxt in the sequence
			if (media->amp)
			{
				// reuse AMP
				mediaTab[media->next].ampID = media->ampID;
				mediaTab[media->next].amp = media->amp;
			}
			else
			{
				// reset the AMP ID and realloc, it may end up with a different value if the parameters are different
				media->ampID = -1;
				if (!allocAMPid(&mediaTab[media->next]))
				{
					printf("FAILED to advance to the next media sequence, reusing the current one\n");
					return;
				}
			}
			*media = mediaTab[media->next];
			printf("\tMedia index %d, next sequence: AMP ID=%d format=%#lx\n", ampIndex, media->ampID, (unsigned long)media->format.mediaType);
		}
	}

	//GUI_INFO("\n===============> End of end_media_presentation <=====================\n");
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * init_parameter(const char *m_URL,const char	*forceType)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       const char *m_URL,const char	*forceType
 * @return 
 *		 int
*/

UINT32 get_media_type(const char *mediaType) {

	if (mediaType == NULL) {
		return MTYPE_APP_NONE;
	}
	printf("\n get_media_type = %s<---------- \n\n ", mediaType);

	if (!strcmp(mediaType, "none")) {
		return MTYPE_APP_NONE;
	}
	else if (!strcmp(mediaType, "flac"))
	{
#ifdef MTYPE_ELEM_FLAC
		return MTYPE_ELEM_FLAC;
#endif
	}
	else if (!strcmp(mediaType, "wma"))
	{
		return MTYPE_ELEM_WMA;
	}
	else if (!strcmp(mediaType, "wmapro"))
	{
		return MTYPE_ELEM_WMAPRO;
	}
	else if (!strcmp(mediaType, "ac3"))
	{
		return MTYPE_ELEM_AC3;
	}
	else if (!strcmp(mediaType, "dd+"))
	{
		return MTYPE_ELEM_DDP;
	}
	else if (!strcmp(mediaType, "aac"))
	{
		return MTYPE_ELEM_AAC;
	}
	else if (!strcmp(mediaType, "aacp"))
	{
		return MTYPE_ELEM_AACP;
	}
	else if (!strcmp(mediaType, "asf"))
	{
		return MTYPE_CONT_ASF;
	}
	else if (!strcmp(mediaType, "avi"))
	{
		return MTYPE_CONT_AVI;
	}
	else if (!strcmp(mediaType, "mp4"))
	{
		return MTYPE_CONT_MP4;
	}
	else if (!strcmp(mediaType, "mkv"))
	{
		return MTYPE_CONT_MKV;
	}
	else if (!strcmp(mediaType, "flv"))
	{
		return MTYPE_CONT_FLV;
	}
	else if (!strcmp(mediaType, "m2v-ts"))
	{
		return (MTYPE_ELEM_MPEG2 | MTYPE_CONT_M2TS);
	}
	else if (!strcmp(mediaType, "avc-ts"))
	{
		return (MTYPE_ELEM_AVC | MTYPE_CONT_M2TS);
	}
	else if (!strcmp(mediaType, "m2v-ps"))
	{
		return (MTYPE_ELEM_MPEG2 | MTYPE_CONT_M2PES);
	}
	else if (!strcmp(mediaType, "avc-ps"))
	{
		return (MTYPE_ELEM_AVC | MTYPE_CONT_M2PES);
	}
	else if (!strcmp(mediaType, "m2ts"))
	{
		return MTYPE_CONT_M2TS;
	}
	else if (!strcmp(mediaType, "m2pes"))
	{
		return MTYPE_CONT_M2PES;
	}
	else if (!strcmp(mediaType, "m1sys"))
	{
		return MTYPE_CONT_M1SYS;
	}

	return MTYPE_APP_NONE;
}

int init_parameter(const char *m_URL,const char	*forceType)
{
	
	GUI_INFO("\n>>>> m_URL = %s, forceType = %s\n", m_URL, forceType);
	amps = 1 ;
	mediaCnt = 0;
	bool newMedia;
	int i = 1;	// parameter index
	memset(&mediaTab[mediaCnt], 0, sizeof(mediaTab[mediaCnt]));
        //memset(&mediaTab, 0, sizeof(mediaTab));
//	do
	{
		struct SMediaSession *media = &mediaTab[mediaCnt];
		// initialize the media parameters
		media->format.mediaType = MTYPE_APP_UNKNOWN;	// default to autodetection
		media->dest.x = -1;
		media->dest.y = -1;
		media->dest.w = -1;
		media->dest.h = -1;
		media->fileToStream = -1;
		media->ampID = -1;
		media->cFormat.mediaType = MTYPE_ELEM_NONE;	// default to no concurrent media	
		// mark the OpenParams structure as binary as pooposed to textual
#ifdef USING_864X
		media->lpb_open_params.zero = 0;
		media->lpb_open_params.a1b2c3d4 = 0xa1b2c3d4;
#endif
		// assume last media
		newMedia = false;
		// continue parameter inspection
		//while (i < argc)
		//if (!strncmp(forceType, "-f:", 3))
		{
#ifdef USING_864X			
				// save the sync flag
				TMediaType syncFlag = media->format.mediaType & MTYPE_IMPL_GLOBALLY_SYNCED;			
#endif
				if (!strcmp(forceType, "-f:lpb"))
				{
					//media->format.mediaType = MTYPE_APP_NONE;
					media->format.mediaType = get_media_type(m_mediaType);
					isResetAudioDigital = TRUE ;
					
				}
#ifndef NO_HLS_SUPPORT
				else if(strcmp(forceType, "-f:lpb -shell:rebuffer=streamnavigator") == 0) {
					//media->format.mediaType = MTYPE_APP_NONE;
					media->format.mediaType = get_media_type(m_mediaType);
					isResetAudioDigital = TRUE ;

                    switch (m_rebufferControl_mode) {
					case MLB_MODE:
					case 2: //LPBRebuffer_Auto_byPlayTime :
						printf("\n\n +++ rebuffer play by params +++ m_rebufferControl.mode == LPBRebuffer_Auto_byPlayTime\n\n");
						media->lpb_open_params.rebufferControl.mode = LPBRebuffer_Auto_byPlayTime;
						media->lpb_open_params.rebufferControl.params.playTime.audioLo 	= m_rebufferControl.params.playTime.audioLo;
						media->lpb_open_params.rebufferControl.params.playTime.audioHi 	= m_rebufferControl.params.playTime.audioHi;
						media->lpb_open_params.rebufferControl.params.playTime.videoLo 	= m_rebufferControl.params.playTime.videoLo;
						media->lpb_open_params.rebufferControl.params.playTime.videoHi 	= m_rebufferControl.params.playTime.videoHi;
						media->lpb_open_params.rebufferControl.params.playTime.fullness = m_rebufferControl.params.playTime.fullness;
						break;
					case 1: //LPBRebuffer_Auto_byFullness :
						printf("\n\n +++ rebuffer play by params +++ m_rebufferControl.mode == LPBRebuffer_Auto_byFullness\n\n");
						media->lpb_open_params.rebufferControl.mode = LPBRebuffer_Auto_byFullness;
						media->lpb_open_params.rebufferControl.params.fullness.audioLo 	= m_rebufferControl.params.fullness.audioLo;
						media->lpb_open_params.rebufferControl.params.fullness.audioHi 	= m_rebufferControl.params.fullness.audioHi;
						media->lpb_open_params.rebufferControl.params.fullness.videoLo 	= m_rebufferControl.params.fullness.videoLo;
						media->lpb_open_params.rebufferControl.params.fullness.videoHi 	= m_rebufferControl.params.fullness.videoHi;
						break;
					case 0: //LPBRebuffer_None :
						printf("\n\n +++ rebuffer play by params +++ m_rebufferControl.mode == LPBRebuffer_None\n\n");
						media->lpb_open_params.rebufferControl.mode = LPBRebuffer_None;
						break;
					}

					if(m_rebufferControl_mode != MLB_MODE){
						printf("\n\n +++ rebuffer play by params REDUCE_BUFFERING_TIME : maxPrebufferSize = %d bytes , stcOffset = %d ms +++\n\n", m_prebuf_max, m_stc_offset_ms );
						media->lpb_open_params.maxPrebufferSize	= m_prebuf_max; //bytes
						media->lpb_open_params.stcOffset		= m_stc_offset_ms;//ms
#ifdef LPBSourceSeekSupport_Enabled
						if (m_lpb_SeekSupport == true) {
							printf("\n +++ m_lpb_SeekSupport = LPBSourceSeekSupport_Enabled \n");
							media->lpb_open_params.sourceSeekSupport = LPBSourceSeekSupport_Enabled;
						} else if (m_lpb_SeekSupport == false) {
							printf("\n +++ m_lpb_SeekSupport = LPBSourceSeekSupport_Disabled \n");
							media->lpb_open_params.sourceSeekSupport = LPBSourceSeekSupport_Disabled;
						}
#endif
					}

					if (m_lpb_indexMode == LPBIndex_None) {
						printf("\n\n +++ media->lpb_open_params.indexMode = LPBIndex_None \n\n");
						media->lpb_open_params.indexMode  = LPBIndex_None; // for live streaming
					} else if (m_lpb_indexMode == LPBIndex_Auto) {
						printf("\n\n +++ media->lpb_open_params.indexMode = LPBIndex_Auto \n\n");
						media->lpb_open_params.indexMode  = LPBIndex_Auto; 
					}

					media->openParam = &media->lpb_open_params;
					media->format.mediaType |= MTYPE_IMPL_SHELL;

				}//LPBRebuffer_Auto_byPlayTime
#endif // #ifndef NO_HLS_SUPPORT
				else if (!strcmp(forceType, "-f:img"))
				{
					GUI_INFO("=========== play image by hardware ===============\n");
					media->dest.x = 0;
					media->dest.y = 0;
#if 0
					media->dest.w = 4094;	// must be an even number less than 4096
					media->dest.h = 4094;
#else
					media->dest.w = 1920;	// must be an even number less than 4096
					media->dest.h = 1080;
#endif
					media->vdst = true;
					media->bdst = false;
					media->ampID = SECONDARY_VIDEO_AMP_ID;

					media->useImg = true;
					media->format.mediaType = MTYPE_ELEM_GRAPHICS;
				}
#ifndef NO_RED_SUPPORT
				else if (!strcmp(forceType, "-f:cdda"))
				{
					media->format.mediaType = MTYPE_APP_CDA;
					if(isResetAudioDigital)
					{
						isResetAudioDigital = FALSE ;
					}
					
				}
				else if (!strcmp(forceType, "-f:dvd"))
				{
					media->format.mediaType = MTYPE_APP_DVD;
					media->ampID = 0; // force primary AMP
#ifdef RMCFG_REGION_CODE_FOR_DVD
					dvd_settings.region = RMCFG_REGION_CODE_FOR_DVD;
#endif
					media->openParam = &dvd_settings;
					if(isResetAudioDigital)
					{
						isResetAudioDigital = FALSE ;
					}
					
				}
				else if (!strcmp(forceType, "-f:vr"))
				{
					media->format.mediaType = MTYPE_APP_DVDVR;
					media->ampID = 0; // force primary AMP
					if(isResetAudioDigital)
					{
						isResetAudioDigital = FALSE ;
					}
					
				}
#ifndef NO_VCD_SUPPORT	
				else if (!strcmp(forceType, "-f:vcd")) // for both VCD and SVCD
				{
					media->format.mediaType = MTYPE_APP_VCD;
					media->ampID = 0; // force primary AMP
				}
#endif // NO_VCD_SUPPORT						
#endif
#ifndef NO_BLUE_SUPPORT					
				else if (!strcmp(forceType, "-f:bdrom"))
				{
					media->format.mediaType = MTYPE_APP_BDROM;
					media->ampID = 0; // force primary AMP
					//media->openParam = format_specific_parameter;
					//media->noAutoPlay = true;
					/*media->openParam = (void *)"a:en:eng:eng:spa";*/
					if(isResetAudioDigital)
					{
						isResetAudioDigital = FALSE ;
					}
					
				}
				else if (!strcmp(forceType, "-f:avchd"))
				{
					media->format.mediaType = MTYPE_APP_AVCHD;
					media->ampID = 0; // force primary AMP
					if(isResetAudioDigital)
					{
						isResetAudioDigital = FALSE ;
					}
				}					
				else if (!strcmp(forceType, "-f:bdre"))
				{
					media->format.mediaType = MTYPE_APP_BDRE;
					media->ampID = 0; // force primary AMP
					media->noAutoPlay = noAuto;
					if(get_reqpin_status())
					{
						media->openParam = get_bdre_pin();
					}
					if(isResetAudioDigital)
					{
						isResetAudioDigital = FALSE ;
					}
					
				}
#endif
				else if (!strcmp(forceType, "-f:ttone"))
				{
					isResetAudio = TRUE;
					strcpy(media->url, "dummy.url");
					media->format.mediaType = MTYPE_ELEM_PCM | MTYPE_APP_CALIBRATION ;
					media->format.formatValid = 1;
					media->format.format.pcm.storage.format = SF_PCM;
					media->format.format.pcm.storage.bitsPerSample = 16;
					media->format.format.pcm.storage.MsbFirst = true;
					media->format.format.pcm.rend.samplingFreq = 48000;
					media->format.format.pcm.rend.channels = CCf_LCRSlSr;//CCf_LCRSlSrBlBr;
					media->format.format.pcm.rend.lfe = 0;
					if(isResetAudioDigital)
					{
						isResetAudioDigital = FALSE ;
					}
					
					goto donotusercdrom;
				}
				else if (!strncmp(forceType, "-f:mp3",6))
				{
					media->format.mediaType = MTYPE_ELEM_MP3;
					media->format.formatValid = 1;
					media->format.format.sound.channels = 2;
					media->format.format.sound.samplingFreq = 44100;
					media->format.format.sound.lfe = 0;

					media->openParam = &media->lpb_open_params;
					media->format.mediaType |= MTYPE_IMPL_SHELL;
#ifndef NO_HLS_SUPPORT
					char *nextParameter =  (char *)(forceType + 7);
					if(!strcmp(nextParameter,"audio_radio_shoutcast")) {
						printf("\n>>HL debug<< audio_radio_shoutcast\n");
						media->lpb_open_params.maxPrebufferSize	= 10; //bytes
#ifdef LPBSourceSeekSupport_Disabled
						media->lpb_open_params.sourceSeekSupport = LPBSourceSeekSupport_Disabled;
#endif
					}
#endif
				}
#ifndef NO_DTV_SUPPORT
				else if (!strcmp(forceType, "-dtv"))
				{
					printf("\n init_parameter:: dtv");
					media->format.mediaType = MTYPE_APP_ATSC;
					media->ampID = mediaCnt; // force primary AMP (MAIN_VIDEO_AMP_ID = 0)
					media->isOpen = TRUE; 
				}
#endif //NO_DTV_SUPPORT
			//else if (argv[i][0] != '-')
			{
				strncpy(media->url, m_URL, sizeof(media->url)-1);
			}
donotusercdrom:
			// advance to next argument
			i++;
#ifdef USING_864X			
			// restore the sync flag
			media->format.mediaType |= syncFlag;
#endif		
		}
		if (!media->url[0])
		{
			GUI_INFO("FATAL ERROR: Missing media URL %d\n", mediaCnt + 1);
			//goto _usage;
		}

		// if we get here, the current media is OK
		mediaCnt++;
	}
//	while (newMedia && (mediaCnt < MAX_AMP_INSTANCES));
	// use default AMP IDs for those that have not been specifically assigned
	GUI_INFO("\nDetected following AMP requests:\n");
	for (i=0; i<mediaCnt; i++)
	{
		struct SMediaSession *media = &mediaTab[i];

		if (media->vdst && !media->useImg)
		{
			GUI_INFO("FATAL ERROR: -vdst used without -f:img !\n");
			//goto _usage;
		}

		if (media->useImg && !media->vdst && (media->dest.w * media->dest.h == 0))
		{
			GUI_INFO("FATAL ERROR: -f:img used without -bdst, -fdst or -vdst !\n");
			//goto _usage;
		}
		allocAMPid(media);
			//goto _usage;
		//printf("\tMedia index %d: AMP ID=%d format=%#lx\n", i, media->ampID, (unsigned long)media->format.mediaType);

		while (media->next > 0)
		{
			if (media->vdst && !media->useImg)
			{
				GUI_INFO("FATAL ERROR: -vdst used without -f:img!\n");
				//goto _usage;
			}

			if (media->useImg && !media->vdst && (media->dest.w * media->dest.h == 0))
			{
				GUI_INFO("FATAL ERROR: -f:img used without -bdst, -fdst or -vdst !\n");
				//goto _usage;
			}

			media = &mediaTab[media->next];
		}
	}
	GUI_INFO("\n");
	amps = mediaCnt;
	return mediaCnt;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * start_amp(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       void
 * @return 
 *		 RMstatus
*/
RMstatus start_amp(void)
{
	DFBResult err;
	GUI_INFO("\n===============> start_amp <=====================\n");
	err = init_amp(0);
	if (err != DFB_OK)
	{
#ifndef NO_RED_SUPPORT		
		if(err == DFB_FAILURE)
		{
			isWrongRegion = TRUE;
		}
		else
			isWrongRegion = FALSE;
#endif // #ifndef NO_RED_SUPPORT		
		return RM_ERROR;		
	}
	//End	
	//InitVideoConfig(ampIndex);
	GUI_INFO("\n===============> set_active_amp <=====================\n");
	set_active_amp(0, true, true);
        isPlayingMediaFile = true; 
	// Workaround to fix WDITR #38542-[MLB] Sometimes there is no audio output
	// Re-configure HDMI using the current settings or using an actual configuration
    cfgamp->SetConfiguration(cfgamp, NULL, NULL);

	return RM_OK;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * StartTone(enum SpeakerOut speaker)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       enum SpeakerOut speaker
 * @return 
 *		 RMstatus
*/
RMstatus  start_tone(enum SpeakerOut speaker)
{
	RMstatus err = RM_ERROR;
	if(amp)
	{
		power_off_amp();
	}
	set_volume_for_speaker(speaker);
	set_effect_on_audio();
	init_parameter("","-f:ttone");
	err =start_amp();
	return err;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * power_off_amp(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       void
 * @return 
 *		 void
*/
void power_off_amp(void)
{
        GUI_INFO("\n\n\n ===========>[power_off_amp] ampIndex = %d <===========\n", ampIndex);
	terminate_amp(ampIndex, true);
	// reset the cooperative level in case the AMP has changed it (Sun JVM does that in single buffered mode)
	gdfb->SetCooperativeLevel( gdfb, DFSCL_NORMAL);
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * OpenBDREDisc(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       void
 * @return 
 *		 RMstatus
*/
#ifndef NO_BLUE_SUPPORT
RMstatus open_bdre_disc(void)
{
	GUI_INFO("\n>>>> OpenBDREDisc\n");
	
	if (open_bdre_media(gdfb, 0) != DFB_OK)
		return RM_ERROR;
	
	return RM_OK;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * OpenBDREMedia(IDirectFB *dfb, int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       IDirectFB *dfb, int ampIndex
 * @return 
 *		 DFBResult
*/
DFBResult open_bdre_media(IDirectFB *dfb, int ampIndex)
{
	DFBResult err = DFB_OK;
#ifndef NO_BLUE_SUPPORT
	TResultCode res;
	struct SMediaSession *media;
	bool alreadyOpened = FALSE;

	if (ampIndex >= mediaCnt)
		return DFB_LIMITEXCEEDED;

	media = &mediaTab[ampIndex];

	RMASSERT(!media->useImg);
	RMASSERT(!media->use_pfs);

	if (!media->amp)
	{
		// create an Advanced Media Provider (tm)
		void *dummy; // just to avoid stupid compiler warning
		err = dfb->GetInterface(dfb, "IAdvancedMediaProvider", "EM8630", (void*)media->ampID, &dummy);
		if (err != DFB_OK)
		{
			D_ERROR(/*stderr,*/ "Advanced Media Provider instantiation failed!!!\n");

			return err;
		}
		media->amp = dummy;

		// reset session-specific fields
		media->running = false;
		media->repeatTitle = 0;
		media->repeatChapter = 0;
		memset(&media->status, 0, sizeof(media->status));

		// get the event buffer
		err = media->amp->GetEventBuffer(media->amp, &media->ampEvent);
		if (err != DFB_OK)
		{
			D_ERROR(/*stderr,*/ "Could not get the AMP event buffer!!!\n");
			end_media_presentation(ampIndex, true);

			return err;
		}
	}

	// get the event buffer
	// This is needed here for Reopen Media to work, because in that case media->amp is NOT released and, consequently,
	// no media->ampEvent would be created
	if (!media->ampEvent)
	{
		// get the event buffer
		err = media->amp->GetEventBuffer(media->amp, &media->ampEvent);
		if (err != DFB_OK)
		{
			D_ERROR(/*stderr,*/ "Could not get the AMP event buffer!!!\n");
			end_media_presentation(ampIndex, true);

			return err;
		}
	}

	if (alreadyOpened)	// if already opened by prefetch_data
		err = DFB_OK;
	else
		err = media->amp->OpenMedia(media->amp, media->url, &media->format, media->openParam);

	if (err != DFB_OK)
	{
		D_ERROR(/*stderr,*/ "Could not open media %s!!!\n", media->url);
		end_media_presentation(ampIndex, true);

		return err;
	}

	// wait for the result
	D_INFO("Waiting for OpenMedia confirmation...\n");
	media->ampEvent->WaitForEvent(media->ampEvent);

	// consume the event
	{
		DFBEvent event;

		media->ampEvent->GetEvent(media->ampEvent, &event);
	}
	APP_ASSERT(media->ampEvent->HasEvent(media->ampEvent) != DFB_OK);

	// check the result
	media->status.generic.size = sizeof(union UMSStatus);
	media->status.generic.mediaSpace = MEDIA_SPACE_UNKNOWN;
	process_media_events(ampIndex); // upload the status and perform the standard processing

	res = media->status.generic.lastCmdResult;

	if (IS_ERROR(res))
	{
		D_ERROR(/*stderr,*/ "Could not open media %d from %s!!!\n", ampIndex, media->url);
		end_media_presentation(ampIndex, true);

		if (TRESULT_VALUE(res) == RESULT_BUSY)
			return DFB_BUSY;
		else if (TRESULT_VALUE(res) == RESULT_UNIMPLEMENTED)
			return DFB_NOIMPL;

		return DFB_FAILURE;
	}

	// verify that is a supported media space
	if (((unsigned int)media->status.generic.mediaSpace >= sizeof(msRTTab)/sizeof(msRTTab[0])) ||
			 (msRTTab[media->status.generic.mediaSpace] == MEDIA_SPACE_UNKNOWN))
	{
		D_ERROR("Unknown media space %d, abort!!!\n", media->status.generic.mediaSpace);
		end_media_presentation(ampIndex, true);
		return DFB_UNSUPPORTED;
	}

	//
	// set up the run-time parameters
	//
	media->msRT = msRTTab[(unsigned int)media->status.generic.mediaSpace];

	// prepare the command and result structures
	media->result.generic.mediaSpace = ampCommand.generic.mediaSpace = media->status.generic.mediaSpace;

	// not needed, it is set by the specific Navigator
	//media->result.generic.dataSize = sizeof(union UMSResult);

	// THIS IS WRONG !!!!!!!!!!!!!!!!!!!
	//ampCommand.generic.dataSize = sizeof(union UMSCommand);

	// command size IS media dependent,
	// otherwise bad things will happen when copying the command in CPlaybackControl::ExecuteCmd()
	if (media->result.generic.mediaSpace == MEDIA_SPACE_BDRE)
	{
		ampCommand.generic.dataSize = sizeof(struct SBDRECommand);

		if (media->status.generic.changes & SBDRESTATUS_REQ_PIN)
		{
			D_ERROR("\n\nMedia %d from %s is protected BD-RE! Enter PIN.\n\n", ampIndex, media->url);
			return DFB_FAILURE;
		}
		else if ((media->status.generic.flags & SBDRESTATUS_DIR) && (media->status.generic.changes & SBDRESTATUS_DIR))
		{
			if (media->status.bdre.bdavDir.resume_PlayList_file_id != 0xFFFFFFFF)
			{
				int idx;
				TBDREItemID *p_play_list_id = media->status.bdre.bdavDir.PlayListID;

				for (idx = 0; idx < media->status.bdre.bdavDir.number_of_PlayLists; idx++, p_play_list_id++ )
				{
					if (*p_play_list_id == media->status.bdre.bdavDir.resume_PlayList_file_id)
					{
						selected_play_list_idx = idx + 1;
						break;
					}
				}
			}
		}

		// this should be -tpar, i.e. the PIN
		if (media->openParam)
		{
			RMint32 pin;
			unsigned int ci;

			pin = atoi((const char *)media->openParam);
			
			// change the default displayed PIN with the user supplied one for the commands that need it
			for (ci=0; ci<media->msRT->cmdCnt; ci++)
			{
				switch (media->msRT->cmdList[ci].cmd)
				{
					case BDRECmd_play_pl_time:
					case BDRECmd_play_pl_mark:
					case BDRECmd_get_pl_info:
						media->msRT->cmdList[ci].defParam2 = pin;
						break;
					case BDRECmd_resume_bookmark:
						media->msRT->cmdList[ci].defParam1 = pin;
						break;
					default:
						break;
				}
			}
		}

		err = DFB_OK;
	}
	else
	{
		D_ERROR("Unknown media space %d, abort!!!\n", media->result.generic.mediaSpace);
		err = DFB_UNSUPPORTED;
	}

#endif
	return err;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * eject_for_bdj_disc(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       void
 * @return 
 *		 RMbool
*/
RMbool handle_media_for_bdrom(RMbool isEject)
{
	//RMbool _result = TRUE;
	struct SLPBCommand cmd;
	struct SResult res;
	DFBResult err;
	TResultCode insertValue= (RMuint16)(TRESULT_ERROR | RESULT_OUT_OF_SYNC); 
	TResultCode ejectValue = (TRESULT_SUCCESS | RESULT_OUT_OF_SYNC); 
	TResultCode usingValue = isEject ? ejectValue : insertValue ;
	
	if(!amp )
	{
		return false;
	}
	GUI_INFO("=================> %s <==================\n",__FUNCTION__);
	cmd.common.dataSize = sizeof(cmd);
	cmd.common.mediaSpace = MEDIA_SPACE_UNKNOWN;
	cmd.cmd = isEject ? (enum ELPBCmd)Cmd_disk_ejecting : (enum ELPBCmd)Cmd_disk_inserted;
	
	res.dataSize = sizeof(res);
	res.mediaSpace = MEDIA_SPACE_UNKNOWN;
	
	err = amp->ExecutePresentationCmd(amp, (struct SCommand *)&cmd, &res);
	if ((err == DFB_OK) && (res.value == usingValue))
	{
		printf("eject_for_bdj_disc ok >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		// AMP has signaled that media is not the one expected, prompt the user for further action
		return isEject;
	}
	printf("----------------------- eject_for_bdj_disc fail  ---------------------------\n");
	return !isEject;

}
#endif // NO_BLUE_SUPPORT
/**
 * 
 * <b>FUNCTION: </b>
 *
 * frame_callback(void* param)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release second video layer
 * 
 * @param
 *		void* param
 * @return 
 *      void
*/
void frame_callback(void* param)
{
	DFBResult err;
	int ampIndex = (int)param;
	struct SFrameCallbackParam *fcparam = amp_frame_callback_table + ampIndex;
	DFBRectangle src_rect;
	
	//	D_INFO("Frame callback\n");
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.w = fcparam->area.w;
	src_rect.h = fcparam->area.h;
	//	DFBCHECK(primary->Blit(primary, fcparam->surf, &src_rect, fcparam->area.x, fcparam->area.y));
	
	DFBCHECK(primary->Flip(primary, NULL, DSFLIP_NONE));
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * get_destination(int ampIndex, bool bgnd, bool secvid, DFBRectangle *area, DVFrameCallback *pCallback, void **pCtx)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int ampIndex, bool bgnd, bool secvid, DFBRectangle *area, DVFrameCallback *pCallback, void **pCtx
 * @return 
 *		 IDirectFBSurface *
*/
// Returns the surface for playback in the specified area and frame callback with context to
// be called upon each frame availability
IDirectFBSurface *get_destination(int ampIndex, bool bgnd, bool secvid, DFBRectangle *area, DVFrameCallback *pCallback, void **pCtx)
{
	DFBResult err;
	IDirectFBSurface *subSurf;
	struct SFrameCallbackParam *fcparam;
	IDirectFBSurface *surf = NULL;
	
	if (bgnd)
	{
		D_INFO("Using background layer.................\n");
		if (!bg)
		{
			//enable_background_layer(out_xres, out_yres, false);
		}
		
		surf = bg;
	}
	else if (secvid)
	{
		D_INFO("Using SV layer.................\n");
		if (!sv)
			enable_sv_layer(area->x + area->w, area->y + area->h);
		amp_table[ampIndex].src_area.w = area->x + area->w;
		amp_table[ampIndex].src_area.h = area->y + area->h;
		surf = sv;
	}
	else
		surf = primary;
	
	DFBCHECK(surf->GetSubSurface(surf, area, &subSurf));
	
	//	if (bgnd)
	//		surf->Release(surf);
	
	fcparam = amp_frame_callback_table + ampIndex;
	fcparam->area = *area;
	fcparam->surf = subSurf;
	
	if (pCallback)
		*pCallback = frame_callback;
	
	if (pCtx)
		*pCtx = (void*)ampIndex;
	
	return subSurf;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetAgeRegionAndProfile4BDROM(RMuint16 age )
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       RMuint16 age 
 * @return 
 *		 void
*/
#ifndef NO_BLUE_SUPPORT
void set_age_region_and_profile_for_bdrom(RMuint16 age )
{
	struct SMediaSession *media = NULL;
	if (ampIndex < MAX_AMP_INSTANCES)
		media = &mediaTab[ampIndex];
	
	// set age
	media->bdPsr[13] = age > 255 ? 255 : age ;
	GUI_INFO("=============>  age = %d <===============\n",age);
	media->bdPsrSetFlag[13] = media->anyBdPsrSetFlag = true;
	//set region
#ifdef RMCFG_REGION_CODE
	media->bdPsr[20] = RMCFG_REGION_CODE ;
	media->bdPsrSetFlag[20] = media->anyBdPsrSetFlag = true;
#endif
   // set profile
#if !defined(BDROM_PLAYER_PROFILE)
	media->bdPsr[31] = 0x00200 ; //profile 1 zero
	media->bdPsrSetFlag[31] = media->anyBdPsrSetFlag = true;
#elif  BDROM_PLAYER_PROFILE ==1
	media->bdPsr[31] = 0x10200 ; //profile 1 one
	media->bdPsrSetFlag[31] = media->anyBdPsrSetFlag = true;
#elif  BDROM_PLAYER_PROFILE ==2
	media->bdPsr[31] = 0x30200 ; //profile 2
	media->bdPsrSetFlag[31] = media->anyBdPsrSetFlag = true;
#elif  BDROM_PLAYER_PROFILE ==3
	media->bdPsr[31] = 0x80200 ; //profile 3 // for audio of bluray // now not support this profile
	media->bdPsrSetFlag[31] = media->anyBdPsrSetFlag = true;
#endif
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetLanguageForBDROM(char *menu, char *subtitle, char *audio)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       char *menu, char *subtitle, char *audio
 * @return 
 *		 void
*/
void set_language_for_bdrom(char *menu, char *subtitle, char *audio)
{
	struct SMediaSession *media = NULL;
	media = &mediaTab[0];
	
	// seta menu language
	if(menu)
	{
		media->bdPsr[18] = (((UINT32)tolower(menu[0]) << BYTE_3_SHIFT) | ((UINT32)tolower(menu[1]) << BYTE_2_SHIFT) | ((UINT32)tolower(menu[2]) << BYTE_1_SHIFT));
		media->bdPsrSetFlag[18] = media->anyBdPsrSetFlag = true;
	}
	// set audio
	if(audio)
	{
		media->bdPsr[16] = (((UINT32)tolower(audio[0]) << BYTE_3_SHIFT) | ((UINT32)tolower(audio[1]) << BYTE_2_SHIFT) | ((UINT32)tolower(audio[2]) << BYTE_1_SHIFT));
		media->bdPsrSetFlag[16] = media->anyBdPsrSetFlag = true;
	}
	if(subtitle)
	{
		media->bdPsr[17] = (((UINT32)tolower(subtitle[0]) << BYTE_3_SHIFT) | ((UINT32)tolower(subtitle[1]) << BYTE_2_SHIFT) | ((UINT32)tolower(subtitle[2]) << BYTE_1_SHIFT));
		media->bdPsrSetFlag[17] = media->anyBdPsrSetFlag = true;
	}
}
#endif // NO_BLUE_SUPPORT
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetLanguageForDVD(char *menu, char *subtitle, char *audio)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       char *menu, char *subtitle, char *audio
 * @return 
 *		 void
*/

#ifndef NO_RED_SUPPORT
void set_language_for_dvd(char *menu, char *subtitle, char *audio)
{
	GUI_INFO("===============> function = %s; Line = %d <==================\n",__FUNCTION__,__LINE__);
	GUI_INFO("=============> menu=%s,audio=%s,subtitle=%s <===========\n",menu,audio,subtitle);
	if(!menu || !subtitle || !audio)
	{
		return;
	}
	else
	{
 		unsigned int sstatus;
		int i, n;
		int menu_index=0,subtitle_index=0,audio_index=0;
		char menu_temp[4],subtitle_temp[4],audio_temp[4];
		char *pIso639_language[] = { 
			/* ISO639 code pairs: "<iso639-2>", "<iso639-1>" */
			"SPA","ES","POR","PT","ZHA","ZA","JPN","JA","TGL","TL",
			"ARG","AN","AVE","AE","BAM","BM","BEN","BN","BIH","BH",
			"BOS","BS","BUL","BG","CES","CS","CHE","CE","CHU","CU",
			"CHV","CV","COR","KW","DIV","DV","EPO","EO","EST","ET",
			"EWE","EE","FAO","FO","FIJ","FJ","FRY","FY","FUL","FF",
			"GLA","GD","GLE","GA","GLV","GV","GRN","GN","HAT","HT",
			"HER","HZ","HMO","HO","IBO","IG","IDO","IO","IKU","IU",
			"ILE","IE","INA","IA","IND","ID","IPK","IK","JAV","JV",
			"KAL","KL","KAN","KN","KAS","KS","KAU","KR","KAZ","KK",
			"KHM","KM","KIN","RW","KIR","KY","KOM","KV","KON","KG",
			"KUA","KJ","LAO","LO","LAV","LV","LIN","LN","LIT","LT",
			"LTZ","LB","LUG","LG","MAH","MH","MAL","ML","MAR","MR",
			"MLG","MG","MLT","MT","MON","MN","MRI","MI","NAV","NV",
			"NBL","NR","NDO","NG","NOB","NB","ORM","OM","PLI","PI",
			"POL","PL","PUS","PS","ROH","RM","RUN","RN","SAG","SG",
			"SLK","SK","SME","SE","SND","SD","SOT","ST","SRD","SC",
			"SWE","SV","TAH","TY","TAT","TT","TSN","TN","TUK","TK",
			"TUR","TR","UIG","UG","WLN","WA"
#if 0 
			/* for the following iso639-2 language codes, the 
			 * iso639-1 equivalent is just the first 2 characters.
			 */
			,"ENG","EN","FRA","FR","DEU","DE","ITA","IT","RUS","RU",
			"DAN","DA","ZHO","ZH","KOR","KO","THA","TH","VIE","VI",
			"HIN","HI","ARA","AR","FIN","FI","NOR","NO","AAR","AA",
			"ABK","AB","AFR","AF","AKA","AK","AMH","AM","ASM","AS",
			"AVA","AV","AYM","AY","AZE","AZ","BAK","BA","BEL","BE",
			"BIS","BI","BOD","BO","BRE","BR","CAT","CA","CHA","CH",
			"COS","CO","CRE","CR","CYM","CY","DZO","DZ","ELL","EL",
			"EUS","EU","FAS","FA","GLG","GL","GUJ","GU","HAU","HA",
			"HEB","HE","HRV","HR","HUN","HU","HYE","HY","III","II",			
			"ISL","IS","KAT","KA","KIK","KI","KUR","KU","LAT","LA",
			"LIM","LI","LUB","LU","MKD","MK","MOL","MO","MSA","MS",
			"MYA","MY","NAU","NA","NDE","ND","NEP","NE","NLD","NL",
			"NNO","NN","NYA","NY","OCI","OC","OJI","OJ","ORI","OR",
			"OSS","OS","PAN","PA","QUE","QU","RON","RO","SAN","SA",
			"SIN","SI","SLV","SL","SMO","SM","SNA","SN","SOM","SO",
			"SQI","SQ","SRP","SR","SSW","SS","SUN","SU","SWA","SW",
			"TAM","TA","TEL","TE","TGK","TG","TIR","TI","TON","TO",
			"TSO","TS","TWI","TW","UKR","UK","URD","UR","UZB","UZ",
			"VEN","VE","VOL","VO","WOL","WO","XHO","XH","YID","YI",
			"YOR","YO","ZUL","ZU"
#endif /* simple iso639-2, iso639-1 pairs */
		};
	
		/* We need to map the ISO 639-2 codes to ISO 639-1 codes for DVD languages */
		n = 2*sizeof(pIso639_language)/(4+4); // number of strings in pIso639_code array
		sstatus = 0;
		GUI_INFO("==========> search audio menu subtitle  sizeofarray= %d <=============\n",sizeof(pIso639_language));

		for(i=0; i<n; i+=2)
		{
			if(!strcmp(menu,pIso639_language[i])) 
			{
				menu_index = i+1; //index to iso639-1 for DVD codes
				sstatus |= 0x1; //found menu language
			}
			if(!strcmp(subtitle,pIso639_language[i])) 
			{
				subtitle_index = i+1; //index to iso639-1 for DVD codes
				sstatus |= 0x2; //found subtitle language
			}
			if(!strcmp(audio,pIso639_language[i])) 
			{
				audio_index = i+1; //index to iso639-1 for DVD codes
				sstatus |= 0x4; //found audio language
			}

			if (sstatus == 0x7) break; // matched all three items
		}
		/* Save matched languages..for unmatched languages use first 2 chars */
		if (sstatus & 0x1) {
			strcpy(menu_temp,pIso639_language[menu_index]);
		} else {
			strcpy(menu_temp,menu);
			menu_temp[2]='\0'; //remove 3rd character
		}
		if (sstatus & 0x2) {
			strcpy(subtitle_temp,pIso639_language[subtitle_index]);
		} else {
			strcpy(subtitle_temp,subtitle);
			subtitle_temp[2]='\0'; //remove 3rd character
		}
		if (sstatus & 0x4) {
			strcpy(audio_temp,pIso639_language[audio_index]);
		} else {
			strcpy(audio_temp,audio);
			audio_temp[2]='\0'; //remove 3rd character
		}
		// convert upper-case to lower-case
		n =2;
		for(i=0;i<n;i++)
		{
			audio_temp[i]=tolower(audio_temp[i]);
			subtitle_temp[i]=tolower(subtitle_temp[i]);
			menu_temp[i]=tolower(menu_temp[i]);
		}
		if(subtitle)
		{
			dvd_settings.initialSubtitleDisplay = 1 ; // show subtitle
		}
		else
		{
			dvd_settings.initialSubtitleDisplay = 0 ; // hide subtitle
		}
		
		GUI_INFO("Menu Language=%s ;  Audio=%s ; Subtitle = %s\n", menu_temp,audio_temp,subtitle_temp);
		dvd_settings.menuLanguageCode = ( menu_temp[0] << 8 ) + menu_temp[1];
		dvd_settings.initialAudioLanguageCode = ( audio_temp[0] << 8 ) + audio_temp[1];
		dvd_settings.initialSubtitleLanguageCode = (subtitle_temp[0]) + subtitle_temp[1];
	}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * BDAge2DVDlevel(RMuint8 age)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       RMuint8 age
 * @return 
 *		 RMdvdParentalLevel
*/
#ifdef UNIFY_PARENTAL_CONTROL
RMdvdParentalLevel bd_age_to_dvd_level(RMuint8 age)
{
	RMuint8 dvdlevel;
	RMdvdParentalLevel age2dvdlevel[] = 
	{ 
		/* returns dvd-level for age: 0 <= age <= 19 */
		RM_DVD_PARENTAL_LEVEL_1, RM_DVD_PARENTAL_LEVEL_2, RM_DVD_PARENTAL_LEVEL_2, RM_DVD_PARENTAL_LEVEL_2, 
			RM_DVD_PARENTAL_LEVEL_2, RM_DVD_PARENTAL_LEVEL_2, RM_DVD_PARENTAL_LEVEL_2, RM_DVD_PARENTAL_LEVEL_2, 
			RM_DVD_PARENTAL_LEVEL_3, RM_DVD_PARENTAL_LEVEL_3, RM_DVD_PARENTAL_LEVEL_3, RM_DVD_PARENTAL_LEVEL_3, 
			RM_DVD_PARENTAL_LEVEL_3, RM_DVD_PARENTAL_LEVEL_4, RM_DVD_PARENTAL_LEVEL_5, RM_DVD_PARENTAL_LEVEL_5, 
			RM_DVD_PARENTAL_LEVEL_5, RM_DVD_PARENTAL_LEVEL_6, RM_DVD_PARENTAL_LEVEL_7, RM_DVD_PARENTAL_LEVEL_8
	};
	
	APP_ASSERT(age<=255);
	
	if ( (age > 18) && (age <= 254) )
	{
		dvdlevel = RM_DVD_PARENTAL_LEVEL_8 ;
	} else if( age == 255 )
	{
		dvdlevel = RM_DVD_PARENTAL_LEVEL_DISABLED ;		
	}
	else {
		dvdlevel = age2dvdlevel[age];
	}
	
	GUI_INFO("[(%s)%s] age: %d, dvd-level: %d\n", __FILE__, __FUNCTION__, age, dvdlevel);
	
	return (dvdlevel);
}
#endif /* UNIFY_PARENTAL_CONTROL */
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetParentalLevelForDVD(RMuint8 value)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       RMuint8 value
 * @return 
 *		 void
*/
void set_parental_level_for_dvd(RMuint8 value)
{
	GUI_INFO("[(%s)%s] dvd_settings.parentalLevel = %d\n", __FILE__, __FUNCTION__, value);
#ifdef UNIFY_PARENTAL_CONTROL
	dvd_settings.parentalLevel = bd_age_to_dvd_level(value);
#else
	dvd_settings.parentalLevel = value;	
#endif /* UNIFY_PARENTAL_CONTROL */
}
#endif // NO_RED_SUPPORT
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetNoAutoPlay(bool no_AutoPlay)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       bool no_AutoPlay
 * @return 
 *		 void
*/
#ifndef NO_BLUE_SUPPORT
void set_noauto_play(bool no_AutoPlay)
{
	noAuto = no_AutoPlay;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetReqPINStatus(bool status)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       bool status
 * @return 
 *		 void
*/
void set_reqpin_status(bool status)
{
	reqPIN = status;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * GetReqPINStatus()
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       
 * @return 
 *		 bool
*/
bool get_reqpin_status()
{
	return reqPIN;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * set_bdre_pin(char *pin)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int idx, char *pin
 * @return 
 *		 void
*/
void set_bdre_pin(char *pin)
{
	if(!pin)
		return;
	
	strcpy(stPIN, pin);
	GUI_INFO("stPIN = %s\n", stPIN);
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			get_bdre_pin()
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       
 * @return 
 *		 char*
*/
char *get_bdre_pin()
{
	return stPIN;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			CheckExecuteStopFunctionForBROM(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *			void
 * @return 
 *		 RMbool
*/
RMbool check_execute_stopfunction_for_bdrom(void)
{
	struct SMediaSession *media = NULL;
	//int i = 0;	
	media = &mediaTab[0];
	
	if (!media || !media->amp)
	{
		return RM_ERROR;
	}
	media->amp->UploadStatusChanges(media->amp, &media->status.generic, (DFBBoolean)false);
	UINT64 flags = media->status.brd.UO_mask_table;
	
	sprintf(media->statusInfo + strlen(media->statusInfo),"Disalowed UOPs: \n");
	
	if(flags & (1llu << 56))
	{
		return RM_ERROR;
	}
	GUI_INFO("=============>  stop  permission <=================\n");
	return RM_OK;
}
#endif // NO_BLUE_SUPPORT
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			CheckExecuteStopFunctionForDVD(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *			void
 * @return 
 *		 RMbool
*/
#ifndef NO_RED_SUPPORT
RMbool check_execute_stopfunction_for_dvd(void)
{
	struct SMediaSession *media = NULL;
	//int i = 0;	
	if (ampIndex < MAX_AMP_INSTANCES)
		media = &mediaTab[ampIndex];
	
	if (!media || !media->amp)
	{
		return RM_ERROR;
	}
	media->amp->UploadStatusChanges(media->amp, &media->status.generic, (DFBBoolean)false);
	UINT32 flags = media->status.dvd.permissions;
	if((flags >>=3 ) & 1 )
	{
		GUI_INFO("=================>  stop  not permission <==================\n");
		return RM_ERROR;
	}
	GUI_INFO("=================>  stop  permission <======================\n");
	return true;
}
#endif // NO_RED_SUPPORT
/**
 * 
 * <b>FUNCTION: </b>
 *
 * mediaType2String(TMediaType type)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       TMediaType type
 * @return 
 *		 char*
*/

static char *mediaType2String(TMediaType type)
{
    char *text = NULL;
    switch (type)
    {
#if 0
        case MTYPE_ELEM_NONE     : // 0x0000  ///< no elementary data exists or the format is irrelevant
            text = "none";
            break;

        case MTYPE_ELEM_UNKNOWN  : // 0x00FF  ///< elementary data exists, is relevant but the format is unknown
            text = "unknown";
            break;

        case MTYPE_ELEM_MPEG1    : // 0x0011
            text = "MPEG1 video";
            break;

        case MTYPE_ELEM_MPEG2    : // 0x0012
            text = "MPEG2 video";
            break;

        case MTYPE_ELEM_MPEG4    : // 0x0014
            text = "MPEG4 video";
            break;

        case MTYPE_ELEM_AVC      : // 0x001A
            text = "MPEG4 AVC video";
            break;

        case MTYPE_ELEM_VC1      : // 0x0020
            text = "VC1 video";
            break;

        case MTYPE_ELEM_DIVX3    : // 0x0033
            text = "DIVX 3 video";
            break;

        case MTYPE_ELEM_DIVX4    : // 0x0034
            text = "DIVX 4 video";
            break;

//        case MTYPE_ELEM_MPA      : // 0x0040      ///< any MPEG audio
//            text = "MPEG audio";
//            break;

        case MTYPE_ELEM_MP1      : // 0x0041
            text = "MPEG1 Layer 1-2 audio";
            break;

//        case MTYPE_ELEM_MP2      : // 0x0042
//            text = "MPEG1 Layer 2 audio";
//            break;

        case MTYPE_ELEM_MP3      : // 0x0043
            text = "MPEG1 Layer 3 (MP3) audio";
            break;

        case MTYPE_ELEM_MP2MC    : // 0x0044
            text = "MPEG2 (MC) audio";
            break;

        case MTYPE_ELEM_AAC      : // 0x0045
            text = "MPEG2 AAC audio";
            break;

        case MTYPE_ELEM_AACP     : // 0x0046
            text = "MPEG2 AAC+ audio";
            break;

        case MTYPE_ELEM_AC3      : // 0x0053
            text = "Dolby Digital (AC3) audio";
            break;

        case MTYPE_ELEM_DDP      : // 0x0054
            text = "Dolby Digital Plus audio";
            break;

        case MTYPE_ELEM_DLLS     : // 0x0055
            text = "? audio";
            break;

        case MTYPE_ELEM_DTS      : // 0x0060
            text = "DTS audio";
            break;

        case MTYPE_ELEM_DTSHD    : // 0x0061
            text = "DTS-HD audio";
            break;

        case MTYPE_ELEM_PCM      : // 0x0070
            text = "PCM audio";
            break;

        case MTYPE_ELEM_WMA      : // 0x0080
            text = "WMA";
            break;

        case MTYPE_ELEM_WMAPRO   : // 0x0081
            text = "WMA-PRO";
            break;

        case MTYPE_ELEM_SPU      : // 0x0090
            text = "DVD Subpicture";
            break;

        case MTYPE_ELEM_SPUHD    : // 0x0091
            text = "HD-DVD Subpicture";
            break;

        case MTYPE_ELEM_IG       : // 0x00A0
            text = "Blu-ray IG";
            break;

        case MTYPE_ELEM_PG       : // 0x00A1
            text = "Blu-ray PG";
            break;

        case MTYPE_ELEM_TS       : // 0x00A2
            text = "Blu-ray TS";
            break;

        case MTYPE_ELEM_JPEG     : // 0x00B0
            text = "JPEG graphics";
            break;

        case MTYPE_ELEM_GIF      : // 0x00B1
            text = "GIF graphics";
            break;

        case MTYPE_ELEM_PNG      : // 0x00B2
            text = "PNG graphics";
            break;

        case MTYPE_ELEM_BMP      : // 0x00B3
            text = "BMP graphics";
            break;

        case MTYPE_ELEM_TIFF     : // 0x00B4
            text = "TIFF graphics";
            break;

        case MTYPE_ELEM_PIXMAP   : // 0x00BF  ///< uncompressed pixel map
            text = "RAW graphics";
            break;

        case MTYPE_ELEM_ASCII    : // 0x00C1
            text = "ASCII text";
            break;

        case MTYPE_ELEM_UTF8     : // 0x00C2
            text = "UTF-8 text";
            break;

		case MTYPE_ELEM_TTFONT   : // 0x00C2  ///< TrueType font data
            text = "TrueType font";
            break;

        case MTYPE_ELEM_OTFONT   : // 0x00C3  ///< OpenType font data
            text = "OpenType font";
            break;

        case MTYPE_ELEM_BINDATA  : // 0x00F0  ///< any binary data
            text = "data";
            break;

        case MTYPE_ELEM_VIDEO    : // 0x00F1  ///< any video type
            text = "video";
            break;

        case MTYPE_ELEM_AUDIO    : // 0x00F2  ///< any audio type
            text = "audio";
            break;

        case MTYPE_ELEM_GRAPHICS : // 0x00F3  ///< any graphics type
            text = "graphics";
            break;

        case MTYPE_ELEM_FONT     : // 0x00F4  ///< any graphics type
            text = "font";
            break;
#endif
        default:
            text = "invalid value";
            break;
    }
    return text;
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * mediaFormat2String(char *linePrefix, struct SMediaFormat *format)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       char *linePrefix, struct SMediaFormat *format
 * @return 
 *		 char*
*/
static char *mediaFormat2String(char *linePrefix, struct SMediaFormat *format)
{
	
	static char text[256];	
    sprintf(text, "Type: %s \n", mediaType2String(GET_ELEMENTARY_MTYPE(format->mediaType)));
	
    if (format->formatValid)
    {
        switch (GET_ELEMENTARY_MTYPE(format->mediaType))
        {
#if 0
		case MTYPE_ELEM_VIDEO    : // 0x00F1  ///< any video type
		case MTYPE_ELEM_MPEG1    : // 0x0011
		case MTYPE_ELEM_MPEG2    : // 0x0012
		case MTYPE_ELEM_MPEG4    : // 0x0014
		case MTYPE_ELEM_AVC      : // 0x001A
		case MTYPE_ELEM_VC1      : // 0x0020
		case MTYPE_ELEM_DIVX3    : // 0x0033
		case MTYPE_ELEM_DIVX4    : // 0x0034
			// fall through
			
		case MTYPE_ELEM_GRAPHICS : // 0x00F3  ///< any graphics type
		case MTYPE_ELEM_JPEG     : // 0x00B0
		case MTYPE_ELEM_GIF      : // 0x00B1
		case MTYPE_ELEM_PNG      : // 0x00B2
		case MTYPE_ELEM_BMP      : // 0x00B3
		case MTYPE_ELEM_TIFF     : // 0x00B4
		case MTYPE_ELEM_PIXMAP   : // 0x00BF  ///< uncompressed pixel map
			sprintf(text + strlen(text), "width: %d \n", format->format.image.width);
			sprintf(text + strlen(text), "height: %d \n", format->format.image.height);
			sprintf(text + strlen(text), "aspectX: %d \n", format->format.image.aspectX);
			sprintf(text + strlen(text), "aspectY: %d \n", format->format.image.aspectY);
			sprintf(text + strlen(text), "interlaced: %d \n", format->format.image.interlaced);
			sprintf(text + strlen(text), "telecine: %d \n", format->format.image.telecine);
			sprintf(text + strlen(text), "rateM: %d \n", format->format.image.rateM);
			sprintf(text + strlen(text), "rateN: %d \n", format->format.image.rateN);
			break;
			
		case MTYPE_ELEM_AUDIO    : // 0x00F2  ///< any audio type
		case MTYPE_ELEM_MPA      : // 0x0040  ///< any MPEG audio
		case MTYPE_ELEM_MP1      : // 0x0041
		case MTYPE_ELEM_MP3      : // 0x0043
		case MTYPE_ELEM_AAC      : // 0x0045
		case MTYPE_ELEM_AACP     : // 0x0046
		case MTYPE_ELEM_AC3      : // 0x0053
		case MTYPE_ELEM_DDP      : // 0x0054
		case MTYPE_ELEM_DLLS     : // 0x0055
		case MTYPE_ELEM_DTS      : // 0x0060
		case MTYPE_ELEM_DTSHD    : // 0x0061
		case MTYPE_ELEM_WMA      : // 0x0080
		case MTYPE_ELEM_WMAPRO   : // 0x0081
			sprintf(text + strlen(text), "nrate: %d \n", format->format.sound.samplingFreq);
			sprintf(text + strlen(text), "channels: %d.%d \n", format->format.sound.channels, format->format.sound.lfe);
			break;
			
		case MTYPE_ELEM_PCM      : // 0x0070
			sprintf(text + strlen(text), "format: %s\n", format->format.pcm.storage.format == SF_PCM ? "PCM" : (format->format.pcm.storage.format == SF_LPCM ? "LPCM" : "INVALID"));
			sprintf(text + strlen(text), "rate: %d \n", format->format.sound.samplingFreq);
			sprintf(text + strlen(text), "channels: %d.%d \n", format->format.sound.channels, format->format.sound.lfe);
			sprintf(text + strlen(text), "bits per sample: %d \n", format->format.pcm.storage.bitsPerSample);
			sprintf(text + strlen(text), "samples per channel: %d \n", format->format.pcm.storage.samplesPerChannel);
			sprintf(text + strlen(text), "MSB first: %s \n", format->format.pcm.storage.MsbFirst ? "Yes" : "No");
			break;
			
		case MTYPE_ELEM_SPU      : // 0x0090
		case MTYPE_ELEM_SPUHD    : // 0x0091
		case MTYPE_ELEM_IG       : // 0x00A0
		case MTYPE_ELEM_PG       : // 0x00A1
		case MTYPE_ELEM_TS       : // 0x00A2
			
		case MTYPE_ELEM_ASCII    : // 0x00C1
		case MTYPE_ELEM_UTF8     : // 0x00C2
		case MTYPE_ELEM_TTFONT   : // 0x00C2  ///< TrueType font data
		case MTYPE_ELEM_OTFONT   : // 0x00C3  ///< OpenType font data
		case MTYPE_ELEM_BINDATA  : // 0x00F0  ///< any binary data
		case MTYPE_ELEM_FONT     : // 0x00F4  ///< any graphics type
			
		case MTYPE_ELEM_NONE     : // 0x0000  ///< no elementary data exists or the format is irrelevant
		case MTYPE_ELEM_UNKNOWN  : // 0x00FF  ///< elementary data exists, is relevant but the format is unknown
			break;
#endif			
		default:
			break;
        }
    }
    return text;
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * processMediaQuery(struct SMediaSession *media)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       struct SMediaSession *media
 * @return 
 *		 void
*/
static void processMediaQuery(struct SMediaSession *media)
{
	if (!media->msRT)
		return;

	// format-specific command values, generic handling
    if (0
#ifndef NO_RED_SUPPORT
    	|| ((media->msRT->mediaSpace == MEDIA_SPACE_DVD) && (*(media->msRT->pCmd) == (int) DVDCmd_Query_Bookmark))
    	|| ((media->msRT->mediaSpace == MEDIA_SPACE_DVD_VR) && (*(media->msRT->pCmd) == (int) VRCmd_Query_Bookmark))
#endif
#ifndef NO_BLUE_SUPPORT
    	|| ((media->msRT->mediaSpace == MEDIA_SPACE_BLU_RAY) && (*(media->msRT->pCmd) == (int) BRDCmd_query_bookmark))
    	|| ((media->msRT->mediaSpace == MEDIA_SPACE_BDRE) && (*(media->msRT->pCmd) == (int) BDRECmd_query_bookmark))
    	|| ((media->msRT->mediaSpace == MEDIA_SPACE_BDJ) && (*(media->msRT->pCmd) == (int) BRDCmd_query_bookmark))
#endif
		)
    {
        sprintf(media->queryResult,
                "%cLast Query: %cBookmark\n"
                "%c  Bookmark %s (%d bytes)",
				CHAR_CODE_SECTION, CHAR_CODE_HIGHLIGHT,
				CHAR_CODE_NORMAL, media->bookmarkBuf ? "set" : "not set",
				media->bookmarkBuf ? media->bookmarkSize : 0);
        return;
    }

    // format-aware handling
    switch (media->msRT->mediaSpace)
    {
        case MEDIA_SPACE_LINEAR_MEDIA:
            if (media->msCmd->cmd == (int) LPBCmd_GET_STREAM_INFO)
                sprintf(media->queryResult,
                        "%cLast Query: %cStream Info \n"
                        "%c/tIndex: %d \n"
                        "/tName: %s \n"
                        "/t%s",
                        CHAR_CODE_SECTION, CHAR_CODE_HIGHLIGHT,
                        CHAR_CODE_NORMAL, media->result.lpb.value.streamInfo.index,
                        media->result.lpb.value.streamInfo.name,
                        mediaFormat2String("  ", &media->result.lpb.value.streamInfo.format));


            break;

#ifndef NO_RED_SUPPORT
        case MEDIA_SPACE_DVD:
            switch (media->msCmd->cmd)
            {
                case DVDCmd_QueryTitle:
                {
                    static char *titleType[] =
                    {
                        "",
                        "FirstPlay",
                        "TopMenu",
                        "Menu",
                        "Sequential",
                        "Random"
                    };

                    sprintf(media->queryResult,
                            "Last Query: Title Info\n"
                            "  Type: %s\n"
                            "  Number: %d\n"
                            "  Video Chapters: %d\n"
                            "  Audio Streams: %d\n"
                            "  SPU Streams: %d\n"
                            "  Angles: %d\n",
                            titleType[media->result.dvd.info.title.type],
                            media->result.dvd.info.title.number,
                            media->result.dvd.info.title.chapters,
                            media->result.dvd.info.title.audios,
                            media->result.dvd.info.title.spus,
                            media->result.dvd.info.title.angles);

                    break;
                }

                case DVDCmd_QueryTXTDT:
                {
                    char *charSet;
                    char titleName[128] = "unsuported character set";
                    char subTitleName[128] = "unsuported character set";
                    char orgTitleName[128] = "unsuported character set";
                    char producerName[128] = "unsuported character set";
                    char directorName[128] = "unsuported character set";
                    char scenarioWriter[128] = "unsuported character set";
                    char originalWriter[128] = "unsuported character set";
                    char actorsName[10][128] = {"", "", "", "", "", "", "", "", "", ""};
                    int i;

                    switch (media->result.dvd.info.TXTDT.charset)
                    {
                        case RM_DVD_CHARSET_UNICODE: // 0x0,
                            charSet = "Unicode";
                            break;

                        case RM_DVD_CHARSET_ISO646: // 0x1, /* 7 bit ascii */
                            charSet = "ISO 646 (7 bit ASCII)";
                            goto _ascii;
                            break;

                        case RM_DVD_CHARSET_JIS: // 0x10, /* JIS Roman & JIS Kanji 1990. see Annex A.4 */
                            charSet = "JIS";
                            break;

                        case RM_DVD_CHARSET_ISO8859: // 0x11, /* 8 bit ascii */
                            charSet = "ISO 8859 (8 bit ASCII)";
_ascii:
                                    strcpy(titleName, (char *)media->result.dvd.info.TXTDT.titleName);
                                    strcpy(subTitleName, (char *)media->result.dvd.info.TXTDT.subTitleName);
                                    strcpy(orgTitleName, (char *)media->result.dvd.info.TXTDT.orgTitleName);
                                    strcpy(producerName, (char *)media->result.dvd.info.TXTDT.producerName);
                                    strcpy(directorName, (char *)media->result.dvd.info.TXTDT.directorName);
                                    strcpy(scenarioWriter, (char *)media->result.dvd.info.TXTDT.scenarioWriter);
                                    strcpy(originalWriter, (char *)media->result.dvd.info.TXTDT.originalWriter);
                                    for (i=0; i<10; i++)
                                        strcpy(actorsName[i], (char *)media->result.dvd.info.TXTDT.actorsName[i]); /* max = 10. Too bad for the others. */

                            break;

                        case RM_DVD_CHARSET_SHIFT_JIS: // 0x12 /* Shift JIS Kanji including JIS Roman & JIS Katakana */
                            charSet = "Shift JIS";
                            break;

                        default:
                            charSet = "invalid";
                            break;
                    };


                    sprintf(media->queryResult,
                            "Last Query: Title Text Info\n"
                            "  Character set: %s\n"
                            "  Title name: %s\n"
                            "  Sub title name: %s\n"
                            "  Original title name: %s\n"
                            "  Producer: %s\n"
                            "  Director: %s\n"
                            "  Scenario writer: %s\n"
                            "  Original writer: %s\n"
                            "  Actors:\n",
                            charSet,
                            titleName,
                            subTitleName,
                            orgTitleName,
                            producerName,
                            directorName,
                            scenarioWriter,
                            originalWriter);

                    for (i=0; i<10; i++)
                        if (actorsName[i][0])
                        {
                            strcat(media->queryResult, "    ");
                            strcat(media->queryResult, actorsName[i]);
                            strcat(media->queryResult, "\n");
                        }

                    break;
                }

                case DVDCmd_QuerySPRMs:
                {
                    RMuint16 *reg = media->result.dvd.info.SPRMs;

                    sprintf(media->queryResult,
                            "Last Query: SP Registers\n"
                            "  %#02x %#02x %#02x %#02x %#02x %#02x %#02x %#02x\n"
                            "  %#02x %#02x %#02x %#02x %#02x %#02x %#02x %#02x\n"
                            "  %#02x %#02x %#02x %#02x %#02x %#02x %#02x %#02x\n",
                            reg[ 0], reg[ 1], reg[ 2], reg[ 3], reg[ 4], reg[ 5], reg[ 6], reg[ 7],
                            reg[ 8], reg[ 9], reg[10], reg[11], reg[12], reg[13], reg[14], reg[15],
                            reg[16], reg[17], reg[18], reg[19], reg[20], reg[21], reg[22], reg[23]);
                    break;
                }

                case DVDCmd_QueryGPRMs:
                {
                    RMuint16 *reg = media->result.dvd.info.SPRMs;

                    sprintf(media->queryResult,
                            "Last Query: GP Registers\n"
                            "  %#02x %#02x %#02x %#02x %#02x %#02x %#02x %#02x\n"
                            "  %#02x %#02x %#02x %#02x %#02x %#02x %#02x %#02x\n",
                            reg[ 0], reg[ 1], reg[ 2], reg[ 3], reg[ 4], reg[ 5], reg[ 6], reg[ 7],
                            reg[ 8], reg[ 9], reg[10], reg[11], reg[12], reg[13], reg[14], reg[15]);
                    break;
                }
                default:
                    break;
            }
            break;

        case MEDIA_SPACE_DVD_VR:
		{
			static char *dvdvrPlaylistType[] =
			{
				"Movie",
				"Still Picture",
				"Hybrid"
			};

            switch (media->msCmd->cmd)
            {
                case VRCmd_Query_PG:
                    sprintf(media->queryResult,
								"%cLast Query: Query PG\n"
								"  Index: %d\n"
								"  Name: %s\n"
								"  Entry points: %d\n"
								"  Cell count: %d\n",
								CHAR_CODE_SECTION,
								media->result.vr.info.pg.number,
								media->result.vr.info.pg.name.asciiName,
								media->result.vr.info.pg.entryPoints,
								media->result.vr.info.pg.cellCount
							);
					break;
                case VRCmd_Query_PL:
					sprintf(media->queryResult,
								"%cLast Query: Query PL\n"
								"  Index: %d\n"
								"  Name: %s\n"
								"  Type: %s\n"
								"  Created: %d/%d/%d %d:%d:%d\n"
								"  Entry points: %d\n"
								"  Cell count: %d\n",
								CHAR_CODE_SECTION,
								media->result.vr.info.pl.number,
								media->result.vr.info.pl.name.asciiName,
								dvdvrPlaylistType[media->result.vr.info.pl.type],
									media->result.vr.info.pl.creationTime.Year,
									media->result.vr.info.pl.creationTime.Month,
									media->result.vr.info.pl.creationTime.Day,
									media->result.vr.info.pl.creationTime.Hour,
									media->result.vr.info.pl.creationTime.Minute,
									media->result.vr.info.pl.creationTime.Second,
								media->status.vr.playList.entryPoints,
								media->status.vr.playList.cellCount
							);
					break;
				case VRCmd_Query_PG_Cell:
					sprintf(media->queryResult,
								"%cLast Query: Query PG Cell\n"
								"  Idx: %d, Type: %s\n"
								"  Create Time: %d/%d/%d %d:%d:%d\n",
								CHAR_CODE_SECTION,
								media->result.vr.info.cell.idx,
								media->result.vr.info.cell.type?"Still":"Movie",
									media->result.vr.info.cell.create_time.Year,
									media->result.vr.info.cell.create_time.Month,
									media->result.vr.info.cell.create_time.Day,
									media->result.vr.info.cell.create_time.Hour,
									media->result.vr.info.cell.create_time.Minute,
									media->result.vr.info.cell.create_time.Second
							);
					break;
				case VRCmd_Query_PL_Cell:
					sprintf(media->queryResult,
								"%cLast Query: Query PL Cell\n"
								"  Idx: %d, Type: %s\n"
								"  Create Time: %d/%d/%d %d:%d:%d\n",
								CHAR_CODE_SECTION,
								media->result.vr.info.cell.idx,
								media->result.vr.info.cell.type?"Still":"Movie",
									media->result.vr.info.cell.create_time.Year,
									media->result.vr.info.cell.create_time.Month,
									media->result.vr.info.cell.create_time.Day,
									media->result.vr.info.cell.create_time.Hour,
									media->result.vr.info.cell.create_time.Minute,
									media->result.vr.info.cell.create_time.Second
							);
					break;

                case VRCmd_Query_PG_EP_Name:
					sprintf(media->queryResult,
								"%cLast Query: Query PG EP\n"
								"  Name: %s\n",
								CHAR_CODE_SECTION,
								media->result.vr.info.ep.asciiName
							);
					break;
                case VRCmd_Query_PL_EP_Name:
					sprintf(media->queryResult,
								"%cLast Query: Query PL EP\n"
								"  Name: %s\n",
								CHAR_CODE_SECTION,
								media->result.vr.info.ep.asciiName
							);
					break;
                case VRCmd_Query_Info:
					sprintf(media->queryResult,
							"%cLast Query: Query Info\n"
							"  Idx: %d\n"
							"  Text: %s\n",
							CHAR_CODE_SECTION,
							media->result.vr.info.info.infoID,
							media->result.vr.info.info.info
						);
                    break;
                default:
                    break;
            }
            break;
		}
#endif
#ifndef NO_BLUE_SUPPORT
        case MEDIA_SPACE_BDJ:
        case MEDIA_SPACE_BLU_RAY:
        {
			static char *brdAudioFormatName[16] = {"Invalid"};	// sparse array, initialized dynamically
			static char *brdAudioFreqName[16] = {"Invalid"};	// sparse array, initialized dynamically
			static char *brdStreamTypeName[256] = {"Invalid"};
			static bool init = false;

			if (!init)
			{
				#define INIT_TABLE(table, name, type) table[type] = name

				INIT_TABLE(brdStreamTypeName, "MPEG2", MPEG2);
				INIT_TABLE(brdStreamTypeName, "AVC", AVC);
				INIT_TABLE(brdStreamTypeName, "VC1", VC1);
				INIT_TABLE(brdStreamTypeName, "LPCM", LPCM);
				INIT_TABLE(brdStreamTypeName, "DolbyDigital", DolbyDigital);
				INIT_TABLE(brdStreamTypeName, "DTS", DTS);
				INIT_TABLE(brdStreamTypeName, "DolbyLossless", DolbyLossless);
				INIT_TABLE(brdStreamTypeName, "DDPlusPrimary", DDPlusPrimary);
				INIT_TABLE(brdStreamTypeName, "DTSHDPrimary", DTSHDPrimary);
				INIT_TABLE(brdStreamTypeName, "DTSHDXLLPrimary", DTSHDXLLPrimary);
				INIT_TABLE(brdStreamTypeName, "DDPlusSecondary", DDPlusSecondary);
				INIT_TABLE(brdStreamTypeName, "DTSHDSecondary", DTSHDSecondary);
				INIT_TABLE(brdStreamTypeName, "PG", PG);
				INIT_TABLE(brdStreamTypeName, "IG", IG);
				INIT_TABLE(brdStreamTypeName, "TS", TS);

    			INIT_TABLE(brdAudioFormatName, "Mono", AudioType_mono);
    			INIT_TABLE(brdAudioFormatName, "Dual", AudioTypeRE_dual_mono);
    			INIT_TABLE(brdAudioFormatName, "Stereo", AudioType_stereo);
    			INIT_TABLE(brdAudioFormatName, "Multilingual", AudioTypeRE_multilingual);
    			INIT_TABLE(brdAudioFormatName, "Surround", AudioTypeRE_surround);
    			INIT_TABLE(brdAudioFormatName, "Multi channel", AudioType_multiChannel);
    			INIT_TABLE(brdAudioFormatName, "Stereo with mc extension", AudioType_stereoBase_multiChannelExt);
    			INIT_TABLE(brdAudioFormatName, "Audio description", AudioTypeRE_audio_description);
    			INIT_TABLE(brdAudioFormatName, "Audio for hard of hearing", AudioTypeRE_audio_hard_hearing);

    			INIT_TABLE(brdAudioFreqName, "48", AudioFreq_48);
    			INIT_TABLE(brdAudioFreqName, "44.1", AudioSamplFreqRE_44_1);
    			INIT_TABLE(brdAudioFreqName, "32", AudioSamplFreqRE_32);
    			INIT_TABLE(brdAudioFreqName, "96", AudioFreq_96);
    			INIT_TABLE(brdAudioFreqName, "192", AudioFreq_192);
    			INIT_TABLE(brdAudioFreqName, "48/192", AudioFreq_48Base_192Ext);
    			INIT_TABLE(brdAudioFreqName, "48/96", AudioFreq_48Base_96Ext);

				init = true;
			}


            switch (media->msCmd->cmd)
            {
                case BRDCmd_get_psr:
                	sprintf(media->queryResult,
                			"%cLast Query: get_psr "
                			"PSR[%d]=0x%lX \n",
                			CHAR_CODE_SECTION,
                			ampCommand.brd.param1.selector,
                			(unsigned long)media->result.brd.info.regValue);
                	break;

                case BRDCmd_get_gpr:
                	sprintf(media->queryResult,
                			"%cLast Query: get_gpr "
                			"GPR[%d]=0x%lX \n",
                			CHAR_CODE_SECTION,
                			ampCommand.brd.param1.selector,
                			(unsigned long)media->result.brd.info.regValue);
                	break;

                case BRDCmd_get_title_info:
            	{
                    if(media->result.brd.info.title.objectType)
      				{
 	            		sprintf(media->queryResult,
		             		"%cLast Query: get_title_info \n"
                            "Number: %d \n"
                            "Object Type: %s \n"
                            "Title search: %s \n"
                            "Title display: %s \n"
                            "Playback: %s \n"
                            "ObjectID: %d \n"
                            "PlayListCount: %d \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.title.title_number,
                            media->result.brd.info.title.objectType == ObjectType_HDMV ? "HDMV" : "BD-J",
                            media->result.brd.info.title.accessType == TitleAccessAndDisplay ? "Y" : "N",
                            media->result.brd.info.title.accessType != NoTitleSupport ? "Y" : "N",
                            media->result.brd.info.title.info.HDMV.playbackType == HDMVSequential ? "Sequential" : "Interactive",
                            media->result.brd.info.title.info.HDMV.mobj_id_ref,
                            media->result.brd.info.title.info.HDMV.playListCount);
      				}
                    else
                    {
 	            		sprintf(media->queryResult,
                    		"%cLast Query: get_title_info \n"
                            "Number: %d \n"
                            "Object Type: %s \n"
                            "Title search: %s \n"
                            "Title display: %s \n"
                            "Playback: %s \n"
								"ObjectID: %ld \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.title.title_number,
                            media->result.brd.info.title.objectType == ObjectType_HDMV ? "HDMV" : "BD-J",
                            media->result.brd.info.title.accessType == TitleAccessAndDisplay ? "Y" : "N",
                            media->result.brd.info.title.accessType != NoTitleSupport ? "Y" : "N",
                            media->result.brd.info.title.info.BDJ.playbackType == BDJSequential ? "Sequential" : "Interactive",
                            media->result.brd.info.title.info.BDJ.bdjo_id);
                    }
                	break;
            	}

            	case BRDCmd_get_pl_id:
					sprintf(media->queryResult,
                            "%cLast Query: BRDCmd_get_pl_id \n"
                            "playlist_id: %ld \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.playListID);
                    break;

                case BRDCmd_get_pl_info:
				{
					static char *brdPlaybackType[] =
					{
						"",
						"Sequential",
						"Random",
						"Shuffle"
					};

					sprintf(media->queryResult,
                            "%cLast Query: get_pl_info \n"
                            "playList_id: %ld \n"
                            "type: %s \n"
                            "number_of_playItems: %d \n"
                            "number_of_marks: %d \n"
                            "number_of_chapters: %d \n"
                            "startMediaTime: %ld \n"
                            "endMediaTime: %ld \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.playList.playList_id,
                            brdPlaybackType[media->result.brd.info.playList.playback_type],
                            media->result.brd.info.playList.number_of_playItems,
                            media->result.brd.info.playList.number_of_marks,
                            media->result.brd.info.playList.number_of_chapters,
                            (unsigned long)media->result.brd.info.playList.startMediaTime,
                            (unsigned long)media->result.brd.info.playList.endMediaTime);
               		break;
				}
                case BRDCmd_get_pi_info:
					sprintf(media->queryResult,
                            "%cLast Query: get_pi_info \n"
                            "playItem_id: %d \n"
                            "clip_id: %ld \n"
                            "number_of_angles: %d \n"
                            "number_of_video_streams: %d \n"
                            "number_of_primary_audio_streams: %d \n"
                            "number_of_pg_ts_streams: %d \n"
                            "number_of_ig_streams: %d \n"
                            "number_of_secondary_audio_streams: %d \n"
                            "number_of_secondary_video_streams: %d \n"
                            "number_of_PiP_PG_textST_streams: %d \n"
                            "startMediaTime: %ld \n"
                            "endMediaTime: %ld \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.playItem.playItem_id,
                            media->result.brd.info.playItem.clip_id,
                            media->result.brd.info.playItem.number_of_angles,
                            media->result.brd.info.playItem.number_of_video_streams,
                            media->result.brd.info.playItem.number_of_primary_audio_streams,
                            media->result.brd.info.playItem.number_of_pg_ts_streams,
                            media->result.brd.info.playItem.number_of_ig_streams,
                            media->result.brd.info.playItem.number_of_secondary_audio_streams,
                            media->result.brd.info.playItem.number_of_secondary_video_streams,
                            media->result.brd.info.playItem.number_of_PiP_PG_textST_streams,
                            (unsigned long)media->result.brd.info.playItem.startMediaTime,
                            (unsigned long)media->result.brd.info.playItem.endMediaTime);
                    break;

                case BRDCmd_get_mark_info:
					sprintf(media->queryResult,
                            "%cLast Query: get_mark_info \n"
                            "mark_id: %d \n"
                            "mark_type: %s \n"
                            "ref_playitem_id: %d \n"
                            "mark_time_stamp: %ld \n"
                            "duration: %ld \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.mark.PL_mark_id,
                            media->result.brd.info.mark.mark_type == 0x1 ? "entry" : "link",
                            media->result.brd.info.mark.ref_playitem_id,
                            (unsigned long)media->result.brd.info.mark.mark_time_stamp,
                            (unsigned long)media->result.brd.info.mark.duration);
                     break;

                case BRDCmd_get_chapter_info:
					sprintf(media->queryResult,
                            "%cLast Query: get_chapter_info \n"
                            "chapter_number: %d \n"
                            "startMediaTime: %ld \n"
                            "endMediaTime: %ld \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.chapter.chapter_number,
                            (unsigned long)media->result.brd.info.chapter.startMediaTime,
                            (unsigned long)media->result.brd.info.chapter.endMediaTime);
                     break;

                case BRDCmd_get_video_stream_info:
				{

					static char *brdVideoFormatName[] =
					{
						"Invalid",
						" 480i",
						" 576i",
						" 480p",
						"1080i",
						" 720p",
						"1080p",
						" 576p"
					};

					static char *brdFrameRateName[] =
					{
						"Invalid",
						"23.97",
						"24   ",
						"25   ",
						"29.97",
						"30   ",	// BD-RE only
						"50   ",
						"59.94",
						"60   "		// BD-RE only
					};

					static char *brdAspectRatioName[] =
					{
						"Invalid",
						"Invalid",
						"  4:3  ",
						" 16:9  ",
						"221:100"	// BD-RE only
					};

					sprintf(media->queryResult,
                            "%cLast Query: get_video_stream_info \n"
                            "stream_number: %d \n"
                            "pid: 0x%X \n"
                            "stream_coding_type: %s \n"
                            "video_format: %s \n"
                            "frame_rate: %s \n"
                            "aspect_ratio: %s \n"
                            "cc_flag: %s \n"
                            "ISRC: %s\n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.stream.stream_number,
                            media->result.brd.info.stream.pid,
                            brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							brdVideoFormatName[media->result.brd.info.stream.codingInfo.stream.video.video_format],
							brdFrameRateName[media->result.brd.info.stream.codingInfo.stream.video.frame_rate],
							brdAspectRatioName[media->result.brd.info.stream.codingInfo.stream.video.aspect_ratio],
                            media->result.brd.info.stream.codingInfo.stream.video.cc_flag ? "Y" : "N",
                            (char*)&media->result.brd.info.stream.codingInfo.ISRC);
                     break;
				}
                case BRDCmd_get_audio_stream_info:
					sprintf(media->queryResult,
                            "%cLast Query: get_audio_stream_info \n"
                            "stream_number: %d \n"
                            "pid: 0x%X \n"
                            "stream_coding_type: %s \n"
                            "audio_presentation_type: %s \n"
                            "sampling_frequency: %sKHz \n"
                            "language_code: %c%c%c \n"
                            "ISRC: %s\n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.stream.stream_number,
                            media->result.brd.info.stream.pid,
                            brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							brdAudioFormatName[media->result.brd.info.stream.codingInfo.stream.audio.audio_presentation_type],
							brdAudioFreqName[media->result.brd.info.stream.codingInfo.stream.audio.sampling_frequency],
							(char)((media->result.brd.info.stream.codingInfo.stream.audio.audio_language_code >> 16) & 0xFF),
							(char)((media->result.brd.info.stream.codingInfo.stream.audio.audio_language_code >> 8) & 0xFF),
							(char)(media->result.brd.info.stream.codingInfo.stream.audio.audio_language_code & 0xFF),
                            (char*)&media->result.brd.info.stream.codingInfo.ISRC);
                	break;

                case BRDCmd_get_pg_ts_stream_info:
					if(media->result.brd.info.stream.codingInfo.stream_coding_type == PG)
						sprintf(media->queryResult,
                            "%cLast Query: get_pg_ts_stream_info \n"
                            "stream_number: %d \n"
                            "pid: 0x%X \n"
                            "stream_coding_type: %s \n"
                            "language_code: %c%c%c \n"
                            "ISRC: %s\n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.stream.stream_number,
                            media->result.brd.info.stream.pid,
                            brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							(char)((media->result.brd.info.stream.codingInfo.stream.pg.PG_language_code >> 16) & 0xFF),
							(char)((media->result.brd.info.stream.codingInfo.stream.pg.PG_language_code >> 8) & 0xFF),
							(char)(media->result.brd.info.stream.codingInfo.stream.pg.PG_language_code & 0xFF),
                            (char*)&media->result.brd.info.stream.codingInfo.ISRC);
					else if(media->result.brd.info.stream.codingInfo.stream_coding_type == TS)
						sprintf(media->queryResult,
							"%cLast Query: get_pg_ts_stream_info \n"
							"stream_number: %d \n"
							"pid: 0x%X \n"
							"stream_coding_type: %s \n"
							"language_code: %c%c%c \n"
							"ISRC: %s\n"
							"character_code: %c\n",
							CHAR_CODE_SECTION,
							media->result.brd.info.stream.stream_number,
							media->result.brd.info.stream.pid,
							brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							(char)((media->result.brd.info.stream.codingInfo.stream.ts.TS_language_code >> 16) & 0xFF),
							(char)((media->result.brd.info.stream.codingInfo.stream.ts.TS_language_code >> 8) & 0xFF),
							(char)(media->result.brd.info.stream.codingInfo.stream.ts.TS_language_code & 0xFF),
							(char*)&media->result.brd.info.stream.codingInfo.ISRC,
                            (int)media->result.brd.info.stream.codingInfo.stream.ts.character_code);
                	break;

                case BRDCmd_get_ig_stream_info:
					sprintf(media->queryResult,
                            "%cLast Query: get_ig_stream_info \n"
                            "stream_number: %d \n"
                            "pid: 0x%X \n"
                            "stream_coding_type: %s \n"
                            "language_code: %c%c%c \n"
                            "ISRC: %s \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.stream.stream_number,
                            media->result.brd.info.stream.pid,
                            brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							(char)((media->result.brd.info.stream.codingInfo.stream.ig.IG_language_code >> 16) & 0xFF),
							(char)((media->result.brd.info.stream.codingInfo.stream.ig.IG_language_code >> 8) & 0xFF),
							(char)( media->result.brd.info.stream.codingInfo.stream.ig.IG_language_code & 0xFF),
                            (char*)&media->result.brd.info.stream.codingInfo.ISRC);
                	break;

				case BRDCmd_get_sec_video_stream_info:
				{
		
					static char *brdVideoFormatName[] =
					{
						"Invalid",
						" 480i",
						" 576i",
						" 480p",
						"1080i",
						" 720p",
						"1080p",
						" 576p"
					};
		
					static char *brdFrameRateName[] =
					{
						"Invalid",
						"23.97",
						"24   ",
						"25   ",
						"29.97",
						"30   ",	// BD-RE only
						"50   ",
						"59.94",
						"60   "		// BD-RE only
					};
		
					static char *brdAspectRatioName[] =
					{
						"Invalid",
						"Invalid",
						"  4:3  ",
						" 16:9  ",
						"221:100"	// BD-RE only
					};
		
					sprintf(media->queryResult,
							"%cLast Query: get_sec_video_stream_info \n"
							"stream_number: %d \n"
							"pid: 0x%X \n"
							"stream_coding_type: %s \n"
							"video_format: %s \n"
							"frame_rate: %s \n"
							"aspect_ratio: %s \n"
							"cc_flag: %s \n"
							"ISRC: %s\n",
							CHAR_CODE_SECTION,
							media->result.brd.info.stream.stream_number,
							media->result.brd.info.stream.pid,
							brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							brdVideoFormatName[media->result.brd.info.stream.codingInfo.stream.video.video_format],
							brdFrameRateName[media->result.brd.info.stream.codingInfo.stream.video.frame_rate],
							brdAspectRatioName[media->result.brd.info.stream.codingInfo.stream.video.aspect_ratio],
							media->result.brd.info.stream.codingInfo.stream.video.cc_flag ? "Y" : "N",
							(char*)&media->result.brd.info.stream.codingInfo.ISRC);
					 break;
				}

                case BRDCmd_get_sec_audio_stream_info:
					sprintf(media->queryResult,
                            "%cLast Query: get_sec_audio_stream_info \n"
                            "stream_number: %d \n"
                            "pid: 0x%X \n"
                            "stream_coding_type: %s \n"
                            "audio_presentation_type: %s \n"
                            "sampling_frequency: %s \n"
                            "language_code: %c%c%c \n"
                            "ISRC: %s \n",
                            CHAR_CODE_SECTION,
                            media->result.brd.info.stream.stream_number,
                            media->result.brd.info.stream.pid,
                            brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							brdAudioFormatName[media->result.brd.info.stream.codingInfo.stream.audio.audio_presentation_type],
							brdAudioFreqName[media->result.brd.info.stream.codingInfo.stream.audio.sampling_frequency],
							(char)((media->result.brd.info.stream.codingInfo.stream.audio.audio_language_code >> 16) & 0xFF),
							(char)((media->result.brd.info.stream.codingInfo.stream.audio.audio_language_code >> 8) & 0xFF),
							(char)(media->result.brd.info.stream.codingInfo.stream.audio.audio_language_code & 0xFF),
                            (char*)&media->result.brd.info.stream.codingInfo.ISRC);
                	break;

				case BRDCmd_get_PiP_PG_textST_stream_info:
					if(media->result.brd.info.stream.codingInfo.stream_coding_type == PG)
						sprintf(media->queryResult,
							"%cLast Query: get_PiP_PG_textST_stream_info \n"
							"stream_number: %d \n"
							"pid: 0x%X \n"
							"stream_coding_type: %s \n"
							"language_code: %c%c%c \n"
							"ISRC: %s\n",
							CHAR_CODE_SECTION,
							media->result.brd.info.stream.stream_number,
							media->result.brd.info.stream.pid,
							brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							(char)((media->result.brd.info.stream.codingInfo.stream.pg.PG_language_code >> 16) & 0xFF),
							(char)((media->result.brd.info.stream.codingInfo.stream.pg.PG_language_code >> 8) & 0xFF),
							(char)(media->result.brd.info.stream.codingInfo.stream.pg.PG_language_code & 0xFF),
							(char*)&media->result.brd.info.stream.codingInfo.ISRC);
					else if(media->result.brd.info.stream.codingInfo.stream_coding_type == TS)
						sprintf(media->queryResult,
							"%cLast Query: get_PiP_PG_textST_stream_info \n"
							"stream_number: %d \n"
							"pid: 0x%X \n"
							"stream_coding_type: %s \n"
							"language_code: %c%c%c \n"
							"ISRC: %s\n"
							"character_code: %c\n",
							CHAR_CODE_SECTION,
							media->result.brd.info.stream.stream_number,
							media->result.brd.info.stream.pid,
							brdStreamTypeName[media->result.brd.info.stream.codingInfo.stream_coding_type],
							(char)((media->result.brd.info.stream.codingInfo.stream.ts.TS_language_code >> 16) & 0xFF),
							(char)((media->result.brd.info.stream.codingInfo.stream.ts.TS_language_code >> 8) & 0xFF),
							(char)(media->result.brd.info.stream.codingInfo.stream.ts.TS_language_code & 0xFF),
							(char*)&media->result.brd.info.stream.codingInfo.ISRC,
							(int)media->result.brd.info.stream.codingInfo.stream.ts.character_code);
					break;

                default:
                    break;
            }
            break;
        }
		case MEDIA_SPACE_BDRE:
			switch (media->msCmd->cmd)
            {
				case BDRECmd_get_pl_info:
					sprintf(media->queryResult,
							"%cLast query: PlayList Info\n"
							"PlayList ID: %ld \n"
							"PlayList type: %s \n"
							"protection: %s \n"
							"writable: %s \n"
							"played: %s \n"
							"edited: %s \n"
							"Character set: %s \n"
							"PlayList name: %s \n"
							"PlayList detail: %s \n"
							"Channel name: %s \n"
							"Channel number: %d \n"
							"PlayList duration: %02d:%02d:%02d \n"
							"Recorded on %d.%02d.%02d at %02d:%02d:%02d \n"
							"Timezone: %c%d.%d hours from UTC, %s \n"
							"Number of PlayItems: %d \n"
							"Number of PlayListMarks: %d \n"
							"Menu thumbnail index: %d (%s) \n",
							CHAR_CODE_SECTION,
							media->result.bdre.info.play_list.playList_id,
							(media->result.bdre.info.play_list.pl_type == T_REAL_PLAYLIST) ? "rpls" : "vpls",
							media->result.bdre.info.play_list.ui_info.flags.playback_protect_flag ? "yes" : "no",
							media->result.bdre.info.play_list.ui_info.flags.write_protect_flag ? "no" : "yes",
							media->result.bdre.info.play_list.ui_info.flags.is_played_flag ? "yes" : "no",
							media->result.bdre.info.play_list.ui_info.flags.is_edited_flag ? "yes" : "no",
							bdreCharSetName[media->result.bdre.info.play_list.ui_info.PlayList_char_set],
							media->result.bdre.info.play_list.ui_info.PlayList_name,
							media->result.bdre.info.play_list.ui_info.PlayList_detail,
							media->result.bdre.info.play_list.ui_info.channel_name,
							media->result.bdre.info.play_list.ui_info.channel_number,
							media->result.bdre.info.play_list.ui_info.PlayList_duration.hours,
							media->result.bdre.info.play_list.ui_info.PlayList_duration.minutes,
							media->result.bdre.info.play_list.ui_info.PlayList_duration.seconds,
							media->result.bdre.info.play_list.ui_info.record_time_and_date.year,
							media->result.bdre.info.play_list.ui_info.record_time_and_date.month,
							media->result.bdre.info.play_list.ui_info.record_time_and_date.day,
							media->result.bdre.info.play_list.ui_info.record_time_and_date.hour,
							media->result.bdre.info.play_list.ui_info.record_time_and_date.minute,
							media->result.bdre.info.play_list.ui_info.record_time_and_date.second,
							(media->result.bdre.info.play_list.ui_info.time_zone.time_zone_sign == 0) ? '+' : '-',
							media->result.bdre.info.play_list.ui_info.time_zone.time_zone_val,
							(media->result.bdre.info.play_list.ui_info.time_zone.half_hour_flag == 1) ? 5 : 0,
							(media->result.bdre.info.play_list.ui_info.time_zone.daylight_saving_time_flag == 1) ? "daylight saving time" : "standard time",
							media->result.bdre.info.play_list.number_of_PlayItems,
							media->result.bdre.info.play_list.number_of_PlayList_marks,
							media->result.bdre.info.play_list.ref_menu_thmb_idx,
							(media->result.bdre.info.play_list.ref_menu_thmb_idx == 0xFFFF) ? "invalid" : "valid");
					break;

				case BDRECmd_get_pi_info:
					sprintf(media->queryResult,
						"%cLast query: PlayItem Info\n"
						"%d Clip Mark(s) in Clip: %ld \n"
						"%d Clip Mark(s) in Bridge Clip: %ld \n",
						CHAR_CODE_SECTION,
						media->result.bdre.info.play_item.number_of_Clip_marks,
						media->result.bdre.info.play_item.clip_id,
						media->result.bdre.info.play_item.number_of_BridgeClip_marks,
						//(media->result.bdre.info.play_item.number_of_BridgeClip_marks != -1) ? media->result.bdre.info.play_item.bridge_clip_id : -1);
						(media->result.bdre.info.play_item.number_of_BridgeClip_marks != 0) ? media->result.bdre.info.play_item.bridge_clip_id : 0);
					break;
					


				case BDRECmd_get_pl_mark_info:
					sprintf(media->queryResult,
							"%cLast query: PlayList Mark Info\n"
							"mark_invalid_flag: %d (%s mark) \n"
							"Mark Type: 0x%02x \n"
							"ref_to_PlayItemID: %d \n"
							"mark_time_stamp: %d \n"
							"entry_ES_PID: 0x%4x \n"
							"Ref to thumbnail index: %d \n"
							"Duration: %d \n"
							"Mark name: %s \n",
							CHAR_CODE_SECTION,
							media->result.bdre.info.pl_mark_info.mark.mark_invalid_flag,
							(media->result.bdre.info.pl_mark_info.mark.mark_invalid_flag == 0) ? "valid" : "invalid",
							media->result.bdre.info.pl_mark_info.mark.mark_type,
							media->result.bdre.info.pl_mark_info.mark.ref_to_PlayItem_id,
							media->result.bdre.info.pl_mark_info.mark.mark_time_stamp,
							media->result.bdre.info.pl_mark_info.mark.entry_ES_PID,
							(media->result.bdre.info.pl_mark_info.mark.mark_type < 3) ?
							media->result.bdre.info.pl_mark_info.mark.ref_to_menu_thumbnail_index :
							media->result.bdre.info.pl_mark_info.mark.ref_to_mark_thumbnail_index,
							media->result.bdre.info.pl_mark_info.mark.duration,
							media->result.bdre.info.pl_mark_info.mark.mark_name);
					break;

				case BDRECmd_get_clip_mark_info:
					sprintf(media->queryResult,
							"%cLast query: Clip Mark Info\n"
							"mark_invalid_flag: %d (%s mark) \n"
							"Mark Type: 0x%02x \n"
							"ref_to_STC_id: %d \n"
							"mark_time_stamp: %d \n"
							"entry_ES_PID: 0x%4x \n"
							"Ref to mark thumbnail index: %d \n"
							"Representative picture time stamp: %d \n"
							"Duration: %d \n"
							"Makers information: %d\n",
							CHAR_CODE_SECTION,
							media->result.bdre.info.clip_mark_info.mark_invalid_flag,
							(media->result.bdre.info.clip_mark_info.mark_invalid_flag == 0) ? "valid" : "invalid",
							media->result.bdre.info.clip_mark_info.mark_type,
							media->result.bdre.info.clip_mark_info.ref_to_STC_id,
							media->result.bdre.info.clip_mark_info.mark_time_stamp,
							media->result.bdre.info.clip_mark_info.entry_ES_PID,
							media->result.bdre.info.clip_mark_info.ref_to_mark_thumbnail_index,
							media->result.bdre.info.clip_mark_info.representative_picture_time_stamp,
							media->result.bdre.info.clip_mark_info.duration,
							media->result.bdre.info.clip_mark_info.makers_information);
					break;

				case BDRECmd_get_thumbnail:
					sprintf(media->queryResult,
							"%cLast query: Thumbnail picture\n"
							"Thumbnail type: %s \n"
							"Horizontal picture size: %d \n"
							"Vertical picture size: %d \n"
							"Picture byte size: %d\n ",
							CHAR_CODE_SECTION,
							(media->result.bdre.info.thumbnail.thmbn_type & MENU_THMBN_FLAG) ? "Menu" : 
							((media->result.bdre.info.thumbnail.thmbn_type & PLST_MARK_THMBN_FLAG) ? "PlayList mark" : "Clip mark"),
							media->result.bdre.info.thumbnail.horizontal_picture_size,
							media->result.bdre.info.thumbnail.vertical_picture_size,
							media->result.bdre.info.thumbnail.picture_byte_size);
					break;

				default:
					break;
			}
			break;
#endif
        default:
            break;
    }

}

/******************************************************************************
******************************************************************************/
void SetVolumeNoMSP(RMuint8 volume)
{
	isDolbyChannel = FALSE;
	struct AudioEngine_Volume_type vol;
	RMuint32 i;
	RMuint32 temp = VOLUME_0DB;

    //D_INFO("+++++++++++++++++++++ volume %d \n", volume);
	RMuint32 idx = 0 ;
	idx =((volume + 1)* MAX_VOLUME_TABLE)/MAX_UI_VOLUME;
    if(idx == MAX_VOLUME_TABLE)
		idx = MAX_VOLUME_TABLE - 1;
	temp = VolumeTable[idx];
	m_uiVolume = volume;
	m_isMute = false;
	//DMA_PRINT("\n******************\n\nSetVolumeNoMSP = %d ++++ index = %ld\n\n********************\n", volume, idx);

	//printf("\n\033[31m****************** Setting ... VolumeNoMSP = %d ********************\033[0m\n", volume);
	printf("\n\033[31m****************** Setting ... VolumeNoMSP[%ld] = %d(db) ********************\033[0m\n",idx, volume);

	for (i=0;i<=11;i++)
	{
		vol.Channel = i;
		vol.Volume = temp;
		RUASetProperty(ampGlobals->pRUA, AudioEngine,
			RMAudioEnginePropertyID_Volume, &vol, sizeof(vol), 0);
	}

	return;
}

void SetVolumeNoMSPForDolby(RMuint8 volume)
{
	isDolbyChannel = TRUE;
	struct AudioEngine_Volume_type vol;
	RMuint32 i;
	RMuint32 temp = VOLUME_0DB;

    //D_INFO("+++++++++++++++++++++ volume %d \n", volume);
	RMuint32 idx = 0 ;
	idx =((volume + 1)* MAX_VOLUME_TABLE_DOLBY)/MAX_UI_VOLUME;
    if(idx == MAX_VOLUME_TABLE_DOLBY)
		idx = MAX_VOLUME_TABLE_DOLBY - 1;
	temp = VolumeTable[idx];
	m_uiVolume = volume;
	//DMA_PRINT("\n******************\n\nSetVolumeNoMSP = %d ++++ index = %ld\n\n********************\n", volume, idx);

	//printf("\n\033[31m****************** Setting ... VolumeNoMSP = %d ********************\033[0m\n", volume);
	printf("\n\033[31m****************** Setting ... VolumeNoMSP[%ld] = %d(db) ********************\033[0m\n",idx, volume);

	for (i=0;i<=11;i++)
	{
		vol.Channel = i;
		vol.Volume = temp;
		RUASetProperty(ampGlobals->pRUA, AudioEngine,
			RMAudioEnginePropertyID_Volume, &vol, sizeof(vol), 0);
	}

	return;
}

void SetMuteNoMSP(void)
{
	struct AudioEngine_Volume_type vol;
    RMuint32 i;
    m_isMute = true;
    printf("****************** Setting ... SetMuteNoMSP ********************\n");
    for (i=0;i<=11;i++)
    {
        vol.Channel = i;
        vol.Volume = 0;
        RUASetProperty(ampGlobals->pRUA, AudioEngine,
        RMAudioEnginePropertyID_Volume, &vol, sizeof(vol), 0);
    }
}
int GetGlobalVolume(void) {
	if (m_uiVolume > 0) {
	return m_uiVolume;
}
	return 0;
}

bool GetGlobalMute(void) {
    return m_isMute;
}

bool DetectOutPutHDMI(void){
	struct SMRUAConfig configuration_tmp;
	memset(&configuration_tmp, 0, sizeof(configuration_tmp));

	configuration_tmp = ampGlobals->shared->configuration;
    
    if(configuration_tmp.output.video.common.hdmi.status == CS_Disconnected) {        
        GUI_INFO("=======Changed to HDMI======================\n");
        return true;
        
    }
    else{        
        //GUI_INFO("=======Changed to HDMI: CS_Connected======================\n");
        return false;
    }       
    
}

bool CheckPlugHDMICable(void){
    //it's true if cable hdmi have plugin/
    //while have changing from AV -> HDMI, it was false (cable HDMI have plugin)
    return check_hdmi_signal();
}

void SetOutputConfiguration(RMbool isOutput)
{
	if(pCrtVideoCfg)
	{
		//bool hdmi = ampGlobals->settings.video.common.hdmi.status != CS_Disconnected;
		//bool component = ampGlobals->settings.video.common.component.status != CS_Disconnected;
		//bool analog = ampGlobals->settings.video.common.composite.status != CS_Disconnected;
		if(isOutput)
		{
			pCrtVideoCfg->common.hdmi.status      = CS_Unknown ;
			pCrtVideoCfg->common.component.status = CS_Connected ;
			pCrtVideoCfg->common.composite.status = CS_Connected ;
		}
		else
		{
			pCrtVideoCfg->common.hdmi.status       = CS_Disconnected ;
			pCrtVideoCfg->common.component.status  = CS_Disconnected ;
			pCrtVideoCfg->common.composite.status  = CS_Disconnected ;
		}
		cfgamp->SetConfiguration(cfgamp, (struct SVideoOutConfig*)pCrtVideoCfg, NULL);
	}
}

void setCloseCaptionStatus( enum EToggle cc_status ) {

    if ( cc_status == TOGGLE_OFF ) {
		pCrtVideoCfg->common.hdmi.cc 		= cc_status;
		pCrtVideoCfg->common.component.cc 	= cc_status;
		pCrtVideoCfg->common.composite.cc 	= cc_status;
		pCrtVideoCfg->common.hdmi.display.ccType = ECC_Unknown;
        pCrtVideoCfg->common.hdmi.display.ccType = ECC_Unknown;
		pCrtVideoCfg->common.composite.display.ccType = ECC_Unknown;
	} else {
		if (pCrtVideoCfg->common.hdmi.status != CS_Disconnected ||
			pCrtVideoCfg->common.component.status  != CS_Disconnected) {

			pCrtVideoCfg->common.hdmi.cc 		= cc_status;
			pCrtVideoCfg->common.hdmi.display.ccType = ECC_608;

			pCrtVideoCfg->common.component.cc 	= cc_status;
			pCrtVideoCfg->common.component.display.ccType = ECC_608;

			pCrtVideoCfg->common.composite.cc 	= TOGGLE_OFF;
		} else if (pCrtVideoCfg->common.composite.status == CS_Connected) {

			pCrtVideoCfg->common.hdmi.cc 		= TOGGLE_OFF;
			pCrtVideoCfg->common.hdmi.display.ccType = ECC_Unknown;
			pCrtVideoCfg->common.component.cc 	= TOGGLE_OFF;
			pCrtVideoCfg->common.component.display.ccType = ECC_Unknown;
            
			pCrtVideoCfg->common.composite.cc 	= cc_status;
			pCrtVideoCfg->common.composite.display.ccType = ECC_608;
		}
	}

    cfgamp->SetConfiguration(cfgamp, (struct SVideoOutConfig*)pCrtVideoCfg, NULL);
}

bool GetTurnOnStandByStatus(void){
    if(pCrtVideoCfg->common.hdmi.status == CS_Disconnected && 
       pCrtVideoCfg->common.component.status == CS_Disconnected &&
			pCrtVideoCfg->common.composite.status  == CS_Disconnected){
        return true;            
    }
    else
        return false;
}


int uiWidth, uiHeight;
/*IDirectFBSurface 		*slidesurface = NULL;
IDirectFBSurface 		*tempsurface  = NULL;
int uiPreMode = 0, uiCurMode=0, usleep_time=100;*/
typedef struct {
    double x;
    double y;
} XY;
XY Bezier(XY *p,int n,double mu);
XY Bezier(XY *p,int n,double mu)
{
    int k,kn,nn,nkn;
    double blend,muk,munk;
    XY b = {0.0,0.0};
    muk = 1;
    munk = pow(1-mu,(double)n);

    for (k=0;k<=n;k++)
    {
        nn = n;
        kn = k;
        nkn = n - k;
        blend = muk * munk;
        muk *= mu;
        munk /= (1-mu);

        while (nn >= 1) {
            blend *= nn;
            nn--;
            if (kn > 1) {
                blend /= (double)kn;
                kn--;
            }
            if (nkn > 1) {
                blend /= (double)nkn;
                nkn--;
            }
        }
        b.x += p[k].x * blend;
        b.y += p[k].y * blend;
    }
    return(b);
}
/***********************************************
************************************************/
/**
 * 
 * <b>FUNCTION: </b>
 *
 * handle_media_keys(int ampIndex, struct SMediaSpaceCmdDefinition p)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int ampIndex, struct SMediaSpaceCmdDefinition p
 * @return 
 *		 bool
*/
bool handle_media_keys(int ampIndex, struct SMediaSpaceCmdDefinition p)
{
	struct SMediaSession *media = NULL;
	DFBResult err;
	enum { ACTION_POST, ACTION_EXECUTE ,} 
	action = ACTION_EXECUTE;
	unsigned int i ;

	if (ampIndex < MAX_AMP_INSTANCES)
		media = &mediaTab[ampIndex];
	if (!media || !media->amp)
	{
		media->tempStatus = TSTS_APP_ERROR;
		return false;
	}
	media->tempStatus = TSTS_NORMAL;
	for (i=0; i<media->msRT->cmdCnt; i++)
	{
		/*if ( media->msRT->cmdList[i].cmd != p.cmd )
			continue;*/
		if ( media->msRT->cmdList[i].selKey != p.selKey)
		{	
			continue;	
		}
		else if( media->msRT->cmdList[i].selKeyType != p.selKeyType )
		{
			continue;
		}
		if ( media->msRT->cmdList[i].cmd != p.cmd )
		{
			continue;
		}
		// found the selected commnd
		media->msCmd = &(media->msRT->cmdList[i]);
		
		// reset the command data
		memset(&ampCommand, 0, sizeof(ampCommand));
		
		// record the command ID
		*(media->msRT->pCmd) = media->msCmd->cmd;
			
		if (media->msCmd->param1Ptr !=NULL)
			*media->msCmd->param1Ptr = p.defParam1 ;
		if (media->msCmd->param2Ptr !=NULL ) 
			*media->msCmd->param2Ptr = p.defParam2 ;
		if (p.timePtr != NULL)
		{
			if( media->msCmd->timePtr == NULL )
			{
				continue;	
			}
			memcpy(media->msCmd->timePtr, p.timePtr, sizeof(p.timePtr)) ;
		}
		break;
	}
#if 0
	GUI_INFO("--------- i = %d ---------------\n", i );
	switch (media->result.generic.mediaSpace)
	{
		case MEDIA_SPACE_LINEAR_MEDIA:
			GUI_INFO(" MEDIA_SPACE_LINEAR_MEDIA >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			action = ACTION_EXECUTE;
			break;

#ifndef NO_RED_SUPPORT
		case MEDIA_SPACE_CDDA:
			GUI_INFO(" MEDIA_SPACE_CDDA >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			break;
#ifndef NO_VCD_SUPPORT	
		case MEDIA_SPACE_VCD:
			break;
#endif // VCD_SUPPORT
		case MEDIA_SPACE_DVD:
			GUI_INFO(" MEDIA_SPACE_DVD >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			break;

		case MEDIA_SPACE_DVD_VR:
			GUI_INFO(" MEDIA_SPACE_DVD_VR >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			break;

#ifndef NO_BLUE_SUPPORT
		case MEDIA_SPACE_BDJ:
			GUI_INFO(" MEDIA_SPACE_BDJ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			break;
		case MEDIA_SPACE_BLU_RAY:
			GUI_INFO(" MEDIA_SPACE_BLU_RAY >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			break;

		case MEDIA_SPACE_BDRE:
			GUI_INFO(" MEDIA_SPACE_BDRE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			break;
#endif
#endif
		

		default:
			GUI_INFO("Unknown media space %d, abort!!!\n", media->result.generic.mediaSpace);
			media->tempStatus = TSTS_AMP_ERROR;
			return false;
	}
#endif
	if(i >= media->msRT->cmdCnt)
	{
		return false ;
	}
	// restore the command and result media space and structure size,
	// they might have been overwritten by another AMP instance
	ampCommand.generic.mediaSpace = media->status.generic.mediaSpace;
	media->result.generic.mediaSpace = media->status.generic.mediaSpace;
	switch (media->result.generic.mediaSpace)
	{
	case MEDIA_SPACE_LINEAR_MEDIA:
		ampCommand.generic.dataSize = sizeof(struct SLPBCommand);
		media->result.generic.dataSize = sizeof(struct SLPBResult);
		break;
		
#ifndef NO_RED_SUPPORT
	case MEDIA_SPACE_CDDA:
		ampCommand.generic.dataSize = sizeof(struct SCDDACommand);
		media->result.generic.dataSize = sizeof(struct SCDDAResult);
		break;
#ifndef NO_VCD_SUPPORT	
	case MEDIA_SPACE_VCD:
		ampCommand.generic.dataSize = sizeof(struct SVCDCommand);
		media->result.generic.dataSize = sizeof(struct SVCDResult);
		break;
#endif // NO_VCD_SUPPORT
	case MEDIA_SPACE_DVD:
		ampCommand.generic.dataSize = sizeof(struct SDVDCommand);
		media->result.generic.dataSize = sizeof(struct SDVDResult);
		break;
		
	case MEDIA_SPACE_DVD_VR:
		ampCommand.generic.dataSize = sizeof(struct SVRCommand);
		media->result.generic.dataSize = sizeof(struct SVRResult);
		break;
#endif
#ifndef NO_BLUE_SUPPORT
	case MEDIA_SPACE_BDJ:
	case MEDIA_SPACE_BLU_RAY:
		ampCommand.generic.dataSize = sizeof(struct SBRDCommand);
		media->result.generic.dataSize = sizeof(struct SBRDResult);
		break;
		
	case MEDIA_SPACE_BDRE:
		ampCommand.generic.dataSize = sizeof(struct SBDRECommand);
		media->result.generic.dataSize = sizeof(struct SBDREResult);
		break;
#endif
#ifndef NO_DTV_SUPPORT
    case MEDIA_SPACE_DTV:
        ampCommand.generic.dataSize = sizeof(struct SDTVCommand);
        media->result.generic.dataSize = sizeof(struct SDTVResult);
        break;

    case MEDIA_SPACE_CAP:
        ampCommand.generic.dataSize = sizeof(struct SCAPCommand);
        media->result.generic.dataSize = sizeof(struct SCAPResult);
        break;
#endif  // DTV_SUPPORT
	default:
		D_ERROR("Unknown media space %d, abort!!!\n", media->result.generic.mediaSpace);
		media->tempStatus = TSTS_AMP_ERROR;
		return false;
	}
	// generic handling
	switch (*(media->msRT->pCmd))
	{
		case Cmd_WakeUpOnPTS:
		{
			char text[128];

			if (media->status.generic.flags & SSTATUS_MEDIA_TIME)
			{
				media->targetPTS = *media->status.generic.mediaTime + 80000; // schedule (almost) one second later event

				sprintf(text, "PTS notify: crt time = %lld, target time = %lld\n", *media->status.generic.mediaTime, media->targetPTS);
				err = media->amp->PostPresentationTimeNotificationRequest(media->amp, media->targetPTS);
				if (err != DFB_OK)
					sprintf(text, "PTS notify error: %d\n", err);
			}
			else
				sprintf(text, "PTS notify fails: media time not currently valid\n");
			//display_status(STSPART_LEFT, text);
			return true;
		}
			
		case Cmd_Adjust:
			switch (adjustment.type)
			{
				case ADJ_MUTE:
					// nothing to do
					D_INFO("Setting mute %d\n", adjustment.value.mute);
					break;

				case ADJ_VOLUME:
				case ADJ_MIXER_VOLUME_MAIN:
				case ADJ_MIXER_VOLUME_SECONDARY:
					{
						unsigned int vol = (int)adjustment.value.volume;
						
						if (vol > 32) vol = 32;
						
						if (vol == 0)
							adjustment.value.volume = 0;
						else
							adjustment.value.volume	= 1 << (vol-1);
						//D_INFO("Setting volume %#x\n", adjustment.value.volume);
						break;
					}

				case ADJ_MIXER_PAN_MAIN:
				case ADJ_MIXER_PAN_SECONDARY:
				{
					// ProcessMediaKeys has stored the pan values as integers, must convert them to the floats they are
                                        union
					{
						float f;
						int i;
					}
					temp;

					temp.f = adjustment.value.pan.x;
					adjustment.value.pan.x = (float)temp.i;

					temp.f = adjustment.value.pan.y;
					adjustment.value.pan.y = (float)temp.i;
					
					if (adjustment.value.pan.x < 0)	adjustment.value.pan.x = 0;
					else if (adjustment.value.pan.x > 30) adjustment.value.pan.x = 30;
					if (adjustment.value.pan.y < 0)	adjustment.value.pan.y = 0;
					else if (adjustment.value.pan.y > 30) adjustment.value.pan.y = 30;
					
					adjustment.value.pan.x = (adjustment.value.pan.x-15)/15;
					adjustment.value.pan.y = (adjustment.value.pan.y-15)/15;
					D_INFO("Setting pan %f x %f\n", adjustment.value.pan.x, adjustment.value.pan.y);
					break;
				}
				case ADJ_MIXER_MODE:
					// nothing to do
					D_INFO("Setting adjustment mode to %s\n", adjustment.value.autoAdjust ? "ON" : "OFF");
					break;
			}

			ampCommand.generic.control.adjustment = &adjustment;
			// perform normal command processing
			break;

		case Cmd_SetCallbacks:
			if (ampCommand.generic.control.callback.configPreview)
			{
				D_INFO("Enable Application Callbacks\n");
				showcfginfo = true;
			}
			else
			{
				D_INFO("Disable Application Callbacks\n");
				showcfginfo = false;
			}
			return true;

		case Cmd_SetConfig:
			if (newDRMConfigIndex != drmConfigIndex)
			{
				DFBAdvancedMediaProviderDescription desc = { 0};

				media->amp->GetDescription(media->amp, &desc);
				//struct SGlobals *ampGlobals = (struct SGlobals *)desc.privateInfo;

				drmConfigIndex = newDRMConfigIndex;
				if (drmConfigIndex >= sizeof(drmConfigTab)/sizeof(drmConfigTab[0]))
					drmConfigIndex = 0;
				pCrtDRMCfg = &drmConfigTab[drmConfigIndex];

				printf("Setting DRM configuration index %d\n", drmConfigIndex);
				print_drm_settings(pCrtDRMCfg);

				struct SGlobals *ampGlobals = (struct SGlobals *)desc.privateInfo;
				ampGlobals->shared->restrictions = *pCrtDRMCfg;

				media->amp->SetConfiguration(media->amp, NULL, NULL);
			}
			if (newVideoConfigIndex != videoConfigIndex)
			{
				videoConfigIndex = newVideoConfigIndex;
				if (videoConfigIndex >= sizeof(videoConfigTab)/sizeof(videoConfigTab[0]))
					videoConfigIndex = 0;
				pCrtVideoCfg = &videoConfigTab[videoConfigIndex];

				printf("Setting video configuration index %d\n", videoConfigIndex);
				print_video_settings(pCrtVideoCfg);
				media->amp->SetConfiguration(media->amp, (struct SVideoOutConfig *)pCrtVideoCfg, NULL);
			}
			if (newAudioConfigIndex != audioConfigIndex)
			{
				audioConfigIndex = newAudioConfigIndex;
				if (audioConfigIndex >= sizeof(audioConfigTab)/sizeof(audioConfigTab[0]))
					audioConfigIndex = 0;
				pCrtAudioCfg = &audioConfigTab[audioConfigIndex];

				printf("Setting audio configuration index %d\n", audioConfigIndex);
				print_audio_settings(pCrtAudioCfg);
				media->amp->SetConfiguration(media->amp, NULL, (struct SAudioOutConfig *)pCrtAudioCfg);
			}
			return true;

		case Cmd_disk_ejecting:
		case Cmd_disk_inserted:
			action = ACTION_EXECUTE;
			break;

		// bogus code do avoid compiler warning on drmConfigTab not being referenced
		case -5555:
			media->amp->SetConfiguration(media->amp, (struct SVideoOutConfig *)&videoConfigTab[videoConfigIndex], NULL);
			break;

		default:
			break;
	}
	// format-specific command values, generic handling
	if (0
#ifndef NO_RED_SUPPORT
		|| ((media->msRT->mediaSpace == MEDIA_SPACE_DVD) && (*(media->msRT->pCmd) == (int) DVDCmd_Query_Bookmark))
		|| ((media->msRT->mediaSpace == MEDIA_SPACE_DVD_VR) && (*(media->msRT->pCmd) == (int) VRCmd_Query_Bookmark))
#endif
#ifndef NO_BLUE_SUPPORT
		|| ((media->msRT->mediaSpace == MEDIA_SPACE_BLU_RAY) && (*(media->msRT->pCmd) == (int) BRDCmd_query_bookmark))
		|| ((media->msRT->mediaSpace == MEDIA_SPACE_BDRE) && (*(media->msRT->pCmd) == (int) BDRECmd_query_bookmark))
		|| ((media->msRT->mediaSpace == MEDIA_SPACE_BDJ) && (*(media->msRT->pCmd) == (int) BRDCmd_query_bookmark))
#endif
	   )
	{
		int cmd = *media->msRT->pCmd;

		if (media->bookmarkBuf)
		{
			free(media->bookmarkBuf);
			media->bookmarkBuf = NULL;
		}

		// get the query's response size
		*media->msRT->pCmd = media->msRT->bookmarkSizeQueryCmd;
		err = media->amp->ExecutePresentationCmd(media->amp, &ampCommand.generic, &media->result.generic);

		if (err != DFB_OK || IS_ERROR(media->result.generic.value))
		{
			D_INFO("\tExecute error: %d\n", err);

			if (err == DFB_BUSY)
				media->tempStatus = TSTS_BUSY;
			else
				media->tempStatus = TSTS_AMP_ERROR;
			return false;
		}

		// alloc the bookmark
		media->bookmarkSize = *(int *)((void *)&media->result.generic + media->msRT->bookmarkSizeOffset);
		media->bookmarkBuf = malloc(media->bookmarkSize);

		if (!media->bookmarkBuf)
		{
			D_ERROR("\tCould not allocate %d bytes for bookmark\n", media->bookmarkSize);
			media->tempStatus = TSTS_AMP_ERROR;
			media->status.generic.lastCmdResult = TRESULT_ERROR | RESULT_NO_MEM;
			return false;
		}

		// prepare the query
		*media->msRT->pCmd = cmd;
		*media->msRT->bookmarkBufPtr = media->bookmarkBuf;
		action = ACTION_EXECUTE;
	}
	else if (0
#ifndef NO_RED_SUPPORT
			 || ((media->msRT->mediaSpace == MEDIA_SPACE_DVD) && (*(media->msRT->pCmd) == (int) DVDCmd_Resume_Bookmark))
			 || ((media->msRT->mediaSpace == MEDIA_SPACE_DVD_VR) && (*(media->msRT->pCmd) == (int) VRCmd_Resume_Bookmark))
#endif
#ifndef NO_BLUE_SUPPORT
			 || ((media->msRT->mediaSpace == MEDIA_SPACE_BLU_RAY) && (*(media->msRT->pCmd) == (int) BRDCmd_resume_bookmark))
			 || ((media->msRT->mediaSpace == MEDIA_SPACE_BDRE) && (*(media->msRT->pCmd) == (int) BDRECmd_resume_bookmark))
			 || ((media->msRT->mediaSpace == MEDIA_SPACE_BDJ) && (*(media->msRT->pCmd) == (int) BRDCmd_resume_bookmark))
#endif
			)
	{
		if (!media->bookmarkBuf)
		{
			D_ERROR("\tInvalid bookmark error\n");
			media->tempStatus = TSTS_USER_ERROR;
			media->status.generic.lastCmdResult = TRESULT_ERROR | RESULT_INVALID;
			return false;
		}

		*media->msRT->bookmarkBufPtr = media->bookmarkBuf;

#ifndef NO_BLUE_SUPPORT
		// special handling of BDRE because the PlayLists may be protected by a PIN and we need synchronous execution
		if (media->msRT->mediaSpace == MEDIA_SPACE_BDRE)
		{
            if (ampCommand.bdre.param1.selector > 9999)
                memset(ampCommand.bdre.param1.PIN, 0xFF, 4 * sizeof(char));
            else
            {
                char pin[4];
                sprintf(pin, "%04d", ampCommand.bdre.param1.selector);
                memcpy(ampCommand.bdre.param1.PIN, pin, 4 * sizeof(char));
            }

			action = ACTION_EXECUTE;
		}
#endif
	}

	// format-aware handling
	switch (media->msRT->mediaSpace)
	{
		case MEDIA_SPACE_LINEAR_MEDIA:
			if (*(media->msRT->pCmd) == (int) LPBCmd_GET_STREAM_INFO)
				action = ACTION_EXECUTE;

#ifdef MLB_SUPPORT
			if (*(media->msRT->pCmd) == (int) LPBCmd_PLAY_TIME){
				action = ACTION_POST;
			}
#endif // #ifdef MLB_SUPPORT

			break;

#ifndef NO_RED_SUPPORT
		case MEDIA_SPACE_DVD:
			if ((*(media->msRT->pCmd) == (int) DVDCmd_QueryTitle) ||
				(*(media->msRT->pCmd) == (int) DVDCmd_QueryTXTDT) ||
				(*(media->msRT->pCmd) == (int) DVDCmd_QuerySPRMs) ||
				(*(media->msRT->pCmd) == (int) DVDCmd_QueryGPRMs))
				action = ACTION_EXECUTE;
			else if (*(media->msRT->pCmd) == (int) DVDCmd_Video_Mode_Change)
			{
				action = ACTION_EXECUTE;
			}
			if((*(media->msRT->pCmd)) == (int)DVDCmd_Play && isPostCmd)
			{
					action = ACTION_POST;
					isPostCmd = FALSE;
			}
			break;
		case MEDIA_SPACE_DVD_VR:
			switch (*(media->msRT->pCmd))
			{
				case VRCmd_Goto_Marker:
					switch (media->msCmd->defParam1)
					{
						case 1:
							// disc marker
							ampCommand.vr.param1.marker = media->status.vr.disc.representativePicture;
							break;

						case 2:
							// PG marker
							ampCommand.vr.param1.marker = media->result.vr.info.pg.picture;
							break;

						case 3:
							// PL marker
							ampCommand.vr.param1.marker = media->result.vr.info.pl.picture;
							break;

						default:
							APP_ASSERT(false);
							break;
					}
					break;

				case VRCmd_Query_PG:
				case VRCmd_Query_PL:
				case VRCmd_Query_PG_Cell:
				case VRCmd_Query_PL_Cell:
				case VRCmd_Query_PG_EP_Name:
				case VRCmd_Query_PL_EP_Name:
				case VRCmd_Query_Info:
					action = ACTION_EXECUTE;
					break;

				default:
					break;
			}
			break;
#endif
#ifndef NO_BLUE_SUPPORT
		case MEDIA_SPACE_BDJ:
		case MEDIA_SPACE_BLU_RAY:
			switch (*(media->msRT->pCmd))
			{
				case BRDCmd_get_psr:
				case BRDCmd_set_psr:
				case BRDCmd_get_gpr:
				case BRDCmd_set_gpr:
				case BRDCmd_get_title_info:
				case BRDCmd_get_pl_id:
				case BRDCmd_get_pl_info:
				case BRDCmd_get_pi_info:
				case BRDCmd_get_mark_info:
				case BRDCmd_get_chapter_info:
				case BRDCmd_get_video_stream_info:
				case BRDCmd_get_audio_stream_info:
				case BRDCmd_get_pg_ts_stream_info:
				case BRDCmd_get_ig_stream_info:
				case BRDCmd_get_sec_video_stream_info:
				case BRDCmd_get_sec_audio_stream_info:
				case BRDCmd_get_PiP_PG_textST_stream_info:
				case BRDCmd_get_meta_data_value:
					action = ACTION_EXECUTE;
					break;

				case BRDCmd_stop_on_title_end:
					if (ampCommand.brd.param3.endAction == PIEA_Stop)
					{
						if (media->status.generic.flags & SBRDSTATUS_TITLE)
						{
							//char text[32];
							media->repeatTitle = media->status.brd.crtTitle.title_number;
							media->repeatIssued = false;
							//sprintf(text, "REPEAT TITLE #%d", media->repeatTitle);	
						}
					}
					else
					{
						media->repeatTitle = 0;
					
					}
					break;

				case BRDCmd_stop_on_chapter_end:
					if (ampCommand.brd.param3.endAction == PIEA_Stop)
					{
						if (media->status.generic.flags & SBRDSTATUS_CHAPTER)
						{
							//char text[32]
							media->repeatChapter = media->status.brd.crtChapter.chapter_number;
							media->repeatIssued = false;
							//sprintf(text, "REPEAT CHAPTER #%d", media->repeatChapter);	
						}
					}
					else
					{
						media->repeatChapter = 0;
						
					}
					break;

				case BRDCmd_stop:
					// reset the repeat function
					media->repeatTitle = 0;
					media->repeatChapter = 0;
					//action = ACTION_POST;
					break;
				case BRDCmd_still_off:
					action = ACTION_POST;
				default:
					break;
			}
			break;

		case MEDIA_SPACE_BDRE:	
			switch (*(media->msRT->pCmd))
			{
				case BDRECmd_play:
				case BDRECmd_forward_play:
					action = ACTION_EXECUTE;
					break;

				case BDRECmd_play_pl_time:
					action = ACTION_EXECUTE;
					// small trick to make the users life easier
					ampCommand.bdre.param3.playListID = media->status.bdre.bdavDir.PlayListID[ampCommand.bdre.param3.playListID];

					if(media->msCmd->timePtr)
						ampCommand.bdre.param2.time = 45000 * ((3600 * media->msCmd->timePtr->Hour) + (60 * media->msCmd->timePtr->Minute) + media->msCmd->timePtr->Second);
					else
						ampCommand.bdre.param2.selector = 0;

					if (ampCommand.bdre.param1.selector > 9999)
						memset(ampCommand.bdre.param1.PIN, 0xFF, 4 * sizeof(char));
					else
					{
						char pin[4];
						printf("\nampCommand.bdre.param1.selector = %d\n\n\n\n", ampCommand.bdre.param1.selector);
						ampCommand.bdre.param1.selector = media->msCmd->defParam2;
						sprintf(pin, "%04d", ampCommand.bdre.param1.selector);
						memcpy(ampCommand.bdre.param1.PIN, pin, 4 * sizeof(char));
					}

					break;

				case BDRECmd_play_pl_mark:
					action = ACTION_EXECUTE;
					// small trick to make the users life easier
					ampCommand.bdre.param3.playListID = media->status.bdre.bdavDir.PlayListID[ampCommand.bdre.param3.playListID];

					if (ampCommand.bdre.param1.selector > 9999)
						memset(ampCommand.bdre.param1.PIN, 0xFF, 4 * sizeof(char));
					else
					{
						char pin[4];
						sprintf(pin, "%04d", ampCommand.bdre.param1.selector);
						memcpy(ampCommand.bdre.param1.PIN, pin, 4 * sizeof(char));
					}

					break;

				case BDRECmd_audio_change:
					if (ampCommand.bdre.param1.selector <= media->status.bdre.crtProgSeq.num_of_streams_in_ps)
						ampCommand.bdre.param1.selector = media->status.bdre.crtProgSeq.stream[ampCommand.bdre.param1.selector].stream_PID;
					else
						ampCommand.bdre.param1.selector	= 0x1FFF;

					break;

				case BDRECmd_view_change:
					if (ampCommand.bdre.param1.selector <= media->status.bdre.crtProgSeq.num_of_streams_in_ps)
						ampCommand.bdre.param1.selector = media->status.bdre.crtProgSeq.stream[ampCommand.bdre.param1.selector].stream_PID;
					else
						ampCommand.bdre.param1.selector	= 0x1FFF;

					break;

				case BDRECmd_get_pl_info:
					action = ACTION_EXECUTE;
					// small trick to make the users life easier
					ampCommand.bdre.param3.playListID = media->status.bdre.bdavDir.PlayListID[ampCommand.bdre.param3.playListID];
					// this is needed in case the BDAV directory is not protected but the selected PlayList is
					//if(media->openParam)
					//	strncpy(ampCommand.bdre.param1.PIN, media->openParam, 4);

					if (ampCommand.bdre.param1.selector > 9999)
						memset(ampCommand.bdre.param1.PIN, 0xFF, 4 * sizeof(char));
					else
					{
						char pin[4];
						sprintf(pin, "%04d", ampCommand.bdre.param1.selector);
						memcpy(ampCommand.bdre.param1.PIN, pin, 4 * sizeof(char));
					}
					break;

				case BDRECmd_get_pi_info:
					action = ACTION_EXECUTE;
					// small trick to make the users life easier
					ampCommand.bdre.param3.playListID = media->status.bdre.bdavDir.PlayListID[ampCommand.bdre.param3.playListID];
					break;

				case BDRECmd_get_pl_mark_info:
					action = ACTION_EXECUTE;
					// small trick to make the users life easier
					ampCommand.bdre.param3.playListID = media->status.bdre.bdavDir.PlayListID[ampCommand.bdre.param3.playListID];
					break;

				case BDRECmd_get_clip_mark_info:
					action = ACTION_EXECUTE;

					// small trick to make the users life easier
					ampCommand.bdre.param3.playListID = media->status.bdre.bdavDir.PlayListID[ampCommand.bdre.param3.playListID];
					break;

				case BDRECmd_get_thumbnail:
					action = ACTION_EXECUTE;

					// small trick to make the users life easier
					ampCommand.bdre.param3.playListID = media->status.bdre.bdavDir.PlayListID[ampCommand.bdre.param3.playListID];

					if (ampCommand.bdre.param2.selector == 0)
						ampCommand.bdre.param1.selector |= MENU_THMBN_FLAG;
					else if (ampCommand.bdre.param2.selector == 1)
						ampCommand.bdre.param1.selector |= PLST_MARK_THMBN_FLAG;
					else if (ampCommand.bdre.param2.selector == 2)
						ampCommand.bdre.param1.selector |= CLIP_MARK_THMBN_FLAG;
					else
						ampCommand.bdre.param1.selector	&= 0x1FFFFFFF;	// clear the bits, i.e. error

					// user supplied buffer
					ampCommand.bdre.param2.bookmarkBuf = (char*) tn_buff;
					break;

				default:
					break;
			}
			break;
#endif
#ifndef NO_DTV_SUPPORT			
#if 0
		case MEDIA_SPACE_DTV:
			switch (*(media->msRT->pCmd))
			{
			case DTVCmd_PLAY:
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_STOP:
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_PAUSE_ON:
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_PAUSE_OFF:
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_SET_SECTCBF:
				memset(&ampCommand.dtv.param1.SecDesc,0,sizeof(ampCommand.dtv.param1.SecDesc));
				if((DTVPARSER_STATUS_PATNPMT & checkInfoCompleteStatus()) == 0)
				{
				    ampCommand.dtv.param1.SecDesc.Pid = 0x00;
				    ampCommand.dtv.param1.SecDesc.Mask[0] = 0xFF;
				    ampCommand.dtv.param1.SecDesc.Mode[0] = 0xFF;
				    ampCommand.dtv.param1.SecDesc.Comp[0] = 0x00;
				    ampCommand.dtv.param1.SecDesc.Cipher = 0;
				
				    ampCommand.dtv.param2.fSecCallback = DTVCallbackFunc;
				    action = ACTION_EXECUTE;
				    
                    
                    printf("Assign PAT\n");
				}
				else if((DTVPARSER_STATUS_PATNPMT & checkInfoCompleteStatus()) == DTVPARSER_STATUS_PAT)
                {
                    ampCommand.dtv.param1.SecDesc.Pid = getCurrentPMTPid();
                    ampCommand.dtv.param1.SecDesc.Mask[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Mode[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Comp[0] = 0x02;
                    ampCommand.dtv.param1.SecDesc.Cipher = 0;
                
                    ampCommand.dtv.param2.fSecCallback = DTVCallbackFunc;
                    action = ACTION_EXECUTE;
                    
                    //display_status(STSPART_LEFT, "Start Monitoring PMT");
                    printf("Assign PMT pid = 0x%x\n",ampCommand.dtv.param1.SecDesc.Pid);
                }
				else if((DTVPARSER_STATUS_PATNPMT & checkInfoCompleteStatus()) == DTVPARSER_STATUS_PATNPMT)
				{
                    printf("Remove PAT monitoring\n");
                    ampCommand.dtv.param1.SecDesc.Pid = 0x00;
                    ampCommand.dtv.param1.SecDesc.Mask[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Mode[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Comp[0] = 0x00;
                    ampCommand.dtv.param1.SecDesc.Cipher = 0;
                
                    ampCommand.dtv.param2.fSecCallback = NULL;
                    action = ACTION_EXECUTE;
                    
                    //display_status(STSPART_LEFT, "Stop Monitoring PAT");
	                resetPATInfo();
				}
				else if((DTVPARSER_STATUS_PATNPMT & checkInfoCompleteStatus()) == DTVPARSER_STATUS_PMT)
				{
                    printf("Remove PMT monitoring Pid = 0x%x\n",getCurrentPMTPid());
                    ampCommand.dtv.param1.SecDesc.Pid = getCurrentPMTPid();
                    ampCommand.dtv.param1.SecDesc.Mask[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Mode[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Comp[0] = 0x02;
                    ampCommand.dtv.param1.SecDesc.Cipher = 0;
                
                    ampCommand.dtv.param2.fSecCallback = NULL;
                    action = ACTION_EXECUTE;
                    
                    //display_status(STSPART_LEFT, "Stop Monitoring PMT");
                    resetPMTInfo();
				}
				else
				{
                    printf("Unexpected case for DTVCmd_SET_SECTCBF\n");
                    ampCommand.dtv.param1.SecDesc.Pid = getCurrentPMTPid();
                    ampCommand.dtv.param1.SecDesc.Mask[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Mode[0] = 0xFF;
                    ampCommand.dtv.param1.SecDesc.Comp[0] = 0x02;
                    ampCommand.dtv.param1.SecDesc.Cipher = 0;
                
                    ampCommand.dtv.param2.fSecCallback = NULL;
                    action = ACTION_EXECUTE;
                    
                    resetPATInfo();
                    resetPMTInfo();
				}
				break;
			case DTVCmd_SET_DSMCCCBF:
			{
				int PidNum = (int)ampCommand.dtv.param1.Input;
				memset(&ampCommand.dtv.param1.SecDesc,0,sizeof(ampCommand.dtv.param1.SecDesc));
				ampCommand.dtv.param1.SecDesc.Pid = PidNum;
				ampCommand.dtv.param1.SecDesc.Mask[0] = 0x00;
				ampCommand.dtv.param1.SecDesc.Mode[0] = 0x00;
				ampCommand.dtv.param1.SecDesc.Comp[0] = 0x00;
				ampCommand.dtv.param1.SecDesc.Cipher = 0;
				
				ampCommand.dtv.param2.fSecCallback = DTVCallbackFunc;
				action = ACTION_EXECUTE;
			}
				break;
			case DTVCmd_SET_ECMKEYS:
#if 0				
				ampCommand.dtv.param1.Pid = EmcPid; // Assign EcmPid
				ampCommand.dtv.param3.CipherType = CipherType;
				
				switch(CipherType)
				{
				case dvbcsa_decryption:
					ampCommand.dtv.param2.CipherKey.DVBCSAKey.key_index = 0;
					ampCommand.dtv.param2.CipherKey.DVBCSAKey.key = pKeyCode;
					break;
				case multi2_decryption:
					ampCommand.dtv.param2.CipherKey.Multi2Key.key_index = 0;		// System will find from Pid table
					ampCommand.dtv.param2.CipherKey.Multi2Key.system_key = pSystem;
					ampCommand.dtv.param2.CipherKey.Multi2Key.iv = pIV;
					ampCommand.dtv.param2.CipherKey.Multi2Key.data_key = pKeyCode;
					break;
				case aes_cbc_decryption:
				case aes_ecb_decryption:
				case aes_ofb_decryption:
				case aes_ctr_decryption:
				case aes_cfb_decryption:
				case aes_nsa_decryption:
					ampCommand.dtv.param2.CipherKey.AesKey.key_index = 0;
					ampCommand.dtv.param2.CipherKey.AesKey.key_size = (*(int *)pSystem);
					ampCommand.dtv.param2.CipherKey.AesKey.key = pKeyCode;
					ampCommand.dtv.param2.CipherKey.AesKey.iv = pIV;
					break;
				default:
					printf("UNSUPPORTED TYPE\n")
				}
#endif				
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_START_FILTS:
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_STOP_FILTS:
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_SET_PESCBF:
			{
				int PidNum = (int)ampCommand.dtv.param1.Input;
				memset(&ampCommand.dtv.param1.PesDesc,0,sizeof(ampCommand.dtv.param1.PesDesc));
				ampCommand.dtv.param1.PesDesc.Pid = PidNum;
				ampCommand.dtv.param1.PesDesc.Type = 0;  
				ampCommand.dtv.param1.PesDesc.Cipher = pid_filter_section;  // 0
				ampCommand.dtv.param1.PesDesc.EcmPid = 0x1FFF;

				ampCommand.dtv.param2.fSecCallback = DTVCallbackFunc;
				action = ACTION_EXECUTE;
			}
				break;
			case DTVCmd_SET_STCDRIFT:
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_GET_VCXO_ADJ:
				ampCommand.dtv.param1.Adj.Adjustment_N = (RMuint32)&AdjN; // Speed N
				ampCommand.dtv.param1.Adj.Adjustment_M = (RMuint32)&AdjM; // Speed M
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_SET_VCXO_ADJ:
				printf("Cmd : DTVCmd_SET_VCXO_ADJ 100 : %d \n",ampCommand.dtv.param2.Input);
				ampCommand.dtv.param1.Adj.Adjustment_N = 100; // Speed N
				ampCommand.dtv.param1.Adj.Adjustment_M = ampCommand.dtv.param2.Input; // Speed M
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_SET_DEMUX_INPUT:
			case DTVCmd_SET_DISPLAY:
				printf("Cmd : 0x%x \n",*(media->msRT->pCmd));
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_SET_AVSOURCE:
				// Underworld
			    if(!fillAVInfo(&ampCommand.dtv.param1.PesDesc.Pid,
			                   &ampCommand.dtv.param2.PesDesc.Pid,
			                   &ampCommand.dtv.param1.PesDesc.EcmPid,
			                   &ampCommand.dtv.param2.PesDesc.EcmPid,
			                   (unsigned *)&ampCommand.dtv.param1.PesDesc.Type,
			                   (unsigned *)&ampCommand.dtv.param2.PesDesc.Type,
			                   (unsigned short *)&ampCommand.dtv.param3.Pid))
			    {
			        ampCommand.dtv.param1.PesDesc.Pid = 0x11;
			        ampCommand.dtv.param1.PesDesc.Type = MPEG2TS_MPEG2v;
			        ampCommand.dtv.param1.PesDesc.Cipher = 0;
			        ampCommand.dtv.param1.PesDesc.EcmPid = 0;
				
			        ampCommand.dtv.param2.PesDesc.Pid = 0x14;
			        ampCommand.dtv.param2.PesDesc.Type = MPEG2TS_Dolby_AC3;
			        ampCommand.dtv.param2.PesDesc.Cipher = 0;
			        ampCommand.dtv.param2.PesDesc.EcmPid = 0;
				
			        ampCommand.dtv.param3.Pid = 0x11;
			    }
				action = ACTION_EXECUTE;
				break;
			case DTVCmd_SET_PARTIALTSCBF:
			{
			    int i,j,count;
			    unsigned short Pids[MAX_TSPidArray_Size],temp;
			    
                for(i =0;i<MAX_TSPidArray_Size;i++)
                {
                    Pids[i] = 0x1FFF;
                    ampCommand.dtv.param1.TsPids.PidArray[i] = 0x1FFF;
                }
                
                Pids[0] = 0x0;                     // PAT
                Pids[1] = getCurrentPMTPid();      // PMT
                if(fillAVInfo(&Pids[2],
                               &Pids[3],
                               &Pids[4],
                               &Pids[5],
                               (unsigned *)NULL,
                               (unsigned *)NULL,
                               (unsigned short *)&Pids[6]))
                {
                    for(i=0;i<MAX_TSPidArray_Size;i++)
                    {
                        for(j=i+1;j<MAX_TSPidArray_Size;j++)
                        {
                            if(Pids[i] != 0x1FFF)
                            {
                                if(Pids[i] > Pids[j])
                                {
                                    temp = Pids[i];
                                    Pids[i] = Pids[j];
                                    Pids[j] = temp;
                                }
                            }
                        }
                    }

                    ampCommand.dtv.param1.TsPids.PidArray[0] = Pids[0];
                    for(i=1,count =0;i<MAX_TSPidArray_Size;i++)
                    {
                        if((Pids[i] != 0x1FFF) && (ampCommand.dtv.param1.TsPids.PidArray[count] != Pids[i]))
                        {
                            ampCommand.dtv.param1.TsPids.PidArray[++count] = Pids[i];
                        }
                        
                    }
                    
                    ampCommand.dtv.param1.TsPids.Size = count+1;
                    
                    printf("DTVCmd_SET_PARTIALTSCBF :  Size = %d\n",ampCommand.dtv.param1.TsPids.Size);
                    for(i=0;i<ampCommand.dtv.param1.TsPids.Size;i++)
                    {
                        printf("Pid  = 0x%x",ampCommand.dtv.param1.TsPids.PidArray[i]);
                    }
                    printf("\n");
                    
                    memset(&ampCommand.dtv.param2.PesDesc,0,sizeof(ampCommand.dtv.param2.PesDesc));
                    ampCommand.dtv.param2.PesDesc.Pid = ESP_PID_PTS;
                    ampCommand.dtv.param2.PesDesc.Type = 0;
                    ampCommand.dtv.param2.PesDesc.Cipher= 0;
                    ampCommand.dtv.param2.PesDesc.EcmPid = 0x1FFF;
                    ampCommand.dtv.param3.fPTSCallback = DTVRecordCallbackFunc;
                    action = ACTION_EXECUTE;
                }
                break;
			}
			case DTVCmd_SET_AUDIO_OUTMODE:
			case DTVCmd_SET_AUDIO_OUTMODE_OTF:
			case DTVCmd_SET_DOWNMIXTABLE: 
			case DTVCmd_SET_ENABLE_DOWNMIXTBL:
			case DTVCmd_SET_COLORSPACE:
				printf("Cmd : 0x%x \n",*(media->msRT->pCmd));
				action = ACTION_EXECUTE;
				break;
			default:
				printf("default : 0x%x \n",*(media->msRT->pCmd));
				break;
				
			}
			break;
#endif
#endif // DTV_SUPPORT
		default:
			break;
	}

	// reset the query result
	media->queryResult[0] = 0;

	if (action == ACTION_POST)
	{
		D_INFO("Post AMP command\n");
		err = media->amp->PostPresentationCmd(media->amp, &ampCommand.generic);
		if (err != DFB_OK)
		{
			if (err == DFB_BUSY)
			{
				D_ERROR("\tAMP BUSY!!!\n");
				media->tempStatus = TSTS_BUSY;
			}
			else
			{
				D_ERROR("\tPost error: %d\n", err);
				media->tempStatus = TSTS_AMP_ERROR;
			}
		}
	}
	else if (action == ACTION_EXECUTE)
	{
		D_INFO("Execute AMP command\n");
		err = media->amp->ExecutePresentationCmd(media->amp, &ampCommand.generic, &media->result.generic);
		if (err != DFB_OK || IS_ERROR(media->result.generic.value))
		{
			if (err == DFB_BUSY)
			{
				D_ERROR("\tAMP BUSY!!!\n");
				media->tempStatus = TSTS_BUSY;
			}
			else
			{
				//D_ERROR("\tExecute error %d (%d)\n", err, media->result.generic.value);
				media->tempStatus = TSTS_AMP_ERROR;
			}
		}
		else
		{
#ifndef NO_RED_SUPPORT
			if ((media->msRT->mediaSpace == MEDIA_SPACE_DVD) && (*(media->msRT->pCmd) == (int) DVDCmd_Video_Mode_Change))
			{
				// force the new video mode on the output
				enum ETVAspectRatio ar = TVAR_Unknown;
				enum EAspectRatioMatch arm = ARM_Auto;

				switch (ampCommand.dvd.param1.videoMode)
				{
					case RM_DVD_VIDEO_4_3__4_3_RECT: // direct (not supported, force "letterbox")
						ar = TVAR_4x3;
						arm = ARM_LetterBox;
						break;
					case RM_DVD_VIDEO_4_3__16_9_ZOOM_ON: // crop (not supported, force "bands")
						ar = TVAR_16x9;
						arm = ARM_SidePanel;//ARM_Crop;
						break;
					case RM_DVD_VIDEO_4_3__16_9_ZOOM_OFF: // bands
						ar = TVAR_16x9;
						arm = ARM_SidePanel;
						break;
					case RM_DVD_VIDEO_16_9__16_9_RECT: // wide
						ar = TVAR_16x9;
						arm = ARM_No;
						break;
					case RM_DVD_VIDEO_16_9__4_3_PANSCAN:
						ar = TVAR_4x3;
						arm = ARM_PanScan;
						break;
					case RM_DVD_VIDEO_16_9__4_3_LETTERBOX:
						ar = TVAR_4x3;
						arm = ARM_LetterBox;
						break;
				}

				{
					struct SMRUAVideoOutConfig vCfg;
					memcpy(&vCfg, pCrtVideoCfg, pCrtVideoCfg->common.size);
					vCfg.common.hdmi.display.aspectRatio = ar;
					vCfg.common.hdmi.arMatch = arm;
					vCfg.common.component.display.aspectRatio = ar;
					vCfg.common.component.arMatch = arm;
					vCfg.common.composite.display.aspectRatio = ar;
					vCfg.common.composite.arMatch = arm;
					D_INFO("Change user preferences: AR=0x%08x, ARM=0x%08x\n", ar, arm);
					media->amp->SetConfiguration(media->amp, (struct SVideoOutConfig *) &vCfg, NULL);
				}
			}
#endif
#ifndef NO_DTV_SUPPORT			
			if ((media->msRT->mediaSpace == MEDIA_SPACE_DTV) && (*(media->msRT->pCmd) == (int) DTVCmd_GET_VCXO_ADJ))
			{
				//D_INFO("VCXO current speed N = 0x%lx, M = 0x%lx\n",AdjN,AdjM);
			}
#endif //DTV_SUPPORT 		
			processMediaQuery(media);
		}
	}
	else
		RMASSERT(false);

	if (media->tempStatus != TSTS_NORMAL)
	{
		return FALSE ;
	}
	struct SStatus oldStatus;
	// save the old status
	oldStatus = media->status.generic;
	media->amp->UploadStatusChanges(media->amp, &media->status.generic, (DFBBoolean)false);
	//processMediaQuery(media);

	if( IS_SUCCESS(media->status.generic.lastCmdResult) )
	{
		GUI_INFO("\n>>>> TRESULT_OK ???????????????????????????\n");
		if(isShowLoading == WAIT_TO_HIDE)
			isShowLoading = HIDE_LOADING; // Tri command here
	}
	else
	{
		GUI_INFO("\n>>>> RESULT_INVALID >>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	}
	return IS_SUCCESS(media->status.generic.lastCmdResult) ? true : false ;
	
 }
/**
 * 
 * <b>FUNCTION: </b>
 *
 * process_media_events(int ampIndex , union UMSStatus **pUMSStatus ,
 *     union UMSResult **pUMSResult)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *       int ampIndex , union UMSStatus **pUMSStatus , union UMSResult **pUMSResult
 * @return 
 *		 bool
*/
//void process_media_events(int ampIndex , union UMSStatus **pUMSStatus ,
//						union UMSResult **pUMSResult)

void process_media_events(int ampIndex)
{
 // DFBResult err;
	struct SStatus oldStatus;
	struct SMediaSession *media = NULL;
 
	if (ampIndex < MAX_AMP_INSTANCES)
		media = &mediaTab[ampIndex];

	 if (!media || !media->amp)
	 {
		return;
	 }
	 // save the old status
	 oldStatus = media->status.generic;
    // upload the changes
    media->amp->UploadStatusChanges(media->amp, &media->status.generic, (DFBBoolean)false);
	if (media->status.generic.flags & SSTATUS_MEDIA_TIME)
	{
		if (media->targetPTS && (*media->status.generic.mediaTime >= media->targetPTS))
		{
			char text[128];
			UINT64 mediaTime = *media->status.generic.mediaTime;
			
			sprintf(text, "Target PTS %lld reached with %ld ms delay", media->targetPTS, (long)((mediaTime - media->targetPTS)/45));
			media->targetPTS = 0;
			
			
		}
	}
#ifndef NO_BLUE_SUPPORT
	if (media->status.generic.mediaSpace == MEDIA_SPACE_BDJ )
	{
		if ((media->status.generic.flags & SLPBSTATUS_MODE) && (media->status.generic.mode.flags & SSTATUS_MODE_TRANSITION) &&
			(!(oldStatus.flags & SLPBSTATUS_MODE) || !(oldStatus.mode.flags & SSTATUS_MODE_TRANSITION)))
		{
			//display_status(STSPART_RIGHT, "Loading BDJ...");
			if(media->status.generic.mode.speed == 1024 ) // // BDApp is show message info and avoid FF, Rev , and SlowFF 
			{
				isShowLoading = SHOW_LOADING ;
			}
#if 0
			if(isShowWarning == WAIT_TO_HIDE_WARNING)
			{
				isShowWarning = HIDE_WARNING;
			}
#endif	
		}
		else if ((oldStatus.flags & SLPBSTATUS_MODE) &&	(oldStatus.mode.flags & SSTATUS_MODE_TRANSITION) &&
			(!(media->status.generic.flags & SLPBSTATUS_MODE) || !(media->status.generic.mode.flags & SSTATUS_MODE_TRANSITION)))
		{
			//display_status(STSPART_RIGHT, NULL);
			if(isShowLoading == WAIT_TO_HIDE)
			{
				isShowLoading = HIDE_LOADING;
			}
#if 0
			if(isShowWarning == WAIT_TO_HIDE_WARNING)
			{
				isShowWarning = HIDE_WARNING;
			}
#endif
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////
		if (media->status.generic.changes & SSTATUS_EXCEPTION)
		{
			if (media->status.generic.flags & SSTATUS_EXCEPTION)
			{
				if (media->status.generic.exception.fatal)
				{
					if (media->status.generic.exception.type == RESULT_PROTECTED)
						isShowWarning = PLAYBACK_CANCELLED_DUE_TO_SECURITY_VIOLATION ;
					else
						isShowWarning = PLAYBACK_CANCELLED_DUE_TO_INTERNAL_ERROR ;
				}
				else if (media->status.generic.exception.delay)
				{
					if (media->status.generic.exception.type == RESULT_IO)
						isShowWarning = PLAYBACK_DELAYED_DUE_TO_IO_ERROR ;
					else
						isShowWarning = PLAYBACK_DELAYED_DUE_TO_INTERNAL_ERROR;
				}
				else
				{
					if (media->status.generic.exception.type == RESULT_IO)
						isShowWarning = PLAYBACK_CHANGED_BY_IO_ERROR;
					else
						isShowWarning = PLAYBACK_CHANGED_BY_INTERNAL_ERROR;
				}
			}
			else
			{
				if(isShowLoading == WAIT_TO_HIDE)
				{
					isShowLoading = HIDE_LOADING;
				}
				if(isShowWarning == WAIT_TO_HIDE_WARNING)
				{
					isShowWarning = HIDE_WARNING;
				}
			}
			
		}
#if 0
		/////////////////////////////////////////////////////////////////////////////////////////////////
		if (media->status.brd.common.changes & SBRDSTATUS_TITLE)
		{
			printf("process_media_events    Entering BD-J title, move GUI to Secondary OSD  \n ");
			printf("media->status.brd.common.changes   SBRDSTATUS_TITLE \n");
			if ((media->status.brd.common.flags & SBRDSTATUS_TITLE) && (media->status.brd.crtTitle.objectType == ObjectType_BDJ))
			{
				printf("process_media_events    Entering BD-J title, move GUI to Secondary OSD \n ");
				if(gui_on_secosd == FALSE )
				{
					isShowGui = SEC_OSD_ENABLE;
					//isShowLoading = HIDE_LOADING ;
				}
			}
			else
			{	
				printf("process_media_events Entering HDMV title, move GUI to Primary OSD  >>>\n");
				if(gui_on_secosd == TRUE )
				{
					// restore the Primary OSD's aspect ratio mode as it was before we forced it to Auto
					printf("process_media_events Entering HDMV title, move GUI to Primary OSD !!!!!!!!!!!!!!!!!!!!!!!! >>>\n");
					isShowGui = OSD_ENABLE;
					//isShowLoading = HIDE_LOADING ;
				}
			}
		}
#endif
	}
#endif
	
		m_upMyAppStatus = &media->status ; 
		m_upMyAppResult = &media->result;
	
} 
/**
 * 
 * <b>FUNCTION: </b>
 *
 * handle_zoom(RMuint8 index)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *		int index
 * @return 
 *      RMstatus
*/
#define FZOOM_DW	32
#define FZOOM_DH	18 //minimum delta to avoid "green" line and keep AR 
#define FZOOM_DX	(FZOOM_DW>>1)
#define FZOOM_DY	(FZOOM_DH>>1)
RMstatus handle_zoom(RMuint8 index)
{

	if (!amp_table[ampIndex].layer)
	{
		printf("************************* layer null ******************************\n");
		return RM_ERROR;
	}
	else
	{
		DFBRectangle rect = amp_table[ampIndex].src_area;
		DFBRectangle lrect;
		int dx, dy, dw, dh;
		DFBDisplayLayerConfig lcfg;
	
		lrect.x = 0;
		lrect.y = 0;
		if (amp_table[ampIndex].layer->GetConfiguration(amp_table[ampIndex].layer, &lcfg) == DFB_OK)
		{
			lrect.w = lcfg.width;
			lrect.h = lcfg.height;
		}
		else
		{
			lrect.w = xres;
			lrect.h = yres;
		}
        m_layer_rect.w = lrect.w;
		m_layer_rect.h = lrect.h;
		D_INFO("Current ZOOM: %d x %d  @  %d, %d  in  %d x %d  layer\n", rect.w, rect.h, rect.x, rect.y, lrect.w, lrect.h);

		dx = dy = dw = dh = 0;

		// Application in video output mode selection modal dialog
		switch (index)
		{
			case ZOOM_MOVE_LEFT: //left
				dx = -lrect.w/16; 
				break;
	
			case ZOOM_MOVE_RIGHT: //right
				dx = lrect.w/16; 
				break;
	
			case ZOOM_MOVE_DOWN: //down
				dy = lrect.h/16;
				break;
	
			case ZOOM_MOVE_UP: //up
				dy = -lrect.h/16;
				break;
	
			case ZOOM_OUT://Prev
				dx = -rect.w/4; 
				dy = -rect.h/4;
				dw = rect.w/2;
				dh = rect.h/2;
				break;
	
			case ZOOM_IN://next
				dx = rect.w/4; 
				dy = rect.h/4;
				dw = -rect.w/2;
				dh = -rect.h/2;
				break;
	
			case ZOOM_OFF://power
				//display_status(STSPART_LEFT, "ZOOM reset");
				rect = lrect;
				set_active_amp(ampIndex, true, false);
				goto _zoom_off;
						
			case 7://zoom off
				//display_status(STSPART_LEFT, "ZOOM save");
				_zoom_off:
				//display_status(STSPART_RIGHT, NULL);
				break;

			default:
				// reject selection but remain in mode
				//display_status(STSPART_LEFT, "Invalid ZOOM selection");
				return RM_ERROR;
				break;
		}
		if(index != ZOOM_OFF)
		{
			rect.x += dx; 
			rect.y += dy; 
			rect.w += dw; 
			rect.h += dh; 
		}
		else
		{
			rect = m_src_rect; 
		}
        m_zoom_rect = rect;
		if (rect.x < 0 || rect.w < lrect.w/16 || rect.x + rect.w > lrect.w ||
			rect.y < 0 || rect.h < lrect.h/16 || rect.y + rect.h > lrect.h)
				return RM_ERROR;
		else
		{
			if (amp_table[ampIndex].layer->SetSourceRectangle(amp_table[ampIndex].layer, rect.x, rect.y, rect.w, rect.h) == DFB_OK)
			{
				amp_table[ampIndex].src_area = rect;
				return RM_OK;
			}
			else
				return RM_ERROR;
		
		}
	}
	return RM_ERROR;
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * handle_zoom_fine(RMuint8 index)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *		int index
 * @return 
 *      RMstatus
*/

RMstatus handle_zoom_fine(RMuint8 index)
{

		if (!amp_table[ampIndex].layer)
	{
		printf("************************* layer null ******************************\n");
		return RM_ERROR;
	}
	else
	{
		DFBRectangle rect = amp_table[ampIndex].src_area;
		DFBRectangle lrect;
		int dx, dy, dw, dh;
		DFBDisplayLayerConfig lcfg;
	
		lrect.x = 0;
		lrect.y = 0;
		if (amp_table[ampIndex].layer->GetConfiguration(amp_table[ampIndex].layer, &lcfg) == DFB_OK)
		{
			lrect.w = lcfg.width;
			lrect.h = lcfg.height;
		}
		else
		{
			lrect.w = xres;
			lrect.h = yres;
		}
        m_layer_rect.w = lrect.w;
		m_layer_rect.h = lrect.h;
		D_INFO("Current ZOOM: %d x %d  @  %d, %d  in  %d x %d  layer\n", rect.w, rect.h, rect.x, rect.y, lrect.w, lrect.h);

		dx = dy = dw = dh = 0;

		// Application in video output mode selection modal dialog
		switch (index)
		{
			case ZOOM_MOVE_LEFT: //left
				dx = -FZOOM_DX;
				break;
	
			case ZOOM_MOVE_RIGHT: //right
				dx = FZOOM_DX;
				break;
	
			case ZOOM_MOVE_DOWN: //down
				dy = FZOOM_DY;
				break;
	
			case ZOOM_MOVE_UP: //up
				dy = -FZOOM_DY;
				break;
	
			case ZOOM_OUT://Prev
				dx = -FZOOM_DX;
				dy = -FZOOM_DY;
				dw = FZOOM_DW;
				dh = FZOOM_DH;
				break;
	
			case ZOOM_IN://next
				dx = FZOOM_DX;
				dy = FZOOM_DY;
				dw = -FZOOM_DW;
				dh = -FZOOM_DH;
				break;
	
			case ZOOM_OFF://power
				//display_status(STSPART_LEFT, "ZOOM reset");
				rect = lrect;
				set_active_amp(ampIndex, true, false);
				goto _zoom_off;
						
			case 7://zoom off
				//display_status(STSPART_LEFT, "ZOOM save");
				_zoom_off:
				//display_status(STSPART_RIGHT, NULL);
				break;

			default:
				// reject selection but remain in mode
				//display_status(STSPART_LEFT, "Invalid ZOOM selection");
				return RM_ERROR;
				break;
		}
		if(index != ZOOM_OFF)
		{
			rect.x += dx; 
			rect.y += dy; 
			rect.w += dw; 
			rect.h += dh; 
		}
		else
		{
			rect = m_src_rect; 
		}
        m_zoom_rect = rect;
		if (rect.x < 0 || rect.w < lrect.w/16 || rect.x + rect.w > lrect.w ||
			rect.y < 0 || rect.h < lrect.h/16 || rect.y + rect.h > lrect.h)
				return RM_ERROR;
		else
		{
			if (amp_table[ampIndex].layer->SetSourceRectangle(amp_table[ampIndex].layer, rect.x, rect.y, rect.w, rect.h) == DFB_OK)
			{
				amp_table[ampIndex].src_area = rect;
				return RM_OK;
			}
			else
				return RM_ERROR;
		
		}
	}
	return RM_ERROR;
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * handle_color_adjustment(RMuint32 Brightness, RMuint32 Contrast, RMuint32 Hue, RMuint32 Saturation)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *		RMuint32 Brightness, RMuint32 Contrast, RMuint32 Hue, RMuint32 Saturation
 * @return 
 *      void
*/
void handle_color_adjustment(RMuint32 Brightness, RMuint32 Contrast, RMuint32 Hue, RMuint32 Saturation)
{
	DFBResult err;	
	DFBColorAdjustment col_adj_mv = {0, };
	Brightness	*= 0x28F ;
	Contrast	*= 0x28F ;
	Hue			*= 0x28F ;
	Saturation	*= 0x28F ;	
	if( !layer_mv )
	{
		return;
	}
	
	if (layer_mv)
		DFBCHECK(layer_mv->GetColorAdjustment(layer_mv, &col_adj_mv));
	if (col_adj_mv.brightness != Brightness)
    {
        col_adj_mv.flags |= DCAF_BRIGHTNESS ;
        col_adj_mv.brightness =  Brightness ;
    }
	
    if (col_adj_mv.contrast != Contrast)
    {
        col_adj_mv.flags |= DCAF_CONTRAST ;
        col_adj_mv.contrast =  Contrast ;
    }
	
    if (col_adj_mv.hue != Hue)
    {
        col_adj_mv.flags |= DCAF_HUE ;
        col_adj_mv.hue    =  Hue ;
    }
	
    if (col_adj_mv.saturation != Saturation)
    {
        col_adj_mv.flags |= DCAF_SATURATION ;
        col_adj_mv.saturation =  Saturation ;
    }
	
	if (layer_mv)
		DFBCHECK(layer_mv->SetColorAdjustment(layer_mv, &col_adj_mv));
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * draw_image_or_text(IDirectFBSurface *surf, char *fileName)
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *		IDirectFBSurface *surf, char *fileName
 * @return 
 *      void
*/
void draw_image_or_text(IDirectFBSurface *surf, char *fileName)
{
	DFBResult err;
	DFBSurfaceDescription sdesc;
	IDirectFBImageProvider *provider = NULL;
	unsigned int i;
	DFBSurfacePixelFormat pf;
	
	DFBCHECK(surf->GetPixelFormat(surf, &pf));
	surf->SetDrawingFlags(surf, DSDRAW_NOFX);
	surf->SetBlittingFlags(surf, DSBLIT_NOFX);
	DirectFBPixelFormatNames(pixelFormatNames);
	for (i=0; i<sizeof(pixelFormatNames)/sizeof(pixelFormatNames[0]); i++)
		if (pixelFormatNames[i].format == pf)
		{
			D_ERROR("\ndraw_image_or_text: Destination pixel format: %s\n\n", pixelFormatNames[i].name);
			break;
		}
		printf("fileName = %s\n",fileName);
		if (fileName && (err = gdfb->CreateImageProvider( gdfb, fileName, &provider )) != DFB_OK)
		{
			D_ERROR("draw_image_or_text: IGNORE: Could not retrieve image %s (error code %x)\n", fileName, err);
			return ;
		}
		if ( provider )
		{
			DFBCHECK(provider->GetSurfaceDescription( provider, &sdesc ));
			for (i=0; i<sizeof(pixelFormatNames)/sizeof(pixelFormatNames[0]); i++)
			{
				if (pixelFormatNames[i].format == sdesc.pixelformat)
				{
					D_ERROR("\ndraw_image_or_text: Image provider pixel format: %s\n\n", pixelFormatNames[i].name);
					break;
				}
			}
			if ((sdesc.pixelformat == pf) || (pf != DSPF_I444) || (sdesc.pixelformat != DSPF_ARGB))
			{
				D_ERROR("\ndraw_image_or_text: Blitting image directly to layer\n\n");
				DFBCHECK(provider->RenderTo( provider, surf, NULL ));
			}
			if(0)
			{
				// destination has different pixel format from source and is not supported by DFB (it is Sigma specific) and the provider is not Sigma's
				// render to an offscreen surface and then blit that one to the BG layer (blits are accelerated)
				//IDirectFBSurface *render_surf;
				DFBRectangle rect;
				IDirectFBSurface*  offscreen_surf = NULL;
				DFBSurfaceDescription  offscreen_dsc;
				
				D_ERROR("\ndraw_image_or_text: Blitting image to layer through intermediate surface\n\n");
				
				rect.x = rect.y = 0;
				DFBCHECK(surf->GetSize(surf, &rect.w, &rect.h));
				//sdesc.width = rect.w ;
				//sdesc.height = rect.h ;			
				//DFBCHECK(gdfb->CreateSurface(gdfb, &sdesc, &render_surf));
				
				offscreen_dsc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT;
				offscreen_dsc.caps = (DFBSurfaceCapabilities)(sdesc.caps | DSCAPS_VIDEOONLY);
				offscreen_dsc.caps = (DFBSurfaceCapabilities)(sdesc.caps &~DSCAPS_SYSTEMONLY);
				offscreen_dsc.width = rect.w; 
				offscreen_dsc.height = rect.h;  
				offscreen_dsc.pixelformat = DSPF_ARGB ;// DSPF_ARGB4444;//DSPF_ARGB;  
				
				
				DFBCHECK(gdfb->CreateSurface(gdfb, &offscreen_dsc, &offscreen_surf));
				DFBCHECK(provider->RenderTo( provider, offscreen_surf, NULL ));
				//DFBCHECK(surf->Blit (surf, offscreen_surf, NULL ,0 , 0));
				DFBCHECK(surf->StretchBlit(surf, offscreen_surf, NULL, &rect));
				offscreen_surf->Release(offscreen_surf);
			}
			provider->Release( provider );
		}
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetNameImageBackground (char *name)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *		char *name
 * @return 
 *      void
*/
void set_name_image_back_ground (char *name)
{
	strcpy(fileNameImageBackGround , name);
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetNameImageBackground (char *name)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *		char *name
 * @return 
 *      RMbool
*/
RMbool IsPicturePlaying(void)
{
	if(sv) // only use ISO ISO Disc because display picture on Second Video 
	{
		GUI_INFO("--------- Picture is PLaying !!!!!!!!! -------------------------\n");
		return RM_OK;
	}
	GUI_INFO("--------- No Picture is PLaying !!!!!!!!! -------------------------\n");
	return RM_ERROR;
}
#if 0
/**
 * 
 * <b>FUNCTION: </b>
 *
 *		SetWndRect_Preview(RMbool bSet, RMbool bThumbnail)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *		RMbool bSet, RMbool bThumbnail
 * @return 
 *      void
*/
void SetWndRect_Preview(RMbool bSet, RMbool bThumbnail)
{
	isSetRecPreview = bSet;
	if(bSet)
	{
		if(bThumbnail){
			xpos = GUI_PREVIEW_THUMBNAIL_X;
			ypos = GUI_PREVIEW_THUMBNAIL_Y;
		}
		else{
			xpos = GUI_PREVIEW_X;
			ypos = GUI_PREVIEW_Y;
		}
		xres = GUI_PREVIEW_W;
		yres = GUI_PREVIEW_H;
	}
	else
	{
		xpos = 0;
		ypos = 0;
		xres = 1920;
		yres = 1080;
	}
}
#endif

/****************************************************************************************************
 *	
 *               PlayList Function
 *
 ****************************************************************************************************/
 /**
 * 
 * <b>FUNCTION: </b>
 *
 * InitPlaylist(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release second video layer
 * 
 * @param
 *		int ampIndex
 * @return 
 *      void
*/
void init_play_list(int ampIndex)
{

}
 /**
 * 
 * <b>FUNCTION: </b>
 *
 * InitPlaylist(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release second video layer
 * 
 * @param
 *		int ampIndex
 * @return 
 *      void
*/
void free_play_list(int ampIndex)
{

}
 /**
 * 
 * <b>FUNCTION: </b>
 *
 * InitPlaylist(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 * release second video layer
 * 
 * @param
 *		int ampIndex
 * @return 
 *      void
*/
int next_play_list_item(int ampIndex)
{
	 return 0;
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * SetPreview(int x, int y, int w, int h)
 * 
 * <b>DESCRIPTION: </b>
 * 
 * preview
 * 
 * @param
 * int x, int y, int w, int h
 * @return: None
*/
void set_preview(bool bSet, int x, int y, int w, int h)
{
    //amp_table[ampIndex].layer->SetScreenRectangle(amp_table[ampIndex].layer, x, y, w, h);
	isSetRecPreview = bSet;
	xpos = x;
	ypos = y;
	xres = w;
	yres = h;

	//picture preview
	x_preview = x;
	y_preview = y;
	h_preview = h;
	w_preview = w;
}

void set_screen_rec(int x, int y, int w, int h)
{
	isSetRecPreview = false;
    amp_table[ampIndex].layer->SetScreenRectangle(amp_table[ampIndex].layer, x, y, w, h);
}

void SetIndexSubInternal(RMuint16 uiIndex)
{
	uiStreamIndex = uiIndex;
}
/*********************SLIDESHOW*****************************/
#ifdef PLAY_PICTURE_HARDWARE
/**
* <b>FUNCTION: </b>
*
* LoadImage(char *filename, bool isusetempsurface)
*
* <b>DESCRIPTION: </b>
*
* @param
*   char *filename, bool isusetempsurface
* @return
*   DFB_OK if success. Otherwise DFB_FAILURE
*/
DFBResult LoadImage(char *filename, bool isusetempsurface){
	D_INFO("\n\033[34m LoadImage() \n\033[0m");
	DFBResult err = DFB_OK;
	IDirectFBImageProvider *provider = NULL;
	DFBSurfaceDescription sdesc;
	DFBRectangle rect;
	DFBRectangle DFBrect;

	err = gdfb->CreateImageProvider(gdfb, filename, &provider);
	if (err != DFB_OK){
		ERR_MESSAGE(ERR_CREATE_IMAGE_PROVIDER);
        return err;
	}

	provider->GetSurfaceDescription(provider, &sdesc);

	if ((sdesc.width > MAX_PICTURE_WIDTH) || (sdesc.height > MAX_PICTURE_HEIGHT)){		
		return !DFB_OK;
	}

    rect.x = rect.y = 0;
	sv->GetSize(sv, &rect.w, &rect.h);
	uiWidth  = rect.w;
	uiHeight = rect.h;
    sdesc.flags  = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT /*| DSDESC_PIXELFORMAT*/);
    sdesc.caps = DSCAPS_VIDEOONLY;

	if(isSetRecPreview){
		DFBrect.w = w_preview;
		DFBrect.h = h_preview;
		DFBrect.x = x_preview;
		DFBrect.y = y_preview;
	}
	else
	{
	if (rect.w * sdesc.height <= rect.h * sdesc.width){
		DFBrect.w = rect.w;
		DFBrect.h = DFBrect.w * sdesc.height / sdesc.width;
	}
	else{
		DFBrect.h = rect.h;
		DFBrect.w = DFBrect.h * sdesc.width / sdesc.height;
	}	

	DFBrect.x = rect.x + (rect.w - DFBrect.w)/2;
	DFBrect.y = rect.y + (rect.h - DFBrect.h)/2;
	}

	sdesc.width = rect.w;
	sdesc.height = rect.h;

	if (isusetempsurface == 0){	//load the first times
	
		err = gdfb->CreateSurface(gdfb, &sdesc, &slidesurface);
		if (err == DFB_OK){
			slidesurface->Clear(slidesurface, 0, 0, 0, 255);
			err = provider->RenderTo(provider, slidesurface, &DFBrect);
			if (err != DFB_OK){
				ERR_MESSAGE(ERR_RENDER_TO_SURFACE);
				return err;
			}
		}else{
			provider->Release(provider);
			ERR_MESSAGE(ERR_CREATE_SURFACE);
			return err;
		}
	}else{ //load to temp surface
		err = gdfb->CreateSurface(gdfb, &sdesc, &tempsurface);
		if (err == DFB_OK){
			tempsurface->Clear(tempsurface, 0, 0, 0, 255);
			err = provider->RenderTo(provider, tempsurface, &DFBrect);
			if (err != DFB_OK){
				ERR_MESSAGE(ERR_RENDER_TO_SURFACE);
				return err;
			}
		}
		else{
			provider->Release(provider);
			ERR_MESSAGE(ERR_CREATE_SURFACE);
            return err;
		}
	}

	provider->Release(provider);

    return DFB_OK;
}
/**
* <b>FUNCTION: </b>
*
* ClearImage()
*
* <b>DESCRIPTION: </b>
*
* @param	No
* @return   No
*/
void ClearImage(){
	D_INFO("\n\033[34m ClearImage() \n\033[0m");
	DFBResult err = DFB_OK;
	DFB_MESSAGE(sv->Flip( sv, NULL, (DFBSurfaceFlipFlags) 0 ), ERR_FLIP_SURFACE);
}
/**
* <b>FUNCTION: </b>
*
* CopyTempSurfacetoSlideSurface()
*
* <b>DESCRIPTION: </b>
*
* @param	No
* @return   No
*/
void CopyTempSurfacetoSlideSurface(void){
	D_INFO("\n\033[34m CopyTempSurfacetoSlideSurface() \n\033[0m");	
	slidesurface = tempsurface;
}
static void DrawSlideShow(RMint8 index);
/**
* <b>FUNCTION: </b>
*
* InitSlideShow(char *filename, RMint8 index)
*
* <b>DESCRIPTION: </b>
*
* @param
*   char *filename, RMint8 index
* @return
*   DFB_OK if success. Otherwise DFB_FAILURE
*/
DFBResult InitSlideShow(char *filename, RMint8 index){
	D_INFO("\n\033[34m InitSlideShow() \n\033[0m");
	DFBResult err = DFB_OK;      

	rect_SlideShow.x = 0;
	rect_SlideShow.y = 0;
	rect_SlideShow.w = GUI_ON_SLIDESHOW_WIDTH;
	rect_SlideShow.h = GUI_ON_SLIDESHOW_HEIGHT;
	enable_sv_layer(GUI_ON_SLIDESHOW_WIDTH, GUI_ON_SLIDESHOW_HEIGHT);
	DFB_MESSAGE(sv->Clear( sv, 0, 0, 0, 255), ERR_CLEAR_SURFACE);
	err = LoadImage(filename, 0);
	if (err == DFB_OK){
        DrawSlideShow(index);
	}
    return err;
}
/**
* <b>FUNCTION: </b>
*
* StartSlideShow(char *filename, RMint8 index)
*
* <b>DESCRIPTION: </b>
*
* @param
*   char *filename, RMint8 index
* @return
*   DFB_OK if success. Otherwise DFB_FAILURE
*/
DFBResult StartSlideShow(char *filename, RMint8 index, RMbool bIsZoomInSlideShow){
	DFBResult err = DFB_OK;
	DFBDisplayLayerConfig dlcfg;
	if (bIsZoomInSlideShow){
		rect_SlideShow.x = 0;
		rect_SlideShow.y = 0;
		rect_SlideShow.w = GUI_ON_SLIDESHOW_WIDTH;
		rect_SlideShow.h = GUI_ON_SLIDESHOW_HEIGHT;

		dlcfg.flags = (DFBDisplayLayerConfigFlags)(DLCONF_WIDTH | DLCONF_HEIGHT | DLCONF_SOURCE);
		dlcfg.width = GUI_ON_SLIDESHOW_WIDTH;
		dlcfg.height = GUI_ON_SLIDESHOW_HEIGHT;
		dlcfg.source = DLSID_SURFACE;
		DFBCHECK(layer_sv->SetConfiguration(layer_sv, &dlcfg));
	}
	if (slidesurface){
		D_INFO("\n\033[34m StartSlideShow():Release slidesurface \n\033[0m");		
		slidesurface->Release(slidesurface);
		slidesurface = NULL;
	}
      
	err = LoadImage(filename, 1);
	if (err == DFB_OK){
		ClearImage();
		CopyTempSurfacetoSlideSurface();
		DrawSlideShow(index);
	}
	return err;
}
/**
* <b>FUNCTION: </b>
*
* DeInitSlideShow()
*
* <b>DESCRIPTION: </b>
*
* @param No
* @return   No
*/
void DeInitSlideShow(void){
	if (slidesurface){
		slidesurface->Clear( slidesurface, 0, 0, 0, 0 );
		slidesurface->Flip( slidesurface, NULL, (DFBSurfaceFlipFlags) 0 );
		slidesurface->Release(slidesurface);
		slidesurface = NULL;
	}
	if (tempsurface){		
		tempsurface = NULL;
	}
	disable_sv_layer();
}
/**
* <b>FUNCTION: </b>
*
* DrawImageRandom_1Direction(int mode)
*
* <b>DESCRIPTION: </b>
*
* @param    int mode
* @return   No
*/
void DrawImageRandom_1Direction(int mode){
	DFBResult err = DFB_OK;
	DFBRectangle rect;
	int i;
	int x, y, w, h;
	int times;

	times = 60; // = USCLN(Width/Height)/2

	switch (mode){
	case SS_TOP:
		x = 0;
		y = 0;
		w = uiWidth;
		h = 0;
		break;
	case SS_BOTTOM:
		x = 0;
		y = uiHeight;
		w = uiWidth;
		h = 0;
		break;
	case SS_LEFT:
		x = 0;
		y = 0;
		w = 0;
		h = uiHeight;
		break;
	case SS_RIGHT:
		x = uiWidth;
		y = 0;
		w = 0;
		h = uiHeight;
		break;
	case SS_TOPLEFT:
		x = 0;
		y = 0;
		w = 0;
		h = 0;
		break;
	case SS_TOPRIGHT:
		x = uiWidth;
		y = 0;
		w = 0;
		h = 0;
		break;
	case SS_BOTTOMLEFT:
		x = 0;
		y = uiHeight;
		w = 0;
		h = 0;
		break;
	case SS_BOTTOMRIGHT:
		x = uiWidth;
		y = uiHeight;
		w = 0;
		h = 0;
		break;
	case SS_TOPCENTER:
		x = uiWidth/2;
		y = 0;
		w = 0;
		h = 0;
		break;
	case SS_BOTTOMCENTER:
		x = uiWidth/2;
		y = uiHeight;
		w = 0;
		h = 0;
		break;
	case SS_LEFTCENTER:
		x = 0;
		y = uiHeight/2;
		w = 0;
		h = 0;
		break;
	case SS_RIGHTCENTER:
		x = uiWidth;
		y = uiHeight/2;
		w = 0;
		h = 0;
		break;
	case SS_FOCUS:
		x = uiWidth/2;
		y = uiHeight/2;
		w = 0;
		h = 0;
		break;
	default:
		return;
		break;
	}

	for (i = 1; i <= times; i++){
		switch (mode)
		{
		case SS_TOP:
			h += uiHeight/times;
			break;
		case SS_BOTTOM:
			y -= uiHeight/times;
			h += uiHeight/times;
			break;
		case SS_LEFT:
			w += uiWidth/times;
			break;
		case SS_RIGHT:
			x -= uiWidth/times;
			w += uiWidth/times;
			break;
		case SS_TOPLEFT:
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_TOPRIGHT:
			x -= uiWidth/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_BOTTOMLEFT:
			y -= uiHeight/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_BOTTOMRIGHT:
			x -= uiWidth/times;
			y -= uiHeight/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_TOPCENTER:
			x -= (uiWidth/2)/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_BOTTOMCENTER:
			x -= (uiWidth/2)/times;
			y -= uiHeight/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_LEFTCENTER:
			y -= (uiHeight/2)/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_RIGHTCENTER:
			x -= uiWidth/times;
			y -= (uiHeight/2)/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		case SS_FOCUS:
			x -= (uiWidth/2)/times;
			y -= (uiHeight/2)/times;
			w += uiWidth/times;
			h += uiHeight/times;
			break;
		default:
			break;
		}

		rect.x	= x;
		rect.y	= y;
		rect.w	= w;
		rect.h	= h;

		DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect, x,y),ERR_PLIT_SURFACE);
		usleep(usleep_time);
	}
}
/**
* <b>FUNCTION: </b>
*
* DrawImage()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImage(){	
	printf("\n\033[31m>>>> Slide Show normal <<<<<<<<\033[0m");
	printf("\n\033[31m>>>> Slide Show normal <<<<<<<<\033[0m");
	printf("\n\033[31m>>>> Slide Show normal <<<<<<<<\033[0m\n");
	DFBResult err = DFB_OK;
	sv->Flip( sv, NULL, (DFBSurfaceFlipFlags) 0 );
	DFB_MESSAGE(sv->Blit(sv, slidesurface, NULL, 0,0),ERR_PLIT_SURFACE);
}
/**
* <b>FUNCTION: </b>
*
* DrawImageSSLeftToRight()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSLeftToRight(){
	DrawImageRandom_1Direction(SS_LEFT);
}
/**
* <b>FUNCTION: </b>
*
* DrawImageSSRightToLeft()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSRightToLeft(){
	DrawImageRandom_1Direction(SS_RIGHT);
}
/**
* <b>FUNCTION: </b>
*
* DrawImageUpToDown()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSTopToBottom(){
	DrawImageRandom_1Direction(SS_TOP);
}
/**
* <b>FUNCTION: </b>
*
* DrawImageDownToUp()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSBottomToTop(){
	DrawImageRandom_1Direction(SS_BOTTOM);
}
/**
* <b>FUNCTION: </b>
*
* DrawImageSSCrossfade()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSCrossfade(void){
	DFBResult err = DFB_OK;
	DFBRectangle rect;
	int i, times = 63;

	rect.x = 0;
	rect.y = 0;
	rect.w = uiWidth;
	rect.h = uiHeight;

	usleep(usleep_time * 5);
	for (i = 0; i < times; i++){
		DFBCHECK(layer_sv->SetOpacity(layer_sv, (0xff-i*4)));
		usleep(usleep_time);
	}

	DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect, 0,0),ERR_PLIT_SURFACE);
	for (i = 0; i < times; i++){
		DFBCHECK(layer_sv->SetOpacity(layer_sv, (i*4)));
		usleep(usleep_time * 1.5);
	}
	DFBCHECK(layer_sv->SetOpacity(layer_sv,  0xff));

}
/**
* <b>FUNCTION: </b>
*
* DrawImageSSWaterfall()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSWaterfall(void){
#define MAX_CTRL   4
#define BLOCK_WIDTH 16
	DFBResult err = DFB_OK;
	DFBRectangle rect1;
    XY sControlPoints[MAX_CTRL];
    int sign=rand()&1;
    int bar_cnt = ((uiWidth+BLOCK_WIDTH-1)/BLOCK_WIDTH);
    int *ypos;
    int i,j,h, min=0, max=0, times=60;
    ypos = malloc(bar_cnt*sizeof(int));

    for(i=0; i<MAX_CTRL; i++){
        sign ^= 1;
        sControlPoints[i].x = (double)(i*uiWidth/(MAX_CTRL-1));
        sControlPoints[i].y = (double)(((rand()&0x7))*((sign)?20:-20));
    }

    for(i=0; i<bar_cnt; i++){
        XY p1 = Bezier(sControlPoints, MAX_CTRL, (double)i/bar_cnt);
        ypos[i] = 10*(int)p1.y+(rand()%200)-100;
        if(ypos[i]<min) min=ypos[i];
        if(ypos[i]>max) max=ypos[i];
    }
    h = (uiHeight+(max-min))/times;

    for(i=0; i<bar_cnt; i++){
        ypos[i] -= max;
    }

    for(i=0; i<times; i++){
        rect1.x = 0;
        rect1.w = BLOCK_WIDTH;
        for(j=0; j<bar_cnt; j++, rect1.x+=BLOCK_WIDTH){
            rect1.y = ypos[j];
            if(rect1.y>=uiHeight){
                continue;
            }
            if(rect1.y>=0){
                if(j==bar_cnt-1) rect1.w = uiWidth-rect1.x;
                if(h+rect1.y>uiHeight)
                    rect1.h = uiHeight-rect1.y;
                else
                    rect1.h = h;
                DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect1, rect1.x, rect1.y),ERR_PLIT_SURFACE);
            }

            if(rect1.y <0 && rect1.y+h>0)
                ypos[j] = 0;
            else
                ypos[j] += h;
        }
        usleep(usleep_time);
    }
    free(ypos);
#undef BLOCK_WIDTH
#undef MAX_CTRL
}
/**
* <b>FUNCTION: </b>
*
* DrawImageSSSnake()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSSnake(void){
#define BLOCK_WIDTH 120
#define BLOCK_HEIGHT 80
	DFBRectangle rect1;
	DFBResult err = DFB_OK;
    int w,h,x,y,cx,cy,dx=0,dy=0,i,segment_len;

    struct SDIR {
        int xy[8];
    } dir, dir_ref[2] = {
        {{1,0, 0,1, -1,0, 0,-1}},
        {{0,1, 1,0, 0,-1, -1,0}}
    };
    struct SPOS {
        int x,y;
    } pos[4] = {
        {0,0}, {1,0}, {1,1}, {0,1}
    };

    int start_pos = rand()&0x03;
    int start_dir = rand()&0x01; //ccw or cw

    dir = dir_ref[start_dir];
    i = (start_dir)?4-start_pos:start_pos;

    cx = w = uiWidth/BLOCK_WIDTH;
    cy = h = uiHeight/BLOCK_HEIGHT;
    while(i--){
        x = dir.xy[0];
        y = dir.xy[1];
        memmove(&dir.xy[0], &dir.xy[2], sizeof(dir.xy[0])*6);
        dir.xy[6] = x;
        dir.xy[7] = y;
    }

    x = pos[start_pos].x*(w-1);
    y = pos[start_pos].y*(h-1);

    dx = dir.xy[0];
    dy = dir.xy[1];

    i=0;
    segment_len = 1;

    do {
        if(!--segment_len){
            x-=dx;
            y-=dy;
            dx = dir.xy[i++];
            dy = dir.xy[i++];
            if(dx){
                segment_len = w;
                h--;
            }
            else{
                segment_len = h;
                w--;
            }
            if(i>=8) i=0;
            x+=dx;
            y+=dy;
            //GUI_INFO("\nsegment_len=%d, x=%d, y=%d, dx=%d, dy=%d\n", segment_len, x, y, dx,dy);
        }
        usleep(0);
        rect1.x = x*BLOCK_WIDTH;
        rect1.y = y*BLOCK_HEIGHT;
        rect1.w = (x+1==cx)?(uiWidth-rect1.x):BLOCK_WIDTH;
        rect1.h = (y+1==cy)?(uiHeight-rect1.y):BLOCK_HEIGHT;
        if(rect1.x+rect1.w > uiWidth) break;
        if(rect1.y+rect1.h > uiHeight) break;

        //GUI_INFO("x=%d, y=%d\n", x, y);
		DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect1, rect1.x, rect1.y),ERR_PLIT_SURFACE);
        x+=dx;
        y+=dy;

    }while(segment_len>0);
#undef BLOCK_WIDTH
#undef BLOCK_HEIGHT
}
/**
* <b>FUNCTION: </b>
*
* DrawImageSSDissolve()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSDissolve(void){
    int times = 60;
    int i, xmax, ymax ;
	DFBRectangle rect1;
	DFBResult err = DFB_OK;
#define RECT_W 24
#define RECT_H 24

    xmax = uiWidth-RECT_W;
    ymax = uiHeight-RECT_H;

    rect1.w = RECT_W;
    rect1.h = RECT_H;

    while(times--){
        for(i=0;i<200;i++){
            rect1.x = rand()%xmax;
            rect1.y = rand()%ymax;

            DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect1, rect1.x, rect1.y),ERR_PLIT_SURFACE);
        }
        usleep(usleep_time);
    }
    rect1.x = 0;
    rect1.y = 0;
    rect1.w = uiWidth;
    rect1.h = uiHeight;
	DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect1, 0, 0),ERR_PLIT_SURFACE);
#undef RECT_W
#undef RECT_H
}
/**
* <b>FUNCTION: </b>
*
* DrawImageSSStripe()
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void DrawImageSSStripe(void){
    int max_div = ((rand()%53)+10)&0xfe;
    int horz = rand()&1;
    int times = 60;
    int i, j, cnt;
	DFBRectangle rect1, rect2;
	DFBResult err = DFB_OK;
    cnt = max_div/2;

    for (i=0; i<times; i++){
        if(horz){
            rect1.x = uiWidth-(i+1)*uiWidth/times;
            rect1.w = uiWidth-rect1.x;
            rect1.h = uiHeight/max_div;
            rect1.y = 0;

            rect2.w = (i+1)*uiWidth/times;
            rect2.h = rect1.h;
            rect2.x = rect2.y = 0;

            for(j=0; j<cnt; j++){
    		    DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect1, 0,rect1.y),ERR_PLIT_SURFACE);
                rect2.y = rect1.y+rect1.h;

                if(j==cnt-1) rect2.h = uiHeight-rect2.y;
	    	    DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect2, uiWidth-rect2.w,rect2.y),ERR_PLIT_SURFACE);
                rect1.y = rect2.y+rect2.h;
            }
        }else{
            rect1.y = uiHeight-(i+1)*uiHeight/times;
            rect1.h = uiHeight-rect1.y;
            rect1.w = uiWidth/max_div;
            rect1.x = 0;

            rect2.h = (i+1)*uiHeight/times;
            rect2.w = rect1.w;
            rect2.x = rect2.y = 0;

            for(j=0; j<cnt; j++){
    		    DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect1, rect1.x,0),ERR_PLIT_SURFACE);
                rect2.x = rect1.x+rect1.w;

                if(j==cnt-1) rect2.w = uiWidth - rect2.x;
	    	    DFB_MESSAGE(sv->Blit(sv, slidesurface, &rect2, rect2.x, uiHeight-rect2.h),ERR_PLIT_SURFACE);
                rect1.x = rect2.x+rect2.w;
            }
        }
        usleep(usleep_time);
    }
}
/**
* <b>FUNCTION: </b>
*
* AllEffectShuffle(void)
*
* <b>DESCRIPTION: </b>
*
* @param    No
* @return   No
*/
void AllEffectShuffle(void)
{

}
enum {
	SS_MODE_NORMAL,
	SS_MODE_CROSSFADE,
	SS_MODE_LEFT_RIGHT,
	SS_MODE_RIGHT_LEFT,
	SS_MODE_TOP_BOTTOM,
	SS_MODE_BOTTOM_TOP,
	SS_MODE_WATERFALL,
	SS_MODE_SNAKE,
	SS_MODE_DISSOLVE,
	SS_MODE_STRIPE,
	SS_MODE_SHUFFLE,
};
typedef struct {
    int nMode;
    void (*func) (void);
} sTAB_SLIDESHOW;

sTAB_SLIDESHOW sSlideShowTable[]={
    {SS_MODE_NORMAL, 	DrawImage},
    {SS_MODE_CROSSFADE, DrawImageSSCrossfade},
    {SS_MODE_LEFT_RIGHT, DrawImageSSLeftToRight},
	{SS_MODE_RIGHT_LEFT, DrawImageSSRightToLeft},
    {SS_MODE_TOP_BOTTOM, DrawImageSSTopToBottom},
    {SS_MODE_BOTTOM_TOP,DrawImageSSBottomToTop},
    {SS_MODE_WATERFALL, DrawImageSSWaterfall},
    {SS_MODE_SNAKE,     DrawImageSSSnake},
    {SS_MODE_DISSOLVE,  DrawImageSSDissolve},
    {SS_MODE_STRIPE,    DrawImageSSStripe},
    {SS_MODE_SHUFFLE,   AllEffectShuffle},
	
};
/**
* <b>FUNCTION: </b>
*
* DrawSlideShow(RMint8 index)
*
* <b>DESCRIPTION: </b>
*
* @param    RMint8 index
* @return   No
*/
static void DrawSlideShow(RMint8 index){
    int i, modes = sizeof(sSlideShowTable)/sizeof(sSlideShowTable[0]);

    if( index >= modes )
        index = rand()%(modes-1)+1;
    for (i=0; i<modes; i++){
        if(index == sSlideShowTable[i].nMode){
            break;
        }
    }
    if(i>=modes) i=0;
    sSlideShowTable[i].func();
}
#endif
// for dlna
#ifdef DLNA_SUPPORT
int InitParameter_DLNA(const char *m_URL,struct SMediaFormat *mformat)
{
	//LOG_DEBUG(">>>> m_URL = %s, format = %d", m_URL, mformat->mediaType);

	amps = 1 ;
	mediaCnt = 0;
   
	//memset(&mediaTab, 0, sizeof(mediaTab));
	//struct SMediaSession *media = &mediaTab[0];
	memset(&mediaTab[mediaCnt], 0, sizeof(mediaTab[mediaCnt]));
    struct SMediaSession *media = &mediaTab[mediaCnt];

	// initialize the media parameters
	media->dest.x = -1;
	media->dest.y = -1;
	media->dest.w = -1;
	media->dest.h = -1;
	media->ampID = -1;

	media->format.mediaType = mformat->mediaType;
	if (media->format.mediaType == MTYPE_ELEM_PCM)
	{
		media->format.formatValid = mformat->mediaType;
		media->format.format.pcm.storage.format = mformat->format.pcm.storage.format;
		media->format.format.pcm.storage.bitsPerSample =mformat->format.pcm.storage.bitsPerSample;
		media->format.format.pcm.storage.MsbFirst = mformat->format.pcm.storage.MsbFirst;
		media->format.format.pcm.rend.samplingFreq = mformat->format.pcm.rend.samplingFreq;
		media->format.format.pcm.rend.channels = mformat->format.pcm.rend.channels;
		media->format.format.pcm.rend.lfe = mformat->format.pcm.rend.lfe;
	}
	else if (media->format.mediaType == MTYPE_ELEM_GRAPHICS) {
		media->dest.x = 0;
		media->dest.y = 0;
		media->dest.w = 1920;	// must be an even number less than 4096
		media->dest.h = 1080;
		media->vdst = true;
		media->bdst = false;
		media->ampID = SECONDARY_VIDEO_AMP_ID;
		media->useImg = true;
	}

	strncpy(media->url, m_URL, sizeof(media->url)-1);
	mediaCnt++;
	// use default AMP IDs for those that have not been specifically assigned
	allocAMPid(media);
    amps = mediaCnt;
	return mediaCnt;
}

/*
* mpOpen
*tDLNAInfo info  VS I may need to add this to pass in more poarams
*/
int mpOpen_DLNA(char* uri, const char* mimetype)
{
	struct SMediaFormat	format;
	char* ptr= NULL;
	char*	delim = NULL;
	//DMA_PRINT("\n >>>mpOpen_DLNA, uri = [%s]\n", uri);
/*	if(m_bPlaying){
		mpStop();	
		sleep(1);
	}
*/
	int iResult		= 0;
	
	// VS TODO - set the appropriate format for rest of the mimetype
	// based on media profile of DLNA standard
	if(strstr(uri, "http://") || strstr(uri, "rtp://")){
		if(strstr(mimetype, "image")){
			m_eFileType = DIR_DLNA_DEMO_FILETYPE_PICTURE;
			format.mediaType = MTYPE_ELEM_GRAPHICS;
		} else if(strcasestr (mimetype, "audio")) {
			m_eFileType = DIR_DLNA_DEMO_FILETYPE_AUDIO;
			// handling LPCM file mime type
			if(strcasestr (mimetype, "L16")) {
				printf("audio format LPCM\n");
				format.mediaType = MTYPE_ELEM_PCM;
				format.format.pcm.storage.bitsPerSample = 16;
				format.formatValid = 1;
				format.format.pcm.storage.format = SF_LPCM;
				format.format.pcm.storage.MsbFirst = true;

				if( (ptr = strstr(mimetype,"rate=")) != NULL){
					char	rate[8]={};

					delim = strstr(ptr, ";");
					ptr = ptr+5;

					if(delim != NULL) {
						strncpy(rate,ptr, delim-ptr);
						rate[delim-ptr] = '\0';
					}else{
						strcpy(rate, ptr);
					}
					format.format.pcm.rend.samplingFreq = atoi(rate);
				}else{
					format.format.pcm.rend.samplingFreq = 44100;
				} 
				if( (ptr = strstr(mimetype,"channels=")) != NULL){
					char channel[3];
					delim = strstr(ptr, ";");
					ptr = ptr+9;

					if(delim != NULL) {
						strncpy(channel,ptr, delim-ptr);
						channel[delim-ptr] = '\0';
					}else{
						strcpy(channel, ptr);
					}
					format.format.pcm.rend.channels = atoi(channel);
				}else{
					format.format.pcm.rend.channels = 2;
				}
				printf("format.format.pcm.rend.samplingFreq = %d\n", format.format.pcm.rend.samplingFreq);
				printf("format.format.pcm.rend.channels= %d\n", format.format.pcm.rend.channels);
				format.format.pcm.rend.lfe = 0;		
			}else if(strcasestr (mimetype, "mpeg")){
				// VS TODO set up the format for mp3 etc
				format.mediaType = MTYPE_ELEM_MP3;
				format.formatValid = 1;
				format.format.sound.channels = 2;
				format.format.sound.samplingFreq = 44100;
				format.format.sound.lfe = 0;
			}else{
				format.mediaType = MTYPE_APP_NONE;
			}
		} else if(strcasestr (mimetype, "video")) {
			m_eFileType = DIR_DLNA_DEMO_FILETYPE_VIDEO;
			printf("\n >>> else types\n");	
			printf("playing video file\n");
			format.mediaType = MTYPE_APP_NONE;
		}else{
			format.mediaType = MTYPE_APP_UNKNOWN;
		}	

		iResult =  InitParameter_DLNA(uri, &format);	
		if (iResult == -1){
			printf("\n>>> Cannot open media !!!\n");		
			return -1 ;
		}	
	}else{
		printf("\n>>> media playback not supported !!!\n");		
		return -1 ;
	} 
	return 0;
}
mpFileType_DLNA_DEMO mpGetFileCategory_DLNA(void)
{
	return m_eFileType;
}

void TerminateAMP_DLNA(int ampIndex, bool releaseAMP)
{
    DFBResult err;
	
	//EndMediaPresentation(ampIndex, releaseAMP);
	end_media_presentation(ampIndex, releaseAMP);
	amp = amp_table[ampIndex].amp = NULL;
	ampfd = amp_table[ampIndex].ampfd = -1;
	if (amp_table[ampIndex].layer)
	{
		// disable the layer output
		DFBScreenMixerConfig mixcfg;
		DFBCHECK(screen->GetMixerConfiguration(screen, 0, &mixcfg));
		mixcfg.flags = (DFBScreenMixerConfigFlags)(DSMCONF_LAYERS);
		DFB_DISPLAYLAYER_IDS_REMOVE(mixcfg.layers, amp_table[ampIndex].layerID);
		DFBCHECK(screen->SetMixerConfiguration(screen, 0, &mixcfg));
		
		// reset the layer 
		amp_table[ampIndex].layer->Release(amp_table[ampIndex].layer);
		amp_table[ampIndex].layer = NULL;
	}
}

/******************************************************************************/
/******************************************************************************/
void PowerOffAMP_DLNA(void)
{
	//printf("\n[%s][%d]\n", __FUNCTION__, __LINE__);
	//FreePlaylist(ampIndex); 
	TerminateAMP_DLNA(ampIndex, true);
	//printf("\n\n>>>>> HoaLe PowerOffAMP_DLNA\n\n");
}
/******************************************************************************/
#endif // DLNA_SUPPORT

char * get_typeVideoWhenPlayMovie(void) {
	char * textResult = NULL;
	if(m_upMyAppStatus) {
		textResult = mediaType2StringForFPTProject(m_upMyAppStatus->lpb.video.format.mediaType);
		printf("\n\n\n get_typeVideoWhenPlayMovie : textResult = %s \n\n\n",textResult);
	}
	return textResult;
}
char * get_typeAudioWhenPlayMovie(void) {
	char * textResult = NULL;
	if(m_upMyAppStatus) {
		textResult = mediaType2StringForFPTProject(m_upMyAppStatus->lpb.audio.format.mediaType);
		printf("\n\n\n get_typeAudioWhenPlayMovie : textResult = %s \n\n\n",textResult);
	}
	return textResult;
}
int get_heightMediaWhenPlayMovie(void){
	int pResult = 720;
	if(m_upMyAppStatus) {
		pResult = m_upMyAppStatus->lpb.video.format.format.image.height;
		if(pResult == 0){ 
			pResult = amp_table[0].img_size.h ;
		}
	}
	else {
		pResult = amp_table[0].img_size.h ;
	}
	printf("\n\n\n get_heightMediaWhenPlayMovie : textResult = %d \n\n\n",pResult);
	return pResult;
}

int get_widthMediaWhenPlayMovie(void){
	int pResult = 1280;
	if(m_upMyAppResult) {
		pResult = m_upMyAppStatus->lpb.video.format.format.image.width;
		if(pResult == 0){ 
			pResult = amp_table[0].img_size.w ;
		}
	}
	else { 
		pResult = amp_table[0].img_size.w ;
	}
	printf("\n\n\n get_widthMediaWhenPlayMovie : textResult = %d \n\n\n",pResult);
	return pResult;
}

double get_Mbps_VideoWhenPlayMovie(void){
	if(m_upMyAppStatus) {
		double pResult = (double)m_upMyAppStatus->generic.statistics.crtBitrate/(1024*1024);
		printf("\n\n\n get_Mbps_VideoWhenPlayMovie : textResult = %.2f \n\n\n",pResult);
		return pResult;
	}
	else {
		return 0;
	}
}

double get_Kbps_AudioWhenPlayMovie(void){
	if(m_upMyAppStatus) {
		double pResult = (double)m_upMyAppStatus->generic.statistics.bufferFullness/(1024*100);
		printf("\n\n\n get_Kbps_AudioWhenPlayMovie : textResult = %.2f \n\n\n",pResult);
		return pResult;
	}
	else {
		return 0;
	}
}

double get_KHz_AudioWhenPlayMovie(void){
	if(m_upMyAppStatus) {
		double pResult = (double)m_upMyAppStatus->lpb.audio.format.format.sound.samplingFreq/1000;
		printf("\n\n\n get_KHz_AudioWhenPlayMovie : textResult = %.2f \n\n\n",pResult);
		return pResult;
	}
	else {
		return 0;
	}
}

bool playVideoWithFramerateSync(void){
	printf("\n\n\n playVideoWithFramerateSync  \n\n\n");
	if(onFramerateSync != 0) {
		if(m_upMyAppStatus) {
			if(m_upMyAppStatus->lpb.video.format.formatValid) {  // .. and we know the content framerate
				if(m_upMyAppStatus->lpb.video.format.format.image.rateM == m_upMyAppStatus->lpb.video.format.format.image.rateM * 30){ //&&	// .. and the content is 30 fps/60i fps
					if(last_hdtv_signal==DSOS_1080p24) {
						set_hdtv_resolution_standard_tv(DSOS_1080i,true);
					}
					else { 
						if(flagEncodeDisplay == false) {
							set_hdtv_resolution_standard_tv(last_hdtv_signal,true);
						}
					}
				}
				else { //&&	// .. and the content is 25 fps/25i fps
					if(m_upMyAppStatus->lpb.video.format.format.image.rateM == m_upMyAppStatus->lpb.video.format.format.image.rateM * 25) {
						if(last_hdtv_signal==DSOS_1080p24) {
							set_hdtv_resolution_standard_tv(DSOS_1080i,false);
						}
					}
					else { //&&	// .. and the content is 24 fps/24 fps
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool playVideoWithActualSize(int wMedia,int hMedia){
	printf("\n\n\n\n ---- change_actual_size: hght =  %d ---- \n\n\n\n",hMedia);
	if (onActualSize) {
		if(hMedia > 0 && hMedia < 720 && wMedia < 1280) {
			printf("\n\n\n\n ---- change_actual_size  sucess---- \n\n\n\n");
			int xScreen = (1280 - wMedia)/2 ;
			int yScreen = (720 - hMedia)/2;
			amp_table[ampIndex].layer->SetScreenRectangle(amp_table[ampIndex].layer, xScreen, yScreen, wMedia, hMedia);
			return true;
		}
	}
	return false;
}


/*******************************************************************
*    @Function
*				set_audio_output_mode()
*		
*    @Description
*       change audio ouput channel        
*				
*    @Param		
*				mode: 0:Stereo, 1:Left, 2:Right, 3:Mono
*				
*    @Return 	
*	            
*				
*******************************************************************/
RMstatus set_audio_output_mode(int mode) // liempham add for Sonca karaoke app
{
	RMstatus status;
	struct AudioEngine_StoreOutputMode_type StoreOutputMode = { 0, };	

	status = RM_ERROR;
	status = RUAGetProperty(ampGlobals->pRUA,
				AudioEngine,
				RMAudioEnginePropertyID_StoreOutputMode,
				&StoreOutputMode,
				sizeof(StoreOutputMode));

	printf("\n\n set_audio_output_mode [%d] ", StoreOutputMode.DualMode);
	if (status == RM_OK) {
		StoreOutputMode.SpeakerConfig = StoreOutputMode.SpeakerConfig;
		StoreOutputMode.DualMode      = mode;
		StoreOutputMode.SPDIF         = StoreOutputMode.SPDIF;
		StoreOutputMode.LFE           = StoreOutputMode.LFE;
		StoreOutputMode.BassMode      = StoreOutputMode.BassMode;
		StoreOutputMode.Surround20    = StoreOutputMode.Surround20;

		status = RUASetProperty(ampGlobals->pRUA, 
								AudioEngine, 
								RMAudioEnginePropertyID_StoreOutputMode, 
								&StoreOutputMode, 
								sizeof(StoreOutputMode), 0);
	}
	return status;
}


void reset_buffer_event()
{
	printf("\033[31mReset key buffer\n\033[0m");
	keybuffer->Reset(keybuffer);	
}

DFBEvent get_event()
{
	DFBEvent event;
	printf("\033[31mGet_event\n\033[0m");
	keybuffer->GetEvent(keybuffer, &event);
	if (&event == NULL){
		printf("Get_event event = NULL\n");
	} else {
		printf("event.input.key_symbol = %u\n", event.input.key_symbol);
	}
	return event;
}

void auto_send_key(void)
{
	unsigned long key = 0xffffffff;
	printf("Auto send key: 0x%08lx\n", key);
}

void post_event(unsigned long key)
{
#if 1
	printf("post_event Key 0x%08lx\n", key);
	DFBInputEvent evt;
	evt.clazz = DFEC_INPUT;
	evt.type = DIET_KEYPRESS;
	evt.flags = DIEF_KEYSYMBOL;
	evt.key_symbol = key;//0x0000f002;//key up

	DFBEvent myEvent;
	myEvent.clazz  = DFEC_INPUT;
	myEvent.input  = evt;
#if 0
	struct SMediaSession *media;
	media = &mediaTab[ampIndex];
	if (media->ampEvent == NULL){
		printf("ampEvent is NULL\n");
		return;
	}
	media->ampEvent->PostEvent(media->ampEvent, &myEvent);
#else
	if (keybuffer == NULL){
		printf("keybuffer is NULL\n");
		return;
	}
	keybuffer->PostEvent(keybuffer, &myEvent);

	//usleep(50000);
	//evt.type = DIET_KEYRELEASE;
	//evt.flags = DIEF_KEYSYMBOL;
	//keybuffer->PostEvent(keybuffer, &myEvent);
#endif
#else
	sendkey(key);
#endif

}
