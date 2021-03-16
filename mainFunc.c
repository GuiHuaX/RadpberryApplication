
// main.c
/*
	Author:	tkz
	Date:	2020/12/05
*/
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ghx_ui.h"	// 自定义函数接口
#include "lcd.h"	// lcd屏函数接口
#include "touch.h"	// 触摸屏函数接口
#include "color.h"	// 颜色定义接口
#include "bmp.h"	// bmp图片函数接口
#include "words.h"  // 字体接口
//#include "jpeg.h" // jpeg图片接口
#include "v4l2.h"	// 摄像头接口

int main()
{
	// 初始化
	lcdInit();		// 初始化屏幕 
	cameraInit();
	
	
	//lcdClear(Khaki); // 卡其布色背景
	lcdDrawRainbow1(); // 渐变彩虹背景
	
	char i,j;
	int Font_color = ~White;
	char *cur_time = (char *)malloc(21*sizeof(char));
	
	
	// 系统基本信息
	printf("#各种类型数据所占内存空间大小：\n");
	printf(" char:%d unsigned char:%d\n",sizeof(char),sizeof(unsigned char));
	printf(" int:%d unsigned int:%d\n",sizeof(int),sizeof(unsigned int));
	printf(" char*:%d int*:%d\n",sizeof(char*),sizeof(int*));
	printf(" float:%d double:%d\n\n",sizeof(float),sizeof(double));
	
	printf("#系统初始化... 完成\n");
	
	
	backGround(); // 背景
	
	// show words
	do{
		lcdDrawWord(520+i*56,400,Font_56_MyName[i],56,56*56/8,Pink);
		i++;
	}while(i < 3);
	lcdDrawString(410,20,"GuiHuaXin!",48,Red,0);
	
	/*camera
	startCapture();
	dataProce();
	stopCapture();
	*/
	char *p = cur_time+16;
	do{
		// 时间
		memset(cur_time,0,sizeof(cur_time)); // malloc分配的内存用完需要清零，不然这里会内存溢出
		getTime(cur_time);
		//printf("Current time: %s\n", cur_time);
		lcdDrawString(50,100,cur_time,48,DarkGray,1);
		sleep(1);
	}while(*p != '0');
	
	free(cur_time);
	cur_time = NULL;
	
	/*
	int flag = 0;
	int x,y;
	while(1)
	{
		get_ts(&touch);
		x = touch.x;
		y = touch.y;
		if(x >550 && x<678 && y>460 && y<588)
		{
			if(flag == 0)
			{
				bmp = create_bitBmp_file("./led-2.bmp");
				lcdDrawBmp(550,460, bmp);
				destroy_bitBmp(bmp);
				flag = 1;
			}else
			{
				bmp = create_bitBmp_file("./led.bmp");
				lcdDrawBmp(550,460, bmp);
				destroy_bitBmp(bmp);
				flag = 0;
			}
		}
	}*/
	
	// 解除初始化 
	cameraUninit();
	lcdUninit();
	return 0;
}



