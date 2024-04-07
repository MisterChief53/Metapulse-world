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
    void UserRegistryController::RegisterUser(const AZ::EntityId& entityId)
    {
        m_userEntityIdVector.push_back(entityId);
        AZLOG_INFO("new user registered");
    }
}