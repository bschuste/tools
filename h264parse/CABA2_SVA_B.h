
#define SIM_PICNUM 3
#define SIM_WIDTH  176
#define SIM_HEIGHT 144
#define SIM_SIZE 2608
         int  simHeight = SIM_WIDTH;
         int  simWidth  = SIM_HEIGHT;
         int  simPicnum = SIM_PICNUM;
         int  simSize   = SIM_SIZE;
unsigned char simBuffer[SIM_SIZE] = {
unsigned char outputCompBuffer[SIM_WIDTH * SIM_HEIGHT * SIM_PICNUM + 4096] = {0xDE,0xAD,0xBE,0xEF} ;
0x00,0x00,0x00,0x01,0x67,0x4d,0x40,0x15,0x8d,0x94,0xc1,0x62,0x62,0x00,0x00,0x00,
0x01,0x68,0xee,0x38,0x80,0x00,0x00,0x00,0x01,0x65,0x88,0x80,0x00,0x40,0x01,0x9f,
0x6f,0x90,0x8c,0xa5,0xe3,0xde,0x45,0xde,0x3f,0xf4,0x8f,0xee,0x51,0x42,0xc2,0x0f,
0xed,0x0e,0x24,0xb0,0x55,0x17,0x80,0xa9,0x85,0xa8,0x0e,0x6b,0x0f,0x55,0x82,0x7a,
0x27,0xa7,0x81,0xfd,0xbc,0x93,0xe1,0x98,0xf2,0x5d,0xde,0x36,0x0c,0x66,0x48,0xd8,
0x15,0xb6,0x10,0x32,0x54,0xb8,0x50,0x48,0x1c,0xd9,0x02,0x96,0xf0,0x22,0x33,0x26,
0xb4,0xff,0xb7,0xd7,0xd4,0x71,0x36,0x43,0x8b,0x5d,0xc6,0xfe,0x7e,0x3f,0xf8,0x04,
0xbd,0x13,0xf0,0x20,0x85,0xab,0x98,0x8f,0x22,0xf6,0xeb,0x20,0x7a,0x6b,0x57,0x64,
0x80,0x0f,0xd6,0xda,0xdf,0xda,0xb3,0x72,0xc1,0x2f,0x40,0xb3,0x85,0x0a,0x81,0x04,
0x1b,0x02,0xf8,0x62,0x86,0xec,0x26,0x50,0x41,0xa1,0xc8,0xfd,0xcf,0xe9,0x25,0x86,
0x9d,0xba,0xcb,0x88,0x3f,0x86,0xfe,0x0e,0x0a,0xd2,0xdc,0x2c,0x56,0x6d,0xa1,0x5b,
0xe1,0x3b,0x2d,0x7c,0xc6,0xe6,0x18,0x86,0x8c,0xf4,0x9f,0x55,0x7d,0x7f,0x11,0x12,
0x7b,0x7f,0x26,0x93,0x99,0x2d,0x67,0x4d,0x8d,0x95,0x99,0x71,0x2e,0x87,0xeb,0x76,
0x69,0x8f,0xd5,0x45,0xaf,0x04,0xd2,0x1b,0x25,0x57,0x0f,0xe8,0x6b,0xa5,0x0a,0xc5,
0xfa,0xd0,0xca,0xd1,0xbe,0x06,0x19,0xc8,0x0f,0xb3,0x0a,0x11,0x57,0x8c,0xfe,0x64,
0x65,0x4a,0x6d,0xa4,0x0c,0x40,0x43,0x52,0x9e,0xe9,0xe0,0x24,0x06,0xb2,0xd2,0xf4,
0x97,0x9a,0xed,0x86,0xe6,0xab,0x1c,0x7a,0xfc,0xb4,0xcd,0x26,0xbe,0x2c,0x0b,0x1a,
0x23,0x08,0x05,0x82,0xa7,0xde,0xa6,0x91,0xfc,0xd1,0xe8,0xe6,0x00,0xd2,0x22,0xc1,
0xc9,0x0d,0xa3,0x41,0xa8,0x28,0xd4,0x53,0xc2,0xd1,0x85,0x6c,0x33,0x1a,0x79,0x4f,
0xf8,0xa5,0x20,0x2c,0x4d,0x94,0x96,0x45,0x13,0xca,0xf4,0xd7,0x42,0x06,0xd5,0xe2,
0xf6,0xab,0x43,0x02,0xe0,0xf1,0x7b,0x30,0x64,0xb6,0x8c,0xa1,0xa5,0x97,0x4c,0x13,
0x20,0x73,0xda,0x54,0xe3,0x50,0x93,0x90,0x04,0x71,0x79,0x44,0x7a,0x91,0x91,0xa6,
0xd6,0xbb,0x4e,0x44,0xfc,0xb4,0xc8,0xe9,0x00,0xec,0x87,0xef,0xaa,0x49,0x3e,0xac,
0x44,0xd6,0x37,0x5b,0xab,0xc5,0x0d,0x82,0xb7,0xb3,0x6a,0x1a,0x78,0xfb,0x4f,0x4e,
0x42,0xd9,0xc7,0x79,0x7a,0x36,0xff,0x57,0x12,0x25,0x64,0x13,0x61,0x53,0x6e,0x37,
0x95,0x5b,0x89,0xb2,0x37,0x10,0x5b,0xe5,0x28,0x80,0x69,0x34,0x5c,0x47,0xe6,0x8e,
0x64,0x81,0xcd,0xfe,0x8d,0xd2,0xa4,0x64,0xb7,0x6a,0xc5,0x03,0xa0,0x5d,0x74,0x07,
0xa9,0x4e,0x3e,0xbe,0x2b,0x40,0xf8,0xd1,0xc8,0xd9,0xf4,0xf6,0x6a,0xee,0x5e,0x35,
0xe1,0x97,0xba,0x64,0xfd,0xfa,0xf7,0x9c,0x9b,0x63,0xc4,0x24,0x86,0x9b,0xcd,0x91,
0x8f,0x77,0x98,0xbf,0xd2,0x2e,0x41,0x4a,0xd6,0x41,0x78,0xca,0x44,0x26,0x7a,0x45,
0x7b,0xc0,0x0f,0x31,0x07,0xbf,0x79,0xf1,0xb1,0x0e,0xe0,0x6b,0x31,0xcf,0xe3,0x10,
0x53,0x2d,0xa4,0x73,0xb2,0x06,0x86,0x0a,0x74,0x54,0xa3,0xdf,0xe3,0x10,0x5f,0x0e,
0x1d,0x4b,0x30,0xdb,0x06,0x65,0xe4,0x81,0x9f,0x9d,0xe9,0xaa,0xd7,0x78,0xae,0x38,
0x83,0x77,0x3d,0x5c,0xbd,0x77,0xfc,0xf8,0x5b,0x75,0x15,0x79,0x3a,0xef,0x60,0x93,
0x43,0xd6,0x04,0xbb,0xff,0x55,0x90,0x71,0xb6,0x6a,0xf2,0xea,0xc0,0x2a,0xf7,0x67,
0x34,0x22,0x72,0x3a,0xfb,0x79,0x54,0x4e,0x10,0xef,0x79,0x23,0xd6,0x1d,0xfc,0xbc,
0x22,0x29,0x45,0xc5,0x0a,0xd2,0x9b,0xc5,0xdb,0xf6,0xbc,0x62,0x6e,0x28,0xa8,0xed,
0xed,0xab,0xd8,0xb2,0x55,0x9c,0x7c,0xde,0xc4,0x96,0xe4,0x46,0x0d,0x83,0x9c,0x8d,
0x34,0xfa,0x73,0x75,0x5a,0x6e,0xeb,0x3e,0xc3,0x77,0x04,0x91,0xfb,0xee,0x3d,0x57,
0x7a,0x46,0x73,0xc3,0xe8,0x93,0xb1,0x67,0x17,0xcf,0xc2,0xa0,0xe3,0x7c,0x9c,0x6e,
0xfa,0x14,0xe1,0x0b,0x43,0x79,0xde,0x13,0x50,0xce,0x17,0x02,0xb3,0x43,0xf4,0x94,
0xf5,0x4d,0xc9,0xc2,0x82,0xaa,0x42,0x5e,0x4d,0xd2,0xee,0xf1,0x00,0x64,0xc9,0x38,
0x77,0x54,0x29,0x8c,0x88,0xfe,0x1a,0xee,0x4e,0x8e,0xfd,0xb7,0xa9,0xbe,0xc5,0xf4,
0x9d,0x6c,0xbd,0xc4,0x86,0x17,0x0f,0xf2,0x6b,0x47,0x42,0x93,0xd4,0xbe,0x55,0xce,
0xaa,0x7f,0xe1,0x82,0x6e,0x30,0xc6,0xa0,0xc8,0xcd,0x0e,0x49,0xcf,0x17,0xf2,0x9d,
0xe7,0x50,0x65,0x54,0x78,0x87,0xd2,0x68,0x1b,0xf5,0xdc,0x3d,0x92,0xc8,0x0d,0x55,
0x18,0x2a,0x2a,0x92,0x41,0x8c,0xe4,0x0a,0xa5,0x75,0x71,0xb9,0x07,0xa3,0x52,0x64,
0xd5,0x9a,0x59,0xd8,0xe5,0xa1,0x22,0xb5,0x2e,0x47,0xcc,0xf5,0xb8,0xfd,0x36,0x58,
0x1d,0xb5,0xf1,0x3a,0x88,0xcd,0x4c,0x4e,0x7a,0x92,0xac,0xea,0x42,0xd7,0x72,0xb5,
0x9f,0x15,0xe6,0xe6,0x10,0x12,0x79,0x19,0xea,0xb9,0x77,0xfa,0x5f,0xe5,0x4a,0x61,
0x48,0x37,0xe7,0x92,0x7a,0xc2,0x28,0x91,0xd4,0x62,0xc2,0xeb,0x71,0x44,0x68,0xbf,
0x17,0x37,0x75,0xfd,0xab,0xb7,0x37,0xbc,0xe9,0xd3,0x5b,0x5c,0x77,0x91,0xde,0x60,
0xde,0xef,0x44,0xfb,0xdf,0x07,0x65,0xc5,0x7e,0x30,0x3f,0x4c,0xab,0x85,0x03,0x8e,
0x4e,0x3c,0x34,0x5d,0x07,0x83,0xc3,0x3b,0xe2,0xb0,0x82,0xf5,0xc5,0x63,0x0e,0x99,
0x50,0x26,0x4a,0xff,0x4a,0x6c,0x35,0x3b,0x5a,0x90,0x5a,0xb1,0x1e,0x7e,0x99,0x3f,
0x1c,0xf1,0xdb,0x81,0xc7,0xda,0x7b,0xa5,0x31,0x75,0x5a,0x97,0xe9,0x68,0x46,0xfc,
0x77,0x37,0x8c,0x3c,0x43,0x55,0xd5,0x9d,0x2b,0x55,0xa7,0xc2,0x57,0x2c,0x6c,0x78,
0xdc,0x3c,0xd6,0x4c,0xa1,0xb2,0x1f,0x87,0xb8,0x08,0x07,0x66,0x22,0x9f,0xc3,0xaa,
0x52,0x11,0x63,0x8e,0x96,0x44,0x02,0xdc,0xab,0x56,0xcf,0xad,0x30,0xba,0xb2,0xc5,
0x98,0xcf,0x89,0xee,0x7a,0x16,0xce,0xa8,0xa5,0x7e,0xb7,0x20,0x54,0x5c,0x5d,0x95,
0x07,0x7e,0xe4,0xe3,0x6f,0x96,0x0e,0x3b,0x2f,0xb1,0xaa,0x2f,0x43,0x51,0x69,0x3a,
0x35,0x03,0xb0,0xed,0x7b,0xd6,0xdb,0x95,0xa8,0x11,0x0a,0xbb,0x8b,0x0e,0xbd,0xe4,
0xfa,0xd3,0xd4,0x85,0xc0,0xb0,0xb9,0x30,0xda,0x17,0x46,0x37,0x59,0x8a,0x20,0x18,
0x12,0x03,0x7b,0x55,0x5a,0xe7,0x2b,0x41,0xf4,0x64,0x45,0x25,0x56,0x15,0x4e,0xbe,
0x41,0x25,0xc0,0x66,0x11,0x9b,0x07,0x42,0x13,0x06,0x55,0xe5,0xe8,0xb6,0x66,0x14,
0xe0,0x1d,0x0e,0x1c,0xca,0x4b,0x4f,0x55,0xf6,0x6e,0xcf,0x3e,0x6c,0x1c,0x0f,0xfc,
0xc9,0xf0,0x2f,0x71,0x8d,0x9d,0xc2,0x47,0xa5,0xc8,0x34,0x21,0x18,0x12,0x7f,0x1a,
0x4c,0x24,0xab,0xe0,0x00,0x30,0xf3,0x2c,0x70,0x4a,0x0e,0x95,0x6a,0x83,0x00,0x72,
0xd5,0x4a,0xcc,0x88,0x00,0xe0,0x5d,0x3f,0x70,0x04,0x8c,0x2d,0x40,0xc4,0x3f,0xff,
0x64,0x7c,0x57,0xa7,0xef,0xcb,0x72,0x9d,0xc6,0x5b,0xee,0xee,0xe8,0xc1,0xa7,0x0d,
0xdf,0x8a,0xfb,0x1d,0xc4,0x77,0x55,0x32,0xa2,0x1e,0xa8,0x44,0xac,0x0e,0x98,0xf6,
0xc5,0x0c,0xbe,0x35,0xd1,0xfe,0x66,0x2b,0x35,0x73,0x2b,0x81,0xff,0xf2,0xdb,0x9d,
0xf0,0x21,0x2b,0x83,0x4d,0x2e,0x53,0x28,0xdc,0xd4,0x1f,0xb8,0xdf,0xad,0x18,0xcf,
0x9f,0xc4,0x12,0x3c,0xcd,0x90,0x62,0x71,0x4a,0x59,0x51,0x03,0x1d,0x0a,0x22,0x2e,
0x24,0x63,0x7e,0xc9,0xd0,0x03,0x6f,0xd8,0x70,0x6e,0x8f,0x5a,0xee,0xee,0x99,0x3e,
0x64,0x40,0xef,0x7f,0x87,0xcb,0x93,0xb8,0xda,0x48,0xc5,0x62,0xf4,0x4b,0xc7,0x98,
0xb1,0xe0,0x69,0xcc,0xe2,0x46,0x91,0xf8,0x1e,0x5a,0x83,0xc8,0x81,0xa8,0x40,0x9e,
0xeb,0x28,0xc2,0xc8,0x83,0x3f,0x2b,0x21,0xb3,0x6d,0x6a,0x32,0x7d,0xab,0x67,0xb8,
0x32,0x13,0x02,0x9f,0xb7,0x4f,0x98,0xa2,0x3f,0xf4,0xc9,0xdf,0xee,0xcd,0xd0,0x93,
0xc9,0xa2,0x2f,0x1e,0xf3,0xc8,0x57,0x0f,0x28,0x3e,0xcc,0x17,0x5c,0x58,0xe9,0xa6,
0xc9,0x3f,0xf2,0x74,0x8a,0x2a,0x6f,0x5c,0x26,0xc8,0xa3,0x4b,0xec,0x01,0xa6,0xca,
0xcd,0xd5,0x94,0x00,0xc0,0x58,0xcc,0xff,0x15,0x81,0x26,0x33,0xf6,0xe2,0x93,0xd4,
0xa1,0x2d,0x99,0x66,0xc6,0xf1,0xb1,0x1f,0x04,0x88,0x8b,0x1f,0xdf,0xda,0xa0,0x2b,
0xea,0x0f,0xd2,0x78,0x0b,0x93,0xee,0xaa,0x03,0x80,0xa9,0xda,0xe8,0x4a,0xf9,0x85,
0x41,0x01,0xf5,0x77,0x76,0xbf,0x21,0xd8,0x6c,0x4a,0x70,0x40,0x06,0x18,0x42,0xc3,
0x2c,0x6e,0xc7,0x2a,0x03,0x62,0x4f,0x60,0x25,0x63,0x4a,0xdf,0xd6,0x86,0xbd,0x17,
0x57,0x30,0xb5,0x3b,0x06,0xee,0x91,0x3b,0xd9,0x5a,0x56,0x5a,0xf3,0x0b,0xcd,0x25,
0xd4,0x76,0x13,0xeb,0xdb,0xc0,0xbf,0x76,0x34,0xa7,0x85,0xf5,0x35,0x93,0x4f,0x2e,
0x18,0x4b,0x19,0x73,0x10,0xbb,0x79,0x12,0x16,0x4d,0x2c,0xb9,0x65,0xb3,0xbb,0xeb,
0x4c,0x28,0x72,0x24,0x6f,0xf5,0x32,0x95,0xba,0x32,0x01,0x43,0x31,0x52,0x9b,0x3a,
0xf8,0xe1,0x74,0x6b,0x0a,0x52,0xed,0x3e,0x37,0x62,0x2c,0x1d,0x7b,0x51,0xec,0xa7,
0x69,0x53,0xf2,0x6c,0x99,0x6a,0x17,0xed,0xef,0x96,0xe5,0xb6,0xc0,0x9a,0x20,0x58,
0x42,0x06,0x84,0x03,0x2f,0x41,0x50,0x9b,0x74,0xb9,0x59,0x88,0x99,0xf0,0xa3,0xbd,
0xc3,0xf8,0x5d,0xe2,0x2c,0xfb,0x63,0xe9,0xb8,0x58,0xbf,0x5e,0x5a,0x55,0x55,0xd3,
0x33,0x89,0x04,0x7f,0x20,0x7d,0x28,0x38,0x8b,0xb8,0x48,0x88,0xac,0xf4,0x2a,0xe8,
0x7c,0x4d,0xe0,0x65,0xcf,0x21,0x66,0x09,0xc6,0xad,0x3d,0xf6,0xac,0xef,0x5b,0xa2,
0x06,0xbd,0x51,0x68,0xf1,0xa1,0x88,0x21,0xc1,0x7f,0xcc,0xc3,0xc5,0x87,0x5a,0x5b,
0x91,0x21,0x6d,0x3f,0xc1,0xfa,0x87,0x73,0x27,0xf0,0x9f,0xf4,0x9c,0x9e,0xde,0x84,
0x5e,0x88,0xb9,0x7c,0xf7,0x2d,0x37,0x62,0x5e,0xec,0x7b,0x4b,0x04,0xc5,0x35,0xed,
0xc7,0xea,0x39,0x35,0xa9,0x61,0x0b,0x69,0x3b,0x56,0x62,0x24,0xd0,0xee,0x44,0x1e,
0x7a,0x7a,0x69,0x08,0x74,0xdc,0x4d,0x6d,0x9f,0x91,0xd5,0xed,0x17,0x7c,0xc3,0x73,
0x58,0x95,0x11,0x04,0x98,0x43,0xaa,0x73,0x2e,0xdb,0xf6,0x6b,0x98,0xb7,0x3e,0x85,
0xbe,0x54,0x71,0x43,0xa5,0x1f,0xa6,0xa6,0x53,0xd9,0x9a,0xa4,0x1d,0x4c,0x90,0x77,
0x71,0xdd,0x74,0xc3,0xd8,0xb3,0x7d,0xae,0xf4,0xe8,0x1d,0x99,0x3d,0x28,0x1d,0xd8,
0x0e,0x53,0x5d,0xdc,0xf4,0x79,0x41,0x5e,0xa7,0xff,0xb9,0x3a,0x48,0x9b,0x97,0xee,
0xbb,0xf0,0x54,0x7d,0x99,0x72,0x76,0xb8,0x37,0x46,0x77,0xcc,0x6a,0x84,0xff,0xbf,
0xd0,0xfb,0x13,0xf0,0xe2,0x52,0xd8,0xbf,0x41,0xa8,0x6c,0x78,0x6b,0x3d,0x2d,0xde,
0x5e,0x01,0x11,0x5c,0xc4,0x8b,0x63,0x75,0xf1,0xfb,0x79,0xc6,0xd5,0x80,0xa6,0x3d,
0xf3,0x34,0x50,0x87,0xdf,0x9e,0x2d,0x14,0xb8,0x37,0x58,0x6d,0x4d,0x9c,0x29,0xa4,
0x1b,0x2e,0xb9,0x2b,0x29,0xf6,0x88,0xc1,0xbc,0x73,0x18,0x61,0xc0,0xb9,0x8d,0x9c,
0x2c,0xe2,0xcc,0x00,0x00,0x00,0x01,0x41,0x9a,0x00,0x02,0x05,0x91,0x9f,0x89,0x76,
0xb5,0xc2,0x20,0x9c,0x14,0xd8,0xbc,0xb1,0x1b,0x92,0xf7,0xa0,0x5b,0x2b,0x2f,0xa5,
0x71,0xba,0x9a,0x4b,0x60,0x8a,0x8d,0xc0,0xa3,0xda,0xa5,0xf8,0x10,0x52,0x21,0xa0,
0xb4,0x60,0xbc,0xb4,0x0b,0x57,0x5b,0x0f,0x95,0xcf,0x48,0x30,0x1f,0x1c,0xe5,0x3b,
0x57,0xa3,0x46,0xd2,0x31,0x3f,0xf9,0x2e,0xc9,0x3f,0xe7,0x39,0x4a,0xc2,0xc3,0x1c,
0xf4,0x81,0xe8,0x5d,0x64,0x26,0x4f,0x83,0x10,0xec,0xb5,0xa0,0xfd,0x7c,0x43,0x9e,
0x3d,0xfc,0x6f,0xcf,0x29,0xaf,0x94,0xbe,0xdf,0x0f,0xe0,0x7b,0x4b,0xf4,0x60,0xe3,
0x63,0xb5,0x38,0x0d,0x32,0xc7,0x11,0x12,0xba,0x55,0x0f,0x38,0x77,0x95,0x09,0xbc,
0x5a,0xa6,0x79,0x00,0xb8,0x0b,0xd4,0xd3,0x3f,0x08,0x5d,0xe4,0xc7,0x7a,0xd3,0xae,
0x1c,0x4e,0xe5,0x28,0xde,0x8b,0xf7,0x7f,0xde,0xc5,0xe5,0x2e,0x73,0xc7,0x51,0xf9,
0x3c,0x9d,0x1c,0x15,0x2a,0x14,0x48,0x90,0xee,0x56,0x42,0x8c,0x77,0x26,0x67,0x4e,
0xc1,0xb1,0x04,0xb5,0x2d,0x4c,0xf0,0x12,0x36,0x02,0x03,0x73,0xf3,0xb8,0x2a,0xcf,
0x3b,0x10,0x7d,0x0b,0xcc,0xf9,0x06,0x3b,0x6e,0xd0,0xe7,0xbf,0x0b,0x68,0x68,0x16,
0x14,0xb3,0x9d,0xfa,0x16,0xb5,0x0a,0x8a,0xf2,0xba,0x1f,0x13,0xbd,0x31,0x4d,0x69,
0x31,0x70,0xb8,0x5a,0xfa,0x4f,0x45,0x6f,0x00,0x00,0x00,0x01,0x41,0x9a,0x00,0x04,
0x09,0x44,0x67,0xeb,0x5f,0x34,0x76,0x3a,0x10,0x33,0x32,0x22,0xc8,0x05,0xc6,0x9a,
0xde,0xe6,0x31,0xa8,0x64,0xe8,0xab,0x8a,0xf0,0x19,0x80,0xc2,0x9c,0xda,0xab,0x15,
0x8c,0x78,0x8b,0x3d,0x29,0x54,0x36,0xd3,0x70,0xa1,0x3b,0x02,0xd6,0x94,0x3e,0x6c,
0x25,0x1b,0x1a,0xa4,0x5b,0xcf,0x22,0xd2,0x17,0x54,0x61,0x06,0x1e,0xf9,0x7e,0x3c,
0x7f,0x56,0x0c,0x41,0xfd,0x9d,0x1f,0x73,0x09,0x29,0xae,0xae,0xc5,0x5a,0xc8,0xae,
0x2a,0x66,0x16,0x1d,0xa5,0xdc,0x5f,0xe9,0x5f,0x80,0x46,0xfd,0x1d,0xb9,0xad,0xd2,
0x50,0x32,0xbb,0xe9,0x6e,0xef,0xfe,0xfb,0xc7,0xd6,0xf5,0xb5,0x9b,0x90,0xcc,0xcf,
0x52,0xba,0x70,0x87,0xf3,0x06,0x67,0xbd,0x5f,0x3b,0x00,0xbe,0x25,0x81,0xcd,0xec,
0x74,0xe9,0xd1,0xd9,0x62,0x4f,0x00,0x34,0x01,0x55,0xa2,0x60,0xcf,0xfe,0x19,0x45,
0x82,0x9f,0x42,0x01,0xdc,0xa5,0xef,0x86,0x88,0xcb,0x5e,0x5c,0x19,0xaa,0x45,0x3e,
0xc5,0x2e,0xa3,0xbe,0xe7,0x2c,0x8f,0x9c,0x6b,0x94,0x2f,0xfb,0x15,0x46,0x29,0x64,
0xc9,0x01,0x97,0x87,0x5e,0x33,0x89,0x51,0xfe,0x1d,0x18,0x9e,0x63,0x38,0x75,0x1c,
0xcc,0x66,0x7e,0x75,0x41,0x25,0x9e,0xc5,0x32,0x27,0x69,0xe2,0x4a,0xf4,0x44,0x5b,
0x6b,0xff,0xff,0x58,0x99,0xac,0xda,0xe5,0x90,0x00,0xcf,0xe5,0xe2,0xfa,0x12,0x6f,
0xf9,0xfe,0xed,0xfc,0x36,0x91,0xe7,0x8b,0x58,0x7d,0xb6,0xfa,0xa1,0x77,0x1a,0x1c,
0xe6,0x91,0x9d,0x64,0xbd,0x12,0xd4,0x99,0x2f,0xa4,0x8c,0x61,0x6b,0xb5,0x45,0xbc,
0x50,0xa8,0x72,0xdd,0xfe,0x70,0x0f,0x33,0xa8,0x20,0x72,0x0d,0x92,0x30,0xf8,0xeb,
0x2f,0xc0,0xaf,0xd6,0x12,0x56,0xe4,0x35,0x9b,0xbd,0xc3,0xd8,0x00,0x00,0x00,0x01,
0x41,0x9a,0x00,0x06,0x0d,0x64,0x67,0x8b,0x6c,0x77,0x9d,0xd1,0x6e,0x2e,0x28,0x95,
0x65,0x60,0xdb,0xe9,0x66,0x77,0xec,0x4f,0xed,0x30,0xc7,0x13,0x80,0x17,0x5b,0x64,
0xfa,0x6b,0xe7,0x8f,0x57,0xe3,0x9e,0x9c,0xee,0x18,0x7a,0x86,0xbd,0xbc,0x63,0x00,
0x34,0xa8,0x64,0x03,0x53,0xe6,0x6c,0x70,0x0f,0x1c,0x46,0xbf,0x75,0x51,0x65,0x5d,
0x39,0xf5,0x5e,0xe4,0x1e,0xbf,0xb4,0x68,0xe1,0xc4,0xdc,0x02,0x98,0x6c,0xc2,0xde,
0xbf,0x54,0xf3,0xe9,0x1c,0xf0,0x5e,0xec,0x1e,0xa3,0x6b,0x26,0xae,0xa7,0x47,0x3f,
0x40,0x84,0xa4,0x8e,0x9e,0xd0,0x9f,0x58,0xa7,0xa3,0xd9,0x3d,0xd0,0xc7,0x4f,0x42,
0xdf,0x9e,0xea,0x50,0x14,0xd1,0x57,0xdd,0xb6,0x2f,0x24,0xbe,0xea,0x2d,0x47,0x70,
0x6d,0x49,0xe4,0xc6,0xc4,0xf2,0x02,0x64,0xfe,0xf7,0x4e,0xcc,0x6b,0x42,0x57,0xc9,
0xf2,0x9a,0x35,0x0f,0x6b,0xe4,0x65,0x8f,0x71,0x60,0xd0,0x46,0x5b,0x50,0x18,0x0c,
0x92,0xe8,0xa5,0xb3,0xe1,0x5c,0xe2,0xef,0xa0,0x74,0x51,0x1c,0x17,0xc4,0x9c,0x81,
0xc0,0xba,0x1c,0xf5,0xb6,0x5e,0x77,0xce,0x66,0xd8,0x1b,0xe3,0x85,0x5d,0xd7,0xb0,
0xef,0x8b,0xd2,0x96,0x4b,0xb3,0x90,0xf6,0xce,0x02,0x48,0x35,0x68,0x7c,0x5b,0x67,
0x2f,0x9a,0x21,0x05,0x96,0xcd,0xfb,0xad,0xbb,0x0c,0x17,0xf7,0x25,0xdc,0x00,0x54,
0xc7,0xc2,0x1a,0xd3,0x1f,0xa5,0xf9,0x12,0x43,0xe7,0xdc,0x4b,0xbc,0x46,0xe8,0x16,
0x91,0xa6,0xaf,0x6c,0xfc,0x26,0xdb,0xd0,0xec,0xc5,0x47,0x26,0x5d,0xe4,0xdd,0x30,
0xbd,0xd7,0xbe,0x63,0x6f,0x4b,0x90,0x38,0x97,0xac,0x5a,0x0b,0xb8,0x01,0x69,0x82,
0xed,0xad,0xc4,0xfe,0x4f,0x9a,0x63,0x06,0x0c,0x3c,0xde,0xec,0x11,0x4b,0x86,0x17,
0x57,0x7d,0x42,0xfd,0xd1,0x05,0x58,0xa9,0xe4,0x00,0xe0,0x5a,0x33,0x6d,0x04,0x39,
0xbe,0x20,0x5b,0xa5,0x75,0xc0,0x3d,0x81,0x65,0x41,0xe7,0x64,0x06,0xc4,0xbe,0xb8,
 };
