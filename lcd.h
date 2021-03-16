
//lcd.h
/*
	Author:	tkz
	Date:	2020/12/05
*/
#ifndef _LCD_H_
#define _LCD_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>  // 内存映射
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>


#include "bmp.h"

static int lcd_fd = -1; 			// 屏幕文件描述符
static int * plcd = NULL; 			// 屏幕内存映射指针地址

struct fb_var_screeninfo lcd_info; 	// 屏幕信息结构体

static char *lcdPATH = "/dev/fb0"; 	// 屏幕设备路经


// 初始化LCD
int lcdInit(void);
// 解初始化LCD
int lcdUninit(void);
// 输出屏幕信息
void printLcdInfo(void);


// 画点 参数：列x 行y 颜色color
int lcdDrawPoint(int x,int y,int color);
// 画横线 参数：列x 行y 颜色color
void lcdDrawLineX(int x,int y,int L,int s,int color);
// 画竖线 参数：列x 行y 颜色color
void lcdDrawLineY(int x,int y,int L,int s,int color);

/* 画框 参数：列x 行y 宽w 高h 粗细s 颜色color */
void lcdDrawBox(int x,int y,int w,int h,int s,int color);
/* 画块 参数：列x 行y 宽w 高h 颜色color */
int lcdDrawBlock(int x0,int y0,int w,int h,int color);

// 清屏 参数：颜色color
void lcdClear(int color);

// 画空心圆 参数：圆心坐标x0、y0，半径r，粗细s，颜色color
void lcdDrawEmptyCircle(int x0,int y0,int r,int s,int color);
// 画实心圆 参数：圆心坐标x0、y0，半径r，颜色color
void lcdDrawFullCircle(int x0,int y0,int r,int color);


// 画 bmp 图片 起始坐标x、y
void lcdDrawBmp(int x, int y, bitBmp *bmp);


#endif
