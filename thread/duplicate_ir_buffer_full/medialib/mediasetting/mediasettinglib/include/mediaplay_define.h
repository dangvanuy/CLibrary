#ifndef PLAY_AMP_H_
#define PLAY_AMP_H_

// prompts the user for password


// Key processing return type
typedef enum { KEY_IGNORED = 0,	// key was ignored, can be processed by other modules
			   KEY_PROCESSED,	// key was processed, can NOT be processed by other modules
			   KEY_WANT_NEXT	// this and the NEXT key are reserved, can NOT be processed by other modules 
			 }
			 TKeyMode;

// status display part
typedef enum
{
	STSPART_LEFT = 1,
	STSPART_RIGHT,
}
TStatusPart;



#define CHAR_CODE_DISABLE	'\x0e'	// character code for disabled text style mode toggle

#define CHAR_CODE_NORMAL	'\x0f'	// character code for normal text style mode toggle

#define CHAR_CODE_HIGHLIGHT	'\a'	// character code for highlighted text style mode toggle

#define CHAR_CODE_SECTION	'\v'	// character code for section header (screen-centered line)

#define CHAR_CODE_BREAK	    '\r'	// character code for line break

#define CHAR_CODE_TAB	    '\t'	// character code for horizontal tab

// Displays one or two multi-line messages in a window on the screen, wipes it out on empty or NULL text
// Lines that start with the 0x0f code are emphasized
// Text lines are arranged to match the display surfaces, may be displayed on the same display line (multi-column display)


#endif /*PLAY_AMP_H_*/
