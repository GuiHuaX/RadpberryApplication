
#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct touch_point // 触摸点坐标
{
	int x;
	int y;
};
struct touch_point touch; // 触摸点坐标

static char *touchPATH = "/dev/input/event1";
static char *mousePATH = "/dev/input/mouse1";

int touchInit(void);

int touchUnInit(void);
// 获取触摸点坐标
int get_ts(struct touch_point *p);


#endif
