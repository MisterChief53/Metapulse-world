
#include "Metapulse-Auth-Client-gemModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <Metapulse-Auth-Client-gem/Metapulse-Auth-Client-gemTypeIds.h>

#include <Clients/Metapulse-Auth-Client-gemSystemComponent.h>

namespace Metapulse_Auth_Client_gem
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(Metapulse_Auth_Client_gemModuleInterface,
        "Metapulse_Auth_Client_gemModuleInterface", Metapulse_Auth_Client_gemModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(Metapulse_Auth_Client_gemModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(Metapulse_Auth_Client_gemModuleInterface, AZ::SystemAllocator);

    Metapulse_Auth_Client_gemModuleInterface::Metapulse_Auth_Client_gemModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            Metapulse_Auth_Client_gemSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList Metapulse_Auth_Client_gemModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<Metapulse_Auth_Client_gemSystemComponent>(),
        };
    }
} // namespace Metapulse_Auth_Client_gem
