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

        // Trade notification bus overrides
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

#if AZ_TRAIT_SERVER
        void HandleRPC_ChangeHudValue(AzNetworking::IConnection* invokingConnection) override;
#endif


    protected:
        int m_hudValue = 0;
    };
}