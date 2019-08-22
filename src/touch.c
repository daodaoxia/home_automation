#include "dong.h"

void touch_ctrl(void)
{	
	//打开触屏文件
	int touch_fd = open("/dev/input/event0",O_RDWR);
	if(touch_fd < 0)
	{
		perror("open touch_fd error\n");
	}
	
	//游戏控制标志
	//game_flag = 0;
	//屏保控制标志
	circle_flag = 0;
	//触摸屏坐标
	int x,y;
	//音乐控制变量
	int jg = 1,jg_1 = 0,name_flag = 0;
	//Led灯控制变量
	int jg_led9 = 0,jg_led8 = 0;
	//歌曲名字符数组
	char name[3][16] = {"mbx.mp3","sx.mp3","wmll.mp3"};
	//系统控制字符数组
	unsigned char cmd0[32];
	unsigned char cmd1[32];
	unsigned char cmd2[32];
	unsigned char cmd3[32];
	unsigned char cmd5[32];
	unsigned char cmd7[32];
	
	//检测触屏
	while(1)
	{
		struct input_event ev;
		//读取触摸屏事件
		int ret = read(touch_fd,&ev,sizeof(ev));
		if(ret < sizeof(ev))
		{
			continue;
		}
		
		//认为是一个X事件
		if(ev.type == EV_ABS && ev.code == ABS_X)
		{
			x = ev.value;
		}
		//认为是一个Y事件
		else if(ev.type == EV_ABS && ev.code == ABS_Y)
		{
			y = ev.value;
		}
		//认为按下去了
		else if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value != 0 )
		{
			
		}
		//认为此时弹起来了
		else if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
		{
			//判断是否在game按钮坐标范围内
			if((x < 790)&&(x > 536)&&(y < 400)&&(y > 230))
			{
				game_flag = 1;
				if (jg_1)
				{
					sprintf(cmd7,"kill -15 %d",pid);
					system(cmd7);
				}
				break;
			}
			//判断是否在Led8坐标范围内
			if((x < 680)&&(x > 540)&&(y < 72)&&(y > 0))
			{
				jg_led8 = ++jg_led8 % 2;
				led_ctrl(8,jg_led8,677,0);
				continue;
			}
			//判断是否在Led9坐标范围内
			else if((x < 680)&&(x > 540)&&(y < 144)&&(y > 72))
			{
				jg_led9 = ++jg_led9 % 2;
				led_ctrl(9,jg_led9,677,72);	
				continue;
			}
			//判断是否在屏保按键坐标范围内
			else if((x < 680)&&(x > 540)&&(y < 216)&&(y > 144))
			{
				circle_flag = 1;
				break;
			}
			//判断是否在下一首按键坐标范围内
			else if((x < 520)&&(x > 460)&&(y < 154)&&(y > 104))
			{
				name_flag ++;
				name_flag = name_flag % 3;
				if(jg_1)
				{
					//杀死已存在子进程
					sprintf(cmd1,"kill -15 %d",pid);
					system(cmd1);
				}
				//播放暂停标志位及首次播放标志位置反
				jg = -1;
				jg_1 = 1;
				//生成新的子进程
				pid = fork();
				if(pid == 0 )	
				{	
					draw_pic("music_pause.bmp",276,74);
					//播放音乐
					execlp("madplay","madplay",name[name_flag],NULL);
				}
				continue;
				
			}
			//判断是否在上一首按键坐标范围内
			else if((x < 340)&&(x > 280)&&(y < 154)&&(y > 104))//93  87  60 50
			{
				name_flag += 2;
				name_flag = name_flag % 3;
				if(jg_1)
				{
					//杀死已存在进程
					sprintf(cmd2,"kill -9 %d",pid);
					system(cmd2);
				}
				//播放暂停标志位及首次播放标志位置反
				jg = -1;
				jg_1 = 1;
				//生成新的子进程
				pid = fork();
				//子进程
				if(pid == 0 )	
				{	
					draw_pic("music_pause.bmp",276,74);
					//播放音乐
					execlp("madplay","madplay",name[name_flag],NULL);
				}
				continue;
			}
			//判断是否在播放/暂停按键坐标范围内
			else if((x < 354)||(x > 447)||(y < 74)||(y > 171))
				continue;
			//标志位置反
			jg = 0 - jg;
			//判断是否多次播放
			if(jg > 0 && jg_1 == 0)
				jg_1 = 1;
			//判断是否首次播放
			if(jg == -1 && jg_1 == 0)
			{	
				pid = fork();
				//子进程
				if(pid == 0 )	
				{	
					draw_pic("music_pause.bmp",276,74);
					//播放音乐
					execlp("madplay","madplay",name[name_flag],NULL);
				}
			}
			if(jg == 1 && jg_1 == 1)
			{
				//暂停进程
				sprintf(cmd0,"kill -19 %d",pid);
				system(cmd0);
				draw_pic("music_play.bmp",276,74);
			}
			if(jg == -1 && jg_1 == 1)
			{
				//继续进程
				sprintf(cmd3,"kill -18 %d",pid);
				system(cmd3);
				draw_pic("music_pause.bmp",276,74);
			}
		}
	}
	return;
}