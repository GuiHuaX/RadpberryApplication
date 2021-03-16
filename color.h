
//color.h
/*
	Author:	tkz
	Date:	2020/12/05
*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdio.h>
#include <string.h>

typedef enum
{
	// 彩虹色
	Red		=	0x00FF0000,	// 红色(三原色)
 	Orange	=	0x00FF7F00,	// 橙色
 	Yellow	=	0x00FFFF00,	// 黄色
 	Green	=	0x0000FF00,	// 绿色(三原色)
 	Cyan	=	0x0000FFFF,	// 青色
	Blue	=	0x000000FF,	// 蓝色(三原色)
	Purple	=	0x00800080,	// 紫色
	// 黑白
	White	=	0x00FFFFFF, // 白色
	Black	=	0x00000000  // 黑色
}Color_type;

int colorArry[9];
char *colorArryChar[9];

// 三原色结构体声明
struct PrimaryColors
{
	// 三原色
	int R;	// 纯红
	int G;	// 纯绿
	int B;	// 纯蓝
	
}Color;

// 字体背景色
#define Background_color 0x00FFFFFF

// 三原色
#define	Green			0x0000FF00	// 纯绿
#define	Blue			0x000000FF	// 纯蓝
#define	Red				0x00FF0000	// 纯红
// 黑白
#define	White			0x00FFFFFF	// 纯白	
#define	Black			0x00000000	// 纯黑
// 彩虹色
#define	Red				0x00FF0000	// 红色
#define	Orange			0x00FF7F00	// 橙色	
#define	Yellow			0x00FFFF00	// 黄色
#define	Green			0x0000FF00	// 绿色
#define	Cyan			0x0000FFFF	// 青色
#define	Blue			0x000000FF	// 蓝色
#define Purple			0x00800080	// 紫色
// 其它
#define	Pink			0x00FFC0CB	// 粉红
#define	Indigo			0x004B0082	// 靛青
#define	Khaki			0x00F0E68C	// 卡其布
#define	DarkKhaki		0x00BDB76B 	// 深卡其布	
#define	Gold			0x00FFD700 	// 金
#define	Brown			0x00A52A2A	// 棕色
#define	Gray			0x00808080	// 灰色
#define	LightGrey		0x00D3D3D3	// 浅灰色
#define	DarkGray		0x00A9A9A9	// 深灰色
#define	DarkBlue		0x0000008B	// 深蓝色		
#define	LightSkyBlue	0x0087CEFA	// 淡蓝色		
#define	SkyBlue			0x0087CEEB	// 天蓝色
#define	LightBLue		0x00ADD8E6	// 淡蓝	
#define	Azure			0x00F0FFFF	// 蔚蓝色
#define	LightCyan		0x00E1FFFF	// 淡青色
#define	DarkCyan		0x00008B8B	// 深青色
#define	LightGreen		0x0090EE90	// 淡绿色
#define	ForestGreen		0x00228B22	// 森林绿
#define	DarkGreen		0x00006400	// 深绿色
#define	LawnGreen		0x007CFC00	// 草坪绿
#define	Beige			0x00F5F5DC	// 米色(浅褐色) 
#define	LightYellow		0x00FFFFE0	// 浅黄色	
#define	Wheat			0x00F5DEB3	// 小麦色	
#define	DarkOrange		0x00FF8C00	// 深橙色	
#define	Chocolate		0x00D2691E	// 巧克力	
#define	OrangeRed		0x00FF4500	// 橙红色	
#define	DarkRed			0x008B0000	// 深红色	
#define	Maroon			0x00800000	// 栗色	
#define	Silver			0x00C0C0C0	// 银白色	


#endif

