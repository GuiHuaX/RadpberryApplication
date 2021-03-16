

// v4l2.h
/*
	Author:	tkz
	Date:	2020/12/05
*/
/*操作的过程:
1、打开摄像头；
2、获取摄像头的capability（它所支持的操作）
3、获取摄像头支持哪些视频/图片帧格式；
4、设置视频/图片/帧格式；
5、在摄像头驱动中申请缓冲空间用来临时存放抓取到的照片；
6、申请程序中的内存空间并将缓冲区映射到内存中；
7、开始采集视频并处理数据；
8、停止视频采集并解除映射；
9、关闭摄像头。
*/

#ifndef _V4L2_H_
#define _V4L2_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <getopt.h>           
#include <fcntl.h>            
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>        
#include <linux/videodev2.h>

#define CAMERA_DEVICE "/dev/video0"  // 设备地址
#define CAPTURE_FILE "frame.jpg"  // 
#define INITIAL_IMAGE_PATH "/home/pi/tkz/"

#define VIDEO_WIDTH 480   // 帧宽
#define VIDEO_HEIGHT 360  // 帧高
#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV  // 帧格式
#define BUFFER_COUNT 4  // 请求的设备缓冲帧数

typedef struct VideoBuffer {  // 申请的程序内存空间
    void   *start;
    size_t  length;
} VideoBuffer;

static int video_fd;
static struct v4l2_capability cap; 		// 驱动功能信息
static struct v4l2_fmtdesc fmtdesc;		// 摄像头支持哪些视频/图片帧格式
static struct v4l2_format format;		// 帧格式
static struct v4l2_requestbuffers reqbuf;// 请求分配的缓存
VideoBuffer*  framebuf; 				// 将buf映射到framebuf
static struct v4l2_buffer buf;         	// VIDIOC_REQBUFS中分配的数据缓存reqbuf转换成的物理地址buf



int cameraInit(void);  // 初始化摄像头
int cameraUninit(void); // 解除初始化
int startCapture(void); // 开始采集
int dataProce(void); // 数据处理
int stopCapture(void); // 停止采集
static void get_image_name(char _str[],int _num); // 获取时间





#endif


