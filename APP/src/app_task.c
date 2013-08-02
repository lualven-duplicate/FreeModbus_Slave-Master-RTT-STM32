#include "app_task.h"

#ifdef __CC_ARM
	extern int Image$$RW_IRAM1$$ZI$$Limit;
#elif __ICCARM__
#pragma section="HEAP"
#else
	extern int __bss_end;
#endif

uint8_t CpuUsageMajor , CpuUsageMinor;//CPUʹ����


//====================����ϵͳ���߳����ȼ�==================================
#define thread_SysMonitor_Prio		    	3
 
ALIGN(RT_ALIGN_SIZE)
//====================����ϵͳ���̶߳�ջ====================================
static rt_uint8_t thread_SysMonitor_stack[512];
			   
struct rt_thread thread_SysMonitor;
									 
//***************************ϵͳ����߳�***************************
//��������: void thread_entry_SysRunLed(void* parameter)
//��ڲ�������
//���ڲ�������
//��    ע��Editor��Liuqiuhu   2013-1-30    Company: BXXJS
//******************************************************************
void thread_entry_SysMonitor(void* parameter)
{
	while(1)
	{	
		cpu_usage_get(&CpuUsageMajor , &CpuUsageMinor);		
	   	LED_LED1_ON;	
		LED_LED2_ON; 
		rt_thread_delay(DELAY_SYS_RUN_LED);
		LED_LED1_OFF;
		LED_LED2_OFF;
		rt_thread_delay(DELAY_SYS_RUN_LED);
		IWDG_Feed();//ι��    
	}
}


//**********************ϵͳ��ʼ������********************************
//��������: int rt_application_init(void)
//��ڲ�������
//���ڲ�������
//��    ע��Editor��Liuqiuhu   2013-1-31   Company: BXXJS
//********************************************************************
int rt_application_init(void)
{  
	rt_thread_init(&thread_SysMonitor,
                   "SysMonitor",
                   thread_entry_SysMonitor,
                   RT_NULL,
                   thread_SysMonitor_stack,
                   sizeof(thread_SysMonitor_stack),
				   thread_SysMonitor_Prio,20);
    rt_thread_startup(&thread_SysMonitor);

  
   return 0;
}


//**************************��ʼ��RT-Thread����*************************************
//��������: void rtthread_startup(void)
//��ڲ�������
//���ڲ�������
//��    ע��Editor��Armink 2011-04-04    Company: BXXJS
//**********************************************************************************
void rtthread_startup(void)
{
	/* init board */
	rt_hw_board_init();

	/* show version */
	rt_show_version();

	/* init tick */
	rt_system_tick_init();

	/* init kernel object */
	rt_system_object_init();

	/* init timer system */
	rt_system_timer_init();

#ifdef RT_USING_HEAP
	#ifdef __CC_ARM
		rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)STM32_SRAM_END);
	#elif __ICCARM__
	    rt_system_heap_init(__segment_end("HEAP"), (void*)STM32_SRAM_END);
	#else
		/* init memory system */
		rt_system_heap_init((void*)&__bss_end, (void*)STM32_SRAM_END);
	#endif
#endif

	/* init scheduler system */
	rt_system_scheduler_init();

	/* init all device */
	rt_device_init_all();

	/* init application */
	rt_application_init();

#ifdef RT_USING_FINSH
	/* init finsh */
	finsh_system_init();
	finsh_set_device("uart1");
#endif

    /* init timer thread */
    rt_system_timer_thread_init();

	/* init idle thread */
	rt_thread_idle_init();

	/* Add CPU usage to system */
	cpu_usage_init();

	/* start scheduler */
	rt_system_scheduler_start();

	/* never reach here */
	return ;
} 
 