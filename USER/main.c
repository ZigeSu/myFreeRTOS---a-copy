#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

//ALIENTEK ̽����STM32F407������ ʵ��1
//�����ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

#define START_TASK_PRIO			1		//�������ȼ�
#define START_STK_SIZE			128		//�����ջ��С
TaskHandle_t StartTask_Handler;			//������
void start_task(void *pvParameters);	//������

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
	delay_init(168);		  //��ʼ����ʱ����
	uart_init(115200);
	LED_Init();		        //��ʼ��LED�˿�
	
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
	taskENTER_CRITICAL();	//�����ٽ���
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
	taskEXIT_CRITICAL();				//�˳��ٽ���
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
