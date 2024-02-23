
#include <Metapulse-Auth-Client-gem/Metapulse-Auth-Client-gemTypeIds.h>
#include <Metapulse-Auth-Client-gemModuleInterface.h>
#include "Metapulse-Auth-Client-gemEditorSystemComponent.h"

namespace Metapulse_Auth_Client_gem
{
    class Metapulse_Auth_Client_gemEditorModule
        : public Metapulse_Auth_Client_gemModuleInterface
    {
    public:
        AZ_RTTI(Metapulse_Auth_Client_gemEditorModule, Metapulse_Auth_Client_gemEditorModuleTypeId, Metapulse_Auth_Client_gemModuleInterface);
        AZ_CLASS_ALLOCATOR(Metapulse_Auth_Client_gemEditorModule, AZ::SystemAllocator);

        Metapulse_Auth_Client_gemEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                Metapulse_Auth_Client_gemEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<Metapulse_Auth_Client_gemEditorSystemComponent>(),
            };
        }
    };
}// namespace Metapulse_Auth_Client_gem

AZ_DECLARE_MODULE_CLASS(Gem_Metapulse_Auth_Client_gem, Metapulse_Auth_Client_gem::Metapulse_Auth_Client_gemEditorModule)
