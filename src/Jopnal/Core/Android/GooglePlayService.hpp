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
#include <memory>

struct android_app;
struct ANativeActivity;
namespace gpg
{
    class GameServices;
}
namespace jop
{
    class GooglePlayService
    {
    public:

        /// \brief Initialize google play services
        ///
        static void init(android_app *app,ANativeActivity* activity);

        /// \brief Unlock achievement
        ///
        /// \param achievementID Achievement ID in google play developer console
        ///
        static void unlockAchievement(const char* achievementID);

        /// \brief Submit high score to a leaderboard
        ///
        /// \param leadearboardID Leaderboard ID same as defined in google play developer console
        /// \param score Score to submit
        ///
        static void submitHighScore(const char* leaderboardID, uint64_t score);

        /// \brief Show all achievements (Default UI)
        ///
        static void showAchievements();

        /// \brief Show a leaderboard
        ///
        /// \param leaderboardID Leaderboard ID same as defined in google play developer console
        ///
        static void showLeaderboard(const char* leaderboardID);

        /// \brief Get player statistics
        ///
        /// Includes: average session length, churn probability, days since last played, number of purchases
        /// number of sessions, session percentile, spend percentile
        ///
        static void getPlayerStats();

    private:

        static std::unique_ptr<gpg::GameServices> m_gameServices;

    };

}

#endif
#endif