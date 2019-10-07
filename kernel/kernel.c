#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "drivers/timer.h"
#include "isr.h"
#include "memory/memory_segment.h"
#include "shell/shell.h"
#include "memory/page_allocator.h"
#include "memory/pdt.h"
#include "memory/pt.h"

extern struct def_vga_screen default_screen;
extern struct def_shell default_shell;
extern struct memory_seg_des * memmap;
extern int * nummem;
extern int max_page;
extern int last_inserted_page;


void _start()
{
    if (1) {
        memmap = (struct memory_seg_des *) 0x9104;
        nummem = (int *) 0x9100;
        last_inserted_page = 0;

        putstring(&default_screen, "Loading IDT\n");
        isr_install();
        putstring(&default_screen, "IDT Loaded\n");
        putstring(&default_screen, "Loading Shell\n");

        default_screen.width   = 80;
        default_screen.height  = 25;
        default_screen.cursorx = 0;
        default_screen.cursory = 0;
        clear(&default_screen);
        int k = 0;


        default_shell.appointed_screen = &default_screen;
        for (int i = 0; i < 256; i++) {
            default_shell.current_input[i] = 0;
        }
        default_shell.current_index = 0;
        putstring(&default_screen, "Shell Loaded\n");
        putstring(&default_screen, "Initializing Keyboard\n");

        asm volatile ("sti");
        // init_timer(10);

        init_page_alloc();
        for (int i = 0; i < 50; i++) {
            preserve(i);
        }
        find_next_free();

        int gp_dir_page = palloc();
        void * general_page_directory = (void *) (gp_dir_page * 0x1000);

        init_pdt(general_page_directory);
        identity_page(general_page_directory, min(0x10000, max_page));
        load_pdt(general_page_directory);

        enable_paging();

        clear(&default_screen);
        putstring(&default_screen, "Paging enabled");
        putchar(&default_screen, '\n');
        init_keyboard();

        shell_invite(&default_shell);
    }
} /* _start */
