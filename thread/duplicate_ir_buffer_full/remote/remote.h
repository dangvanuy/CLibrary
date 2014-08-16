#ifndef __REMOTE_H__
#define __REMOTE_H__

#ifdef __cplusplus
extern "C"
{
#endif

void *smp_remote_event_thread(void *n);
int  driver_open_device(void);
void run_remote(void);
void stop_remote(void);

void *smp_remote_event_thread_dfb(void *n);
void run_remote_dfb(void);
void stop_remote_dfb(void);
void set_handle(void *handle);
void AutoSendKey(unsigned long key);
void AutoSendKey_GPIO(unsigned long key);

void AutoSendKey_TCP(unsigned long key);

#define Key_POWER 0x0000f00f //0xb54acb04
#define Key_ENTER 0x0000000d //0xfd02cb04
#define Key_RIGHT 0x0000f001 //0xfe01cb04
#define Key_LEFT  0x0000f000 //0xbc43cb04
#define Key_UP    0x0000f002 //0xf906cb04
#define Key_DOWN  0x0000f003 //0xf50acb04
#define Key_ZERO  0x00000030 //0xe41bcb04
#define Key_ONE   0x00000031 //0xbe41cb04
#define Key_TOW   0x00000032 //0xfc03cb04
#define Key_THREE 0x00000033 //0xbd42cb04
#define Key_FOUR  0x00000034 //0xba45cb04
#define Key_FIVE  0x00000035 //0xf807cb04
#define Key_SIX   0x00000036 //0xb946cb04
#define Key_SEVEN 0x00000037 //0xb946cb04
#define Key_EIGHT 0x00000038 //0xe817cb04
#define Key_NINE  0x00000039 //0xa956cb04

#ifdef __cplusplus
}
#endif


#endif //__REMOTE_H__

