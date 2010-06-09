#ifndef _H_KD
#define _H_KD

#include <gctypes.h>

#ifdef __cplusplus
   extern "C" {
#endif

#define KD_DOWNLOADFLAGS_MANUAL BIT(1)//Specify a entry to download, must be set when used from Broadway.
#define KD_DOWNLOADFLAGS_SUBTASKS BIT(2)//Set this to download subTasks.

//These errors can be returned by KD_Download.
#define KD_ESIGFAIL -45//RSA signature verification failed. Or KD_CreateRSA failed.
#define KD_EHTTP -32//HTTP error?
#define KD_ECACHE -15//"Detected AKAMAI's cache refreshing."
#define KD_EINVAL -3//Invalid input.
#define KD_ESHUTDOWN -48//Only happens when STM_Wakeup needs to be called by KD: "Shutdown required. Quit processing."
#define KD_ENOSPACE -6//No space is available in VFF.

typedef struct _skd_timebuf
{
	s32 retval;//Zero for success, -30 when "Universal time is not ready.".
	u64 time;//UTC time in seconds.
} kd_timebuf;

s32 KD_Open();
s32 KD_Close();

s32 KD_GetUTCTime(kd_timebuf *time);//If KD_Open wasn't called when any of these KD ioctl functions are called, LIBWC24_EINIT will be returned.
s32 KD_SetUTCTime(kd_timebuf *time);
s32 KD_CorrectRTC(u64 diff);

s32 KD_Download(u32 flags, u16 index, u32 subTaskBitmap);//flags is the KD_DOWNLOADFLAG defines, index is nwc24dl_entry index, subTaskBitmap is a bitmap of subTasks to download starting at bit zero for subTask zero when flags KD_DOWNLOADFLAGS_SUBTASKS is set.

#ifdef __cplusplus
   }
#endif

#endif
