
#include "AuthClientSystemComponent.h"

#include <AuthClient/AuthClientTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace AuthClient
{
    AZ_COMPONENT_IMPL(AuthClientSystemComponent, "AuthClientSystemComponent",
        AuthClientSystemComponentTypeId);

    void AuthClientSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AuthClientSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void AuthClientSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("AuthClientService"));
    }

    void AuthClientSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("AuthClientService"));
    }

    void AuthClientSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void AuthClientSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    AuthClientSystemComponent::AuthClientSystemComponent()
    {
        if (AuthClientInterface::Get() == nullptr)
        {
            AuthClientInterface::Register(this);
        }
    }

    AuthClientSystemComponent::~AuthClientSystemComponent()
    {
        if (AuthClientInterface::Get() == this)
        {
            AuthClientInterface::Unregister(this);
        }
    }

    void AuthClientSystemComponent::Init()
    {
    }

    void AuthClientSystemComponent::Activate()
    {
        AuthClientRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void AuthClientSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        AuthClientRequestBus::Handler::BusDisconnect();
    }

    void AuthClientSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace AuthClient
