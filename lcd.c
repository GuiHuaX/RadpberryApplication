
//lcd.c
/*
	Author:	tkz
	Date:	2020/12/05
*/
#include "lcd.h"


// 输出屏幕信息
void printLcdInfo(void)
{
	// 打印屏幕信息
	printf("LCD屏幕信息如下：\n ---lcd_info---\n");
	printf(" lenth = %d,\t high = %d,\t color depth = %d\n",
			lcd_info.xres,lcd_info.yres,lcd_info.bits_per_pixel);
	printf(" red.offset=%d,\t red.length=%d,\t red.msb_right = %d\n",
			lcd_info.red.offset,lcd_info.red.length,lcd_info.red.msb_right);
	printf(" green.offset=%d, green.length=%d, green.msb_right = %d\n",
			lcd_info.green.offset,lcd_info.green.length,lcd_info.green.msb_right);
	printf(" blue.offset=%d,\t blue.length=%d,\t blue.msb_right = %d\n",
			lcd_info.blue.offset,lcd_info.blue.length,lcd_info.blue.msb_right);
	printf(" ---End---\n\n");
	return ;
}

/*
	功能：屏幕初始化 即打开和映射屏幕 
	参数：无
	返回值：成功返回0 失败返回 -1  
*/
int lcdInit(void)
{
	// 打开屏幕 
	//lcd_fd = open("/dev/ubuntu_lcd",O_RDWR);
	lcd_fd = open(lcdPATH,O_RDWR);// 可读可写
	if(lcd_fd < 0)
	{
		perror("open lcd error\n");
		return -1;
	}
	
	// 获取屏幕属性 
	int ret = ioctl(lcd_fd,FBIOGET_VSCREENINFO,&lcd_info);
	if(ret < 0)
	{
		printf("get lcd info error\n");
		close(lcd_fd);
		return -1;
	}
	
	// 输出屏幕信息
	printLcdInfo();
	
	// 内存映射屏幕 
	plcd = (int*)mmap(NULL,lcd_info.xres*lcd_info.yres*lcd_info.bits_per_pixel/8,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(plcd == NULL)
	{
		perror("mmap lcd error\n");
		close(lcd_fd);
		return -1;
	}
	lcdClear(0x00FFFFFF); // 初始化屏幕为白色
	return 0;
}	
	
/*	
	功能：lcd 屏幕的解除初始化  即解除映射,以及关闭屏幕 
	参数：无
	返回值：成功返回0 失败返回 -1
*/	
int lcdUninit(void)
{
	//解除映射 参数：映射地址，大小
	int ret = munmap(plcd,lcd_info.xres*lcd_info.yres*lcd_info.bits_per_pixel/8);
	if(ret < 0)
	{
		perror("munmap lcd error\n");
		return -1;
	}
	ret = close(lcd_fd);
	if(ret < 0)
	{
		perror("close lcd error");
		return -1;
	}
	return 0;
}
	
/*
	函数功能：在屏幕上实现显示一个像素点
	函数参数：x表示  y表示坐标 color
	函数返回值：
*/
int lcdDrawPoint(int x,int y,int color)
{
	if(x >= lcd_info.xres || y >= lcd_info.yres) // 0~799 0~599
	{
		//printf("越界，超出屏幕范围\n坐标(x=%d,y=%d)\n",x,y);
		return -1;
	}
	*(plcd+y*lcd_info.xres+x) = color; // 画点
	return 0;
}

/*
	画块起始坐标,x0水平坐标,y0表示垂直坐标 宽width  高high 颜色color
*/
int lcdDrawBlock(int x0,int y0,int w,int h,int color)
{
	if(x0+w >= lcd_info.xres || y0+h >= lcd_info.yres)
	{
		printf("越界，超出屏幕范围\n坐标(x0+w=%d,y0+h=%d)\n",x0+w,y0+h);
		return -1;
	}
	// 首先确定起点坐标
	int i,j;
	for(j = y0;j <= y0+h ;j++)  // 0~599
	{
		for(i = x0;i <= x0+w;i++) // 0~799
		{
			lcdDrawPoint(i,j,color);
		}
	}
	//printf("块 起始坐标(x,y)=(%d,%d) 宽%d 高%d\n",x0,y0,w,h);
	return 0;
}
// 清屏
void lcdClear(int color)
{
	int i;
	int *p = plcd;
	for(i = 0; i < lcd_info.xres*lcd_info.yres; i++)
		p[i] =  color;
	return ;
}

// 画横线 起始坐标  长度
void lcdDrawLineX(int x,int y,int L,int s,int color){
	int i;	
	int cnt = 0;
	do
	{
		for(i = x;i <= x+L;i++) // 0~799
			lcdDrawPoint(i,y+cnt,color);
		cnt++;
	}while(cnt < s); // cnt用于给线加粗
	return ;
}
// 画竖线 起始坐标  长度
void lcdDrawLineY(int x,int y,int L,int s,int color){
	int i;
	int cnt = 0;
	do
	{
		for(i = y;i <= y+L;i++) // 0~599
			lcdDrawPoint(x+cnt,i,color);
		cnt++;
	}while(cnt < s); // cnt用于给线加粗
	return ;
}

// 画框 起始坐标x,y  宽w 高h 粗细s
void lcdDrawBox(int x,int y,int w,int h,int s,int color){
	int i,j;
	int cnt = 0;
	do
	{
		for(i = x;i <= x+w;i++) // 横
		{
			lcdDrawPoint(i,y+cnt,color);	 // 上
			lcdDrawPoint(i,y+h-cnt,color); // 下 
		}
		for(j = y;j <= y+h;j++) // 竖
		{
			lcdDrawPoint(x+cnt,j,color); 	 // 左
			lcdDrawPoint(x+w-cnt,j,color); // 右
		}
		cnt++;
	}while(cnt < s); // cnt用于给线加粗
	//printf("框 起始坐标(x,y)=(%d,%d) 宽%d 高%d\n",x,y,w,h);
	return ;
}

//实心圆 参数：圆心 坐标，半径，颜色
void lcdDrawFullCircle(int x0,int y0,int r,int color)
{
	int i = 0,j = 0;
	int d = 0;
	for(i = x0-r; i < x0+r; i++)
	{
		for(j = y0-r; j < y0+r; j++)
		{
			d = (i-x0)*(i-x0)+(j-y0)*(j-y0);
			if(d <= r*r)
			{
				lcdDrawPoint(i, j, color);
			}
		}
	}
	//printf("实心圆 圆心(x,y)=(%d,%d) 半径%d\n",x0,y0,r);
	return ;
}

//空心圆
void lcdDrawEmptyCircle(int x0,int y0,int r,int s,int color)
{
	int i = 0,j = 0;
	int d = 0;
	for(i = x0-r; i < x0+r; i++)
	{
		for(j = y0-r; j < y0+r; j++)
		{
			d = (i-x0)*(i-x0)+(j-y0)*(j-y0);
			if(d <= r*r && d >= (r-s)*(r-s))
			{
				lcdDrawPoint(i, j, color);
			}
		}
	}
	//printf("空心圆 圆心(x,y)=(%d,%d) 半径%d 粗细%d\n",x0,y0,r,s);
	return ;
}

/*	
int draw_bmp(char* pathname,int width,int high)
{
	//第一步：打开图片文件 
	int pic_fd = open(pathname,O_RDWR);
	if(pic_fd < 0)
	{
		perror("open pic error");
		return -1;
	}

	unsigned char buf[800*480*3] = {0};//注意 为什么需要用无符号表示
	//显示图片的时候 将54个字节的文件头偏移走！
	//前54个字节为文件头 表示文件的格式 长 宽 大小 以及色深等信息 需要跳过 
	int ret = lseek(pic_fd,54,SEEK_SET);	
	if(ret < 0)
		{
			perror("lseek pic error");
			return -1;
		}
	ret = read(pic_fd,buf,800*480*3);
	if(ret < 0)
	{
		perror("read pic error");
		return -1;
	}

	//第三步：写入数据并且显示

	int i,j;
	unsigned int a,r,g,b,color;
	unsigned char *p = buf;

	for(i=(480-high)/2;i<480;i++)
	{
		for(j=(800-width)/2;j<800;j++)
		{
			a = 0x00;
			b = *p++;  // 0 3 6 9 ...
			g = *p++;  // 1 4 7 10 ...
			r = *p++;  // 2 5 8 11 ....

			color = a<<24|r<<16|g<<8|b;
			lcd_draw_point(479-i,j,color);
		}
	}
}
*/

void lcdDrawBmp(int x, int y, bitBmp *bmp)
{
	int *p = plcd+y*lcd_info.xres+x; // 获取起点坐标
	int *pic = bmp->startBmp; // bmp 图片地址
	for(int i = 0; i < bmp->bh; i++)
	{
		memcpy(p, pic, bmp->bw*bmp->pertype);
		p += lcd_info.xres;
		pic += bmp->bw;
	}
	printf("bmp 起始坐标(x,y)=(%d,%d)\n",x,y);
}
