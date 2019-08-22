#include "dong.h"

int main(void)
{	
	//忽略SIGCHLD信号
	signal(SIGCHLD, SIG_IGN); 
	//打开显示屏文件
	fd0 = open("/dev/fb0",O_RDWR);
	if(-1 == fd0)
	{
		printf("Open error !\n");
		return -1;
	}
	//映射	
	plcd = mmap(NULL,SIZE_OF_LCD,
	PROT_WRITE | PROT_READ,MAP_SHARED,fd0,0);
	//映射成功检测
	if(MAP_FAILED == plcd)
	{
		printf("Map lcd failed !\n");
		return -1;
	}
	//显示苹果开机界面
	draw_pic("apple.bmp",0,0);
	sleep(3);
	draw_pic("apple1.bmp",0,0);
	usleep(500000);
	draw_pic("apple2.bmp",0,0);
	usleep(500000);

	//创建线程获取GY39的各项数据并实时显示
	pthread_t pid;
	pthread_create(&pid,NULL,get_data_gy39,NULL);
	
	while(1)
	{
		
		//初始化界面
		init_interface();
		//创建子进程滚动显示“欢迎使用智能家居系统”
		int pid1 = fork();
		if(pid1 == 0)
		{
			print_roll_words(NUM_OF_WORDS2,WORD_HEIGHT,WORD_WIDTH,0,427,GRAY);
		}
		else if(pid1> 0)
		{

			//触屏控制
			touch_ctrl();
			//游戏判断
			if(game_flag || circle_flag)
			{
				//杀死子进程 专心玩小游戏
				unsigned char cmd6[32];
				sprintf(cmd6,"kill -15 %d",pid1);
				system(cmd6);
				//等待GY39线程先完成一次循环
				usleep(500000);
				if(circle_flag)
				{
					circle_ctrl(300,200,60,2,plcd);
					usleep(10000);
				}
				else
				{
					//显示游戏按钮
					draw_pic("game_load.bmp",536,230);
					delay_clear_part_lcd();
					//运行小游戏
					play_game();
					usleep(200000);
				}
			}
		}
	}
	//关闭映射
	munmap(plcd,SIZE_OF_LCD);
	//关闭文件
	close(fd0);
	return 0;
}