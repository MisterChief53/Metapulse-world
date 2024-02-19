
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "Metapulse-worldSystemComponent.h"

#include <Metapulse-world/Metapulse-worldTypeIds.h>

namespace Metapulse_world
{
    class Metapulse_worldModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(Metapulse_worldModule, Metapulse_worldModuleTypeId, AZ::Module);
        AZ_CLASS_ALLOCATOR(Metapulse_worldModule, AZ::SystemAllocator);

        Metapulse_worldModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                Metapulse_worldSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<Metapulse_worldSystemComponent>(),
            };
        }
    };
}// namespace Metapulse_world

AZ_DECLARE_MODULE_CLASS(Gem_Metapulse_world, Metapulse_world::Metapulse_worldModule)
