/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/


/**
 * 
 * <b>FUNCTION: </b>
 *
 *									configuration for audio and video 
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
bool								process_config_confirm(const struct RUA *pRUA,
													  const struct SContentInfo *content,
													  const struct SMRUAConfig *configuration) ;

bool								process_config_preview(const struct RUA *pRUA,
													  const struct SContentInfo *content,
													  const struct rmcci *drm,
													  const struct SMRUAOutputConfig *settings,
													  const struct SAdjustment *adjustments,
													  struct SMRUAConfig *configuration)   ;

bool								check_hdmi_signal(void) ;

void								check_hotplug_hdmi_cable(void);

/**
 * 
 * <b>FUNCTION: </b>
 *
 *									configuration for audio 
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

RMbool								set_digital_audio(RMuint16 index, RMbool SetConfig);

void								set_dynamic_compression(RMuint16);

void								set_effect_on_audio(void);

void 								set_TV_mode(UINT32 tv_mode);

void								set_pcm_down_sampling(RMbool);

RMbool								set_echannel_config(enum EChannelConfig speaker);

RMbool								set_lfe_bassmode_on_audio(RMbool lfe, RMuint32 speaker_size);	

void								set_speaker_config(RMuint16 L, RMuint16 C, RMuint16 R, RMuint16 SL, RMuint16 SR, 
														RMuint16 LEF, RMuint16 BL, RMuint16 BR , RMbool SetConfig);

void								set_volume_for_speaker(enum SpeakerOut speaker); // use for tone

void								reset_digital_audio(RMbool isConvertToPCM);

void								set_bass_mode(enum EBassMode 	bassMode, RMbool isSetConfig);

void								set_surround_mode(RMuint16 mode);

void                                set_PassThrough_Downmix_Audio(const unsigned long value, const unsigned short executeAudioConfig );

void 								set_HDMI_Connected(const bool isConnected);
void								set_connect_audio_analog_stereo(RMuint16 connect,enum ESurroundMode surroundMode,enum EBassMode bassMode,RMuint16 volume,RMbool isEffect);
/**
 * 
 * <b>FUNCTION: </b>
 *
 *									configuration for video
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

RMstatus							set_hdtv_resolution(DFBScreenOutputSignals signal) ;

DFBResult							get_signal_resolution(DFBScreenOutputSignals signal) ;

DFBResult							get_signal_resolution(DFBScreenOutputSignals signal) ;

DFBScreenOutputSignals				get_resolution(RMuint32 iDx) ;

RMstatus							set_output_mode (RMuint8 index) ;

RMstatus 							set_output_mode_for_PAL (RMuint8 index);

RMstatus 							set_output_mode_for_NTSC (RMuint8 index);

void								set_aspect_ratio(RMuint16 as_type);

RMuint16							get_index_of_resolution(DFBScreenOutputSignals signal);

void								set_source_direct_for_tvmode(const struct SContentInfo *content,
															  const struct SMRUAOutputConfig *settings,
															  struct SMRUAConfig *configuration,bool is_hdmi,bool is_component);

void								set_connecting_for_spdif_and_analog(RMbool status);
void 								update_encoder_configuration(char *aspect_ratio);


// code C for FPT probject

bool								 change_framerate_sync(const struct RUA *pRUA,
														   const struct SContentInfo *content,
														   const struct rmcci *drm,
														   const struct SMRUAOutputConfig *settings,
														   const struct SAdjustment *adjustments,
														   struct SMRUAConfig *configuration) ;
bool								 change_actual_size(const struct RUA *pRUA,
														   const struct SContentInfo *content,
														   const struct rmcci *drm,
														   const struct SMRUAOutputConfig *settings,
														   const struct SAdjustment *adjustments,
														   struct SMRUAConfig *configuration) ;

RMstatus							set_output_mode_have_standard_tv (RMuint8 index);
RMstatus 							set_hdtv_resolution_standard_tv(DFBScreenOutputSignals signal, bool bType);
void 								set_color_space_FPT(RMuint16 as_type);
void 								set_TVType_FPT(RMuint16 as_type);
void 								set_VideoZoom_FPT(RMuint16 as_type);
void								set_Framerate_sync(int _type);

char								*mediaType2StringForFPTProject(TMediaType type);








