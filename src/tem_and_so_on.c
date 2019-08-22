#include "dong.h"

//��ʼ������
//file: ��������Ӧ���ļ���
//baudrate��������
int init_serial(const char *file, int baudrate)
{ 
	int fd;
	
	fd = open(file, O_RDWR);
	if (fd == -1)
	{
		perror("open device error:");
		return -1;
	}

	struct termios myserial;
	//��սṹ��
	memset(&myserial, 0, sizeof (myserial));
	//O_RDWR               
	myserial.c_cflag |= (CLOCAL | CREAD);
	//���ÿ���ģʽ״̬���������ӣ�����ʹ��
	//���� ����λ
	myserial.c_cflag &= ~CSIZE;   //�������λ
	myserial.c_cflag &= ~CRTSCTS; //��Ӳ��������
	myserial.c_cflag |= CS8;      //����λ:8

	myserial.c_cflag &= ~CSTOPB;//   //1λֹͣλ
	myserial.c_cflag &= ~PARENB;  //��ҪУ��
	//myserial.c_iflag |= IGNPAR;   //��ҪУ��
	//myserial.c_oflag = 0;  //����ģʽ
	//myserial.c_lflag = 0;  //�������ն�ģʽ

	switch (baudrate)
	{
		case 9600:
			cfsetospeed(&myserial, B9600);  //���ò�����
			cfsetispeed(&myserial, B9600);
			break;
		case 115200:
			cfsetospeed(&myserial, B115200);  //���ò�����
			cfsetispeed(&myserial, B115200);
			break;
		case 19200:
			cfsetospeed(&myserial, B19200);  //���ò�����
			cfsetispeed(&myserial, B19200);
			break;
	}
	
	/* ˢ���������,��������ܵ����� */
	tcflush(fd, TCIFLUSH);

	/* �ı����� */
	tcsetattr(fd, TCSANOW, &myserial);

	return fd;
}
void *get_data_gy39(void *arg)
{
	//��ʼ��������
	int serial_fd = init_serial("/dev/ttySAC1",9600);
	//����Led10���Ʒ�
	int jg_led10 = 0;
	while(1)
	{
		unsigned char cmd[3] = {0xA5,0x83,0x28};
		unsigned char buf[24] = {0};
		
		int ret = write(serial_fd,cmd,3);//��ָ��
		if(ret < 3)
		{
			perror("write serial_fd error\n");
		}
		ret = read(serial_fd,buf,24);//������
		if(ret < 24 || buf[0] != 0x5A || buf[1] != 0x5A)
		{	
			usleep(200000);
			continue;
		}
		//��������
		
		//����ǿ�� 20
		int lux = buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7];
		lux = lux / 100;
		//����Led10����
		if(lux < 12)
		{
			jg_led10 = 1;
			led_ctrl(10,jg_led10,677,144);
		}
		else
		{
			jg_led10 = 0;
			led_ctrl(10,jg_led10,677,144);
		}

		//����  100
		int h = buf[21] << 8 | buf[22];
			
		//��ѹ  90000
		int p = buf[15] << 24 | buf[16] << 16 | buf[17] << 8 | buf[18];
		p = p / 100;

		//�¶�  20
		int t = buf[13] << 8 | buf[14];
		t = t / 100;
		
		//ʪ��  
		int hum = buf[19] << 8 | buf[20];
		hum = hum / 100;
		
		if(!game_flag && !circle_flag)
		{
			//��ʾ�¶�/ʪ��ģ��
			draw_pic("2_part.bmp",276,172);
			//��ʾ��ǿ/����/ѹǿ/ģ��
			draw_pic("3_part.bmp",10,10);
			//��ʾ��ǿ
			print_num(lux,48,24,116,79,GRAY);
			//��ʾ����
			print_num(h,48,24,101,200,GRAY);
			//��ʾѹǿ
			print_num(p,48,24,77,330,GRAY);
			//��ʾ�¶�
			print_num(t,48,24,376,216,GRAY);
			//��ʾʪ��
			print_num(hum,48,24,380,330,GRAY);
			usleep(500000);
		}
		else
		{
			usleep(800000);
		}
	}
	close(serial_fd); 
	return NULL;
}