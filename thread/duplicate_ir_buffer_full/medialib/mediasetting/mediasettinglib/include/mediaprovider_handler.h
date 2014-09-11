/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

 /**
 * 
 * <b>FUNCTION: </b>
 *
 *									start_media_presentation, set_active_amp , start_amp , init_parameter , set_app_call_back
									data_buffer_thread      . process_media_events       . frame_callback . init_amp
									get_destination         . prefetch_data
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *									
 * @return 
 *		 DFBResult
*/
UINT32 							get_media_type(const char *mediaType);
int								init_parameter(const char *m_URL,const char *forceType) ;

DFBResult						init_amp(int i) ;
//	Opens the AMP specified in the presIndex-th set of command line parameters
// Returns its event file descriptor and corresponding layer ID (-1 for surface plauback or video-less AMP)
DFBResult						start_media_presentation(IDirectFB *dfb, int ampIndex, int *pEventFD, int *pLayerID, 
																	IAdvancedMediaProvider **ppAMP, DFBDimension *pImgSz);
void							set_active_amp(int index, bool allow_src_rect_change, bool allow_screen_rect_change) ;

RMstatus						start_amp(void) ;

void							set_app_call_back(void);	

void							*data_buffer_thread(DirectThread *thread, void *arg) ;

//void							process_media_events(int ampIndex , union UMSStatus **pUMSStatus ,
//													 union UMSResult **pUMSResult) ;
void							process_media_events(int ampIndex);

void							frame_callback(void* param);

RMstatus 						set_audio_output_mode(int mode);
void 							reset_buffer_event(void);
DFBEvent						get_event(void);

// Returns the surface for playback in the specified area and frame callback with context to
// be called upon each frame availability
IDirectFBSurface				*get_destination(int ampIndex, bool bgnd, bool secvid, DFBRectangle *area, DVFrameCallback *pCallback, void **pCtx);

DFBResult						prefetch_data(int ampIndex);

/**
 * 
 * <b>FUNCTION: </b>
 *
 *								end_media_presentation
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *							int ampIndex, bool releaseAMP
 * @return 
 *		 void
*/	
void							end_media_presentation(int ampIndex, bool releaseAMP);
// terminates the specified AMP object
void							terminate_amp(int ampIndex, bool releaseAMP) ;

void							power_off_amp(void) ;
void 							DeInitSlideShow(void);

/**
 * 
 * <b>FUNCTION: </b>
 *
 *								DVD Control :   parental_level_change_callback . set_language_for_dvd 
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *							
 * @return 
 *		 void
*/	
#ifndef NO_RED_SUPPORT
int								parental_level_change_callback(int level, void *context) ;

void							set_language_for_dvd(char *menu, char *subtitle, char *audio);

RMbool							check_execute_stopfunction_for_dvd(void);

void							set_parental_level_for_dvd(RMuint8 value);
#endif // NO_RED_SUPPORT

/**
 * 
 * <b>FUNCTION: </b>
 *
 *								BDROM Control :   handle_media_for_bdrom . set_age_region_and_profile_for_bdrom ,
 *												  set_language_for_bdrom . check_execute_stopfunction_for_bdrom
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *							
 * @return 
 *		 void
*/	
#ifndef NO_BLUE_SUPPORT
RMbool							handle_media_for_bdrom(RMbool isEject);

void							set_age_region_and_profile_for_bdrom(RMuint16 age );

void							set_language_for_bdrom(char *menu, char *subtitle, char *audio);

RMbool							check_execute_stopfunction_for_bdrom(void);
#endif // NO_BLUE_SUPPORT
#ifndef NO_RED_SUPPORT
#ifdef UNIFY_PARENTAL_CONTROL
RMdvdParentalLevel				bd_age_to_dvd_level(RMuint8 age);
#endif // UNIFY_PARENTAL_CONTROL
#endif // NO_RED_SUPPORT
/**
 * 
 * <b>FUNCTION: </b>
 *
 *								BDRE Control :   open_bdre_media    . open_bdre_disc    , set_noauto_play
 *												 set_reqpin_status . get_reqpin_status , set_bdre_pin 
 *												 get_pin_code
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *							
 * @return 
 *		 void
*/	
#ifndef NO_BLUE_SUPPORT
// Opens the BDRE media to find out the number of BDAV directories
DFBResult					  open_bdre_media(IDirectFB *dfb, int ampIndex);

RMstatus					  open_bdre_disc(void);
//KhanhNgo add
void						  set_noauto_play(bool no_AutoPlay);

void						  set_reqpin_status(bool status);

bool						  get_reqpin_status(void);

void						  set_bdre_pin(char *pin);

char						  *get_bdre_pin(void);
#endif // NO_BLUE_SUPPORT
/**
 * 
 * <b>FUNCTION: </b>
 *
 *								media Control :   handle_media_keys    . handle_zoom    , set_noauto_play
 *												 handle_color_adjustment 
 *
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *							
 * @return 
 *		 void
*/	
bool						handle_media_keys(int ampIndex, struct SMediaSpaceCmdDefinition p) ;	 

RMstatus					handle_zoom(RMuint8 index) ;

RMstatus					handle_zoom_fine(RMuint8 index) ;

void						handle_color_adjustment(RMuint32 Brightness, RMuint32 Contrast, RMuint32 Hue, RMuint32 Saturation) ;

RMstatus					start_tone(enum SpeakerOut speaker);


RMbool						IsPicturePlaying(void);
#if 0
void						SetWndRect_Preview(RMbool bSet, RMbool bThumbnail);
#endif
/**
 * 
 * <b>FUNCTION: </b>
 *
 *								Play List Contorl :   init_play_list    . free_play_list    , next_play_list_item
 *												 handle_color_adjustment 
 *
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *							
 * @return 
 *		 void
*/	
void						init_play_list(int ampIndex);
void						free_play_list(int ampIndex);
int							next_play_list_item(int ampIndex);
void						set_preview(bool bSetPreview, int x, int y, int w, int h);
void 						set_screen_rec(int x, int y, int w, int h);
/**
 * 
 * <b>FUNCTION: </b>
 *
 *								play picture to background layer :   draw_image_or_text    . draw_image_or_text    
 *												  
 *
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *							
 * @return 
 *		 void
*/	
void						draw_image_or_text(IDirectFBSurface *surf, char *fileName) ;

void						set_name_image_back_ground (char *name) ;

void						SetIndexSubMkv(RMuint16 uiIndex);
//Nghia add
void 						SetVolumeNoMSP(RMuint8 volume);
void                        SetMuteNoMSP(void);
void 						SetVolumeNoMSPForDolby(RMuint8 volume);
bool                        GetGlobalMute(void);

int 						GetGlobalVolume(void);

bool                        DetectOutPutHDMI(void);

bool                        CheckPlugHDMICable(void);

void                        SetOutputConfiguration(RMbool isOutput);

void 						setCloseCaptionStatus(enum EToggle cc_status);

bool                        GetTurnOnStandByStatus(void);
void 						auto_send_key(void);

void 						post_event(unsigned long key);
// for dlna
#ifdef DLNA_SUPPORT
void PowerOffAMP_DLNA(void);
void TerminateAMP_DLNA(int ampIndex, bool releaseAMP);
mpFileType_DLNA_DEMO mpGetFileCategory_DLNA(void);
int mpOpen_DLNA(char* uri, const char* mimetype);
int InitParameter_DLNA(const char *m_URL,struct SMediaFormat *mformat);
#endif // DLNA_SUPPORT
#ifdef PLAY_PICTURE_HARDWARE
/*************slide show***************/
/**
 *
 * <b>FUNCTION: </b>
 *
 *		Slide Show Control :   LoadImage    . ClearImage    , InitSlideShow		, StartSlideShow
 *							 DrawImage	 .	CopyTempSurfacetoSlideSurface	  ,	DeInitSlideShow
 *
 *
 * <b>DESCRIPTION: </b>
 *
 *
 *
 * @param
 *
 * @return
 *		 void
*/
DFBResult LoadImage(char *filename, bool isusetempsurface);
void ClearImage(void);
DFBResult InitSlideShow(char *filename, RMint8 index);
DFBResult StartSlideShow(char *filename, RMint8 index, RMbool bIsZoomInSlideShow);
void DrawImage(void);
void CopyTempSurfacetoSlideSurface(void);
void DeInitSlideShow(void);
void DrawImageSSLeftToRight(void);
void DrawImageSSRightToLeft(void);
void DrawImageSSTopToBottom(void);
void DrawImageSSBottomToTop(void);
void DrawImageRandom(void);
void DrawImageRandom_1Direction(int mode);
void DrawImageRandom_2Directions(int mode);
void DrawImageRandom_4Directions(int mode);
void DrawImageRandom_Rotate(int mode);
void DrawImageSSCrossfade(void);
void DrawImageSSWaterfall(void);
void DrawImageSSSnake(void);
void DrawImageSSDissolve(void);
void DrawImageSSStripe(void);
void AllEffectShuffle(void);
char * get_typeVideoWhenPlayMovie(void);
char * get_typeAudioWhenPlayMovie(void);
int	 get_heightMediaWhenPlayMovie(void);
int  get_widthMediaWhenPlayMovie(void);
double  get_Mbps_VideoWhenPlayMovie(void);
double  get_Kbps_AudioWhenPlayMovie(void);
double  get_KHz_AudioWhenPlayMovie(void);
bool playVideoWithFramerateSync(void);
bool playVideoWithActualSize(int wMedia,int hMedia);
void SetIndexSubInternal(RMuint16 uiIndex);
#endif//end of play picture by hardware
#define VOLUME_0DB			0x10000000
#define VOLUME_INDEX_0DB	74//50//79//47//49
#define MAX_VOLUME_TABLE 	49//47//74//55//
#define MAX_VOLUME_TABLE_DOLBY 	74//47//74//55//
#define MAX_UI_VOLUME       100
#define MLB_MODE 			3
