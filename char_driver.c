#include <linux/module.h>           // Core header for loading LKMs into the kernel
//#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit
#include <linux/cdev.h>
#include <linux/slab.h>


// need 'static' so that does expose the name and
// reduce risk of running into the name collisions.
static char buffer[PAGE_SIZE];
struct cdev *my_cdev;
dev_t my_dev;
char devname[] = "nad2000";

static int device_open(struct inode *inode,
		       struct file *file)
{
	/* This can be used to synchronization purpose
	 * to say if the device is already in use
	 */
	return 0;
}

static ssize_t device_write(
	struct file *file,
	const char *buf,
	size_t lbuf,
	loff_t *ppos)
{
	int nbytes = lbuf - copy_from_user(buffer + *ppos, buf, lbuf);
	*ppos += nbytes;
	pr_info("Rec'vd data from app %s, nbytes=%d\n", buffer, nbytes);
	return nbytes;
}


static ssize_t device_read(
	struct file *file,
	char *buf,
	size_t lbuf,
	loff_t *ppos)
{
	int nbytes, maxbytes, bytes_to_do;
	
	maxbytes = PAGE_SIZE - *ppos;
	if (maxbytes > lbuf)
		bytes_to_do = lbuf;
	else
		bytes_to_do = maxbytes;

	nbytes = bytes_to_do - copy_to_user(buf, buffer + *ppos, bytes_to_do);
	*ppos += nbytes;
	pr_info("Data sent to app %s, nbytes=%d\n", buffer, nbytes);
	return nbytes;
}

static struct file_operations device_fops = {
	.owner = THIS_MODULE,
	.write = device_write,
	.open = device_open,
	.read = device_read,
};

static int __init c_driver_init(void)
{
	int status;
	/* // Check /proc/deevices for available major number */
	/* my_dev = MKDEV(100, 0); // Major and Minor Number */
	/* // to create a device file use: */
	/* // mknod /dev/DEV_NAME c 100 0 */
	/* register_chrdev_region(my_dev, 1, devname); */

	// alternatively use alloc_chrdev_region dynamically:
	status = alloc_chrdev_region(&my_dev, 0, 1, devname);

	my_cdev = cdev_alloc();
	cdev_init(my_cdev, &device_fops);
	status = cdev_add(my_cdev, my_dev, 1);
	if (status < 0) {
		pr_info("Driver init failed\n");
		return status;
	}

	printk(KERN_INFO "Driver for %s loaded.\n", devname);
	pr_info("Major number: %d, minor number: %d\n", MAJOR(my_dev), MINOR(my_dev));
	memset(buffer, '\0', PAGE_SIZE);
	return 0;
}

static void __exit c_driver_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(my_dev, 1);
	printk(KERN_DEBUG "Driver for %s unloaded.\n", devname);
}
	

module_init(c_driver_init);
module_exit(c_driver_exit);


MODULE_AUTHOR("Radomirs Cirskis");
MODULE_DESCRIPTION("Device to demostrate read/write calls of char driver");
MODULE_LICENSE("GPL"); // need to be comlient with the distro
