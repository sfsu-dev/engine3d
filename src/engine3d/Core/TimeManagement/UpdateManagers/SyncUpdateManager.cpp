#include "TimeManagement/UpdateManagers/SyncUpdateManager.hpp"
#include "EngineLogger.hpp"
#include "Event/KeyCodes.hpp"
#include <GLFW/glfw3.h>
#include <time.h>

namespace engine3d
{
    SyncUpdateManager::SyncUpdateManager()
    {
        m_LocalTimer = new Timer();
        m_keyEvent = new InputPoll();
        m_localUpdateTime = m_LocalTimer->getCurrentTime();

        m_localDeltaTime = 0.0;
        m_maxVariance = 2;
        m_minFrames = 0;
        m_localUpdateCounter = 0;
        m_localFPS = 0;
        srand(std::time(NULL));
        m_randomFrame = (rand() % m_maxVariance) + m_minFrames;

        test = 0;
    }

    //! @note this does not work per object this might need to change a little.
    //! Possible pass gameObjects with the virtual functions.
    //! Possibly seperate active scripts to non active ones in scenes.
    void SyncUpdateManager::runUpdate(float deltaTime, SyncUpdate* start, SyncUpdate* end)
    {
        //! @note unsafe!!!!
        for(auto it1 = start; it1 != end; it1++)
        {
            it1->runTickUpdate(deltaTime);
        }
        /** 
        * @note Render is not quite placed in the right position here
        * @param randomFrame Used both to determine update and
        * used to allow render to optimise gpu sending if needed
        *
        * @note I used random in this case becuase it is hard for a
        * human to catch wether the render is correct or not.
        * Benchmark later.
        */
        render(m_localDeltaTime, m_randomFrame);
        if(m_randomFrame <= m_localUpdateCounter)
        {
            m_randomFrame = (rand() % m_maxVariance) + m_minFrames;
            m_localUpdateCounter = 0;

            update(m_localDeltaTime);
            lateUpdate(m_localDeltaTime);

            m_localDeltaTime = duration_cast<std::chrono::microseconds>
                (m_LocalTimer->getCurrentTime() - m_localUpdateTime).count();

            m_localUpdateTime = m_LocalTimer->getCurrentTime();
            m_localFPS++;

            //! @note for showcase purposes
            // for(int i = 0; i < 1350; i++)
            // {
            //     for(int j = 0; j < 10000; j++){
            //         test++;
            //     }
            // }
        }
        else 
        {
            m_localUpdateCounter++;
        }

        if(m_LocalTimer->elapsedSec() >= 1.0)
        {
            m_LocalTimer->reset();

            //! @note Key event added to allow switch between global and local.
            if(m_keyEvent->IsKeyPressed(KeyCode::F2))
            {
                ConsoleLogInfo("Local FPS: {0}, Local Delta Time: {1}",
                    m_localFPS, m_localDeltaTime);
            }
            m_localFPS = 0;
        }
        
    }

    SyncUpdateManager::~SyncUpdateManager()
    {
        delete m_LocalTimer;
        delete m_keyEvent;
    }
}