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
#include <Jopnal/Core/Android/GooglePlayService.hpp>
#ifdef JOP_OS_ANDROID
#include <Jopnal/Core/DebugHandler.hpp>

#include <android/native_activity.h>
#include "gpg/achievement.h"
#include "gpg/achievement_manager.h"
#include "gpg/builder.h"
#include "gpg/default_callbacks.h"
#include "gpg/game_services.h"
#include "gpg/leaderboard.h"
#include "gpg/leaderboard_manager.h"
#include "gpg/platform_configuration.h"
#include "gpg/player_manager.h"
#include "gpg/score_page.h"
#include "gpg/types.h"
#include "gpg/playerstats.h"
#include "gpg/stats_manager.h"
#include "gpg/android_initialization.h"
//////////////////////////////////////////////


namespace jop
{ 
    /// \brief Callback function for OnAuthActionStarted
    ///
    static void onAuthActionStarted(gpg::AuthOperation op);

    /// \brief Callback function for OnAuthActionFinished
    ///
    static void onAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status);

    /// \brief Callback for fetching player stats
    ///
    static void fetchPlayerStats(gpg::StatsManager::FetchForPlayerResponse const &response);

    void GooglePlayService::init(struct android_app *app, struct ANativeActivity *activity)
    {

        gpg::AndroidInitialization::android_main(app);
        

        if(m_gameServices != nullptr)
            return; // service already exists!

        
        gpg::AndroidPlatformConfiguration platform_config;
        platform_config.SetActivity(activity->clazz);

        // Create new game services
        gpg::GameServices::Builder builder;

        // Set callbacks & create
        m_gameServices = builder.SetOnAuthActionStarted([](gpg::AuthOperation op) {onAuthActionStarted(op);}).
            SetOnAuthActionFinished([](gpg::AuthOperation op, gpg::AuthStatus status) {onAuthActionFinished(op, status);}).
            Create(platform_config);

    }

    //////////////////////////////////////////////

    void GooglePlayService::unlockAchievement(const char* achievementID)
    {
        if(m_gameServices->IsAuthorized())
        {
            m_gameServices->Achievements().Unlock(achievementID);
        }
    }

    //////////////////////////////////////////////

    void GooglePlayService::submitHighScore(const char* leaderboardID, uint64_t score)
    {
        if(m_gameServices->IsAuthorized())
        {
            m_gameServices->Leaderboards().SubmitScore(leaderboardID, score);
        }
    }

    //////////////////////////////////////////////

    void GooglePlayService::showAchievements()
    {
        if(m_gameServices->IsAuthorized())
        {
            m_gameServices->Achievements().ShowAllUI([](const gpg::UIStatus&){});
        }
    }

    //////////////////////////////////////////////

    void GooglePlayService::showLeaderboard(const char *leaderboardID)
    {
        if(m_gameServices->IsAuthorized())
        {
            m_gameServices->Leaderboards().ShowUI(leaderboardID,[](const gpg::UIStatus&){});
        }
    }

    //////////////////////////////////////////////

    void GooglePlayService::getPlayerStats()
    {
        if(m_gameServices->IsAuthorized())
        {
            m_gameServices->Stats().FetchForPlayer(&fetchPlayerStats);
        }
    }

    //////////////////////////////////////////////

    void onAuthActionStarted(gpg::AuthOperation op)
    {
        switch(op)
        {
        case gpg::AuthOperation::SIGN_IN:
            //Signing in;
            break;
        case gpg::AuthOperation::SIGN_OUT:
            //Signing out
            break;
        
        }
    }

    //////////////////////////////////////////////

    void onAuthActionFinished(gpg::AuthOperation op, gpg::AuthStatus status)
    {
        // OnAuthActionFinished

        if(op == gpg::AuthOperation::SIGN_IN)
        {
            // Do something after signing in?
            return;
        }
        else
        {
            // Do something after signing out?
        }
    }

    //////////////////////////////////////////////

    void fetchPlayerStats(gpg::StatsManager::FetchForPlayerResponse const &response)
    {
        if(IsError(response.status))
        {
            // error occurred
            return;
        }
        else
        {
            gpg::PlayerStats const& player_stats = response.data;

            if(player_stats.HasAverageSessionLength())
            {
                // The average session length of the player in minutes
                JOP_DEBUG_INFO("Average session length for player: (" << player_stats.AverageSessionLength() << ") minutes.\n");
            }

            if(player_stats.HasDaysSinceLastPlayed())
            {
                // The approximate number of days since the player last played
                JOP_DEBUG_INFO("It has been: (" << player_stats.DaysSinceLastPlayed() << ") days since player last played.\n");
            }
            
            if(player_stats.HasNumberOfSessions())
            {
                // The approximate number of sessions of the player
                // Sessions are determined by the number of times that a player signs in to gpg services
                JOP_DEBUG_INFO("Player has played: (" << player_stats.NumberOfSessions() << ") sessions.\n");
            }

            if (player_stats.HasSessionPercentile())
            {
                // The approximation of sessions percentile for the player, given as a decimal value between 0 and 1
                // This value indicates how many sessions the current player has played in comparison to the rest of this game's player base
                float percentile = player_stats.SessionPercentile() * 100;
                // 49> less 51<more
                JOP_DEBUG_INFO("This player has played: (" << percentile << "%) more/less in comparison to the rest of player base\n");
            }

            if(player_stats.HasChurnProbability())
            {
                // The prediction of whether a player will churn in the next day, given as 0 (low probability of churn) or 1 (high probability of churn)
                // Churn is defined as 7 days of inactivity
            }

            if(player_stats.HasNumberOfPurchases())
            {
                // The approximate number of in-app purchases for the player
                JOP_DEBUG_INFO("This player has made: " << player_stats.NumberOfPurchases() << " number of purchases in-game\n");
            }

            if(player_stats.HasSpendPercentile())
            {
                // The approximate spend percentile of the player, given as a decimal value between 0 and 1
                // This value indicates how much the current player has spent in comparison to the rest of this game's player base
            }
                
        }
    }

}

#endif