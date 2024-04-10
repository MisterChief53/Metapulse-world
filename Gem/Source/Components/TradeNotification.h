#pragma once

#include <Source/AutoGen/TradeNotification.AutoComponent.h>
#include <Components/Interfaces/TradeNotificationBus.h>

namespace metapulseWorld
{
    class TradeNotification
        : public TradeNotificationBase
        , public TradeNotificationBus::Handler
    {
    public:
        AZ_MULTIPLAYER_COMPONENT(metapulseWorld::TradeNotification, s_tradeNotificationConcreteUuid, metapulseWorld::TradeNotificationBase);

        static void Reflect(AZ::ReflectContext* context);

        void OnInit() override;
        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        // Trade notificatin bus overrides

        Multiplayer::NetworkEntityHandle GetHandleBus() override;


    protected:

    };

    class TradeNotificationController
        : public TradeNotificationControllerBase
    {
    public:
        explicit TradeNotificationController(TradeNotification& parent);

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
        int m_hudValue = 0;
    };
}