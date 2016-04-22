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


#ifdef JOP_OS_WINDOWS
namespace
{
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
        }

        _open_osfhandle(INT_PTR(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);

        FILE* pCout = nullptr;
        freopen_s(&pCout, "CONOUT$", "w", stdout);

        std::cout.clear();

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
            RGB(0x99, 0x99, 0x99), // 11. Gray
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

        CONSOLE_FONT_INFOEX font;
        std::memset(&font, 0, sizeof(font));
        font.cbSize = sizeof(font);

        if (!GetConsoleScreenBufferInfoEx(consoleHandle, &info) || !GetCurrentConsoleFontEx(consoleHandle, FALSE, &font))
            return;

        std::memcpy(info.ColorTable, table, sizeof(table));

        font.dwFontSize.X = 5;
        font.dwFontSize.Y = 8;

        SetConsoleScreenBufferInfoEx(consoleHandle, &info);
        SetCurrentConsoleFontEx(consoleHandle, FALSE, &font);

        // Move console to second monitor if available
        glm::ivec2 pos(0);

        {
            glfwInit();

            int count;
            auto monitors = glfwGetMonitors(&count);

            for (int i = 0; i < count && count > 1; ++i)
            {
                if (monitors[i] != glfwGetPrimaryMonitor())
                {
                    glfwGetMonitorPos(monitors[i], &pos.x, &pos.y);

                    MoveWindow(GetConsoleWindow(), pos.x, pos.y, 1, 1, FALSE);

                    break;
                }
            }
        }

        // Set the console size
        {
            {
                auto consoleSize = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));
                consoleSize.X -= 7;
                consoleSize.Y = 1000;

                SetConsoleScreenBufferSize(consoleHandle, consoleSize);

                ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
            }

            {
                RECT consoleSize;
                GetWindowRect(GetConsoleWindow(), &consoleSize);

                MoveWindow(GetConsoleWindow(), pos.x + 5, pos.y + 5, consoleSize.right - consoleSize.left, consoleSize.bottom - consoleSize.top - 25, TRUE);
            }
        }
    }

    void closeConsoleWindow()
    {
        if (checkConsoleWindow())
        {
            ShowWindow(GetConsoleWindow(), SW_HIDE);
            FreeConsole();
        }
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
}
#endif

namespace jop
{
    DebugHandler::DebugHandler()
        : m_stream          (),
          m_displaySeverity (Severity::Diagnostic),
          m_lastSeverity    (Severity::Diagnostic),
          m_last            (),
          m_consoleEnabled  (false),
          m_noSpam          (true),
          m_debuggerOutput  (false),
          m_mutex           ()
    {
    #ifdef JOP_DEBUG_MODE
        setEnabled(true);
    #endif
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
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_consoleEnabled && checkConsoleWindow();
    }

    //////////////////////////////////////////////

    void DebugHandler::setEnabled(const bool enabled)
    {
        if (m_consoleEnabled != enabled)
        {
            enabled ? openConsoleWindow() : closeConsoleWindow();

            m_consoleEnabled = enabled;
        }
    }

    //////////////////////////////////////////////

    void DebugHandler::setVerbosity(const Severity severity)
    {
        m_displaySeverity = severity;
    }

    //////////////////////////////////////////////

    DebugHandler::Severity DebugHandler::getSeverity() const
    {
        return m_displaySeverity;
    }

    //////////////////////////////////////////////

    void DebugHandler::setReduceSpam(const bool set)
    {
        m_noSpam = set;
    }

    //////////////////////////////////////////////

    void DebugHandler::setDebuggerOutput(const bool set)
    {
        m_debuggerOutput = set;
    }

    //////////////////////////////////////////////

    DebugHandler& DebugHandler::operator <<(const Severity severity)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        m_lastSeverity = severity;
        return *this;
    }

    //////////////////////////////////////////////

    DebugHandler& DebugHandler::operator <<(std::basic_ostream<char, std::char_traits<char>>& (*)(std::basic_ostream<char, std::char_traits<char>>&))
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if ((isConsoleEnabled() || m_debuggerOutput) && m_lastSeverity <= m_displaySeverity)
        {
            std::string newStr(m_stream.str());

            if (!m_noSpam || m_last != newStr)
            {
                static const char* const severityStr[] =
                {
                    "ERROR:\t\t",
                    "WARNING:\t",
                    "INFO:\t\t",
                    "DIAG:\t\t"
                };

                const std::string baseStr = std::string("[JOPNAL] ") + severityStr[static_cast<int>(m_lastSeverity)];

                if (isConsoleEnabled())
                {
                    std::cout << baseStr << newStr << '\n' << std::endl;
                    setConsoleColor(Color::White);
                }

            #ifdef JOP_OS_WINDOWS
                if (m_debuggerOutput)
                    OutputDebugString((baseStr + newStr + '\n').c_str());
            #endif
            }

            m_last = newStr;
        }
        
        m_stream.str("");
        m_stream.clear();

        return *this;
    }

    //////////////////////////////////////////////

    DebugHandler& DebugHandler::operator <<(const jop::Color& color)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isConsoleEnabled())
            setConsoleColor(color);

        return *this;
    }
}