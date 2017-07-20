#ifndef _GPIO_ADAPTOR_H_
#define _GPIO_ADAPTOR_H_

#ifdef __cplusplus
    extern "C" {
#endif

#include "gpio_adaptor.h"
#include <poll.h>

#define MXJ_MSG(args...) printf(args)

#define INTR_MAX 32
#define ONE_SEC 1
/*

@¿¿¿¿¿¿

*/

typedef struct {
	int gpio;	/*¿¿¿gpio*/
	int fd ;
	int index; 
	int avail; /*¿¿¿¿¿¿ 0¿¿¿ 1¿¿*/
	void (*callback)(void);
}inter_t;


enum direction {
	INPUT = 0 ,
	OUTPUT
};

enum edge{
	NONE = 0,
	RISING,
	FALLING,
	BOTH
};

enum level{
	LOW = 0,
	HIGH
};

/*
@¿¿gpio¿¿¿
@pin :gpio¿¿¿¿¿
*/
int mxj_gpio_get(int pin);


/*
@¿¿gpio¿¿¿
@pin :gpio¿¿¿¿¿
*/
int mxj_gpio_release(int pin);

/*
@¿¿gpio¿¿¿¿¿¿¿¿¿¿  
@pin :gpio¿¿¿¿¿
@dir :0¿¿ 1 ¿¿
*/
int mxj_gpio_direction(int pin, int dir);

/*
@¿¿gpio¿¿¿¿
@pin :gpio¿¿¿¿¿
@value: 1¿¿¿¿¿0¿¿¿¿
*/
int mxj_gpio_write(int pin, int value);

/*
@¿¿gpio¿¿¿¿
@pin :gpio¿¿¿¿¿
@value: 1¿¿¿¿¿0¿¿¿¿
*/
int mxj_gpio_read(int pin);

/*
@¿¿gpio¿¿¿¿¿¿
@pin :gpio¿¿¿¿¿
@edge: 0¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿ 
@1¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
@2¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
@3¿¿¿¿¿¿¿¿¿¿¿¿¿¿
*/
int mxj_gpio_edge(int pin ,int edge);

/*
@¿¿gpio¿¿¿¿¿¿¿
@pin :gpio¿¿¿¿¿
@callback:¿¿¿¿¿¿¿¿
*/
int mxj_gpio_inter_set(int pin , void (*function)(void));

/*
@¿¿¿¿¿¿
*/
int mxj_gpio_inter_start(void);

int test(void);

#ifdef __cplusplus
}
#endif

#endif

 
