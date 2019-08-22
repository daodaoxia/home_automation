#ifndef _DONG_H_
#define _DONG_H_
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/input.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <string.h>
#include <pthread.h>

#define SIZE_OF_LCD 800*480*4
#define SIZE_OF_PIC 800*480*3
#define RED   	0x00FF0000
#define GREEN 	0x0000FF00
#define GRAY  	0x00BEBEBE
#define ORANGE 	0x00EC715F
#define BLACK 	0x00000000


#define NUM_OF_WORDS1	4
#define NUM_OF_WORDS2	10
#define WORD_HEIGHT	 	48
#define WORD_WIDTH		48
#define LCD_WIDTH		800
#define LCD_HEIGHT		480
//#define SIZE_OF_WORD (WORD_HEIGHT*WORD_WIDTH/8)

//全局变量：显示屏映射 显示屏文件 进程标志 游戏标志
int *plcd,fd0,pid,game_flag,circle_flag;
//unsigned int buf[NUM_OF_WORDS][WORD_HEIGHT*WORD_WIDTH/8];

//单个像素点上色
void draw_point(int x,int y,int color);
//任意图片任意位置显示
void draw_pic(char *pic_ops,int x,int y);
//单个汉字/数字显示
void print_word(unsigned char* word,int height,int width,int x,int y,int color);
//多个汉字显示
void print_words(int num,int height,int width,int x,int y,int color);
//多个汉字滚动显示
void print_roll_words(int num,int height,int width,int x,int y,int color);
//多位数字显示
void print_num(int num1,int height,int width,int x,int y,int color);
//显示屏清屏
void clear_lcd(void);
//显示屏部分清屏
void clear_part_lcd(void);
//显示屏部分延时清屏
void delay_clear_part_lcd(void);
//触屏控制/
void touch_ctrl(void);
//LED灯控制
void led_ctrl(int led_num,int on_off,int x,int y);
//2048游戏
void play_game(void);
//获取GY39数据
void *get_data_gy39(void* arg);
//初始化界面
void init_interface(void);
//串口初始化
int init_serial(const char *file, int baudrate);
//屏保 画动圆
void lcd_draw_run_circle(unsigned r_x, unsigned int r_y, unsigned int radius, unsigned int mv_speed, unsigned int *lcd_ptr);
//屏保动圆控制
void circle_ctrl(unsigned r_x, unsigned int r_y, unsigned int radius, unsigned int mv_speed, unsigned int *lcd_ptr);

#endif