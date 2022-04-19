/*
 * ac-random.c
 *
 *  Created on: Apr 3, 2017
 *      Author: brigitte
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/random.h>

#if 1
#include <sys/syscall.h>
//This does work on our raspi - make sure to include the prototype sys/syscall.h
int init_random(void * data, size_t size) {
  while(syscall(SYS_getrandom, data, size, GRND_RANDOM) != size);
  return 0;
}
//To use it:

//printf("calling SYS_getrandom\n");
//init_random((void *)data, 32);

#endif

int main (int argc, void **argv[]) {
  uint8_t data[64]={1};
  int i;
  int entropy;
  int fd = open("/dev/random", O_RDONLY);
  int result = ioctl(fd, RNDGETENTCNT, &entropy);
  if (entropy < sizeof(int) * 8) {
    printf("Error - there's not enough bits of entropy in the random device to fill the buffer\n");
    close(fd);
    return;
  }
  if (result) {
    printf("Error - /dev/random isn't actually a random device\n");
    close(fd);
    return;
  }
  close(fd);

  fd = open("/dev/urandom", O_RDONLY);
  result = ioctl(fd, RNDGETENTCNT, &entropy);
  if (result) {
    printf("Error - /dev/urandom isn't actually a random device\n");
    close(fd);
    return;
  }

  // Upon success data now contains amount of entropy available in bits
  // If fails /dev/urandom has been compomised.
  printf("ioctl returned %d\n", result);

  read(fd, data, 32);

  printf("Here is a random number: \n");
  for (i=0;i<32;i++) {
    printf("%02X", data[i]);
  }
  printf("\n");

  printf("calling SYS_getrandom\n");
  init_random((void *)data, 32);

  printf("Here is another random number: \n");
  for (i=0;i<32;i++) {
    printf("%02X", data[i]);
  }
  printf("\n");

  return 0;
}
//typedef basic_string<unsigned char> ustring;
