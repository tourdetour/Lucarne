#ifndef SHELL_H
#define SHELL_H
#include "../drivers/screen.h"
#include "../fonts/font_desc.h"
#include "../drivers/ATA/ata_pio.h"

struct def_shell {
    struct def_vga_screen * appointed_screen;
    char                    current_input[256];
    uint8_t                 current_index;
    uint32_t                term_color;
    struct font_desc *      font;
};

void putchar_sh(struct def_shell * sh, char c);

void shellexec(struct def_shell * sh);

void shell_invite(struct def_shell * sh);

void backspace(struct def_shell * sh);

struct def_shell default_shell;

#endif /* ifndef SHELL_H */
