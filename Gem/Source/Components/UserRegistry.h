#pragma once

#include <Source/AutoGen/UserRegistry.AutoComponent.h>
#include <Components/Interfaces/UserRegistryBus.h>

namespace metapulseWorld
{
    class UserRegistry
        : public UserRegistryBase
        , public UserRegistryGettersBus::Handler
    {
    public:
        AZ_MULTIPLAYER_COMPONENT(metapulseWorld::UserRegistry, s_userRegistryConcreteUuid, metapulseWorld::UserRegistryBase);

        static void Reflect(AZ::ReflectContext* context);

        void OnInit() override;
        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        
        AZStd::string BusGetPlayer1() override;
        AZStd::string BusGetPlayer2() override;
        
    protected:
        
    };

    class UserRegistryController
        : public UserRegistryControllerBase
        , public UserRegistryBus::Handler
    {
    public:
        explicit UserRegistryController(UserRegistry& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        
        
        void RegisterUser(const AZ::EntityId& entityId) override;

        void UnregisterUser(const AZStd::string& entityId) override;
        
        
        
        
        
        
        
        
        

    protected:
        
        
        
        
    };
}