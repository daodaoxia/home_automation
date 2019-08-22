#include "dong.h"

char *bmpname[]={"0.bmp",
				 "2.bmp",
				 "4.bmp",
				 "8.bmp",
				 "16.bmp",
				 "32.bmp",
				 "64.bmp",
				 "128.bmp",
				 "256.bmp",
				 "512.bmp",
				 "1024.bmp",
				 "2048.bmp",
				 "4096.bmp",
				 "8192.bmp",
				 "16384.bmp",
				 "32768.bmp",
				 "65536.bmp",
				 };

int bound[4][4]={0,0,0,0,
				0,2,0,0,
				0,0,0,0,
				0,0,0,4};

int gmover_flag;
/*****************************
映射屏幕画点
x表示行；
y表示列
******************************/



/*****************************
创建一个界面，
x0，y0为界面在arm版上起点的位置
w为界面的宽度
h为界面的高度
color为界面的颜色
******************************/
void lcd_bound(int x0,int y0,int w,int h,int color)
{
	int x,y;
	for(x=x0;x<x0+w;x++)
		{
			for(y=y0;y<y0+h;y++)
				{
					draw_point(y,x,color);
				}
		}
}

/********************************
在创建好的界面上建立一个4*4的矩阵
x0,y0表示小正方形的初始位置
n表示小正方形的边长
dis表示矩阵之间的距离
color表示矩阵的颜色
*********************************/
void bound_matrix(int x0,int y0,int n,int dis,int color)
{
	int i,j;
	for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
				{
					lcd_bound(x0+i*dis,y0+j*dis,n,n,color);
				}
		}

}


/*******************************
在x0,y0处插入图片
*********************************/
void lcd_bmp(char*file,int x0,int y0)
{
	int fd_picture=open(file,O_RDONLY);
	if(-1==fd_picture)
		{
			printf("open error!");
			return ;
		}
	//把图片信息读取
	char buf[100*100*3+54];
	read(fd_picture,buf,100*100*3+54);
	//关闭图片
	close(fd_picture);
	//把像素解析到屏幕
	char b,g,r;
	int x,y,color;
	int i=0;
	for(x=100+x0;x>x0;x--)
	{
		for(y=y0;y<100+y0;y++)
			{
				b=buf[54+i];
				g=buf[55+i];
				r=buf[56+i];
				i+=3;
				color=(r<<16)|(g<<8)|b;
				//显示到开发板相应的位置
				draw_point(y,x,color);
				
			}
	}
	close(fd_picture);	
}
/*******************************
获得图片的名字
*******************************/
char* get_bmpname(int digit)
{
	int index;
	switch (digit)
	{ 
		case 0:
			index = 0;
			break;
		case 2:
			index = 1;
			break;
		case 4:
			index = 2;
			break;
		case 8:
			index = 3;
			break;
		case 16:
			index = 4;
			break;
		case 32:
			index = 5;
			break;
		case 64:
			index = 6;
			break;
		case 128:
			index = 7;
			break;
		case 256:
			index = 8;
			break;
		case 512:
			index = 9;
			break;
		case 1024:
			index = 10;
			break;
		case 2048:
			index = 11;
			break;
		case 4096:
			index = 12;
			break;
		case 8192:
			index = 13;
			break;
		case 16384:
			index = 14;
			break;
		case 32768:
			index = 15;
			break;
		case 65536:
			index = 16;
			break;
		default:
			index = 1;
			break;
	}
	 
	return bmpname[index];

}

/*********************************
将图片传到矩阵
**********************************/
void lcd_matrix()
{
	int i, j;
	
	for (i = 0 ; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
		
			lcd_bmp(get_bmpname(bound[i][j]),10+120*i,170+120*j);
			
		}
	}
}

/*********************************************
随机函数
**********************************************/
void addRandomNum()
{
	srand((int)time(NULL));
	int x=rand()%20;
	int i,j,w=0;
	int m,n;
	for( m=0;m<4;m++)
		{
			for( n=0;n<4;n++)
				{
					if(bound[m][n]==0)
						{
							w++;
						}
				}
		}
	while(1)
		{
			if(w)
			{
				i=rand()%4;
				j=rand()%4;
				if(bound[i][j]==0)
					{
						bound[i][j]=(x==4?4:2);
						break;
					}
				else
					{
					  continue;
					}
			}
			else
				{
					break;
				}
		}
}
void moveup()
{
	int i,j,k;
	for(j=0;j<4;j++)
	{
		int flag=1;
		for(i=1;i<4;i++)
		{//移动点i，j 
			for(k=i;k>=1;k--)//移动到的现在的位置 
			if(bound[k-1][j]||k==1)//此时停住
			{
				if(k==1&&bound[k-1][j]==0)
				{
					bound[k-1][j]=bound[i][j];
					bound[i][j]=0;
					continue;
				}
				if(bound[i][j]==bound[k-1][j]&&flag)
				{
					bound[k-1][j]=2*bound[i][j];
					bound[i][j]=0;
					flag=0;
				}
				else
				if(k!=i)//如果移动了 
				{
					bound[k][j]=bound[i][j];
					bound[i][j]=0;
				}
				break;
			}
		}
	}
}
void movedown()
{
	int i,j,k;
	for(j=0;j<4;++j)
	{
	    int flag=1;
		for(i=2;i>=0;--i)
		{//移动点i，j 
			
			for(k=i;k<=2;++k)//移动到的现在的位置 
			if(bound[k+1][j]||k==2)//此时停住
			{
				if(k==2&&bound[k+1][j]==0)
				{
					bound[k+1][j]=bound[i][j];
					bound[i][j]=0;
					continue;
				}
				if(bound[i][j]==bound[k+1][j]&&flag)
				{
					bound[k+1][j]=2*bound[i][j];
					bound[i][j]=0;
					flag=0;
				}
				else
				if(k!=i)//如果移动了 
				{
					bound[k][j]=bound[i][j];
					bound[i][j]=0;
				}
				break;
			}
		}
	}
}
void moveleft()
{
	int i,j,k;
	for(i=0;i<4;++i)
	{
		int flag=1;
		for(j=1;j<4;++j)
		if(bound[i][j])
		{//移动点i，j 
			for(k=j;k>=1;--k)//移动到的现在的位置
			if(bound[i][k-1]||k==1)//此时停住
			{
				if(k==1&&bound[i][k-1]==0)
				{
					bound[i][k-1]=bound[i][j];
					bound[i][j]=0;
					continue;
				}
				if(bound[i][j]==bound[i][k-1]&&flag)
				{
					bound[i][k-1]=2*bound[i][j];
					bound[i][j]=0;
					flag=0;
				}
				else
				if(k!=j)//如果移动了 
				{
					bound[i][k]=bound[i][j];
					bound[i][j]=0;
				}
				break;
			}
		}
	}
}
void moveright()
{
	int i,j,k;
	for(j=2;j>=0;--j)
	{
		for(i=0;i<4;++i)
		{//移动点i，j 
			int flag=1;
			for(k=j;k<=2;++k)//移动到的现在的位置
			if(bound[i][k+1]||k==2)//此时停住
			{
				if(k==2&&bound[i][k+1]==0)
				{
					bound[i][k+1]=bound[i][j];
					bound[i][j]=0;
					continue;
				}
				if(bound[i][j]==bound[i][k+1]&&flag)
				{
					bound[i][k+1]=2*bound[i][j];
					bound[i][j]=0;
					flag=0;
				}
				else
				if(k!=j)//如果移动了 
				{
					bound[i][k]=bound[i][j];
					bound[i][j]=0;
				}
				break;
			}
		}
	}
}

/**********************************************
判断手指滑动屏幕的方向
***********************************************/
void lcd_touch()
{
	int fd_abs = open("/dev/input/event0",O_RDWR);
		if(fd_abs == -1)
		{
			printf("open event0 error\n");
			return ;
		}
		//2.read  输入事件(struct input_event)
		struct input_event ev;
		int x1=-1,y1=-1;
		int x2,y2;
		while(1)
		{
			read(fd_abs,&ev,sizeof(ev));
			//3.分析读到的数据
			//判断当前的事件是否为触摸事件
			if(ev.type == EV_ABS)
			{
				if(ev.code == ABS_X)
				{
					if(x1 == -1)
					{
						x1 = ev.value;
					}
					x2 = ev.value;
				}else if(ev.code == ABS_Y)
				{
					if(y1 == -1)
					{
						y1 = ev.value;
					}
					y2 = ev.value;
				}
			}
			if((x2 < 112)&&(x2 > 0)&&(y2 < 364)&&(y2 > 196))
			{
				gmover_flag = 1;
				return;
			}
			//手指弹起来
			/*
			if(ev.type == EV_ABS && 
				ev.code == ABS_PRESSURE &&
				ev.value == 0)
			*/
			
			if(ev.type == EV_KEY && 
				ev.code == BTN_TOUCH &&
				ev.value == 0)
			{
				break;
			}	
		}
		//4.close
		close(fd_abs);
		if( abs(y2-y1) > abs(x2-x1) && y2 < y1)
		{
			moveup();
		}else if( abs(y2-y1) > abs(x2-x1) && y2 > y1)
		{
			movedown();
		}else if( abs(y2-y1) < abs(x2-x1) && x2 < x1)
		{
			moveleft();
		}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
		{
			moveright();
		}

}
/**********************************
游戏结束
**********************************/
int gameover()
{
	int i,j,n=0;
	if(gmover_flag)
	{
		return 1;
	}
	for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
				{
					if(bound[i][j]==0)
						{
							return 0;
						}
				}
		}

		for(i=0;i<4;i++)
		{
			for(j=0;j<3;j++)
				{
					if(bound[i][j]==bound[i][j+1])
						{
							return 0;
						}
				}
		}
	for(j=0;j<4;j++)
			{
				for(i=0;i<3;i++)
					{
						if(bound[i+1][j]==bound[i][j])
							{
								return 0;
							}
					}
			}
	return 1;
}

void play_game(void)
{
	
	draw_pic("game_bg.bmp",0,0);
	usleep(500000);
	bound_matrix(10,170,100,120,0x00ff7fff);
	lcd_matrix();
	draw_pic("game_back.bmp",0,150);
	gmover_flag = 0;
	int i,j;
	
	while(1)
	{
		lcd_touch();
		addRandomNum();
		lcd_matrix();
		if(gameover())
		{
			draw_pic("game_over.bmp",0,0);
			usleep(800000);
			//bound初始化
			for(i = 0 ;i < 4 ;i++ )
			{
				for(j = 0 ; j < 4 ; j++)
				{
					bound[i][j] = 0;
				}
			}
			bound[1][1] = 2;
			bound[3][3] = 4;
			game_flag = 0 ;

			break;
		}
	}
}