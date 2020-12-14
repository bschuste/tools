/*!
 ***********************************************************************
 **
   \file 
       elUdpPattern.h
   \brief
       Test Patterns for UDP stress 
   \date
       Last update: 2005/07/13
   \author
       Brigitte Schuster
 **                  
 ***********************************************************************/  
#ifndef TST_PATTERN_H_
#define TST_PATTERN_H_

/*
--> First Packet

   B0     B1     B2     B3     B4     B5 ................. Bn
+--------------------------------------------------------------+
| 0x47 | 0xZZ | 0xZZ | 0xZZ | 0xZZ | 0xP0 | 0xP0 | .... | 0xP0 |
+--------------------------------------------------------------+
         MSB                   LSB
  Hdr   <------- ZZZ Counter ------><---- Pattern Length ----->

--> Second Packet

   B0     B1     B2     B3     B4     B5 ................. Bn
+--------------------------------------------------------------+
| 0x47 | 0xZZ | 0xZZ | 0xZZ | 0xZZ | 0xP1 | 0xP1 | .... | 0xP1 |
+--------------------------------------------------------------+
         MSB                   LSB
  Hdr   <------- ZZZ Counter ------><---- Pattern Length ----->

 */

#define TOTAL_AMOUNT_PATTERNS           4*64        // in bytes
#define DEFAULT_UTILIZED_PATTERNS       8           // in words should be less or equal than the 64 mentioned above

unsigned char patternTable[TOTAL_AMOUNT_PATTERNS] = {
      //P0
      0xFF, 0xFF, 0xFF, 0xFF,
      //P1
      0x00, 0x00, 0x00, 0x00,
      //P2
      0xAA, 0xAA, 0xAA, 0xAA,
      //P3
      0x55, 0x55, 0x55, 0x55,
      //P4
      0x55, 0xAA, 0x55, 0xAA,
      //P5
      0xAA, 0x55, 0xAA, 0x55,
      //P6
      0xF0, 0x0F, 0xF0, 0x0F,
      //P7
      0x0F, 0xF0, 0x0F, 0xF0,
      //P8
      0xE0, 0xFE, 0x0F, 0xFF,
  };

#define DEFAULT_HEADER_LEN               5                // bytes
#define DEFAULT_BYTE_RATE                10000            // bytes per second
#define DEFAULT_PATTERN_LENGTH           (64 - DEFAULT_HEADER_LEN) // bytes
#define DEFAULT_PATTERN_HEADER           0x47                    
#define DEFAULT_TEST_DURATION            2                // in seconds
#define DEFAULT_COUNTER_VALUE            0xFE

#endif
