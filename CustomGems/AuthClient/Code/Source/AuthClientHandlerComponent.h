
#pragma once

#include <AzCore/Component/Component.h>
#include <AuthClient/AuthClientHandlerInterface.h>
#include <AzCore/Interface/Interface.h>
#include "Clients/AuthClientInterfaceClient.h"
#include <Clients/AuthClientInterfaceImplementation.h>
#include <AuthClient/AuthClientHandlerInterface.h>
#include <AzCore/Console/ILogger.h>

namespace AuthClient
{
    /*
    * TODO: Register this component in your Gem's AZ::Module interface by inserting the following into the list of m_descriptors:
    *       AuthClientHandlerComponent::CreateDescriptor(),
    */

    class AuthClientHandlerComponent
        : public AZ::Component
        , public AuthClient::AuthClientHandlerRequests
        //, public AuthClientHandlerRequestBus::Handler
        //, public AZ::Interface<AuthClient::AuthClientHandlerRequests>::Registrar
    {
    public:
        // Define UUID for component
        AZ_COMPONENT(AuthClient::AuthClientHandlerComponent, "{04C56263-9B8E-4CC7-9B5E-3F42C6B7F0E3}", AZ::Component);
        
        AZStd::unique_ptr<AuthClient::AuthClientInterfaceImplementation> interface;

        AuthClientHandlerComponent();
        ~AuthClientHandlerComponent();

        //int token;

        /* ############## Interface overrides ################# */
        /*Request to get the token*/
        int getToken() override;
        int login() override;
        int signup() override;

        /*
        * Reflects component data into the reflection contexts, including the serialization, edit, and behavior contexts.
        */
        static void Reflect(AZ::ReflectContext* context);

        /*
        * Specifies the services that this component provides.
        * Other components can declare a dependency on these services. The system uses this
        * information to determine the order of component activation.
        */
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);

        /*
        * Specifies the services that this component cannot operate with.
        * For example, if two components provide a similar service and the system cannot use the
        * services simultaneously, each of those components would specify the other component as
        * an incompatible service.
        * If an entity cannot have multiple instances of this component, include this component's
        * provided service in the list of incompatible services.
        */
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

        /*
        * Specifies the services that this component requires.
        * The system activates the required services before it activates this component.
        * It also deactivates the required services after it deactivates this component.
        * If a required service is missing before this component is activated, the system
        * returns an error and does not activate this component.
        */
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        /*
        * Specifies the services that this component depends on, but does not require.
        * The system activates the dependent services before it activates this component.
        * It also deactivates the dependent services after it deactivates this component.
        * If a dependent service is missing before this component is activated, the system
        * does not return an error and still activates this component.
        */
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        /*
        * Puts this component into an active state.
        * The system calls this function once during activation of each entity that owns the
        * component. You must override this function. The system calls a component's Activate()
        * function only if all services and components that the component depends on are present
        * and active.
        */
        void Activate() override;

        /*
        * Deactivates this component.
        * The system calls this function when the owning entity is being deactivated. You must
        * override this function. As a best practice, ensure that this function returns this component
        * to a minimal footprint. The order of deactivation is the reverse of activation, so your
        * component is deactivated before the components it depends on.
        *
        * The system always calls a component's Deactivate() function before destroying the component.
        * However, deactivation is not always followed by the destruction of the component. An entity and
        * its components can be deactivated and reactivated without being destroyed. Ensure that your
        * Deactivate() implementation can handle this scenario.
        */
        void Deactivate() override;
    };
} // namespace AuthClient
