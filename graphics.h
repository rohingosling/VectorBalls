//****************************************************************************
// Program: Vector Balls
// Version: 1.1
// Date:    1992-01-05
// Author:  Rohin Gosling
//
// Module:  Graphics Header
//
// Description:
//
//   Shared type definitions, global variable declarations, and function
//   prototypes used across all modules.
//
//****************************************************************************

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <DOS.H>
#include <STDIO.H>
#include <ALLOC.H>
#include <MEM.H>
#include <STDLIB.H>
#include <CONIO.H>
#include "gfx13.h"

//---------------------------------------------------------------------------
// Hash Defines
//---------------------------------------------------------------------------

#define MAX_BALLS           60 // Maximum number of balls
#define MAX_BALL_BIT_BLOCKS 36 // Number of ball frames

#define MAX_STAR_BIT_BLOCKS 25
#define MAX_STARS           4000

//---------------------------------------------------------------------------
// Type Defines
//---------------------------------------------------------------------------

typedef int    CALCULATION_TYPE;
typedef double INCREMENTATION_TYPE;

// Bit block info structure

typedef struct
{
	unsigned          size;		// Bit block size.
	unsigned          xs, ys;   // X and Y dimensions of bit block.
	unsigned          xd, yd;   // Center displacement, from top left corner off screen.
	unsigned char far *block;	// Raw image data.

} BIT_BLOCK;

// Program state flags.

struct PROGRAM_FLAGS_TYPE
{
	unsigned char	quit    :1; // 0=Running, 1=Quit
	unsigned char	pause   :1; // 0=Running, 1=Paused
	unsigned char	fade    :2; // 0=Fade in, 1=No fading, 2=Fade out
};

//---------------------------------------------------------------------------
// Shared Global Variables (defined in main.c)
//---------------------------------------------------------------------------

extern unsigned char far screen_buffer_320x200 [ 64000 ];
extern int               col_offs;
extern unsigned char     temp_palette [ 768 ];
extern unsigned char     ball_palette [ 768 ];
extern int               sine [ 451 ];
extern struct PROGRAM_FLAGS_TYPE program_flags;

//---------------------------------------------------------------------------
// Keyboard Functions (main.c)
//---------------------------------------------------------------------------

void InstallKeyHandler   ( void );
void RestoreKeyHandler   ( void );

//---------------------------------------------------------------------------
// Palette Functions (main.c)
//---------------------------------------------------------------------------

void PaletteControl ( void );

//---------------------------------------------------------------------------
// Starfield Functions (starfield.c)
//---------------------------------------------------------------------------

void InitStarField ( void );
void PlotStars     ( void );

//---------------------------------------------------------------------------
// Vector Ball Functions (vector_balls.c)
//---------------------------------------------------------------------------

void GetBallData ( void );
void FreeRAM     ( void );
void Balls       ( void );

#endif
