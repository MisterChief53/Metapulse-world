
#include <AzCore/Serialization/SerializeContext.h>
#include "Metapulse-Auth-Client-gemEditorSystemComponent.h"

#include <Metapulse-Auth-Client-gem/Metapulse-Auth-Client-gemTypeIds.h>

namespace Metapulse_Auth_Client_gem
{
    AZ_COMPONENT_IMPL(Metapulse_Auth_Client_gemEditorSystemComponent, "Metapulse_Auth_Client_gemEditorSystemComponent",
        Metapulse_Auth_Client_gemEditorSystemComponentTypeId, BaseSystemComponent);

    void Metapulse_Auth_Client_gemEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<Metapulse_Auth_Client_gemEditorSystemComponent, Metapulse_Auth_Client_gemSystemComponent>()
                ->Version(0);
        }
    }

    Metapulse_Auth_Client_gemEditorSystemComponent::Metapulse_Auth_Client_gemEditorSystemComponent() = default;

    Metapulse_Auth_Client_gemEditorSystemComponent::~Metapulse_Auth_Client_gemEditorSystemComponent() = default;

    void Metapulse_Auth_Client_gemEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("Metapulse_Auth_Client_gemEditorService"));
    }

    void Metapulse_Auth_Client_gemEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("Metapulse_Auth_Client_gemEditorService"));
    }

    void Metapulse_Auth_Client_gemEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void Metapulse_Auth_Client_gemEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void Metapulse_Auth_Client_gemEditorSystemComponent::Activate()
    {
        Metapulse_Auth_Client_gemSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void Metapulse_Auth_Client_gemEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        Metapulse_Auth_Client_gemSystemComponent::Deactivate();
    }

} // namespace Metapulse_Auth_Client_gem
