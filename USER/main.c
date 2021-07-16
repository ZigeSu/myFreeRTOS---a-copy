#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

//ALIENTEK 探索者STM32F407开发板 实验1
//跑马灯实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

#define START_TASK_PRIO			1		//任务优先级
#define START_STK_SIZE			128		//任务堆栈大小
TaskHandle_t StartTask_Handler;			//任务句柄
void start_task(void *pvParameters);	//任务函数

#define LED0_TASK_PRIO			2		
#define LED0_STK_SIZE			50
TaskHandle_t LED0Task_Handler;
void led0_task(void *p_arg);

#define LED1_TASK_PRIO			3
#define LED1_STK_SIZE			50
TaskHandle_t LED1Task_Handler;
void led1_task(void *p_arg);

#define FLOAT_TASK_PRIO			4
#define FLOAT_STK_SIZE			128
TaskHandle_t FLOATTask_Handler;
void float_task(void *p_arg);

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);		  //初始化延时函数
	uart_init(115200);
	LED_Init();		        //初始化LED端口
	
	// start create task
	xTaskCreate((TaskFunction_t		)start_task,			//task function
				(const char*		)"start_task",			//task name
				(uint16_t			)START_STK_SIZE,		//task stack size
				(void*				)NULL,					//parameters passd to task function
				(UBaseType_t		)START_TASK_PRIO,		//task priority
				(TaskHandle_t *		)&StartTask_Handler);	//task handler
	vTaskStartScheduler();
}

void start_task(void*pvParameters)
{
	taskENTER_CRITICAL();	//进入临界区
	//create LED0 task
	xTaskCreate((TaskFunction_t)led0_task,
				(const char*)"led0_task",
				(uint16_t)LED0_STK_SIZE,
				(void*)NULL,
				(UBaseType_t)LED0_TASK_PRIO,
				(TaskHandle_t*)&LED0Task_Handler);
	
	xTaskCreate((TaskFunction_t)led1_task,
				(const char*)"led1_task",
				(uint16_t)LED1_STK_SIZE,
				(void*)NULL,
				(UBaseType_t)LED0_TASK_PRIO,
				(TaskHandle_t*)&LED1Task_Handler);
	
	xTaskCreate((TaskFunction_t)float_task,
				(const char*)"float_task",
				(uint16_t )FLOAT_STK_SIZE,
				(void*)NULL,
				(UBaseType_t)FLOAT_TASK_PRIO,
				(TaskHandle_t*)&FLOATTask_Handler);
	
	vTaskDelete(StartTask_Handler);		//Delete start task
	taskEXIT_CRITICAL();				//退出临界区
}

//LED0 task function
void led0_task(void *p_arg)
{
	while(1)
	{
		LED0 = ~LED0;
		vTaskDelay(500);
	}
}

//LED1 task function
void led1_task(void*p_arg)
{
	while(1)
	{
		LED1 = 0;
		vTaskDelay(200);
		LED1 = 1;
		vTaskDelay(800);
	}
}

//float_test task function
void float_task(void*p_arg)
{
	static float float_num = 0.00;
	while(1)
	{
		float_num += 0.01f;
		printf("float_num = %.4f\r\n",float_num);
		vTaskDelay(1000);
	}
}
