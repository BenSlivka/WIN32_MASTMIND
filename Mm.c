/***    MM.C - Master Mind Game
 *
 *      (c) 1991, Benjamin W. Slivka
 *
 *      MasterMind is a game (c) 1981 Pressman Toy Corporation,
 *      200 Fifth Avenue, New York, NY  10010.
 *
 *      The object of MasterMind is to guess a secret code consisting of
 *      a series of 4 colored pegs.  Each guess results in feedback which
 *      should narrow down the possible code values.  Using this feedback,
 *      the player will (with suitable application of logic) be able to
 *      deduce the computer's code.
 *
 *      To begin, the computer builds a code of 4 colored pegs.  For
 *      example: red, white, green, yellow.  The color and order of the
 *      pegs make up the code.
 *
 *      The player then builds a guess of 4 colored pegs, and asks the
 *      computer for feedback on the guess.  The computer reports the number
 *      of pegs in the guess which match the code:
 *          (1) in both color and position, and
 *          (2) in color only
 *
 *      Any pegs counted for (1) are not eligible to be counted for (2).
 *
 *
 *      Visual Layout of Game Window
 *      ============================
 *
 *      +-+-----------------------------------------------+-+
 *      |=|                 Master Mind                   |V|
 *      +-+-----------------------------------------------+-+
 *      | Options Help                                      |
 *      +---------------------------------------------------+
 *      |            ^                                      |
 *      | cxBorder   +-- cyBorder                           |
 *      | |          V                                      |
 *      | | +---------------------------+ ^                 |
 *      | | |                           | |                 |
 *      |<+>|  Well Area                | +- cyWell         |
 *      |   |                           | |                 |
 *      |   +---------------------------+ V                 |
 *      |      ^                                   cxBorder |
 *      |      +-cyWellToMove +cxMoveToResult             | |
 *      |      V              |                           | |
 *      |     +-------------+ | +-----+ |    /----------\ | |
 *      |     |          ^  |<+>|     | |    | New Game |<+>|
 *      |     |          |  |   |     | |    \----------/   |
 *      |     |          |  |   |  R  | |                   |
 *      |     |  Move    |  |   |  e  | |    /----------\   |
 *      |     |  Area    |  |   |  s  | |<+->|  Guess   |   |
 *      |     |          |  |   |  u  |   |  \----------/   |
 *	|     | 	 |  |	|  l  |   |		    |
 *	|     | 	 |  |	|  t  |   cxWellToButton    |
 *	|     | 	 |  |	|     | 		    |
 *	|     | 	 |  |	|  A  |      +----------+   |
 *	|     | 	 |  |	|  r  |      |		|   |
 *	|     |      cyMove |	|  e  |      |		|   |
 *	|     | 	 |  |	|  a  |      |	 Fun	|   |
 *	|     | 	 |  |	|     |      |		|   |
 *	|     | 	 |  |	|     |      |	 Area	|   |
 *	|     | 	 |  |	|     |      |		|   |
 *	|     |<- cxMove -->|	|     |      |		|   |
 *	|     | 	 |  |	|<-+->|      |		|   |
 *	|     | 	 V  |	|  |  |      |		|   |
 *	|     +-------------+	+--|--+      |		|   |
 *	|	^		   |	     |		|   |
 *	|	+--cyMoveToAnswer  cxResult  |		|   |
 *	|	V			     |		|   |
 *	|     +-------------+ \ 	     |		|   |
 *	|     | Answer Area |  +- cyAnswer   |		|   |
 *	|     +-------------+ / 	     +----------+   |
 *	|	 ^					    |
 *	|	 +--cyBorder				    |
 *      |        V                                          |
 *      +---------------------------------------------------+
 *
 *
 *      Visual Layout of Peg Box
 *      ========================
 *          The "Peg Box" is the building block for the "Well Area" and the
 *      "Move Area".  It has room for a "Peg" centered inside the box.
 *
 *
 *          <-- cxPegBox --->
 *        ^ +---------------+
 *        | |               |
 *        | | cxPegOffset   |
 *          | |             | --
 *        c | |   -----     |  ^
 *        y | |  /     \    |  |
 *        P | | /       \   |  |
 *        e | | |  Peg  |   |  +-- cyPeg
 *        g |<->|       |   |  |
 *        B |   \       /   |  |
 *        o |    \     /    |  |
 *        x |     -----     |  V
 *          |       ^       | --
 *        | |       +-- cyPegOffset
 *        | |       V       |
 *        V +---------------+
 *             |         |
 *             |<---+--->|
 *                  |
 *
 *                cxPeg
 *
 *
 *      Visual Layout of Pin Box
 *      ========================
 *          The "Pin Box" is the building block for the "Result Area".
 *      It has room for up to 4 Pins grouped around the center of the box,
 *      so that Pins in one pin box do not blend visually with the pins of
 *      an adjacent Pin Box.
 *
 *
 *          <---- cxPinBox --->
 *        ^ +-----------------+
 *        | |     ^           |
 *        | |     +- cyPinOffset
 *          |     V           |
 *        c |    /-\  /-\     |
 *        y |    \-/  \-/     |
 *        P |            ^+cyPinSpace
 *        i |            V    | --
 *        n |    /-\  /-\     |  ^--cyPin
 *        B |<+->\-/  \-/     |  V
 *        o | |     <>        | --
 *        x | |      +cxPinSpace
 *          | |               |
 *        | | cxPinOffset     |
 *        | |                 |
 *        V +-----------------+
 *                   |   |
 *                   |<+>|
 *                     |
 *
 *                   cxPin
 *
 *
 *      Algorithmic Relationship Between Dimensions
 *      ===========================================
 *
 *      nPeg = 4 = number of pegs in a guess
 *      nColor = 6 = number of different colored pegs
 *
 *      cxPegBox = x width of a Peg Box
 *      cyPegBox = y width of a Peg Box
 *      cxPeg    = x width of a Peg
 *      cyPeg    = y width of a Peg
 *      cxPegOffset = x distance between edge of Peg Box and Peg
 *      cyPegOffset = y distance between edge of Peg Box and Peg
 *
 *      cxMove = nPeg * cxPegBox = x width of Move Area
 *      cyMove = maxMove * cyPegBox = y height of Move Area
 *      cxResult = x width of Result Area
 *      cyResult = cyMove = y height of Result Area
 *      cxWell = cxPegBox = x width of Well Area
 *      cyWell = nColor * cyPegBox = y height of Well Area
 *      cyBorder = y width of top and bottom border
 *      cxBorder = x width of left and right border
 *      cxButton = width of widest button (computed at run-time)
 *
 *      cxMoveToResult = cxWell - (cxMove + cxResult) = x width between
 *                                                       Move and Result Areas
 *      cxWellToButton = x width between Well Area and Buttons
 *
 *      cyWellToMove = y height between Well and Move Areas
 *
 *      cxClient = 2*cxBorder + cxWell + cxWellToButton + cxButton
 *               = x width of client area
 *
 *	cyClient = 2*cyBorder + cyWell + cyWellToMove + cyMove +
 *		      cyMoveToAnswer + cyAnswer
 *               = y height of client area
 *
 *
 *      Notes:
 *
 *      (1) The Well Area is positioned over the Move and Result Areas
 *      (2) The Move and Result Areas are centered under the Well Area
 *      (2) The Buttons are postioned to the right of the Result Area
 *
 *      "Peg Box" - The bounding rectangle of a "Peg".  This is used to
 *              to lay out the "Move Area" and "Well Area".
 *
 *      "Well Area" - The area where one peg of each color is displayed.  The
 *              user drags a peg from this "well" into the "Move Area", and
 *              over the "Active Row" to place a peg for a "Guess".
 *
 *      "Move Area" - The area where the player builds Guesses.
 *
 *      "Result Area" - The area where the results of a players guesses are
 *              displayed.  A *black* "Pin" is displayed for each Peg in the
 *              Guess that matches a Peg in the computer's "Goal" in both
 *              color and position.  A *white* Pin is displayed for each Peg
 *              in the Guess that matches a Peg in the computer's Goal in
 *              color only.
 *
 *	"Fun Area" - The area where visual indication of game won/lost occurs.
 *
 *	"Answer Area" - The area where the computer code is revealed.
 *
 *
 *      Performance Notes
 *      =================
 *      (1) We create a Memory DC to hold a bitmap that contains images of
 *          each peg, the peg hole, and all combinations of pin results.
 *          Then, when painting the board, we do a BitBlt(...,SRC_COPY,...)
 *          from the memory DC to the the screen DC!
 *
 *          Since we cleverly made <cxPegBox,cyPegBox> be the same size as
 *          <cxPinBox,cyPinBox>, all we need is (conceptually) an array of
 *          images of this size in a single bitmap.
 *
 *          We need to construct the following images in the bitmap:
 *
 *          Count   Description of Image(s)
 *          ------  --------------------------------------------------------
 *          nColor  Colored Pegs
 *          1       Peg Hole
 *          15      All possible Result Pin patterns (see below)
 *
 *          Result Pin Patterns
 *          -------------------
 *              The tricky thing here is to pack these images tightly into
 *          the bitmap, so we need a mapping from (cPos,cClr) to [0..14]:
 *
 *               i  Pos  Clr
 *              --- ---  ---
 *               0   0    0
 *               1   0    1
 *               2   0    2
 *               3   0    3
 *               4   0    4
 *               5   1    0
 *               6   1    1
 *               7   1    2
 *               8   1    3
 *               9   2    0
 *              10   2    1
 *              11   2    2
 *              12   3    0
 *              13   3    1
 *              14   4    0
 *
 *          The solution is a two-dimensional array, mpResultToLibrary,
 *          indexed by cPos and cClr.  We build this array as we build
 *          the images in the image bitmap.  For valid combinations, we
 *          store the index of the image in the Bitmap into the array.
 *          For invalid combinations, the array entry is set to -1.
 *
 *      (2) A Performance vs. Size Tradeoff in the organization of the
 *          Image Library Bitmap.
 *
 *          We can either construct this as a horizontal vector of images,
 *          or a vertical vector of images.
 *
 *          A horizontal organization is likely to consume less memory, since
 *          the bitmap will (we presume) only waste space at the end of a
 *          row -- and there will only be *cxPegBox* rows.  However, this
 *          is could lead to non-byte-aligned BitBlts, which are slower.
 *
 *          A vertical organization, on the other hand, ensures that at least
 *          the source bitmap is aligned on a byte boundary.  The cost is that
 *          we may waste some bytes at the end of each pixel row.  Since we
 *          are using BitBlt for speed, we will continue to follow that maxim
 *          here.
 */

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mm.h"


/**************
 *** Macros *****************************************************************
 **************/

#define MIR(x)  MAKEINTRESOURCE(x)  // Make DialogBox lines shorter

#define AssertMsg(x)        // BUGBUG - Enable this for DEBUG build


/*****************
 *** Constants **************************************************************
 *****************/

#define cbMaxString   100       // Length of longest string resource

#define nColor          6       // Number of colors
#define nPeg            4       // Number of pegs per move
#define maxMove        10       // Maximum number of moves

#define nPin            2       // Number of result pin colors

#define cxPegBox       24               // x width of a Peg Box
#define cyPegBox       24               // y height of a Peg Box

#define cxPeg          16               // x width of a Peg
#define cyPeg          16               // y height of a Peg

#define cxPegOffset ((cxPegBox-cxPeg)/2) // x offset of Peg in Peg Box
#define cyPegOffset ((cyPegBox-cyPeg)/2) // y offset of Peg in Peg Box

#define cxPegHole       8               // x width of a Peg Hole
#define cyPegHole       8               // y height of a Peg Hole

#define cxPegHoleOffset ((cxPegBox-cxPegHole)/2) // x offset of Peg Hole
#define cyPegHoleOffset ((cyPegBox-cyPegHole)/2) // y offset of Peg Hole

#define cxPinBox    (cxPegBox)          // x width of a Pin Box
#define cyPinBox    (cyPegBox)          // y width of a Pin Box

#define cxPin           6               // x width of a Pin
#define cyPin           6               // y width of a Pin

#define cxPinSpace  (cxPin/2)           // x distance between Pins in Pin Box
#define cyPinSpace  (cyPin/2)           // y distance between Pins in Pin Box

#define cxPinOffset ((cxPinBox-2*cxPin-cxPinSpace)/2) // x offset of Pin
#define cyPinOffset ((cyPinBox-2*cyPin-cyPinSpace)/2) // y offset of Pin


#define cxImageBoxOffset  1         // x border around image
#define cyImageBoxOffset  1         // y border around image

#define cxImageBox  (cxPegBox - 2*cxImageBoxOffset)
#define cyImageBox  (cyPegBox - 2*cyImageBoxOffset)

#define nLibraryImage (nColor+1+15) // Colors + Peg Hole + Result Pin Patterns
				    // See "Performance Notes (1)" above.
#define cxLibrary   (cxImageBox)          // Width of Image Library Bitmap
#define cyLibrary   (cyImageBox*nLibraryImage) // Height of Image Library Bitmap


#define cxMove      (nPeg*cxPegBox)     // x width of Move Area
#define cyMove      (maxMove*cyPegBox)  // y height of Move Area

#define cyBorder        8               // y width of client border
#define cxBorder        8               // x width of client border

#define cxResult    (cxPinBox)          // x width of Result Area
#define cyResult    (cyMove)            // y height of Result Area

#define cxWell      (nColor*cxPegBox)   // x width of Well Area
#define cyWell      (cyPegBox)          // y height of Well Area

#define cxMoveToResult    8             // x width between Move and Result Area
#define cxWellToButton    8             // x width between Result Area & Buttons

#define cyWellToMove      8             // y height between Well and Move Area
#define cyMoveToAnswer	  8		// y height between Move and Answer Area

#define xWell   (cxBorder)              // x left of Well Area
#define xMove   (xWell + (cxWell - (cxMove+cxResult+cxMoveToResult))/2)
#define xResult (xMove+cxMove+cxMoveToResult) // x left of Result Area
#define xButton (xWell+cxWell+cxWellToButton) // x left of Buttons

#define yWell   (cyBorder)              // y top of Well Area
#define yMove   (yWell+cyWell+cyWellToMove)  // y top of Move Area
#define yResult (yMove)                 // y top of Result Area
#define yButton (cyBorder)              // y top of Buttons

#define xAnswer  (xMove)
#define yAnswer  (yMove + cyMove + cyMoveToAnswer)
#define cxAnswer (cxMove)
#define cyAnswer (cyPegBox)

/*
 *  Rectangle to bound cursor while dragging a peg
 */
#define xCursorLeft     xWell
#define yCursorTop      yWell
#define xCursorRight    (xWell+cxWell)
#define yCursorBottom   (yMove+cyMove)


/************************
 *** Type Definitions *******************************************************
 ************************/

typedef int PEG; /* peg */  // Peg value
#define PEG_BLANK   (nColor)    // PEG color of empty peg box


typedef PEG GUESS[nPeg]; /* guess */

typedef struct _MOVE {  /* mv */
    GUESS   guess;          // The guess
    int     cPosition;      // Pegs that match both position and color
    int     cColor;         // Pegs that match color but not position
} MOVE, *PMOVE;

typedef struct _GLOBAL { /* g */  // Global Variables
    HWND    hwnd;               // Client window
    int     cxMain;		// X width of main window
    int     cyMain;		// Y width of main window
    int     cxClient;           // X width of client area of window
    int     cyClient;           // Y width of client area of window
    int     cxButton;           // X width of widest button
    int     cyButton;           // Y height of tallest button
    int     cyButtonSpace;      // Y separation between buttons
    int     xFun;		// X of Fun area
    int     yFun;		// Y of Fun area
    int     cxFun;		// X width of Fun area
    int     cyFun;		// Y height of Fun area
    GUESS   guessCode;          // The code
    int     iMove;              // Current move index
    PEG     pegMove;            // Peg value being dragged
    MOVE    amove[maxMove];     // The game history
    HANDLE  hInstance;          // App instance handle
    FARPROC lpfnAboutDlgProc;   // About DlgProc instance function pointer
    BOOL    fGameOver;		// TRUE => current game is over
    BOOL    fGameWon;		// TRUE => player won current game
    BOOL    fGuessAllowed;	// TRUE => guess button enabled
    HDC     hdcLibrary;         // HDC for Image Library
    HBITMAP hbmLibrary;         // HBM for Image Library
    HCURSOR hcurCurrent;        // Current cursor
    HCURSOR hcurDefault;        // Default cursor
    HCURSOR hcurOverWell;       // Cursor when over Well, but not dragging
    HCURSOR hcurDrag;           // Cursor during Drag
    HCURSOR hcurDragOver;       // Cursor at Drag Over Move row in Move Area
    HFONT   hfntButton; 	// Button font
} GLOBAL,*PGLOBAL;


// Where is the Mouse?
typedef enum {WH_NOTOURS,WH_WELL, WH_MOVE} WHERE; /* wh */


// BUTTON - Information used to create a button in the client area.
typedef struct tagButtonDescription { /* btn */
    int     ids;    // String ID in resource file
    int     idc;    // Command ID
    long    style;  // Button Style
    HWND    hwnd;   // Button window handle
} BUTTON, *PBUTTON;


/*****************
 *** Variables **************************************************************
 *****************/

GLOBAL g;   // Globals

DWORD   clrPeg[nColor] = {  // Maps PEG value to screen color
    CLR_BLACK,
    CLR_BLUE,
    CLR_GREEN,
    CLR_YELLOW,
    CLR_RED,
    CLR_WHITE,
    };

// Brush indicies for painting pins
int aPinColor[nPin] = {
    BLACK_BRUSH,    // Match Position and Color
    WHITE_BRUSH,    // Match Color but not Position
};

// Client-Area button definitions
BUTTON abutton[] = {
  /* string id     control ID    style          hwnd */
  /* ---------     ------------  -------------  ---- */
    {IDS_NEW_GAME, IDC_NEW_GAME, BS_PUSHBUTTON, NULL},
    {IDS_GUESS,    IDC_GUESS   , BS_PUSHBUTTON, NULL},
};
#define nButton (sizeof(abutton)/sizeof(BUTTON))

// Button indicies
#define iButtonNewGame  0
#define iButtonGuess    1


// mpResultToImage - This array is used to find the index in the Image
//      Library Bitmap of a particular Result.  The first index is the
//      cPosition, the second index is the cColor.
//
//      CreateImageLibrary initializes this array.

int mpResultToLibrary[nPeg+1][nPeg+1];


/***************************
 *** Function Prototypes ****************************************************
 ***************************/

int PASCAL WinMain(HANDLE hInstance,
		   HANDLE hPrevInstance,
		   LPSTR lpszCmdLine,
		   int nCmdShow);

BOOL FAR PASCAL AboutDlgProc(HWND hDlg,UINT msg,UINT wParam,LONG lParam);
long FAR PASCAL WndProc(HWND hDlg,UINT msg,UINT wParam,LONG lParam);

BOOL   BeginMM(HANDLE hInstance,HANDLE hPrevInstance);
VOID   CreateButtons(HWND hwnd);
VOID   CreateImageLibrary(HDC hdcDisplay);
VOID   DestroyButtons(VOID);
VOID   DoCommand(HWND hwnd,UINT wParam,LONG lParam);
BOOL   DoMouse(HWND hwnd,UINT msg,UINT wParam,LONG lParam);
VOID   EndMM(VOID);
VOID   EraseForNewGame(HWND hwnd);
VOID   FastSetCursor(HCURSOR hcur);
BOOL   MouseInArea(int xM,int yM,int x,int y,int cx,int cy);
VOID   NewGame(VOID);
BOOL   QueryResignGame(HWND hwnd);
VOID   PaintAnswer(HWND hwnd);
VOID   PaintAnswerSub(HDC hdc);
VOID   PaintBoard(HDC hdc);
VOID   PaintHolesForPegs(HWND hwnd);
VOID   PaintLibrary(HDC hdc, int x, int y, int iLibrary);
VOID   PaintPeg(HWND hwnd, int ix, int iy);
VOID   PaintPegSub(HWND hwnd, int ix, int iy);
VOID   PaintResult(HWND hwnd);
VOID   PaintResultSub(HDC hdc, int iMove);
VOID   PickCode(VOID);
VOID   PlayerLost(HWND hwnd);
VOID   PlayerLostSub(HDC hdc);
VOID   PlayerTextOut(HDC hdc, char *psz);
VOID   PlayerWon(HWND hwnd);
VOID   PlayerWonSub(HDC hdc);
VOID   Randomize(VOID);
VOID   ReleaseMouse(VOID);
VOID   SetMouse(HWND hwnd);
BOOL   TestGuess(VOID);


/***************
 *** WinMain ****************************************************************
 ***************/

int PASCAL WinMain(HANDLE hInstance,
		   HANDLE hPrevInstance,
		   LPSTR lpszCmdLine,
		   int nCmdShow)
{
    MSG         msg;

    if (!BeginMM(hInstance,hPrevInstance))
	exit(1);

    //  Show window

    ShowWindow(g.hwnd,nCmdShow);
    UpdateWindow(g.hwnd);

    while(GetMessage(&msg,NULL,0,0))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}

    EndMM();

    return msg.wParam;
}


/************************************************
 *** Functions - Listed in Alphabetical Order *******************************
 ************************************************/

/***    AboutDlgProc - About Dialog Box Procedure
 *
 */
BOOL FAR PASCAL AboutDlgProc(HWND hdlg,UINT msg,UINT wParam,LONG lParam)
{
    switch (msg)
	{
	case WM_INITDIALOG:
	    return TRUE;

	case WM_COMMAND:
	    switch (wParam)
		{
		case IDOK:
		case IDCANCEL:
		    EndDialog(hdlg, 0);
		    return TRUE;
		}
	    break;
	}
    return FALSE;
}


/***    BeginMM - Initialize MasterMind
 *
 */
BOOL BeginMM(HANDLE hInstance,HANDLE hPrevInstance)
{
    char        ach[cbMaxString];
    int         cch;
    int         cchButton;
    int         cxChar;
    int         cyChar;
    HDC         hdc;
    HFONT	hfnt;
    int         i;
    long        lStyle;
    static char szAppName [] = "MastMind";
    TEXTMETRIC  tm;
    WNDCLASS    wndclass;

    // Randomize number generator
    Randomize();

    // Save hInstance
    g.hInstance = hInstance;

    // Load Cursors

    g.hcurDefault  = LoadCursor(NULL,IDC_ARROW);
    g.hcurCurrent  = g.hcurDefault;  // Assume current is default
    g.hcurOverWell = LoadCursor(g.hInstance,MIR(CUR_OVER_WELL));
    g.hcurDrag     = LoadCursor(g.hInstance,MIR(CUR_DRAG));
    g.hcurDragOver = LoadCursor(g.hInstance,MIR(CUR_DRAG_OVER));

    // Register window class, if necessary
    if (!hPrevInstance)
	{
	wndclass.style	       = CS_DBLCLKS;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = g.hInstance;
	wndclass.hIcon         = LoadIcon(hInstance, szAppName);
	wndclass.hCursor       = NULL;  // We want total control of cursor
	wndclass.hbrBackground = COLOR_APPWORKSPACE+1;
	wndclass.lpszMenuName  = szAppName;
	wndclass.lpszClassName = szAppName;

	RegisterClass(&wndclass);
	}

    // Get system font metrics (to size buttons)
    hdc = CreateDC("DISPLAY",NULL,NULL,NULL);

    // Create Image Library DC and Bitmap

    CreateImageLibrary(hdc);

    // Get button font

    g.hfntButton = GetStockObject(SYSTEM_FONT);

    if (g.hfntButton == 0) {
	// BUGBUG bens 01-Jul-1991 Create Font failed
    }

    // Get Font Metrics for sizing buttons

    hfnt = SelectObject(hdc,g.hfntButton);
    GetTextMetrics(hdc, &tm);
    cxChar = tm.tmAveCharWidth;
    cyChar = tm.tmHeight + tm.tmExternalLeading;

    // Done with Display DC

    SelectObject(hdc,hfnt);         // Restore original font
    DeleteDC(hdc);


    // Compute button dimensions

    cchButton = 0;
    for (i=0; i<nButton; i++) { // Get length of longest string
	cch = LoadString(g.hInstance,abutton[i].ids,ach,sizeof(ach));
	cchButton = max(cch,cchButton);
    }

    g.cxButton = (cchButton+4)*cxChar; // Give 2 char room on either side
    g.cyButton = (7 * cyChar) / 4;  // BUGBUG - Magic number from petzold ex.
    g.cyButtonSpace = g.cyButton/2;     // Button Spacing


    // Compute size of client area and main window

    g.cxClient = 2*cxBorder + cxWell + cxWellToButton + g.cxButton;

    g.cyClient = 2*cyBorder + cyWell +
		   cyWellToMove + cyMove +
		   cyMoveToAnswer + cyAnswer;

    g.cxMain   = g.cxClient +
		 GetSystemMetrics(SM_CXBORDER);

    g.cyMain   = g.cyClient +
		 GetSystemMetrics(SM_CYBORDER) +
		 GetSystemMetrics(SM_CYCAPTION) +
		 GetSystemMetrics(SM_CYMENU);


    // Compute Fun area dimensions

    g.xFun  = xButton;
    g.yFun  = yButton + 2*(g.cyButton+g.cyButtonSpace);
    g.cxFun = g.cxButton;
    g.cyFun = g.cyClient - (g.yFun+cyBorder);


    // Create main window

    LoadString(g.hInstance,IDS_APP_TITLE,ach,sizeof(ach));
    lStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    g.hwnd = CreateWindow(
		szAppName,          // Window class
		ach,                // Window Title
		lStyle,             // Window Style
		CW_USEDEFAULT,      // Left edge of window
		CW_USEDEFAULT,      // Top edge of window
		g.cxMain,	    // Width of window
		g.cyMain,	    // Height of window
		NULL,               // Parent window
		NULL,               // Control ID
		hInstance,          // App instance
		NULL                // lpCreateStruct
	     );

    // Create dialog procedure instance(s)
    g.lpfnAboutDlgProc = MakeProcInstance(AboutDlgProc,g.hInstance);

    return TRUE;
}


/***    CreateButtons - Create buttons in client area
 *
 */
VOID CreateButtons(HWND hwnd)
{
    char        ach[cbMaxString];
    HFONT	hfnt;
    int         i;
    long        lStyle;
    int         x,y;

    hfnt = GetStockObject(ANSI_VAR_FONT);   // Font for button text

    x = xButton;
    y = yButton;

    for (i=0; i<nButton; i++) {
	LoadString(g.hInstance,abutton[i].ids,ach,sizeof(ach));
	lStyle = WS_CHILD | abutton[i].style;
	abutton[i].hwnd = CreateWindow(
	    "button",           // Window class
	    ach,                // Button text
	    lStyle,             // Button style;
	    x,                  // Left edge of button
	    y,                  // Top edge of button
	    g.cxButton,         // Width of button
	    g.cyButton,         // Height of button
	    hwnd,               // Parent window (gets BM_ messages)
	    abutton[i].idc,     // Button Control ID
	    g.hInstance,        // App instance
	    NULL);              // lpCreateStruct
	y += (g.cyButton + g.cyButtonSpace);  // Advance button position

	// Set button font
	SendMessage(abutton[i].hwnd,WM_SETFONT,g.hfntButton,FALSE);

	// Show button
	ShowWindow(abutton[i].hwnd,SW_SHOWNORMAL);
    }
}


/***    CreateImageLibrary - Create bitmap with all peg and pin images
 *
 *      Entry
 *          hdc - Display DC
 */
VOID CreateImageLibrary(HDC hdcDisplay)
{
    int     acPins[2];
    int     cPinsPainted;
    HPEN    hpenSave;
    HBRUSH  hbrush;
    HBRUSH  hbrushLast;
    HBRUSH  hbrushSave;
    HDC     hdc;
    int     i;
    int     iClr;
    int     iLoop;
    int     iPinBox;
    int     iPos;
    int     x1,y1,x2,y2;

    // Create Image Library Bitmap -- Same color depth as display!
    g.hbmLibrary = CreateCompatibleBitmap(hdcDisplay,cxLibrary,cyLibrary);

    // Create the memory DC
    hdc = CreateCompatibleDC(hdcDisplay); // Image Library DC
    g.hdcLibrary = hdc;                 // Save so we can free it at exit

    SelectObject(hdc,g.hbmLibrary);     // Select bitmap into DC

    // Set desired pen/brush and save default pen/brush

    hpenSave = SelectObject(hdc,GetStockObject(BLACK_PEN));
    hbrushSave = SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));

    // Fill bitmap with background color (selected brush above!)

    PatBlt(hdc,0,0,cxLibrary,cyLibrary,PATCOPY);

    // Draw Color Pegs

    x1 = cxPegOffset - cxImageBoxOffset;
    x2 = x1+cxPeg;

    y1 = cyPegOffset - cyImageBoxOffset;
    for (i=0; i<nColor; i++) {
	y2 = y1+cyPeg;
	hbrush = CreateSolidBrush(clrPeg[i]); // Get brush for peg color
	hbrushLast = SelectObject(hdc,hbrush);
	if (i > 0)                      // Have a brush from last iteration
	    DeleteObject(hbrushLast);   // Delete it!
	Ellipse(hdc,x1,y1,x2,y2);
	y1 += cyImageBox;
    }

    // Now, we have to delete the brush we created in the final iteration.
    // So, we select in the brush we need for drawing the peg hole, and then
    // we are able to delete that last brush we created!

    hbrush = SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));
    DeleteObject(hbrush);               // Delete brush from final iteration

    //
    // Draw Peg Hole
    //

    x1 = cxPegHoleOffset - cxImageBoxOffset;
    x2 = x1 + cxPegHole;

    y1 = nColor*cyImageBox + cyPegHoleOffset - cyImageBoxOffset;
    y2 = y1 + cyPegHole;

    Ellipse(hdc,x1,y1,x2,y2);

    //
    // Draw all valid combinations of Result Pin Patterns
    //

    iPinBox = nColor + 1;               // Index of first pin box in Library
    for (iPos=0; iPos<=nPeg; iPos++) {
	for (iClr=0; iClr<=nPeg; iClr++) {
	    if ((iPos+iClr) > nPeg) {   // Invalid combination
		mpResultToLibrary[iPos][iClr] = -1; // No image index
		continue;               // Do next iteration of for loop!
	    }

	    mpResultToLibrary[iPos][iClr] = iPinBox; // Set image index

	    acPins[0] = iPos;           // Set count of positions for painting
	    acPins[1] = iClr;           // Set count of colors for painting

	    cPinsPainted = 0;           // No pins painted in this box, yet

	    x1 = cxPinOffset - cxImageBoxOffset;
	    y1 = iPinBox*cyImageBox + cyPinOffset - cyImageBoxOffset;

	    // Paint black, then white pins
	    for (iLoop=0; iLoop<nPin; iLoop++) {
		// Select color
		SelectObject(hdc,GetStockObject(aPinColor[iLoop]));

		// Paint pins of this color
		for (i=0; i<acPins[iLoop]; i++) {
		    x2 = x1 + cxPin;
		    y2 = y1 + cyPin;
		    Ellipse(hdc,x1,y1,x2,y2); // Draw pin
		    x1 = x2 + cxPinSpace;     // x for second pin
		    cPinsPainted++;
		    if (cPinsPainted == 2) {  // Wrap to second row of pins
			x1 = cxPinOffset - cxImageBoxOffset; // x of third pin
			y1 = y2 + cyPinSpace; // y for third and fourth pins
		    }
		}
	    }
	    iPinBox++;                  // Count Result Pin Pattern
	}
    }

    //
    // Set restore original pen/brush
    //

    SelectObject(hdc,hpenSave);
    SelectObject(hdc,hbrushSave);
}


/***    DestroyButtons - Destroy buttons in client area
 *
 */
VOID DestroyButtons()
{
    int         i;

    for (i=0; i<nButton; i++) {
	DestroyWindow(abutton[i].hwnd);
#ifdef DEBUG
	abutton[i].hwnd = NULL;
#endif
    }
}


/***    DoCommand - Handle COMMAND messages
 *
 */
VOID DoCommand(HWND hwnd,UINT wParam,LONG lParam)
{
    BOOL        f;

    switch (wParam) {
	case IDM_ABOUT:
	    DialogBox(g.hInstance,MIR(IDD_ABOUT),hwnd,g.lpfnAboutDlgProc);
	    return;

	case IDC_GUESS:
	    f = TestGuess();
	    PaintResult(hwnd);  // Show result

	    // Disable Guess button
	    EnableWindow(abutton[iButtonGuess].hwnd,FALSE);
	    g.fGuessAllowed = FALSE;

	    if (f) {    // Guess is correct
		PlayerWon(hwnd); // Indicate a win

		// Show answer
		PaintAnswer(hwnd);

		// Game won, disable play until New Game selected
		g.fGameOver = TRUE;
		g.fGameWon = TRUE;
	    }
	    else {  // Guess is not correct
		g.iMove++;  // Advance to next row
		if (g.iMove >= maxMove) { // Used up all guesses

		    // Reset move index to last row, to keep PaintBoard
		    // from crashing
		    g.iMove = maxMove - 1;

		    // Indicate the loss
		    PlayerLost(hwnd);

		    // Show answer
		    PaintAnswer(hwnd);

		    // Disable play until New Game selected
		    g.fGameOver = TRUE;
		    g.fGameWon = FALSE;
		}
		else {  // Player still has more guess(s) to make
		    PaintHolesForPegs(hwnd); // Show where next moves go
		}
	    }
	    return;

	case IDC_NEW_GAME:
	    if ( (g.iMove > 0) && (!g.fGameOver) ) { // Is game in progress?
		// Verify that user wants to resign current game
		if (QueryResignGame(hwnd)) {
		    // Yes, user wants to resign.

		    // Indicate loss
		    PlayerLost(hwnd);

		    // Show answer
		    PaintAnswer(hwnd);

		    // Disable play until New Game selected
		    g.fGameOver = TRUE;
		    g.fGameWon = FALSE;
		}
		else // No, user does not want to resign
		    return; // Ignore command
	    }
	    else {  // No game underway, do new game
		NewGame();  // New game
		EraseForNewGame(hwnd);
	    }
	    return;
    }
}


/***    DoMouse - Handle mouse messages
 *
 *	Entry
 *	    Mouse message
 *
 *	Exit
 *	    Return TRUE if we handled message
 *	    Return FALSE if caller should handle message
 */
BOOL DoMouse(HWND hwnd,UINT msg,UINT wParam,LONG lParam)
{
    int     i;
    int     iWell;
    int     ixMove,iyMove;
    int     j;
    WHERE   wh;
    int     x,y;

    static  BOOL    fDragging=FALSE;    // TRUE ==> Peg being dragged
    static  int     ixMoveSrc,iyMoveSrc; // Source of copy/drag in Move Area
    static  PEG     pegColor;           // Color of peg being moved
    static  WHERE   whSource;           // Where Peg came from during drag

    // Get mouse coordinates

    x = LOWORD(lParam);
    y = HIWORD(lParam);

    // Figure out where mouse is

    if (MouseInArea(x,y,xWell,yWell,cxWell,cyWell)) {
	wh = WH_WELL;	// Mouse in Well Area
	iWell = (x-xWell)/cxPegBox; // Color selected
    }
    else if (MouseInArea(x,y,xMove,yMove,cxMove,cyMove)) {
	wh = WH_MOVE;	// Mouse in Move Area
	ixMove = (x-xMove)/cxPegBox; // Column selected
	iyMove = (y-yMove)/cyPegBox; // Row selected
    }
    else {
	wh = WH_NOTOURS;// Mouse is someplace else
    }

    // Now handle mouse message

    switch (wh) {

    case WH_NOTOURS:
	switch (msg) {
	case WM_MOUSEMOVE:
	    if (fDragging)
		FastSetCursor(g.hcurDrag);
	    else
		FastSetCursor(g.hcurDefault);
	    return TRUE;
	    break;

	case WM_LBUTTONUP:
	    if (fDragging) {
		fDragging = FALSE;
		ReleaseMouse();
		FastSetCursor(g.hcurDefault);
	    }
	    else
		FastSetCursor(g.hcurDefault);
	    return TRUE;            // We handled message
	}
	break;

    case WH_WELL:
	switch (msg) {
	case WM_MOUSEMOVE:
	    if (fDragging)
		FastSetCursor(g.hcurDrag);
	    else
		FastSetCursor(g.hcurOverWell);
	    return TRUE;

	case WM_LBUTTONDOWN:
	    pegColor = iWell;           // Record color
	    fDragging = TRUE;           // Note that we are dragging
	    whSource = WH_WELL;         // It came from Well Area
	    SetMouse(hwnd);             // Get that mouse
	    FastSetCursor(g.hcurDrag);  // Change cursor
	    return TRUE;                // We handled message

	case WM_LBUTTONUP:
	    if (fDragging) {
		fDragging = FALSE;
		ReleaseMouse();
	    }
	    FastSetCursor(g.hcurOverWell);
	    return TRUE;                // We handled message
	}
	break;

    case WH_MOVE:
	switch (msg) {
	case WM_MOUSEMOVE:
	    if (fDragging) {
		if (iyMove == g.iMove)  // Over possible drop target
		    FastSetCursor(g.hcurDragOver);
		else
		    FastSetCursor(g.hcurDrag);
	    }
	    else if (g.amove[iyMove].guess[ixMove] != PEG_BLANK)
		FastSetCursor(g.hcurOverWell); // Player can move/copy this peg
	    else
		FastSetCursor(g.hcurDefault);
	    return TRUE;

	case WM_LBUTTONDOWN:
	    pegColor = g.amove[iyMove].guess[ixMove]; // Record color
	    if (pegColor == PEG_BLANK)  // No peg in that position
		return TRUE;            // We handled message

	    ixMoveSrc = ixMove;         // Keep track of source of drag/copy
	    iyMoveSrc = iyMove;

	    fDragging = TRUE;           // We are dragging
	    whSource = WH_MOVE;         // It came from Move Area
	    SetMouse(hwnd);             // Get that mouse
	    FastSetCursor(g.hcurDrag);  // Set cursor
	    return TRUE;                // We handled message

	case WM_LBUTTONDBLCLK:
	    if (iyMove == g.iMove) {	// Double Click occured on play row
		// Check for first move
		if (g.iMove == 0)	// If this is the first row
		    return TRUE;	//  Do nothing

		// Copy move down from previous row
		for (i=0; i<nPeg; i++)
		    g.amove[iyMove].guess[i] = g.amove[iyMove-1].guess[i];

		// Paint pegs we just moved
		// BUGBUG 03-Jul-1991 bens Would be faster to create DC once
		for (i=0; i<nPeg; i++) {
		    PaintPeg(hwnd,i,g.iMove);
		}

		// Turn on Guess button
		EnableWindow(abutton[iButtonGuess].hwnd,TRUE);
		g.fGuessAllowed = TRUE;

		// NOTE: The button up message will change the cursor, so
		//	 we do not do it here.
	    }
	    return TRUE;

	case WM_LBUTTONUP:
	    if (fDragging) {
		fDragging = FALSE;      // No longer dragging
		ReleaseMouse();

		// Only do move if in active play row

		if (iyMove != g.iMove) { // Drop did not occur on play row
		    if (g.amove[iyMove].guess[ixMove] != PEG_BLANK)
			FastSetCursor(g.hcurOverWell); // Move/Copy valid
		    else
			FastSetCursor(g.hcurDefault);
		    return TRUE;        // We handled message
		}

		j = g.amove[iyMove].guess[ixMove]; // Remember destination

		// Set color of destination

		g.amove[iyMove].guess[ixMove] = pegColor; // dst = src
		PaintPeg(hwnd,ixMove,iyMove); // Paint Peg

		// See if we have to do a move/exchange

		if (whSource == WH_MOVE) {  // Do Move or Exchange
		    if (iyMoveSrc == iyMove) {  // Do exchange
			g.amove[iyMoveSrc].guess[ixMoveSrc] = j; // src = dst
			PaintPeg(hwnd,ixMoveSrc,iyMoveSrc);
		    }
		}

		// Finally, if all pegs are placed, enable Guess button
		j = 0;
		for (i=0; i<nPeg; i++) {    // Count number of pegs
		    if (g.amove[iyMove].guess[i] != PEG_BLANK) {
			j++;
		    }
		}
		if (j == nPeg) {
		    EnableWindow(abutton[iButtonGuess].hwnd,TRUE);
		    g.fGuessAllowed = TRUE;
		}
	    }
	    if (g.amove[iyMove].guess[ixMove] != PEG_BLANK)
		FastSetCursor(g.hcurOverWell); // Player can move/copy this peg
	    else
		FastSetCursor(g.hcurDefault);
	    return TRUE;                // We handled message
	}
	break;

    default:
	AssertMsg("Unexpected where value.");
    }

    // We did not handle message
    return FALSE;
}


/***    EndMM - End MasterMind
 *
 */
VOID EndMM(VOID)
{
    // Free dialog procedure instances

    FreeProcInstance(g.lpfnAboutDlgProc);

    // Free image library DC and Bitmap
    // BUGBUG - Is it okay to delete DC and then Bitmap?  This is how
    //          petzold does it.  This is different than everything other
    //          object selected into a DC, where you must first select
    //          back in the original object.

    DeleteDC(g.hdcLibrary);
    DeleteObject(g.hbmLibrary);

    // Free cursors

    // NOTE: Do not free g.hcurDefault -- it is a SYSTEM cursor!
    DestroyCursor(g.hcurOverWell);
    DestroyCursor(g.hcurDrag);
    DestroyCursor(g.hcurDragOver);
}


/***    EraseForNewGame - Erase Result and Move Areas
 *
 */
VOID EraseForNewGame(HWND hwnd)
{
    HBRUSH  hbrush;
    HDC     hdc;
    HPEN    hpen;
    int     ix;

    hdc = GetDC(hwnd);

    // Set desired pen/brush and save default pen/brush
    hpen = SelectObject(hdc,GetStockObject(BLACK_PEN));
    hbrush = SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));

    // Clear Result, Move, and Fun Areas
    Rectangle(hdc,xResult,yResult,xResult+cxResult,yResult+cyResult);
    Rectangle(hdc,xMove,yMove,xMove+cxMove,yMove+cyMove);
    Rectangle(hdc,xAnswer,yAnswer,xAnswer+cxAnswer,yAnswer+cyAnswer);
    Rectangle(hdc,g.xFun,g.yFun,g.xFun+g.cxFun,g.yFun+g.cyFun);

    // Paint Peg Holes in Move Area for 1st Move

    for (ix=0; ix<nPeg; ix++) {
	PaintPegSub(hdc,ix,0);
    }

    // Restore default pen and brush
    SelectObject(hdc,hpen);
    SelectObject(hdc,hbrush);

    ReleaseDC(hwnd,hdc);
}


/***    FastSetCursor
 *
 *      Only set cursor if current setting is different from new setting.
 */
VOID FastSetCursor(HCURSOR hcur)
{
    if (hcur != g.hcurCurrent) {
	SetCursor(hcur);
	g.hcurCurrent = hcur;
    }
}


/***	MouseInArea - Test if mouse coordinate is in rectangular area
 *
 *	Entry
 *	    xM,yM - mouse coordinate
 *	    x,y   - left upper corner of area
 *	    cx,cy - width and height of area
 *
 *	Exit
 *	    Returns TRUE if coordinate is in area.
 *	    Returns FALSE if coordinate is NOT in area.
 */
BOOL MouseInArea(int xM,int yM,int x,int y,int cx,int cy)
{
    if ((x <= xM) && (xM < (x+cx)) &&
	(y <= yM) && (yM < (y+cy)) )

	return TRUE;
    else
	return FALSE;
}


/***    NewGame - Start a new game
 *
 */
VOID NewGame(VOID)
{
    int ix;
    int iy;

    g.iMove = 0;
    for (iy=0; iy<maxMove; iy++) {
	for (ix=0; ix<nPeg; ix++) {
	    g.amove[iy].guess[ix] = PEG_BLANK;
	}
	g.amove[iy].cPosition = 0;
	g.amove[iy].cColor = 0;
    }

    // Pick a new code
    PickCode();

    // Disable Guess button
    EnableWindow(abutton[iButtonGuess].hwnd,FALSE);
    g.fGuessAllowed = FALSE;

    // Game not over
    g.fGameOver = FALSE;
    g.fGameWon = FALSE;

}


/***	QueryResignGame - See if player wants to resign game
 *
 *	Entry
 *	    hwnd - main window
 *
 *	Exit
 *	    Returns TRUE if player wants to resign.
 *	    Returns FALSE if player does NOT want to resign.
 */
BOOL QueryResignGame(HWND hwnd)
{
    char    achText[cbMaxString];
    char    achCaption[cbMaxString];
    int     i;

    LoadString(g.hInstance,IDS_RESIGN,achText,sizeof(achText));
    LoadString(g.hInstance,IDS_APP_TITLE,achCaption,sizeof(achCaption));
    i = MessageBox(hwnd,achText,achCaption,MB_ICONQUESTION | MB_YESNO);
    return (i == IDYES);
}


/***	PaintAnswer - Paint the answer on board
 *
 *      Entry
 *	    hwnd - hwnd of main window
 */
VOID PaintAnswer(HWND hwnd)
{
    HDC     hdc;

    hdc = GetDC(hwnd);
    PaintAnswerSub(hdc);
    ReleaseDC(hwnd,hdc);
}


/***	PaintAnswerSub - Paint the answer on board
 *
 *      Entry
 *          hdc - DC of client area
 */
VOID PaintAnswerSub(HDC hdc)
{
    int i;
    int x;

    x = xAnswer;
    for (i=0; i<nPeg; i++) {
	PaintLibrary(hdc,x,yAnswer,g.guessCode[i]); // Paint a peg
	x += cxPegBox;
    }
}


/***    PaintBoard - Paint the game board
 *
 *      Entry
 *          hdc - DC of client area
 */
VOID PaintBoard(HDC hdc)
{
    HBRUSH  hbrush;
    HPEN    hpen;
    int     i,ix,iy;
    int     x;

    // Set desired pen/brush and save default pen/brush

    hpen = SelectObject(hdc,GetStockObject(BLACK_PEN));
    hbrush = SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));

    // Draw Well, Move, Fun, and Result Areas

    Rectangle(hdc,xWell,yWell,xWell+cxWell,yWell+cyWell);
    Rectangle(hdc,xMove,yMove,xMove+cxMove,yMove+cyMove);
    Rectangle(hdc,xAnswer,yAnswer,xAnswer+cxAnswer,yAnswer+cyAnswer);
    Rectangle(hdc,g.xFun,g.yFun,g.xFun+g.cxFun,g.yFun+g.cyFun);
    Rectangle(hdc,xResult,yResult,xResult+cxResult,yResult+cyResult);

    // Draw pegs in Well Area

    x = xWell;
    for (i=0; i<nColor; i++) {
	PaintLibrary(hdc,x,yWell,i);  // Paint a peg
	x += cxPegBox;
    }

    // BUGBUG 02-Jul-1991 bens	Assert(g.iMove < maxMove);

    // Paint Moves

    for (iy=0; iy<=g.iMove; iy++) { // Paint pegs in move area
	for (ix=0; ix<nPeg; ix++) {
	    PaintPegSub(hdc,ix,iy);
	}
    }

    // Paint answer, if appropriate

    if (g.fGameOver) {
	PaintAnswerSub(hdc);
	if (g.fGameWon)
	    PlayerWonSub(hdc);
	else
	    PlayerLostSub(hdc);
    }


    // Paint Results

    for (i=0; i<=g.iMove; i++) {
	PaintResultSub(hdc,i);
    }

    // Restore default pen and brush

    SelectObject(hdc,hpen);
    SelectObject(hdc,hbrush);
}


/***    PaintLibrary - Paint an Image from the Image Library
 *
 *      Entry
 *          hdc - Destination of paint
 *          x   - Left
 *          y   - Top
 *          i   - Image index =
 *                  0..nColor-1         : Paint peg of color i
 *                  nColor              : Paint peg hole
 *                  nColor+1..nColor+15 : Paint a Result Pin Pattern
 *                                        (see mpResultToLibrary)
 */
VOID PaintLibrary(HDC hdc, int x, int y, int iLibrary)
{
    BitBlt(hdc,                 // Destination DC
	   x+cxImageBoxOffset,  // Destination x left
	   y+cyImageBoxOffset,  // Destination y top
	   cxImageBox,          // Width of BLT
	   cyImageBox,          // Height of BLT
	   g.hdcLibrary,        // Source DC
	   0,                   // Source x left
	   iLibrary*cyImageBox, // Source y top
	   SRCCOPY);            // RasterOp (Copy Src to Dst)
}


/***    PaintHolesForPegs - Paint Peg holes for Next Guess
 *
 */
VOID PaintHolesForPegs(HWND hwnd)
{
    HDC     hdc;
    int     ix;

    hdc = GetDC(hwnd);

    for (ix=0; ix<nPeg; ix++) {
	PaintPegSub(hdc,ix,g.iMove);
    }

    ReleaseDC(hwnd,hdc);
}


/***    PaintPeg - Paint a peg in the Move Area
 *
 *      Entry
 *          hwnd = client window
 *          ix   = Column in Move box
 *          iy   = Row in Move box
 */
VOID PaintPeg(HWND hwnd, int ix, int iy)
{
    HDC     hdc;

    hdc = GetDC(hwnd);
    PaintPegSub(hdc,ix,iy);             // Paint the peg
    ReleaseDC(hwnd,hdc);
}


/***    PaintPegSub - Worker routine to pain a Peg in the Move Area
 *
 *      Entry
 *          hdc = client window
 *          ix  = Column in Move box
 *          iy  = Row in Move box
 */
VOID PaintPegSub(HDC hdc, int ix, int iy)
{
    PEG     peg;
    int     x,y;

    peg = g.amove[iy].guess[ix];        // Peg value

    x = xMove + ix*cxPegBox;
    y = yMove + iy*cyPegBox;

    PaintLibrary(hdc,x,y,peg);          // Paint the peg (or peg hole)
}


/***    PaintResult - Paint a the results for a move
 *
 *      Entry
 *          hwnd = client window
 *          g.iMove = move to paint results for
 */
VOID PaintResult(HWND hwnd)
{
    HDC     hdc;

    hdc = GetDC(hwnd);
    PaintResultSub(hdc,g.iMove);        // Paint the result
    ReleaseDC(hwnd,hdc);
}


/***    PaintResultSub - Worker Routine to paint results for a move
 *
 *      Entry
 *          hdc   = client window DC
 *          iMove = move to paint result for
 *
 *      Exit
 *          returns hbrush of DC at entry.
 */
VOID PaintResultSub(HDC hdc, int iMove)
{
    int     iPos,iClr;
    int     x,y;

    x = xResult;
    y = yResult + iMove*cyPinBox;

    iPos = g.amove[iMove].cPosition;
    iClr = g.amove[iMove].cColor;

    PaintLibrary(hdc,x,y,mpResultToLibrary[iPos][iClr]);
}


/***	Randomize - Initialize random number generator
 *
 */
VOID Randomize(VOID)
{
    int     i;
    time_t  t;

    time(&t);

    i = (int)(t>>16);
    srand(i);   // Set seed of random number generator
}


/***    PickCode - Create a new Code
 *
 */
VOID PickCode(VOID)
{
    int     fColor[nColor]; // Keep track of colors already picked
    int     i;
    int     j;

    // Zero color flags
    for (i=0; i<nColor; i++) {
	fColor[i] = 0;
    }

    // Loop until we have filled in all the pegs
    i = 0;
    while (i < nPeg) {
	j = rand() % nColor;    // Pick a color
	if (fColor[j] == 0) {   // Color not already used, use it
	    g.guessCode[i] = j; // Set peg color
	    fColor[j] = 1;      // Remember we used this color
	    i++;                // Get color for next peg
	}
    }
}


/***	PlayerLost - Indicate visually that player lost
 *
 */
VOID PlayerLost(HWND hwnd)
{
    HDC     hdc;

    hdc = GetDC(hwnd);
    PlayerLostSub(hdc);
    ReleaseDC(hwnd,hdc);
}


/***	PlayerLostSub - Indicate visually that player lost
 *
 */
VOID   PlayerLostSub(HDC hdc)
{
    PlayerTextOut(hdc,"Loser!");
}


/***	PlayerTextOut - Indicate win/lose
 *
 */
VOID PlayerTextOut(HDC hdc, char *psz)
{
    HFONT   hfnt;
    int     nBkMode;
    int     x,y;

    hfnt = SelectObject(hdc,g.hfntButton);
    nBkMode = SetBkMode(hdc,TRANSPARENT);

    x = g.xFun + 8; // BUGBUG 02-Jul-1991 bens Hack positioning
    y = g.yFun + g.cyFun/2;

    TextOut(hdc,x,y,psz,strlen(psz));

    SetBkMode(hdc,nBkMode);	// Restore original background mode
    SelectObject(hdc,hfnt);	// Restore original font
}


/***	PlayerWon - Indicate visually that player won
 *
 */
VOID PlayerWon(HWND hwnd)
{
    HDC     hdc;

    hdc = GetDC(hwnd);
    PlayerWonSub(hdc);
    ReleaseDC(hwnd,hdc);
}


/***	PlayerWonSub - Indicate visually that player won
 *
 */
VOID   PlayerWonSub(HDC hdc)
{
    PlayerTextOut(hdc,"Winner!");
}


/***    ReleaseMouse - Release mouse capture and unrestrict mouse motion
 *
 */
VOID ReleaseMouse(VOID)
{
    ClipCursor(NULL);               // Restore mouse mobility
    ReleaseCapture();               // Release mouse capture
}


/***    SetMouse - Capture mouse and set ClipCursor area
 *
 *	Entry
 *	    hwnd - window handle of main window
 */
VOID SetMouse(HWND hwnd)
{
    RECT    rc;

    SetCapture(hwnd);               // Capture that mouse

    rc.left   = xCursorLeft;
    rc.top    = yCursorTop;
    rc.right  = xCursorRight;
    rc.bottom = yCursorBottom;

    ClientToScreen(hwnd,(NPPOINT)(&rc)); // Convert left,top coordinates
    ClientToScreen(hwnd,&(((NPPOINT)(&rc))[1])); // Convert right,bottom

    ClipCursor(&rc);                // Restrict mouse movements
}


/***    TestGuess - Test player guess against code
 *
 *      Entry   g.iMove = move index
 *              g.amove(g.iMove) = player guess
 *              g.guessCode = code
 *
 *      Exit    returns TRUE if guess matches code
 *              else returns FALSE
 *
 *              g.amove[g.iMove].cPosition filled in.
 *              g.amove[g.iMove].cColor filled in.
 */
BOOL TestGuess(VOID)
{
    int cColor = 0;             // Count of color-only matches
    int codeColors[nColor];     // Count of each color in code
    int cPosition = 0;          // Count of color-and-position matches
    int guessColors[nColor];    // Count of each color in player guess
    int i;
    int j;
    int n = g.iMove;            // Current move index

    // Zero color counts
    for (i=0; i<nColor; i++) {
	guessColors[i] = 0;
	codeColors[i] = 0;
    }

    // Count colors in guess and code
    for (i=0; i<nPeg; i++) {
	j = g.amove[n].guess[i];    // peg color
	guessColors[j]++;           // Count this peg color
	j = g.guessCode[i];         // peg color
	codeColors[j]++;            // Count this peg color
    }
    
    // Count exact matches
    for (i=0; i<nPeg; i++) {
	j = g.amove[n].guess[i];    // Get guess color
	if (j == g.guessCode[i]) {  // Exact match!
	    // Begin Assert
	    if (guessColors[j] <= 0) {  // counted wrong
		AssertMsg("Found exact match, but guessColors[] entry <= 0.");
	    }
	    if (codeColors[j] <= 0) {   // counted wrong
		AssertMsg("Found exact match, but codeColors[] entry <= 0.");
	    }
	    // End Assert
	    guessColors[j]--;       // Reduce color count in guess
	    codeColors[j]--;        // Reduce color count in goal
	    cPosition++;            // Count exact match
	}
    }

    // Count color matches that remain after exact matches
    for (i=0; i<nColor; i++) {
	cColor += min(codeColors[i],guessColors[i]);
    }

    // Store findings
    g.amove[n].cPosition = cPosition;
    g.amove[n].cColor    = cColor;

    // If correct postions same as number of pegs, we have a winner
    return (cPosition == nPeg);
}


/***    WndProc - Main Window Procedure
 *
 */
long FAR PASCAL WndProc(HWND hwnd,UINT msg,UINT wParam,LONG lParam)
    {
    HDC         hdc;
    PAINTSTRUCT ps;

    switch(msg) {
	case WM_CREATE:
	    CreateButtons(hwnd);
	    NewGame();  // Start a new game
	    return 0;

	case WM_KEYDOWN:
	    switch (wParam) {
		case VK_RETURN:
		    // See if guess is allowed
		    if (g.fGuessAllowed) {
			// Simulate push of Guess button
			DoCommand(hwnd,IDC_GUESS,0L);
			return 0;
		    }
		    break;
	    }
	    break;

	case WM_COMMAND:
	    DoCommand(hwnd,wParam,lParam);
	    return 0;

	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONUP:
	    // See if guess is allowed
	    if (g.fGuessAllowed) {
		// Simulate push of Guess button
		DoCommand(hwnd,IDC_GUESS,0L);
		return 0;
	    }
	    break;

	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

	    // If game is over, do not process mouse
	    if (g.fGameOver)
		break;	// Let Windows handle the message

	    if (DoMouse(hwnd,msg,wParam,lParam)) // We handled it
		return TRUE;    // Get out of here
	    break;  // Let Windows handle the message

	case WM_PAINT:
	    hdc = BeginPaint(hwnd, &ps);
	    PaintBoard(hdc);
	    EndPaint(hwnd,&ps);
	    return 0;

	case WM_DESTROY:
	    DestroyButtons();
	    PostQuitMessage(0);
	    return 0;
    }
    return DefWindowProc(hwnd,msg,wParam,lParam);
}
