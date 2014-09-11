/*****************************************
 Copyright  2005
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

#include <stdio.h>    /* for `fprintf()' */
#include <limits.h>
#include <assert.h>

#include <rmdef/rmdef.h>
#include <emhwlib/include/emhwlib.h>
#include <rua/include/rua.h>

// from directfb/install/dfb-smp86xx/lib/direct/types.h
#if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)
#ifndef false
#define false (0)
#endif
#ifndef true
#define true (!false)
#endif
//typedef __u8 bool;
typedef unsigned char bool;
#endif

#ifndef __INTS_DEFINED__
#define __INTS_DEFINED__
typedef RMint16 INT16;      ///< INT16 must be defined before including mdefs.h

typedef RMuint16 UINT16;    ///< UINT16 must be defined before including mdefs.h

typedef RMint32 INT32;      ///< INT32 must be defined before including mdefs.h

typedef RMuint32 UINT32;    ///< UINT32 must be defined before including mdefs.h

typedef RMint64 INT64;      ///< INT64 must be defined before including mdefs.h

typedef RMuint64 UINT64;    ///< UINT64 must be defined before including mdefs.h

#endif

#include "globals.h"
#include "include/mediaconfig_dump.h"

static char colorSpace[][16] =
{
	"Default",
	"RGB_16_235",
	"RGB_0_255",
	"YUV_601",
	"YUV_709"
};

static char aspectRatioMatch[][16] =
{
	"NotSet",
	"Auto",
	"LetterBox",
	"PanScan",
	"SidePanel",
	"Crop",
	"Zoom",
	"Panorama",
	"None"
};

static char bassMode[][24] = 
{
	"None",
	"all_2_lfe",
	"CLsRs_2_LR",
	"CLsRs_2_lfe",
	"LsRs_2_LCR",
	"LsRs_2_lfe",
	"C_2_LR",
	"C_2_lfe",
	"Auto"
};

static void print_audio_decoder_config(const struct SMRUAAudioDecoderConfig *decoder)
{
	static char outputSPDIF[][24] = 
	{
		"Disable",
		"Compressed",
		"Invalid",
		"Uncompressed",
		"Invalid",
		"NoDecodeCompressed",
	};

	struct AudioDecoder_AACParameters_type *aac	= 
		(struct AudioDecoder_AACParameters_type*)&decoder->params.params;	///< use #MTYPE_ELEM_AAC as type
	struct AudioDecoder_Ac3Parameters_type *ac3	= 
		(struct AudioDecoder_Ac3Parameters_type*)&decoder->params.params;	///< use #MTYPE_ELEM_AC3 as type
	struct AudioDecoder_DtsParameters_type *dts = 
		(struct AudioDecoder_DtsParameters_type*)&decoder->params.params;	///< use #MTYPE_ELEM_DTS as type
	struct AudioDecoder_MpegParameters_type *mpeg = 
		(struct AudioDecoder_MpegParameters_type*)&decoder->params.params;    ///< use #MTYPE_ELEM_MPA as type
	struct AudioDecoder_LpcmVobParameters_type *lpcmVOB =
		(struct AudioDecoder_LpcmVobParameters_type*)&decoder->params.params; 	///< use #MTYPE_ELEM_PCM | #MTYPE_APP_DVD as type
	struct AudioDecoder_LpcmAobParameters_type *lpcmAOB = 
		(struct AudioDecoder_LpcmAobParameters_type*)&decoder->params.params; 	///< use #MTYPE_ELEM_PCM | #MTYPE_APP_DVDA as type
	struct AudioDecoder_PcmCdaParameters_type *pcmCDA = 
		(struct AudioDecoder_PcmCdaParameters_type*)&decoder->params.params;   	///< use #MTYPE_ELEM_PCM | #MTYPE_APP_CDA as type
	struct AudioDecoder_LpcmBDParameters_type *lpcmBD = 
		(struct AudioDecoder_LpcmBDParameters_type*)&decoder->params.params;	///< use #MTYPE_ELEM_PCM | #MTYPE_APP_BRD
	struct AudioDecoder_PCMXParameters_type *PCMXParams = 
		(struct AudioDecoder_PCMXParameters_type*)&decoder->params.params;	///< use #MTYPE_ELEM_PCM | #MTYPE_APP_BDJ
	struct AudioDecoder_DVDAParameters_type *trueHD	= 
		(struct AudioDecoder_DVDAParameters_type*)&decoder->params.params;	///< use #MTYPE_ELEM_MLP as type
	struct AudioDecoder_DraParameters_type *dra	=
		(struct AudioDecoder_DraParameters_type*)&decoder->params.params;	///< use #MTYPE_ELEM_DRA as type

	printf("\t\t\tMRUA type: ");
	switch(decoder->type)
	{
		case AudioDecoder_Codec_DRA:
			printf("DRA\n");
			break;
		case AudioDecoder_Codec_AC3:
			printf("Dolby Digital (%#lx)\n", decoder->subType);
			break;
		case AudioDecoder_Codec_MPEG1:
			printf("MPEG-2 Audio (%#lx)\n", decoder->subType);
			break;
		case AudioDecoder_Codec_AAC:
			printf("MPEG-4 Audio (AAC)\n");
			break;
		case AudioDecoder_Codec_DVDA:
			printf("DVDA\n");
			break;
		case AudioDecoder_Codec_PCM:
			/* MRUA codec subtype: 0:PCMcda, 1:VOB, 2:AOB, 3:BDpcm*/
			switch(decoder->subType)
			{
				case 0:
					printf("PCM cda\n");
					break;
				case 1:
					printf("PCM VOB\n");
					break;
				case 2:
					printf("PCM AOB\n");
					break;
				case 3:
					printf("PCM BD\n");
					break;
				default:
					printf("PCM unknown subtype!!!\n");
					break;
			}
			break;
		case AudioDecoder_Codec_DTS:
			printf("DTS (%#lx)\n", decoder->subType);
			break;
		case AudioDecoder_Codec_DTSLBR:
			printf("DTS-HD LBR\n");
			break;
		case AudioDecoder_Codec_WMA:
			printf("WMA\n");
			break;
		case AudioDecoder_Codec_WMAPRO:
			printf("WMAPRO\n");
			break;
		case AudioDecoder_Codec_TTONE:
			printf("TTONE\n");
			break;
		case AudioDecoder_Codec_WMAPRO_SPDIF:
			printf("WMAPRO_SPDIF\n");
			break;
		case AudioDecoder_Codec_PCMX:
			printf("PCMX\n");
			break;
		default:
			printf("UNKNOWN (%#x)!!!\n", decoder->type);
			break;
	}
	
	printf("\t\t\tMRUA module ID: %#x\n",(unsigned int) (decoder->decoderID));

	printf("\t\t\tContent index: %d\n", decoder->contentIndex);
	printf("\t\t\t\tUsing alternate content: %s\n", decoder->useAltContent ? "Yes" : "No");
	printf("\t\t\t\tSPDIF using alternate content: %s\n", decoder->spdifAltContent ? "Yes" : "No");

	printf("\t\t\tFormat-specific parameters:\n");
	switch (GET_ELEMENTARY_MTYPE(decoder->params.type))
	{
		case MTYPE_ELEM_DRA:
			printf("\t\t\t\ttype: DRA\n");
			printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[dra->OutputSpdif & 0x0F], dra->OutputSpdif);
			printf("\t\t\t\tOutputDualMode: %d\n",		dra->OutputDualMode);
			printf("\t\t\t\tOutputChannels: %#x\n",		dra->OutputChannels);
			printf("\t\t\t\tOutputLfe: %d\n",			dra->OutputLfe);
			printf("\t\t\t\tOutputSurround20: %d\n",	dra->OutputSurround20);
			printf("\t\t\t\tBassMode: %#lx (%s)\n",		dra->BassMode, bassMode[dra->BassMode]);
			break;
		case MTYPE_ELEM_AAC:
			printf("\t\t\t\ttype: AAC\n");
	_aac:
			printf("\t\t\t\tInputFormat: %d\n",			aac->InputFormat);
			printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[aac->OutputSpdif & 0x0F], aac->OutputSpdif);
			printf("\t\t\t\tOutputDualMode: %d\n",		aac->OutputDualMode);
			printf("\t\t\t\tOutputChannels: %#x\n",		aac->OutputChannels);
			printf("\t\t\t\tOutputLfe: %d\n",			aac->OutputLfe);
			printf("\t\t\t\tOutputSurround20: %d\n",	aac->OutputSurround20);
			printf("\t\t\t\tBassMode: %#lx (%s)\n",		aac->BassMode, bassMode[aac->BassMode]);
			break;
		case MTYPE_ELEM_AACP:
			printf("\t\t\t\ttype: AAC Plus\n");
			goto _aac;

		case MTYPE_ELEM_AC3:
			printf("\t\t\t\ttype: AC3\n");
	_ac3:
			printf("\t\t\t\tOutputDualMode: %d\n",		ac3->OutputDualMode);
			printf("\t\t\t\tOutputChannels: %#x\n",		ac3->OutputChannels);
			printf("\t\t\t\tOutputLfe: %d\n",			ac3->OutputLfe);
			printf("\t\t\t\tOutputSurround20: %d\n",	ac3->OutputSurround20);
			printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[ac3->OutputSpdif & 0x0F], ac3->OutputSpdif);
			printf("\t\t\t\tAcmod2DualMode: %d\n",		ac3->Acmod2DualMode);
			printf("\t\t\t\tCompMode: %d\n",			ac3->CompMode);
			printf("\t\t\t\tDynScaleHi: %#lx\n",		ac3->DynScaleHi);
			printf("\t\t\t\tDynScaleLo: %#lx\n",		ac3->DynScaleLo);
			printf("\t\t\t\tPcmScale: %#lx\n",			ac3->PcmScale);
			printf("\t\t\t\tBassMode: %#lx (%s)\n",		ac3->BassMode, bassMode[ac3->BassMode]);
			break;
		case MTYPE_ELEM_DDP:
			printf("\t\t\t\ttype: DolbyDigital Plus\n");
			goto _ac3;

		case MTYPE_ELEM_DLLS:
			printf("\t\t\t\ttype: DolbyDigital Lossless\n");
			printf("\t\t\t\tOutputDualMode: %d\n",		trueHD->OutputDualMode);
			printf("\t\t\t\tOutputChannels: %#x\n",		trueHD->OutputChannels);
			printf("\t\t\t\tOutputLfe: %d\n",			trueHD->OutputLfe);
			printf("\t\t\t\tOutputSurround20: %d\n",	trueHD->OutputSurround20);
			printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[trueHD->OutputSpdif & 0x0F], trueHD->OutputSpdif);
			printf("\t\t\t\tBassMode: %#lx (%s)\n",		trueHD->BassMode, bassMode[trueHD->BassMode]);
			printf("\t\t\t\tChConfig: %#lx\n",			trueHD->Chconfig);
			printf("\t\t\t\tDRCenable: %#lx\n",			trueHD->DRCenable);
			printf("\t\t\t\tDRCboost: %#lx\n",			trueHD->DRCboost);
			printf("\t\t\t\tDRCcut: %#lx\n",			trueHD->DRCcut);
			printf("\t\t\t\tDRCdialref: %#lx\n",		trueHD->DRCdialref);
			break;

		case MTYPE_ELEM_DTS:
			printf("\t\t\t\ttype: DTS\n");
	_dts:
			printf("\t\t\t\tOutputDualMode: %d\n",		dts->OutputDualMode);
			printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[dts->OutputSpdif & 0x0F], dts->OutputSpdif);
			printf("\t\t\t\tOutputChannels: %#x\n",		dts->OutputChannels);
			printf("\t\t\t\tOutputLfe: %d\n",			dts->OutputLfe);
			printf("\t\t\t\tOutputSurround20: %d\n",	dts->OutputSurround20);
			printf("\t\t\t\tBassMode: %#lx (%s)\n",		dts->BassMode, bassMode[dts->BassMode]);
			if (decoder->type == AudioDecoder_Codec_DTS)
				printf("\t\t\t\tDTS CD mode: %s\n",		dts->dts_CD ? "ON" : "off");
			break;
		case MTYPE_ELEM_DTSHD:
			if (decoder->type == AudioDecoder_Codec_DTS)
				printf("\t\t\t\ttype: DTS-HD\n");
			else
				printf("\t\t\t\ttype: DTS-HD LBR\n");
			goto _dts;

//		case MTYPE_ELEM_MPA:
//			printf("\t\t\t\ttype: MPEG Audio\n");
//			goto _mpa;
		case MTYPE_ELEM_MP1:
			printf("\t\t\t\ttype: MPEG1 Layer 1-2 Audio\n");
	_mpa:
			printf("\t\t\t\tOutputDualMode: %d\n",		mpeg->OutputDualMode);
			printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[mpeg->OutputSpdif & 0x0F], mpeg->OutputSpdif);
			printf("\t\t\t\tOutputChannels: %#x\n",		mpeg->OutputChannels);
			printf("\t\t\t\tOutputLfe: %d\n",			mpeg->OutputLfe);
			printf("\t\t\t\tOutputSurround20: %d\n",	mpeg->OutputSurround20);
			printf("\t\t\t\tBassMode: %#lx (%s)\n",		mpeg->BassMode, bassMode[mpeg->BassMode]);
			break;
//		case MTYPE_ELEM_MP2:
//			printf("\t\t\t\ttype: MPEG1 Layer 2 Audio\n");
//			goto _mpa;
		case MTYPE_ELEM_MP3:
			printf("\t\t\t\ttype: MPEG1 Layer 3 (MP3) Audio\n");
			goto _mpa;
		case MTYPE_ELEM_MP2MC:
			printf("\t\t\t\ttype: MPEG2 MC Audio\n");
			goto _mpa;
			break;

		case MTYPE_ELEM_PCM:
			switch (GET_APPLICATION_MTYPE(decoder->params.type))
			{
				case MTYPE_APP_DVD:
				case MTYPE_APP_DVDVR:
					printf("\t\t\t\ttype: DVD-Video LPCM\n");
					printf("\t\t\t\tOutputDualMode: %d\n",		lpcmVOB->OutputDualMode);
					printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[lpcmVOB->OutputSpdif & 0x0F], lpcmVOB->OutputSpdif);
					printf("\t\t\t\tOutputChannels: %#x\n",		lpcmVOB->OutputChannels);
					printf("\t\t\t\tOutputLfe: %d\n",			lpcmVOB->OutputLfe);
					printf("\t\t\t\tOutputSurround20: %d\n",	lpcmVOB->OutputSurround20);
					printf("\t\t\t\tBassMode: %#lx (%s)\n",		lpcmVOB->BassMode, bassMode[lpcmVOB->BassMode]);
					printf("\t\t\t\tChannelAssign: %d\n",		lpcmVOB->ChannelAssign);
					printf("\t\t\t\tBitsPerSample: %ld\n",		lpcmVOB->BitsPerSample);
					printf("\t\t\t\tSamplingFrequency: %ld\n",	lpcmVOB->SamplingFrequency);
					printf("\t\t\t\tDownMix: %d\n",				lpcmVOB->DownMix);
//					printf("\t\t\t\tCoefLR: %ld\n",				lpcmVOB->CoefLR);
					break;
					
				case MTYPE_APP_DVDA:
					printf("\t\t\t\ttype: DVD-Audio LPCM\n");
					printf("\t\t\t\tOutputDualMode: %d\n",		lpcmAOB->OutputDualMode);
					printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[lpcmAOB->OutputSpdif & 0x0F], lpcmAOB->OutputSpdif);
					printf("\t\t\t\tOutputChannels: %#x\n",		lpcmAOB->OutputChannels);
					printf("\t\t\t\tOutputLfe: %d\n",			lpcmAOB->OutputLfe);
					printf("\t\t\t\tOutputSurround20: %d\n",	lpcmAOB->OutputSurround20);
					printf("\t\t\t\tBassMode: %#lx (%s)\n",		lpcmAOB->BassMode, bassMode[lpcmAOB->BassMode]);
					printf("\t\t\t\tChannelAssign: %d\n",		lpcmAOB->ChannelAssign);
					printf("\t\t\t\tBitsPerSampleGroup1: %ld\n",lpcmAOB->BitsPerSampleGroup1);
					printf("\t\t\t\tBitsPerSampleGroup2: %ld\n",lpcmAOB->BitsPerSampleGroup2);
					printf("\t\t\t\tSamplingFrequencyGroup1: %ld\n",	lpcmAOB->SamplingFrequencyGroup1);
					printf("\t\t\t\tSamplingFrequencyGroup2: %ld\n",	lpcmAOB->SamplingFrequencyGroup2);
					printf("\t\t\t\tDownMix: %d\n",			lpcmAOB->DownMix);
//					printf("\t\t\t\tCoefLR: %ld\n",				lpcmVOB->CoefLR);
					printf("\t\t\t\tPhaseLR: %ld\n",			lpcmAOB->PhaseLR);
					printf("\t\t\t\tGroup2Shift: %ld\n",		lpcmAOB->Group2Shift);
					break;
					
				case MTYPE_APP_CDA:
				case MTYPE_APP_NONE:
					printf("\t\t\t\ttype: CD-DA LPCM\n");
					printf("\t\t\t\tOutputDualMode: %d\n",		pcmCDA->OutputDualMode);
					printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[pcmCDA->OutputSpdif & 0x0F], pcmCDA->OutputSpdif);
					printf("\t\t\t\tOutputChannels: %#x\n",		pcmCDA->OutputChannels);
					printf("\t\t\t\tOutputLfe: %d\n",			pcmCDA->OutputLfe);
					printf("\t\t\t\tOutputSurround20: %d\n",	pcmCDA->OutputSurround20);
					printf("\t\t\t\tBassMode: %#lx (%s)\n",		pcmCDA->BassMode, bassMode[pcmCDA->BassMode]);
					printf("\t\t\t\tChannelAssign: %d\n",		pcmCDA->ChannelAssign);
					printf("\t\t\t\tBitsPerSample: %ld\n",		pcmCDA->BitsPerSample);
					printf("\t\t\t\tSamplingFrequency: %ld\n",	pcmCDA->SamplingFrequency);
					printf("\t\t\t\tMsbFirst: %d\n",			pcmCDA->MsbFirst);
					printf("\t\t\t\tSignedPCM: %d\n",			pcmCDA->SignedPCM);
					break;
					
				case MTYPE_APP_BRD:
				case MTYPE_APP_BDRE:
					printf("\t\t\t\ttype: Blu-ray LPCM\n");
					printf("\t\t\t\tOutputDualMode: %d\n",		lpcmBD->OutputDualMode);
					printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[lpcmBD->OutputSpdif & 0x0F], lpcmBD->OutputSpdif);
					printf("\t\t\t\tOutputChannels: %#x\n",		lpcmBD->OutputChannels);
					printf("\t\t\t\tOutputLfe: %d\n",			lpcmBD->OutputLfe);
					printf("\t\t\t\tOutputSurround20: %d\n",	lpcmBD->OutputSurround20);
					printf("\t\t\t\tBassMode: %#lx (%s)\n",		lpcmBD->BassMode, bassMode[lpcmBD->BassMode]);
					printf("\t\t\t\tChannelAssign: %d\n",		lpcmBD->ChannelAssign);
					printf("\t\t\t\tBitsPerSample: %ld\n",		lpcmBD->BitsPerSample);
					printf("\t\t\t\tSamplingFrequency: %ld\n",	lpcmBD->SamplingFrequency);
					break;
					
				case MTYPE_APP_BDJ:
					printf("\t\t\t\ttype: Interactive Audio LPCM\n");
					printf("\t\t\t\tOutputDualMode: %d\n",		PCMXParams->OutputDualMode);
					printf("\t\t\t\tOutputSpdif: %s (%#x)\n",	outputSPDIF[PCMXParams->OutputSpdif & 0x0F], PCMXParams->OutputSpdif);
					printf("\t\t\t\tOutputChannels: %#x\n",		PCMXParams->OutputChannels);
					printf("\t\t\t\tOutputLfe: %d\n",			PCMXParams->OutputLfe);
					printf("\t\t\t\tBassMode: %#lx (%s)\n",		PCMXParams->BassMode, bassMode[PCMXParams->BassMode]);
					printf("\t\t\t\tChannelAssign: %d\n",		PCMXParams->ChannelAssign);
					printf("\t\t\t\tBitsPerSample: %ld\n",		PCMXParams->BitsPerSample);
					printf("\t\t\t\tSamplingFrequency: %ld\n",	PCMXParams->SamplingFrequency);
					printf("\t\t\t\tMsbFirst: %d\n",			PCMXParams->MsbFirst);
					printf("\t\t\t\tSignedPCM: %d\n",			PCMXParams->SignedPCM);
					break;
					
				default:
					printf("\t\t\t\ttype: UNKNOWN!!!\n");
					break;
			}
			break;
	
		default:
			printf("\t\t\t\ttype: UNKNOWN!!!\n");
			break;
	}

	printf("\t\t\tMixer weights:\n"
		   "\t\t\t\t%#lx\t%#lx\t%#lx\t%#lx\n"
		   "\t\t\t\t%#lx\t%#lx\t%#lx\t%#lx\n",
		   decoder->mixWeight.MixerValue_ch0,
		   decoder->mixWeight.MixerValue_ch1,
		   decoder->mixWeight.MixerValue_ch2,
		   decoder->mixWeight.MixerValue_ch3,
		   decoder->mixWeight.MixerValue_ch4,
		   decoder->mixWeight.MixerValue_ch5,
		   decoder->mixWeight.MixerValue_ch6,
		   decoder->mixWeight.MixerValue_ch7);

	printf("\t\t\tUse shadow decoder: %s\n", decoder->UseShadowAudio ? "Yes" : "No");
}   

static void print_audio_engine_config(const struct SMRUAAudioEngineConfig *engine)
{
	int i;

	printf("\t\tMRUA module ID: %#x\n",(unsigned int) (engine->engineID));
	printf("\t\tSampling freq ");
	if (engine->sampleFrequencyFromStream)
		printf("(from stream)");
	printf(": %d\n",(unsigned int)(engine->sampleFrequency));
	printf("\t\tVolume per channel:\n");
	for (i=0;i<12;i++)
	{
		printf("\t\t\t %d: %#x\n",i, (unsigned int)(engine->volume[i]));
	}
	printf("\t\tDelay per channel:\n");
	for (i=0;i<8;i++)
	{
		printf("\t\t\t %d: %d\n",i,(unsigned int)engine->delay[i]);
	}

	if (engine->i2sEnable == AudioEngine_SerialOut_SO_ENABLE)
	{
		printf("\t\ti2S: ON\n");
		printf("\t\t\tDataAlignment: %d\n" 	,(unsigned int)(engine->i2sConfig.DataAlignment));
		printf("\t\t\tSClkInvert: %d\n"		,engine->i2sConfig.SClkInvert);
		printf("\t\t\tFrameInvert: %d\n"		,engine->i2sConfig.FrameInvert);
		printf("\t\t\tMSBFirst: %d\n"		,engine->i2sConfig.MSBFirst);
		printf("\t\t\tSampleSize16Bit: %d\n"	,engine->i2sConfig.SampleSize16Bit);
	}
	else if (engine->i2sEnable == AudioEngine_SerialOut_SO_DISABLE)
		printf("\t\ti2S: OFF\n");
	else
		printf("\t\ti2S: Invalid state !!!\n");
		
	switch (engine->spdifEnable)
	{
		case AudioEngine_SpdifOut_Disabled:
			printf("\t\tSPDIF: OFF\n");
			break;
			
		case AudioEngine_SpdifOut_ActiveData0:
			printf("\t\tSPDIF: ON (Data 0)\n");
			goto _spdif_config;
			break;
			
		case AudioEngine_SpdifOut_Active:
			printf("\t\tSPDIF: ON\n");
	_spdif_config:
			printf("\t\t\tStatus mask: %#x\n", (unsigned int)engine->spdifConfig.Mask);
			printf("\t\t\tStatus value: %#x\n",(unsigned int)engine->spdifConfig.Value);
			printf("\t\t\t\tUse: %s\n", GET_SPDIF_STATUS(engine->spdifConfig.Value, 0, 1) ? "?" : "Consumer");
			printf("\t\t\t\tType: %s\n", GET_SPDIF_STATUS(engine->spdifConfig.Value, 1, 1) ? "Bitstream" : "LPCM");
			printf("\t\t\t\tCopy Bit: %lu\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 2, 1));
			printf("\t\t\t\tEmphasis: %lu\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 3, 3));
			printf("\t\t\t\tMode: %lu\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 6, 1));
			printf("\t\t\t\tCategory: %#lx\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 8, 7));
			printf("\t\t\t\tL Bit: %lu\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 15, 1));
			printf("\t\t\t\tSource number: %lu\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 16, 4));
			printf("\t\t\t\tChannel number: %lu\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 20, 4));
			printf("\t\t\t\tSampling rate: %#lx\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 24, 4));
			printf("\t\t\t\tClock accuracy: %lu\n",GET_SPDIF_STATUS(engine->spdifConfig.Value, 28, 2));
			printf("\t\t\tBit width: %d\n",(unsigned int)engine->force16bitsSpdif ? 16 : 24);
			break;

		default:
			printf("\t\tSPDIF: Invalid state !!!\n");
			break;
	}
	printf("\t\tDownmixing done in: %s phase\n", engine->forcePostProcessingDownmixing ? "Post-processing" : "Decoding");
	printf("\t\tCenter channel up: %s\n", engine->centerUp ? "Yes" : "No");
	printf("\t\tMaster engine: %s\n", engine->MasterEngine ? "Yes" : "No");
}

static void print_content_info(const struct SContentInfo *content)
{
	int i;

	static char *groupName[] =
	{
//								GRID_First = 0,
		"free",				//	GRID_Free = GRID_First,     ///< not allocated, free to use
		"Main",				//	GRID_Main,                  ///< main AV presentation
		"Secondary",		//	GRID_Secondary,             ///< secondary AV presentation (PIP)
		"Interactive",		//	GRID_Interactive,         	///< interactive audio presentation
		"Deinterlacing",	//	GRID_Deinterlacing,         ///< deinterlacing resource
		"Graphics",			//	GRID_Graphics,              ///< graphics device
		"SecondaryGraphics",//	GRID_SecondaryGraphics,     ///< secondary graphics device
		"GFXEngineSlave",	//	GRID_GFXEngineSlave,        ///< for GFX multiscaler when used as a scaling companion in a stretch operation
		"SubpicSlave",		//	GRID_SubpicSlave,      		///< for SPU multiscaler when used in subpicture mode
		"Background",		//	GRID_Background,            ///< for the DFB background plane
		"N/A",		//	GRID_NotAvailable,          ///< not available in current configuration
	};
	
	printf("\t\tGroup ID: %s\n", content->groupID < GRID_Last ? groupName[content->groupID] : "invalid");
	printf("\t\ttype: ");
	switch (GET_ELEMENTARY_MTYPE(content->format.mediaType))
	{
		case MTYPE_ELEM_DRA:
			printf("DRA %s\n", content->format.bitrateType == HighBitrate ? "Extension" : "");
			goto _audio;
		case MTYPE_ELEM_AAC:
			printf("AAC\n");
			goto _audio;
		case MTYPE_ELEM_AACP:
			printf("AAC Plus\n");
			goto _audio;
		case MTYPE_ELEM_AC3:
			printf("AC3\n");
			goto _audio;
		case MTYPE_ELEM_DDP:
			printf("DolbyDigital Plus\n");
			goto _audio;
		case MTYPE_ELEM_DLLS:
			printf("DolbyDigital Lossless\n");
			goto _audio;
		case MTYPE_ELEM_DTS:
			printf("DTS\n");
			goto _audio;
		case MTYPE_ELEM_DTSHD:
			printf("DTS-HD %s\n", content->format.bitrateType == HighBitrate ? "MA" : (content->format.bitrateType == LowBitrate ? "LBR" : ""));
			goto _audio;
//		case MTYPE_ELEM_MPA:
//			printf("MPEG Audio\n");
//			goto _audio;
		case MTYPE_ELEM_MP1:
			printf("MPEG1 Layer 1-2 Audio\n");
			goto _audio;
//		case MTYPE_ELEM_MP2:
//			printf("MPEG1 Layer 2 Audio\n");
//			goto _audio;
		case MTYPE_ELEM_MP3:
			printf("MPEG1 Layer 3 (MP3) Audio\n");
			goto _audio;
		case MTYPE_ELEM_MP2MC:
			printf("MPEG2 MC Audio\n");
	_audio:
			if (content->format.formatValid)
			{
				static char *chAssign[] =
				{
				    "none",			// CCf_none,           	
				    "1/0/0",		// CCf_C,				
				    "2/0/0",		// CCf_LR,             	
				    "3/0/0",		// CCf_LCR,				
				    "2/2/0",		// CCf_LRSlSr,			
				    "3/2/0",		// CCf_LCRSlSr,			
				    "2/2/2",		// CCf_LRSlSrBlBr,		
				    "3/2/2",		// CCf_LCRSlSrBlBr,		
				    "invalid",
				    "invalid",
				    "invalid",
				    "invalid",
				    "invalid",
				    "invalid",
				    "invalid",
				    "invalid",
				    "1+1/0/0",		// CCf_DualMono = 0x10,
				    "1/1/0",		// CCf_CS,
				    "2/1/0",		// CCf_LRS,
				    "3/1/0",		// CCf_LCRS,
				    "invalid",
				    "3/0/1",		// CCf_LCRB
				    "2/0/2",		// CCf_LRBlBr
				    "3/0/2",		// CCf_LCRBlBr
				    "2/2/1",		// CCf_LRSlSrB
				    "3/2/1",		// CCf_LCRSlSrB
				};

				printf("\t\tchannels: %d.%d\n", content->format.format.sound.channels, content->format.format.sound.lfe);
				printf("\t\tchannel assignment: %s (%#x)\n",
					(unsigned int)content->format.format.sound.channelAssignment < sizeof(chAssign)/sizeof(chAssign[0])
						? chAssign[content->format.format.sound.channelAssignment]
						: "invalid",
					content->format.format.sound.channelAssignment);
				printf("\t\trate: %ld\n", content->format.format.sound.samplingFreq);

				if (content->format.format.sound.alt.mediaType)
				{
					printf("\t\tAlternate type: ");
					switch (GET_ELEMENTARY_MTYPE(content->format.format.sound.alt.mediaType))
					{
						case MTYPE_ELEM_AC3:
							printf("AC3\n");
							break;

						case MTYPE_ELEM_DTS:
							printf("DTS\n");
							break;

						default:
							printf("%#X\n", (unsigned)content->format.format.sound.alt.mediaType);
							break;
					}
					printf("\t\t\tchannels: %d.%d\n", content->format.format.sound.alt.channels, content->format.format.sound.alt.lfe);
					printf("\t\t\trate: %ld\n", content->format.format.sound.alt.samplingFreq);
				}
			}
			else
				printf("\t\tno details available\n");

			printf("\t\tDecoder info%s\n", content->decoderInfo && content->decoderInfo->isValid ? (content->decoderInfo->isDirty ? " (dirty):" : ":") : " - unavailable");
			if (content->decoderInfo && content->decoderInfo->isValid)
			{
				for (i=0; i<RMCFG_AUDIO_ENGINE_CNT; i++)
				{
					if (content->decoderInfo->params.audio[i].CodecID)
					{
						static char *chAssign[] =
						{
							"1+1/0/0",		// CCf_DualMono
							"1/0/0",		// CCf_C
							"2/0/0",		// CCf_LR
							"3/0/0",		// CCf_LCR
							"2/1/0",		// CCf_LRS
							"3/1/0",		// CCf_LCRS
							"2/2/0",		// CCf_LRSlSr
							"3/2/0",		// CCf_LCRSlSr
							"3/2/2",		// CCf_LCRSlSrBlBr
							"3/0/2",		// CCf_LCRBlBr	
							"3/2/1",		// CCf_LCRSlSrB
							"2/2/2",		// CCf_LRSlSrBlBr
							"3/0/1",		// CCf_LCRB
							"2/0/2",		// CCf_LRBlBr
							"2/2/1",		// CCf_LRSlSrB
							"unsupported",
						};
		
						printf("\t\t\tOn audio engine %d:\n", i);
						printf("\t\t\t\tsample rate: %d\n", (int)content->decoderInfo->params.audio[i].SampleRate);
						printf("\t\t\t\tchannels: %d.%d\n", (int)content->decoderInfo->params.audio[i].ChannelNumber, (int)content->decoderInfo->params.audio[i].lfe);
						printf("\t\t\t\tchannel assignment: %s (%#x)\n",
								(unsigned int)content->decoderInfo->params.audio[i].ChannelAssignment < sizeof(chAssign)/sizeof(chAssign[0])
									? chAssign[content->decoderInfo->params.audio[i].ChannelAssignment]
									: "invalid",
								content->decoderInfo->params.audio[i].ChannelAssignment);
					}
				}
			}
			break;

		case MTYPE_ELEM_PCM:
			switch (GET_APPLICATION_MTYPE(content->format.mediaType))
			{
				case MTYPE_APP_DVD:
					printf("DVD-Video LPCM\n");
					goto _pcm;
				case MTYPE_APP_DVDVR:
					printf("DVDVR-Video LPCM\n");
					goto _pcm;
				case MTYPE_APP_DVDA:
					printf("DVD-Audio LPCM\n");
					goto _pcm;
				case MTYPE_APP_CDA:
				case MTYPE_APP_NONE:
					printf("CD-DA LPCM\n");
					goto _pcm;
				case MTYPE_APP_BRD:
				case MTYPE_APP_BDRE:
					printf("Blu-ray LPCM\n");
					goto _pcm;
				case MTYPE_APP_BDJ:
					printf("Interactive Audio LPCM\n");
			_pcm:
					if (content->format.formatValid)
					{
					    printf("\t\tformat: %s\n", content->format.format.pcm.storage.format == 1 ? "PCM" : (content->format.format.pcm.storage.format == 2 ? "LPCM" : "INVALID"));
					    printf("\t\tbits per sample: %d\n", content->format.format.pcm.storage.bitsPerSample);
					    printf("\t\tsamples per channel: %d\n", content->format.format.pcm.storage.samplesPerChannel);
					    printf("\t\tMSB first: %d\n", content->format.format.pcm.storage.MsbFirst);
					}
				    goto _audio;
				default:
					printf("UNKNOWN PCM type (%lx) !!!\n", GET_APPLICATION_MTYPE(content->format.mediaType));
					break;
			}
			break;
	
		case MTYPE_ELEM_MPEG1:
			printf("MPEG1 video\n");
			goto _video;
		case MTYPE_ELEM_MPEG2:
			printf("MPEG2 video\n");
			goto _video;
		case MTYPE_ELEM_MPEG4:
			printf("MPEG4 video\n");
			goto _video;
		case MTYPE_ELEM_AVC:
			printf("AVC video\n");
			goto _video;
		case MTYPE_ELEM_VC1:
			printf("VC1 video\n");
			goto _video;
		case MTYPE_ELEM_JPEG:
			printf("JPEG\n");
	_video:
			if (content->format.formatValid)
			{
				printf("\t\twidth: %d\n", content->format.format.image.width);
				printf("\t\theight: %d\n", content->format.format.image.height);
				printf("\t\taspectX: %d\n", content->format.format.image.aspectX);
				printf("\t\taspectY: %d\n", content->format.format.image.aspectY);

				printf("\t\taspectMatchCaps:");
				if (content->format.format.image.aspectMatchCaps)
				{
					for (i=1; i < (int)(sizeof(aspectRatioMatch)/sizeof(aspectRatioMatch[0])); i++)
						if (content->format.format.image.aspectMatchCaps & ASPECT_MATCH_TO_BITMASK((enum EAspectRatioMatch)i))
							printf(" %s", aspectRatioMatch[i]);
				}
				else
					printf(aspectRatioMatch[0]);
				printf("\n");

				printf("\t\tinterlaced: %d\n", content->format.format.image.interlaced);
				printf("\t\ttelecine: %d\n", content->format.format.image.telecine);
				printf("\t\trateM: %d\n", content->format.format.image.rateM);
				printf("\t\trateN: %d\n", content->format.format.image.rateN);
				printf("\t\tcolorSpace: %s\n", colorSpace[content->format.format.image.colorSpace]);
			}
			else
				printf("\t\tno details available\n");

			printf("\t\tDecoder info%s\n", content->decoderInfo && content->decoderInfo->isValid ? (content->decoderInfo->isDirty ? " (dirty):" : ":") : " - unavailable");
			if (content->decoderInfo && content->decoderInfo->isValid)
			{
				printf("\t\t\twidth: %d\n", content->decoderInfo->params.video.generic.width);
				printf("\t\t\theight: %d\n", content->decoderInfo->params.video.generic.height);
				printf("\t\t\taspect ratio: %d:%d\n", content->decoderInfo->params.video.generic.aspectX, content->decoderInfo->params.video.generic.aspectY);
				printf("\t\t\tembedded 16:9 image: %s\n", content->decoderInfo->params.video.generic.aspectMatchCaps & ASPECT_MATCH_TO_BITMASK(ARM_Crop) ? "Yes" : "No");
			}
			break;
	
		default:
			printf("UNKNOWN (%lx) !!!\n", content->format.mediaType);
			break;
	}
}

static char connectionStatus[][32] = 
{
	"Unknown",
	"Connected",
	"INVALID",
	"Connected and Secure",
	"Disconnected"
};

static char toggle[][16] = 
{
	"Auto",
	"Off",
	"On",
};

static void print_digital_audio_settings(const struct SDigitalAudioConfig *setting)
{
	static char format[][16] =
	{
		"PCM",
		"AC3",
		"MP1-2",
		"MP3",
		"MP2MC",
		"AAC",
		"DTS",
		"ATRAC",
		"OneBit",
		"DD+",
		"DTSHD",
		"TrueHD",
		"DST",
		"WMAPRO",
		"WMA",
		"AAC+",
		"AAC+ v2",
		"MPEG Srnd",
		"DRA",
	};
	static int rate[] =
	{
		0,
		8000,
		11025,
		16000,
		22050,
		32000,
		44100,
		48000,
		88200,
		96000,
		176400,
		192000,
	};
	
	unsigned int i, j;
	
	printf("\t\t\tstatus: %s\n", connectionStatus[setting->status]);
	if (setting->status != CS_Disconnected)
	{
		for (i=0; i<sizeof(setting->formatParamTab)/sizeof(setting->formatParamTab[0]); i++)
		{
			if (setting->formats & AUDIO_MTYPE_TO_BITMASK(i+MTYPE_ELEM_AUDIO_FIRST))
			{
				printf("\t\t\t%s %s\n", format[i], 
						(i == MTYPE_ELEM_DTSHD - MTYPE_ELEM_AUDIO_FIRST) && (setting->formatParamTab[i].maxBitrate > 6500000)
						? "MA"
						: "");
					
				if (setting->formatParamTab[i].samplingRates)
				{
					printf("\t\t\t\tSampling rates:");
					for (j=1; j<sizeof(rate)/sizeof(rate[0]); j++)
						if (setting->formatParamTab[i].samplingRates & ASRATE_TO_BITMASK(j))
							printf(" %d", rate[j]);
					printf("\n");
				}
				if (setting->formatParamTab[i].maxBitrate)
				{
					if (i)
						printf("\t\t\t\tMax bitrate: %d\n", setting->formatParamTab[i].maxBitrate);
					else
					{
						static char sampleSize[][16] = 
						{
							"none",
							"16",
							"20",
							"16 20",
							"24",
							"16 24",
							"20 24",
							"16 20 24",
						};
						
						printf("\t\t\t\tSample size(s): %s bits\n", sampleSize[setting->formatParamTab[i].maxBitrate & 7]);
					}
				}
			}
		}
		printf("\t\t\tPCM channels: %d\n", setting->pcmChannels);
		printf("\t\t\tPCM rate: %d\n", setting->pcmRate);
		printf("\t\t\tOriginal bitstream: %s\n", setting->preserveOriginalBitstream ? "Yes" : "No");
		printf("\t\t\tOriginal quality: %s\n", setting->preserveOriginalQuality ? "Yes" : "No");
	}
}

static void print_speaker_settings(const struct SSpeakerConfig *setting)
{
    printf("\t\t\tspeakers: %d\n", setting->speakers);
    printf("\t\t\tlfe: %d\n", setting->lfe);
    printf("\t\t\tdelay: %d %d %d %d %d %d %d %d\n", setting->delay[0], setting->delay[1], setting->delay[2], setting->delay[3], setting->delay[4], setting->delay[5], setting->delay[6], setting->delay[7]);
    printf("\t\t\tvolume: %#lx %#lx %#lx %#lx %#lx %#lx %#lx %#lx\n", setting->volume[0], setting->volume[1], setting->volume[2], setting->volume[3], setting->volume[4], setting->volume[5], setting->volume[6], setting->volume[7]);
    printf("\t\t\tsurroundMode: %d\n", setting->surroundMode);
    printf("\t\t\tbassMode: %s\n", bassMode[setting->bassMode]);
};

void print_audio_settings(const struct SMRUAAudioOutConfig *settings)
{
	static char drcMode[][16] = 
	{
		"Off",
		"Auto",
		"Custom",
		"Max",
	};
	
	static char dualMode[][16] = 
	{
	    "Stereo",
	    "LeftMono,",
	    "RightMono",
	    "MixMono",
	};
	
    printf("\tAudio:\n");
    printf("\t\tHDMI\n");
    print_digital_audio_settings(&settings->common.hdmi.bitstream);
	if (settings->common.hdmi.bitstream.status != CS_Disconnected)
	{
	    print_speaker_settings(&settings->common.hdmi.output);
	    printf("\t\t\tengines: %#lx\n", settings->hdmiEngines);
	    printf("\t\t\tI2S capable engines: %#lx\n", settings->hdmiI2sEngines);
	    printf("\t\t\tSPDIF capable engines: %#lx\n", settings->hdmiSpdifEngines);
	}
    printf("\t\tAnalog\n");
	printf("\t\t\tstatus: %s\n", connectionStatus[settings->common.multiChannel.status]);
	if (settings->common.multiChannel.status != CS_Disconnected)
	{	
	    print_speaker_settings(&settings->common.multiChannel.output);
	    printf("\t\t\tengines: %#lx\n", settings->analogEngines);
	}
	printf("\t\tStereo\n");
	printf("\t\t\tstatus: %s\n", connectionStatus[settings->common.stereo.status]);
	if (settings->common.stereo.status != CS_Disconnected)
	{
	    printf("\t\t\tvolume: %#lx %#lx\n", settings->common.stereo.volume[0], settings->common.stereo.volume[1]);
	    printf("\t\t\tengines: %#lx\n", settings->stereoEngines);
	}
	printf("\t\tSPDIF\n");
    print_digital_audio_settings(&settings->common.spdif.bitstream);
	if (settings->common.spdif.bitstream.status != CS_Disconnected)
	{
	    print_speaker_settings(&settings->common.spdif.output);    
	    printf("\t\t\tengines: %#lx\n", settings->spdifEngines);
	}
	printf("\t\tDRC\n");
    printf("\t\t\tmode: %s\n", drcMode[settings->common.drc.mode]);
    printf("\t\t\tboost: %#lx\n", (long)settings->common.drc.boost);
    printf("\t\t\tcut: %#lx\n", (long)settings->common.drc.cut);
    printf("\t\t\tpartial compression: %#lx\n", (long)settings->common.drc.partComp);
    printf("\t\t\tdialog normalization: %#lx\n", (long)settings->common.drc.dialogRef);
    printf("\t\tDual mode: %s\n", dualMode[settings->common.dualMode]);
    printf("\t\taudio delay: %ld (%ld)\n", settings->audioDelay.time, settings->audioDelay.time_resolution);

};

static void print_video_decoder_config(const struct SMRUAVideoDecoderConfig *decoder)
{
	printf("\t\tMRUA type: ");
	switch(decoder->type)
	{
		case VideoDecoder_Codec_MPEG2_SD:
			printf("MPEG2 SD\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_DVD:
			printf("MPEG2 DVD\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_HD:
			printf("MPEG2 HD\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_SD:
			printf("MPEG42 SD\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_SD_Padding:
			printf("MPEG4 SD padding\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_HD:
			printf("MPEG2 HD\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_HD_Padding:
			printf("MPEG2 HD padding\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_SD_Packed:
			printf("MPEG2 SD packed\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_DVD_Packed:
			printf("MPEG2 DVD packed\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_HD_Packed:
			printf("MPEG2 HD packed\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_SD_Packed:
			printf("MPEG4 SD packed\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_HD_Packed:
			printf("MPEG4 HD packed\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_SD_DeInt:
			printf("MPEG2 SD deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_DVD_DeInt:
			printf("MPEG2 SD deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_HD_DeInt:
			printf("MPEG2 SD deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_SD_DeInt:
			printf("MPEG2 SD deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_SD_DeInt_Padding:
			printf("MPEG2 SD deinterlaced padding\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_HD_DeInt:
			printf("MPEG2 SD deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_HD_DeInt_Padding:
			printf("MPEG2 SD deinterlaced padding\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_SD_Packed_DeInt:
			printf("MPEG2 SD packed deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_DVD_Packed_DeInt:
			printf("MPEG2 SD packed deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG2_HD_Packed_DeInt:
			printf("MPEG2 SD packed deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_SD_Packed_DeInt:
			printf("MPEG2 SD packed deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_MPEG4_HD_Packed_DeInt:
			printf("MPEG2 SD packed deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_WMV_SD:
			printf("WMV SD\n");
			break;
			
		case VideoDecoder_Codec_WMV_816P:
			printf("WMV 816P\n");
			break;
			
		case VideoDecoder_Codec_WMV_HD:
			printf("WMV HD\n");
			break;
			
		case VideoDecoder_Codec_DIVX3_SD:
			printf("DIVX3 SD\n");
			break;
			
		case VideoDecoder_Codec_DIVX3_HD:
			printf("DIVX3 HD\n");
			break;
			
		case VideoDecoder_Codec_DIVX3_SD_Packed:
			printf("DIVX3 SD packed\n");
			break;
			
		case VideoDecoder_Codec_DIVX3_HD_Packed:
			printf("DIVX3 HD packed\n");
			break;
			
		case VideoDecoder_Codec_H264_SD:
			printf("H264 SD\n");
			break;
			
		case VideoDecoder_Codec_H264_HD:
			printf("H264 HD\n");
			break;
			
		case VideoDecoder_Codec_H264_SD_DeInt:
			printf("H264 SD deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_H264_HD_DeInt:
			printf("H264 HD deinterlaced\n");
			break;
			
		case VideoDecoder_Codec_VC1_SD:
			printf("VC1 SD\n");
			break;
			
		case VideoDecoder_Codec_VC1_HD:
			printf("VC1 HD\n");
			break;
			
		default:
			printf("Unknown\n");
			break;
	}
	printf("\t\tMRUA engine ID: %#lx\n",decoder->engineID);
	printf("\t\tMRUA module ID: %#lx\n",decoder->decoderID);
	printf("\t\tMRUA surface ID: %#lx\n",decoder->surfaceID);
//	printf("\t\tMRUA picture ID: %#lx\n",(long)decoder->currentPic);
	printf("\t\tContent index: %d\n", decoder->contentIndex);
}

static void print_video_config(const struct SVideoOutConfigModule *config)
{
	static char tvSystem[][16] =
	{
		"Unknown",
		"VESA",
		"CVT",
		"SECAM",
		"PAL",
		"NTSC",
		"HDTV50",
		"HDTV60",
	};
	
	static char tvSignal[][16] =
	{
	    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	    //
	    // !!!!! Order here must match that of the DCC-HD signal definitions in cdfs_av.h !!!!!!!!!!!!
	    //
	    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		"Unknown",
		"NTSC",
		"NTSC_J",
		"SECAM",
		"PAL",
		"PAL_M",
		"PAL_60",
		"SDTV",
		"EDTV",
		"640x480",
		"800x600",
		"1024x768",
	 	"1152x864",
		"720p",
	    "1280x768",
	 	"1280x960",
	 	"1280x1024",
	    "1360x768",
	    "1366x768",
	 	"1400x1050",
	    "1440x900",
	 	"1600x1200",
	    "1680x1050",
	    "1792x1344",
	    "1856x1392",
		"1080i",
		"1080p24",
		"1080p",
	    "1920x1200",
	    "1920x1440",
		"EDID",
	};
		
	static char tvAspectRatio[][16] =
	{
		"Unknown",
		"4x3",
		"16x9",
	};
	
	static char ccType[][16] =
	{
		"Unknown",
		"608",
		"708",
	};
	
	static char dexingType[][24] =
	{
		"None",
			"Weave",
			"Adaptive",
			"Reverse TeleCine",
			"Progressive",
			"Adaptive - 2 fields",
	};	
	unsigned i;
	
	printf("\t\t\tstatus: %s\n", connectionStatus[config->status]);
	if (config->status != CS_Disconnected)
	{
		printf("\t\t\t\033[36mTV system(s):");
		if (config->display.system)
		{
			for (i=1; i < sizeof(tvSystem)/sizeof(tvSystem[0]); i++)
				if (config->display.system & TV_SYSTEM_TO_BITMASK((enum ETVSystem)i))
					printf(" %s", tvSystem[i]);
		}
		else
			printf(" %s", tvSystem[0]);
		printf("\033[0m\n");
		
		printf("\t\t\t\033[36mTV signal(s) (* = preferred):");
		if (config->display.signal)
		{
			for (i=1; i < sizeof(tvSignal)/sizeof(tvSignal[0]); i++)
				if (config->display.signal & TV_SIGNAL_TO_BITMASK((enum ETVSignal)i))
				{
					printf(" %s", tvSignal[i]);
					
					if (config->display.preferred_signal & TV_SIGNAL_TO_BITMASK((enum ETVSignal)i))
						printf("*");
				}
		}
		else
			printf(" %s", tvSignal[0]);
		if (config->display.unsupportedHDTV50Signals)
		{
			printf("\n\t\t\t\tUnsupported HDTV50 signal(s):");
			for (i=1; i < sizeof(tvSignal)/sizeof(tvSignal[0]); i++)
				if (config->display.unsupportedHDTV50Signals & TV_SIGNAL_TO_BITMASK((enum ETVSignal)i))
				{
					printf(" %s", tvSignal[i]);
				}
		}
		if (config->display.unsupportedHDTV60Signals)
		{
			printf("\n\t\t\t\tUnsupported HDTV60 signal(s):");
			for (i=1; i < sizeof(tvSignal)/sizeof(tvSignal[0]); i++)
				if (config->display.unsupportedHDTV60Signals & TV_SIGNAL_TO_BITMASK((enum ETVSignal)i))
				{
					printf(" %s", tvSignal[i]);
				}
		}
		printf("\033[0m\n");
		
		printf("\t\t\taspect ratio: %s\n", tvAspectRatio[config->display.aspectRatio]);
		printf("\t\t\tcolor space: %s\n", colorSpace[config->display.colorSpace]);
		printf("\t\t\tCC type: %s\n", ccType[config->display.ccType]);
		printf("\t\t\taspect ratio match: %s\n", aspectRatioMatch[config->arMatch]);
		printf("\t\t\tdeinterlacing type: %s\n", dexingType[config->dexType]);
		printf("\t\t\tCC status: %s\n", toggle[config->cc]);
	}
};


void print_video_settings(const struct SMRUAVideoOutConfig *setting)
{
    printf("\tVideo:\n");
    printf("\t\tHDMI\n");
    print_video_config(&setting->common.hdmi);
	if (setting->common.hdmi.status != CS_Disconnected)
	    printf("\t\t\tblocks: %#lx\n", setting->hdmiOutputBlocks);
    printf("\t\tComponent\n");
    print_video_config(&setting->common.component);
	if (setting->common.component.status != CS_Disconnected)
	    printf("\t\t\tblocks: %#lx\n", setting->componentOutputBlocks);
    printf("\t\tComposite\n");
    print_video_config(&setting->common.composite);
	if (setting->common.composite.status != CS_Disconnected)
	    printf("\t\t\tblocks: %#lx\n", setting->compositeOutputBlocks);
}

static void print_configuration(char *prefix, const struct SMRUAConfig *configuration)
{
    int i,j;

    printf("%s output configuration:\n", prefix);
	if (configuration->audio.engineCnt)
		print_audio_settings(&configuration->output.audio);
	print_video_settings(&configuration->output.video);
       
    printf("%s MRUA configuration:\n", prefix);
    if (configuration->audio.engineCnt)
    	printf("\tAudio mixing mode: %s\n", configuration->audio.InterEngineMixing ? "inter-engine" : "intra-engine/none");
	for (i=0; i<configuration->audio.engineCnt; i++)
	{
	    printf("\tAudio engine %d:\n", i);
	    print_audio_engine_config(&configuration->audio.engine[i]);
	    
	    // decoder table is sparse
        for (j=0; j</*configuration->audio.engine[i].decoderCnt */RMCFG_AUDIO_DECODER_CNT; j++)
		{
			if ( (int)configuration->audio.engine[i].decoder[j].decoderID > 0 )
			{
				printf("\t\tAudio decoder %d:\n", j);
				print_audio_decoder_config(&configuration->audio.engine[i].decoder[j]);
			}
		}
	}

    // decoder table is sparse
	for (i=0; i</*configuration->video.decoderCnt*/RMCFG_VIDEO_DECODER_CNT; i++)
	{
		if ( (int)configuration->video.decoder[i].decoderID > 0 )
		{
		    printf("\tVideo decoder %d:\n", i);
		    print_video_decoder_config(&configuration->video.decoder[i]);
		}
	}
}

void print_drm_settings(const struct rmcci *drm)
{
	printf("DRM Restrictions:\n");
	printf("\tAnalogAudio_disable: %d\n",drm->AnalogAudio_disable);
	printf("\tDigitalCompressedAudio_disable: %d\n",drm->DigitalCompressedAudio_disable);
	printf("\tDigitalUncompressedAudio_disable: %d\n",drm->DigitalUncompressedAudio_disable);
	printf("\tSPDIF_PCMLimit: %d\n",drm->SPDIF_PCMLimit);
	printf("\tSPDIF_CpBit: %d\n",drm->SPDIF_CpBit);
	printf("\tSPDIF_LBit: %d\n",drm->SPDIF_LBit);
	printf("\tAnalogVideo_disable: %d\n",drm->AnalogVideo_disable);
	printf("\tVideo_agc: %ld\n",drm->Video_agc);
	printf("\tVideo_CGMSA: %ld\n",drm->Video_CGMSA);
	printf("\tDigitalVideo_disable: %d\n",drm->DigitalVideo_disable);
	printf("\tAnalogVideo_imageConstraint: %d\n",drm->AnalogVideo_imageConstraint);
	printf("\tHDCP_Enable: %d\n",drm->HDCP_Enable);
	printf("\tVideo_aps: %ld\n",drm->Video_aps);
	printf("\tVideo_rcd: %d\n",drm->Video_rcd);
}	

// A/V configuration preview callback	
bool ConfigPreview(const struct RUA *pRUA,
				  const struct SContentInfo *content,
				  const struct rmcci *drm,
				  const struct SMRUAOutputConfig *settings,
				  const struct SAdjustment *adjustments,
				  struct SMRUAConfig *configuration)
{
   	int i;
	bool hasContent = false;

	for (i=0; i<CONTENT_TABLE_LENGTH; i++)
		if (content[i].groupID != GRID_Free)
		{
			if (!hasContent)
				printf("Content info:\n");
			hasContent = true;
			printf("\tContent index: %d\n", i);
			print_content_info(content+i);
		}
	if (!hasContent)
		printf("\tNo content available:\n");
	
	print_drm_settings(drm);
	
    printf("Settings:\n");
	print_audio_settings(&settings->audio);
	print_video_settings(&settings->video);
    printf("\nGlobal adjustments:\n"
           "\tVolume: %#08lX\n"
           "\tMute: %s\n",
           (adjustments->type == ADJ_VOLUME) || (adjustments->type == (ADJ_VOLUME + ADJ_MUTE)) ? adjustments->value.volume : (UINT32)-1,
           adjustments->type == ADJ_MUTE
			   ? (adjustments->value.mute ? "ON" : "OFF")
       		   : (adjustments->type == (ADJ_VOLUME + ADJ_MUTE)) ? "ON" : "?"
		  );


#if 0    
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
					configuration->output.video.common.hdmi.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60);
					configuration->output.video.common.component.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV60);
					configuration->output.video.common.composite.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_NTSC);
					configuration->output.video.common.composite.display.signal = TV_SIGNAL_TO_BITMASK(TVS_NTSC);
				}
				else if ((rateM == rateN * 25 || rateM == rateN * 50) &&
						 (settings->video.common.hdmi.display.system & TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50)) != 0)
				{
					printf("\n\n\nForced HDTV60 mode for %5.2fHz content !!!\n\n\n", (float)rateM / rateN);
					configuration->output.video.common.hdmi.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50);
					configuration->output.video.common.component.display.system = TV_SYSTEM_TO_BITMASK(TVSYS_HDTV50);
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
			    		configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
			    		configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_601;
			    		configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
			    		configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
			    	}
			    }
			    else
			    {
					if ((configuration->output.video.common.hdmi.status != CS_Disconnected && configuration->output.video.common.hdmi.display.signal != TV_SIGNAL_TO_BITMASK(TVS_EDTV)) ||
						configuration->output.video.common.component.display.signal != TV_SIGNAL_TO_BITMASK(TVS_EDTV))
			    	{
			    		printf("\n\n\nForced EDTV mode for progressive SD format!!!\n\n\n");
			    		configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_EDTV);
			    		configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_601;
			    		configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_EDTV);
			    		configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
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
		    		configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080p);
		    		configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_709;
		    		configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
		    		configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
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
		    		configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080p24);
		    		configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_709;
		    		configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_SDTV);
		    		configuration->output.video.common.component.display.colorSpace = CS_YUV_601;
		    	}
		    }
		    else
		    {
		    	if (configuration->output.video.common.component.display.signal != TV_SIGNAL_TO_BITMASK(TVS_1080i))
		    	{
		    		printf("\n\n\nForced 1080i mode for interlaced HD format!!!\n\n\n");
		    		configuration->output.video.common.hdmi.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080i);
		    		configuration->output.video.common.hdmi.display.colorSpace = CS_YUV_709;
		    		configuration->output.video.common.component.display.signal = TV_SIGNAL_TO_BITMASK(TVS_1080i);
		    		configuration->output.video.common.component.display.colorSpace = CS_YUV_709;
		    	}
		    }
		}
	}
#endif
	
	print_configuration("Proposed", configuration);

	return true;
}									  

// A/V configuration confirmation callback	
bool ConfigConfirm(const struct RUA *pRUA,
				  const struct SContentInfo *content,
				  const struct SMRUAConfig *configuration)
{
    print_configuration("Confirmed", configuration);
	return true;
}


static int AVAccessCount[4] = {0,};

bool AVAccessRequest(int dataPathIndex, char *fileName, UINT64 fileOffset, UINT32 dataAmount, UINT32 bufferFullness, UINT32 maxBitRate)
{
	printf("Cnt: %d, Path: %d, File: %s, Offset: %#llX, Amount: %ld KB, Fullness: %ld KB, Bitrate: %ld Mb/s\n",
			AVAccessCount[dataPathIndex]++, dataPathIndex, fileName, fileOffset, dataAmount>>10, bufferFullness>>10, maxBitRate>>20);
	
	return true;
}

void AVAccessNotify(int dataPathIndex, long bufferEmptyness)
{
	if (bufferEmptyness == LONG_MIN)
		printf("Cnt: %d, Path: %d, End of Media Access\n",
				--AVAccessCount[dataPathIndex], dataPathIndex);
	else
		printf("Cnt: %d, Path: %d, Emptyness: %ld KB\n",
				--AVAccessCount[dataPathIndex], dataPathIndex, bufferEmptyness >> 10);
}

