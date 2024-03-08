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

Rect gLines[kNumLines];

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
    RectRgn(mBarRgn, &mBarRect);
    UnionRgn(window->visRgn, mBarRgn, window->visRgn);
    DisposeRgn(mBarRgn);
    SetPort(window);
    FillRect(&(window->portRect), &qd.black); /* Change black to ltGray, */
    PenMode(patXor); /*<-- and comment out this line*/

}


void LinesInit(void) {

    short i;

    HideCursor();
    GetDateTime((unsigned long *) (&qd.randSeed));
    RandomRect(&(gLines[0]));
    DrawLine(0);
    for (i = 1; i < kNumLines; i++) {
        gLines[i] = gLines[i - 1];
        RecalcLine(i);
        DrawLine(i);
    }


}


void MainLoop(void) {
    short i;
    unsigned long tick, startTick;

    startTick = TickCount();
    while (!Button()) {

        // NOTE the original loop is too fast on Basilisk II.
        // so I added this delay.
        tick = TickCount();
        if (tick - startTick < 1 ) {
            continue;
        }
        startTick = tick;
        DrawLine(kNumLines - 1);
        for (i = kNumLines - 1; i > 0; i--) {
            gLines[i] = gLines[i - 1];
        }
        RecalcLine(0);
        DrawLine(i);

    }

    LMSetMBarHeight(gOldMBarHeight);

}

void RandomRect(Rect *rectPtr) {
    WindowPtr window;
    window = FrontWindow();

    rectPtr->left = Randomize(
            window->portRect.right
            - window->portRect.left
    );
    rectPtr->right = Randomize(
            window->portRect.right
            - window->portRect.left
    );
    rectPtr->top = Randomize(
            window->portRect.bottom
            - window->portRect.top
    );
    rectPtr->bottom = Randomize(
            window->portRect.bottom
            - window->portRect.top
    );

}

short Randomize(short range) {
    long randomNumber;
    randomNumber = Random();

    if (randomNumber < 0) {
        randomNumber *= -1;
    }
    return ((randomNumber * range) / kRandomUpperLimit);
}

void RecalcLine(short i) {
    WindowPtr window;
    window = FrontWindow();

    gLines[i].top += gDeltaTop;
    if ((gLines[i].top < window->portRect.top)
        || (gLines[i].top > window->portRect.bottom)) {
        gDeltaTop *= -1;
        gLines[i].top += 2 * gDeltaTop;
    }

    gLines[i].bottom += gDeltaBottom;
    if ((gLines[i].bottom < window->portRect.top)
        || (gLines[i].bottom > window->portRect.bottom)) {
        gDeltaBottom *= -1;
        gLines[i].top += 2 * gDeltaBottom;
    }

    gLines[i].left += gDeltaLeft;
    if ((gLines[i].left < window->portRect.left)
        || (gLines[i].left > window->portRect.right)) {
        gDeltaLeft *= -1;
        gLines[i].left += 2 * gDeltaLeft;
    }

    gLines[i].right += gDeltaRight;
    if ((gLines[i].right < window->portRect.left)
        || (gLines[i].right > window->portRect.right)) {
        gDeltaRight *= -1;
        gLines[i].right += 2 * gDeltaRight;
    }

}


void DrawLine(short i) {
    MoveTo(gLines[i].left, gLines[i].top);
    LineTo(gLines[i].right, gLines[i].bottom);

}