#pragma once

#include <Source/AutoGen/UserCamera.AutoComponent.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld
{
    /*
    * This is the controller for the UserCamera multiplayer component, that sets the rotation
    * for the camera independent of the entity's rotation
    */
    class UserCameraController
        : public UserCameraControllerBase
        , public AZ::TickBus::Handler
    {
    public:
        explicit UserCameraController(UserCamera& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        /*
        * On each tick, we retrieve the pitch rotation from the user component and
        * rotate the camera depending on that information.
        */
        void OnTick(float deltaTime, AZ::ScriptTimePoint) override;
    protected:
        AZ::Entity* m_activeCameraEntity = nullptr;
        /*
        * Returns the world's active camera. Even though this is in the controller,
        * it will only retrieve the client's rotation since the server does not have
        * an active camera, and the actions performed here are not replicated in there.
        */
        AZ::Entity* GetActiveCamera();
        float currentPitchValue = 0;
    };
}