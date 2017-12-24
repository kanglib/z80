/* zextest.c
 * Example program using z80emu to run the zexall and zexdoc tests. This will 
 * check if the Z80 is correctly emulated.
 *
 * Copyright (c) 2012, 2016 Lin Ke-Fong
 * Copyright (c) 2012 Chris Pressey
 *
 * This code is free, do whatever you want with it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "zextest.h"
#include "z80emu.h"
#include <string.h>
#include "z80user.h"

#define Z80_CPU_SPEED           4000000   /* In Hz. */
#define CYCLES_PER_STEP         (Z80_CPU_SPEED / 50)
#define MAXIMUM_STRING_LENGTH   100

static void	emulate (char *filename);

static unsigned char mem[] = {
	0x22, 0x47, 0x00, 0x3d, 0x53, 0x77, 0x23, 0x3d, 0x45, 0x77, 0x23, 0x3d, 0x43,
	0x77, 0x23, 0x77, 0x23, 0xc5, 0x0c, 0x77, 0x23, 0xc5, 0xfd, 0x77, 0x23, 0x3d,
	0x7b, 0x77, 0x23, 0x39, 0x44, 0x00, 0x47, 0xc5, 0x46, 0x31, 0x44, 0x00, 0x78,
	0x31, 0x46, 0x00, 0xfd, 0x22, 0xf9, 0x1e, 0x00, 0xfd, 0x7b, 0xf1, 0x1e, 0x00,
	0x77, 0x23, 0x39, 0x45, 0x00, 0x3e, 0x31, 0x45, 0x00, 0xc1, 0x1e, 0x00, 0x3d,
	0x7d, 0x77, 0x75, 0x03, 0x0b, 0x09,
};

int main (void)
{
	emulate(NULL);
        return EXIT_SUCCESS;
}

/* Emulate "zexdoc.com" or "zexall.com". */

static void emulate (char *filename)
{
	ZEXTEST	context;
	memcpy(context.memory, mem, sizeof(mem));
	Z80Reset(&context.state);
	do
		Z80Emulate(&context.state, CYCLES_PER_STEP, &context);
	while (context.state.status != Z80_STATUS_HALT);
	printf("%s\n", context.memory + 0x47);
}

/* Emulate CP/M bdos call 5 functions 2 (output character on screen) and 9
 * (output $-terminated string to screen).
 */

void SystemCall (ZEXTEST *zextest)
{
        if (zextest->state.registers.byte[Z80_C] == 2)

                printf("%c", zextest->state.registers.byte[Z80_E]);

        else if (zextest->state.registers.byte[Z80_C] == 9) {

                int     i, c;

                for (i = zextest->state.registers.word[Z80_DE], c = 0; 
                        zextest->memory[i] != '$';
                        i++) {

                        printf("%c", zextest->memory[i & 0xffff]);
                        if (c++ > MAXIMUM_STRING_LENGTH) {

                                fprintf(stderr,
                                        "String to print is too long!\n");
                                exit(EXIT_FAILURE);

                        }

                }

        }
}
