/*
	ROM spec file

	Main memory map

  	0x80000000  exception vectors, ...
  	0x80000400  zbuffer (size 320*240*2)
  	0x80025c00  codesegment
	      :  
  	0x8030F800  Audio Heap
  	0x8038F800  cfb 16b 3buffer (size 320*240*2*3)

        Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <nusys.h>

/* Use all graphic and audio micro codes */
beginseg
	name	"code"
	flags	BOOT OBJECT
	entry 	nuBoot
	address NU_SPEC_BOOT_ADDR
        stack   NU_SPEC_BOOT_STACK
	include "codesegment.o"
	include "$(ROOT)/usr/lib/PR/rspboot.o"
	include "$(ROOT)/usr/lib/PR/aspMain.o"
	include "$(ROOT)/usr/lib/PR/n_aspMain.o"
	include "$(ROOT)/usr/lib/PR/gspF3DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspL3DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspF3DEX2.Rej.fifo.o"
        include "$(ROOT)/usr/lib/PR/gspF3DEX2.NoN.fifo.o"
        include "$(ROOT)/usr/lib/PR/gspF3DLX2.Rej.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspS2DEX2.fifo.o"
endseg

beginseg
	name "midibank"
	flags RAW
	include "wave.ctl"
endseg

beginseg
	name "miditable"
	flags RAW
	include "wave.tbl"
endseg

beginseg
	name "seq"
	flags RAW
	include "midi.sbk"  
endseg

beginseg
	name "sfxbank"
	flags RAW
	include "se.ctl"
endseg

beginseg
	name "sfxtable"
	flags RAW
	include "se.tbl"
endseg

beginwave
	name	"nu3"
	include	"code"
	include "miditable"
	include "midibank"
	include "sfxbank"
	include "sfxtable"
	include "seq"
endwave
