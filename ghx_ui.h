
// ghx.h
/*
	Author:	tkz
	Date:	2020/12/05
*/
#ifndef _GHX_H_
#define _GHX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lcd.h"
#include "color.h"
#include "words.h"

typedef unsigned char uchar;
typedef unsigned int uint;


// 画字 参数：列x 行y 字 宽w 大小size 颜色color
void lcdDrawWord(int x0,int y0,const unsigned char *word, int len,int size,int color);
// 画字 参数：列x 行y 字 宽w 高h 颜色color
void lcdDrawFont(int x,int y,const unsigned char *word,int w,int h,Color_type color);
// 写ASCII单个字符
void lcdDrawChar(int x,int y,char word,Font_type font,int color,int mode);
// 写ASCII字符串(可调字体大小)
void lcdDrawString(int x,int y,char * const str,int font,int color,int mode);

// m^n函数 返回值:m^n次方.
int Pow(int m,int n);
// 显示数字num:数值(0~4294967295) 高位0不显示 起点x,y 位数len 字体size
void lcdShowNum(int x,int y,unsigned int num,int len,int size,int color);	
		
// 画线 起点坐标x1,y1 终点坐标x2,y2
void lcdDrawLine(int x1,int y1,int x2,int y2,int color);
// 在指定位置画一个指定大小的圆
void lcdDrawCircle(int x,int y,int r,int color);
// 画矩形(x1,y1),(x2,y2):矩形的对角坐标
void lcdDrawRectangle(int x1, int y1, int x2, int y2,int color);
// 画三角:三点坐标
void lcdDrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int color);

// 调色板
void colorPalette(int x,int y,int w,int h);
// 彩虹效果1
void lcdDrawRainbow1(void);
// 网格特效
void gridEffect(int s,int color);
// 回环框特效
void loopFrameEffect(int color);
// back
void backGround(void);
// 
int yuv_to_rgb(int y, int u, int v);
// show camera
void process_yuv_image(unsigned char *yuv, int width, int height);
// get time
void getTime(char *cur_time);

#endif
