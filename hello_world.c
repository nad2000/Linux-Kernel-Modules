//#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h>           // Core header for loading LKMs into the kernel
//#include <linux/kernel.h>           // Contains types, macros, functions for the kernel

MODULE_LICENSE("GPL"); // need to be comlient with the distro

// need 'static' so that does expose the name and
// reduce risk of running into the name collisions.

static int x = 0;

static int __init insert_hello(void)
{
	printk(KERN_INFO "Hello World inserted\n");
	x++;
	return 0;
}

static void __exit remove_hello(void)
{
	printk(KERN_DEBUG "Hello World removed\n");
}

// macro from <linux/module.h>:
module_init(insert_hello);
module_exit(remove_hello);

