//****************************************************************************
// Program: Vector Balls
// Version: 1.1
// Date:    1992-01-05
// Author:  Rohin Gosling
//
// Module:  Starfield
//
// Description:
//
//   3D starfield initialization and rendering. Stars are positioned in a
//   3D volume and projected onto the screen with perspective division.
//   Each frame, stars advance toward the viewer and wrap at the near plane.
//
//****************************************************************************

#include "graphics.h"

//---------------------------------------------------------------------------
// File-Scope Variables
//---------------------------------------------------------------------------

// Star buffer

static struct
{
	long x, y, z;

} star_coords [ MAX_STARS ];


// Star field control structure

static struct
{
	long x_max;
  	long y_max;
  	long z_max;
	long z_inc;

} star_data;

//----------------------------------------------------------------------------
// Function: InitStarField
//
// Description:
//
//   Initializes the 3D starfield by setting the bounding volume dimensions
//   and the per-frame Z increment, then populating all MAX_STARS entries in
//   star_coords with random (x, y, z) positions.
//
//   X and Y are centered around the origin; Z ranges from 0 to Z_Max.
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

void InitStarField ( void )
{
	// Loop counter for initializing each star.

	long star_count = 0;

	// Define the 3D volume boundaries for the starfield and the per-frame Z movement speed.

	star_data.x_max = 32000;
	star_data.y_max = 32000;
	star_data.z_max = 1000;
	star_data.z_inc = -1;

	randomize ();

	// Assign each star a random position within the 3D volume, centered around the origin on X and Y.

	for ( star_count = 0; star_count < MAX_STARS; star_count++ )
  	{
		star_coords [ star_count ].x = random ( star_data.x_max ) - ( star_data.x_max / 2 );
		star_coords [ star_count ].y = random ( star_data.y_max ) - ( star_data.y_max / 2 );
		star_coords [ star_count ].z = random ( star_data.z_max );
	}
}

//----------------------------------------------------------------------------
// Function: PlotStars
//
// Description:
//
//   Projects all stars from 3D space onto the 2D screen using perspective
//   division and plots each visible star as a single pixel.
//
//   - Star brightness is computed from depth so that closer stars appear
//     brighter.
//
//   - After rendering, each star is advanced toward the viewer along the
//     Z axis.
//
//   - Stars that pass the near plane are wrapped back to the far plane,
//     creating a continuous forward-flying starfield effect.
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

void PlotStars ( void )
{
	// Local variables for perspective projection, screen coordinates, and the 3D position of each star.

	int   star_count = 0;
	float sx         = 0;
  	float sy         = 0;
  	float v          = 1;
	float x          = 0;
  	float y          = 0;
  	float z          = 0;

	// Star color intensity and the segment address of the virtual screen buffer.

	unsigned char col        = 0;
	unsigned      vga_buffer = FP_SEG ( screen_buffer_320x200 );

	// Iterate through all stars, projecting each from 3D space onto the 2D screen.

	for ( star_count = 0; star_count < MAX_STARS; star_count++ )
  	{
		// Retrieve and scale the star's 3D coordinates. X and Y are scaled up for perspective spread.

		x = ( float ) star_coords [ star_count ].x * 10.0;
		y = ( float ) star_coords [ star_count ].y * 10.0;
		z = ( float ) star_coords [ star_count ].z;

		// Guard against division by zero when the star is at the viewer's plane.

		if ( z == 0 ) z++;

		// Perspective divide: project 3D position onto 2D screen, centered at (160, 100).

		sx = v * ( x / z ) + 160;
		sy = v * ( y / z ) + 100;

		// Compute brightness based on depth. Closer stars are brighter, then scale down to the greyscale range.

		col = 31 - ( z * 31 / star_data.z_max );
		col = 2 * col / 3;

		// Clip to screen bounds (320x200) before plotting the star pixel.

		if ( ( sx >= 0 && sx < 320 ) && ( sy >= 0 && sy < 200 ) )
		{
      		PutPixel ( ( int ) sx, ( int ) sy, col, 0, vga_buffer );
		}

		// Move the star toward the viewer along the Z axis. If it passes the near plane, wrap it to the far plane.

		z = star_coords [ star_count ].z += star_data.z_inc;

		if ( z <= 0 )
		{
			star_coords [ star_count ].z = star_data.z_max;
		}
	}
}
