#include <linux/module.h>

int symbol_variable = 56;

extern int symbol_function(int);

int init_module(void) {
        printk(KERN_INFO "Insert module_B successfully\n");
	printk("symbol: %d\n", symbol_variable);
	symbol_function(symbol_variable);
        return 0;
}

void cleanup_module(void) {
        printk(KERN_INFO "Remove module_B successfully\n");
}

EXPORT_SYMBOL(symbol_variable);



MODULE_LICENSE("GPL");
MODULE_AUTHOR("FE");
MODULE_DESCRIPTION("OSLAB_MODULE: module_B");
