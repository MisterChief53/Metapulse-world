
#include "AuthClientModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <AuthClient/AuthClientTypeIds.h>

#include <Clients/AuthClientSystemComponent.h>

namespace AuthClient
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(AuthClientModuleInterface,
        "AuthClientModuleInterface", AuthClientModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(AuthClientModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(AuthClientModuleInterface, AZ::SystemAllocator);

    AuthClientModuleInterface::AuthClientModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            AuthClientSystemComponent::CreateDescriptor(),
            AuthClientHandlerComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList AuthClientModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<AuthClientSystemComponent>(),
        };
    }
} // namespace AuthClient
