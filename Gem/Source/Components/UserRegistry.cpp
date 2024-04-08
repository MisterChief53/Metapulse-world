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
        m_userEntityIdMap[entityId.ToString()] = entityId;
        AZLOG_INFO("################################### new user registered ###################################");
    }
    void UserRegistryController::UnregisterUser(const AZStd::string& entityId)
    {
        if (m_userEntityIdMap.find(entityId) != m_userEntityIdMap.end()) {
            m_userEntityIdMap.erase(entityId);
        }
        else {
            AZLOG_ERROR("Could not find user to deregister from user registry");
        }
    }
    AZStd::unordered_map<AZStd::string, AZ::EntityId> UserRegistryController::GetUserMap()
    {
        return m_userEntityIdMap;
    }
}