
#include <AzCore/Serialization/SerializeContext.h>
#include "AuthClientEditorSystemComponent.h"

#include <AuthClient/AuthClientTypeIds.h>

namespace AuthClient
{
    AZ_COMPONENT_IMPL(AuthClientEditorSystemComponent, "AuthClientEditorSystemComponent",
        AuthClientEditorSystemComponentTypeId, BaseSystemComponent);

    void AuthClientEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AuthClientEditorSystemComponent, AuthClientSystemComponent>()
                ->Version(0);
        }
    }

    AuthClientEditorSystemComponent::AuthClientEditorSystemComponent() = default;

    AuthClientEditorSystemComponent::~AuthClientEditorSystemComponent() = default;

    void AuthClientEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("AuthClientEditorService"));
    }

    void AuthClientEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("AuthClientEditorService"));
    }

    void AuthClientEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void AuthClientEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void AuthClientEditorSystemComponent::Activate()
    {
        AuthClientSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void AuthClientEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AuthClientSystemComponent::Deactivate();
    }

} // namespace AuthClient
