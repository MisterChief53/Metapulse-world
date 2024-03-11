
#include <AzCore/Serialization/SerializeContext.h>

#include "metapulseWorldSystemComponent.h"

#include <metapulseWorld/metapulseWorldTypeIds.h>

#include <Source/AutoGen/AutoComponentTypes.h>

namespace metapulseWorld
{
    AZ_COMPONENT_IMPL(metapulseWorldSystemComponent, "metapulseWorldSystemComponent",
        metapulseWorldSystemComponentTypeId);

    void metapulseWorldSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<metapulseWorldSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void metapulseWorldSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("metapulseWorldService"));
    }

    void metapulseWorldSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("metapulseWorldService"));
    }

    void metapulseWorldSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void metapulseWorldSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    metapulseWorldSystemComponent::metapulseWorldSystemComponent()
    {
        if (metapulseWorldInterface::Get() == nullptr)
        {
            metapulseWorldInterface::Register(this);
        }
    }

    metapulseWorldSystemComponent::~metapulseWorldSystemComponent()
    {
        if (metapulseWorldInterface::Get() == this)
        {
            metapulseWorldInterface::Unregister(this);
        }
    }

    void metapulseWorldSystemComponent::Init()
    {
    }

    void metapulseWorldSystemComponent::Activate()
    {
        //! Register our gems multiplayer components to assign NetComponentIds
        RegisterMultiplayerComponents();

        metapulseWorldRequestBus::Handler::BusConnect();
    }

    void metapulseWorldSystemComponent::Deactivate()
    {
        metapulseWorldRequestBus::Handler::BusDisconnect();
    }
}
