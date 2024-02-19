
#include <AzCore/Serialization/SerializeContext.h>

#include "Metapulse-worldSystemComponent.h"

#include <Metapulse-world/Metapulse-worldTypeIds.h>

namespace Metapulse_world
{
    AZ_COMPONENT_IMPL(Metapulse_worldSystemComponent, "Metapulse_worldSystemComponent",
        Metapulse_worldSystemComponentTypeId);

    void Metapulse_worldSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<Metapulse_worldSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void Metapulse_worldSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("Metapulse_worldService"));
    }

    void Metapulse_worldSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("Metapulse_worldService"));
    }

    void Metapulse_worldSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void Metapulse_worldSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    Metapulse_worldSystemComponent::Metapulse_worldSystemComponent()
    {
        if (Metapulse_worldInterface::Get() == nullptr)
        {
            Metapulse_worldInterface::Register(this);
        }
    }

    Metapulse_worldSystemComponent::~Metapulse_worldSystemComponent()
    {
        if (Metapulse_worldInterface::Get() == this)
        {
            Metapulse_worldInterface::Unregister(this);
        }
    }

    void Metapulse_worldSystemComponent::Init()
    {
    }

    void Metapulse_worldSystemComponent::Activate()
    {
        Metapulse_worldRequestBus::Handler::BusConnect();
    }

    void Metapulse_worldSystemComponent::Deactivate()
    {
        Metapulse_worldRequestBus::Handler::BusDisconnect();
    }
}
