//#define TARGET_OS_MAC (1)

#include <TargetConditionals.h>

#include <Packages.h>
#include <GestaltEqu.h>
#include <OSUtils.h>
#include <Sound.h>
#include <Quickdraw.h>
#include <Events.h>
#include <Fonts.h>
#include <Resources.h>


#define kBaseResID    128
#define kMoveToFront    (WindowPtr)-1L
#define kUseDefaultProc    (void *-IL
#define ksleep    201
#define kLeaveWhereItIs    FALSE

#define kIncludeSeconds    TRUE
#define kTicksPerSecond    60
#define kSecondsPerHour    3600L

#define kAddCheckMark    true
#define kRemoveCheckMark    false

#define kPopupControlID    kBaseResID
#define kNotANormalMenu    -1

#define mApple    kBaseResID
#define About    1

#define mFile    kBaseResID+1
#define iQuit    1

#define mFont    100

#define mStyle    101
#define iPlain    1
#define iBold    2
#define italic    3
#define Underline    4
#define Outline    5

#define kPlainStyle 0

#define kExtraPopupPixels 25

#define kClockLeft 12
#define kClockTop  25
#define kClockSize 24

#define kCurrentTimeZone 1
#define kNewYorkTimeZone 2

#define kMoscowTimeZone 3
#define kUlanBatorTimeZone 4

#define TopLeft(r)  (*(Point*) &(r).top)
#define BottomRight(r)    (*(Point *) &(r) .bottom)

#define IsHighBitSet(longNum)  ({longNum >> 23) & 1)
#define SetHighByte(longNum)  ( longNum I= OxFFOOOOOO )
#define ClearHighByte(longNum)  ( longNum &= OxOOFFFFFF )

/* Globals */

Boolean gDone, gHasPopupControl;
short gLastFont = 1, gCurrentZoneID;
Style gCurrentStyle = kPlainStyle;
Rect gClockRect;


void ToolBoxinit(void);
void Windowinit(void);
void MenuBarinit(void);
void EventLoop(void);
void DoEvent( EventRecord *eventPtr );
void HandleNull(EventRecord * eventPtr );
void HandleMouseDown(EventRecord *eventPtr);
void SetUpZoomPosition(WindowPtr window, short zoominOrOut);

void HandleAppleChoice(short item);
void HandleFileChoice(short item);
void HandleFontChoice(short item);
void HandleStyleChoice(short item);
void DoUpdate(EventRecord *eventPtr);
long GetZoneOffset(void);


int main(void) {

    ToolBoxinit () ;
    Windowinit();
    MenuBarinit();
    EventLoop();

}

void ToolBoxInit() {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nil);
    InitCursor();
}





