#pragma once

#include <Source/AutoGen/User.AutoComponent.h>
#include <StartingPointInput/InputEventNotificationBus.h>
#include <AzFramework/Physics/CharacterBus.h>

namespace metapulseWorld
{

    // Input Event Ids for Player Controls (from input bindings!)
    const StartingPointInput::InputEventNotificationId MoveFwdEventId("move_fwd");
    const StartingPointInput::InputEventNotificationId MoveBackEventId("move_back");
    const StartingPointInput::InputEventNotificationId MoveLeftEventId("move_left");
    const StartingPointInput::InputEventNotificationId MoveRightEventId("move_right");

    class UserController
        : public UserControllerBase
        , private StartingPointInput::InputEventNotificationBus::MultiHandler
        , protected Physics::CharacterNotificationBus::Handler
    {
    public:

        explicit UserController(User& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        //! Common input creation logic for the NetworkInput.
        //! Fill out the input struct and the MultiplayerInputDriver will send the input data over the network
        //!    to ensure it's processed.
        //! @param input  input structure which to store input data for sending to the authority
        //! @param deltaTime amount of time to integrate the provided inputs over
        void CreateInput(Multiplayer::NetworkInput& input, float deltaTime) override;

        //! Common input processing logic for the NetworkInput.
        //! @param input  input structure to process
        //! @param deltaTime amount of time to integrate the provided inputs over
        void ProcessInput(Multiplayer::NetworkInput& input, float deltaTime) override;

    protected:




    };
}