/* One function windows message to char* library.
 *
 * This is to be preferred over the popular MessageToString snippet because:
 *
 * 	1. It's not pointlessly object oriented.
 * 	2. It polutes your namespace much less (introducing only the function
 * 	   wm2str).
 * 	3. The implementation is simpler.
 * 	4. This is licensed.
 *
 * Code substantially based on the popular unlicensed MessageToString. I've
 * licensed it to encourage further use, not to assert ownership or that I am
 * the primary author.
 *
 * https://github.com/cmcaine/wm2str
 *
 * {{{ BSD 3-Clause License
 * 
 * Copyright 2017 Colin Caine
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * }}}
 */

#include <windows.h>
#include <string>

/* char* of the macro name that matches the UINT windowMessage or NULL.
 *
 * If second argument is false, very frequent window messages are not returned.
 *
 * Performance considerations:
 * 	If the compiler doesn't optimise this to a lookup table, then each
 * 	lookup costs worst case ~200 comparisons.
 *
 * Example use:

 	// Log WM_*, except for the very frequent messages.
	const char* msgName = wm2str(msg, false);
	if (msgName) {
		log(msgName);
	}

*/
static const std::string wm2str(UINT windowMessage, bool showFrequentMessages = true) {
	// {{{ Message to string array.
	struct WM_MAP_STR {
		UINT wm;
		const std::string str;
	};

#define DEFINE_MESSAGE(wm){ wm, #wm }
	static const WM_MAP_STR allMessages[] =
	{
		DEFINE_MESSAGE(WM_CREATE),
		DEFINE_MESSAGE(WM_DESTROY),
		DEFINE_MESSAGE(WM_MOVE),
		DEFINE_MESSAGE(WM_SIZE),
		DEFINE_MESSAGE(WM_ACTIVATE),
		DEFINE_MESSAGE(WM_SETFOCUS),
		DEFINE_MESSAGE(WM_KILLFOCUS),
		DEFINE_MESSAGE(WM_ENABLE),
		DEFINE_MESSAGE(WM_SETREDRAW),
		DEFINE_MESSAGE(WM_SETTEXT),
		DEFINE_MESSAGE(WM_GETTEXT),
		DEFINE_MESSAGE(WM_GETTEXTLENGTH),
		DEFINE_MESSAGE(WM_PAINT),
		DEFINE_MESSAGE(WM_CLOSE),
		DEFINE_MESSAGE(WM_QUERYENDSESSION),
		DEFINE_MESSAGE(WM_QUIT),
		DEFINE_MESSAGE(WM_QUERYOPEN),
		DEFINE_MESSAGE(WM_ERASEBKGND),
		DEFINE_MESSAGE(WM_SYSCOLORCHANGE),
		DEFINE_MESSAGE(WM_ENDSESSION),
		DEFINE_MESSAGE(WM_SHOWWINDOW),
		DEFINE_MESSAGE(WM_CTLCOLORMSGBOX),
		DEFINE_MESSAGE(WM_CTLCOLOREDIT),
		DEFINE_MESSAGE(WM_CTLCOLORLISTBOX),
		DEFINE_MESSAGE(WM_CTLCOLORBTN),
		DEFINE_MESSAGE(WM_CTLCOLORDLG),
		DEFINE_MESSAGE(WM_CTLCOLORSCROLLBAR),
		DEFINE_MESSAGE(WM_CTLCOLORSTATIC),
		DEFINE_MESSAGE(WM_WININICHANGE),
		DEFINE_MESSAGE(WM_SETTINGCHANGE),
		DEFINE_MESSAGE(WM_DEVMODECHANGE),
		DEFINE_MESSAGE(WM_ACTIVATEAPP),
		DEFINE_MESSAGE(WM_FONTCHANGE),
		DEFINE_MESSAGE(WM_TIMECHANGE),
		DEFINE_MESSAGE(WM_CANCELMODE),
		DEFINE_MESSAGE(WM_SETCURSOR),
		DEFINE_MESSAGE(WM_MOUSEACTIVATE),
		DEFINE_MESSAGE(WM_CHILDACTIVATE),
		DEFINE_MESSAGE(WM_QUEUESYNC),
		DEFINE_MESSAGE(WM_GETMINMAXINFO),
		DEFINE_MESSAGE(WM_ICONERASEBKGND),
		DEFINE_MESSAGE(WM_NEXTDLGCTL),
		DEFINE_MESSAGE(WM_SPOOLERSTATUS),
		DEFINE_MESSAGE(WM_DRAWITEM),
		DEFINE_MESSAGE(WM_MEASUREITEM),
		DEFINE_MESSAGE(WM_DELETEITEM),
		DEFINE_MESSAGE(WM_VKEYTOITEM),
		DEFINE_MESSAGE(WM_CHARTOITEM),
		DEFINE_MESSAGE(WM_SETFONT),
		DEFINE_MESSAGE(WM_GETFONT),
		DEFINE_MESSAGE(WM_QUERYDRAGICON),
		DEFINE_MESSAGE(WM_COMPAREITEM),
		DEFINE_MESSAGE(WM_COMPACTING),
		DEFINE_MESSAGE(WM_NCCREATE),
		DEFINE_MESSAGE(WM_NCDESTROY),
		DEFINE_MESSAGE(WM_NCCALCSIZE),
		DEFINE_MESSAGE(WM_NCHITTEST),
		DEFINE_MESSAGE(WM_NCPAINT),
		DEFINE_MESSAGE(WM_NCACTIVATE),
		DEFINE_MESSAGE(WM_GETDLGCODE),
		DEFINE_MESSAGE(WM_NCMOUSEMOVE),
		DEFINE_MESSAGE(WM_NCLBUTTONDOWN),
		DEFINE_MESSAGE(WM_NCLBUTTONUP),
		DEFINE_MESSAGE(WM_NCLBUTTONDBLCLK),
		DEFINE_MESSAGE(WM_NCRBUTTONDOWN),
		DEFINE_MESSAGE(WM_NCRBUTTONUP),
		DEFINE_MESSAGE(WM_NCRBUTTONDBLCLK),
		DEFINE_MESSAGE(WM_NCMBUTTONDOWN),
		DEFINE_MESSAGE(WM_NCMBUTTONUP),
		DEFINE_MESSAGE(WM_NCMBUTTONDBLCLK),
		DEFINE_MESSAGE(WM_KEYDOWN),
		DEFINE_MESSAGE(WM_KEYUP),
		DEFINE_MESSAGE(WM_CHAR),
		DEFINE_MESSAGE(WM_DEADCHAR),
		DEFINE_MESSAGE(WM_SYSKEYDOWN),
		DEFINE_MESSAGE(WM_SYSKEYUP),
		DEFINE_MESSAGE(WM_SYSCHAR),
		DEFINE_MESSAGE(WM_SYSDEADCHAR),
		DEFINE_MESSAGE(WM_KEYLAST),
		DEFINE_MESSAGE(WM_INITDIALOG),
		DEFINE_MESSAGE(WM_COMMAND),
		DEFINE_MESSAGE(WM_SYSCOMMAND),
		DEFINE_MESSAGE(WM_TIMER),
		DEFINE_MESSAGE(WM_HSCROLL),
		DEFINE_MESSAGE(WM_VSCROLL),
		DEFINE_MESSAGE(WM_INITMENU),
		DEFINE_MESSAGE(WM_INITMENUPOPUP),
		DEFINE_MESSAGE(WM_MENUSELECT),
		DEFINE_MESSAGE(WM_MENUCHAR),
		DEFINE_MESSAGE(WM_ENTERIDLE),
		DEFINE_MESSAGE(WM_MOUSEWHEEL),
		DEFINE_MESSAGE(WM_MOUSEMOVE),
		DEFINE_MESSAGE(WM_LBUTTONDOWN),
		DEFINE_MESSAGE(WM_LBUTTONUP),
		DEFINE_MESSAGE(WM_LBUTTONDBLCLK),
		DEFINE_MESSAGE(WM_RBUTTONDOWN),
		DEFINE_MESSAGE(WM_RBUTTONUP),
		DEFINE_MESSAGE(WM_RBUTTONDBLCLK),
		DEFINE_MESSAGE(WM_MBUTTONDOWN),
		DEFINE_MESSAGE(WM_MBUTTONUP),
		DEFINE_MESSAGE(WM_MBUTTONDBLCLK),
		DEFINE_MESSAGE(WM_PARENTNOTIFY),
		DEFINE_MESSAGE(WM_MDICREATE),
		DEFINE_MESSAGE(WM_MDIDESTROY),
		DEFINE_MESSAGE(WM_MDIACTIVATE),
		DEFINE_MESSAGE(WM_MDIRESTORE),
		DEFINE_MESSAGE(WM_MDINEXT),
		DEFINE_MESSAGE(WM_MDIMAXIMIZE),
		DEFINE_MESSAGE(WM_MDITILE),
		DEFINE_MESSAGE(WM_MDICASCADE),
		DEFINE_MESSAGE(WM_MDIICONARRANGE),
		DEFINE_MESSAGE(WM_MDIGETACTIVE),
		DEFINE_MESSAGE(WM_MDISETMENU),
		DEFINE_MESSAGE(WM_CUT),
		DEFINE_MESSAGE(WM_COPYDATA),
		DEFINE_MESSAGE(WM_COPY),
		DEFINE_MESSAGE(WM_PASTE),
		DEFINE_MESSAGE(WM_CLEAR),
		DEFINE_MESSAGE(WM_UNDO),
		DEFINE_MESSAGE(WM_RENDERFORMAT),
		DEFINE_MESSAGE(WM_RENDERALLFORMATS),
		DEFINE_MESSAGE(WM_DESTROYCLIPBOARD),
		DEFINE_MESSAGE(WM_DRAWCLIPBOARD),
		DEFINE_MESSAGE(WM_PAINTCLIPBOARD),
		DEFINE_MESSAGE(WM_VSCROLLCLIPBOARD),
		DEFINE_MESSAGE(WM_SIZECLIPBOARD),
		DEFINE_MESSAGE(WM_ASKCBFORMATNAME),
		DEFINE_MESSAGE(WM_CHANGECBCHAIN),
		DEFINE_MESSAGE(WM_HSCROLLCLIPBOARD),
		DEFINE_MESSAGE(WM_QUERYNEWPALETTE),
		DEFINE_MESSAGE(WM_PALETTEISCHANGING),
		DEFINE_MESSAGE(WM_PALETTECHANGED),
		DEFINE_MESSAGE(WM_DROPFILES),
		DEFINE_MESSAGE(WM_POWER),
		DEFINE_MESSAGE(WM_WINDOWPOSCHANGED),
		DEFINE_MESSAGE(WM_WINDOWPOSCHANGING),
		DEFINE_MESSAGE(WM_HELP),
		DEFINE_MESSAGE(WM_NOTIFY),
		DEFINE_MESSAGE(WM_CONTEXTMENU),
		DEFINE_MESSAGE(WM_TCARD),
		DEFINE_MESSAGE(WM_MDIREFRESHMENU),
		DEFINE_MESSAGE(WM_MOVING),
		DEFINE_MESSAGE(WM_STYLECHANGED),
		DEFINE_MESSAGE(WM_STYLECHANGING),
		DEFINE_MESSAGE(WM_SIZING),
		DEFINE_MESSAGE(WM_SETHOTKEY),
		DEFINE_MESSAGE(WM_PRINT),
		DEFINE_MESSAGE(WM_PRINTCLIENT),
		DEFINE_MESSAGE(WM_POWERBROADCAST),
		DEFINE_MESSAGE(WM_HOTKEY),
		DEFINE_MESSAGE(WM_GETICON),
		DEFINE_MESSAGE(WM_EXITMENULOOP),
		DEFINE_MESSAGE(WM_ENTERMENULOOP),
		DEFINE_MESSAGE(WM_DISPLAYCHANGE),
		DEFINE_MESSAGE(WM_STYLECHANGED),
		DEFINE_MESSAGE(WM_STYLECHANGING),
		DEFINE_MESSAGE(WM_GETICON),
		DEFINE_MESSAGE(WM_SETICON),
		DEFINE_MESSAGE(WM_SIZING),
		DEFINE_MESSAGE(WM_MOVING),
		DEFINE_MESSAGE(WM_CAPTURECHANGED),
		DEFINE_MESSAGE(WM_DEVICECHANGE),
		DEFINE_MESSAGE(WM_PRINT),
		DEFINE_MESSAGE(WM_PRINTCLIENT),
#ifdef _DDEHEADER_INCLUDED_
		// DDE Specific messages
		DEFINE_MESSAGE(WM_DDE_INITIATE),
		DEFINE_MESSAGE(WM_DDE_TERMINATE),
		DEFINE_MESSAGE(WM_DDE_ADVISE),
		DEFINE_MESSAGE(WM_DDE_UNADVISE),
		DEFINE_MESSAGE(WM_DDE_ACK),
		DEFINE_MESSAGE(WM_DDE_DATA),
		DEFINE_MESSAGE(WM_DDE_REQUEST),
		DEFINE_MESSAGE(WM_DDE_POKE),
		DEFINE_MESSAGE(WM_DDE_EXECUTE),
#endif
#ifdef _MFC_VER
		// MFC specific messages
		// Not sure that ifdef is right...
		DEFINE_MESSAGE(WM_SIZEPARENT),
		DEFINE_MESSAGE(WM_SETMESSAGESTRING),
		DEFINE_MESSAGE(WM_IDLEUPDATECMDUI),
		DEFINE_MESSAGE(WM_INITIALUPDATE),
		DEFINE_MESSAGE(WM_COMMANDHELP),
		DEFINE_MESSAGE(WM_HELPHITTEST),
		DEFINE_MESSAGE(WM_EXITHELPMODE),
#endif
	};
#undef DEFINE_MESSAGE
	// }}}

	// Optionally don't report very frequently sent messages.
	if ( !showFrequentMessages && 
		(
			windowMessage == WM_MOUSEMOVE         ||
			windowMessage == WM_NCMOUSEMOVE       ||
			windowMessage == WM_NCHITTEST         ||
			windowMessage == WM_SETCURSOR         ||
			windowMessage == WM_CTLCOLORBTN       ||
			windowMessage == WM_CTLCOLORDLG       ||
			windowMessage == WM_CTLCOLOREDIT      ||
			windowMessage == WM_CTLCOLORLISTBOX   ||
			windowMessage == WM_CTLCOLORMSGBOX    ||
			windowMessage == WM_CTLCOLORSCROLLBAR ||
			windowMessage == WM_CTLCOLORSTATIC    ||
			windowMessage == WM_ENTERIDLE         ||
			windowMessage == WM_CANCELMODE
		)
	   )
	{
		return "";
	}

	// Search allMessages for the desired window message.
	for (int index=0; index < ARRAYSIZE(allMessages); index++)
	{
		if (allMessages[index].wm == windowMessage )
		{
			return allMessages[index].str;
		}
	}

	// If no match is found.
	return "";
}
