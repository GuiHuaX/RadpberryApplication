
//ghx.c 
/*
	Author:	tkz
	Date:	2020/12/05
*/
#include "ghx_ui.h"


// back
void backGround(void)
{
	// 左上角的框
	lcdDrawBlock(15,15,375,275,Gray); 	// 背景
	lcdDrawBox(10,10,385,285,2,Azure);	// 小框
	// 右边的调色板
	colorPalette(720,50,60,40); // 调色板
	// show effect
	gridEffect(50,Pink); // 网格特效
	// show background
	lcdDrawBox(5,5,685,590,2,Blue); 	// 大框
	lcdDrawLineX(5,300,685,2,Blue);		// 中横线
	lcdDrawLineY(400,5,590,2,Blue);		// 中竖线
	lcdDrawFullCircle(400,300,6,Blue);	// 中心点
	
	lcdDrawLine(250,250,550,350,Blue);
	lcdDrawCircle(400,300,100,Red);
	lcdDrawTriangle(350,350,250,500,400,500,DarkBlue);
	
	// show bmp pic
	bitBmp *bmp = create_bitBmp_file("./led.bmp");
	lcdDrawBmp(550,460, bmp);
	destroy_bitBmp(bmp);
	
	lcdDrawString(20,450,"GUIhuasndsan125",12,DarkGray,0);
	lcdDrawString(20,465,"GUIhuasndsan125",16,DarkGray,0);
	lcdDrawString(20,480,"GUIhuasndsan125",24,DarkGray,0);
	lcdDrawString(20,505,"GUIhuasndsan125",32,DarkGray,0);
	lcdDrawString(20,540,"GUIhuasndsan125",48,DarkGray,0);
}

// 显示不同体的单个字符或单个汉字（像素点，易于理解）
void lcdDrawFont(int x,int y,const unsigned char *word,int w,int h,Color_type color)
{
	int i = 0,j = 0;
	int mode = 0;
	for (j = 0; j < h; j++) 
	{
		for (i = 0; i < w; i++) 
		{
			if (*word & (0x80 >> (i % 8))) 
			{
				lcdDrawPoint(x + i, y + j, color);
			}else if(mode == 1){
				lcdDrawPoint(x + i, y + j, Background_color);
			}
			if (i % 8 == 7) { // 下一个字节
				word++;
			}
		}
		
		if (w % 8 != 0) { // 这里有点迷 取模的宽应该都是8的倍数，这一步个人觉得可以取消
			word++;
		}
		//printf("w:%d w%8:%d\n",w,w%8);
	}
}
// 显示一个文字 文字的数组名word，起始地址x0、y0 ，宽度len一行是多少像素点,字体大小size，颜色color
void lcdDrawWord(int x0,int y0,const unsigned char *word,int len,int size,int color)
{
	// 算出每一行是几个字节
	int flag = len/8; // 一个字的方块 宽度为len个像素点 一行有len/8个字节
	int i = 0,j = 0;
	int mode = 0;
	for(i=0;i<size;i++) //遍历每一个字节 即显示数组中的每一个字节 一共是8位 只能从最低位开始遍历
	{
		for(j=0;j<8;j++) //遍历每一个字节的每一位 
		{
			if(word[i]>>j & 0x01)  //为真表示这一位为1 			
			{
				// i%flag表示：一行的第i%flag个字节 , i/flag表示：第i/flag行
				lcdDrawPoint(x0+(i%flag)*8+(7-j),y0+i/flag,color);
			}
			else if(mode == 1)
			{
				//第i位为0 则表示这个点不显示
				lcdDrawPoint(x0+(i%flag)*8+(7-j),y0+i/flag,Background_color);
			}
		}
	}
}	

// lcd显示单个字符（简单，易于理解）
void lcdDrawChar(int x,int y,char word,Font_type font,int color,int mode)
{
	int i,j;
	int size;			// 字符所占字节数
	int len = font/2; 	// 字符的宽度（像素点）
	while(len%8 != 0) 	// 宽度不为8的倍数，则使其成为8的倍数
	{	
		len = len + 1; 
	};
	size = len*font/8;
	//printf("字宽:%d 字高:%d\n",len,font);
	
	const unsigned char *ptr;	// 字体数组地址
	switch(font)
	{
		case 12:
			ptr = Font_12_ASCII[word-32]; // 调用12*12字体
			break;
		case 16:
			ptr = Font_16_ASCII[word-32]; // 调用16*16字体
			break;
		case 24:
			ptr = Font_24_ASCII[word-32]; // 调用24*24字体
			break;
		case 32:
			ptr = Font_32_ASCII[word-32]; // 调用32*32字体
			break;
		case 48:
			ptr = Font_48_ASCII[word-32]; // 调用48*48字体
			break;
		default :
			ptr = Font_16_ASCII[word-32]; // 默认调用16*16字体
			break;
	};
	
	for (j = 0; j < font; j++) 
	{
		for (i = 0; i < len; i++) 
		{
			if (*ptr & (0x80 >> (i % 8))) 
			{
				lcdDrawPoint(x + i, y + j, color); // 需要显示的像素点
			}else if(mode == 1) { // 是否叠加字体背景,叠加背景是为了以后动态显示字符时减少清屏这部分的工作
				lcdDrawPoint(x + i, y + j, Background_color);// 不需要显示的像素点
			}
			if (i % 8 == 7) { // 下一个字节
				ptr++;
			}
		}
		
		if (len % 8 != 0) { // 这里有点迷 取模的宽应该都是8的倍数，这一步个人觉得可以取消
			ptr++;
		}
	}
	return ;
}
// 写ASCII字符串 是否叠加字体背景mode
void lcdDrawString(int x,int y,char * const str,int font,int color,int mode)
{
	unsigned int cnt = 0; 	// 字符数统计
	int len = font/2; 		// ASCII字符的宽度（像素点）
	char *p = str;
	while(*p != '\0') 	// 判断是否到达字符串末尾
	{
		// 写单个ASCII字符，不叠加字符背景
		lcdDrawChar(x+cnt*len,y,*p,font,color,mode);
		p++; // 指向字符串下一个字符
		cnt++; // 字符数累加
	}
	return ;
}
//m^n函数
//返回值:m^n次方.
int Pow(int m,int n)
{
	int result = 1;	 
	while(n--)result*=m;    
	return result;
}
// 显示数字 高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);
void lcdShowNum(int x,int y,unsigned int num,int len,int size,int color)
{
	int i;
	int temp;
	char enshow=0;
	for(i = 0;i < len;i++)
	{
		temp=(num/Pow(10,len-i-1))%10; // 取出temp的每一位数字，从高位开始
		if(enshow==0 && i<(len-1)) // 一开始就判断高位 是否为零
		{	
			if(temp==0) // 高位为0不显示，之后的位数显示
			{	
				lcdDrawChar(x+(size/2)*i,y,' ',size,Red,0); // 高位为0，显示空格
				continue;
			}
			else 
				enshow=1; // 高位为0不显示，之后的位数显示
		}
	 	lcdDrawChar(x+(size/2)*i,y,temp+'0',size,Red,0); // 显示数字
	}
	return ;
}

// 画线 x1,y1:起点坐标 x2,y2:终点坐标  
void lcdDrawLine(int x1,int y1,int x2,int y2,int color)
{
	int t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; // 计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	
	if(delta_x > 0) incx = 1; 			//设置单步方向 
	else if(delta_x == 0) incx = 0;		//垂直线 
	else {incx = -1;delta_x = -delta_x;} 
	
	if(delta_y > 0) incy = 1; 
	else if(delta_y == 0) incy = 0;			//水平线 
	else{incy = -1;delta_y = -delta_y;} 
	
	if( delta_x > delta_y) distance = delta_x;//选取基本增量坐标轴 
	else distance = delta_y; 
	
	for(t=0;t<=distance+1;t++ )			// 画线输出 
	{  
		lcdDrawPoint(uRow,uCol,color);	//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
	return ;
}

// 画矩形(x1,y1),(x2,y2):矩形的对角坐标
void lcdDrawRectangle(int x1, int y1, int x2, int y2,int color)
{
	lcdDrawLine(x1,y1,x2,y1,color);
	lcdDrawLine(x1,y1,x1,y2,color);
	lcdDrawLine(x1,y2,x2,y2,color);
	lcdDrawLine(x2,y1,x2,y2,color);
	return ;
}
// 画三角
void lcdDrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int color)
{
	lcdDrawLine(x1,y1,x2,y2,color);
	lcdDrawLine(x1,y1,x3,y3,color);
	lcdDrawLine(x2,y2,x3,y3,color);
	return ;
}
// 在指定位置画一个指定大小的圆
void lcdDrawCircle(int x,int y,int r,int color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		// 使用Bresenham算法画圆
		lcdDrawPoint(x+a,y-b,color);             //5
 		lcdDrawPoint(x+b,y-a,color);             //0           
		lcdDrawPoint(x+b,y+a,color);             //4               
		lcdDrawPoint(x+a,y+b,color);             //6 
		lcdDrawPoint(x-a,y+b,color);             //1       
 		lcdDrawPoint(x-b,y+a,color);             
		lcdDrawPoint(x-a,y-b,color);             //2             
  		lcdDrawPoint(x-b,y-a,color);             //7     	         
		a++;
		// 使用Bresenham算法画圆    
		// 参考网址:https://www.cnblogs.com/wlzy/p/8695226.html
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
	return ;
} 			

// 调色板
void colorPalette(int x,int y,int w,int h)
{
	int i;
	for(i = 0;i < 9;i++)
	{
		lcdDrawBlock(x,y+h*i,w,h,colorArry[i]);
		lcdDrawString(x+5,y+5+h*i,colorArryChar[i],16,~colorArry[i],0);
	}
	lcdDrawString(x-15,y+360,"colorPalette",16,Red,0);
	printf("调色板信息：起始坐标(x,y)=(%d,%d) 色块宽 %d 高 %d\n",x,y,w,h);
}



// 彩虹效果1
void lcdDrawRainbow1(void)
{
	int i = 0,j = 0;
	int t = 0; // 七个颜色块
	int s = 114; // 渐变间距
	int color = 0x00FFFFFF; // 渐变色

	for(j = 0;j < 600;j++) // 逐行扫描
	{
		for(t = 0;t < 7;t++) // 确定每一块渐变色
		{
			int colorBegin = colorArry[t] ,colorEnd = colorArry[t+1];
			int r0 = (colorBegin >> 16) & 0xff;
			int g0 = (colorBegin >> 8) & 0xff;
			int b0 = colorBegin & 0xff;
			
			int r1 = (colorEnd >> 16) & 0xff;
			int g1 = (colorEnd >> 8) & 0xff;
			int b1 = colorEnd & 0xff;
			
			int F,rr,gg,bb;
			for(int i = 0; i < s; i++) // s为渐变间距
			{
				F = (i << 16) / s;
				rr = r0 + ((F * (r1 - r0)) >> 16);
				gg = g0 + ((F * (g1 - g0)) >> 16);
				bb = b0 + ((F * (b1 - b0)) >> 16);
				color = (rr << 16 | gg << 8 | bb);
				lcdDrawPoint(i+t*s,j,color);
			}
		}
	}
	return ;
}

// 网格特效
void gridEffect(int s,int color)
{
	int i;
	for(i = 0;i < 800;i+=s)
	{
		if(i < 600)
			lcdDrawLineX(0,i,800,1,color);
		
		lcdDrawLineY(i,0,600,1,color);
		lcdDrawLineY(i,0,600,1,color);
	}
}
// 回环框特效
void loopFrameEffect(int color)
{
	int i;
	for(i = 0;i < 300;i+=10)
		lcdDrawBox(5+i,5+i,790-2*i,590-2*i,1,color);
}

void getTime(char *cur_time) {
	char Year[6] = {0};
	char Month[4] = {0};
	char Day[4] = {0};
	char Hour[4] = {0};
	
	char Min[4] = {0};
	char Sec[4] = {0};

	time_t current_time;
	struct tm* now_time;
	time(&current_time);
	now_time = localtime(&current_time);

	strftime(Year, sizeof(Year), "%Y-", now_time);
	strftime(Month, sizeof(Month), "%m-", now_time);
	strftime(Day, sizeof(Day), "%d ", now_time);
	strftime(Hour, sizeof(Hour), "%H:", now_time);
	strftime(Min, sizeof(Min), "%M:", now_time);
	strftime(Sec, sizeof(Sec), "%S", now_time);

	strncat(cur_time, Year, 5);
	strncat(cur_time, Month, 3);
	strncat(cur_time, Day, 3);
	strncat(cur_time, Hour, 3);
	strncat(cur_time, Min, 3);
	strncat(cur_time, Sec, 3);
}
int yuv_to_rgb(int y, int u, int v)
{
	unsigned int pixel32 = 0;
	unsigned char pixel[3];// = (unsigned char *)&pixel32;
	int r, g, b;
	r = y + (1.370705 * (v-128));
	g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
	b = y + (1.732446 * (u-128));
	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;
	pixel[0] = r * 220 / 256;
	pixel[1] = g * 220 / 256;
	pixel[2] = b * 220 / 256;

	pixel32 = (pixel[0] << 16 ) | (pixel[1] << 8) | (pixel[2]);
	return pixel32;
}


void process_yuv_image(unsigned char *yuv, int width, int height)
{
	int i;
	int x=20,y=20;
	int Y0, U, Y1, V;
	int r1,g1,b1;
	int r2,g2,b2;
	int color1,color2;

	int pixel_num = 0;

 	for(i = 0; i < width * height * 2; i += 4) // 800*600 *2 就是数组的总大小  每4个一组
	{
		//将几个数据分量提取出来  y0在最低字节 u在 次低字节 y1在次高字节 V在 高字节 
		// Y0 = yuv[i + 0];
		// U  = yuv[i + 1] <<  8;
		// Y1 = yuv[i + 2] << 16;
		// V  = yuv[i + 3] << 24;
		
		Y0 = yuv[i + 0];
		U  = yuv[i + 1];
		Y1 = yuv[i + 2];
		V  = yuv[i + 3];
		
		r1 = Y0+1.370705*(V-128);
		g1 = Y0-0.698001*(V-128)-0.337633*(U-128);
		b1 = Y0+1.732446*(U-128);
		if(r1 > 255) r1 = 255;
		if(g1 > 255) g1 = 255;
		if(b1 > 255) b1 = 255;
		if(r1 < 0) r1 = 0;
		if(g1 < 0) g1 = 0;
		if(b1 < 0) b1 = 0;
		
		r1 = r1 * 220 / 256;
		g1 = g1 * 220 / 256;	
		b1 = b1 * 220 / 256;
		color1 = (r1 << 16 ) | (g1 << 8) | b1;
		lcdDrawPoint(x+pixel_num%width,y+pixel_num/width,color1);
		pixel_num++;
		
		r2 = Y1+1.4075*(V-128);
		g2 = Y1-0.3455*(V-128)-0.7169*(U-128);
		b2 = Y1+1.779*(U-128);
		if(r2>255) r2 = 255;
		if(g2>255) g2 = 255;
		if(b2>255) b2 = 255;
		if(r2<0) r2 = 0;
		if(g2<0) g2 = 0;
		if(b2<0) b2 = 0;
		
		r2 = r2 * 220 / 256;
		g2 = g2 * 220 / 256;
		b2 = b2 * 220 / 256;
		color2 = (r2 << 16 ) | (g2 << 8) | b2;
		lcdDrawPoint(x+pixel_num%width,y+pixel_num/width,color2);
		pixel_num++;
	}
	return ;
}

// 我的时钟
void Myclam(void)
{
	int h = 12,m = 0,s = 0;
	// 时钟
	if(s > 59)
	{
		s = 0;
		m++;
	}
	if(m > 59)
	{
		m = 0;
		h++;
	}
	if(h > 23)
		h =0;
	s++;
	sleep(1);
}


