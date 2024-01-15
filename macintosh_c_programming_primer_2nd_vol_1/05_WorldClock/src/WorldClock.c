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
#define iAbout   ( 1)

#define mFile    (kBaseResID+1)
#define iQuit    1

#define mFont    100
#define mStyle    101
#define iPlain    1
#define iBold    2
#define iItalic    3
#define iUnderline  4
#define iOutline    5
#define iShadow    6

#define kPlainStyle 0

#define kExtraPopupPixels ((short)25)

#define kClockLeft 12
#define kClockTop  25
#define kClockSize 24

#define kCurrentTimeZone 1
#define kNewYorkTimeZone 2
#define kMoscowTimeZone 3
#define kUlanBatorTimeZone 4


/* Rect를 Point로 변환 */
#define TopLeft(r)        (*(Point *) &(r).top)
#define BottomRight(r)    (*(Point *) &(r).bottom)

#define IsHighBitSet(longNum)  ((longNum >> 23) & 1)
#define SetHighByte(longNum)  ( longNum |= 0xFF000000 )
#define ClearHighByte(longNum)  ( longNum &= 0x00FFFFFF )

/* Globals */

Boolean gDone, gHasPopupControl;

short gLastFont = 1;  /* 기본 폰트 */
short gCurrentZoneID = kCurrentTimeZone;
Style gCurrentStyle = kPlainStyle;
Rect gClockRect;

void ToolBoxInit(void);

void WindowInit(void);

void MenuBarInit(void);

void EventLoop(void);

void DoEvent(EventRecord *eventPtr);

void HandleNull(EventRecord *eventPtr);

void HandleMouseDown(EventRecord *eventPtr);

void SetUpZoomPosition(WindowPtr window, short zoominOrOut);

void HandleMenuChoice(long menuChoice);

void HandleAppleChoice(short item);

void HandleFileChoice(short item);

void HandleFontChoice(short item);

void HandleStyleChoice(short item);

void DoUpdate(EventRecord *eventPtr);

long GetZoneOffset(void);


int main(void) {

//    printf("Hello, World\n");
    ToolBoxInit();
    WindowInit();
    MenuBarInit();
    EventLoop();

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


    menuBar = GetNewMBar(kBaseResID); // MBAR resource
    SetMenuBar(menuBar);
    menu = GetMHandle(mApple);
    AddResMenu(menu, 'DRVR');

    menu = GetMenu(mFont);
    // menu bar 에는 표시하지 않음
    InsertMenu(menu, kNotANormalMenu);
    AddResMenu(menu, 'FONT');

    menu = GetMenu(mStyle);
    InsertMenu(menu, kNotANormalMenu);
    // 체크 마크 표시
    CheckItem(menu, iPlain, true);

    DrawMenuBar();
    HandleFontChoice(gLastFont);
    myErr = Gestalt(gestaltPopupAttr, &feature);

    // 팝업 컨트롤 활성화?
    gHasPopupControl = ((myErr == noErr)
                        && (feature & (1 << gestaltPopupPresent)));

    if (gHasPopupControl) {
        control = GetNewControl(kPopupControlID, FrontWindow());
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


void DoEvent(EventRecord *eventPtr) {

    char theChar;
    // menu ID 2bytes, item number 2bytes
    long menuKey;
    switch (eventPtr->what) {

        case mouseDown:
            HandleMouseDown(eventPtr);
            break;
        case keyDown:
        case autoKey:
            // 입력된 character
            theChar = eventPtr->message & charCodeMask;
            if ((eventPtr->modifiers & cmdKey) != 0) {
                menuKey = MenuKey(theChar);
                HandleMenuChoice(menuKey);
            }
            break;
        case updateEvt:
            DoUpdate(eventPtr);
            break;
    }

}


void HandleNull(EventRecord *eventPtr) {

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

    // 어느 window의 어느 part에 이벤트가 발생했는지 찾음
    thePart = FindWindow(eventPtr->where, &whichWindow);

    switch (thePart) {
        case inMenuBar:
            menuChoice = MenuSelect(eventPtr->where);
            HandleMenuChoice(menuChoice);
            break;
        //
        case inSysWindow:
            SystemClick(eventPtr, whichWindow);
            break;

        case inContent:
            SetPort(whichWindow);
            // 로컬 좌표로 변환
            GlobalToLocal(&eventPtr->where);
            // 컨트롤을 찾는다
            if (FindControl(eventPtr->where, whichWindow, &control)) {

                // 팝업
                ignored = TrackControl(control, eventPtr->where, kUseDefaultProc);
                // 팝업에서 선택된 값
                gCurrentZoneID = GetCtlValue(control);

                // printf("gCurrentZoneID=%x\n", gCurrentZoneID);

            }
            break;

        case inDrag:
            DragWindow(whichWindow, eventPtr->where, &qd.screenBits.bounds);
            break;

        case inZoomIn:
        case inZoomOut:
            if (TrackBox(whichWindow, eventPtr->where, thePart)) {
                SetUpZoomPosition(whichWindow, thePart);
                ZoomWindow(whichWindow, thePart, kLeaveWhereItIs);
            }
            break;
    }
}

/*
 * zoom box
 * */
void SetUpZoomPosition(WindowPtr window, short zoominOrOut) {

    WindowPeek wPeek;
    WStateData *wStatePtr;
    Rect windowRect;
    Boolean isBig;
    short deltaPixels;

    wPeek = (WindowPeek) window;
    wStatePtr = (WStateData *) *(wPeek->dataHandle);
    windowRect = window->portRect;
    LocalToGlobal(&TopLeft(windowRect));
    LocalToGlobal(&BottomRight(windowRect));

    wStatePtr->stdState = windowRect;
    wStatePtr->userState = wStatePtr->stdState;

    // 팝업 컨트롤은 System 7부터 가능
    if (gHasPopupControl) {

        // 현재 크기는?
        isBig = (windowRect.bottom - windowRect.top) >
                (gClockRect.bottom - gClockRect.top);

        if (isBig) {
            deltaPixels = -kExtraPopupPixels;
        } else {
            deltaPixels = kExtraPopupPixels;
        }

        if (zoominOrOut == inZoomIn) {
            wStatePtr->userState.bottom += deltaPixels;
        } else {
            wStatePtr->stdState.bottom += deltaPixels;
        }
    //     System 6 이하일 경우
    } else {
        SysBeep(20);
    }

}


void HandleMenuChoice(long menuChoice) {

    short menu;
    short item;

    if (menuChoice != 0) {

        menu = HiWord(menuChoice);
        item = LoWord(menuChoice);

        switch (menu) {
            case mApple:
                HandleAppleChoice(item);
                break;
            case mFile:
                HandleFileChoice(item);
                break;
            case mFont:
                HandleFontChoice(item);
                break;
            case mStyle:
                HandleStyleChoice(item);
                break;

        }

        // 메뉴 끄기
        HiliteMenu(0);

    }


}


void HandleAppleChoice(short item) {
    MenuHandle appleMenu;
    Str255 accName;
    short accNumber;

    switch (item) {

        case iAbout: /* We'll put up an about box next chapter.*/
            SysBeep(20);
            break;
        default:
            appleMenu = GetMHandle(mApple);
            GetItem(appleMenu, item, accName);
            accNumber = OpenDeskAcc(accName);
            break;

    }

}


void HandleFileChoice(short item) {
    switch (item) {

        // 프로그램 종료
        case iQuit:
            gDone = true;
            break;
    }
}


void HandleFontChoice(short item) {
    short fontNumber;
    Str255 fontName;
    MenuHandle menuHandle;

    menuHandle = GetMHandle(mFont);

    CheckItem(menuHandle, gLastFont, kRemoveCheckMark);
    CheckItem(menuHandle, item, kAddCheckMark);
    gLastFont = item;
    GetItem(menuHandle, item, fontName);
    GetFNum(fontName, &fontNumber);
    // 폰트 변경
    TextFont(fontNumber);
}

void HandleStyleChoice(short item) {
    MenuHandle menuHandle;

    switch (item) {
        case iPlain:
            gCurrentStyle = kPlainStyle;
            break;
        case iBold:
            // gCurrentStyle ^= bold;
            if (gCurrentStyle & bold) {
                gCurrentStyle -= bold;
            } else {
                gCurrentStyle |= bold;
            }
            break;
        case iItalic:
            if (gCurrentStyle & italic) {
                gCurrentStyle -= italic;
            } else {
                gCurrentStyle |= italic;
            }
            break;
        case iUnderline:
            if (gCurrentStyle & underline) {
                gCurrentStyle -= underline;
            } else {
                gCurrentStyle |= underline;
            }
            break;
        case iOutline:
            if (gCurrentStyle & outline) { gCurrentStyle -= outline; }
            else {
                gCurrentStyle |= outline;
            }
            break;
        case iShadow:
            if (gCurrentStyle & shadow) {
                gCurrentStyle -= shadow;
            } else {
                gCurrentStyle |= shadow;
            }
            break;

    }


    menuHandle = GetMHandle(mStyle);
    CheckItem(menuHandle, iPlain, gCurrentStyle == kPlainStyle);
    CheckItem(menuHandle, iBold, gCurrentStyle & bold);
    CheckItem(menuHandle, iItalic, gCurrentStyle & italic);
    CheckItem(menuHandle, iUnderline, gCurrentStyle & underline);
    CheckItem(menuHandle, iOutline, gCurrentStyle & outline);
    CheckItem(menuHandle, iShadow, gCurrentStyle & shadow);
    TextFace(gCurrentStyle);

}

void DoUpdate(EventRecord *eventPtr) {
    WindowPtr window;
    Str255 timeString;
    unsigned long curTimeinSecs;

    window = (WindowPtr) eventPtr->message;

    BeginUpdate(window);
    GetDateTime(&curTimeinSecs);
    curTimeinSecs += GetZoneOffset();
    // Pascal 스트링으로 변환
    IUTimeString((long) curTimeinSecs, kIncludeSeconds, timeString);
    EraseRect(&gClockRect);
    MoveTo(kClockLeft, kClockTop);
    DrawString(timeString);
    DrawControls(window);
    EndUpdate(window);

}


long GetZoneOffset(void) {
    MachineLocation loc;
    long delta, defaultZoneOffset;

    // PRAM에서 설정된 지역과 타임존을 가져 온다.
    ReadLocation(&loc);

    // loc.gmtFlags.gmtDelta
    defaultZoneOffset = ClearHighByte(loc.u.gmtDelta);
    if (IsHighBitSet(defaultZoneOffset)) {
        SetHighByte(defaultZoneOffset);
    }

    switch (gCurrentZoneID) {
        case kCurrentTimeZone:
            delta = defaultZoneOffset;
            break;
        case kNewYorkTimeZone :
            delta = -5L * kSecondsPerHour;
            break;
        case kMoscowTimeZone :
            delta = 3L * kSecondsPerHour;
            break;
        case kUlanBatorTimeZone :
            delta = 8L * kSecondsPerHour;
            break;
    }

    delta -= defaultZoneOffset;

    return delta;

}







