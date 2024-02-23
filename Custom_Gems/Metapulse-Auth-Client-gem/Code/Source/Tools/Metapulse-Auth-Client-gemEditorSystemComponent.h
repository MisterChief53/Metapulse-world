
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/Metapulse-Auth-Client-gemSystemComponent.h>

namespace Metapulse_Auth_Client_gem
{
    /// System component for Metapulse_Auth_Client_gem editor
    class Metapulse_Auth_Client_gemEditorSystemComponent
        : public Metapulse_Auth_Client_gemSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = Metapulse_Auth_Client_gemSystemComponent;
    public:
        AZ_COMPONENT_DECL(Metapulse_Auth_Client_gemEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        Metapulse_Auth_Client_gemEditorSystemComponent();
        ~Metapulse_Auth_Client_gemEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace Metapulse_Auth_Client_gem
