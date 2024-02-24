
#include <AuthClient/AuthClientTypeIds.h>
#include <AuthClientModuleInterface.h>
#include "AuthClientEditorSystemComponent.h"

namespace AuthClient
{
    class AuthClientEditorModule
        : public AuthClientModuleInterface
    {
    public:
        AZ_RTTI(AuthClientEditorModule, AuthClientEditorModuleTypeId, AuthClientModuleInterface);
        AZ_CLASS_ALLOCATOR(AuthClientEditorModule, AZ::SystemAllocator);

        AuthClientEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                AuthClientEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<AuthClientEditorSystemComponent>(),
            };
        }
    };
}// namespace AuthClient

AZ_DECLARE_MODULE_CLASS(Gem_AuthClient, AuthClient::AuthClientEditorModule)
