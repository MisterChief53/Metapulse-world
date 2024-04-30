#pragma once

#include <Source/AutoGen/UserRegistry.AutoComponent.h>
#include <Components/Interfaces/UserRegistryBus.h>

namespace metapulseWorld
{
    /*
    * Component used to read the information from the userregistry network
    * component, and perform client side operations like sending data
    * to clients.
    */
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

        /*
        * Get the entityId of the first player that is connected, in string form.
        */
        AZStd::string BusGetPlayer1() override;

        /*
        * Get the entityId if the second player connected, in string form.
        */
        AZStd::string BusGetPlayer2() override;
        
    protected:
        
    };

    /*
    * Controller for the user registry multiplayer component. This controller is the one
    * that sets the user entityIds serverside so that they can be replicated to the clients
    */
    class UserRegistryController
        : public UserRegistryControllerBase
        , public UserRegistryBus::Handler
    {
    public:
        explicit UserRegistryController(UserRegistry& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        
        /*
        * Registers a user. At most, it can register 2 users.
        */
        void RegisterUser(const AZ::EntityId& entityId) override;

        /*
        * Removes a user's EntityId from the user Registry.
        */
        void UnregisterUser(const AZStd::string& entityId) override;
        
        
        
        
        
        
        
        
        

    protected:
        
        
        
        
    };
}