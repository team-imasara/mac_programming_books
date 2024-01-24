// for Old codes
//#define THINK_C 1
//#define  OLDROUTINENAMES 1
//#define  TARGET_OS_MAC 1
//#define  TARGET_CPU_68K 1

#include <TargetConditionals.h>

//#define  CALL_NOT_IN_CARBON 0
//
//#define  TARGET_API_MAC_OS8 1
//#define  TARGET_API_MAC_CARBON 0
//#define  TARGET_API_MAC_OSX 0

#define  CALL_NOT_IN_CARBON 1
#include <LowMem.h>

/*
 *  ConditionalMacros.h 참조
 * */

#include <Packages.h>
#include <Gestalt.h>
#include <OSUtils.h>
#include <Sound.h>
#include <Quickdraw.h>
#include <Events.h>
#include <Fonts.h>
#include <Resources.h>
#include <Devices.h>
// Retro68
#include <ToolUtils.h>

#include <stdio.h>


#define kNumLines  50
#define kMoveToFront (WindowPtr)-1L
#define kRandomUpperLimit 0x8000
#define kEmptyString "\p"
#define kEmptyTitle  kEmptyString
#define kVisible  true
#define kNoGoAway false
#define kNilRefCon (long)nil

Rect gLines[kNumLines
);
short gDeltaTop = 3, gDeltaBottom = 3; /* These four are the key to FlyingLine!*/
short gDeltaLeft = 2, gDeltaRight = 6;
short gOldMBarHeight;


void ToolBoxInit(void);

void WindowInit(void);

void LinesInit(void);

void MainLoop(void);

void RandomRect(Rect *rectPtr);

short Randomize(short range);

void RecalcLine(short i);

void DrawLine(short i);


int main(void) {

//    printf("Hello, World\n");
    ToolBoxInit();
    WindowInit();
    LinesInit();
    MainLoop();

}

void ToolBoxInit(void) {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nil);
    InitCursor();
}


void WindowInit(void) {

    Rect totalRect, mBarRect;
    RgnHandle mBarRgn;
    WindowPtr window;
    /*
     http://preserve.mactech.com/articles/mactech/Vol.10/10.08/OddsandEnds/index.html
     */
    // MBarHeight = 0;
    gOldMBarHeight = LMGetMBarHeight(); //GetMBarHeight();
    LMSetMBarHeight(0);

    window = NewWindow(nil, &(qd.screenBits.bounds),
                       kEmptyTitle,
                       kVisible, plainDBox,
                       kMoveToFront,
                       kNoGoAway, kNilRefCon);

    SetRect(&mBarRect, qd.screenBits.bounds.left,
            qd.screenBits.bounds.top,
            qd.screenBits.bounds.right,
            qd.screenBits.bounds.top + gOldMBarHeight
    );

    mBarRgn = NewRgn();
    RectRgn( mBarRgn, &mBarRect );
    UnionRgn( window->visRgn, mBarRgn, window->visRgn ) ;
    DisposeRgn( mBarRgn );
    SetPort( window);
    FillRect( &(window->portRect), qd.black); /* Change black to ltGray, */
    PenMode( patXor ); /*<-- and comment out this line*/

}


void LinesInit(void) {

    short i;
    HideCursor();




}




