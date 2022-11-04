/*
 * ATARI ST HDC Emulator
 * 
 * File:    emuscsi.h
 * Author:  Steve Bradford
 * Created: September 2022
 *
 * 
 */

#ifndef _EMU_H
#define _EMU_H

#include "emu.h"

//extern DRIVES drv []; 

extern void modeSense0 ( char *, DRIVES *, int );
extern void modeSense4 ( char *, DRIVES *, int );

#endif
