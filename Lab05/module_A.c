#include <linux/module.h>

int symbol_function(int x) {
        if(x > 100) {
		printk("symbol_function(%d) = 1\n", x);
                return 1;
        }
        else if(x == 100) {
		printk("symbol_function(%d) = 0\n", x);
                return 0;
        }
        else if(x < 100) {
		printk("symbol_function(%d) = -1\n", x);
                return -1;
        }
}

int init_module(void) {
        printk(KERN_INFO "module_A insert successfully\n");
        return 0;
}

void cleanup_module(void) {
        printk(KERN_INFO "module_A remove successfully\n");
}

EXPORT_SYMBOL(symbol_function);
