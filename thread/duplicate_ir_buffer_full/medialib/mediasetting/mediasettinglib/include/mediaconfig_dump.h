#ifndef CONFIG_DUMP_H_
#define CONFIG_DUMP_H_

void print_video_settings(const struct SMRUAVideoOutConfig *settings);
void print_audio_settings(const struct SMRUAAudioOutConfig *settings);
void print_drm_settings(const struct rmcci *drm);


bool ConfigPreview(const struct RUA *pRUA,
				  const struct SContentInfo *content,
				  const struct rmcci *drm,
				  const struct SMRUAOutputConfig *settings,
				  const struct SAdjustment *adjustments,
				  struct SMRUAConfig *configuration);

bool ConfigConfirm(const struct RUA *pRUA,
				  const struct SContentInfo *content,
				  const struct SMRUAConfig *configuration);

bool AVAccessRequest(int dataPathIndex, char *fileName, UINT64 fileOffset, UINT32 dataAmount, UINT32 bufferFullness, UINT32 maxBitRate);

void AVAccessNotify(int dataPathIndex, long bufferEmptyness);

#endif /*CONFIG_DUMP_H_*/
