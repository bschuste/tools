
#define SIM_PICNUM 144
#define SIM_WIDTH  176
#define SIM_HEIGHT 144
#define SIM_SIZE 3115
         int  simHeight = SIM_WIDTH;
         int  simWidth  = SIM_HEIGHT;
         int  simPicnum = SIM_PICNUM;
         int  simSize   = SIM_SIZE;
unsigned char outputCompBuffer[SIM_WIDTH * SIM_HEIGHT * SIM_PICNUM + 4096] = {0xDE,0xAD,0xBE,0xEF} ;
unsigned char simBuffer[SIM_SIZE] = {
copying....
0x00,0x00,0x00,0x01,0x67,0x58,0x00,0x0b,0x8d,0x94,0xc1,0x62,0x62,0x00,0x00,0x00,
0x01,0x68,0xce,0x3c,0x80,0x00,0x00,0x00,0x01,0x65,0x88,0x80,0x00,0x40,0x01,0x8b,
0x1a,0x5d,0xe0,0x20,0xf5,0xbf,0x00,0x66,0x04,0x1f,0xd8,0x75,0x11,0x7d,0xe1,0x7f,
0x99,0xf3,0x8f,0x40,0xfb,0xf3,0xe2,0xa0,0x0d,0xc3,0x1f,0x42,0x88,0x60,0xc9,0xd2,
0xd0,0x5f,0xe0,0xc5,0x80,0x63,0xa5,0xe0,0x5f,0xc4,0x80,0x85,0xa7,0x32,0x0b,0x5d,
0x86,0x86,0x86,0xb5,0xac,0x6a,0x83,0xc8,0x80,0xaa,0x4c,0xdb,0x82,0x8d,0x43,0x17,
0x2f,0xfe,0x3d,0xdd,0xdf,0xdf,0xe2,0x91,0x46,0xc0,0xd7,0xa4,0x55,0xbe,0x6f,0x97,
0xbc,0x06,0x78,0x38,0xd1,0x80,0xf8,0xfd,0x8d,0x28,0x39,0x51,0xcf,0x38,0x20,0x40,
0x94,0xd3,0xb7,0xc9,0x19,0x7f,0xdd,0xcd,0xff,0xee,0xf5,0x0c,0x45,0x16,0x01,0x40,
0x7c,0x7e,0xc6,0x3f,0xb1,0xaf,0x9c,0x11,0xef,0x14,0x11,0x5f,0x29,0x13,0x4d,0x97,
0xfd,0xef,0x7b,0xbe,0xf5,0x5e,0x02,0xa8,0xa3,0xc0,0xe1,0xe0,0x4e,0x06,0x3e,0x14,
0xbe,0x2d,0xea,0x27,0xff,0x9f,0xbd,0x9d,0x6b,0xee,0x66,0x06,0x95,0xcd,0xee,0xcf,
0xb9,0x3f,0xfa,0x81,0x08,0xa9,0xcc,0xc3,0xa4,0x94,0xcd,0xff,0x1b,0xb1,0xf7,0xc1,
0x22,0x42,0x02,0xf2,0x37,0x78,0xea,0xb1,0xff,0x9e,0x54,0xd9,0x2f,0xc6,0xd3,0x7b,
0x8f,0x51,0xff,0x0b,0x73,0xec,0xbf,0x95,0xbf,0x5e,0x27,0x89,0xd7,0x35,0x54,0x9a,
0xe0,0x44,0xe4,0x74,0x6b,0xe5,0xb0,0xf6,0x85,0xd8,0x94,0x9b,0x02,0x33,0xae,0xdb,
0x53,0xfd,0x93,0x26,0x39,0x6f,0x3f,0xcf,0x3d,0x25,0x9e,0x1e,0x56,0x7c,0xba,0x36,
0xec,0x57,0x18,0xc8,0x2b,0x9c,0x2a,0x6f,0x53,0xcf,0x3e,0x87,0xb8,0x1b,0x8e,0x80,
0xed,0xcd,0xa7,0x4c,0x10,0xdd,0xde,0x59,0x41,0xbc,0xfd,0xc7,0xc6,0x4d,0x7e,0x0d,
0x9d,0xdd,0x9e,0x7a,0x66,0x79,0xf5,0x00,0x71,0x51,0x90,0x3d,0xe4,0xa0,0x98,0x27,
0x6f,0x56,0x6c,0xf0,0x15,0xb2,0xdd,0x94,0xe7,0xfe,0x95,0x78,0xc9,0xe0,0xd7,0x19,
0x1b,0xcc,0xf9,0xbf,0x3d,0x2a,0x4d,0x68,0xe5,0x9c,0x7d,0x2e,0xee,0x03,0x37,0x32,
0x01,0x16,0x62,0x2e,0xd3,0x42,0x91,0x82,0x1d,0x6e,0x3b,0x4e,0x4d,0x2e,0xed,0xf5,
0x93,0x9d,0x9a,0x0e,0xe5,0x8d,0xce,0xc1,0x40,0x5f,0x84,0x0c,0x6f,0xad,0x30,0x14,
0xe4,0x26,0xab,0x34,0x63,0xef,0xea,0x3e,0x7c,0x12,0xe3,0x76,0x03,0xbe,0x2c,0xfb,
0x50,0x10,0xb8,0xa3,0xfc,0x0c,0xdc,0x91,0xc9,0x1e,0xfc,0xfe,0xef,0x09,0xbb,0xb9,
0x3b,0xbc,0xb7,0xe0,0x5c,0xc6,0xb0,0x46,0x67,0x43,0x86,0x38,0xcb,0x0e,0x52,0x11,
0x56,0xb7,0xcc,0xc6,0x47,0xc4,0xb0,0x6a,0x97,0xfb,0xee,0xae,0xff,0xf8,0x5a,0xb6,
0x54,0x97,0x37,0xd8,0x26,0xc1,0xd6,0xf8,0xcd,0x18,0x4f,0x20,0xee,0xf1,0xd3,0xe7,
0x5f,0xff,0xe5,0xa9,0xe7,0xfc,0xdf,0x80,0xaa,0xa7,0x45,0x79,0xac,0xbf,0xff,0xc2,
0x37,0xb5,0xb5,0xd1,0x53,0xf4,0xf6,0xc5,0x55,0x6a,0xf3,0x57,0x77,0x62,0xf1,0x91,
0x70,0xc1,0x78,0x8d,0xdf,0x3c,0xe6,0x89,0xaf,0xe7,0xf1,0xf1,0x93,0x5e,0x3a,0x93,
0x2c,0xad,0xe2,0x3e,0xbe,0xff,0x48,0x18,0x25,0x3b,0x8f,0x8c,0x8e,0xc7,0x30,0x95,
0x75,0x9e,0xe6,0xf7,0xce,0x87,0x61,0xe9,0xe0,0x9e,0x61,0x3b,0x82,0x56,0x6c,0xeb,
0xe8,0x63,0xa0,0xbf,0x85,0xd1,0x93,0xf4,0x6b,0x0c,0xae,0xf7,0xb0,0x44,0xdb,0x7f,
0x90,0x30,0x17,0xe1,0x0f,0xde,0xb1,0xfc,0x37,0x48,0x3b,0x3e,0x0b,0x04,0xdf,0x9e,
0x99,0xe7,0xd7,0xf1,0xa8,0xe1,0x1a,0x28,0xee,0x3f,0xfb,0xbb,0xbf,0x0a,0xe0,0x42,
0xd3,0x99,0xb0,0xc7,0x79,0xec,0x68,0x30,0x33,0xa5,0x1a,0x1b,0x47,0x74,0x1e,0xf7,
0xbb,0xf7,0xe9,0xf0,0x23,0x11,0x47,0x71,0x90,0x88,0x04,0x2d,0x73,0x17,0xfa,0x95,
0x28,0x72,0x69,0x7d,0x81,0x7a,0x7a,0x99,0xd5,0xc4,0xe1,0xd9,0x32,0xcb,0xee,0xc6,
0xde,0xff,0xe7,0xa8,0x7c,0x57,0xa7,0x48,0x2f,0x7a,0x76,0x63,0x6f,0xf7,0xe6,0x95,
0xff,0xfd,0xdf,0x17,0x87,0x78,0x2f,0xfe,0x9c,0x30,0x32,0x1e,0x64,0x54,0x65,0xa6,
0x9f,0xff,0xe2,0x2e,0xe3,0xa0,0xed,0x2d,0x87,0x7c,0xce,0x29,0xa5,0xb8,0xbf,0xb6,
0xdf,0xff,0xfd,0x05,0x72,0x11,0x52,0xde,0x6a,0x09,0x90,0x7e,0x94,0xf8,0x08,0x8a,
0x11,0xdf,0x8b,0x86,0xcf,0xad,0x16,0x32,0x50,0xa0,0xff,0xf5,0x58,0xed,0xf9,0x64,
0x13,0x3b,0xbb,0x93,0x2c,0xae,0x56,0x4e,0x68,0xef,0x3c,0xff,0x03,0xce,0x13,0xbe,
0xf4,0x8a,0xc4,0xcc,0x54,0x76,0x69,0x23,0x72,0xe0,0x6d,0x63,0x36,0xee,0xe7,0xe8,
0xa9,0x94,0x4f,0x30,0x33,0xb9,0x93,0xec,0x65,0x8c,0x10,0xfd,0x9c,0x54,0x0c,0xbb,
0x99,0xad,0x36,0xe5,0xa7,0xe0,0x9b,0xd1,0xdc,0xb8,0x00,0x33,0xbb,0xc1,0xc2,0x5e,
0xe0,0x3a,0x44,0xf8,0xd7,0xc9,0xae,0x26,0x9b,0xa7,0xe3,0x25,0xec,0x62,0xf2,0x69,
0xd4,0x11,0x38,0xc4,0xe8,0x7b,0x9d,0x73,0xaf,0xeb,0xff,0xe8,0x13,0xc3,0x27,0x92,
0xbe,0xdf,0x3f,0x70,0x8e,0x4c,0xff,0xfe,0x6d,0xf5,0xbb,0xca,0x12,0xc9,0x7a,0x40,
0x31,0x22,0x1e,0x20,0x37,0x3a,0x02,0xe6,0x35,0xc7,0x9e,0xb9,0x7c,0x84,0x2c,0xb7,
0xe5,0xe1,0x45,0x55,0xe0,0x00,0x04,0x17,0xa6,0x4f,0x7f,0xfd,0xdf,0x5c,0xc5,0xf5,
0xe5,0x26,0xe3,0xf0,0xe1,0xed,0xbd,0x2f,0xb4,0x14,0x7c,0xfc,0xf9,0x22,0xfe,0xb7,
0x1f,0xeb,0xbe,0xd2,0xaa,0xa4,0xaf,0xdf,0x5e,0xdf,0x1a,0xf4,0x97,0x54,0x2a,0xe5,
0xfb,0xbf,0x88,0x7b,0xbf,0x55,0x05,0x3a,0x19,0xba,0xe4,0xf5,0x2e,0xef,0x5d,0xc7,
0xd4,0xb9,0x3d,0xa9,0xf4,0x9a,0xa2,0xaf,0x81,0xfb,0xf0,0xbe,0x79,0x9e,0x75,0x21,
0xfc,0x60,0x48,0xc4,0x3d,0x3c,0x66,0x96,0x45,0x46,0x36,0x63,0x39,0x7a,0x66,0xa9,
0x91,0x7a,0xdd,0x4b,0x67,0x5f,0xac,0xf7,0xe6,0xb5,0xe2,0x75,0xd2,0x6e,0xc5,0xb3,
0x9b,0x8c,0x75,0x39,0x73,0x3f,0xa6,0x07,0x0e,0xec,0x4b,0xbc,0xc4,0x7a,0x96,0xf9,
0x6f,0x1a,0x1d,0x8e,0x3f,0xa0,0x39,0x5d,0x46,0x02,0x06,0xf4,0xde,0x21,0x0f,0x84,
0x1d,0xdd,0xde,0x15,0xc0,0x55,0x14,0x5a,0x02,0x20,0x09,0xf0,0xbe,0x67,0x09,0xec,
0x54,0x35,0x22,0x2f,0xb3,0x1b,0xa5,0xef,0xb8,0x90,0x8a,0xbb,0xf7,0xc1,0x19,0x9d,
0x09,0xcd,0x5e,0x84,0x8c,0xc2,0x07,0x70,0xed,0xd3,0x4b,0xaf,0xac,0x64,0x59,0x1b,
0x37,0xb7,0x3b,0xbd,0xdf,0xc7,0x9e,0x26,0x03,0x51,0x45,0x55,0xcc,0x78,0xa2,0x50,
0xd4,0x31,0xd6,0x9f,0x3d,0x79,0x9a,0xad,0xba,0xf9,0xb1,0x1e,0xe0,0xdc,0x2d,0xc6,
0x29,0xc6,0x5b,0xad,0x8f,0xae,0xe3,0x79,0x6d,0xb9,0xff,0xef,0xe6,0x10,0x35,0x4e,
0xb8,0x87,0x88,0x73,0xd3,0xab,0x86,0x91,0xbf,0x3e,0xb3,0x50,0x08,0x3d,0xda,0x25,
0x71,0x5c,0x28,0xad,0xee,0xdb,0xe9,0xb5,0xaa,0xf2,0x00,0xdb,0x4a,0x2d,0x77,0xe0,
0xf3,0xe0,0xf7,0x8d,0x1c,0xe1,0x3f,0x97,0x55,0x77,0x62,0xdc,0xea,0x8c,0x21,0xb3,
0x75,0x3a,0xc4,0xbb,0x69,0x6a,0x7a,0x89,0xf1,0x69,0xf7,0xf0,0x2c,0xdb,0x54,0x6b,
0xf1,0xc9,0x75,0x48,0x7f,0xfb,0x0f,0xf0,0xaf,0x9d,0x4b,0xf3,0xe7,0x15,0x37,0x9f,
0x7f,0x0c,0x2e,0xd0,0x66,0xca,0xe0,0x57,0x19,0x95,0xe1,0xa7,0xbf,0x02,0x2e,0x14,
0xcd,0x03,0xc3,0x33,0x99,0xe7,0xe8,0x64,0x60,0x64,0xfc,0x60,0x65,0xc7,0xc6,0x4b,
0xd1,0xf0,0x70,0x8b,0x00,0x04,0x3c,0x2a,0x64,0xcd,0x86,0x60,0x36,0xd8,0x35,0xf9,
0x99,0x7f,0x55,0x9f,0xa7,0xd7,0xe7,0x34,0x9a,0xfd,0x8a,0x96,0xef,0xb6,0x63,0x3d,
0x1d,0x2f,0x2f,0xf5,0x35,0x8a,0xad,0x51,0x7e,0x2e,0xbb,0xba,0xb1,0x77,0xbb,0xe3,
0x40,0xeb,0xa2,0x50,0x62,0x2a,0xce,0x0f,0x7d,0xff,0x79,0xb2,0x36,0xe9,0x00,0x27,
0x80,0xc7,0x48,0xcf,0xb7,0xff,0xb7,0xb6,0xc5,0x2c,0xd5,0x6f,0x57,0x4b,0xe2,0x7e,
0x5e,0xd6,0x14,0x55,0xb9,0xdb,0x98,0xb3,0x54,0xd4,0xf7,0xbe,0xa0,0x75,0x84,0xbb,
0x75,0x6a,0xf8,0xc4,0x38,0x92,0xe5,0x55,0x8b,0x95,0x48,0xe4,0xe9,0x03,0x9f,0x4a,
0x35,0xaa,0xf4,0x4a,0xd6,0x21,0xd8,0xef,0xad,0x6d,0xf7,0xd7,0x3c,0xe2,0x7a,0x9a,
0xac,0x13,0xcc,0x54,0xcd,0x70,0x20,0x4b,0x9b,0x27,0x07,0x57,0xd2,0xd8,0x5b,0x8c,
0x7f,0x5c,0x40,0xcd,0x66,0x70,0xaf,0x0d,0x0d,0x4b,0xbb,0xf8,0x19,0xe7,0x0c,0xeb,
0x8a,0xd0,0x37,0x70,0x89,0x9d,0x49,0x85,0x35,0xbd,0x87,0xb0,0x86,0xca,0x76,0xe8,
0x87,0x60,0x17,0xd6,0xd2,0x6f,0xdf,0x5d,0xef,0x23,0x84,0xa7,0xac,0x9a,0x26,0x13,
0x3f,0xa3,0x45,0x5f,0xda,0x5b,0xeb,0x51,0xce,0x8b,0x44,0xc2,0x11,0x5b,0x58,0x9f,
0x26,0xb9,0x2b,0x53,0xdd,0xa9,0xc3,0x1f,0x9e,0xec,0xcf,0x63,0x59,0xe2,0x1c,0xaa,
0x7b,0xfc,0x26,0xef,0x02,0xc4,0xc6,0xb7,0x0e,0x03,0xcb,0x7d,0x9e,0x7a,0x30,0xcb,
0xfc,0x5c,0x97,0xa9,0x0b,0x95,0xce,0x64,0x9e,0xfb,0x6e,0xa6,0x53,0x8f,0xae,0xee,
0xf1,0x3b,0xf4,0x9d,0x54,0x49,0xb7,0xe3,0xe8,0x15,0x7d,0x53,0x65,0xbf,0x37,0xca,
0xff,0xe7,0xf2,0x6f,0xed,0x0a,0xb5,0x76,0x27,0xab,0x1b,0xbb,0x3e,0x5f,0x71,0xf9,
0x4c,0x7f,0xfc,0xdb,0x6d,0x9e,0x75,0x3f,0x8d,0x55,0x69,0xd0,0x35,0x23,0x6b,0x66,
0xb8,0x15,0x0a,0x04,0xbe,0xec,0xc6,0xab,0xfe,0x85,0xff,0x59,0x8c,0xf3,0xa7,0xbd,
0xe9,0xe1,0xa6,0xa5,0xc0,0x80,0x6b,0x46,0xce,0xbf,0x1d,0x85,0xd4,0xc2,0xd7,0x9a,
0x95,0xaf,0xdf,0xc2,0x29,0x03,0xfc,0x70,0xa6,0xbd,0x7d,0x12,0xaf,0x14,0x9a,0x7c,
0x17,0xed,0xeb,0xef,0xf9,0x61,0xce,0x91,0x33,0x44,0x40,0x3a,0x7d,0xbd,0xc7,0x4d,
0xa7,0x38,0xad,0xfb,0x21,0x34,0x2f,0xb2,0xb7,0x3b,0x74,0x0b,0xcf,0xb1,0x64,0x0e,
0x0e,0x22,0xd1,0xcd,0xe6,0x9b,0x7d,0x0d,0xaa,0xa9,0xaa,0x55,0x5e,0x2e,0x5e,0xfe,
0xed,0x1b,0x37,0xf7,0x1a,0x83,0xf6,0xe9,0xfb,0x93,0xe3,0x17,0x2d,0xeb,0xda,0xbc,
0xaa,0x6b,0xbc,0x28,0xa8,0xad,0xb1,0x3a,0xc5,0xac,0xf9,0xc3,0x5a,0xa8,0xb5,0x4d,
0xd3,0xaa,0xef,0x5e,0x8b,0xde,0xf7,0x4f,0x8d,0xe5,0xd8,0xf4,0x34,0x5f,0x45,0xd1,
0x2f,0xd4,0xeb,0x02,0x87,0x40,0x9a,0x70,0x51,0x7b,0xc2,0x6f,0xac,0x65,0x91,0x1d,
0x3c,0xbe,0x3f,0xbb,0xc2,0xbd,0xc4,0x39,0xbf,0x49,0xdb,0x2c,0xa1,0x4a,0xea,0xfc,
0xe6,0x87,0x7c,0xdc,0x39,0xce,0xe1,0x67,0x2b,0x17,0x45,0x39,0xa1,0x1a,0xa6,0x9b,
0xac,0x7d,0xa2,0xde,0x2b,0x94,0x19,0x7c,0xcb,0xc2,0x35,0xf7,0x52,0xba,0x36,0x9f,
0x6b,0x74,0x34,0x14,0x9e,0x0d,0xbe,0x87,0x8e,0xf6,0xbc,0x54,0x69,0xd1,0x53,0xbe,
0xfd,0xdd,0x42,0x8f,0xe3,0xc3,0xfb,0x3c,0x3f,0x4d,0x0a,0x9a,0x0a,0xf3,0xf9,0x77,
0xf0,0x26,0x7b,0x34,0x7e,0xa9,0x51,0x9f,0x45,0x7c,0xb6,0xbb,0x49,0x0b,0xac,0x5d,
0x63,0xf5,0x9a,0x19,0x1f,0xe4,0x1c,0x7b,0xf6,0xf8,0x77,0x52,0xdd,0xc1,0x8d,0x96,
0x04,0x0d,0xf1,0x7d,0xd8,0x0a,0xf9,0x66,0x11,0x1a,0x18,0x5d,0x80,0x9c,0xdf,0x12,
0xc0,0x1a,0xc6,0x05,0x6d,0x5b,0xaf,0x2a,0xab,0x0a,0xaa,0xda,0xe1,0x51,0x97,0x8f,
0xab,0x63,0x12,0x07,0x8c,0x86,0xe9,0x6f,0x51,0xe9,0x7f,0x5b,0x08,0x82,0xff,0xa3,
0x6a,0xe1,0xca,0x66,0x43,0xc0,0xeb,0xde,0xbe,0x04,0xba,0xb7,0xc8,0xb0,0xdc,0x67,
0x13,0x8d,0x8a,0x02,0x3c,0xe3,0x28,0xc7,0x0d,0x89,0xdf,0xee,0x15,0x02,0xfb,0xbd,
0xde,0xd0,0x1b,0x6a,0x58,0x2f,0xe7,0xce,0x2f,0x92,0x09,0x66,0x36,0x51,0x01,0x53,
0x19,0x88,0xd8,0xd4,0x2f,0x8b,0x58,0x4e,0xc2,0x85,0xa3,0x0f,0x15,0x19,0x63,0x86,
0x0f,0xee,0x7e,0x21,0xd8,0x00,0x00,0x00,0x01,0x41,0x9a,0x00,0x02,0x09,0x83,0x14,
0xbb,0xfe,0x54,0x75,0xae,0xba,0xd5,0xf7,0xc5,0x6a,0x78,0x78,0x47,0x26,0x05,0x7a,
0x6e,0xce,0x63,0x17,0x83,0xd7,0xe7,0x3e,0xfb,0xca,0xb9,0x85,0x18,0x40,0x91,0x75,
0x79,0x5f,0xdc,0x9c,0x8f,0x91,0xef,0xf9,0x54,0x85,0x23,0xc3,0x7c,0xf8,0x78,0x78,
0xf0,0x88,0x46,0x14,0x8e,0xf7,0x5b,0xde,0x24,0x97,0x12,0x47,0x9b,0xf5,0x82,0x91,
0x5c,0x0b,0xa1,0x61,0x7c,0x56,0x2b,0x6a,0xf9,0xbf,0xc7,0xea,0xe8,0x2b,0xcb,0x8f,
0x35,0x1f,0x97,0xa8,0x7c,0x7c,0x15,0x8a,0x88,0x31,0xf4,0xe0,0xac,0x49,0x62,0xbe,
0xdd,0x75,0xad,0x95,0x82,0x61,0x61,0x4e,0xe3,0xff,0x44,0x96,0xf5,0x95,0x17,0xf9,
0x71,0xce,0x1f,0x2c,0xbe,0x62,0x66,0x10,0x77,0xb1,0xb2,0xf6,0x34,0xa3,0x6e,0xef,
0xcb,0x76,0x34,0xf4,0x6e,0x2d,0x87,0x84,0x9b,0xc3,0x82,0xb7,0x2b,0x54,0x0a,0x06,
0x02,0x81,0x61,0xb6,0x43,0x88,0x73,0xa5,0x7a,0xd5,0x35,0xcb,0xcb,0xab,0x40,0x30,
0x85,0x47,0x61,0x98,0x27,0xa6,0x7b,0x9a,0x75,0xb9,0xb0,0x1a,0xf9,0x04,0xde,0xf9,
0xbf,0xe4,0xdd,0x1b,0xd9,0xe3,0xe4,0xf2,0xf9,0x6d,0xc4,0x49,0x0b,0x85,0x84,0x93,
0x0e,0x05,0xc5,0x6f,0xf3,0xc8,0x18,0xba,0xbf,0x8a,0xdb,0xfa,0x55,0x79,0x6e,0x42,
0xc2,0xd1,0xc6,0x4f,0x1b,0x44,0xc2,0x55,0x7e,0xf6,0xa6,0xe7,0xfd,0x2b,0xc2,0x5e,
0xaa,0xbe,0x52,0xf0,0x8c,0x16,0x72,0xe5,0xcf,0x9d,0xde,0xc6,0x56,0x4b,0x08,0xfc,
0xac,0x12,0xdc,0x20,0x5c,0x10,0x66,0x71,0x9e,0xea,0x38,0x55,0xcb,0xb1,0x5b,0xbc,
0x43,0xec,0x1e,0x81,0xc4,0x31,0x1d,0x2b,0x37,0xfd,0x6a,0xbe,0x96,0xb4,0xee,0xae,
0x70,0x57,0xc5,0xe2,0x98,0xbc,0x5f,0x13,0xa7,0xb7,0x8f,0x8c,0xd4,0xb8,0x3f,0xcc,
0x2c,0xf8,0xad,0xfa,0x0e,0xf8,0x96,0x3a,0xf0,0xf1,0xc7,0xb9,0x7b,0x15,0x0b,0x65,
0xf1,0x37,0xdc,0xb5,0xdd,0xdc,0x8b,0x79,0xf5,0x9a,0x8c,0x88,0x99,0xad,0x20,0x7a,
0xf9,0x36,0xe6,0xaa,0xf4,0x7d,0xdb,0xd5,0x63,0x7b,0xc5,0x72,0x18,0xa0,0x2f,0x9b,
0xcd,0x3c,0xce,0x94,0xcf,0xae,0xb5,0x88,0xaf,0xb8,0xaf,0xbd,0x8d,0x0d,0x63,0x84,
0x0a,0xbb,0xef,0x6b,0x2b,0xb8,0xf6,0x30,0x68,0x7b,0x1c,0x18,0x07,0xf1,0x45,0xba,
0x76,0x9b,0xd6,0xeb,0x96,0xf7,0x56,0x31,0x12,0x89,0x8f,0x63,0xcb,0xa9,0xa1,0x70,
0xae,0xca,0x2c,0xe1,0x2d,0x5e,0x95,0x4a,0x5a,0x63,0xa2,0x56,0x4b,0x1d,0x15,0x12,
0xde,0xee,0xfd,0x4d,0xc2,0xcd,0x5c,0xd9,0x41,0x9a,0xaa,0x68,0x18,0x0c,0x62,0x6c,
0x18,0x0f,0x04,0xc3,0xc7,0x45,0x67,0xce,0xf5,0x4f,0x3c,0x28,0x82,0x71,0x2e,0x5d,
0xe9,0x86,0xf1,0x48,0x0f,0xb2,0xcf,0xff,0xbf,0x78,0x00,0x00,0x00,0x01,0x01,0x9e,
0x00,0x04,0x05,0xa8,0x10,0x41,0xc6,0x1e,0x13,0x8d,0xf0,0xf9,0x92,0x8c,0x51,0xe2,
0xa7,0x24,0xe9,0x3d,0x87,0x84,0xee,0x7d,0x41,0x1a,0xc5,0xc3,0x07,0x09,0x1e,0x25,
0x88,0xec,0x84,0xc5,0x42,0xe6,0x09,0x0c,0x88,0x24,0xa2,0x83,0x26,0xd4,0x36,0x24,
0xd8,0xdf,0xba,0x50,0x86,0x25,0x49,0xd9,0xcf,0xb0,0xa4,0xbc,0x3d,0x52,0xa1,0x68,
0x9c,0x90,0x00,0x00,0x00,0x01,0x41,0x9a,0x00,0x04,0x11,0x41,0x95,0x7d,0xb3,0x60,
0x85,0xb5,0xe2,0xa9,0x76,0xdb,0x1c,0x1a,0x9a,0x6b,0xef,0x6e,0x55,0xbb,0x12,0xb8,
0x8f,0x2f,0x9a,0x4e,0x19,0x93,0x8f,0x78,0xa3,0x7a,0x4d,0xa2,0x64,0xb8,0xb0,0x08,
0xb0,0x35,0xdc,0xb0,0x3b,0xd2,0x9b,0x98,0x25,0x4a,0xdc,0x27,0xd9,0x59,0x1a,0xbb,
0x75,0xe2,0x52,0x22,0x67,0x82,0xdd,0x56,0xdf,0xbf,0x1f,0x65,0xdc,0x12,0xe5,0xf1,
0xcc,0x8c,0x19,0x54,0xce,0x3e,0xb2,0x9f,0x2e,0x99,0x4b,0xf9,0x4b,0x91,0xd3,0x9a,
0x5f,0x04,0x2a,0xd2,0xb6,0xf1,0x07,0xaf,0xad,0x40,0x27,0x01,0xfb,0x55,0x95,0x69,
0xc1,0x68,0xa3,0x28,0xd7,0xac,0xb9,0x34,0x43,0xf0,0xac,0x9c,0xee,0xac,0x11,0xf5,
0x46,0xbd,0xae,0x29,0xb1,0x73,0xd1,0xa7,0x73,0xd9,0xed,0xb8,0xeb,0x97,0xfb,0x05,
0xab,0x16,0xc5,0x50,0x86,0xe9,0xd2,0x06,0x5c,0x47,0x16,0xb6,0x56,0x51,0x77,0xd5,
0xe9,0x33,0x95,0x7d,0xeb,0xb2,0xbb,0x63,0x5b,0xe5,0x4a,0x4e,0xf3,0x2a,0x44,0xda,
0x71,0x4e,0xf2,0xa8,0x6b,0x68,0x86,0x65,0x69,0xec,0xeb,0x5b,0xfd,0xce,0x04,0xe0,
0xea,0x25,0x51,0x12,0x93,0x66,0xc1,0xc8,0x58,0xed,0x61,0x0f,0xc2,0x54,0x73,0x31,
0x85,0x30,0xa8,0x89,0x5a,0xa4,0xfe,0x98,0x68,0x81,0x44,0x01,0x71,0x04,0xcc,0x76,
0x5a,0x0d,0x37,0x16,0x25,0xe9,0xb1,0x97,0xa0,0x60,0x17,0x90,0x19,0xf6,0x04,0xb0,
0x4e,0xe7,0xce,0x5c,0x4f,0x6c,0xc3,0xf1,0x4e,0xaa,0x5d,0x78,0xad,0xa7,0xab,0xdc,
0x56,0xfd,0x6b,0x2b,0xb4,0x0a,0x21,0x73,0x8b,0x58,0x77,0x71,0x60,0x9d,0x09,0x7a,
0xd8,0x4b,0xb5,0xd9,0x76,0x84,0x18,0x78,0xf1,0xa2,0x02,0x01,0x55,0x10,0x28,0x20,
0xda,0x7c,0xf5,0xa3,0x01,0xaf,0x19,0x85,0xc4,0x54,0x27,0xe2,0x44,0xb4,0x58,0x3f,
0x2d,0x26,0xb3,0x57,0x67,0x12,0x49,0x31,0x2b,0x1d,0xf6,0xfe,0x9d,0x17,0xd2,0x88,
0x3c,0x58,0xe0,0xf2,0xb5,0x86,0xac,0x3a,0xaa,0x6c,0x18,0x6f,0xc4,0xff,0xbf,0x96,
0x19,0x5d,0xc5,0x44,0xc5,0xe2,0xfd,0xaa,0x93,0x7d,0x71,0x01,0xb2,0x12,0xe2,0xb9,
0x33,0x82,0x93,0x38,0x91,0xa3,0xfa,0xef,0xd7,0x2f,0xa4,0xc5,0x9c,0x48,0x78,0x40,
0xa9,0x71,0xdd,0x34,0xad,0x62,0xb5,0xa1,0x4d,0x0a,0x2a,0x92,0xfc,0xc2,0x08,0x20,
0x33,0xab,0xd4,0x38,0x21,0xbe,0xf4,0xde,0xcc,0x39,0xb3,0x22,0x2d,0x41,0x2b,0x22,
0xcd,0xfc,0x51,0xc4,0xb1,0xcf,0xb6,0x1a,0x7f,0x4e,0xc1,0x36,0x56,0xec,0x77,0x1e,
0x70,0x11,0xc0,0x34,0x1f,0xc5,0xbe,0x2f,0xdb,0x27,0x98,0x55,0x11,0x70,0x1b,0x6f,
0x3a,0x4a,0xbe,0x92,0x69,0x2f,0xbb,0xb5,0xef,0x39,0x85,0xaa,0x9b,0x24,0xda,0x25,
0xcf,0x2d,0xe3,0xe0,0x96,0xf4,0x1e,0x29,0x62,0x1a,0x66,0xf2,0x3a,0xdb,0x20,0xf1,
0x33,0xe7,0x01,0x3c,0x2c,0x5d,0x25,0x56,0x43,0x2a,0xea,0x8d,0x33,0x4e,0xb9,0x9a,
0xad,0xcb,0xcc,0x08,0x34,0x17,0xd8,0xaf,0x0e,0xa9,0x8c,0xde,0x39,0x05,0xcc,0x31,
0x6e,0x2b,0xd6,0xd8,0xac,0x90,0x35,0x7d,0xbb,0xff,0x71,0x75,0x99,0x00,0xdd,0x9c,
0xe9,0x59,0xe4,0x41,0x11,0x3c,0x95,0x2f,0xdb,0x2f,0x83,0xfe,0xff,0x6f,0x1d,0x02,
0x98,0x14,0xc6,0x23,0x8f,0xe4,0x53,0xa8,0x39,0x3a,0x0e,0x5d,0x17,0x2c,0xe9,0x43,
0x3b,0x69,0x73,0x1c,0x87,0x38,0xbc,0x53,0x66,0xd6,0x1d,0x1f,0xaa,0xa6,0xe2,0xde,
0xf9,0xb0,0xee,0xd7,0x27,0xc4,0x7b,0x33,0x12,0x52,0x64,0x57,0x50,0x15,0x7b,0xc5,
0xe0,0x24,0x67,0x83,0x0c,0xbb,0xa9,0x2d,0x75,0x17,0x91,0xba,0xe6,0xeb,0x8b,0x73,
0x4f,0x0a,0xaf,0xd7,0xab,0xee,0xe2,0xb6,0xf4,0xdd,0xc4,0x8c,0x1f,0xbb,0xe6,0xcc,
0x0c,0xef,0xa9,0xab,0xbd,0x06,0x31,0x0d,0xfc,0xb8,0xc6,0x6f,0x42,0x0f,0xb4,0x96,
0x6c,0xd2,0x7c,0x72,0x96,0xdf,0xdd,0xdd,0x2d,0x66,0x7e,0xf2,0xbd,0x90,0x4b,0x08,
0x55,0x55,0x51,0x2a,0x43,0x4c,0xb3,0xa4,0x15,0xe2,0xe1,0x48,0xa6,0x77,0x14,0x92,
0x32,0x2e,0x56,0x4a,0x46,0x80,0xd3,0x12,0x4b,0x44,0x8b,0xe4,0x2d,0xda,0xc7,0xd3,
0xd6,0x42,0x85,0x22,0x61,0x91,0x52,0x15,0xcf,0xe5,0x21,0x7a,0x69,0x7e,0x8b,0xcb,
0x85,0xcf,0x8b,0xe5,0xc0,0xd3,0x2d,0xba,0xc7,0x7d,0x00, };
