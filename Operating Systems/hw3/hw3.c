#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/uaccess.h>

static int open(struct inode *inode, struct file *file){
	printk("Open file\n");
	return 0;
}

static int release(struct inode *inodep, struct file *filp){
	printk("Close file\n");
	return 0;
}

static ssize_t read(struct file *file, char *buff, size_t count, loff_t *ppos){

	char hello_str[50];
	//char *hello_str = "Hello, world!";
	int len;

	//current time in hh:mm:ss
	struct timespec start_ts;
	getnstimeofday(&start_ts);

	sprintf(hello_str, "Hello world, the time now is %.2lu:%.2lu:%.2lu \r\n", (start_ts.tv_sec / 3600) % (24), (start_ts.tv_sec / 60) % (60), start_ts.tv_sec % 60);

	len = strlen(hello_str);

	if (count < len) return -EINVAL;
	if (*ppos != 0) return 0;

	copy_to_user(buff, hello_str, len);

	*ppos = len;
	return len;
}

static const struct file_operations hello_fops = {
	.owner		= THIS_MODULE,
	.open   	= open,
	.release 	= release,
	.read 		= read,
};

static struct miscdevice hello_dev = {
	.minor 		= MISC_DYNAMIC_MINOR,
	.name 		= "hello",
	.fops 		= &hello_fops,
};

static int __init hello_init(void){
	int retval;

	retval = misc_register(&hello_dev);
	return retval;

}

static void __exit hello_exit(void){
	misc_deregister(&hello_dev);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("dev");
