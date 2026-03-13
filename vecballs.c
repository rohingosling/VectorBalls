//****************************************************************************
// Program: Vector Balls
// Version: 1.1
// Date:    1992-01-05
// Author:  Rohin Gosling
//
// Module:  Vector Balls
//
// Description:
//
//   Vector ball loading, rendering, key-framing, and rendering functions.
//
//****************************************************************************

#include "graphics.h"

//---------------------------------------------------------------------------
// Local Constants
//---------------------------------------------------------------------------

#define NUMBER_BALLS          20
#define NUMBER_BIT_MAPS       36
#define NUMBER_CONFIGURATIONS 13
#define NUMBER_MORPHS         18

#define PIT_FREQUENCY         1193182.0	// 8254 PIT clock frequency (Hz)
#define REFERENCE_FRAME_RATE  70.0		// Original target frame rate (fps)

//---------------------------------------------------------------------------
// File-Scope Variables
//---------------------------------------------------------------------------

// Ball sprite data.

static BIT_BLOCK ball [ MAX_BALL_BIT_BLOCKS ];

// Ball configuration data.
// Each configuration defines (x, y, z) positions for all 20 balls.

static CALCULATION_TYPE configuration [ NUMBER_CONFIGURATIONS ][ NUMBER_BALLS ][ 3 ] =
{
	// configuration 1 (Single Ball)

	{
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   },
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   },
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   },
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   },
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }
	},

	// configuration 2

	{
		{0   ,10  ,0   }, {-10 ,10  ,10  }, {0   ,10  ,10  }, {10  ,10  ,10  },
		{10  ,10  ,0   }, {10  ,10  ,-10 }, {0   ,10  ,-10 }, {-10 ,10  ,-10 },
		{-10 ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   },
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   },
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }
	},

	// configuration 3

	{
		{0   ,10  ,0   }, {-10 ,10  ,10  }, {0   ,10  ,10  }, {10  ,10  ,10  },
		{10  ,10  ,0   }, {10  ,10  ,-10 }, {0   ,10  ,-10 }, {-10 ,10  ,-10 },
		{-10 ,10  ,0   }, {-5  ,10  ,5   }, {5   ,10  ,5   }, {5   ,10  ,-5  },
		{-5  ,10  ,-5  }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   },
		{0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }, {0   ,10  ,0   }
	},

	// configuration 4 (Pyramid1)

	{
		{0   ,10  ,0   }, {-10 ,10  ,10  }, {0   ,10  ,10  }, {10  ,10  ,10  },
		{10  ,10  ,0   }, {10  ,10  ,-10 }, {0   ,10  ,-10 }, {-10 ,10  ,-10 },
		{-10 ,10  ,0   }, {-5  ,0   ,5   }, {5   ,0   ,5   }, {5   ,0   ,-5  },
		{-5  ,0   ,-5  }, {0   ,-10 ,0   }, {0   ,-10 ,0   }, {0   ,-10 ,0   },
		{0   ,-10 ,0   }, {0   ,-10 ,0   }, {0   ,-10 ,0   }, {0   ,-10 ,0   }
	},

	// configuration 5 (Cube)

	{
		{-10 ,10  ,10  }, {0   ,10  ,10  }, {10  ,10  ,10  }, {10  ,10  ,0   },
		{10  ,10  ,-10 }, {0   ,10  ,-10 }, {-10 ,10  ,-10 }, {-10 ,10  ,0   },
		{-10 ,0   ,10  }, {10  ,0   ,10  }, {10  ,0   ,-10 }, {-10 ,0   ,-10 },
		{-10 ,-10 ,10  }, {0   ,-10 ,10  }, {10  ,-10 ,10  }, {10  ,-10 ,0   },
		{10  ,-10 ,-10 }, {0   ,-10 ,-10 }, {-10 ,-10 ,-10 }, {-10 ,-10 ,0   }
	},

	// configuration 6 (3 axis right angled star 1)

	{
		{0   ,20  ,0   }, {0   ,10  ,0   }, {0   ,0   ,10  }, {10  ,0   ,0   },
		{0   ,0   ,0   }, {0   ,0   ,0   }, {0   ,0   ,0   }, {0   ,0   ,0   },
		{0   ,0   ,20  }, {20  ,0   ,0   }, {0   ,0   ,-20 }, {-20 ,0   ,0   },
		{0   ,0   ,0   }, {0   ,0   ,0   }, {0   ,0   ,0   }, {0   ,0   ,0   },
		{0   ,0   ,-10 }, {-10 ,0   ,0   }, {0   ,-10 ,0   }, {0   ,-20 ,0   }
	},

	// configuration 7 (3 axis right angled star 2)

	{
		{0   ,20  ,0   }, {0   ,10  ,0   }, {0   ,0   ,10  }, {10  ,0   ,0   },
		{-5  ,5   ,5   }, {5   ,5   ,5   }, {5   ,5   ,-5  }, {-5  ,5   ,-5  },
		{0   ,0   ,20  }, {20  ,0   ,0   }, {0   ,0   ,-20 }, {-20 ,0   ,0   },
		{-5  ,-5  ,5   }, {5   ,-5  ,5   }, {5   ,-5  ,-5  }, {-5  ,-5  ,-5  },
		{0   ,0   ,-10 }, {-10 ,0   ,0   }, {0   ,-10 ,0   }, {0   ,-20 ,0   }
	},

	// configuration 8 (N)

	{
		{-10 ,15  ,-10 }, {-10 ,15  ,-5  }, {-10 ,15  ,0   }, {-10 ,15  ,5   },
		{-10 ,15  ,10  }, {-5  ,15  ,5   }, {0   ,15  ,0   }, {5   ,15  ,-5  },
		{10  ,15  ,-10 }, {10  ,15  ,-5  }, {10  ,15  ,0   }, {10  ,15  ,5   },
		{10  ,15  ,10  }, {0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   },
		{0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   }
	},

	// configuration 9 (E)

	{
		{10  ,-15 ,-10 }, {5   ,-15 ,-10 }, {0   ,-15 ,-10 }, {-5  ,-15 ,-10 },
		{-10 ,-15 ,-10 }, {-10 ,-15 ,-5  }, {-10 ,-15 ,0   }, {-10 ,-15 ,5   },
		{-10 ,-15 ,10  }, {-5  ,-15 ,10  }, {0   ,-15 ,10  }, {5   ,-15 ,10  },
		{10  ,-15 ,10  }, {-5  ,-15 ,0   }, {0   ,-15 ,0   }, {5   ,-15 ,0   },
		{0   ,-15 ,0   }, {0   ,-15 ,0   }, {0   ,-15 ,0   }, {0   ,-15 ,0   }
	},

	// configuration 10 (X)

	{
		{-10 ,15  ,10  }, {-5  ,15  ,5   }, {0   ,15  ,0   }, {5   ,15  ,-5  },
		{10  ,15  ,-10 }, {-10 ,15  ,-10 }, {-5  ,15  ,-5  }, {5   ,15  ,5   },
		{10  ,15  ,10  }, {0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   },
		{0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   },
		{0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   }
	},

	// configuration 11 (U)

	{
		{-10 ,-15 ,10  }, {-10 ,-15 ,5   }, {-10 ,-15 ,0   }, {-10 ,-15 ,-5  },
		{-5  ,-15 ,-10 }, {0   ,-15 ,-10 }, {5   ,-15 ,-10 }, {10  ,-15 ,-5  },
		{10  ,-15 ,0   }, {10  ,-15 ,5   }, {10  ,-15 ,10  }, {10  ,-15 ,0   },
		{10  ,-15 ,10  }, {-10 ,-15 ,10  }, {5   ,-15 ,-10 }, {10  ,-15 ,0   },
		{-10 ,-15 ,10  }, {-10 ,-15 ,5   }, {-10 ,-15 ,0   }, {-10 ,-15 ,-5  }
	},

	// configuration 12 (S)

	{
		{10  ,15  ,10  }, {5   ,15  ,10  }, {0   ,15  ,10  }, {-5  ,15  ,10  },
		{-10 ,15  ,5   }, {-5  ,15  ,0   }, {0   ,15  ,0   }, {5   ,15  ,0   },
		{10  ,15  ,-5  }, {5   ,15  ,-10 }, {0   ,15  ,-10 }, {-5  ,15  ,-10 },
		{-10 ,15  ,-10 }, {0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   },
		{0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   }, {0   ,15  ,0   }
	},

	// configuration 13 (Star 3)

	{
		{0   ,20  ,0   }, {5   ,15  ,5   }, {5   ,0   ,15  }, {15  ,0   ,5   },
		{-10 ,10  ,10  }, {10  ,10  ,15  }, {10  ,15  ,-10 }, {-10 ,10  ,-15 },
		{5   ,0   ,20  }, {20  ,-5  ,5   }, {5   ,0   ,-20 }, {-20 ,0   ,0   },
		{-15 ,-10 ,10  }, {10  ,-15 ,10  }, {10  ,-10 ,-15 }, {-10 ,-15 ,-10 },
		{5   ,0   ,-10 }, {-15 ,0   ,5   }, {0   ,-15 ,5   }, {0   ,-20 ,0   }
	}

}; // End of configuration initialization

// Distance-sorted rendering buffer, indexed by apparent sprite
// size (bit map index) as a proxy for Z depth.
//
// - We cheat with the z buffer by using the number of bit maps instead
//   of the defined z space for the objects, taking advantage of our use
//   of distance queued bit maps.
//
// - There will be errors, but hopefully they will not be noticed, as
//   they only manifest them selves a very close and very far distances.
//
// - The [2] is for the x and y coords of that ball.

static struct
{
	char z_count;

	struct
	{
		unsigned x, y, r;

	} ball [ NUMBER_BALLS ];

} z_buffer [ NUMBER_BIT_MAPS ];

// Morphing buffer.

static CALCULATION_TYPE m_buffer [ NUMBER_BALLS ][ 3 ];

// Morphing info structure

static struct
{
	unsigned inc;           	// Morph progression increment
	unsigned flag;          	// 0=no morph, 1=morph
	INCREMENTATION_TYPE timer;	// Used to determine when to morph
	unsigned count;         	// Next morph to perform
	unsigned complete;      	// 0 = complete, 1 = Not complete

	struct
	{
		unsigned char config;
		long          wait;   // Period to wait befor preceding with morph

	} queue [ NUMBER_MORPHS ];

} morph;

// Screen data.

static unsigned vga_buffer = 0;
static unsigned sdx        = 160;	// X Screen displacement
static unsigned sdy        = 100;	// Y Screen displacement

// 3D state (persists across frames).

static INCREMENTATION_TYPE rx  = 0, ry  = 0, rz  = 0;		// Rotation factors
static CALCULATION_TYPE    dx  = 0, dy  = 0, dz  = 0;		// Displacement factors
static CALCULATION_TYPE    v   = 0;							// View point
static CALCULATION_TYPE    r3d = 0;							// Ball radius

static INCREMENTATION_TYPE xdi  = 0, ydi  = 0, zdi  = 0;	// Displacement  variables
static INCREMENTATION_TYPE xsi  = 0, ysi  = 0, zsi  = 0;	// Scale incrementation variables
static INCREMENTATION_TYPE xri  = 0, yri  = 0, zri  = 0;	// Rotation incrementation variables
static INCREMENTATION_TYPE xri2 = 0, yri2 = 0, zri2 = 0;	// Rotation inc incrementation variables

//----------------------------------------------------------------------------
// Function: GetBallData
//
// Description:
//
//   Opens the binary sprite data file (sprites.dat) and loads all ball sprites
//   into the global Ball array.
//
//   - For each sprite the function reads the metadata record (dimensions,
//     center displacement, size, file offset), allocates far memory for the
//     raw image data, then reads the pixel data from the file at the
//     stored offset.
//
// File Structure: sprites.dat
//
//      0 +--------------------------+
//        |       Sprite count       |
//      2 +--------------------------+
//        | Sprite Meta Data Records |
//        |           ...            |
//        |           ...            |
//        |           ...            |
//   12*N +--------------------------+
//        | Sprite Pixel Data Blocks |
//        |                          |
//        |           ...            |
//        |                          |
//        |           ...            |
//        |                          |
//        |           ...            |
//        |                          |
//        +--------------------------+
//
//   Top-level "sprites.dat" Binary File Layout:
//
//   - The file begins with a 2-byte sprite count, followed by a contiguous 
//     array of metadata records, then the raw pixel data blocks that each 
//     record points into.
//
//   +------+------+------------+--------------------------------------------+
//   | Offs | Size | Name       | Description                                |
//   +------+------+------------+--------------------------------------------+
//   | 0    | 2    | num_balls  | Number of ball sprites in the file.        |
//   +------+------+------------+--------------------------------------------+
//   | 2    | 12*N | records[]  | Array of N Sprite metadata records.        |
//   +------+------+------------+--------------------------------------------+
//   | vary | vary | pixel_data | Raw pixel data referenced by each record.  |
//   +------+------+------------+--------------------------------------------+
//                                                                              
//   Sprite Metadata Record (12 bytes):
//
//   - Each record stores the sprite dimensions, center displacement offsets, 
//     pixel data size, and an absolute file offset used to seek to the raw 
//     pixel data.
//
//   +------+------+------+--------------------------------------------------+
//   | Offs | Size | Name | Description                                      |
//   +------+------+------+--------------------------------------------------+
//   | 0    | 2    | xs   | Sprite width in pixels.                          |
//   +------+------+------+--------------------------------------------------+
//   | 2    | 2    | ys   | Sprite height in pixels.                         |
//   +------+------+------+--------------------------------------------------+
//   | 4    | 2    | xd   | Horizontal center displacement for positioning.  |
//   +------+------+------+--------------------------------------------------+
//   | 6    | 2    | yd   | Vertical center displacement for positioning.    |
//   +------+------+------+--------------------------------------------------+
//   | 8    | 2    | size | Size of raw pixel data in bytes.                 |
//   +------+------+------+--------------------------------------------------+
//   | 10   | 2    | offs | Absolute file offset to raw pixel data.          |
//   +------+------+------+--------------------------------------------------+
//
//   How to load file:
//
//   1. Open the file and read the 2-byte frame count, to know how many 
//      sprites to look for in the file.
//
//   2. For each of the N sprites, seek to the current metadata position
//      and read one 12-byte metadata record. Save the file position so
//      the next record can be read on the following iteration.
//
//   3. Copy the metadata fields (xs, ys, xd, yd, size) into the
//      corresponding BIT_BLOCK entry in the ball array.
//
//   4. Seek to the absolute file offset stored in the record's offs
//      field, allocate a far memory block of size bytes, and read the
//      raw pixel data into it.
//
//   5. Close the file after all sprites have been loaded.
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

void GetBallData ( void )
{

	struct BALL_DATA
  	{
		unsigned xs;
		unsigned ys;
		unsigned xd;
		unsigned yd;
		unsigned size;
    	unsigned offs;
	};

	struct BALL_DATA  init_ball_data = { 0, 0, 0, 0, 0, 0 };
  	struct BALL_DATA* ball_data      = &init_ball_data;

	FILE* ball_file;

	unsigned ball_count = 0;
  	unsigned num_balls  = 0;
	long     file_offs  = 0;

	// Open the ball data file and find out how many balls.

	if ( ( ball_file = fopen ( "sprites.dat", "rb" ) ) == NULL )
  	{
		clrscr   ();
		textattr ( 0x4F );
		cprintf  ( "\n\r\n\r LOAD ERROR : Unable to load demo...(Loading files are missing)\n\r\n\r" );
		textattr ( 8 );
	}

	// Read the total number of ball frames stored in the file header, then record the current file position.

	fread ( &num_balls, 2, 1, ball_file );
	file_offs = ftell ( ball_file );

	// Iterate through each ball frame, reading its metadata and allocating far memory for its image data.

	for ( ball_count = 0; ball_count < num_balls; ball_count++ )
  	{
		// Seek to the current metadata position and read one 12-byte record.

		fseek ( ball_file, file_offs, SEEK_SET );
		fread ( ball_data, sizeof ( struct BALL_DATA ), 1, ball_file );

		// Save the file position so the next record can be read on the following iteration.

		file_offs = ftell ( ball_file );

		ball [ ball_count ].size = ball_data->size;
		ball [ ball_count ].xs   = ball_data->xs;
		ball [ ball_count ].ys   = ball_data->ys;
		ball [ ball_count ].xd   = ball_data->xd;
		ball [ ball_count ].yd   = ball_data->yd;

		// Get Image data from the ball data file.

		fseek ( ball_file, ball_data->offs, SEEK_SET );

		ball [ ball_count ].block =	( unsigned char far* ) farcalloc ( ball_data->size, sizeof ( unsigned char ) );

		fread ( ball [ ball_count ].block, ball_data->size, 1, ball_file );
	}

	// All frames loaded. Close the ball data file.

	fclose ( ball_file );
}

//----------------------------------------------------------------------------
// Function: FreeRAM
//
// Description:
//
//   Deallocates the far memory blocks that were allocated for each ball
//   sprite frame by GetBallData.
//
//   - Iterates through all MAX_BALL_BIT_BLOCKS entries in the global Ball
//     array and calls farfree on each Block pointer.
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

void FreeRAM ( void )
{
	unsigned count = 0;

	// Free ball bit map data.

	for ( count = 0; count < MAX_BALL_BIT_BLOCKS; count++ )
	{
		farfree ( ball [ count ].block );
	}
}

//----------------------------------------------------------------------------
// Function: InitMorphQueue
//
// Description:
//
//   Initializes the morph state and populates the 18-entry morph queue.
//   Each entry specifies which ball configuration to morph into and how
//   many frames to hold before advancing to the next morph.
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

static void InitMorphQueue ( void )
{
	// Initialize morphing info

	morph.inc      = 1;		// {{Add comments}}
	morph.flag     = 0;		// {{Add comments}}
	morph.timer    = 0;		// {{Add comments}}
	morph.count    = 0;		// {{Add comments}}
	morph.complete = 0;		// {{Add comments}}

	// Morph 1 (9 flat)

	morph.queue [ 0 ].config = 1;
	morph.queue [ 0 ].wait   = 200;

	// Morph 2 (13 flat)

	morph.queue [ 1 ].config = 2;
	morph.queue [ 1 ].wait   = 150;

	// Morph 3 (Pirimyd 1)

	morph.queue [ 2 ].config = 3;
	morph.queue [ 2 ].wait   = 400;

	// Morph 4 (Cube)

	morph.queue [ 3 ].config = 4;
	morph.queue [ 3 ].wait   = 400;

	// Morph 5 (3 Axis star 1)

	morph.queue [ 4 ].config = 5 ;
	morph.queue [ 4 ].wait   = 400;

	// Morph 6 (3 Axis star 2)

	morph.queue [ 5 ].config = 6;
	morph.queue [ 5 ].wait   = 200;

	// Morph 7

	morph.queue [ 6 ].config = 4;
	morph.queue [ 6 ].wait   = 400;

	// Morph 8   (N)

	morph.queue [ 7 ].config = 7;
	morph.queue [ 7 ].wait   = 400;

	// Morph 9

	morph.queue [ 8 ].config = 12;
	morph.queue [ 8 ].wait   = 300;

	// Morph 10  (E)

	morph.queue [ 9 ].config = 8;
	morph.queue [ 9 ].wait   = 10;

	// Morph 11

	morph.queue [ 10 ].config = 12;
	morph.queue [ 10 ].wait   = 300;

	// Morph 12  (X)

	morph.queue [ 11 ].config = 9;
	morph.queue [ 11 ].wait   = 10;

	// Morph 13

	morph.queue [ 12 ].config = 12;
	morph.queue [ 12 ].wait   = 300;

	// Morph 14  (U)

	morph.queue [ 13 ].config = 10;
	morph.queue [ 13 ].wait   = 10;

	// Morph 15

	morph.queue [ 14 ].config = 12;
	morph.queue [ 14 ].wait   = 300;

	// Morph 16  (S)

	morph.queue [ 15 ].config = 11;
	morph.queue [ 15 ].wait   = 10;

	// Morph 17
	morph.queue[16].config = 12;
	morph.queue[16].wait   = 300;

	// Morph 18

	morph.queue [ 17 ].config = 4;
	morph.queue [ 17 ].wait   = 400;
}

//----------------------------------------------------------------------------
// Function: StepMorph
//
// Description:
//
//   Steps the morph buffer one increment toward the target configuration.
//   For each ball, the X, Y, and Z coordinates are independently incremented
//   or decremented by one unit toward the target values.
//
//   If all coordinates for all balls have reached their targets, the morph 
//   is complete and the queue advances to the next entry.
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

static void StepMorph ( void )
{
	unsigned ball_count = 0;

	morph.complete = 0;

	for ( ball_count = 0; ball_count<NUMBER_BALLS; ball_count++ )
	{
		//--------------------------------------------------------------------
		// Morph X
		//--------------------------------------------------------------------

		if ( m_buffer [ ball_count ][ 0 ] <	configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 0 ])
		{
			// Positive incrementation.

			if ( m_buffer [ ball_count ][ 0 ] != configuration[morph.queue [ morph.count ].config ][ ball_count ][ 0 ])
			{
				m_buffer [ ball_count ][ 0 ]++;
				morph.complete = 1;
			}
		}
		else
		{
			// Negative incrementation.

			if ( m_buffer [ ball_count ][ 0 ] != configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 0 ])
			{
				m_buffer [ ball_count ][ 0 ]--;
				morph.complete = 1;
			}
		}

		//--------------------------------------------------------------------
		// Morph Y
		//--------------------------------------------------------------------

		if ( m_buffer [ ball_count ][ 1 ] <	configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 1 ] )
		{
			// Positive incrementation.

			if ( m_buffer [ ball_count ][ 1 ] != configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 1 ] )
			{
				m_buffer [ ball_count ][ 1 ]++;
				morph.complete = 1;
			}
		}
		else
		{
			// Negative incrementation.

			if ( m_buffer [ ball_count ][ 1 ] != configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 1 ] )
			{
				m_buffer [ ball_count ][ 1 ]--;
				morph.complete = 1;
			}
		}

		//--------------------------------------------------------------------
		// Morph Z
		//--------------------------------------------------------------------

		if ( m_buffer [ ball_count ][ 2 ] < configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 2 ])
		{
			// Positive incrementation.

			if ( m_buffer [ ball_count ][ 2 ] !=	configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 2 ])
			{
				m_buffer [ ball_count ][ 2 ]++;
				morph.complete = 1;

			}
		}
		else
		{
			// Negative incrementation.

			if ( m_buffer [ ball_count ][ 2 ] !=	configuration [ morph.queue [ morph.count ].config ][ ball_count ][ 2 ])
			{
				m_buffer [ ball_count ][ 2 ]--;
				morph.complete = 1;
			}
		}

	} // for

	// If all balls have reached their target positions, advance to the next morph sequence in the queue.

	if ( morph.complete == 0 )
	{
		morph.count++;
		if ( morph.count >= NUMBER_MORPHS ) morph.count = 0;
		morph.flag = 0;
	}
}

//----------------------------------------------------------------------------
// Function: TransformAndQueueBalls
//
// Description:
//
//   For each ball in the current morph buffer, applies 3-axis rotation
//   (pitch, yaw, roll) using the precomputed sine table, adds displacement,
//   projects from 3D to 2D screen coordinates, and inserts the ball into the
//   Z-buffer at the depth slot corresponding to its apparent sprite size.
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

static void TransformAndQueueBalls ( void )
{
	unsigned ball_count = 0;
	unsigned sx  = 0, sy = 0;    // Actual screen coordinates
	unsigned ar  = 0;            // Balls apparent radius

	CALCULATION_TYPE x3d   = 0;
	CALCULATION_TYPE y3d   = 0;
	CALCULATION_TYPE z3d   = 0;
	CALCULATION_TYPE tx    = 0;
	CALCULATION_TYPE ty    = 0;
	CALCULATION_TYPE tz    = 0;
	CALCULATION_TYPE z_div = 0;

	for ( ball_count = 0; ball_count < NUMBER_BALLS; ball_count++ )
	{
		// Raw object coordinate retrieval.

		x3d = m_buffer [ ball_count ][ 0 ];
		y3d = m_buffer [ ball_count ][ 1 ];
		z3d = m_buffer [ ball_count ][ 2 ];

		// X rotation (Pitch).

		ty = ( ( y3d * sine [ rx + 90 ]) >> 8 ) - ( ( z3d * sine [ rx ] ) >> 8 );
		tz = ( ( y3d * sine [ rx ] ) >> 8 ) + ( ( z3d * sine [ rx + 90 ] ) >> 8 );

		y3d = ty;
		z3d = tz;

		// Y rotation (Yaw).

		tx = ( ( x3d * sine [ ry + 90 ] ) >> 8 ) - ( ( z3d * sine [ ry ] ) >> 8 );
		tz = ( ( x3d * sine [ ry ] ) >> 8 ) + ( ( z3d * sine [ ry + 90 ] ) >> 8 );

		x3d = tx;
		z3d = tz;

		// Z rotation (Roll).

		tx = ( ( x3d * sine [ rz + 90 ] ) >> 8 ) - ( ( y3d * sine [ rz ] ) >> 8 );
		ty = ( ( x3d * sine [ rz ] ) >> 8 ) + ( ( y3d * sine [ rz + 90 ] ) >> 8 );

		x3d = tx;
		y3d = ty;

		// Object displacement.

		//x3d += dx;
		//y3d += dy;
		z3d += dz;

		yri = ( 2 * sine [ yri2 ] ) >> 8;
		zri = ( 1 * sine [ zri2 ] ) >> 8;

		// Divide by zero check.

		if ( ( z_div = v+z3d ) == 0 ) z_div = 1;

		// 3D to 2D projection.

		sx  = ( unsigned ) sdx + ( v * x3d ) / ( z_div );
		sy  = ( unsigned ) sdy - ( v * y3d ) / ( z_div );
		ar  = ( v * r3d ) / ( z_div );

		// Bit map index, boundary overflow check.

		if ( ar >= NUMBER_BIT_MAPS ) ar = NUMBER_BIT_MAPS-1;
		if ( ar < 0) ar = 0;

		z_buffer [ ar ].ball [ z_buffer [ ar ].z_count ].x = sx;
		z_buffer [ ar ].ball [ z_buffer [ ar ].z_count ].y = sy;
		z_buffer [ ar ].ball [ z_buffer [ ar ].z_count ].r = ar;
		z_buffer [ ar ].z_count++;

	} // for
}

//----------------------------------------------------------------------------
// Function: RenderBalls
//
// Description:
//
//   Traverses the Z-buffer from the smallest (farthest) sprite size to the
//   largest (nearest), rendering each queued ball using the appropriate
//   distance-scaled bitmap. This produces correct back-to-front ordering.
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

static void RenderBalls ( void )
{
	unsigned z_queue = 0;	// Current distance slot index in the Z-buffer.
	unsigned z_count = 0;	// Ball counter within the current distance slot.
	unsigned sx      = 0;	// Screen X coordinate of the current ball.
	unsigned sy      = 0;	// Screen Y coordinate of the current ball.
	unsigned ar      = 0;	// Sprite index (apparent radius) for bitmap selection.

	// Iterate through the Z-buffer from smallest (farthest) to largest (nearest).

	for ( z_queue = 0; z_queue < NUMBER_BIT_MAPS; z_queue++ )
	{
		// Skip empty distance slots.

		if ( z_buffer [ z_queue ].z_count > 0 )
		{
			// Reset the ball counter for this distance slot.

			z_count = 0;

			while ( z_count < z_buffer [ z_queue ].z_count )
			{
				// Retrieve screen position and sprite index for this ball.

				sx = z_buffer [ z_queue ].ball [ z_count ].x;
				sy = z_buffer [ z_queue ].ball [ z_count ].y;
				ar = z_buffer [ z_queue ].ball [ z_count ].r;

				// Blit the distance-scaled ball sprite to the screen buffer.

				PutImage
				(
					sx + ball [ ar ].xd - 41,
					sy + ball [ ar ].yd - 33,
					ball [ ar ].xs,
					ball [ ar ].size,
					0,
					FP_SEG ( ball [ ar ].block ),
					FP_OFF ( ball [ ar ].block ),
					vga_buffer
				);

				// Advance to the next ball in this distance slot.

				z_count++;

			} // while
		} // if
	} // for
}

//----------------------------------------------------------------------------
// Function: GetHighResolutionTicks
//
// Description:
//
//   Returns a high-resolution tick count by combining the BIOS tick counter
//   (18.2 Hz) with the 8254 PIT channel 0 counter for sub-tick precision.
//   The result is in units of the PIT clock (1,193,182 Hz).
//
// Arguments:
//
//   - None
//
// Returns:
//
//   - Tick count as unsigned long.
//
//----------------------------------------------------------------------------

static unsigned long GetHighResolutionTicks ( void )
{
	unsigned long bios_ticks;
	unsigned char low;
	unsigned char high;
	unsigned int  pit_counter;

	// Disable interrupts for an atomic read of the BIOS tick count
	// and PIT counter.

	disable ();

	// Read BIOS tick count from 0040:006C

	bios_ticks = *( unsigned long far * ) MK_FP ( 0x0040, 0x006C );

	// Latch PIT counter 0 and read its current countdown value

	outportb ( 0x43, 0x00 );
	low  = inportb ( 0x40 );
	high = inportb ( 0x40 );

	enable ();

	pit_counter = ( ( unsigned int ) high << 8 ) | low;

	// PIT counts down from 65536, so invert to get elapsed sub-ticks.
	// Combined with bios_ticks shifted left by 16, this gives a
	// monotonically increasing tick count.

	return ( bios_ticks << 16 ) + ( 65536UL - ( unsigned long ) pit_counter );
}

//----------------------------------------------------------------------------
// Function: Balls
//
// Description:
//
//   Main animation loop for the vector ball demo.
//
//   - Switches to Mode 13h, then runs a continuous loop that clears the
//     screen, plots the starfield, transforms and renders distance-queued
//     3D balls, flips the double buffer, and controls the palette fade.
//
//   - The 20 balls are arranged in one of 13 predefined configurations and
//     smoothly morph between them through 18 sequenced transitions.
//
//   - 3D rotation is applied on three axes using a precomputed sine table,
//     and balls are Z-sorted via a distance queue indexed by apparent
//     sprite size.
//
//   - The loop exits when the fade-out completes after the user presses ESC.
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

void Balls ( void )
{
	// Local variables used only during initialization.

	unsigned         ball_count     = 0;	// Loop counter for ball/frame iteration.
	unsigned         config_size    = 0;	// Byte size of one configuration's coordinate data.
	unsigned         z_queue        = 0;	// Loop counter for Z-buffer initialization.
	CALCULATION_TYPE config_count   = 0;	// Loop counter for configuration iteration.
	CALCULATION_TYPE sx             = 0;	// X scaling factor.
	CALCULATION_TYPE sy             = 0;	// Y scaling factor.
	CALCULATION_TYPE sz             = 0;	// Z scaling factor.
	CALCULATION_TYPE rs             = 0;	// Radius scaling factor.

	// High-resolution timer for frame delta calculation.

	unsigned long       previous_ticks;				// PIT tick count at start of previous frame.
	unsigned long       current_ticks;				// PIT tick count at start of current frame.
	INCREMENTATION_TYPE delta_time;					// Elapsed time since previous frame, in seconds.
	INCREMENTATION_TYPE frame_step;					// Elapsed time normalized to reference frame rate.
	INCREMENTATION_TYPE morph_step_accumulator;		// Fractional morph step carry-over between frames.

	// Initialize VGA buffer segment.

	vga_buffer = FP_SEG ( screen_buffer_320x200 );

	// Initialize morph queue.

	InitMorphQueue ();

	// Initialize the z buffer.

	for ( z_queue = 0; z_queue < NUMBER_BIT_MAPS; z_queue++ )
	{
		z_buffer [ z_queue ].z_count = 0;
	}

	// Clear the quit flag so the main loop will run.

	program_flags.quit = 0;

	// Switch to Mode 13h, load the ball palette, and clear the screen.

	SetMode13   ();
	SetPalette  ( 0, 256, FP_SEG ( ball_palette ), FP_OFF ( ball_palette ) );
	ClearScreen ( 0, vga_buffer );

	// Initialize ball count and compute the byte size of one configuration.

	ball_count  = 0;
	config_size = NUMBER_BALLS * 3 * sizeof ( CALCULATION_TYPE );

	// Initialize 3D transformation parameters.

	v  = 60;            // Set view point

	sx = 6;             //
  	sy = 6;             // Set scaling factors
  	sz = 6;             //
  	rs = 6;             //

	dx = 0;             //
  	dy = 0;             // Set displacement factors
  	dz = 100;           //

	rx = 0;             //
  	ry = 0;             // Set Rotation factors
  	rz = 0;             //

	r3d = 7 * rs / 2;   // Set Ball radius

	yri  = 0;			// Yaw rotation increment (oscillates via sine of yri2).
  	yri2 = 0;			// Yaw oscillation phase angle (degrees).

	zri  = 0;			// Roll rotation increment (oscillates via sine of zri2).
  	zri2 = 90;			// Roll oscillation phase angle (degrees), offset 90 from yaw.

	// Scale configurations so that we dont have to do it in real time.

	for ( config_count = 0; config_count < NUMBER_CONFIGURATIONS; config_count++ )
	{
		for ( ball_count = 0; ball_count < NUMBER_BALLS; ball_count++ )
    	{
			configuration [ config_count ][ ball_count ][ 0 ] *= sx;
			configuration [ config_count ][ ball_count ][ 1 ] *= sy;
			configuration [ config_count ][ ball_count ][ 2 ] *= sz;

		}
	}

	// Initialize the morph buffer to a configuration

	movedata
  	(
		FP_SEG ( configuration [ 4 ] ),
		FP_OFF ( configuration [ 4 ] ),
		FP_SEG ( m_buffer ),
		FP_OFF ( m_buffer ),
		config_size
	);

	// Initialize high-resolution timer for frame delta calculation.

	previous_ticks         = GetHighResolutionTicks ();
	morph_step_accumulator = 0.0;

	// Perform main loop

	while ( program_flags.quit == 0 )
  	{
		// Calculate frame delta time.

		current_ticks  = GetHighResolutionTicks ();
		delta_time     = ( INCREMENTATION_TYPE )( current_ticks - previous_ticks ) / PIT_FREQUENCY;
		previous_ticks = current_ticks;

		// Guard against abnormal delta (timer wrap, stall, etc.)

		if ( delta_time <= 0.0 || delta_time > 0.5 ) delta_time = 1.0 / REFERENCE_FRAME_RATE;

		// Scale factor: how many "reference frames" worth of time elapsed.

		frame_step = delta_time * REFERENCE_FRAME_RATE;

		PaletteControl ();						// Update the palette for the fade effect.
		PlotStars ();							// Plot the star field.

		// Execute morph steps proportional to elapsed time.

		morph_step_accumulator += frame_step;

		while ( morph_step_accumulator >= 1.0 )
		{
			if ( morph.flag == 1 ) StepMorph ();
			morph_step_accumulator -= 1.0;
		}

		TransformAndQueueBalls ();			// Calculate coords of all balls and insert into Z-buffer.
		RenderBalls ();						// Render all balls back-to-front from the Z-buffer.
		FlipScreen  ( vga_buffer, 0xA000 );	// Update the screen		
		ClearScreen ( 0, vga_buffer );		// Wait_Retrace();

		// Update Morph timer

		morph.timer += frame_step;

		if ( morph.timer >= morph.queue [ morph.count ].wait )
    	{
			morph.flag                 = 1;
			morph.timer                = 0.0;
			morph_step_accumulator     = 0.0;
		}

		// Re-initialize the z buffer

		for ( z_queue = 0; z_queue < NUMBER_BIT_MAPS; z_queue++ )
		{
			z_buffer [ z_queue ].z_count = 0;
		}

		// Update rotation displacements, scaled by frame step.

		yri2 += frame_step;
		while ( yri2 >= 360.0 ) yri2 -= 360.0;
		while ( yri2 < 0.0    ) yri2 += 360.0;

		zri2 += frame_step;
		while ( zri2 >= 360.0 ) zri2 -= 360.0;
		while ( zri2 < 0.0    ) zri2 += 360.0;

		rx += frame_step;
		while ( rx >= 360.0 ) rx -= 360.0;
		while ( rx < 0.0    ) rx += 360.0;

		ry += yri * frame_step;
		while ( ry >= 360.0 ) ry -= 360.0;
		while ( ry < 0.0    ) ry += 360.0;

		rz += zri * frame_step;
		while ( rz >= 360.0 ) rz -= 360.0;
		while ( rz < 0.0    ) rz += 360.0;

		// Delay

		delay ( 1 );

	} // while

	// Restore 80x25 text mode before returning to DOS.

	SetTextMode ( TEXT_MODE_25_ROWS );
}
