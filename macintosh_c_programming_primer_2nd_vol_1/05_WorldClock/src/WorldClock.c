// for Old codes
#define  TARGET_OS_MAC 1
#define  TARGET_CPU_68K 1
#define  OLDROUTINENAMES 1
//#define  TARGET_API_MAC_CARBON 0
//#define  CALL_NOT_IN_CARBON 0
/*
 *  ConditionalMacros.h 참조
 * */
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
#define kMoveToFront   ( (WindowPtr)-1L)
#define kUseDefaultProc    ((void *)-1L)
#define kSleep    201
#define kLeaveWhereItIs    FALSE

#define kIncludeSeconds    TRUE
#define kTicksPerSecond    60
#define kSecondsPerHour    3600L

#define kAddCheckMark    true
#define kRemoveCheckMark    false

#define kPopupControlID    kBaseResID
#define kNotANormalMenu    (-1)

#define mApple    kBaseResID
#define About   ( 1)

#define mFile    (kBaseResID+1)
#define iQuit    (1)

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

void DoEvent(EventRecord *eventPtr);

void HandleNull(EventRecord *eventPtr);

void HandleMouseDown(EventRecord *eventPtr);

void SetUpZoomPosition(WindowPtr window, short zoominOrOut);

void HandleMenuChoice(short item);

void HandleAppleChoice(short item);

void HandleFileChoice(short item);

void HandleFontChoice(short item);

void HandleStyleChoice(short item);

void DoUpdate(EventRecord *eventPtr);

long GetZoneOffset(void);


int main(void) {

    ToolBoxinit();
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


void Windowlnit(void) {
    WindowPtr window;
    window = GetNewWindow(kBaseResID, nil, kMoveToFront);

    if (window == nil) {
        SysBeep(10);
        ExitToShell();
    }

    SetPort(window);
    TextSize(kClockSize);

    gClockRect = window->portRect;
    ShowWindow(window);


}


void MenuBarInit(void) {

    Handle menuBar;
    MenuHandle menu;
    ControlHandle control;
    OSErr myErr;
    long feature;

    menuBar = GetNewMBar(kBaseResID);
    SetMenuBar(menuBar);
    menu = GetMHandle(mApple);
    AddResMenu(menu, 'DRVR');

    menu = GetMenu(mFont);
    InsertMenu(menu, kNotANormalMenu);
    AddResMenu(menu, 'FONT');


    menu = GetMenu(mStyle);
    InsertMenu(menu, kNotANormalMenu);
    CheckItem(menu, iPlain, true);

    DrawMenuBar();
    HandleFontChoice(gLastFont);
    myErr = Gestalt(gestaltPopupAttr, &feature);
    gHasPopupControl((myErr == noErr) && (feature &
                                          (1 << gestaltPopupPresent)));

    if (gHasPopupControl) {
        control = GetNewControl(kPopupControlID,
                                FrontWindow());
    }
}


void EventLoop(void) {

    EventRecord event;
    gDone = false;
    while (gDone == false) {
        if (WaitNextEvent(everyEvent, &event, kSleep, nil)) {
            DoEvent(&event);
        } else {
            HandleNull(&event);
        }
    }
}


void
DoEvent( EventRecord *eventPtr )
{


    char theChar;
    switch (eventPtr->what) {

        case mouseDown:
            HandleMouseDown(eventPtr);
            break;
        case keyDown:
        case autoKey:
            theChar = eventPtr->message & charCodeMask;
            if ((eventPtr->modifiers & cmdKey) != 0) {
                HandleMenuChoice(MenuKey(theChar));
            }
            break;
        case updateEvt:
            DoUpdate(eventPtr);
            break;
    }

}


void HandleNull( EventRecord *eventPtr) {

    static long lastTime = 0;
    if ((eventPtr->when / kTicksPerSecond) > lastTime) {
        InvalRect(&gClockRect);
        lastTime = eventPtr->when / kTicksPerSecond;
    }

}

void HandleMouseDown(EventRecord *eventPtr) {
    WindowPtr whichWindow;
    GrafPtr oldPort;
    short thePart;
    long menuChoice;
    ControlHandle control;
    short ignored;

    thePart = FindWindow(eventPtr->where, &whichWindow);

    switch (thePart) {
        case inMenuBar:
            menuChoice = MenuSelect(eventPtr->where);
            HandleMenuChoice(menuChoice);
            break;
        case inSysWindow:
            SystemClick(eventPtr, whichWindow);
            break;
        case inContent:
            SetPort(whichWindow);
            GlobalToLocal(&eventPtr->where);

            if (FindControl(eventPtr->where, whichWindow, &control)) {

                ignored = TrackControl(control, eventPtr->where, kUseDefaultProc);

                gCurrentZoneID = GetCtlValue(control);
            }

            break;
        case inDrag:
            DragWindow(whichWindow, eventPtr->where, &screenBits.bounds);
            break;

        case inZoomIn:
        case inZoomOut:
            if (TrackBox(whichWindow, eventPtr->where,
                         thePart)) {
                SetUpZoomPosition(whichWindow, thePart);
                ZoomWindow(whichWindow, thePart, kLeaveWhereItIs);
            }
            break;
    }
}

void SetUpZoornPosition( WindowPtr window, short zoominOrOut ) {




}


void HandleMenuChoice( short item ) {



}


void HandleAppleChoice( short item ) {



}


void HandleFontChoice( short item ) {


}

void DoUpdate( EventRecord *eventPtr ) {


}


long GetZoneOffset( void) {
    MachineLocation loc;
    long delta, defaultZoneOffset;
    delta = 0;


    return delta;
}







