#pragma once

#include <Source/AutoGen/UserCamera.AutoComponent.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld
{

    class UserCameraController
        : public UserCameraControllerBase
        , public AZ::TickBus::Handler
    {
    public:
        explicit UserCameraController(UserCamera& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        void OnTick(float deltaTime, AZ::ScriptTimePoint) override;
    protected:
        AZ::Entity* m_activeCameraEntity = nullptr;
        AZ::Entity* GetActiveCamera();



    };
}