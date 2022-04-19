/*
 * hotplug.c
 *
 *  Created on: 18 déc. 2013
 *      Author: brigitte
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <linux/types.h>
#include <linux/netlink.h>

int die(char *s)
{
	write(2,s,strlen(s));
	return 1;
}

/* support for 4 hotplug devices key and value up to 256 chars*/
char hotplug_table_key[8][2][256]=
{
				"DEVNAME", "", // sda or sda1
				"DEVTYPE", "", // disk or partition
				"DEVNAME", "", // sda or sda1
				"DEVTYPE", "", // disk or partition
				"DEVNAME", "", // sda or sda1
				"DEVTYPE", "", // disk or partition
				"DEVNAME", "", // sda or sda1
				"DEVTYPE", "", // disk or partition
};

/*
====> KEY=DEVNAME VALUE=sda
====> KEY=DEVTYPE VALUE=disk
====> KEY=SEQNUM VALUE=663
====> KEY=ACTION VALUE=add
====> KEY=DEVPATH VALUE=/devices/platform/musb-da8xx/musb-hdrc/usb2/2-1/2-1:1.0/host5/target5:0:0/5:0:0:0/block/sda/sda1
====> KEY=SUBSYSTEM VALUE=block
====> KEY=MAJOR VALUE=8
====> KEY=MINOR VALUE=1
====> KEY=DEVNAME VALUE=sda1
====> KEY=DEVTYPE VALUE=partition
====> KEY=SEQNUM VALUE=664
 */

static int quit = 0;
static void catcher (int sig)
{

	fprintf(stderr, "Caught signal %d\n", sig);
	fflush(stderr);
	quit = 1;
}

static char devname[256];
static char firmware[256];
static int inserted = 0; //0: inactive   1: active
static int removed = 0;  //0: inactive   1: active

int detection(int mode)
{
	int ret = 0;
	if (inserted == 1)
	{
		if ( ((devname[0] != '/')&&(mode==0)) ||
			 ((strncmp(devname,"/dev",4) == 0)&&(mode==1)) )
		{
			inserted = 0;
			ret = 1;
			printf("====> USB KEY !!INSERTED!!     ");
		}
	}
	if (removed == 1)
	{
		if ( ((devname[0] != '/')&&(mode==0)) ||
			 ((strncmp(devname,"/dev",4) == 0)&&(mode==1)) )
		{
			printf("====> USB KEY !!REMOVED!!   ");
			ret = 1;
			removed = 0;
		}
	}
	return ret;
}

int main(int argc, char *argv[])
{
	struct sockaddr_nl nls;
	struct pollfd pfd;
	char buf[512];
	int rc = 0;
	int val = 0;
	char keyname[256];
	char keyvalue[256];
	int j=0; int i;

    signal(SIGTERM, catcher);
    signal(SIGINT, catcher);

	// Open hotplug event netlink socket

	memset(&nls,0,sizeof(struct sockaddr_nl));
	nls.nl_family = AF_NETLINK;
	nls.nl_pid = getpid();
	nls.nl_groups = -1;

	pfd.events = POLLIN;
	pfd.fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (pfd.fd==-1)
		die("Not root\n");

	// Listen to netlink socket

	if (bind(pfd.fd, (struct sockaddr *)&nls, sizeof(struct sockaddr_nl)))
	{
		die("Bind failed\n");
	}
	while ((quit != 1) && (-1!=poll(&pfd, 1, -1)) )
	{
		int len = recv(pfd.fd, buf, sizeof(buf), MSG_DONTWAIT);
		if (len == -1) die("recv\n");

		// Print the data to stdout.
		i = 0;
		while (i<len)
		{

//			printf("%s\n", buf+i);
			rc = sscanf(buf+i, "%128[^=]=%d", keyname, &val);
			if (rc == 2) {
				printf("====> KEY=%s VALUE=%d\n", keyname, val);
			}
			else
			if (rc != 2)
			{
				rc = sscanf(buf+i, "%128[^=]=%s", keyname, keyvalue);
				if (rc == 2)
				{
					printf("====> key=%s \t value=%s\n", keyname, keyvalue);
					if ((strcmp(keyname, "ACTION") == 0))
					{
						if ((strcmp(keyvalue, "remove") == 0))
						{
							removed = 1 ; inserted = 0 ;
						}
						if ((strcmp(keyvalue, "add") == 0))
						{
							inserted = 1 ; removed = 0 ;
						}
					}

          if ((strcmp(keyname, "DEVNAME") == 0))
          {
            strncpy(devname, keyvalue, 256); //remember the device name
          }
          if ((strcmp(keyname, "FIRMWARE") == 0))
          {
            if (inserted == 1)
            {
              strncpy(firmware, keyvalue, 256); //remember the firmware name
              printf("====>  INSERT FIRMWARE=%s\n", firmware);
            }
          }
          if ((strcmp(keyname, "DEVTYPE") == 0))
          {
            if ((strcmp(keyvalue, "partition") == 0))
            {
              if (inserted == 1)
              {
                if (devname[0] != '/')
                {
                  inserted = 0;
                  printf("====> USB KEY !!INSERTED!!   DEVNAME=%s \tDEVTYPE=%s \n", devname, keyvalue);
                }
              }
              if (removed == 1)
              {
                if (devname[0] != '/')
                {
                  printf("====> USB KEY !!REMOVED!!   DEVNAME=%s \tDEVTYPE=%s \n", devname, keyvalue);
                  removed = 0;
                }
              }
            }
          }
#if 0
    //There is since release 10 a network daemon to take care of this
    //                            [DBUG] sbrUpdate.cpp, hotplugThread, 541: ====> key=SUBSYSTEM    value=net
    //                            [DBUG] sbrUpdate.cpp, hotplugThread, 541: ====> key=INTERFACE    value=eth0
                                if ((strcmp(keyname, "SUBSYSTEM") == 0))
                                {
                                    if (inserted == 1)
                                    {
                                        TRACE_UPDATE("====> !!INSERTED!!\n");
                                        if ((strcmp(keyvalue, "net") == 0))
                                        {
                                            TRACE_UPDATE("====> !!NET_INSERTED!!\n");
                                            net_removed = 0 ; net_inserted = 1 ;
                                            inserted = 0;
                                        }
                                    }
                                    if (removed == 1)
                                    {
                                        TRACE_UPDATE("====> !!REMOVED!!\n");
                                        if ((strcmp(keyvalue, "net") == 0))
                                        {
                                            TRACE_UPDATE("====> !!NET_REMOVED!!\n");
                                             net_removed = 1 ; net_inserted = 0 ;
                                             removed = 0;
                                        }
                                    }
                                }
                                if ((strcmp(keyname, "INTERFACE") == 0))
                                {
                                    if (net_inserted == 1)
                                    {
                                        net_inserted = 0;
                                        TRACE_UPDATE("====> USB ETHERNET !!INSERTED!! INTERFACE=%s\n", keyvalue);
                                        sprintf(cmd, "ifup %s", keyvalue);
    //                                    system(cmd);
                                    }
                                    if (net_removed == 1)
                                    {
                                        net_removed = 0;
                                        TRACE_UPDATE("====> USB ETHERNET !!REMOVED!!  INTERFACE=%s\n", keyvalue);
                                        sprintf(cmd, "ifdown %s", keyvalue);
    //                                    system(cmd);
                                    }
                                }
#endif


					if ((strcmp(keyname, "DEVNAME") == 0))
					{
						strncpy(devname, keyvalue, 256); //remember the device name
					}
					if ((strcmp(keyname, "ID_TYPE") == 0))
					{
            if (detection(1))
              printf("DEVNAME=%s \tID_TYPE=%s \n", devname, keyvalue);
//						if ((strcmp(keyvalue, "video") == 0)) /*---- This is for a webcam ----*/
//						{
//							if ((strncmp(devname, "video", 5) == 0)) /*---- This is for the video (and not the audio) ----*/
//							{
//								if (detection(1))
//									printf("DEVNAME=%s \tID_TYPE=%s \n", devname, keyvalue);
//							}
//						}
					}
					if ((strcmp(keyname, "DEVTYPE") == 0))
					{
            if (detection(0))
              printf("DEVNAME=%s \tDEVTYPE=%s \n", devname, keyvalue);
//						if ((strcmp(keyvalue, "partition") == 0)) /*---- This is for a USB key ----*/
//						{
//							if (detection(0))
//								printf("DEVNAME=%s \tDEVTYPE=%s \n", devname, keyvalue);
//						}
					}
				}
			}
			i += strlen(buf+i)+1;
		}
	}
#if 0
	printf("RECORDED KEY VALUE PAIRS\n");
	for (j=0;j<8;j++)
	{
		//dump table content
		printf("====> KEY=%s VALUE=%s\n", hotplug_table_key[j][0], hotplug_table_key[j][1]);
	}
#endif
	die("poll\n");

	// Dear gcc: shut up.
	return 0;
}


/*
Note that for example str = " x = 193" will be store string " x " to keyname so you still need to get rid of the extra spaces
if you want flexible syntax with regards to spaces around the assignment and before the variable name.
Once you got the variable name and its integer value stored in keyname and val,
you can do like @unwind above suggest to see which variable you should assign val to, testing with strcmp.
If you have a lot of fixed name variables, you could have a table and use a look-up function to make this prettier
than one heck of a unmaintainable and error prone long list of if statements.
*/


