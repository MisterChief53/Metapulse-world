#include <Source/Components/UserRegistry.h>

#include <AzCore/Serialization/SerializeContext.h>

#include <AzCore/Console/ILogger.h>

namespace metapulseWorld
{
    void UserRegistry::Reflect(AZ::ReflectContext* context)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
        if (serializeContext)
        {
            serializeContext->Class<UserRegistry, UserRegistryBase>()
                ->Version(1);
        }
        UserRegistryBase::Reflect(context);
    }

    void UserRegistry::OnInit()
    {
    }

    void UserRegistry::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        UserRegistryGettersBus::Handler::BusConnect();
    }

    void UserRegistry::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        UserRegistryGettersBus::Handler::BusDisconnect();
    }

    AZStd::string UserRegistry::BusGetPlayer1()
    {
        AZLOG_INFO("Getting player 1");
        return GetPlayer1();
    }

    AZStd::string UserRegistry::BusGetPlayer2()
    {
        AZLOG_INFO("Getting player 2");
        return GetPlayer2();
    }

    UserRegistryController::UserRegistryController(UserRegistry& parent)
        : UserRegistryControllerBase(parent)
    {
    }

    void UserRegistryController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        UserRegistryBus::Handler::BusConnect();
    }

    void UserRegistryController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        UserRegistryBus::Handler::BusDisconnect();
    }
    void UserRegistryController::RegisterUser([[maybe_unused]] const AZ::EntityId& entityId)
    {
#if AZ_TRAIT_SERVER
        if (GetPlayer1().empty()) {
            SetPlayer1(entityId.ToString());
            AZLOG_INFO("Registered user: %s", GetPlayer1().c_str());
        }
        else if (GetPlayer2().empty()) {
            SetPlayer2(entityId.ToString());
            AZLOG_INFO("Registered user: %s", GetPlayer2().c_str());
        }
#endif
    }
    void UserRegistryController::UnregisterUser([[maybe_unused]] const AZStd::string& entityId)
    {
#if AZ_TRAIT_SERVER
        AZLOG_INFO("Unregistering a user!");
        if (entityId == GetPlayer1()) {
            SetPlayer1("");
        }
        else if (entityId == GetPlayer2()) {
            SetPlayer2("");
        }
#endif
    }
}