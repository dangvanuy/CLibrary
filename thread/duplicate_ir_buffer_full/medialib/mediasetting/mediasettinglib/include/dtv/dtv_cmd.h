/*****************************************
 Copyright  2008
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/
/**
* @file   dtv_cmd.h
* @brief  DTV presentation engine commands

* @author Jake Lee, Kshitij Shah
* @date   2008-12-07
*/

//#include "apitest.h"

//#include "../generalmedia.h"
#include <ctype.h>
#include <sys/resource.h>
//#include <directfb_strings.h>
#include <rmdef/rmdef.h>
#include <emhwlib/include/emhwlib.h>
#include <rua/include/rua.h>
//#include <mdefs.h>
//#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <directfb.h>
#include <directfb_keynames.h>
#include <directfb_version.h>
#include <direct/thread.h>
//#include <misc/conf.h>
#include <time.h>     /* for `clock()'   */
#include <stdio.h>    /* for `D_INFO()' */
#include <stdlib.h>   /* for `rand()'    */
#include <unistd.h>   /* for `sleep()'   */
#include <math.h>     /* for `sqrt()'    */
#include <assert.h>	  /* for assert()	*/
#include <wchar.h>     /* for `sqrt()'    */
#include <stddef.h>   /* for `offsetof()' */
#include <ctype.h>
#include <sys/vfs.h>
//RUA
#include <rua/include/rua.h>
#include <rua/include/rua_property.h>
#include <dcc/include/dcc.h>
#include <dcc/src/dcc_common.h>
#include <rmlibcw/include/rmlibcw.h>
#include <advancedmediaprovider.h>

#include <cdefs_lpb.h>
#include "dtv/cdefs_dtv.h"
#include "dtv/cdefs_cap.h"

//#include "../mediaplay_define.h"
//#include "../media_def.h"
//#include "../media.h"


#ifdef __cplusplus
extern "C"
{
#endif
void set_stop(int ampIndex);
void set_stop_last_frame(int ampIndex);
void set_pause(int ampIndex, bool bPause);
void set_play(int ampIndex);
void set_DSMCCCBF(int ampIndex,SectionDescriptor_t secdesc,DTVSectionCBF_t fseccallback);
void set_sect_CBF(int ampIndex,SectionDescriptor_t secdesc,DTVSectionCBF_t fseccallback);
void set_AV_source(int ampIndex,PESDescriptor_t pesdescV,PESDescriptor_t pesdescA,unsigned short pid);
void set_audio_source(int ampIndex,PESDescriptor_t pesdescA,unsigned short pid,ePlayMode_t playmode);
void set_video_source(int ampIndex,PESDescriptor_t pesdescV,unsigned short pid,ePlayMode_t playmode);
void set_PESCBF(int ampIndex,PESDescriptor_t pesdesc,DTVPESDataCBF_t fpescallback);
void set_VCXO_Adj(int ampIndex,VCXO_Adjustment_t adj);
void get_VCXO_Adj(int ampIndex,VCXO_Adjustment_t adj);
void set_down_mix_table(int ampIndex,struct AudioEngine_Downmixing_tables_type adownmix);
void set_tuner_ex(int ampIndex,int Frequency, int Model, int Mode);	
void set_stream(int ampIndex,char *url);
void set_format_on_CBF(int ampIndex,DTVFormatChng_t fformatcallback);
void set_EOF_CBF(int ampIndex,DTVFormatChng_t fformatcallback);
void set_ECMKeys(int ampIndex,int pid,Cipher_Key_t cipherkey,CipherType_t ciphertype);
void set_index_file(int ampIndex,const char *pFileName, unsigned short Pid);
void set_step(int ampIndex);
void set_seek_pos(int ampIndex,int msPos);
void set_fast_Fwd(int ampIndex,int Speed);
void set_fast_Rwd(int ampIndex,int Speed);
void set_slow_Fwd(int ampIndex,int Speed);
void set_slow_Rwd(int ampIndex,int Speed);
void set_partial_TS_scramble(int ampIndex,TSPidArray_t tspid,PESDescriptor_t pesdesc,DTVTSDataCBF_t fptscallback);
void set_partial_TSATSCBF(int ampIndex,TSPidArray_t tspid,PESDescriptor_t pesdesc,DTVTSDataCBF_t fptscallback);
void set_partial_TSCBF(int ampIndex,TSPidArray_t tspid,PESDescriptor_t pesdesc,DTVTSDataCBF_t fptscallback);
#ifdef WITH_CC
void CCCallbackFunc(void *pBuffer,	size_t Size);
void set_cc_on(int ampIndex, int CC_mode);
void set_cc_off(int ampIndex, int CC_mode);
void set_cc_on_off(int ampIndex,int cc_on_off,int CCmode, DTVUserDataCBF_t fUserDataCallback);
#endif
void set_volume_dtv(RMuint8 value);
void set_volume_dtv_for_dolby(RMuint8 value);

#ifdef __cplusplus
}
#endif

