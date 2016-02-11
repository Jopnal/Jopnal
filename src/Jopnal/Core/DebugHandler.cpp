// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
#ifdef JOP_OS_WINDOWS

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
        HWND foregroundWindow = GetForegroundWindow();

        if (!checkConsoleWindow())
        {
            if (!AllocConsole())
            {
                JOP_ASSERT(false, "Failed to allocate console window!");
                return;
            }

            SetForegroundWindow(foregroundWindow);

            _open_osfhandle(INT_PTR(GetStdHandle(STD_INPUT_HANDLE)), _O_TEXT);
            _open_osfhandle(INT_PTR(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);

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

        std::memcpy(info.ColorTable, table, sizeof(table));

        SetConsoleScreenBufferInfoEx(consoleHandle, &info);

        // Set the console size
        COORD c;
        c.X = static_cast<SHORT>(jop::SettingManager::getUint("uConsoleWindowSizeX", 120));
        c.Y = static_cast<SHORT>(jop::SettingManager::getUint("uConsoleWindowSizeY", 80));

        SetConsoleScreenBufferSize(consoleHandle, c);
        ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

        // Restore the main window to the top
        SetForegroundWindow(foregroundWindow);
    }

    void closeConsoleWindow()
    {
        if (checkConsoleWindow())
            FreeConsole();
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
    std::string ns_last;
    bool ns_consoleEnabled;
}

namespace jop
{
    DebugHandler::DebugHandler()
    {
        ns_consoleEnabled = SettingManager::getBool("bConsoleEnabled", true);

        if (ns_consoleEnabled)
            openConsoleWindow();
        else
            closeConsoleWindow();

        const unsigned int defaultVerbosity =

        #ifdef JOP_DEBUG_MODE
            2;      
        #else
            0;
        #endif

        ns_displaySeverity = static_cast<Severity>(std::min(static_cast<unsigned int>(Severity::Info), SettingManager::getUint("uConsoleVerbosity", defaultVerbosity)));
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

    //////////////////////////////////////////////

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
        std::string newStr(ns_stream.str());

        static const bool debugConsole = SettingManager::getBool("bDebuggerOutput", false);

        if ((isConsoleEnabled() || debugConsole) && ns_lastSeverity <= ns_displaySeverity)
        {
            static const bool noSpam = SettingManager::getBool("bReduceConsoleSpam", true);

            if (!noSpam || ns_last != newStr)
            {
                if (isConsoleEnabled())
                {
                    std::cout << newStr << '\n' << std::endl;
                    setConsoleColor(Color::White);
                }

            #ifdef JOP_OS_WINDOWS
                if (debugConsole)
                    OutputDebugString((newStr + '\n').c_str());
            #endif
            }
        }

        ns_last = newStr;
        
        ns_stream.str("");
        ns_stream.clear();

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