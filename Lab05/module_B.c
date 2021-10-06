#include <linux/module.h>

int symbol_variable = 56;

extern int symbol_function(int);

int init_module(void) {
        printk(KERN_INFO "module_B insert successfully\n");
	printk("symbol: %d\n", symbol_variable);
	symbol_function(symbol_variable);
        return 0;
}

void cleanup_module(void) {
        printk(KERN_INFO "module_B remove successfully\n");
}

EXPORT_SYMBOL(symbol_variable);
