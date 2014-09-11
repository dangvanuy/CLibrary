#ifndef __MEDIA_DTV_H__
#define __MEDIA_DTV_H__

#include "dtv_define.h"  
#include "globals.h"

#ifdef __cplusplus
extern "C"
{
#endif


void set_wnd_rect(int ampIdx, long double x, long double y, int w, int h);
void set_default_wnd_rect(int ampIdx);

/***********************************************************************************
 ** PIP ****************************************************************************
 ***********************************************************************************/
void set_sub_screen_rect(int pos);
void close_amp_for_pip(int ampIdx);
void swap_amp(int ampIdx, int showPos);

/***********************************************************************************
 ** Functions for playing and stop channel for dtv *********************************
 ***********************************************************************************/
void send_set_play(int ampIndex);
void send_set_stop(int ampIndex);
RMstatus play_dtv(int ampIndex);
RMstatus play_a_program(int ampIdx, int vPid, int vType, int aPid, int aType, int pcrPid);



/***********************************************************************************
 ** Functions to open and close amp for dtv ****************************************
 ***********************************************************************************/
RMstatus initialze_media_tabs(const char *pOpenParam, const char *pFileName);
RMstatus set_open_param(const char *pOpenParam);
RMstatus open_dtv_media(char* pOpenParam, char* pFileName, int *ampIdx);
void close_all_dtv_amps(void);
void close_dtv_amp(int ampIdx);
int is_amp_opened(int i);
void print_amps_opened(void);

/***********************************************************************************
 ** Functions for DVR **************************************************************
 ***********************************************************************************/
RMstatus send_set_partial_TSCBF(int ampIndex, void* pPid, DTVTSDataCBF_t fFunc, RMbool bEnable);
void write_TS_file(int ampIdx,void *pBuffer, size_t Size);
void close_TS_file(int ampIdx);


/***********************************************************************************
 ** Thread for dtv *****************************************************************
 ***********************************************************************************/
DirectThread *DFBCreateThread( DirectThreadType      thread_type,
							  DirectThreadMainFunc  thread_main,
							  void                 *arg,
							  const char           *name );
void DFBJoinThread(DirectThread *thread);
void DFBDestroyThread(DirectThread *thread);

/***********************************************************************************
 ** Functions for showing subtitle *************************************************
 ***********************************************************************************/
struct RUA* get_pRUA_globals(void);
void enable_second_window_on_osd(void);     
void set_raise_to_top(int windowosd);  
void clear_stt_region(void);
void clear_rec(RMuint16 x, RMuint16 y, RMuint16 width, RMuint16 height);
void draw_picture_data(RMuint16 x, RMuint16 y, RMuint16 width, RMuint16 height, RMuint32 *pData);
void convert_coordinate(RMuint16 *x, RMuint16 *y);
bool belong_to_sec_window_on_osd(RMuint16 x, RMuint16 y, RMuint16 width, RMuint16 height);
void disable_second_window_on_osd(void);
void SetVolumeDTV(RMuint8 volume);
void SetVolumeDTVForDolby(RMuint8 volume);
void reset_volume(void);
RMstatus reset_timersync(void);

/**********************************************************************************/
/**********************************************************************************/



















#define VOLUME_0DB			0x10000000
#define VOLUME_INDEX_0DB	74//50//47//49//79//
#define MAX_VOLUME_TABLE 	49//47//74//55//
#define MAX_VOLUME_TABLE_DOLBY 	74//47//74//55//
#define MAX_UI_VOLUME       100

void set_volume(RMuint8 ampIndex, enum EAdjustment adjustment, RMuint8 value);
void reset_media_command_def();

#ifdef __cplusplus
}
#endif

#endif /*__MEDIA_DTV_H__*/