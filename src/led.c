#include "dong.h"

void led_ctrl(int led_num,int on_off,int x,int y)
{	
	int led_fd = open("/dev/led_drv",O_RDWR);
	if(led_fd < 0)
	{
		perror("open led_fd error\n");
	}
	
	unsigned char cmd[2];
	cmd[0] = on_off;  //cmd[0]����ǵƵ�״̬1���� 0����
	cmd[1] = led_num; //cmd[1]����ǵƵı��
	
	write(led_fd,cmd,2);
	close(led_fd);
	if(!game_flag && !circle_flag)
	{
		if(on_off)
		{
			draw_pic("light_on.bmp",x,y);
		}
		else
		{
			draw_pic("light_off.bmp",x,y);
		}
	}
	return;
}