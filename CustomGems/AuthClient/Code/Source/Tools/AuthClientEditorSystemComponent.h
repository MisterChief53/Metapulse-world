
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/AuthClientSystemComponent.h>

namespace AuthClient
{
    /// System component for AuthClient editor
    class AuthClientEditorSystemComponent
        : public AuthClientSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = AuthClientSystemComponent;
    public:
        AZ_COMPONENT_DECL(AuthClientEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        AuthClientEditorSystemComponent();
        ~AuthClientEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace AuthClient
