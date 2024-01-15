#include "Processes.r"

data 'MENU' (128, "Apple Menu") {
	$"0080 0000 0000 0000 0000 FFFF FFFB 0114"            /* .�........����.. */
	$"1341 626F 7574 2057 6F72 6C64 436C 6F63"            /* .About WorldCloc */
	$"6B2E 2E2E 0000 0000 012D 0000 0000 0100"            /* k........-...... */
	$"0000 0000 00"                                       /* ..... */
};

data 'MENU' (129, "File") {
	$"0081 0000 0000 0000 0000 FFFF FFFF 0446"            /* .�........����.F */
	$"696C 6504 5175 6974 0051 0000 00"                   /* ile.Quit.Q... */
};

data 'MENU' (130, "Edit") {
	$"0082 0000 0000 0000 0000 FFFF FFFB 0445"            /* .�........����.E */
	$"6469 7404 556E 646F 005A 0000 012D 0000"            /* dit.Undo.Z...-.. */
	$"0000 0343 7574 0058 0000 0443 6F70 7900"            /* ...Cut.X...Copy. */
	$"4300 0005 5061 7374 6500 5600 0005 436C"            /* C...Paste.V...Cl */
	$"6561 7200 0000 0000"                                /* ear..... */
};

data 'MENU' (131, "Special") {
	$"0083 0000 0000 0000 0000 FFFF FFFF 0753"            /* .�........����.S */
	$"7065 6369 616C 0446 6F6E 7400 1B64 0005"            /* pecial.Font..d.. */
	$"5374 796C 6500 1B65 0000"                           /* Style..e.. */
};

data 'MENU' (100, "Font") {
	$"0064 0000 0000 0000 0000 FFFF FFFF 0446"            /* .d........����.F */
	$"6F6E 7400"                                          /* ont. */
};

data 'MENU' (101, "Style") {
	$"0065 0000 0000 0000 0000 FFFF FFFF 0553"            /* .e........����.S */
	$"7479 6C65 0550 6C61 696E 0000 0000 0442"            /* tyle.Plain.....B */
	$"6F6C 6400 0000 0106 4974 616C 6963 0000"            /* old.....Italic.. */
	$"0002 0955 6E64 6572 6C69 6E65 0000 0004"            /* ...Underline.... */
	$"074F 7574 6C69 6E65 0000 0008 0653 6861"            /* .Outline.....Sha */
	$"646F 7700 0000 1000"                                /* dow..... */
};

data 'MENU' (132, "Time Zone") {
	$"0084 0000 0000 0000 0000 FFFF FFFF 0954"            /* .�........����.T */
	$"696D 6520 5A6F 6E65 0743 7572 7265 6E74"            /* ime Zone.Current */
	$"0000 0000 084E 6577 2059 6F72 6B00 0000"            /* .....New York... */
	$"0006 4D6F 7363 6F77 0000 0000 0A55 6C61"            /* ..Moscow.....Ula */
	$"6E20 4261 746F 7200 0000 0000"                      /* n Bator..... */
};

data 'CNTL' (128) {
	$"0014 0005 0041 00B4 0000 0100 0046 0084"            /* .....A.�.....F.� */
	$"03F0 0000 0000 0A54 696D 6520 5A6F 6E65"            /* .�.....Time Zone */
	$"3A"                                                 /* : */
};

data 'WIND' (128) {
	$"0032 0014 0052 00CE 000C 0100 0100 0000"            /* .2...R.�........ */
	$"0000 0A57 6F72 6C64 436C 6F63 6B"                   /* ...WorldClock */
};

data 'MBAR' (128) {
	$"0004 0080 0081 0082 0083"                           /* ...�.�.�.� */
};

# background ???
resource 'SIZE' (-1) {
	reserved,
	ignoreSuspendResumeEvents,
	reserved,
	canBackground,
	multiFinderAware,
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreAppDiedEvents,
	is32BitCompatible,
	notHighLevelEventAware,
	onlyLocalHLEvents,
	notStationeryAware,
	dontUseTextEditServices,
	notDisplayManagerAware,
	reserved,
	reserved,
	300 * 1024,
	100 * 1024
};
