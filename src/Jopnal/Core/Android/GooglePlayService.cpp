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
//////////////////////////////////////////////


namespace jop
{ 

    void GooglePlayService::init()
    {
        if(m_service != nullptr)
            return; // service already exists!

        gpg::AndroidInitialization::android_main(m_androidApp);

        // Create new game services
    }

    void GooglePlayService::unlockAchievement(const char *achievementID)
    {

    }

    void GooglePlayService::submitHighScore(const char *leaderBoardID, uint64_t score)
    {

    }

    void GooglePlayService::showAchievements()
    {

    }

    void GooglePlayService::showLeaderboard()
    {

    }

    void GooglePlayService::getPlayerStats()
    {

    }

}

#endif