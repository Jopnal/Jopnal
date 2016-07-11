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

#ifndef JOP_GOOGLEPLAYSERVICE_HPP
#define JOP_GOOGLEPLAYSERVICE_HPP

// Headers
#include <Jopnal/Header.hpp>

#ifdef JOP_OS_ANDROID


namespace jop
{
    class GooglePlayService
    {
    public:

        static void init();

        static void unlockAchievement(const char *achievementID);

        static void submitHighScore(const char *leaderBoardID, uint64_t score);

        static void showAchievements();

        static void showLeaderboard();

        static void getPlayerStats();

    private:

        // callback funcs for gpg
        void onAuthActionStarted(gpg::AuthOperation op);

        void onAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status);

        void fetchPlayerStats(gpg::StatsManager::FetchForPlayerResponse const &response);

        // m_gameService // unique ptr gpg::GameServices
        // m_androidApp // native activity app instance

    };

}

#endif
#endif