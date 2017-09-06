/*
 * FILE NAME rtl_gpio.c
 *
 * BRIEF MODULE DESCRIPTION
 *  GPIO For Flash Reload Default
 *
 *  Author: jimmylin@realtek.com.tw
 *
 *  Copyright (c) 2011 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */


#include <generated/autoconf.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/reboot.h>
#include <linux/kmod.h>
#include <linux/proc_fs.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/seq_file.h>
#include <linux/platform_device.h>
#include <linux/platform_data/rtl819x-gpio.h>
#include <bspchip.h>

//#define CONFIG_USING_JTAG 1
#define AUTO_CONFIG

/*
enabled immediate mode pbc ; must enabled "USE_INTERRUPT_GPIO" and "IMMEDIATE_PBC"
gpio will rx pbc event and trigger wscd by signal method
note:also need enabled IMMEDIATE_PBC at wsc.h (sdk/users/wsc/src/)
*/
//#define USE_INTERRUPT_GPIO	//undefine USE_INTERRUPT_GPIO
//#define IMMEDIATE_PBC 

#ifdef IMMEDIATE_PBC
int	wscd_pid = 0;
struct pid *wscd_pid_Ptr=NULL;
#endif

///////////////////////////////////////////////////////
#define PROBE_TIME	5
#define PROBE_NULL		0
#define PROBE_ACTIVE	1
#define PROBE_RESET		2
#define PROBE_RELOAD	3
#define RTL_R32(addr)		(*(volatile unsigned long *)(addr))
#define RTL_W32(addr, l)	((*(volatile unsigned long*)(addr)) = (l))
#define RTL_R8(addr)		(*(volatile unsigned char*)(addr))
#define RTL_W8(addr, l)		((*(volatile unsigned char*)(addr)) = (l))

//#define  GPIO_DEBUG
#ifdef GPIO_DEBUG
/* note: prints function name for you */
#  define DPRINTK(fmt, args...) printk("%s: " fmt, __FUNCTION__ , ## args)
#else
#  define DPRINTK(fmt, args...)
#endif

#ifdef CONFIG_EFM_PATCH
#undef USE_IRQ_FOR_WPS

int run_period = HZ;
#define RUN_LED_BLINK 0x1
#define RUN_LED_ON 0x2
#define RUN_LED_OFF 0x3
int run_blink = RUN_LED_OFF;
int reset_op_start;

#endif

#ifdef CONFIG_P3_GPIO
void RTLWIFINIC_GPIO_config(unsigned int, unsigned int);
void RTLWIFINIC_GPIO_write(unsigned int, unsigned int);
int RTLWIFINIC_GPIO_read(unsigned int);
#endif

static struct timer_list probe_timer;
#ifdef CONFIG_EFM_PATCH
static struct timer_list wps_timer;
#endif
static unsigned int    probe_counter;
static unsigned int    probe_state;

static char default_flag='0';
//Brad add for update flash check 20080711
int start_count_time=0;
int Reboot_Wait=0;

static unsigned int wps_btn_gnum, wps_led_gnum, reset_btn_gnum, reset_led_gnum, rtl819x_gpio_actlow = 0;
#ifdef CONFIG_EFM_PATCH
static unsigned int led_2g_gnum, led_5g_gnum = 0;
#endif

//static int get_dc_pwr_plugged_state(void);

#if defined(USE_INTERRUPT_GPIO)
struct gpio_wps_device
{
	unsigned int name;
};
struct gpio_wps_device priv_gpio_wps_device;
#endif

#ifdef USE_INTERRUPT_GPIO
static int wps_button_push = 0;
#endif

int reset_button_pressed(void)
{	
	return gpio_get_value(reset_btn_gnum) ^ (rtl819x_gpio_actlow ? 1 : 0 );
}

void reset_led_on(void)
{
#ifdef CONFIG_P3_GPIO
	DPRINTK("line: %d\n", __LINE__);
	RTLWIFINIC_GPIO_write(reset_led_gnum, (rtl819x_gpio_actlow ? 0 : 1));
#else
	gpio_set_value(reset_led_gnum, (rtl819x_gpio_actlow ? 0 : 1));
#endif
}

void reset_led_off(void)
{
#ifdef CONFIG_P3_GPIO
	DPRINTK("line: %d\n", __LINE__);
	RTLWIFINIC_GPIO_write(reset_led_gnum, (rtl819x_gpio_actlow ? 1 : 0));
#else
	gpio_set_value(reset_led_gnum, (rtl819x_gpio_actlow ? 1 : 0));
#endif
}

#ifdef AUTO_CONFIG
static unsigned int		AutoCfg_LED_Blink;
static unsigned int		AutoCfg_LED_Toggle;
static unsigned int		AutoCfg_LED_Slow_Blink;
static unsigned int		AutoCfg_LED_Slow_Toggle;

void autoconfig_gpio_off(void)
{
	gpio_set_value(wps_led_gnum, (rtl819x_gpio_actlow ? 1 : 0));
	AutoCfg_LED_Blink = 0;
}

void autoconfig_gpio_on(void)
{
	gpio_set_value(wps_led_gnum, (rtl819x_gpio_actlow ? 0 : 1));
	AutoCfg_LED_Blink = 0;
}

void autoconfig_gpio_blink(void)
{
	gpio_set_value(wps_led_gnum, (rtl819x_gpio_actlow ? 0 : 1));
	AutoCfg_LED_Blink = 1;
	AutoCfg_LED_Toggle = 1;
    /*check if 98C support slow blink?*/
	////AutoCfg_LED_Slow_Blink = 0;
}

int autoconfig_button_pressed(void)
{
	return gpio_get_value(wps_btn_gnum) ^ (rtl819x_gpio_actlow ? 1 : 0);
}

void autoconfig_led_on(void)
{
	gpio_set_value(wps_led_gnum, (rtl819x_gpio_actlow ? 0 : 1));
}

void autoconfig_led_off(void)
{
	gpio_set_value(wps_led_gnum, (rtl819x_gpio_actlow ? 1 : 0));
}
#endif // end of AUTO_CONFIG


extern struct proc_dir_entry proc_root;

#ifdef CONFIG_EFM_PATCH
void led_2g_on(void)
{
        gpio_set_value(led_2g_gnum, (rtl819x_gpio_actlow ? 0 : 1));
}

void led_2g_off(void)
{
        gpio_set_value(led_2g_gnum, (rtl819x_gpio_actlow ? 1 : 0));
}

void led_5g_on(void)
{
        gpio_set_value(led_5g_gnum, (rtl819x_gpio_actlow ? 0 : 1));
}

void led_5g_off(void)
{
        gpio_set_value(led_5g_gnum, (rtl819x_gpio_actlow ? 1 : 0));
}

static int check_reset_bt(void)
{
        return reset_button_pressed();
}

static int check_wps_bt(void)
{
        return autoconfig_button_pressed();
}

int gpio_shadow;
int DIL_run_led_on(void)
{
        gpio_set_value(reset_led_gnum, (rtl819x_gpio_actlow ? 0 : 1));
        return 0;
}

int DIL_run_led_off(void)
{
        gpio_set_value(reset_led_gnum, (rtl819x_gpio_actlow ? 1 : 0));
        return 0;
}

int DIL_run_led_toggle(void)
{
        static int led_stat = 0;

        if(led_stat)    DIL_run_led_off();
        else            DIL_run_led_on();

        led_stat = (led_stat)?0:1;

        return 0;
}

static int send_sig_pid( int sig, int pid)
{
        struct task_struct *p = NULL;

        p = find_task_by_vpid(1);
        return send_sig(sig,p,0);
}

int pushed_count, reset_start;
int use_wps_only_button;

static void rtl_wps_timer(unsigned long data)
{
        if(use_wps_only_button && check_wps_bt())
        {
                printk("Send WPS signal..\n");
                run_blink=RUN_LED_ON;
                send_sig_pid(SIGPWR, 1); /* send WPS signal */
        }
        mod_timer(&wps_timer, jiffies + HZ/10);
}

#ifdef CONFIG_RTL_KERNEL_MIPS16_CHAR
__NOMIPS16
#endif

static int write_run_led( struct file *file, const char *buffer, u_long count, void *data )
{
        if(!strncmp(buffer,"off",3))
        {
                DIL_run_led_off();
                run_blink = RUN_LED_OFF;
        }
        else if(!strncmp(buffer,"on",2))
        {
                run_blink = RUN_LED_ON;
        }
        else if(!strncmp(buffer,"blink",5))
        {
                run_blink = RUN_LED_BLINK;
        }

        if(check_reset_bt())
        {
                printk("Pushed..");
        }

        return count;
}

static ssize_t proc_write_run_led(struct file * file, const char __user * userbuf,
                     size_t count, loff_t * off)
{
        return write_run_led(file, userbuf,count, off);
}

#ifdef CONFIG_RTL_KERNEL_MIPS16_CHAR
__NOMIPS16
#endif

static int proc_read_run_period(struct seq_file *s, void *v)
{
        seq_printf(s,"%d", run_period);

        return 0;
}

int run_led_open(struct inode *inode, struct file *file)
{
        return(single_open(file, proc_read_run_period, NULL));
}

#ifdef CONFIG_RTL_KERNEL_MIPS16_CHAR
__NOMIPS16
#endif

static int write_wps_bt( struct file *file, const char *buffer, u_long count, void *data )
{
        return 0;
}

static ssize_t proc_write_wps_bt(struct file * file, const char __user * userbuf,
                     size_t count, loff_t * off)
{
        return write_run_led(file, userbuf,count, off);
}

static int proc_read_wps_bt(struct seq_file *s, void *v)
{
        if(!check_wps_bt())
                seq_printf(s,"1");
        else
                seq_printf(s,"0");

        return 0;
}

int wps_bt_open(struct inode *inode, struct file *file)
{
        return(single_open(file, proc_read_wps_bt, NULL));
}

static int proc_read_reset_bt(struct seq_file *s, void *v)
{
        if(!check_reset_bt())
                seq_printf(s,"1");
        else
                seq_printf(s,"0");

        return 0;
}

int reset_bt_open(struct inode *inode, struct file *file)
{
        return(single_open(file, proc_read_reset_bt, NULL));
}

static int write_reset_bt( struct file *file, const char *buffer, u_long count, void *data )
{
        if(!strncmp(buffer,"start",5))
        {
                reset_op_start=1;
                printk("Reset Button Start\n");
        }
#ifdef CONFIG_RTL8192CD
        else if(!strncmp(buffer,"wps_function_off",5))
                use_wps_only_button = 1;
#endif

        return count;
}

static ssize_t proc_write_reset_bt(struct file * file, const char __user * userbuf,
                     size_t count, loff_t * off)
{
        return write_reset_bt(file, userbuf,count, off);
}

static int proc_read_led_2g(struct seq_file *s, void *v)
{
        return 0;
}

int led_2g_open(struct inode *inode, struct file *file)
{
        return(single_open(file, proc_read_led_2g, NULL));
}

static int write_led_2g( struct file *file, const char *buffer, u_long count, void *data )
{
        if(!strncmp(buffer,"on",2))
        {
                led_2g_on();
        }
        else if(!strncmp(buffer,"off",3))
        {
                led_2g_off();
        }

        return count;
}

static ssize_t proc_write_led_2g(struct file * file, const char __user * userbuf,
                     size_t count, loff_t * off)
{
        return write_led_2g(file, userbuf,count, off);
}

static int proc_read_led_5g(struct seq_file *s, void *v)
{
        return 0;
}

int led_5g_open(struct inode *inode, struct file *file)
{
        return(single_open(file, proc_read_led_5g, NULL));
}

static int write_led_5g( struct file *file, const char *buffer, u_long count, void *data )
{
        if(!strncmp(buffer,"on",2))
        {
                led_5g_on();
        }
        else if(!strncmp(buffer,"off",3))
        {
                led_5g_off();
        }

        return count;
}

static ssize_t proc_write_led_5g(struct file * file, const char __user * userbuf,
                     size_t count, loff_t * off)
{
        return write_led_5g(file, userbuf,count, off);
}

struct file_operations resetbt_proc_fops = {
        .open            = reset_bt_open,
        .write           = proc_write_reset_bt,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release         = single_release,
};

struct file_operations wpsbt_proc_fops = {
        .open            = wps_bt_open,
        .write           = proc_write_wps_bt,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release         = single_release,
};

struct file_operations runled_proc_fops = {
        .open            = run_led_open,
        .write           = proc_write_run_led,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release         = single_release,
};

struct file_operations led2g_proc_fops = {
        .open            = led_2g_open,
        .write           = proc_write_led_2g,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release         = single_release,
};

struct file_operations led5g_proc_fops = {
        .open            = led_5g_open,
        .write           = proc_write_led_5g,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release         = single_release,
};

static  int init_iptime_led_spec(void)
{
        char name[128];

        printk("Init IPTIME GPIO: \n");

        sprintf(name, "driver/reset_bt");
        proc_create_data(name, 0, &proc_root,
                        &resetbt_proc_fops, NULL);

        sprintf(name, "driver/wps_bt");
        proc_create_data(name, 0, &proc_root,
                        &wpsbt_proc_fops, NULL);

        sprintf(name, "driver/run_led");
        proc_create_data(name, 0, &proc_root,
                        &runled_proc_fops, NULL);

        sprintf(name, "driver/led2g");
        proc_create_data(name, 0, &proc_root,
                        &led2g_proc_fops, NULL);

        sprintf(name, "driver/led5g");
        proc_create_data(name, 0, &proc_root,
                        &led5g_proc_fops, NULL);

        return 0;
}

#endif  /* CONFIG_EFM_PATCH */


static void rtl_gpio_timer(unsigned long data)
{
#ifdef CONFIG_EFM_PATCH
        if(run_blink == RUN_LED_BLINK)
                DIL_run_led_toggle();
        else if(run_blink == RUN_LED_OFF)
                DIL_run_led_off();
        else if(run_blink == RUN_LED_ON)
                DIL_run_led_on();
#else
	unsigned int pressed=1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
	struct pid *pid;
#endif
#endif

#ifdef CONFIG_EFM_PATCH
        if(!reset_op_start)
        {
                mod_timer(&probe_timer, jiffies + run_period);
                return;
        }
        if(check_reset_bt())
        {
                pushed_count++;

                /* Turn OFF Wireless LED */
                if(pushed_count == 7)
                {
                        pushed_count = 0;
                        reset_start = 1;
                        run_blink = RUN_LED_BLINK;
                        run_period = HZ/10;
                }
        }
        else
        {
                if(reset_start)
                {
                        printk("Send Reset signal..\n");
                        send_sig_pid( SIGPOLL , 1 );
                }
                pushed_count = 0;
        }
#else   /* CONFIG_EFM_PATCH */
	if(reset_button_pressed() == 0) //mark_es
	{
		pressed = 0;
		//turn off LED0
		reset_led_off();
	}
	else
	{
		DPRINTK("Key pressed %d!\n", probe_counter+1);
	}


	if (!autoconfig_button_pressed())
	{
#ifdef USE_INTERRUPT_GPIO
		wps_button_push = 0;
#endif
	}else{
#ifdef USE_INTERRUPT_GPIO
		wps_button_push++;
#endif
	}

	if (probe_state == PROBE_NULL)
	{
		if (pressed)
		{
			probe_state = PROBE_ACTIVE;
			probe_counter++;
		}
		else
			probe_counter = 0;
	}
	else if (probe_state == PROBE_ACTIVE)
	{
		if (pressed)
		{
			probe_counter++;
			if ((probe_counter >=2 ) && (probe_counter <=PROBE_TIME))
			{
				DPRINTK("2-5 turn on led\n");
				//turn on LED0
				reset_led_on();
			}
			else if (probe_counter >= PROBE_TIME)
			{
				// sparkling LED0
				DPRINTK(">5 \n");
				if (probe_counter & 1)
				{ 				
					reset_led_off();
				}	
				else
				{
					reset_led_on();
				}	
			}
		}
		else
		{
			if (probe_counter < 2)
			{
				probe_state = PROBE_NULL;
				probe_counter = 0;
				DPRINTK("<2 \n");
				#if defined(CONFIG_RTL865X_SC)
					ResetToAutoCfgBtn = 1;
				#endif
			}
			else if (probe_counter >= PROBE_TIME)
			{
				//reload default
				default_flag = '1';
				//kernel_thread(reset_flash_default, (void *)1, SIGCHLD);
				return;
			}
			else
			{
				DPRINTK("2-5 reset 1\n");
			#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
				kill_proc(1,SIGTERM,1);
			#else
				pid = get_pid(find_vpid(1));
				kill_pid(pid,SIGTERM,1);
			#endif
				DPRINTK("2-5 reset 2\n");
				//kernel_thread(reset_flash_default, 0, SIGCHLD);
				return;
			}
		}
	}

#ifdef AUTO_CONFIG
	if (AutoCfg_LED_Blink==1)
	{
		if (AutoCfg_LED_Toggle) {
			autoconfig_led_off();
		}
		else {
			autoconfig_led_on();
		}
				
		if(AutoCfg_LED_Slow_Blink)
		{
			if(AutoCfg_LED_Slow_Toggle)
				AutoCfg_LED_Toggle = AutoCfg_LED_Toggle;
			else
				AutoCfg_LED_Toggle = AutoCfg_LED_Toggle? 0 : 1;
			
			AutoCfg_LED_Slow_Toggle = AutoCfg_LED_Slow_Toggle? 0 : 1;
		}
		else
			AutoCfg_LED_Toggle = AutoCfg_LED_Toggle? 0 : 1;
		
	}
#endif	/* AUTO_CONFIG */
#endif	/* CONFIG_EFM_PATCH */

#ifdef CONFIG_EFM_PATCH
        mod_timer(&probe_timer, jiffies + run_period);
#else
	mod_timer(&probe_timer, jiffies + HZ);
#endif
}



#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE

#define SYSTEM_CONTRL_DUMMY_REG 0xb8003504

int is_bank2_root()
{
	//boot code will steal System's dummy register bit0 (set to 1 ---> bank2 booting
	//for 8198 formal chip 
	
	if ((RTL_R32(SYSTEM_CONTRL_DUMMY_REG)) & (0x00000001))  // steal for boot bank idenfy
		return 1;
	
	return 0;
}
static int read_bootbank_proc(struct seq_file *s, void *v)
{
	int len;
	char flag='1';

	if (is_bank2_root())  // steal for boot bank idenfy
		flag='2';


	seq_printf(s,"%c\n", flag);

	return 0;
#if 0		
	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
#endif
}

int bootbank_open(struct inode *inode, struct file *file)
{
        return(single_open(file, read_bootbank_proc, NULL));
}

struct file_operations bootbank_proc_fops = {
        .open           = bootbank_open,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release        = single_release,
};

#endif


#ifdef AUTO_CONFIG
#if defined(USE_INTERRUPT_GPIO)
static irqreturn_t gpio_interrupt_isr(int irq, void *dev_instance)
{
	DPRINTK("gpio irq %d interrupt triggered\n", irq);
	wps_button_push = 1; 		

#ifdef IMMEDIATE_PBC
	if(wscd_pid>0)
	{
		rcu_read_lock();
		wscd_pid_Ptr = get_pid(find_vpid(wscd_pid));
		rcu_read_unlock();	

		if(wscd_pid_Ptr){
			printk("(%s %d);signal wscd ;pid=%d\n",__FUNCTION__ , __LINE__,wscd_pid);			
			kill_pid(wscd_pid_Ptr, SIGUSR2, 1);
			
		}
	}
#endif

	return IRQ_HANDLED;
}
#endif /* USE_INTERRUPT_GPIO */

//static int read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
static int read_proc(struct seq_file *s, void *v)
{
//	int len;
	char flag;

#if  defined(USE_INTERRUPT_GPIO)
// 2009-0414		
	if (wps_button_push) {
		flag = '1';
		//wps_button_push = 0; //mark it for select wlan interface by button pressed time		
	}
	else{
		if (!autoconfig_button_pressed()){
			flag = '0';
		}else{
			//printk("wps button be held \n");
			flag = '1';
		}
	}
// 2009-0414		
#else

	if (!autoconfig_button_pressed())
		flag = '0';
	else 
		flag = '1';
	
#endif // CONFIG_RTL865X_KLD					
//	len = sprintf(page, "%c\n", flag);
	seq_printf(s, "%c\n", flag);

#if 0
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
#else
	return 0;
#endif	
}

#ifdef CONFIG_RTL_KERNEL_MIPS16_CHAR
__NOMIPS16
#endif 
static int write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char flag[20];
//Brad add for update flash check 20080711

	char start_count[10], wait[10];

	if (count < 2)
		return -EFAULT;

	DPRINTK("file: %08x, buffer: %s, count: %lu, data: %08x\n",
		(unsigned int)file, buffer, count, (unsigned int)data);

	if (buffer && !copy_from_user(&flag, buffer, 1)) {
		if (flag[0] == 'E') {
			/* Fix me, if autoconfig_gpio_init() needed again */
		}
		else if (flag[0] == '0')
			autoconfig_gpio_off();
		else if (flag[0] == '1')
			autoconfig_gpio_on();
		else if (flag[0] == '2')
			autoconfig_gpio_blink();
//Brad add for update flash check 20080711
		else if (flag[0] == '4'){
			start_count_time= 1;
			sscanf(buffer, "%s %s", start_count, wait);
			Reboot_Wait = (simple_strtol(wait,NULL,0))*100;
		}

		else
			{}

		return count;
	}
	else
		return -EFAULT;
}
#ifdef IMMEDIATE_PBC
static unsigned long atoi_dec(char *s)
{
	unsigned long k = 0;

	k = 0;
	while (*s != '\0' && *s >= '0' && *s <= '9') {
		k = 10 * k + (*s - '0');
		s++;
	}
	return k;
}
static int read_gpio_wscd_pid(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag;

	DPRINTK("wscd_pid=%d\n",wscd_pid);	
	
	len = sprintf(page, "%d\n", wscd_pid);
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
}
static int write_gpio_wscd_pid(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	char flag[20];
	char start_count[10], wait[10];
	if (count < 2)
		return -EFAULT;

	DPRINTK("file: %08x, buffer: %s, count: %lu, data: %08x\n",
		(unsigned int)file, buffer, count, (unsigned int)data);

	if (buffer && !copy_from_user(&flag, buffer, 1)) {

		wscd_pid = atoi_dec(buffer);
		DPRINTK("wscd_pid=%d\n",wscd_pid);	
		return count;
	}
	else{
		return -EFAULT;
	}
}
#endif
#endif // AUTO_CONFIG

//static int default_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
static int default_read_proc(struct seq_file *s, void *v)
{
#if 0
	int len;

	len = sprintf(page, "%c\n", default_flag);
	if (len <= off+count) *eof = 1;
	  *start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	  return len;
#else
	seq_printf(s, "%c\n", default_flag);
	return 0;
#endif
}

#ifdef CONFIG_RTL_KERNEL_MIPS16_CHAR
__NOMIPS16
#endif 
static int default_write_proc(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;
	if (buffer && !copy_from_user(&default_flag, buffer, 1)) {
		return count;
	}
	return -EFAULT;
}

#ifdef CONFIG_PROC_FS


int gpio_single_open(struct inode *inode, struct file *file)
{
        return(single_open(file, read_proc, NULL));
}

static ssize_t gpio_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return write_proc(file, userbuf,count, off);
}


struct file_operations gpio_proc_fops = {
        .open           = gpio_single_open,
	 .write		= gpio_single_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};

int load_default_single_open(struct inode *inode, struct file *file)
{
        return(single_open(file, default_read_proc, NULL));
}

static ssize_t load_default_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return default_write_proc(file, userbuf,count, off);
}

struct file_operations load_default_proc_fops = {
        .open           = load_default_single_open,
	 .write           = load_default_single_write,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release        = single_release,
};





#endif

#if 0 // example
struct proc_dir_entry *hostap_proc;

static int __init hostap_init(void)
{
	if (init_net.proc_net != NULL) {
		hostap_proc = proc_mkdir("rtl865x", &proc_root);
		if (!hostap_proc)
			printk(KERN_WARNING "Failed to mkdir "
			       "/proc/rtl865x\n");
	} else
		hostap_proc = NULL;

	return 0;
}

static void __exit hostap_exit(void)
{
	if (hostap_proc != NULL) {
		hostap_proc = NULL;
		remove_proc_entry("rtl865x", &proc_root);
	}
}
#endif

int __init rtl_gpio_init(void)
{
#if defined(USE_INTERRUPT_GPIO)
	int irq, err;
#endif

#if defined(IMMEDIATE_PBC)
	struct proc_dir_entry *res=NULL;
#endif

	printk("Realtek GPIO Driver for Flash Reload Default\n");

	#if defined(USE_INTERRUPT_GPIO)
	if((irq = gpio_to_irq(wps_btn_gnum)) < 0)
		printk(KERN_ERR "error: gpio to irq%d failed!\n", irq);
       
	if((err = request_irq(irq, gpio_interrupt_isr, IRQF_SHARED | IRQF_TRIGGER_FALLING
			, "wps btn", (void *)&priv_gpio_wps_device)))
		printk(KERN_ERR "error: gpio request_irq(%d) error(%d)!\n", irq, err);

    #endif
    #ifdef AUTO_CONFIG
#if 0	
	res = create_proc_entry("gpio", 0, NULL);
	if (res) {
		res->read_proc = read_proc;
		res->write_proc = write_proc;
	}
	else {
		printk("Realtek GPIO Driver, create proc failed!\n");
	}
#else
	proc_create_data("gpio", 0, &proc_root,
			 &gpio_proc_fops, NULL);
#endif

    #ifdef	USE_INTERRUPT_GPIO
    #ifdef  IMMEDIATE_PBC
	res = create_proc_entry("gpio_wscd_pid", 0, NULL);
	if (res)
	{
		res->read_proc = read_gpio_wscd_pid;
		res->write_proc = write_gpio_wscd_pid;
		DPRINTK("create gpio_wscd_pid OK!!!\n\n");
	}
	else{
		printk("create gpio_wscd_pid failed!\n\n");
	}
    #endif	
    #endif
#endif /* AUTO_CONFIG */

#if 0
	res = create_proc_entry("load_default", 0, NULL);
	if (res) {
		res->read_proc = default_read_proc;
		res->write_proc = default_write_proc;
	}
#else	
	proc_create_data("load_default", 0, &proc_root,
			 &load_default_proc_fops, NULL);
#endif

#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE	
	proc_create_data("bootbank", 0, &proc_root,
			&bootbank_proc_fops, NULL);
#endif

#ifdef CONFIG_EFM_PATCH
        init_iptime_led_spec();
#endif

	init_timer(&probe_timer);
	probe_counter = 0;
	probe_state = PROBE_NULL;
	probe_timer.expires = jiffies + HZ;
	probe_timer.data = (unsigned long)NULL;
	probe_timer.function = &rtl_gpio_timer;
	mod_timer(&probe_timer, jiffies + HZ);

#ifdef CONFIG_EFM_PATCH
        init_timer(&wps_timer);
        wps_timer.expires = jiffies + HZ;
        wps_timer.data = (unsigned long)NULL;
        wps_timer.function = &rtl_wps_timer;
        mod_timer(&wps_timer, jiffies + HZ);
#endif

	return 0;
}


static void __exit rtl_gpio_exit(void)
{
	printk("Unload Realtek GPIO Driver \n");
	del_timer_sync(&probe_timer);
}

module_exit(rtl_gpio_exit);
module_init(rtl_gpio_init);

static int rtl819x_led_probe(struct platform_device *dev) {
	struct rtl819x_gpio_platdata *pdata = dev->dev.platform_data;
	int ret = 0;
	
	if(!strcmp(pdata->name, "reset led"))
		reset_led_gnum = pdata->gpio;
	else if(!strcmp(pdata->name, "wps led"))
		wps_led_gnum = pdata->gpio;
#ifdef CONFIG_EFM_PATCH
        else if(!strcmp(pdata->name, "led2g")){
                led_2g_gnum = pdata->gpio;
        }
        else if(!strcmp(pdata->name, "led5g")){
                led_5g_gnum = pdata->gpio;
        }
#endif
	
	if(reset_led_gnum != wps_led_gnum) {
#ifdef CONFIG_P3_GPIO
		/* We move configuration steps to Wifi GPIO read/write function
		 * Wifi GPIO config only applys after Wifi chip is ready 
		 */
		//RTLWIFINIC_GPIO_config(pdata->gpio, GPIOF_OUT_INIT_HIGH);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
		ret = devm_gpio_request_one(&dev->dev, pdata->gpio, GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED, pdata->name);
#else
		if((ret = gpio_request_one(pdata->gpio, GPIOF_OUT_INIT_HIGH, pdata->name)) == 0)
			ret = gpio_export(pdata->gpio, false);
#endif
#endif
	}
#ifdef CONFIG_EFM_PATCH
        else{
                if(led_2g_gnum != led_5g_gnum) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
                        ret = devm_gpio_request_one(&dev->dev, pdata->gpio, GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED, pdata->name);
#else
                        if((ret = gpio_request_one(pdata->gpio, GPIOF_OUT_INIT_HIGH, pdata->name)) == 0)
                                ret = gpio_export(pdata->gpio, false);
#endif
                }
        }
#endif
	
	if(ret < 0)
		return ret;
	
	if(pdata->flags & RTL819X_GPIO_ACTLOW) {
		gpio_sysfs_set_active_low(pdata->gpio, 1);
		rtl819x_gpio_actlow = RTL819X_GPIO_ACTLOW;
	}
	
	return ret;
}

static struct platform_driver rtl819x_led_driver = {
	.probe		= rtl819x_led_probe,
//	.remove		= rtl819x_led_remove,
	.driver		= {
		.name	= "rtl819x_led",
		.owner	= THIS_MODULE,
	}, 
};
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
module_platform_driver(rtl819x_led_driver);
#else
static int __init rtl819x_led_driver_init(void)
{
	return platform_driver_register(&rtl819x_led_driver);
}
module_init(rtl819x_led_driver_init);
#endif

static int rtl819x_btn_probe(struct platform_device *dev) {
	struct rtl819x_gpio_platdata *pdata = dev->dev.platform_data;
	int ret = 0;
	
	if(!strcmp(pdata->name, "reset btn"))
		reset_btn_gnum = pdata->gpio;
	else if(!strcmp(pdata->name, "wps btn"))
		wps_btn_gnum = pdata->gpio;
	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	ret = devm_gpio_request_one(&dev->dev, pdata->gpio, GPIOF_IN | GPIOF_EXPORT_DIR_FIXED, pdata->name);
#else
	if((ret = gpio_request_one(pdata->gpio, GPIOF_IN, pdata->name)) == 0)
		ret = gpio_export(pdata->gpio, false);
#endif
	
	if(ret < 0)
		return ret;
	
	if(pdata->flags & RTL819X_GPIO_ACTLOW)
		gpio_sysfs_set_active_low(pdata->gpio, 1);
	
	return ret;
}

static struct platform_driver rtl819x_btn_driver = {
	.probe		= rtl819x_btn_probe,
//	.remove		= rtl819x_btn_remove,
	.driver		= {
		.name	= "rtl819x_btn",
		.owner	= THIS_MODULE,
	}, 
};
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
module_platform_driver(rtl819x_btn_driver);
#else
static int __init rtl819x_btn_driver_init(void)
{
	return platform_driver_register(&rtl819x_btn_driver);
}
module_init(rtl819x_btn_driver_init);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO driver for Reload default");

