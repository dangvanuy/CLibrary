/*****************************************
 Copyright  2008
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/
/**
* @file   dtv_cmd.c
* @brief  DTV presentation engine commands

* @author Jake Lee, Kshitij Shah
* @date   2008-12-07
*/

#include "dtv_cmd.h"
#include "mediaconfig_dump.h"
#include "mediaplaylist.h"




/**
* 
* <b>FUNCTION: </b>
*
* Declare extern variables 
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
*/

extern union UMSCommand ampCommand;
extern struct SMediaSession mediaTab[3*MAX_AMP_INSTANCES];
static action_t action = ACTION_NONE;
static struct SMediaSession *media; 	


/**
* 
* <b>FUNCTION: </b>
*
* Define cmd_preprocess_def()
*
* <b>DESCRIPTION: </b>
* 
*
* 
* @param
*       
*
*
**/

static void cmd_preprocess_def(void)
{
		media = &mediaTab[ampIndex]; 
    action = ACTION_EXECUTE; 
    memset(&ampCommand.dtv,0,sizeof(ampCommand.dtv)); 
    ampCommand.generic.dataSize = sizeof(struct SDTVCommand); 
    media->result.generic.dataSize = sizeof(struct SDTVResult); 
    ampCommand.generic.mediaSpace = media->status.generic.mediaSpace; 
    media->result.generic.mediaSpace = media->status.generic.mediaSpace;
}


/**
 * 
 * <b>FUNCTION: </b>
 *
 * static void set_dtv_action(int ampIndex, action_t action)
 *
 * <b>DESCRIPTION: </b>
 *      Set actions for dtv
 *
 * 
 * @param
 *  	int ampIndex, action_t action
 * @return 
 *      None
**/

static void set_dtv_action(int ampIndex, action_t action)
{
	DFBResult err;
	struct SMediaSession *media;
	media = &mediaTab[ampIndex];

	// reset the query result
	media->queryResult[0] = 0;

	if (action == ACTION_POST)
	{
		err = media->amp->PostPresentationCmd(media->amp, &ampCommand.generic);
		if (err != DFB_OK)
		{
			if (err == DFB_BUSY)
			{
				media->tempStatus = TSTS_BUSY;
			} else
			{
				media->tempStatus = TSTS_AMP_ERROR;
			}
		}
	} else if (action == ACTION_EXECUTE)
	{
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
                D_ERROR("\tExecute error %d (%ld)\n", err, media->result.generic.value);
				media->tempStatus = TSTS_AMP_ERROR;
            }
		} 
		else
		{
			if ((media->msRT->mediaSpace == MEDIA_SPACE_DTV) && (*(media->msRT->pCmd) == (int) DTVCmd_SET_PARTIALTSCBF))
			{
				if (!ampCommand.dtv.param3.fPTSCallback)
					close_TS_file(ampIndex);
			}
			processMediaQuery(media);
		}
	} else
	{
		assert(false);
	}

	// set up adjustment structure for the next time
	adjustment.type = ADJ_MIXER_PAN_MAIN;

	return;
}


/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_sect_CBF((int ampIndex, SectionDescriptor_t secdesc,
 * 				    DTVSectionCBF_t fseccallback) 
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  SectionDescriptor_t secdesc,
 *  	DTVSectionCBF_t fseccallback
 * @return 
 *      void
**/
void set_sect_CBF(int ampIndex,SectionDescriptor_t secdesc,DTVSectionCBF_t fseccallback)
{
    struct SMediaSession *media;
    media = &mediaTab[ampIndex];
    action_t action = ACTION_EXECUTE;

    memset(&ampCommand.dtv.param1.SecDesc,0,sizeof(ampCommand.dtv.param1.SecDesc));

    ampCommand.generic.dataSize = sizeof(struct SDTVCommand);
    media->result.generic.dataSize = sizeof(struct SDTVResult);
    ampCommand.generic.mediaSpace = media->status.generic.mediaSpace;
    media->result.generic.mediaSpace = media->status.generic.mediaSpace;
    *(media->msRT->pCmd) = DTVCmd_SET_SECTCBF;

	ampCommand.dtv.param1.SecDesc = secdesc;
	ampCommand.dtv.param2.fSecCallback = fseccallback;
	ampCommand.dtv.param3.Input = ampIndex;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_play(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex
 * @return 
 *      void
**/
void set_play(int ampIndex)
{
    struct SMediaSession *media;
    media = &mediaTab[ampIndex];
    action_t action = ACTION_EXECUTE;
    memset(&ampCommand.dtv.param1.SecDesc,0,sizeof(ampCommand.dtv.param1.SecDesc));

    ampCommand.generic.dataSize = sizeof(struct SDTVCommand);
    media->result.generic.dataSize = sizeof(struct SDTVResult);
    ampCommand.generic.mediaSpace = media->status.generic.mediaSpace;
    media->result.generic.mediaSpace = media->status.generic.mediaSpace;
    *(media->msRT->pCmd) = DTVCmd_PLAY;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void SetDSMCCCBF(int ampIndex, SectionDescriptor_t secdesc,
 *                  DTVSectionCBF_t fseccallback)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  SectionDescriptor_t secdesc,
 *  	DTVSectionCBF_t fseccallback
 * @return 
 *      void
**/
void set_DSMCCCBF(int ampIndex,SectionDescriptor_t secdesc,DTVSectionCBF_t fseccallback)
{
    cmd_preprocess_def();

    ampCommand.dtv.param1.SecDesc = secdesc;
	ampCommand.dtv.param2.fSecCallback = fseccallback;
    *(media->msRT->pCmd) = DTVCmd_SET_DSMCCCBF;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_stop(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex
 * @return 
 *      void
**/
void set_stop(int ampIndex)
{
	cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_STOP;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_stop_last_frame(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex
 * @return 
 *      void
**/
void set_stop_last_frame(int ampIndex)
{
	printf("\n \033[31m set_stop_last_frame set_stop_last_frame set_stop_last_frame\033[0m ");
	cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_STOP;
	ampCommand.dtv.param1.Input = 1;
	//ampCommand.dtv.param1.Input = 0;
    set_dtv_action(ampIndex,action);
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_pause(int ampIndex, bool bPause)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex, bool bPause
 * @return 
 *      void
**/
void set_pause(int ampIndex, bool bPause)
{
	cmd_preprocess_def();
    *(media->msRT->pCmd) = bPause ? DTVCmd_PAUSE_ON : DTVCmd_PAUSE_OFF;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_AV_source(int ampIndex, PESDescriptor_t pesdescV,
 *                  PESDescriptor_t pesdescA,unsigned short pid)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,     PESDescriptor_t pesdescV
 * 		PESDescriptor_t pesdescA,    unsigned short pid
 * @return 
 *      void
**/
void set_AV_source(int ampIndex,PESDescriptor_t pesdescV,PESDescriptor_t pesdescA,unsigned short pid)
{
    //printf("--------thinh test SetAVsource\n");
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_AVSOURCE;

	ampCommand.dtv.param1.PesDesc = pesdescV;
	ampCommand.dtv.param2.PesDesc = pesdescA;
	ampCommand.dtv.param3.Pid = pid;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_audio_source(int ampIndex,PESDescriptor_t pesdescA, 
 *                     unsigned short pid,ePlayMode_t playmode) 
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,   PESDescriptor_t pesdescA
 * 		unsigned short pid,   ePlayMode_t playmode
 * @return 
 *      void
**/
void set_audio_source(int ampIndex,PESDescriptor_t pesdescA,unsigned short pid,ePlayMode_t playmode)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_UPDATE_AUD_SRC;

	ampCommand.dtv.param1.PesDesc = pesdescA;
	ampCommand.dtv.param2.Input = (int)playmode;
	ampCommand.dtv.param3.Pid = pid;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_video_source(int ampIndex, PESDescriptor_t pesdescV,
 *                     unsigned short pid,ePlayMode_t playmode) 
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,   PESDescriptor_t pesdescV
 * 		unsigned short pid,   ePlayMode_t playmode
 * @return 
 *      void
**/
void set_video_source(int ampIndex,PESDescriptor_t pesdescV,unsigned short pid, ePlayMode_t playmode)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_UPDATE_VID_SRC;

	ampCommand.dtv.param1.PesDesc = pesdescV;
	ampCommand.dtv.param2.Input = (int)playmode;
	ampCommand.dtv.param3.Pid = pid;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_PESCBF(int ampIndex, PESDescriptor_t pesdesc, 
 *                DTVPESDataCBF_t fpescallback) 
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex, PESDescriptor_t pesdesc,
 * 		DTVPESDataCBF_t fpescallback
 * @return 
 *      void
**/
void set_PESCBF(int ampIndex,PESDescriptor_t pesdesc,DTVPESDataCBF_t fpescallback)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_PESCBF;
	ampCommand.dtv.param1.PesDesc = pesdesc;
	ampCommand.dtv.param2.fPesCallback = fpescallback;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void get_VCXO_Adj(int ampIndex, VCXO_Adjustment_t adj)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 * 		int ampIndex, VCXO_Adjustment_t adj
 * @return 
 *      void
**/
void get_VCXO_Adj(int ampIndex,VCXO_Adjustment_t adj)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_GET_VCXO_ADJ;
	ampCommand.dtv.param1.Adj = adj;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_VCXO_Adj(int ampIndex,VCXO_Adjustment_t adj)
 *
 * <b>DESCRIPTION: </b>
  @param
 * 		int ampIndex,VCXO_Adjustment_t adj
 * @return 
 *      void
**/
void set_VCXO_Adj(int ampIndex,VCXO_Adjustment_t adj)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_VCXO_ADJ;
	ampCommand.dtv.param1.Adj = adj;

    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_down_mix_table(int ampIndex, struct 
 *                AudioEngine_Downmixing_tables_type adownmix) 
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex
 * 		struct AudioEngine_Downmixing_tables_type adownmix
 * @return 
 *      void
**/
void set_down_mix_table(int ampIndex,struct AudioEngine_Downmixing_tables_type adownmix)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_DOWNMIXTABLE;
	ampCommand.dtv.param1.ADownMix = adownmix;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_tuner_ex(int ampIndex,int Frequency, 
 * 					 int Model, int Mode)
 *
 * <b>DESCRIPTION: </b>
 * @param
 *  	int ampIndex,  int Frequency
 * 		int Model,     int Mode
 * @return 
 *      void
**/
void set_tuner_ex(int ampIndex,int Frequency, int Model, int Mode)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_TUNER;
	 ampCommand.dtv.param1.Input = Frequency;
     ampCommand.dtv.param2.Input = Model;
     ampCommand.dtv.param3.Input = Mode;
     set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void SetStream(int ampIndex,char *url)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  char *url
 * @return 
 *      void
**/
void set_stream(int ampIndex,char *url)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_STREAM;
	 ampCommand.dtv.param1.FileName = url;
     set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_format_on_CBF(int ampIndex, 
 *                     DTVFormatChng_t fformatcallback)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  DTVFormatChng_t fformatcallback
 * @return 
 *      void
**/
void set_format_on_CBF(int ampIndex,DTVFormatChng_t fformatcallback)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_FORMATMONCBF;
	 ampCommand.dtv.param1.fFormatCallback = fformatcallback;
     set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_partial_TSATSCBF(int ampIndex, 
 *                     DTVFormatChng_t fformatcallback)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  DTVFormatChng_t fformatcallback
 * @return 
 *      void
**/
void set_partial_TSATSCBF(int ampIndex,TSPidArray_t tspid,PESDescriptor_t pesdesc,DTVTSDataCBF_t fptscallback)
{
	cmd_preprocess_def();
	*(media->msRT->pCmd) = DTVCmd_SET_PARTIALTS_ATSCBF;
	ampCommand.dtv.param1.TsPids = tspid;
	ampCommand.dtv.param2.PesDesc = pesdesc;
	ampCommand.dtv.param3.fPTSCallback = fptscallback;
	set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_EOF_CBF(int ampIndex, DTVFormatChng_t 
 * fformatcallback) 
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex, DTVFormatChng_t fformatcallback
 * @return 
 *      void
**/
void set_EOF_CBF(int ampIndex, DTVFormatChng_t fformatcallback)
{
	cmd_preprocess_def();
	*(media->msRT->pCmd) = DTVCmd_SET_ENDOFFILECBF;
	ampCommand.dtv.param1.fFormatCallback = fformatcallback;
	set_dtv_action(ampIndex,action);
	
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_partial_TSCBF(int ampIndex, 
 *  					TSPidArray_t tspid,
 *  					PESDescriptor_t pesdesc,
 * 						DTVTSDataCBF_t fptscallback)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,
 *  	TSPidArray_t tspid,
 *  	PESDescriptor_t pesdesc,
 * 		DTVTSDataCBF_t fptscallback
 * @return 
 *      void
**/
void set_partial_TSCBF(int ampIndex,TSPidArray_t tspid,PESDescriptor_t pesdesc,DTVTSDataCBF_t fptscallback)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_PARTIALTSCBF;
    ampCommand.dtv.param1.TsPids = tspid;
    ampCommand.dtv.param2.PesDesc = pesdesc;
    ampCommand.dtv.param3.fPTSCallback = fptscallback;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_ECMKeys(int ampIndex, int pid, 
 *  			   Cipher_Key_t cipherkey,
 * 				   CipherType_t ciphertype)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  int pid,
 *  	Cipher_Key_t cipherkey,
 * 		CipherType_t ciphertype
 * @return 
 *      void
**/
void set_ECMKeys(int ampIndex,int pid,Cipher_Key_t cipherkey,CipherType_t ciphertype)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_ECMKEYS;
	ampCommand.dtv.param1.Pid = pid;
	ampCommand.dtv.param2.CipherKey = cipherkey;
	ampCommand.dtv.param3.CipherType = ciphertype;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_index_file(int ampIndex, 
 *  				 const char *pFileName,
 * 					 unsigned short Pid)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,const char *pFileName, unsigned short Pid
 * @return 
 *      void
**/
void set_index_file(int ampIndex,const char *pFileName, unsigned short Pid)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_INDEX_FILE;
    ampCommand.dtv.param1.FileName = pFileName;
    ampCommand.dtv.param2.Input = Pid;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_step(int ampIndex)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex
 * @return 
 *      void
**/
void set_step(int ampIndex)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_STEP;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_seek_pos(int ampIndex,int msPos)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  int msPos
 * @return 
 *      void
**/
void set_seek_pos(int ampIndex,int msPos)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SEEKPOS;
    ampCommand.dtv.param1.Input = msPos;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_fast_Fwd(int ampIndex,int Speed)
 *
 * <b>DESCRIPTION: </b>
 * @param
 *  	int ampIndex,  int Speed
 * @return 
 *      void
**/
void set_fast_Fwd(int ampIndex,int Speed)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_FASTFWD;
    ampCommand.dtv.param1.Input = Speed;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_fast_Rwd(int ampIndex,int Speed)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  int Speed
 * @return 
 *      void
**/
void set_fast_Rwd(int ampIndex,int Speed)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_FASTRWD;
    ampCommand.dtv.param1.Input = Speed;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_slow_Fwd(int ampIndex,int Speed)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  int Speed
 * @return 
 *      void
**/
void set_slow_Fwd(int ampIndex,int Speed)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SLOWFWD;
    ampCommand.dtv.param1.Input = Speed;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_slow_Rwd(int ampIndex,int Speed)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *  	int ampIndex,  int Speed
 * @return 
 *      void
**/
void set_slow_Rwd(int ampIndex,int Speed)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SLOWRWD;
    ampCommand.dtv.param1.Input = Speed;
    set_dtv_action(ampIndex,action);
}

/**
 * 
 * <b>FUNCTION: </b>
 *
 * void set_partial_TS_scramble(int ampIndex, 
 *  						 TSPidArray_t tspid,
 * 							 PESDescriptor_t pesdesc,
 * 							 DTVTSDataCBF_t fptscallback)
 *
 * <b>DESCRIPTION: </b>
 *
 * @param
 *  	int ampIndex,
 *  	TSPidArray_t tspid,
 *  	PESDescriptor_t pesdesc,
 * 		DTVTSDataCBF_t fptscallback
 * @return 
 *      void
**/
void set_partial_TS_scramble(int ampIndex,TSPidArray_t tspid,PESDescriptor_t pesdesc,DTVTSDataCBF_t fptscallback)
{
    cmd_preprocess_def();
    *(media->msRT->pCmd) = DTVCmd_SET_PARTIALTS_SCRAMBLE;
    ampCommand.dtv.param1.TsPids = tspid;
    ampCommand.dtv.param2.PesDesc = pesdesc;
    ampCommand.dtv.param3.fPTSCallback = fptscallback;
    set_dtv_action(ampIndex,action);
}

#ifdef WITH_CC

void CCCallbackFunc(void *pBuffer,	size_t Size)
{
	
    printf("pBuffer = %s\n", (char*)(pBuffer));
    printf("Size = %d\n", Size);
	printf("\033[0m \n");
}

void set_cc_on(int ampIndex, int CC_mode)
{
	//CC_mode = EDT_CC_608;

    DTVUserDataCBF_t fUserDataCallback = CCCallbackFunc;
	set_cc_on_off(ampIndex, TRUE, CC_mode, fUserDataCallback);
}

void set_cc_off(int ampIndex, int CC_mode)
{
	set_cc_on_off(ampIndex, FALSE, CC_mode, NULL);
}

void set_cc_on_off(int ampIndex,int cc_on_off,int CCmode, DTVUserDataCBF_t fUserDataCallback)
{
    cmd_preprocess_def();
	*(media->msRT->pCmd) = DTVCmd_SET_CC_USR_MON;
	ampCommand.dtv.common.mediaSpace = MEDIA_SPACE_DTV;
	ampCommand.dtv.common.dataSize = sizeof(ampCommand.dtv);
    ampCommand.dtv.param1.Input = cc_on_off;
	ampCommand.dtv.param2.Input = CCmode;
    ampCommand.dtv.param3.fUserDataCallback = fUserDataCallback;
	printf("ampCommand.dtv.param1.Input = %d\n", ampCommand.dtv.param1.Input);
    printf("ampCommand.dtv.param2.Input = %d\n", ampCommand.dtv.param2.Input);
    set_dtv_action(ampIndex,action);
}
#endif

void set_volume_dtv(RMuint8 value)
{
	RMuint32 temp = VOLUME_0DB;
	RMuint32 idx = 0 ;
	idx =((value + 1)* MAX_VOLUME_TABLE)/MAX_UI_VOLUME;
    if(idx == MAX_VOLUME_TABLE)
		idx = MAX_VOLUME_TABLE - 1;
	temp = VolumeTable[idx];
	printf("set_volume_dtv temp = 0x%lx\n", temp);

	cmd_preprocess_def();
	*(media->msRT->pCmd) = /*Cmd_Adjust*/ DTVCmd_SET_VOLUME;
	adjustment.value.volume = temp;
	adjustment.type = ADJ_VOLUME;
	ampCommand.generic.control.adjustment = &adjustment;
	set_dtv_action(0, action);
}

void set_volume_dtv_for_dolby(RMuint8 value)
{
	RMuint32 temp = VOLUME_0DB;
	RMuint32 idx = 0 ;
	idx =((value + 1)* MAX_VOLUME_TABLE_DOLBY)/MAX_UI_VOLUME;
    if(idx == MAX_VOLUME_TABLE_DOLBY)
		idx = MAX_VOLUME_TABLE_DOLBY - 1;
	temp = VolumeTable[idx];
	printf("set_volume_dtv_for_dolby temp = 0x%lx\n", temp);

	cmd_preprocess_def();
	*(media->msRT->pCmd) = /*Cmd_Adjust*/ DTVCmd_SET_VOLUME;
	adjustment.value.volume = temp;
	adjustment.type = ADJ_VOLUME;
	ampCommand.generic.control.adjustment = &adjustment;
	set_dtv_action(0, action);
}

