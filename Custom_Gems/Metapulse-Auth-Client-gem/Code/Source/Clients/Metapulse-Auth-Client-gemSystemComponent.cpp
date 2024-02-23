
#include "Metapulse-Auth-Client-gemSystemComponent.h"

#include <Metapulse-Auth-Client-gem/Metapulse-Auth-Client-gemTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace Metapulse_Auth_Client_gem
{
    AZ_COMPONENT_IMPL(Metapulse_Auth_Client_gemSystemComponent, "Metapulse_Auth_Client_gemSystemComponent",
        Metapulse_Auth_Client_gemSystemComponentTypeId);

    void Metapulse_Auth_Client_gemSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<Metapulse_Auth_Client_gemSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void Metapulse_Auth_Client_gemSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("Metapulse_Auth_Client_gemService"));
    }

    void Metapulse_Auth_Client_gemSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("Metapulse_Auth_Client_gemService"));
    }

    void Metapulse_Auth_Client_gemSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void Metapulse_Auth_Client_gemSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    Metapulse_Auth_Client_gemSystemComponent::Metapulse_Auth_Client_gemSystemComponent()
    {
        if (Metapulse_Auth_Client_gemInterface::Get() == nullptr)
        {
            Metapulse_Auth_Client_gemInterface::Register(this);
        }
    }

    Metapulse_Auth_Client_gemSystemComponent::~Metapulse_Auth_Client_gemSystemComponent()
    {
        if (Metapulse_Auth_Client_gemInterface::Get() == this)
        {
            Metapulse_Auth_Client_gemInterface::Unregister(this);
        }
    }

    void Metapulse_Auth_Client_gemSystemComponent::Init()
    {
    }

    void Metapulse_Auth_Client_gemSystemComponent::Activate()
    {
        Metapulse_Auth_Client_gemRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void Metapulse_Auth_Client_gemSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        Metapulse_Auth_Client_gemRequestBus::Handler::BusDisconnect();
    }

    void Metapulse_Auth_Client_gemSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace Metapulse_Auth_Client_gem
