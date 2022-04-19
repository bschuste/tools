
//#include "alsa.h"
/*
 * gcc get_device_info.c -I/usr/include -I/usr/include/sound -L/usr/lib/x86_64-linux-gnu -lasound -o devinfo
   gcc get_device_info.c -I/usr/include -I/usr/include/sound -I/usr/lib/arm-linux-gnueabihf -lasound -o devinfo

*/
#include <alsa/asoundlib.h>

#define DEVICE_ZERO             "hw:0"
static char card[4][16] = {{"hw:0"},{"hw:1"},{"hw:2"},{"hw:3"}};

int AvailableAudioDevices()
{

  snd_ctl_card_info_t *info;
  snd_pcm_info_t *pcminfo;


  snd_ctl_card_info_alloca(&info);
  snd_pcm_info_alloca(&pcminfo);

  int card = -1;
  while (snd_card_next(&card) >= 0 && card >= 0) {
    int err = 0;
    snd_ctl_t *handle;
    char name[20];
    snprintf(name, sizeof(name), "hw:%d", card);
    if ((err = snd_ctl_open(&handle, name, 0)) < 0) {
      continue;
    }

    if ((err = snd_ctl_card_info(handle, info)) < 0) {
      snd_ctl_close(handle);
      continue;
    }

    int dev = -1;
    while (snd_ctl_pcm_next_device(handle, &dev) >= 0 && dev >= 0) {
      snd_pcm_info_set_device(pcminfo, dev);
      snd_pcm_info_set_subdevice(pcminfo, 0);
      snd_pcm_info_set_stream(pcminfo, SND_PCM_STREAM_CAPTURE);
      if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0) {
        continue;
      }

      char szDeviceID[20];
      snprintf(szDeviceID, sizeof(szDeviceID), "plughw:%d,%d", card, dev);
      printf("szDeviceID=%s, name %s\n", szDeviceID, snd_ctl_card_info_get_name(info));
//      Device.push_back(DeviceInfo(szDeviceID,
//          snd_ctl_card_info_get_name(info)));
    }

    snd_ctl_close(handle);
  }
  return 0;
}

int main (int argc, void **argv[]) {

  AvailableAudioDevices();

  return 0;
}
