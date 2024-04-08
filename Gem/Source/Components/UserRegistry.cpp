/// Place in your .cpp
#include <Source/Components/UserRegistry.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace metapulseWorld
{
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
        }
        else if (GetPlayer2().empty()) {
            SetPlayer2(entityId.ToString());
        }
        else {
            AZLOG_ERROR("Cannot register more users!");
            return;
        }
        AZLOG_INFO("################################### new user registered ###################################");
#endif
    }
    void UserRegistryController::UnregisterUser([[maybe_unused]] const AZStd::string& entityId)
    {
#if AZ_TRAIT_SERVER
        if (entityId == GetPlayer1()) {
            SetPlayer1("");
        }
        else if (entityId == GetPlayer2()) {
            SetPlayer2("");
        }
        else {
            AZLOG_ERROR("Could not find user to deregister from user registry");
            return;
        }

        AZLOG_INFO("User logged out succesfully");
#endif

    }
    AZStd::string UserRegistryController::BusGetPlayer1()
    {
        AZLOG_INFO("First user: %s", GetPlayer1().c_str());
        return GetPlayer1();
    }
    AZStd::string UserRegistryController::BusGetPlayer2()
    {
        return GetPlayer2();
    }
}