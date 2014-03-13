/**Copyright (c) 2011-2014, Taiki
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of Mavy nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.**/

#include "main.h"

#ifdef _WIN32
#include <windows.h>

char GetCh (void)
{
    HANDLE hStdin = GetStdHandle (STD_INPUT_HANDLE);
    INPUT_RECORD irInputRecord;
    DWORD dwEventsRead;
    char cChar;

    while(ReadConsoleInputA (hStdin, &irInputRecord, 1, &dwEventsRead)) /* Read key press */
        if (irInputRecord.EventType == KEY_EVENT
            &&irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_SHIFT
            &&irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_MENU
            &&irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_CONTROL)
        {
            cChar = irInputRecord.Event.KeyEvent.uChar.AsciiChar;
            ReadConsoleInputA (hStdin, &irInputRecord , 1, &dwEventsRead); /* Read key release */
            return cChar;
        }
    return EOF;
}
#endif


