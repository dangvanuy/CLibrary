/*****************************************
 Copyright  2005
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**
  @file   avconfig.c
  @brief  Audio/Video configuration definition
  
  This file is included in another C files to initialize AV configuration tables

  @author Sorin Papuc
  @date   2006-05-14

  $Log: mediaconfig.c,v $
  Revision 1.1.1.1  2013/02/20 09:35:10  liempham
  Imported sources

  Revision 1.2  2013/01/24 01:02:24  hdo
  BugId: 38635
  Description:
   - Import dtv apis into sigmaobjectextension
  SEND_COMMENT_BUGZILLA

  Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
  add new

  Revision 1.3  2011/12/17 03:33:11  thuytran

  BugId: 34253
  Bug Description: Audio is not output when spdif is set to bitstream and hdmi is
  set to pcm or both output pcm
  Reason: select multi output at the same time
  How to fix: change GUI to stereo, optical and hdmi, users select only one in
  tree option
  SEND_COMMENT_BUGZILLA

  Revision 1.2  2011/08/26 07:13:07  liempham

  BugId: 31699
    - execute command line dos2unix to convert these files to linux format.
  SEND_COMMENT_BUGZILLA

  Revision 1.1  2011/04/25 04:35:04  liempham

  BugId: 31699 add sc_sdk module
  SEND_COMMENT_BUGZILLA

  Revision 1.2  2010/10/11 01:32:17  trihuynh
  BugId : None
  	Control using hand to push tray in.

  Revision 1.1.1.1  2010/07/19 02:47:55  cvsadmin
  add new

  Revision 1.5  2009/04/01 00:26:39  pvu
  BugId:ct-6322
  Only DM_stereo dual-mode is working. Will ask audio group to check why other dual-modes are not working.

  Revision 1.4  2008/10/16 21:08:20  trihuynh
  BugId:none
  	Fix: add some format in audio Table.

  Revision 1.3  2008/09/25 00:52:56  pvu
  BugId:none
  Fixed the formats and rates supported by SPDIF,i.e. no MPEG audio.

  Revision 1.2  2008/08/02 09:03:04  pvu
  BugId: none
  Changes to configure player's TV system: NTSC, NTSC_J, PAL, SECAM.

  Revision 1.1.1.1  2008/06/09 11:51:54  pvu
  Initial check-in 6/9/2008.


  Revision 1.40  2008-05-23 20:47:19  sorin
  BugId: none
  Added support for 2 fields adaptive deinterlacing mode

  Revision 1.39  2008-04-29 00:49:43  sorin
  BugId: none
  Modified standard audio configurations to increase testing coverage

  Revision 1.38  2007-05-08 20:35:49  sotescu
  BUGCT: none
  Added EDTV to list of supported component modes for video configs 5,7.

  Revision 1.37  2007/04/12 22:01:47  sorinp
  set SPDIF sampling rate capabilities to 22.5, 32, 44.1 and 48 KHz for all SD formats, none for HD formats

  Revision 1.36  2007/04/12 15:36:56  sotescu
  BUGCT: 2940, 2942
  Added 2 new configs (6, 7) for unrestricted NTSC.

  Revision 1.35  2007/04/11 14:04:08  sotescu
  BUGCT: 2642
  Removed ARM limitations from video config #1.

  Revision 1.34  2007/04/04 03:00:40  sorinp
  Merged experimental_core_audio_config_support branch

  Revision 1.33.2.4  2007/04/03 02:46:27  sorinp
  typo

  Revision 1.33.2.3  2007/04/03 02:45:12  sorinp
  Fixed analog speaker config value

  Revision 1.33.2.2  2007/04/03 02:38:13  sorinp
  Removed impossible SPDIF formats: DDP, DTSHD and TrueHD

  Revision 1.33.2.1  2007/04/02 23:42:17  sorinp
  Added preserveOriginalQuality audio parameter

  Revision 1.33  2007/03/21 01:25:20  sorinp
  Made separate SPDIF and I2S connections to the HDMI output

  Revision 1.32  2007/03/09 16:57:38  sotescu
  BUGCT: 2611
  Added 2 new PAL video configs that don't force a specific ARM mode.

  Revision 1.31  2007/01/15 13:54:10  sotescu
  videoConfigTab now contains 4 configurations: the first 2 are NTSC-based (high-end + low-end), the last 2 are PAL-based (same idea).

  Revision 1.30  2006/12/02 03:15:39  sorinp
  Added DRM configuration (7) for all digital audio disable

  Revision 1.29  2006/11/30 00:01:13  sorinp
  Bass management for all but the multichannel output is ignored, set it to BM_None in config structures.

  Revision 1.28  2006/11/28 07:20:30  sorinp
  improved bass management support

  Revision 1.27  2006/11/10 02:02:16  sorinp
  Minimum set of SPDIF rates is 32000, 44100 and 48000 Hz

  Revision 1.26  2006/11/07 23:21:17  sorinp
  Added support for interactive DRM config

  Revision 1.25  2006/11/07 22:32:41  sorinp
  Added comments for easy identification

  Revision 1.24  2006/11/07 02:18:58  sorinp
  Forced PCM-only support on the HDMI connection of the low-end configuration

  Revision 1.23  2006/11/06 20:59:12  sotescu
  Added the new fields from rmcci to DRM configurations.

  Revision 1.22  2006/10/24 16:56:14  sorinp
  Added upsampling capabilities and consistent use of the sampling rate mask for rate calculations

  Revision 1.21  2006/09/26 20:05:12  sorinp
  Fixed CC settings

  Revision 1.20  2006/09/26 19:36:59  anagy
  CCtype for HDMI

  Revision 1.19  2006/09/22 02:07:07  sotescu
  Now low-end config forces 4x3 component output.

  Revision 1.18  2006/08/30 19:19:32  sorinp
  SPDIF is currently capable of only up to 96 KHz

  Revision 1.17  2006/08/26 04:24:41  sorinp
  More usefull bass management settings

  Revision 1.16  2006/07/14 05:48:04  smanciulea
  - connection statusfor multi-channel, SPDIF and Stereo is based on the flags defined in DCCHD.env for different boards

  Revision 1.15  2006/07/12 21:06:42  sorinp
  Added original bitstream preservation support

  Revision 1.14  2006/06/29 05:29:13  sorinp
  Added adjustment, config and callback support in amp_test

  Revision 1.13  2006/06/22 02:15:42  sorinp
  force RGB for low-end HDMI connections

  Revision 1.12  2006/06/20 21:37:31  sorinp
  made the preferred signals a separate field to avoid conflicts with existing code (multiple bits set in the signals field)

  Revision 1.11  2006/06/16 22:38:48  sorinp
  configuration-related changes

  Revision 1.10  2006/06/13 20:27:46  sorinp
  Updated default configuration

  Revision 1.9  2006/06/10 02:44:38  sorinp
  fixed preferecne settings

  Revision 1.8  2006/06/07 21:17:23  sorinp
  changed TV system from single value to bitfield
  Implemenetd update_video_settings by parsing the actual HDMI info
  Work in progress, compiles but may not work

  Revision 1.7  2006/06/06 02:46:46  sorinp
  fixes and updates, mostly related to av config

  Revision 1.6  2006/06/01 06:42:52  sorinp
  Added support for DRM rules

  Revision 1.5  2006/05/30 09:01:59  sorinp
  allow detailed bitstream settings in digital audio connections

  Revision 1.4  2006/05/28 22:06:25  sorinp
  audio configuration related changes

  Revision 1.3  2006/05/15 16:21:44  sorinp
  Removed generic multichannel setting

  Revision 1.2  2006/05/15 02:53:45  sorinp
  syntax fixes

  Revision 1.1  2006/05/15 01:17:58  sorinp
  new configurationscheme related changes

*/


static struct SMRUAAudioOutConfig audioConfigTab[] =
{
	// high-end configuration, with bitstream preservation
	{
		{
			sizeof(struct SMRUAAudioOutConfig),

			// hdmi
			{
				{
					CS_Unknown,		// detected automatically
					0,				// supported formats auto-detected
					{
						{0, 0},
					},
					CCf_default,	// autodetect number of channels on PCM link
					0,				// autodetect max sample rate on PCM link
					1,				// preserve original bitstream
					1,				// preserve original quality
				},

				{
					CCf_LCRSlSrBlBr,// force 7 main speaker configuration
//					CCf_LCRBlBr,// force 5 main speaker configuration
					1,				// force LFE
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_OFF,	  	  // surround mode
					BM_None,
				},
			},

			// multiChannel
			{
#if RMCFG_ANALOG_ENGINES == 0
				CS_Disconnected,
#else
				CS_Connected,
#endif
				{
					CCf_default,	  // default (as per platform definition) speaker configuration
					0,				  // LFE speaker will be inferred from the field above
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_OFF,	  	  // surround mode
					BM_None,
				},
				0,				// preserve original quality
			},

			// stereo
			{
#if RMCFG_STEREO_ENGINES == 0
				CS_Disconnected,
#else
				CS_Connected,
#endif
				{ 0x10000000, 0x10000000},	// volume
				SRND_Default,	  			// surround mode
				BM_None,				  	// bass management
			},

			// spdif
			{
				{
#if RMCFG_SPDIF_ENGINES == 0
					CS_Disconnected,
#else
					CS_Connected,
#endif
					  AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_PCM)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_AC3)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_AAC)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_DTS)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_WMAPRO)
					,
					{
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 7}, // PCM
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // AC3
						{0, 0}, // MPEG1 layer 1&2
						{0, 0}, // MPEG1 layer 3
						{0, 0}, // MPEG2 BC
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // AAC
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // DTS
						{0, 0}, // ATRAC
						{0, 0}, // One Bit Audio (SACD)
						{0, 0}, // Dolby Digital +
						{0, 0}, // DTS-HD
						{0, 0}, // MAT(MLP) DolbyDigital Lossless (TrueHD)
						{0, 0}, // DST
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // WMAPRO
					},
					CCf_LR,		// 2 PCM channels
					48000,		// max sample rate on PCM link
					1,			// preserve original bitstream
					1,			// preserve original quality
				},

				{
					CCf_LCRSlSr, 	  // multi-channel speaker configuration
					1,				  // LFE
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_Default,	  // surround mode
					BM_None,		  // bass management
				},
			},

			// DRC
			{
				0x10000000,		// default scaleLo
				0x10000000,		// default scaleHi
				0x10000000,		// default scalePCM
				COMP_digital,	// compression type
			},
			
			// dual mode..seems only DM_Stereo works!!
			DM_Stereo,
			//DM_LeftMono,
			//DM_RightMono,
			//DM_MixMono,
		},
		RMCFG_HDMI_ENGINES,
		RMCFG_ANALOG_ENGINES,
		RMCFG_STEREO_ENGINES,
		RMCFG_SPDIF_ENGINES,

		{ 0, 0},	// no audio/video offset

		RMCFG_HDMI_I2S_ENGINES,
		RMCFG_HDMI_SPDIF_ENGINES,
	},

	// high-end configuration, no original bitstream preservation
	{
		{
			sizeof(struct SMRUAAudioOutConfig),

			// hdmi
			{
				{
					CS_Unknown,		// detected automatically
					0,				// supported formats auto-detected
					{
						{0, 0}
					},
					CCf_default,	// autodetect number of channels on PCM link
					0,				// autodetect max sample rate on PCM link
					0,				// preserve original bitstream
					1,				// preserve original quality
				},

				{
					CCf_LCRSlSrBlBr,  // force 7 main speaker configuration
//					CCf_default,	  // autodetect
					1,				  // force LFE
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_OFF,	  	  // surround mode
					BM_None,		  // bass management
				},
			},

			// multiChannel
			{
#if RMCFG_ANALOG_ENGINES == 0
				CS_Disconnected,
#else
				CS_Connected,
#endif
				{
					CCf_default,	  // default (as per platform definition) speaker configuration
					0,				  // LFE speaker will be inferred from the field above
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_OFF,	  	  // surround mode
					BM_Auto,		  // bass management
				},
				0,		// preserve original quality
			},

			// stereo
			{
#if RMCFG_STEREO_ENGINES == 0
				CS_Disconnected,
#else
				CS_Connected,
#endif
				{ 0x10000000, 0x10000000},	// volume
				SRND_OFF,	  				// surround mode
				BM_None,			  		// bass management
			},

			// spdif
			{
				{
#if RMCFG_SPDIF_ENGINES == 0
					CS_Disconnected,
#else
					CS_Connected,
#endif
					  AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_PCM)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_AC3)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_AAC)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_DTS)
					| AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_WMAPRO)
					,
					{
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 7}, // PCM
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // AC3
						{0, 0}, // MPEG1 layer 1&2
						{0, 0}, // MPEG1 layer 3
						{0, 0}, // MPEG2 BC
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // AAC
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // DTS
						{0, 0}, // ATRAC
						{0, 0}, // One Bit Audio (SACD)
						{0, 0}, // Dolby Digital +
						{0, 0}, // DTS-HD
						{0, 0}, // MAT(MLP) DolbyDigital Lossless (TrueHD)
						{0, 0}, // DST
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 512000}, // WMAPRO
					},
					CCf_LR,		// 2 PCM channels
					48000,		// max sample rate on PCM link
					0,			// preserve original bitstream
					0,			// preserve original quality
				},

				{
					CCf_LCRSlSr, 	  // multi-channel speaker configuration
					1,				  // LFE
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_Default,	  // surround mode
					BM_None,		  // bass management
				},
			},

			// DRC
			{
				0x10000000,		// default scaleLo
				0x10000000,		// default scaleHi
				0x10000000,		// default scalePCM
				COMP_digital,	// compression type
			},

			// dual mode
			DM_Stereo,
		},
		RMCFG_HDMI_ENGINES,
		RMCFG_ANALOG_ENGINES,
		RMCFG_STEREO_ENGINES,
		RMCFG_SPDIF_ENGINES,

		{ 0, 0},	// no audio/video offset

		RMCFG_HDMI_I2S_ENGINES,
		RMCFG_HDMI_SPDIF_ENGINES,
	},

	// low-level configuration
	{
		{
			sizeof(struct SMRUAAudioOutConfig),

			// hdmi
			{
				{
					CS_Unknown,		// detected automatically
					AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_PCM), // force PCM only
					{
						{0, 0},
					},
					CCf_default,	// autodetect number of channels on PCM link
					0,				// autodetect max sample rate on PCM link
					0,				// preserve original bitstream
					0,				// preserve original quality
				},

				{
					CCf_default,	  // autodetect speaker configuration
					0,  			  // autodetect LFE
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_Default,	  // surround mode
					BM_None,		  // bass management
				}
			},

			// multiChannel
			{
#if RMCFG_ANALOG_ENGINES == 0
				CS_Disconnected,
#else
				CS_Connected,
#endif
				{
					CCf_default,	  // default (as per platform definition) speaker configuration
					0,				  // LFE speaker will be inferred from the field above
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_Default,	  // surround mode
					BM_all_2_lfe,	  // bass management
				},
				0,				// preserve original quality
			},

			// stereo
			{
#if RMCFG_STEREO_ENGINES == 0
				CS_Disconnected,
#else
				CS_Connected,
#endif
				{ 0x10000000, 0x10000000},	// volume
				SRND_ON,			  		// surround mode
				BM_None,			  		// bass management
			},


			// spdif
			{
				{
#if RMCFG_SPDIF_ENGINES == 0
					CS_Disconnected,
#else
					CS_Connected,
#endif
					AUDIO_MTYPE_TO_BITMASK(MTYPE_ELEM_PCM),
					{
						{ASRATE_TO_BITMASK(FREQ_22050) | ASRATE_TO_BITMASK(FREQ_32000) | ASRATE_TO_BITMASK(FREQ_44100) | ASRATE_TO_BITMASK(FREQ_48000), 7}, // PCM
					},
					CCf_LR,		// 2 PCM channels
					48000,		// max sample rate on PCM link
					0,				// preserve original bitstream
					0,				// preserve original quality
				},

				{
					CCf_LR,
					0,				  // LFE
					{ 0}, 			  // delay
					{ 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000, 0x10000000}, 	// default volume
					SRND_ON,	  	  // surround mode
					BM_None, 		  // bass management
				},
			},

			// DRC
			{
				0x10000000,		// default scaleLo
				0x10000000,		// default scaleHi
				0x10000000,		// default scalePCM
				COMP_digital,	// compression type
			},

			// dual mode
			DM_Stereo,
		},
		RMCFG_HDMI_ENGINES,
		RMCFG_ANALOG_ENGINES,
		RMCFG_STEREO_ENGINES,
		RMCFG_SPDIF_ENGINES,

		{ 0, 0},	// no audio/video offset

		RMCFG_HDMI_I2S_ENGINES,
		RMCFG_HDMI_SPDIF_ENGINES,
	},

};
	
static struct SMRUAVideoOutConfig videoConfigTab[] = 
{
#ifdef RMCFG_TV_STD_PAL
	{
		{
			sizeof(struct SMRUAVideoOutConfig),

			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_Unknown,
					CS_Default,
					ECC_Unknown,
				},
				ARM_SidePanel,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_608,
				},
				ARM_SidePanel,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_PAL),
					TV_SIGNAL_TO_BITMASK(TVS_PAL),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_LetterBox,
				DEINT_Adaptive,
				TOGGLE_ON,	// CC on
			}
		},

		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},
#elif defined RMCFG_TV_STD_SECAM
	{
		{
			sizeof(struct SMRUAVideoOutConfig),
	
			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_Unknown,
					CS_Default,
					ECC_Unknown,
				},
			 	ARM_SidePanel,
			 	DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},
			
			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_608,
				},
			 	ARM_SidePanel,
			 	DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},
		
			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_SECAM),
					TV_SIGNAL_TO_BITMASK(TVS_SECAM),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
			 	ARM_LetterBox,
			 	DEINT_Adaptive,
				TOGGLE_ON,	// CC on
			}
		},

		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	}
#elif defined RMCFG_TV_STD_NTSC_J // Japan
	{
		{
			sizeof(struct SMRUAVideoOutConfig),
	
			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_Unknown,
					CS_Default,
					ECC_Unknown,
				},
			 	ARM_SidePanel,
			 	DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},
			
			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_608,
				},
			 	ARM_SidePanel,
			 	DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},
		
			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_NTSC),
					TV_SIGNAL_TO_BITMASK(TVS_NTSC_J),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
			 	ARM_LetterBox,
			 	DEINT_Adaptive,
				TOGGLE_ON,	// CC on
			}
		},

		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	}
#elif defined RMCFG_TV_STD_NTSC
	{
		{
			sizeof(struct SMRUAVideoOutConfig),
				
				// hdmi
			{
				CS_Unknown,		// detected automatically
				{
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
						TV_SIGNAL_TO_BITMASK(TVS_Unknown),
						0,
						TVAR_Unknown,
						TVAR_Unknown,
						CS_Default,
						ECC_Unknown,
						0,
						0,
						8,
						SM_Default
				},
				ARM_SidePanel,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},
			
			// component
			{
						CS_Connected,
						{
							TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
								TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
								0,
								TVAR_16x9,
								TVAR_Unknown,
								CS_Default,
								ECC_608,
								0,
								0,
								8,
								SM_Default
						},
						ARM_SidePanel,
						DEINT_Adaptive,
						TOGGLE_OFF,	// no CC
					},
					
					// composite
					{
								CS_Connected,
								{
									TV_SYSTEM_TO_BITMASK(TVSYS_NTSC),
										TV_SIGNAL_TO_BITMASK(TVS_NTSC),
										0,
										TVAR_4x3,
										TVAR_Unknown,
										CS_Default,
										ECC_608,
										0,
										0,
										8,
										SM_Default
								},
								ARM_LetterBox,
								DEINT_Adaptive,
								TOGGLE_ON,	// CC on
							},
							
							0,
							true
		},
		
		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},
#else
	#error *** Unknown TV system or TV system not defined! ***
#endif /* RMCFG_TV_STD_xxx */

#if 0 // deprecated...reference video configurations
	// NTSC high-end configuration
	{
		{
			sizeof(struct SMRUAVideoOutConfig),
	
			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_Unknown,
					CS_Default,
					ECC_Unknown,
				},
			 	ARM_SidePanel,
			 	DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},
			
			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_608,
				},
			 	ARM_SidePanel,
			 	DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},
		
			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_NTSC),
					TV_SIGNAL_TO_BITMASK(TVS_NTSC),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
			 	ARM_LetterBox,
			 	DEINT_Adaptive,
				TOGGLE_ON,	// CC on
			}
		},

		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},
				
	// NTSC low-end configuration
	{
		{
			sizeof(struct SMRUAVideoOutConfig),
	
			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_Unknown,
					CS_RGB_16_235,
					ECC_608,
				},
			 	ARM_NotSet,
			 	DEINT_Adaptive2Fields,
				TOGGLE_ON,	// CC on
			},
			
			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
			 	ARM_NotSet,
			 	DEINT_Adaptive2Fields,
				TOGGLE_ON	// CC on
			},
			
			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_NTSC),
					TV_SIGNAL_TO_BITMASK(TVS_NTSC),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
			 	ARM_NotSet,
			 	DEINT_Adaptive2Fields,
				TOGGLE_OFF,	// no CC
			}
		},
		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},
				
	// PAL high-end configuration
	{
		{
			sizeof(struct SMRUAVideoOutConfig),

			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_Unknown,
					CS_Default,
					ECC_Unknown,
				},
				ARM_SidePanel,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_608,
				},
				ARM_SidePanel,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_PAL),
					TV_SIGNAL_TO_BITMASK(TVS_PAL),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_LetterBox,
				DEINT_Adaptive,
				TOGGLE_ON,	// CC on
			}
		},

		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},

	// PAL low-end configuration
	{
		{
			sizeof(struct SMRUAVideoOutConfig),

			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_Unknown,
					CS_RGB_16_235,
					ECC_608,
				},
				ARM_SidePanel,
				DEINT_Adaptive2Fields,
				TOGGLE_ON,	// CC on
			},

			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_Panorama,
				DEINT_Adaptive2Fields,
				TOGGLE_ON	// CC on
			},

			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_PAL),
					TV_SIGNAL_TO_BITMASK(TVS_PAL),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_PanScan,
				DEINT_Adaptive2Fields,
				TOGGLE_OFF,	// no CC
			}
		},
		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},

	// PAL high-end configuration without AR restrictions
	{
		{
			sizeof(struct SMRUAVideoOutConfig),

			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_Unknown,
				},
				ARM_NotSet,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_PAL),
					TV_SIGNAL_TO_BITMASK(TVS_PAL),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive,
				TOGGLE_ON,	// CC on
			}
		},

		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},

	// PAL low-end configuration without AR restrictions
	{
		{
			sizeof(struct SMRUAVideoOutConfig),

			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_4x3,
					CS_RGB_16_235,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive2Fields,
				TOGGLE_ON,	// CC on
			},

			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive2Fields,
				TOGGLE_ON	// CC on
			},

			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_PAL),
					TV_SIGNAL_TO_BITMASK(TVS_PAL),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive2Fields,
				TOGGLE_OFF,	// no CC
			}
		},
		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},

	// NTSC high-end configuration without AR restrictions
	{
		{
			sizeof(struct SMRUAVideoOutConfig),

			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_Unknown,
				},
				ARM_NotSet,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV) | TV_SIGNAL_TO_BITMASK(TVS_720p) | TV_SIGNAL_TO_BITMASK(TVS_1080i),
					0,
					TVAR_16x9,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive,
				TOGGLE_OFF,	// no CC
			},

			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_NTSC),
					TV_SIGNAL_TO_BITMASK(TVS_NTSC),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive,
				TOGGLE_ON,	// CC on
			}
		},

		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},

	// NTSC low-end configuration without AR restrictions
	{
		{
			sizeof(struct SMRUAVideoOutConfig),

			// hdmi
			{
				CS_Unknown,		// detected automatically
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_Unknown),
					0,
					TVAR_4x3,
					CS_RGB_16_235,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive2Fields,
				TOGGLE_ON,	// CC on
			},

			// component
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60),
					TV_SIGNAL_TO_BITMASK(TVS_SDTV) | TV_SIGNAL_TO_BITMASK(TVS_EDTV),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive2Fields,
				TOGGLE_ON	// CC on
			},

			// composite
			{
				CS_Connected,
				{			
					TV_SYSTEM_TO_BITMASK(TVSYS_NTSC),
					TV_SIGNAL_TO_BITMASK(TVS_NTSC),
					0,
					TVAR_4x3,
					CS_Default,
					ECC_608,
				},
				ARM_NotSet,
				DEINT_Adaptive2Fields,
				TOGGLE_OFF,	// no CC
			}
		},
		RMCFG_VOUTPUT_MAIN_DIGITAL,
		RMCFG_VOUTPUT_MAIN_ANALOG,
		RMCFG_VOUTPUT_CVBS_ANALOG,
	},
#endif /* reference */
};


static struct rmcci drmConfigTab[] = 
{
	// index 0
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		FALSE, // DigitalCompressedAudio_disable
		FALSE, // DigitalUncompressedAudio_disable
		FALSE, // SPDIF_PCMLimit
		TRUE, // SPDIF_CpBit
		FALSE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		1, // Video_agc
		1, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		FALSE, // HDCP_Enable
		1,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 1
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		FALSE, // DigitalCompressedAudio_disable
		FALSE, // DigitalUncompressedAudio_disable
		FALSE, // SPDIF_PCMLimit
		FALSE, // SPDIF_CpBit
		TRUE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		2, // Video_agc
		2, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		TRUE, // HDCP_Enable
		1,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 2
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		FALSE, // DigitalCompressedAudio_disable
		FALSE, // DigitalUncompressedAudio_disable
		FALSE, // SPDIF_PCMLimit
		TRUE, // SPDIF_CpBit
		TRUE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		3, // Video_agc
		3, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		TRUE, // HDCP_Enable
		1,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 3
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		FALSE, // DigitalCompressedAudio_disable
		FALSE, // DigitalUncompressedAudio_disable
		TRUE, // SPDIF_PCMLimit
		TRUE, // SPDIF_CpBit
		TRUE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		3, // Video_agc
		3, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		TRUE, // AnalogVideo_imageConstraint
		TRUE, // HDCP_Enable
		0,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 4
	{
		TRUE, // DirtyBit
		TRUE, // AnalogAudio_disable
		FALSE, // DigitalCompressedAudio_disable
		FALSE, // DigitalUncompressedAudio_disable
		TRUE, // SPDIF_PCMLimit
		TRUE, // SPDIF_CpBit
		TRUE, // SPDIF_LBit
		TRUE, // AnalogVideo_disable
		0, // Video_agc
		0, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		TRUE, // HDCP_Enable
		1,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 5
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		TRUE, // DigitalCompressedAudio_disable
		FALSE, // DigitalUncompressedAudio_disable
		TRUE, // SPDIF_PCMLimit
		TRUE, // SPDIF_CpBit
		TRUE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		3, // Video_agc
		3, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		TRUE, // HDCP_Enable
		2,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 6
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		FALSE, // DigitalCompressedAudio_disable
		TRUE, // DigitalUncompressedAudio_disable
		TRUE, // SPDIF_PCMLimit
		TRUE, // SPDIF_CpBit
		TRUE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		3, // Video_agc
		3, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		TRUE, // HDCP_Enable
		3,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 7
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		TRUE, // DigitalCompressedAudio_disable
		TRUE, // DigitalUncompressedAudio_disable
		FALSE, // SPDIF_PCMLimit
		FALSE, // SPDIF_CpBit
		FALSE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		3, // Video_agc
		3, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		TRUE, // HDCP_Enable
		3,	// Video_aps
		FALSE,	// Video_rcd
	},

	// index 8
	// back to normal
	{
		TRUE, // DirtyBit
		FALSE, // AnalogAudio_disable
		FALSE, // DigitalCompressedAudio_disable
		FALSE, // DigitalUncompressedAudio_disable
		FALSE, // SPDIF_PCMLimit
		FALSE, // SPDIF_CpBit
		FALSE, // SPDIF_LBit
		FALSE, // AnalogVideo_disable
		0, // Video_agc
		0, // Video_CGMSA
		FALSE, // DigitalVideo_disable
		FALSE, // AnalogVideo_imageConstraint
		FALSE, // HDCP_Enable
		0,	// Video_aps
		FALSE,	// Video_rcd
	},
};

