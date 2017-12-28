/*
 * make sure debugfs is included in the kerel and mounted:
 * mount -d debugfs nono /sys/kernel/debug
 */


#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit

#include <linux/debugfs.h>
#include <asm/uaccess.h>

#define FSNAME "nad2000"
#define FILENAME "demo_file"

struct semaphore my_sem;
char kbuffer[] = "This is kernel data which can be modified";
struct dentry *my_dirent;


static ssize_t debugfs_read(
	struct file *file,
	char *buf,
	size_t lbuf,
	loff_t *ppos)
{
	int stat = -1;

	if (down_interruptible(&my_sem) == 0)
	{
		stat = simple_read_from_buffer
			(buf, strlen(kbuffer), ppos, kbuffer, strlen(kbuffer));
		up(&my_sem);
	}
	
	return stat;
}
static ssize_t debugfs_write(struct file *file, const char *buf, size_t lbuf,
			     loff_t *ppos)
{
	int stat = -1;
	if (lbuf > strlen(kbuffer))
	{
		pr_info("Write: returning EINVAL\n");
		return -EINVAL;
	}

	if (down_interruptible(&my_sem) == 0)
	{
		stat = simple_write_to_buffer(
			kbuffer, strlen(buf), ppos, buf, lbuf);
		kbuffer[lbuf] = '\0';
		up(&my_sem);
	}
	return stat;
}

const struct file_operations fops_debugfs = {
	.read = debugfs_read,
	.write = debugfs_write,
};

static int __init init_debugfs(void)
{
	sema_init(&my_sem, 1);

	my_dirent = debugfs_create_dir(FSNAME, NULL);
	debugfs_create_file(FILENAME, 0666, my_dirent, (void *)kbuffer,
			    &fops_debugfs);
	pr_info("File '%s' should be created in '%s'.\n", FILENAME, FSNAME);
	return 0;
}

static void __exit exit_debugfs(void)
{
	pr_info("kernel buffer while exiting the module is <%s>\n", kbuffer);
	debugfs_remove_recursive(my_dirent);
}

module_init(init_debugfs);
module_exit(exit_debugfs);


MODULE_AUTHOR("Radomirs Cirskis");
MODULE_DESCRIPTION("Demo for Debugfs");
MODULE_LICENSE("GPL"); // need to be comlient with the distro
