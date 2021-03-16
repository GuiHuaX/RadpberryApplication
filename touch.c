
#include "touch.h"

int touchInit(void)
{
	// 打开触摸屏幕 
	int ts_fd = open(touchPATH,O_RDWR);
	if(ts_fd < 0)
	{
		perror("open touch ts_lcd error\n");
		return -1;
	}
	return 0;
}

/*获取触摸点坐标*/
int get_ts(struct touch_point *p)
{
	// 打开触摸屏幕 
	int ts_fd = open(touchPATH,O_RDWR);
	if(ts_fd < 0)
	{
		perror("open touch ts_lcd error\n");
		return -1;
	}
	
	p->x = -1; // 刚开始没有获取到操作坐标的时候应该为 -1 -1 
	p->y = -1; // p是一个结构体的指针(首地址),这个地址中有两个成员分别是 x 和 y 
	
	//第二步：读取触摸屏操作
	struct input_event data; 		// 中间结构体变量
	memset(&data,0,sizeof(data));   // 将这个存储单元中的所有内容全部清为 0 
	while(1)
	{
		int ret = read(ts_fd,&data,sizeof(data));
		if(ret < sizeof(data))
		{
			printf("read touch data failed\n");
			continue; // 继续
		}
		printf("type=%hd code=%hd value=%d\n", data.type, data.code, data.value);
		// EV_ABS:3  ABS_X:0  ABS_Y:1
		if(data.type == EV_ABS && data.code == ABS_X) // 类型为触摸点坐标
		{
			//此时读取到的是 X轴坐标 这个值在value中  但是你需要赋值给 x 
			if(p->x < 0 && p->y < 0)
				p->x = data.value;
		}
		else if(data.type == EV_ABS && data.code == ABS_Y) {
			// 此时读取到的是 Y轴坐标 这个值在value中 但是你需要赋值给 y 
			if(p->x >= 0 && p->y < 0)
			{
				p->y = data.value;
				break;
			}
		}
	}
	//把1024*600的坐标换算800*480坐标
	//*x=(*x)*800/1024;
	//*y=(*y)*480/600;
	printf("触摸操作！\n (x,y)=(%d,%d);\n",p->x,p->y);
	return 0;
}	

