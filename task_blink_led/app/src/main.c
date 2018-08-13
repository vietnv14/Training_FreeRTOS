/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Anh Vo Tuan <votuananhs@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "project.h"
#include "driver.h"
#include "clock.h"
#include "gpio.h"
#include "interrupt.h"
#include "systick.h"
#include "usart.h"
#include "cli.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

extern volatile unsigned char rx_data;

TaskHandle_t xHandle_led3 = NULL;
TaskHandle_t xHandle_led4 = NULL;

/*************************************************************************************************/
#define NUM_LIST_PINS       4
const gpio_pin_t init_list_pins[NUM_LIST_PINS] =
{
    /* led 3 - PC9 */
    {
        LD3_PORT,
        LD3_PIN,
        GPIO_MODE_OPTION_OUTPUT,
    },
    /* led 4 - PC8 */
    {
        LD4_PORT,
        LD4_PIN,
        GPIO_MODE_OPTION_OUTPUT,
    },
    /* Tx - PA9 */
    {
        PORT_A,
        9,
        GPIO_MODE_OPTION_ALT | GPIO_MODE_OPTION_AF1,
    },
    /* Rx - PA10 */
    {
        PORT_A,
        10,
        GPIO_MODE_OPTION_ALT | GPIO_MODE_OPTION_AF1,
    },
};

cli_t list_commands[] =
{
        /* control led on/off */
        {
            "led",
            led_func,
            2,
            "Control led on or off. \n\r\tSyntax: led <on/off> <led_id>",
            NULL,
        },
        /* print message in Terminal app */
        {
            "test",
            test_func,
            0,
            "Print call test_func() in terminal",
            NULL,
        },
};

/*************************************************************************************************/

void delay(unsigned int timeout)
{
    unsigned int t1, t2;
    for (t1 = 0; t1 < timeout; t1++)
    {
        for (t2 = 0; t2 < 0xFFF; t2++)
        {
          asm(" nop");
        }
    }
}

void led_on(uint8_t led_id)
{
    if (LD3 == led_id)
    {
        write_pin(LD3_PORT, LD3_PIN, HIGH);
        usart_send_string("\n\r--> Turn on LED3 successfully");
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, HIGH);
        usart_send_string("\n\r--> Turn on LED4 successfully");
    }
    else
    {
        usart_send_string("\n\r--> Don't support the led_id");
    }
}

void led_off(uint8_t led_id)
{
    if (LD3 == led_id)
    {
        write_pin(LD3_PORT, LD3_PIN, LOW);
        usart_send_string("\n\r--> Turn off LED3 successfully");
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, LOW);
        usart_send_string("\n\r--> Turn off LED4 successfully");
    }
    else
    {
        usart_send_string("\n\r--> Don't support the led_id");
    }
}

void led_func(uint8_t argc, uint8_t **argv)
{
 //   uint8_t temp = argv[1][0] - '0';

 //   if(!strcmp("on", argv[0]))
 //   {
  //      led_on(temp);
  //  }
  //  else if(!strcmp("off", argv[0]))
  //  {
  //      led_off(temp);
  //  }
  //  else
   // {
        /* nothing to be run here */
        /* printf("\n\rDon't support the input parameter !"); */
   // }
}

void test_func(uint8_t argc, uint8_t **argv)
{
    usart_send_string("\n\rCall test_func");
}


void led3_task( void * pvParameters )
{
	static uint8_t state_led3 = 0;
	while (1)
	{
		if(0 == state_led3)
		{
			led_on(LD3);
			state_led3 = 1;
		}
		else
		{
			led_off(LD3);
			state_led3 = 0;
		}
		vTaskDelay(100);
	}
}


void led4_task( void * pvParameters )
{
	static uint8_t state_led4 = 0;

	//while (1)
	//{
		if(0 == state_led4)
		{
			led_on(LD4);
			state_led4 = 1;
		}
		else
		{
			led_off(LD4);
			state_led4 = 0;
		}
		vTaskDelete(xHandle_led4);
	//}
}

void led_task(void * pvParameters)
{
	uint8_t temp = *(uint8_t *)pvParameters;
	uint32_t temp_delay;
	static uint8_t state_led = 0;

	if (LD3 == temp)
	{
		temp_delay = 100;
	}
	else
	{
		temp_delay = 150;
	}
	while(1)
	{
		if(0 == state_led)
		{
			led_on(temp);
			state_led = 1;
		}
		else
		{
			led_off(temp);
			state_led = 0;
		}
		vTaskDelay(temp_delay);
	}
}

void main(void)
{
	
	uint8_t par_led3 = LD3;
	uint8_t par_led4 = LD4;

    system_init(PREDIV_1, PLL_MUL_12, PLL_SCR_HSI, PREDIV_1, SYSCLKSOURCE_PLLCLK, APB1_HCLK_1);
    enabled_clock();
    init_pin(init_list_pins, NUM_LIST_PINS);
    usart_init();
    systick_init();
    interrupt_init();
	
	


	/* create led3 task */
	#if 1
	xTaskCreate(	led3_task,	/* pvTaskCode */
					"led 3", /* pcName */
					100, /* usStackDepth */
					NULL, /* pvParameters */
					3, /* uxPriority */
					&xHandle_led3 /* pxCreatedTask */
				);

	/* create led4 task */
	xTaskCreate(	led4_task,	/* pvTaskCode */
					"led 4", /* pcName */
					100, /* usStackDepth */
					NULL, /* pvParameters */
					2, /* uxPriority */
					&xHandle_led4 /* pxCreatedTask */
				);
	#else
	xTaskCreate(	led_task,	/* pvTaskCode */
					"led 3", /* pcName */
					100, /* usStackDepth */
					&par_led3, /* pvParameters */
					3, /* uxPriority */
					NULL /* pxCreatedTask */
				);
	xTaskCreate(	led_task,	/* pvTaskCode */
					"led 3", /* pcName */
					100, /* usStackDepth */
					&par_led4, /* pvParameters */
					3, /* uxPriority */
					NULL /* pxCreatedTask */
				);
	#endif

	vTaskStartScheduler();


    while(1)
    {
    }
}

