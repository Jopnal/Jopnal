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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/DebugHandler.hpp>

    #include <Jopnal/Core/FileLoader.hpp>
    #include <Jopnal/Core/Win32/Win32.hpp>

    #ifdef JOP_OS_DESKTOP

        #include <GLFW/glfw3.h> 
        #include <iostream>

    #endif

    #if defined(JOP_OS_ANDROID)
        #include <android/log.h>

    #elif defined(JOP_OS_WINDOWS)

        #include <io.h>
        #include <fcntl.h>

    #endif

#endif

//////////////////////////////////////////////


namespace
{
#ifdef JOP_OS_WINDOWS

    BOOL WINAPI handleConsoleEvent(DWORD event)
    {
        return event == CTRL_C_EVENT;
    }

#endif

    bool checkConsoleWindow()
    {
    #if defined(JOP_OS_WINDOWS)
        return GetConsoleWindow() != NULL;
    #elif defined(JOP_OS_ANDROID)
        return true;
    #endif
    }

    void openConsoleWindow()
    {
    #if defined(JOP_OS_WINDOWS)

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
            0x00000000,

            RGB(0xFF, 0x00, 0x00), // 1. Red
            RGB(0xFF, 0xFF, 0x00), // 2. Yellow
            RGB(0xFF, 0xFF, 0xFF), // 3. White
            RGB(0x99, 0x99, 0x99), // 4. Gray
            RGB(0x00, 0x89, 0xFF), // 5. Cyan/blue

            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF,
            0x00FFFFFF
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

        if (IsWindows8OrGreater())
        {
            font.dwFontSize.X = 9;
            font.dwFontSize.Y = 12;
        }

        SetConsoleScreenBufferInfoEx(consoleHandle, &info);
        SetCurrentConsoleFontEx(consoleHandle, FALSE, &font);

        // Move console to second monitor if available
        glm::ivec2 pos(0);

        {
            glfwInit();

            int count;
            auto monitors = glfwGetMonitors(&count);

            for (int i = count - 1; i >= 0; --i)
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

                MoveWindow(GetConsoleWindow(), pos.x + 5 - (IsWindows8OrGreater() * 10), pos.y + 5, consoleSize.right - consoleSize.left - (IsWindows8OrGreater() * 6), consoleSize.bottom - consoleSize.top - 25, TRUE);
            }
        }

    #endif
    }

    void closeConsoleWindow()
    {
        if (checkConsoleWindow())
        {
        #if defined(JOP_OS_WINDOWS)

            ShowWindow(GetConsoleWindow(), SW_HIDE);
            FreeConsole();

        #endif
        }
    }

#if defined(JOP_OS_WINDOWS)

    void setConsoleColor(const jop::Color& color)
    {
        using jop::Color;

        WORD attrib = 3; // White

        if (color == Color::Red)
            attrib = 1;
        else if (color == Color::Yellow)
            attrib = 2;
        else if (color == Color::White)
            attrib = 3;
        else if (color == Color::Gray)
            attrib = 4;
        else if (color == Color::Cyan)
            attrib = 5;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attrib);
    }

#endif
}

namespace jop
{
    DebugHandler::DebugHandler()
        : m_stream          (),
          m_displaySeverity (Severity::Diagnostic),
          m_lastSeverity    (Severity::Diagnostic),
          m_last            (),
          m_consoleEnabled  (false),
          m_noSpam          (true),
          m_debuggerOutput  (true),
          m_fileLogging     (false),
          m_mutex           (),
          m_fileHandles     ()
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

    void DebugHandler::setFileLogging(const bool set)
    {
        m_fileLogging = set;
    }

    //////////////////////////////////////////////

    bool DebugHandler::fileLoggingEnabled() const
    {
    #ifdef JOP_OS_ANDROID
        return false;
    #else
        return m_fileLogging;
    #endif
    }

    //////////////////////////////////////////////

    std::recursive_mutex& DebugHandler::getMutex()
    {
        return m_mutex;
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

        if ((isConsoleEnabled() || m_debuggerOutput) && (m_lastSeverity <= m_displaySeverity || m_lastSeverity == Severity::__Always))
        {
            const std::string newStr(m_stream.str());

            if (!m_noSpam || m_last != newStr)
            {
                unsigned int severity = static_cast<unsigned int>(m_lastSeverity);

            #ifndef JOP_OS_ANDROID

                static const char* const severityStr[] =
                {
                    "ERROR:\t\t",
                    "WARNING:\t",
                    "INFO:\t\t",
                    "DIAG:\t\t",
                    ""
                };

                static const Color severityColor[] =
                {
                    Color::Red,
                    Color::Yellow,
                    Color::White,
                    Color::Gray,
                    Color::Cyan
                };

                const std::string baseStr = std::string("[JOPNAL] ") + severityStr[severity];
                const std::string finalString = baseStr + newStr + '\n';

            #endif

                if (isConsoleEnabled())
                {
                #ifdef JOP_OS_ANDROID

                    static const android_LogPriority androidSeverity[] =
                    {
                        ANDROID_LOG_ERROR,
                        ANDROID_LOG_WARN,
                        ANDROID_LOG_INFO,
                        ANDROID_LOG_VERBOSE,
                        ANDROID_LOG_INFO
                    };

                    __android_log_write(androidSeverity[severity], "jopnal", newStr.c_str());

                #else

                    setConsoleColor(severityColor[severity]);
                    std::cout << finalString << std::endl;

                #endif
                }

            #ifndef JOP_OS_ANDROID

                severity = std::min(static_cast<unsigned int>(Severity::Diagnostic), severity);

                if (fileLoggingEnabled() && m_fileHandles[severity].is_open())
                {
                    m_fileHandles[severity] << finalString;
                    m_fileHandles[severity].flush();
                }

            #endif

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

    void DebugHandler::openFileHandles()
    {
    #ifndef JOP_OS_ANDROID

        for (std::size_t i = 0; i < m_fileHandles.size(); ++i)
        {
            static const char* const filepath[] =
            {
                "error.log",
                "warning.log",
                "info.log",
                "diag.log"
            };

            const char sep = FileLoader::getDirectorySeparator();

            m_fileHandles[i].open((FileLoader::getDirectory(FileLoader::Directory::User) + sep + "Log" + sep + filepath[i]).c_str(), std::ios::out | std::ios::trunc);

            if (!m_fileHandles[i].good())
                JOP_DEBUG_ERROR("Failed to open log file \"" << filepath[i] << "\" for writing");
        }

    #endif
    }

    void DebugHandler::closeFileHandles()
    {
    #ifndef JOP_OS_ANDROID

        for (auto& i : m_fileHandles)
            i.close();

    #endif
    }
}