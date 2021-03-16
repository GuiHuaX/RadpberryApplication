/*
	Author:	ghx-tkz
	Date:	2020.06.14
*/

#ifndef _WORDS_H_
#define _WORDS_H_

typedef enum
{
	Font_12 = 12, // 12*12
	Font_16 = 16, // 16*16
	Font_24 = 24, // 24*24
	Font_32 = 32, // 32*32
	Font_48 = 48, // 48*48
	Font_56 = 56  // 56*56
}Font_type;

// ASCII字符点阵数组(宋体) 
const unsigned char Font_12_ASCII[95][8*12/8];
const unsigned char Font_16_ASCII[95][8*16/8];
const unsigned char Font_24_ASCII[95][16*24/8];
const unsigned char Font_32_ASCII[95][16*32/8];
const unsigned char Font_48_ASCII[95][24*48/8];

// ASCII字符点阵数组(楷体) 

//unsigned char Font_18_MyWord[20902][32*32/8];

// 汉字字符点阵数组(宋体)
unsigned char Font_24_MyName[3][24*24/8];
// 汉字字符点阵数组(楷体)
unsigned char Font_56_MyName[3][56*56/8];

#endif

