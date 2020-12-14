

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


#define BUFFER_SIZE             64
#define DEFAULT_IP_ADDRESS      "10.178.4.2"
#define DEFAULT_PORT            5000
#define INVALID_SOCKET          -1


char* printnip(unsigned long ip)
{
   static char printbuf[40];
   sprintf(printbuf,"%ld.%ld.%ld.%ld", (ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff);
   return printbuf;
}

#define STRING_IP(ip)  ((ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff)
#define TRUE    1
#define FALSE   0

FILE *fpWrite = NULL; //file log
short lsocket = -1;   // listening socket
unsigned char sndBuf[BUFFER_SIZE]={0x01,0xba,0xad,0xbe,0xef,0xfe,0xed,0xca,0xfe};
unsigned char rcvBuf[BUFFER_SIZE];
struct sockaddr_in rcv_sin;   // Server address structure

void recvThread(void *lpParameter)
{
	int	nbytes;
    int i;
    int socklen;

    socklen = sizeof(struct sockaddr_in);
    for (i=0;i<BUFFER_SIZE;i++) {
        rcvBuf[i] = 0;
    }

	while (1) {
        nbytes = recv( lsocket,
                      (char *)&rcvBuf[0], 
                       BUFFER_SIZE, 
                       0
                     );
/*        nbytes = recvfrom( lsocket,
                          (char *)&rcvBuf[0], 
                           BUFFER_SIZE, 
                           0,
       		              (struct sockaddr *)&rcv_sin, 
                          &socklen
                         );
 */
        if ((nbytes == 0) | (nbytes < 0))
        {
            printf("recv(): connection ended\n");
            break;
        }
        printf(" Received error code 0x%02x\n", rcvBuf[0]);
        i = 1;
/*        while ((i<64) & (rcvBuf[0] != 0xff)) 
        {
          printf(" 0x%02x", rcvBuf[i]);
          i++;
        }
        printf("\n"); */
	}
	return;
}


int main(int argc, char **argv)
{
   struct sockaddr_in snd_sin;   // Server address structure
   int  rc = 0, i, k;
//   char c='\0';
   char ins[128];
   int socklen;
   unsigned short lport;
   unsigned long  lip_addr;
//   unsigned int   ThreadId;
   pthread_t  recvThreadHandle=NULL;
   char writeFile[] = "ncon_log.txt";
//   int eofSession = 0;
   int cvbsEnable  = 0;
//   int ntscEnable  = 0;
//   int tsEnable    = 0;
   int bitrate     = 5000; //kbps
   int vbrEnable   = 1;
   int ttmvQuant   = 4;
//   int audioEnable = 1;
//   int audioSampleRate = 48000;
//   int ttmvWidth  = 720;
//   int ttmvHeight = 576;
   int ttmvFim = 25;
   int filter  = 0;
   int deInterlace = 0;
   int brightness = 128;
   int contrast = 64;
   int saturation = 64;
   int hue = 0;
   int toSend; 

   if ((argc!=1)&(argc!=2)&(argc!=3)&(argc!=4))
   {
	    printf(" ncon [portnumber - default is %d] [ipaddress - default is %s] [log - default is off]\n",
                DEFAULT_PORT, DEFAULT_IP_ADDRESS);
		printf(" example: ncon 46999 192.168.2.102\n");
        printf("  version 1.0b\n");
        printf("  send e-mail to brigitte@electropix.com for additional features\n\n");
		return 0;
	}

    printf("  %s version 1.0\n", argv[0]);
    printf("  send e-mail to brigitte@tigress.fr for additional features\n");
    lport = DEFAULT_PORT;
	if (argc >= 2) lport = atoi(argv[1]);
    lip_addr = inet_addr(DEFAULT_IP_ADDRESS);
    if (argc == 3) lip_addr = inet_addr(argv[2]);
    if (argc == 4){
        if (strncmp(argv[3],"log",3)==0) {
            if((fpWrite = fopen(writeFile,"w"))==NULL) {
		        printf(" error: unable to open file %s errno %d\n", writeFile, errno);
		        return(-1);
            }
        }
	}
    printf("  using port %d ip address %s\n", lport, printnip(lip_addr));
	// Create a  TCP/IP socket
	if ((lsocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
//	if ((lsocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf ("Allocating socket failed. Error: %d\n", errno);
      while (1) {};
	}

	// Fill out the local socket's address information.
	memset((char *)&snd_sin, sizeof(struct sockaddr_in), 0);
	snd_sin.sin_family = AF_INET;
	snd_sin.sin_port = htons(lport);
	snd_sin.sin_addr.s_addr = lip_addr;

	rcv_sin.sin_family = AF_INET;
	rcv_sin.sin_port = htons(lport);  
	rcv_sin.sin_addr.s_addr = htonl(INADDR_ANY);

	rc = connect(lsocket, (struct sockaddr *)&snd_sin, sizeof(snd_sin));
    if ( rc < 0 )
    {
        printf("Error connect. Error: %d\n", errno);
        while (1) {};
	} 

//    rc = bind(lsocket, (struct sockaddr *)&rcv_sin, sizeof(struct sockaddr_in));
//    if ( rc < 0 )
//    {
//        printf("Error binding socket to interface errno %d\n", errno);
//        return rc;
//    }

    pthread_create(&recvThreadHandle, NULL, recvThread, &fpWrite); //

    socklen = sizeof(struct sockaddr_in);
    k = 0;
    memset(ins,0,128);

    printf("\n");
    printf("  Enter a command\n");
    printf("  get           retrieve remote application's parameters\n");
    printf("  set           configure the remote application\n");
    printf("  net           configure the remote application network parameters\n");
    printf("  default       apply default values\n");
    printf("  open          start display\n");
    printf("  start         start MPEG-2 streaming\n");
    printf("  stop          stop  MPEG-2 streaming\n");
    printf("  close         close application\n");
    printf("  flipx         flipx\n");
    printf("  flipy         flipy\n");
    printf("  inv           inverse components\n");
    printf("  contrast      contrast\n");
    printf("  saturation    saturation\n");
    printf("  brightness    brightness\n");
    printf("  hue           hue\n");
    printf("  quit          exit ncon\n");
    printf("> ");

    while (strncmp(ins, "quit", 4) != 0)
    {
        for (i=0;i<BUFFER_SIZE;i++) {
           sndBuf[i] = 0xff;
        }
        fgets(ins, 128, stdin);
        toSend = FALSE;            

        if (strncmp(ins, "get", 1) == 0) 
        {
            printf(" got get\n");
            sndBuf[0] = 0x01;
            toSend = TRUE;            
        };
        if (strncmp(ins, "set", 3) == 0) 
        {
            printf(" got set\n");
            sndBuf[0]  = 0x02;
            printf(" Enter input video signal (0:SVHS, 1:CVBS or 2:PATGEN) [%d] : ", cvbsEnable);
            fgets(ins, 128, stdin);
            if (ins[0]!='\n') 
                cvbsEnable = atoi(ins);
            if ((cvbsEnable != 0) & (cvbsEnable != 1) & (cvbsEnable != 2)) 
            {
                printf(" Wrong input video signal value %d, forcing to 0\n", cvbsEnable);
                cvbsEnable = 0;
            }
            sndBuf[1]  = cvbsEnable;
#if 0
            printf(" Enter NTSC (0 or 1) [%s] : ", (ntscEnable==1)?"enable":"disable");
            fgets(ins, 128, stdin);
            ntscEnable = atoi(ins);
            sndBuf[2]  = ntscEnable;
            printf(" Enter TS (0 or 1) [%s] : ", (tsEnable==1)?"enable":"disable");
            fgets(ins, 128, stdin);
            tsEnable = atoi(ins);
            sndBuf[3]  = tsEnable;
#endif
            printf(" Enter target bit rate (0 < bitrate < 15000) Kbps [%d] : ", bitrate);
            fgets(ins, 128, stdin);
            if (ins[0]!='\n') 
                bitrate = atoi(ins);
            if ((bitrate == 0) | (bitrate > 15000))
            {
                printf(" Wrong target bit rate value %d, forcing to 15000\n", bitrate);
                bitrate = 15000;
            }
            sndBuf[4]  = (bitrate>>8)&0xff; //bitrate in kbits per second
            sndBuf[5]  = (bitrate&0xff);
            printf(" Enter variable bit rate (0 or 1) [%s] : ", (vbrEnable==1)?"enable":"disable");
            fgets(ins, 128, stdin);
            if (ins[0]!='\n') 
                vbrEnable = atoi(ins);
            if ((vbrEnable != 0) & (vbrEnable != 1)) 
            {
                printf(" Wrong variable bit rate value %d, forcing to 0\n", vbrEnable);
                vbrEnable = 0;
            }
            sndBuf[6]  = vbrEnable;

            printf(" Enter quantizer (3 < quant <= 16) [%d] : ", ttmvQuant);
            fgets(ins, 128, stdin);
            if (ins[0]!='\n') 
                ttmvQuant = atoi(ins);
            if ((ttmvQuant < 4) | (ttmvQuant > 16))
            {
                printf(" Wrong quantizer value %d, forcing to 4\n", ttmvQuant);
                ttmvQuant = 4;
            }
            sndBuf[7]  = ttmvQuant;
#if 0
            printf(" Enter AUDIO (0 or 1) [%s] : ", (audioEnable==1)?"enable":"disable");
            fgets(ins, 128, stdin);
            audioEnable = atoi(ins);
            sndBuf[8]  = audioEnable;
            printf(" Enter AUDIO SAMPLE RATE (32000, 44100, 48000) [%d] : ", audioSampleRate);
            fgets(ins, 128, stdin);
            audioSampleRate = atoi(ins);
            sndBuf[9]  = (audioSampleRate>>8)&0xff;
            sndBuf[10] = (audioSampleRate&0xff);
            printf(" Enter WIDTH (16 < width <= 720) [%d] : ", ttmvWidth);
            fgets(ins, 128, stdin);
            ttmvWidth = atoi(ins);
            sndBuf[11] = (ttmvWidth>>8)&0xff;
            sndBuf[12] = (ttmvWidth&0xff);
            printf(" Enter HEIGHT (16 < height <= 720) [%d] : ", ttmvHeight);
            fgets(ins, 128, stdin);
            ttmvHeight = atoi(ins);
            sndBuf[13] = (ttmvHeight>>8)&0xff;
            sndBuf[14] = (ttmvHeight&0xff);
#endif
            printf(" Enter frame rate (0 < frame rate <= 25) [%d] : ", ttmvFim);
            fgets(ins, 128, stdin);
            if (ins[0]!='\n') 
                ttmvFim = atoi(ins);
            if ((ttmvFim == 0) | (ttmvFim > 25))
            {
                printf(" Wrong frame rate value %d, forcing to 25\n", ttmvFim);
                ttmvFim = 25;
            }
            sndBuf[15]  = ttmvFim;

            printf(" Enter de-interlacing (0 or 1) [%s] : ", (deInterlace==1)?"enable":"disable");
            fgets(ins, 128, stdin);
            if (ins[0]!='\n') 
                deInterlace = atoi(ins);
            if ((deInterlace != 0) & (deInterlace != 1)) 
            {
                printf(" Wrong de-interlacing value %d, forcing to 0\n", deInterlace);
                deInterlace = 0;
            }
            sndBuf[16]  = deInterlace;

            printf(" Enter pre-filtering (0 or 1) [%s] : ", (filter==1)?"enable":"disable");
            fgets(ins, 128, stdin);
            if (ins[0]!='\n') 
                filter = atoi(ins);
            if ((filter != 0) & (filter != 1)) 
            {
                printf(" Wrong pre-filtering value %d, forcing to 0\n", filter);
                deInterlace = 0;
            }
            sndBuf[17]  = filter;
            toSend = TRUE;            
        }
        if (strncmp(ins, "net", 3) == 0) 
        {
            printf(" got net\n");
            sndBuf[0] = 0x03;
            toSend = TRUE;            
        }
        if (strncmp(ins, "open", 4) == 0) 
        {
            printf(" got open\n");
            sndBuf[0] = 0x04;
            toSend = TRUE;            
        }
        if (strncmp(ins, "start", 5) == 0) 
        {
            printf(" got start\n");
            sndBuf[0] = 0x05;
            toSend = TRUE;            
        }
        if (strncmp(ins, "default", 7) == 0) 
        {
            printf(" got default\n");
            sndBuf[0] = 0x06;
            toSend = TRUE;            
        }
        if (strncmp(ins, "stop", 4) == 0) 
        {
            printf(" got stop\n");
            sndBuf[0] = 0x07;
            toSend = TRUE;            
        }
        if (strncmp(ins, "close", 5) == 0) 
        {
            printf(" got close\n");
            sndBuf[0] = 0x08;
            toSend = TRUE;            
        }
        if (strncmp(ins, "brightness", 10) == 0) 
        {
            printf(" got brightness\n");
            sndBuf[0] = 0x0B;
            printf(" Enter new value [%d] : ", brightness);
            fgets(ins, 128, stdin);
            brightness = atoi(ins);
            sndBuf[1]  = (brightness)&0xff;
            toSend = TRUE;            
        }
        if (strncmp(ins, "contrast", 8) == 0) 
        {
            printf(" got contrast\n");
            sndBuf[0] = 0x0A;
            printf(" Enter new value [%d] : ", contrast);
            fgets(ins, 128, stdin);
            contrast = atoi(ins);
            sndBuf[1]  = (contrast)&0xff;
            toSend = TRUE;            
        }
        if (strncmp(ins, "saturation", 10) == 0) 
        {
            printf(" got saturation\n");
            sndBuf[0] = 0x0C;
            printf(" Enter new value [%d] : ", saturation);
            fgets(ins, 128, stdin);
            saturation = atoi(ins);
            sndBuf[1]  = (saturation)&0xff;
            toSend = TRUE;            
        }
        if (strncmp(ins, "hue", 3) == 0) 
        {
            printf(" got hue\n");
            sndBuf[0] = 0x0D;
            printf(" Enter new value [%d] : ", hue);
            fgets(ins, 128, stdin);
            hue = atoi(ins);
            sndBuf[1]  = (hue)&0xff;
            toSend = TRUE;            
        }
        if (strncmp(ins, "inv", 3) == 0) 
        {
            printf(" got inv\n");
            sndBuf[0] = 0x09;
            sndBuf[1] = 0x03;
            toSend = TRUE;            
        }
        if (strncmp(ins, "flipx", 5) == 0) 
        {
            printf(" got flipx\n");
            sndBuf[0] = 0x09;
            sndBuf[1] = 0x01;
            toSend = TRUE;            
        }
        if (strncmp(ins, "flipy", 5) == 0) 
        {
            printf(" got flipy\n");
            sndBuf[0] = 0x09;
            sndBuf[1] = 0x02;
            toSend = TRUE;            
        }
        if (toSend == TRUE) {
            rc = send ( lsocket, 
                       (char *)&sndBuf[0],
                        BUFFER_SIZE, 
                        0
                      );

            if (rc < 0)
            {
                printf("send(): error %d\n", errno);
                while (1) {};
            }
            printf(" 0x%02x 0x%02x 0x%02x 0x%02x \n", sndBuf[0], sndBuf[1], sndBuf[2], sndBuf[3]);
        }
        printf("> ");
   }
   printf(" Exiting ncon application...\n");
   if (recvThreadHandle != NULL) pthread_exit(recvThreadHandle);
   if (lsocket) close(lsocket);
   if (fpWrite) fclose(fpWrite);


   return 0;
}
