#include <stdio.h>
#include <string.h>
#include <stdlib.h>   
#include <unistd.h>
#include <fcntl.h> 
#include <poll.h>
#include "gpio_adaptor.h"
#include "mxj_error.h"


struct pollfd fds[INTR_MAX];
inter_t poll_fun[INTR_MAX];
int cont;
int mxj_gpio_get(int pin)
{
	char buffer[64];  
	int len;  
	int fd;  

	fd = open("/sys/class/gpio/export", O_WRONLY);  
	if (fd < 0) 
	{  
		MXJ_MSG("Failed to open export for writing!\r\n");  
		return -MXJ_FAIL;  
	}  

	len = snprintf(buffer, sizeof(buffer), "%d", pin);  
	if (write(fd, buffer, len) < 0) 
	{  
		MXJ_MSG("Failed to export gpio!\r\n");  
		return -MXJ_FAIL;  
	}  

	close(fd);  
	return MXJ_SUCCESS;    
	
}


int mxj_gpio_release(int pin)
{
	char buffer[64];  
    int len;  
    int fd;  
  
    fd = open("/sys/class/gpio/unexport", O_WRONLY);  
    if (fd < 0) 
	{  
        MXJ_MSG("Failed to open unexport for writing!\r\n");  
        return -MXJ_FAIL;  
    }  
  
    len = snprintf(buffer, sizeof(buffer), "%d", pin);  
    if (write(fd, buffer, len) < 0) 
	{  
        MXJ_MSG("Failed to unexport gpio!\r\n");  
        return -MXJ_FAIL;  
    }  
     
    close(fd);   
	
	return MXJ_SUCCESS;

}

int mxj_gpio_direction(int pin, int dir)
{
	static const char dir_str[] = "in\0out";  
    char path[64];  
    int fd;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) 
	{  
        MXJ_MSG("Failed to open gpio direction for writing!\r\n");  
        return -MXJ_FAIL;  
    }  
  
    if (write(fd, &dir_str[dir == 0 ? 0 : 3], dir == 0 ? 2 : 3) < 0) 
	{  
        MXJ_MSG("Failed to set direction!\r\n");  
        return -MXJ_FAIL;  
    }  
  
    close(fd);  
    
	return MXJ_SUCCESS;

}

int mxj_gpio_write(int pin, int value)

{
	static const char values_str[] = "01";  
    char path[64];  
    int fd;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) 
	{  
        MXJ_MSG("Failed to open gpio value for writing!\r\n");  
        return -MXJ_FAIL;  
    }  
  
    if (write(fd, &values_str[value == 0 ? 0 : 1], 1) < 0) 
	{  
        MXJ_MSG("Failed to write value!\r\n");  
        return -MXJ_FAIL;  
    }  
  
    close(fd);  
      
	return MXJ_SUCCESS;

}

int mxj_gpio_read(int pin)
{
	char path[64];  
    char value_str[3];  
    int fd;  
  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_RDONLY);  
    
    if (fd < 0) 
	{  
        MXJ_MSG("Failed to open gpio value for reading!\r\n");  
        return -MXJ_FAIL;  
    }  
  
    if (read(fd, value_str, 3) < 0) 
	{  
        MXJ_MSG("Failed to read value!\r\n");  
        return -MXJ_FAIL;  
    }  
  
    close(fd);  
    return (atoi(value_str));

}

int mxj_gpio_edge(int pin ,int edge)
{
	const char dir_str[] = "none\0rising\0falling\0both"; 
	char ptr;
	char path[64];  
	int fd; 
	switch(edge)
	{
		case 0:
		ptr = 0;
		break;
		case 1:
		ptr = 5;
		break;
		case 2:
		ptr = 12;
		break;
		case 3:
		ptr = 20;
		break;
		default:
		ptr = 0;
	} 

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/edge", pin);  
	fd = open(path, O_WRONLY);  
	if (fd < 0) 
	{  
		MXJ_MSG("Failed to open gpio edge for writing!\r\n");  
		return -MXJ_FAIL;  
	}  

	if (write(fd, &dir_str[ptr], strlen(&dir_str[ptr])) < 0)
	{  
		MXJ_MSG("Failed to set edge!\r\n");  
		return -MXJ_FAIL;  
	}  

	close(fd);  
	
	return MXJ_SUCCESS;
}

int mxj_gpio_inter_set(int pin , void (*function)(void))
{	
	int i=0;
	char buff[10];
	int ret= 0 ;
	int gpio_fd ;
	  
	for (i;i<INTR_MAX;i++)
	{
		if(poll_fun[i].avail == 0)
		{
			poll_fun[i].gpio = pin;
			poll_fun[i].fd = gpio_fd;
			poll_fun[i].index=i;
			poll_fun[i].avail=1;
			poll_fun[i].callback = function;
			cont++;
			
			break;
		}
	}
	//printf("i : %d \r\n",i);
	return MXJ_SUCCESS;

}

#if 1
int mxj_gpio_inter_start(void)
{	
	int i = 0;
	int gpio_fd, ret;
	char path[64]; 
	char buff[10];
	
	for(i;i<INTR_MAX;i++)
	{	
		if(poll_fun[i].avail == 1)
		{
			snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value",poll_fun[i].gpio);  
	    	gpio_fd= open(path, O_RDONLY);
			if(gpio_fd < 0)
			{
				MXJ_MSG("Failed to open value!\n");  
				return -MXJ_FAIL;	
			}
			poll_fun[i].fd = gpio_fd;
			fds[i].fd = gpio_fd;
			fds[i].events  = POLLPRI;
			ret = read(gpio_fd,buff,10);
			if( ret == -1 )
				MXJ_MSG("read\n");
		}
	}
	while(1)
	{	
		int i = 0;
		ret = poll(fds,cont+1,0);
		if( ret == -1 )
		MXJ_MSG("poll\n");
		for(i;i<cont;i++)
		{
		if( fds[i].revents & POLLPRI)
			{	
				printf("jianshu");
				ret = lseek(poll_fun[i].fd,0,SEEK_SET);
				if( ret == -1 )
				MXJ_MSG("lseek\n");
				ret = read(poll_fun[i].fd,buff,10);
				if( ret == -1 )
				MXJ_MSG("read\n");
				poll_fun[i].callback();
			}
		}
	usleep(1000);
	}
	return MXJ_SUCCESS;
	
}  
#endif


#if 0
int mxj_gpio_inter_start(void)
{
	int ret= 0 ;
	
	#if 0
	ret = read(poll_fun[0].fd,buff,10);
	if( ret == -1 )
	MXJ_MSG("read\n");
	#endif
	//lseek(poll_fun[0].fd,0,SEEK_SET);
	while(1)
	{
		printf(" ret ::%d ,revents::%d ----\r\n ",ret ,fds[0].revents);
	    ret = poll(fds,1,-1);
		printf("ret ::%d ,revents::%d GGGGG\r\n ",ret ,fds[0].revents);
		if( ret == -1 )
		MXJ_MSG("poll\r\n");
		
		if(fds[0].revents & POLLPRI)
		{	
			printf("%d ,%d \r\n ",ret ,fds[0].revents);
			printf("lijianshu_____++++++\r\n");
			ret = lseek(poll_fun[0].fd,0,SEEK_SET);
			if( ret == -1 )
			MXJ_MSG("lseek\r\n");
			//ret = read(poll_fun[0].fd,buff,10);
			if( ret == -1 )
			MXJ_MSG("read\r\n");
			printf("lijianshu___gggg__++++++%d\r\n",ret);
		}
		usleep(100000);
	}
	return MXJ_SUCCESS;
	
	}  
#endif




