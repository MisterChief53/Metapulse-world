#include <Source/Components/TradeNotification.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Console/ILogger.h>

namespace metapulseWorld
{
    void TradeNotification::Reflect(AZ::ReflectContext* context)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
        if (serializeContext)
        {
            serializeContext->Class<TradeNotification, TradeNotificationBase>()
                ->Version(1);
        }
        TradeNotificationBase::Reflect(context);
    }

    void TradeNotification::OnInit()
    {
    }

    void TradeNotification::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        TradeNotificationBus::Handler::BusConnect();
    }

    void TradeNotification::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        TradeNotificationBus::Handler::BusDisconnect();
    }

    Multiplayer::NetworkEntityHandle TradeNotification::GetHandleBus()
    {
        AZLOG_INFO("Triggered get handle bus function");
        return this->GetEntityHandle();
    }

    TradeNotificationController::TradeNotificationController(TradeNotification& parent)
        : TradeNotificationControllerBase(parent)
    {
    }

    void TradeNotificationController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }

    void TradeNotificationController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }


#if AZ_TRAIT_SERVER
    void TradeNotificationController::HandleRPC_ChangeHudValue([[maybe_unused]] AzNetworking::IConnection* invokingConnection) {
        AZLOG_INFO("RPC invoked");
    }
#endif
}