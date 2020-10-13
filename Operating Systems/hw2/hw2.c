#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>

MODULE_LICENSE("Dual BSD/GPL");


static int hello_init(void)
{
	struct timespec start_ts;
	getnstimeofday(&start_ts);

	printk(KERN_ALERT "Hello, world \n");

	//current time in hh:mm:ss
	printk("TIME: %.2lu:%.2lu:%.2lu \r\n",
	                   (start_ts.tv_sec / 3600) % (24),
	                   (start_ts.tv_sec / 60) % (60),
	                   start_ts.tv_sec % 60);
	return 0;
}

static void hello_exit(void)
{
	struct timespec end_ts;
	getnstimeofday(&end_ts);

	printk(KERN_ALERT "Goodbye, cruel world\n"); 

	//current time in hh:mm:ss
	printk("TIME: %.2lu:%.2lu:%.2lu \r\n",
	                   (end_ts.tv_sec / 3600) % (24),
	                   (end_ts.tv_sec / 60) % (60),
	                   end_ts.tv_sec % 60);

}

module_init(hello_init); 
module_exit(hello_exit);
