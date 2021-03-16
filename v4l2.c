

#include "v4l2.h"
#include "ghx_ui.h"
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
enum v4l2_buf_type v4l2BufType = V4L2_BUF_TYPE_VIDEO_CAPTURE;

int cameraInit()
{
	// 打开设备
    video_fd = open(CAMERA_DEVICE, O_RDWR, 0);
    if (video_fd < 0) {
        printf("Open %s failed\n", CAMERA_DEVICE);
        return -1;
    }
	printf("#Open camera... successful\n");
	
    // 获取驱动功能信息
	int ret = ioctl(video_fd, VIDIOC_QUERYCAP, &cap);
    if (ret < 0) {
        printf("VIDIOC_QUERYCAP failed (%d)\n", ret);
        return ret;
    }
	printf("#Get capabilityInfo... successful\n");
    // Print capability infomations
    printf("#Capability Informations:\n");
    printf(" driver: %s\n", cap.driver);
    printf(" card: %s\n", cap.card);
    printf(" bus_info: %s\n", cap.bus_info);
    printf(" version: %08X\n", cap.version);
    printf(" capabilities: %08X\n", cap.capabilities);
	printf("\n");
	
	// 枚举摄像头支持哪些视频/图片帧格式
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("#摄像头支持的视频/图片帧格式\n");
	do{
		ret = ioctl(video_fd,VIDIOC_ENUM_FMT,&fmtdesc);
		printf(" index:%d,flag:%d, format name:%s, pixelformat=0x%x \n",
			fmtdesc.index,fmtdesc.flags,fmtdesc.description,fmtdesc.pixelformat);
		fmtdesc.index++;
	}while(0 == ret);
	printf("\n");
	
    // 设置视频格式
    memset(&format, 0, sizeof(format));
    format.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE; // 
    format.fmt.pix.width       = VIDEO_WIDTH;// 帧宽 必须是16的倍数
    format.fmt.pix.height      = VIDEO_HEIGHT;// 帧高 必须是16的倍数
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;// 视频数据存储类型，例如是YUV4：2：2还是RGB：（V4L2_PIX_FMT_JPEG、 V4L2_PIX_FMT_YUYV）
    format.fmt.pix.field       = V4L2_FIELD_INTERLACED; 
	
	// 验证设置的格式是否正确
	ret = ioctl(video_fd, VIDIOC_TRY_FMT, &format);
	if(ret < 0)
	{
		printf("VIDIOC_TRY_FMT failed (%d)\n", ret);
        return ret;
	}
	printf("#Check format... successful\n");

	// 设置当前驱动的捕获格式 
    ret = ioctl(video_fd, VIDIOC_S_FMT, &format);
    if (ret < 0) {
        printf("VIDIOC_S_FMT failed (%d)\n", ret);
        return ret;
    }
	printf("#Set format... successful\n");
	
    // 读取当前驱动的捕获格式 
    ret = ioctl(video_fd, VIDIOC_G_FMT, &format);
    if (ret < 0) {
        printf("VIDIOC_G_FMT failed (%d)\n", ret);
        return ret;
    }
    // Print Stream Format
    printf("#Stream Format Informations:\n");
    printf(" type: %d\n", format.type);
    printf(" width: %d\n", format.fmt.pix.width);
    printf(" height: %d\n", format.fmt.pix.height);
    char fmtstr[8];
    memset(fmtstr, 0, 8);
    memcpy(fmtstr, &format.fmt.pix.pixelformat, 4);
    printf(" pixelformat: %s\n", fmtstr);
    printf(" field: %d\n", format.fmt.pix.field);
    printf(" bytesperline: %d\n", format.fmt.pix.bytesperline);
    printf(" sizeimage: %d\n", format.fmt.pix.sizeimage);
    printf(" colorspace: %d\n", format.fmt.pix.colorspace);
    printf(" priv: %d\n", format.fmt.pix.priv);
    printf(" raw_date: %s\n", format.fmt.raw_data);
	printf("\n");

    // 申请视频缓冲
	memset(&reqbuf,0,sizeof(reqbuf));
    reqbuf.count = BUFFER_COUNT;// 请求的缓冲帧数最大是 30  1s钟最多获取30张图片
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    // 在摄像头驱动中申请缓冲空间 设备中分配内存
    ret = ioctl(video_fd , VIDIOC_REQBUFS, &reqbuf);
    if(ret < 0) {
        printf("VIDIOC_REQBUFS failed (%d)\n", ret);
        return ret;
    }
	printf("#Request buf... successful\n");

    // 申请程序中的内存空间 并将缓冲区映射到内存中 再把它加入到采集队列中去。
	framebuf = calloc(reqbuf.count, sizeof(*framebuf) );
	int i;
    for (i = 0; i < reqbuf.count; i++) 
    {
		memset(&buf, 0, sizeof(buf));
        buf.index = i;// i 就是一帧数据中的第几张图片 
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // 缓冲类型
        buf.memory = V4L2_MEMORY_MMAP; // 内存类型。
		
		// 将VIDIOC_REQBUFS中分配的数据缓存转换成物理地址
        ret = ioctl(video_fd , VIDIOC_QUERYBUF, &buf);
        if(ret < 0) {
            printf("VIDIOC_QUERYBUF (%d) failed (%d)\n", i, ret);
            return ret;
        }

        // 将缓冲区映射到内存中 每查到一个内核缓冲 就映射一次
        framebuf[i].length = buf.length;
        framebuf[i].start = (char *) mmap(0, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, video_fd, buf.m.offset);
        if (framebuf[i].start == MAP_FAILED) {
            printf("mmap (%d) failed: %s\n", i, strerror(errno));
            return -1;
        }
    
        // Queen buffer 把记录缓冲区数据的结构体放入采集队列中，以便记录图像帧
        ret = ioctl(video_fd , VIDIOC_QBUF, &buf);
        if (ret < 0) {
            printf("VIDIOC_QBUF (%d) failed (%d)\n", i, ret);
            return -1;
        }

        printf("Frame buffer %d: address=0x%x, length=%d\n", i, (unsigned int)framebuf[i].start, framebuf[i].length);
    }
	printf("#Mmap buf... successful\n");
	printf("#Camera is ready!\n");
	return 0;
}
// 解除初始化 释放空间
int cameraUninit(void)
{
	int i;
	// Release the resource 停止视频采集并解除映射
    for (i = 0; i< reqbuf.count; i++) 
    {
        munmap(framebuf[i].start, framebuf[i].length);
    }
	printf("#Munmap buf... successful\n");

    if(close(video_fd) < 0)
	{
		printf("failed to close camera!\n");
		return -1;
	}
	printf("#Close camera... successful\n");
    printf("#Camera test Done.\n");
	
	// free memory 
    free(framebuf);
    return 0;
}
// 开始采集
int startCapture(void)
{
	// 查询缓冲区结束后才可以开始采集
    // 开始录制 开始采集视频数据
    int ret = ioctl(video_fd, VIDIOC_STREAMON, &v4l2BufType);
    if (ret < 0) {
        printf("VIDIOC_STREAMON failed (%d)\n", ret);
        return ret;
    }
	return 0;
}
// 停止采集
int stopCapture(void)
{
	//停止采集
	int ret = ioctl(video_fd,VIDIOC_STREAMOFF,&v4l2BufType);
	if(ret < 0)
	{
		perror("ioctl VIDIOC_STREAMOD error");
		return -1;
	}
	return 0;
}
int dataProce(void)
{
	//设置了一个 IO多路复用来管理fd这一个文件 
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(video_fd,&rfds); //将fd加入到了 文件描述符集中 
    
	
	while(1)
	{
		int r = select(video_fd+1,&rfds,NULL,NULL,NULL);
		if(r<=0)
			continue;//失败或者是 超时 再次监听 
		else  //监听到了 
		{
			if(FD_ISSET(video_fd,&rfds)) //操作  
			{
				// Get frame 读取frame
				int ret = ioctl(video_fd, VIDIOC_DQBUF, &buf);
				if (ret < 0) {
					printf("VIDIOC_DQBUF failed (%d)\n", ret);
					return ret;
				}
				
				//process_yuv_image(framebuf[buf.index].start,VIDEO_WIDTH,VIDEO_HEIGHT);
				
				/*
				char image_path[80]=INITIAL_IMAGE_PATH;
				// 获取当前时间，叠加到帧名上
				strcpy(image_path,INITIAL_IMAGE_PATH);
				//strcpy(image_path,CAPTURE_FILE);
				get_image_name(image_path,1);//get new path name
				
				// Process the frame 处理帧数据
				FILE *fp = fopen(image_path, "wb");
				if (fp < 0) {
					printf("open frame data file failed\n");
					return -1;
				}
				fwrite(framebuf[buf.index].start, 1, buf.length, fp);
				fclose(fp); // 关闭文件
				printf("#Capture one frame saved in %s\n", image_path);
				*/
				
				// Re-queen buffer将出队的缓冲帧重新放入缓存队列,以便再次获得
				ret = ioctl(video_fd, VIDIOC_QBUF, &buf);
				if (ret < 0) {
					printf("VIDIOC_QBUF failed (%d)\n", ret);
					return ret;
				}
			}
		}
	}
	return 0;
}


//get each image's path name via different catching time
void get_image_name(char _str[],int _num) // 获取时间
{
	char time_str[30];
	
	time_t current_time;
	struct tm *p;
	time(&current_time);
	p=localtime(&current_time);
	
	// 把时间值存放在time_str中
	sprintf(time_str,"%d%d%d_%d_%d_%d_%d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec,_num);
	strcat(time_str,".jpg"); // 加上后缀
	strcat(_str,time_str);	
}

