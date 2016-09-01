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

#if 0
//#ifdef JOP_OS_ANDROID
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
namespace
{
    std::unique_ptr<gpg::GameServices> m_gameServices;
}
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

    void GooglePlayService::init(android_app *app, ANativeActivity *activity)
    {

        gpg::AndroidInitialization::android_main(app);
        

        if(m_gameServices != nullptr)
            return; // service already exists!

        
        gpg::AndroidPlatformConfiguration platform_config;
        platform_config.SetActivity(activity->clazz);

        // Create new game services
        gpg::GameServices::Builder builder;

        // Set callbacks & create
        m_gameServices = builder.SetDefaultOnLog(gpg::LogLevel::VERBOSE).
            SetOnAuthActionStarted(&onAuthActionStarted).
            SetOnAuthActionFinished(&onAuthActionFinished).
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
                //m_playerStats.averageSessionLength = player_stats.AverageSessionLength();
            }

            if(player_stats.HasDaysSinceLastPlayed())
            {
                //m_playerStats.daysSinceLastPlayed = player_stats.DaysSinceLastPlayed();
            }
            
            if(player_stats.HasNumberOfSessions())
            {
                // Sessions are determined by the number of times that a player signs in to gpg services
                //m_playerStats.numberOfSessions = player_stats.NumberOfSessions();
            }

            if (player_stats.HasSessionPercentile())
            {  
                // This value indicates how many sessions the current player has played in comparison to the rest of this game's player base
                //m_playerStats.sessionPercentile = player_stats.SessionPercentile();;
            }

            if(player_stats.HasChurnProbability())
            {
                // Churn is defined as 7 days of inactivity
                //m_playerStats.churnProbability = player_stats.ChurnProbability();
            }

            if(player_stats.HasNumberOfPurchases())
            {
                //m_playerStats.numberOfPurchases = player_stats.NumberOfPurchases();
            }

            if(player_stats.HasSpendPercentile())
            {
                // This value indicates how much the current player has spent in comparison to the rest of this game's player base
                //m_playerStats.spendPercentile = player_stats.SpendPercentile();
            }
                
        }
    }

}

#endif