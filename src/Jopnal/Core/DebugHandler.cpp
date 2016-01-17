// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
#ifdef JOP_OS_WINDOWS

    FILE ns_oldOutBuffer;
    FILE ns_oldInBuffer;

    BOOL WINAPI handleConsoleEvent(DWORD event)
    {
        return event == CTRL_C_EVENT;
    }

    bool checkConsoleWindow()
    {
        return GetConsoleWindow() != NULL;
    }

    void openConsoleWindow()
    {
        if (!checkConsoleWindow())
        {
            if (!AllocConsole())
            {
                JOP_ASSERT(false, "Failed to allocate console window!");
                return;
            }

            _open_osfhandle(INT_PTR(GetStdHandle(STD_INPUT_HANDLE)), _O_TEXT);
            _open_osfhandle(INT_PTR(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);
            ns_oldInBuffer = *stdin;
            ns_oldOutBuffer = *stdout;

            FILE* pCout = nullptr;
            freopen_s(&pCout, "CONOUT$", "w", stdout);
            freopen_s(&pCout, "CONIN$", "r", stdin);
        }

        SetConsoleCtrlHandler(handleConsoleEvent, TRUE);
        EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);

        // Set custom color table
        COLORREF table[] =
        {
            RGB(0x00, 0x00, 0x00), // 1. Black
            RGB(0xFF, 0xFF, 0xFF), // 2. White
            RGB(0xFF, 0x00, 0x00), // 3. Red
            RGB(0x00, 0xFF, 0x00), // 4. Green
            RGB(0x00, 0x00, 0xFF), // 5. Blue
            RGB(0x00, 0xFF, 0xFF), // 6. Cyan
            RGB(0xFF, 0xFF, 0x00), // 7. Yellow
            RGB(0xFF, 0x00, 0xFF), // 8. Magenta
            RGB(0x80, 0x00, 0xFF), // 9. Purple
            RGB(0xFF, 0x80, 0x00), // 10. Orange
            RGB(0x80, 0x80, 0x80), // 11. Gray
            RGB(0x80, 0x64, 0x00), // 12. Brown

            0x00FFFFFF, // 13. White
            0x00FFFFFF, // 14. White
            0x00FFFFFF, // 15. White
            0x00FFFFFF, // 16. White
        };

        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFOEX info;
        std::memset(&info, 0, sizeof(info));
        info.cbSize = sizeof(info);

        if (!GetConsoleScreenBufferInfoEx(consoleHandle, &info))
            return;

        // Setting the horizontal size stops the horizontal scroll bar from appearing.
        info.dwMaximumWindowSize.X = 200;
        info.srWindow.Right = info.dwMaximumWindowSize.X;

        // Triple the vertical buffer size
        info.dwSize.Y = info.dwSize.Y * 3;

        std::memcpy(info.ColorTable, table, sizeof(table));

        SetConsoleScreenBufferInfoEx(consoleHandle, &info);
    }

    void setConsoleColor(const jop::Color& color)
    {
        using jop::Color;

        WORD attrib = 1; // White

        if (color == Color::White)
            attrib = 1;
        else if (color == Color::Red)
            attrib = 2;
        else if (color == Color::Green)
            attrib = 3;
        else if (color == Color::Blue)
            attrib = 4;
        else if (color == Color::Cyan)
            attrib = 5;
        else if (color == Color::Yellow)
            attrib = 6;
        else if (color == Color::Magenta)
            attrib = 7;
        else if (color == Color::Purple)
            attrib = 8;
        else if (color == Color::Orange)
            attrib = 9;
        else if (color == Color::Gray)
            attrib = 10;
        else if (color == Color::Brown)
            attrib = 11;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attrib);
    }

#endif

    std::ostringstream ns_stream;
    jop::DebugHandler::Severity ns_displaySeverity, ns_lastSeverity;
    bool ns_consoleEnabled;
}

namespace jop
{
    DebugHandler::DebugHandler()
    {
        ns_consoleEnabled = SettingManager::getBool("bConsoleEnabled", true);

        if (ns_consoleEnabled)
            openConsoleWindow();

        ns_displaySeverity = static_cast<Severity>(std::min(static_cast<unsigned int>(Severity::Info), SettingManager::getUint("uConsoleVerbosity", 0)));
    }

    //////////////////////////////////////////////

    std::ostringstream& DebugHandler::getStream()
    {
        return ns_stream;
    }

    //////////////////////////////////////////////

    DebugHandler& DebugHandler::getInstance()
    {
        static DebugHandler instance;
        return instance;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    bool DebugHandler::isConsoleEnabled()
    {
        return ns_consoleEnabled && checkConsoleWindow();
    }

    //////////////////////////////////////////////

    DebugHandler& DebugHandler::operator <<(const Severity severity)
    {
        ns_lastSeverity = severity;
        return *this;
    }

    //////////////////////////////////////////////

    DebugHandler& DebugHandler::operator <<(std::basic_ostream<char, std::char_traits<char>>& (*)(std::basic_ostream<char, std::char_traits<char>>&))
    {
        if (isConsoleEnabled() && ns_lastSeverity <= ns_displaySeverity)
        {
            std::cout << ns_stream.str() << '\n' << std::endl;
        }
        
        ns_stream.str("");
        ns_stream.clear();

        setConsoleColor(Color::White);

        return *this;
    }

    //////////////////////////////////////////////

    DebugHandler& DebugHandler::operator <<(const jop::Color& color)
    {
        if (isConsoleEnabled())
            setConsoleColor(color);

        return *this;
    }
}