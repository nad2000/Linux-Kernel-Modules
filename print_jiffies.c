#include <linux/module.h>	// Core header for loading LKMs into the kernel
//#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
//#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/init.h>		// Macros used to mark up functions e.g., __init __exit
#include <linux/slab.h>

static int __init _init_module(void)
{
	pr_info("Loading 'print_jiffies'\n");
	pr_info("Value of jififes is %lu\n", jiffies);
	return 0;
}

static void __exit _exit_module(void)
{
	pr_info("Unloading 'print_jiffies'\n");
	pr_info("Value of jififes is %lu\n", jiffies);
}


module_init(_init_module);
module_exit(_exit_module);

MODULE_AUTHOR("Radomirs Cirskis");
MODULE_DESCRIPTION("Print jiffies");
MODULE_LICENSE("GPL");
