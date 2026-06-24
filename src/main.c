//****************************************************************************
// Program: Vector Balls
// Version: 1.1
// Date:    1992-01-05
// Author:  Rohin Gosling
//
// Description:
//
//   Entry point for the Vector Ball demo. Handles graphics primitives, 
//   keyboard interrupt handler, and palette control.
//
//****************************************************************************

#include "graphics.h"

//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------

unsigned char far screen_buffer_320x200 [ 64000 ];	// 64000 byte virtual screen
int               col_offs = 63; 					// Palette data
unsigned char     temp_palette [ 768 ];				// Temporary palette data storage

// Ball palette.

unsigned char ball_palette [ 768 ] =
{
	0,0,0,0,0,0,2,2,2,4,4,4,6,6,6,8,8,8,10,10,10,12,12,12,12,12,12,13,13,13,
	14,14,14,15,15,15,16,16,16,17,17,17,18,18,18,19,19,19,21,21,21,23,23,23,
	25,25,25,27,27,27,29,29,29,31,31,31,33,33,33,36,36,36,40,40,40,43,43,43,
	46,46,46,50,50,50,53,53,53,56,56,56,60,60,60,63,63,63,6,6,36,6,8,37,6,
	13,37,6,17,35,6,20,36,7,7,32,7,7,39,7,10,38,7,12,39,7,19,39,7,24,40,8,
	8,34,8,8,41,8,10,40,8,11,42,8,16,41,8,21,40,9,9,36,9,9,43,9,10,45,9,12,
	44,9,15,43,9,18,43,9,23,43,9,24,45,10,10,41,10,10,47,10,12,48,10,13,46,
	10,15,48,10,16,45,10,18,46,10,21,46,10,24,47,11,11,36,11,11,39,11,11,43,
	11,11,50,11,15,50,11,17,49,11,19,48,11,21,49,11,24,49,12,12,45,12,12,52,
	12,16,52,12,19,53,12,21,51,12,24,51,13,13,38,13,13,41,13,13,54,13,15,55,
	13,17,54,13,19,56,13,21,55,13,22,53,13,24,54,14,14,57,14,17,56,14,18,58,
	14,21,58,14,23,57,15,15,42,15,15,47,15,15,51,15,15,59,15,16,61,15,19,60,
	15,21,61,15,23,59,15,24,61,16,16,44,16,16,49,16,16,57,16,16,63,16,18,62,
	16,20,63,16,23,63,16,26,61,16,27,63,17,17,47,17,17,55,17,17,59,17,25,63,
	17,29,63,18,18,45,18,18,52,18,18,63,18,21,63,18,31,63,18,34,63,18,38,63,
	19,19,50,19,19,54,19,19,58,19,23,63,19,26,63,19,36,63,19,40,63,19,43,63,
	19,49,63,20,20,47,20,20,61,20,28,63,20,32,63,20,45,63,20,54,63,20,61,63,
	21,21,49,21,21,52,21,21,58,21,24,63,21,30,63,21,34,63,21,37,63,21,47,63,
	21,50,63,21,63,63,22,22,54,22,22,62,22,26,63,22,43,63,22,52,63,22,55,63,
	22,59,63,23,23,50,23,28,63,23,31,63,23,36,63,23,39,63,23,45,63,23,49,63,
	24,24,52,24,24,63,24,34,63,24,42,63,25,25,57,25,26,63,25,29,63,26,26,60,
	26,31,63,26,37,63,26,41,63,27,27,55,27,27,58,27,27,63,27,33,63,28,29,63,
	29,29,59,29,32,63,29,36,63,29,39,63,30,34,63,31,31,60,31,31,63,32,33,63,
	32,36,63,32,39,63,34,34,63,34,37,63,35,39,63,36,36,63,36,42,63,38,38,63,
	38,41,63,39,43,63,40,40,63,40,45,63,42,46,63,43,44,63,45,45,63,45,48,63,
	46,50,63,47,47,63,48,51,63,50,52,63,51,54,63,54,57,63,56,59,63,59,62,63,
	61,63,63,2,2,23,3,3,25,4,7,32,7,15,38,8,8,38,9,9,33,9,9,39,11,13,49,12,
	12,37,12,14,51,0,0,0,0,0,19,0,3,19,0,6,19,0,8,20,0,11,19,1,1,21,1,1,24,
	1,3,22,1,5,23,1,11,23,2,2,26,2,5,26,2,10,26,2,13,25,3,3,28,3,5,29,3,7,
	27,3,17,27,4,4,26,4,4,31,4,7,30,4,9,31,4,12,30,4,17,31,5,5,28,5,5,33,5,
	9,33,5,11,34,5,14,34,5,20,32,6,6,30
};

// Trigonometric table:
// - sin(t), where 0 <= t <= (360+90)
// - We goto 451 instead of 450, to alow for a slight overflow of 1. <-- I need to fix this.

int sine [ 451 ] =
{
	0,4,8,13,17,22,26,31,35,39,44,
	48,53,57,61,65,70,74,78,83,87,91,
	95,99,103,107,111,115,119,123,127,131,135,
	138,142,146,149,153,156,160,163,167,170,173,
	177,180,183,186,189,192,195,198,200,203,206,
	208,211,213,216,218,220,223,225,227,229,231,
	232,234,236,238,239,241,242,243,245,246,247,
	248,249,250,251,251,252,253,253,254,254,254,
	254,254,255,254,254,254,254,254,253,253,252,
	251,251,250,249,248,247,246,245,243,242,241,
	239,238,236,234,232,231,229,227,225,223,220,
	218,216,213,211,208,206,203,200,198,195,192,
	189,186,183,180,177,173,170,167,163,160,156,
	153,149,146,142,138,135,131,127,123,119,115,
	111,107,103,99,95,91,87,83,78,74,70,
	65,61,57,53,48,44,39,35,31,26,22,
	17,13,8,4,0,-4,-8,-13,-17,-22,-26,
	-31,-35,-39,-44,-48,-53,-57,-61,-65,-70,-74,
	-78,-83,-87,-91,-95,-99,-103,-107,-111,-115,-119,
	-123,-127,-131,-135,-138,-142,-146,-149,-153,-156,-160,
	-163,-167,-170,-173,-177,-180,-183,-186,-189,-192,-195,
	-198,-200,-203,-206,-208,-211,-213,-216,-218,-220,-223,
	-225,-227,-229,-231,-232,-234,-236,-238,-239,-241,-242,
	-243,-245,-246,-247,-248,-249,-250,-251,-251,-252,-253,
	-253,-254,-254,-254,-254,-254,-255,-254,-254,-254,-254,
	-254,-253,-253,-252,-251,-251,-250,-249,-248,-247,-246,
	-245,-243,-242,-241,-239,-238,-236,-234,-232,-231,-229,
	-227,-225,-223,-220,-218,-216,-213,-211,-208,-206,-203,
	-200,-198,-195,-192,-189,-186,-183,-180,-177,-173,-170,
	-167,-163,-160,-156,-153,-149,-146,-142,-138,-135,-131,
	-127,-123,-119,-115,-111,-107,-103,-99,-95,-91,-87,
	-83,-78,-74,-70,-65,-61,-57,-53,-48,-44,-39,
	-35,-31,-26,-22,-17,-13,-8,-4,0,4,8,
	13,17,22,26,31,35,39,44,48,53,57,
	61,65,70,74,78,83,87,91,95,99,103,
	107,111,115,119,123,127,131,135,138,142,146,
	149,153,156,160,163,167,170,173,177,180,183,
	186,189,192,195,198,200,203,206,208,211,213,
	216,218,220,223,225,227,229,231,232,234,236,
	238,239,241,242,243,245,246,247,248,249,250,
	251,251,252,253,253,254,254,254,254,254,255
};

// Program state flags.

struct PROGRAM_FLAGS_TYPE program_flags = { 0, 0, 0 };


// Keyboard interrupt vector storage.

void interrupt far                 ( *int_09h )();
void interrupt far NewKeyHandler ();

//----------------------------------------------------------------------------
// Function: main
//
// Description:
//
//   The main function is the entry point for the program, it sets up the
//   system and calls the main animation sequence, it also handles the clean
//   up when the program exits.
//
// Arguments:
//
//   - None
//
// Returns:
//
//   - None
//
//----------------------------------------------------------------------------

void main ( void )
{
	// Hook the keyboard interrupt so we can detect key presses during the animation.

	InstallKeyHandler ();

	// Load all ball sprite frames and their metadata from the binary data file.

	GetBallData   ();

	// Initialize the 3D starfield with random star positions.

	InitStarField ();

	// Run the main animation loop. This blocks until the user presses ESC and the fade-out completes.

	Balls ();

	// Deallocate far memory used by ball sprite bitmaps.

	FreeRAM ();

	// Restore the original INT 0x09 keyboard handler before exiting to DOS.

	RestoreKeyHandler ();
}

//----------------------------------------------------------------------------
// Function: InstallKeyHandler
//
// Description:
//
//   Saves the current INT 0x09 keyboard interrupt vector into int_09h and
//   replaces it with NewKeyHandler.
//
//   This allows the program to intercept key presses (e.g., ESC) while the
//   animation is running.
//
// Arguments:
//
//   - None
//
// Returns:
//
//   - None
//
//----------------------------------------------------------------------------

void InstallKeyHandler ( void )
{
	int_09h = getvect ( 0x09 );
	setvect ( 0x09, NewKeyHandler );
}

//----------------------------------------------------------------------------
// Function: RestoreKeyHandler
//
// Description:
//
//   Restores the original BIOS INT 0x09 keyboard interrupt vector from the
//   saved int_09h pointer.
//
//   Must be called before exiting to DOS to prevent the system from jumping
//   to deallocated code on subsequent key presses.
//
// Arguments:
//
//   - None
//
// Returns:
//
//   - None
//
//----------------------------------------------------------------------------

void RestoreKeyHandler ( void )
{
	setvect ( 0x09, int_09h );

	// Clear keyboard buffer before we exit, so that we don't send stray
	// keys presses to the terminal after exiting.

	while ( kbhit () ) getch ();
}

//----------------------------------------------------------------------------
// Function: NewKeyHandler
//
// Description:
//
//   Custom keyboard interrupt service routine that replaces INT 0x09.
//
//   - Reads the scan code from port 0x60 and checks for specific key presses.
//
//   - When the ESC key is detected, it sets the fade-out flag to begin the
//     exit sequence.
//
//   - After processing, it chains to the original BIOS keyboard handler so
//     that standard keyboard servicing continues.
//
// Arguments:
//
//   - ... : Variadic (interrupt handler convention; no explicit arguments).
//
// Returns:
//
//   - None
//
//----------------------------------------------------------------------------

void interrupt far NewKeyHandler ()
{
	// Variable to hold the scan code read from the keyboard controller port.

	unsigned int port_60h = 0;

	// Read the scan code from port 0x60. If the high bit is clear, this is a key-press event (not a key-release).

	if ( ( port_60h = inportb ( 0x60 ) ) < 0x80 )
  	{
		// Dispatch on the scan code to handle specific key presses.

		switch ( port_60h )
    	{
			// ESC key (scan code 0x01): trigger the palette fade-out sequence to exit the program.

			case 0x01: program_flags.fade = 2;   // ESC
		}
	}

	// Chain to the original BIOS keyboard interrupt handler so normal processing continues.

	( *int_09h )();
}

//----------------------------------------------------------------------------
// Function: PaletteControl
//
// Description:
//
//   Manages gradual palette fade-in and fade-out transitions.
//
//   - When the fade flag is 0 (fade-in), it incrementally raises each RGB
//     component of temp_palette toward the target ball_palette values and
//     applies the result to the VGA DAC.
//
//   - When the fade flag is 2 (fade-out), it decrements each non-zero RGB
//     component toward black. Once all components reach zero the quit flag is
//     set, signalling the main loop to exit.
//
// Arguments:
//
//   - None
//
// Returns:
//
//   - None
//
//----------------------------------------------------------------------------

void PaletteControl ( void )
{
	// Palette component iterator (768 entries = 256 colors x 3 RGB components).

	unsigned col_count = 0;

	// Fade-in: gradually increase palette intensity toward the target ball_palette values.

	if ( program_flags.fade == 0 )
  	{
		// Increment each palette component that has not yet reached its target value minus the current offset.

		for ( col_count = 0; col_count < 768; col_count++ )
		{
			if ( ball_palette [ col_count ]-col_offs >= 0 )
			{
				temp_palette [ col_count ]++;
			}
		}

		// Reduce the offset, bringing the palette one step closer to full brightness.

		col_offs--;

		// Once the offset reaches zero, the fade-in is complete. Switch to the steady-state flag.

		if ( col_offs == 0 )
		{
			program_flags.fade = 1;
		}

		// Apply the updated temporary palette to the VGA hardware.

		SetPalette ( 0, 256, FP_SEG ( temp_palette ), FP_OFF ( temp_palette ) );
	}

	// Fade-out: gradually decrease palette intensity to black after the user presses ESC.

	if ( program_flags.fade == 2 )
  	{
		// Optimistically assume the fade-out is finished. If any component is still non-zero, we clear this below.

		program_flags.quit = 1;

		// Decrement each non-zero palette component toward black.

		for ( col_count = 0; col_count < 768; col_count++ )
		{
			// If this component still has brightness remaining, decrement it and signal that fading is not yet done.

			if ( temp_palette [ col_count ] > 0 )
      		{
				temp_palette [ col_count ]--;
				program_flags.quit = 0;
			}
		}

		// Apply the faded palette to the VGA hardware.

		SetPalette ( 0, 256, FP_SEG ( temp_palette ), FP_OFF ( temp_palette ) );
	}
}
