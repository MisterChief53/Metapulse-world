
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "metapulseWorldSystemComponent.h"

#include <metapulseWorld/metapulseWorldTypeIds.h>

#include "Source/AutoGen/AutoComponentTypes.h"

#include <Components/StartMenuComponent.h>

#if AZ_TRAIT_CLIENT
#include <Components/APIRequestsComponent.h>
#include <Components/UIAdminComponent.h>
#include <Components/IngameMenuComponent.h>
#include <Components/InventoryMenuComponent.h>
#include <Components/HUDComponent.h>
#include <Components/UserMenuComponent.h>
#include <Components/NotificationComponent.h>
#include <Components/TradeMenuComponent.h>
#include <Components/OtherUserTradeSpawner.h>
#endif


namespace metapulseWorld
{
    class metapulseWorldModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(metapulseWorldModule, metapulseWorldModuleTypeId, AZ::Module);
        AZ_CLASS_ALLOCATOR(metapulseWorldModule, AZ::SystemAllocator);

        metapulseWorldModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                metapulseWorldSystemComponent::CreateDescriptor(),
                StartMenuComponent::CreateDescriptor(),
#if AZ_TRAIT_CLIENT
                APIRequestsComponent::CreateDescriptor(),
                UIAdminComponent::CreateDescriptor(),
                IngameMenuComponent::CreateDescriptor(),
                InventoryMenuComponent::CreateDescriptor(),
                HUDComponent::CreateDescriptor(),
                UserMenuComponent::CreateDescriptor(),
                NotificationComponent::CreateDescriptor(),
                TradeMenuComponent::CreateDescriptor(),
                OtherUserTradeSpawner::CreateDescriptor(),
#endif
            });

            CreateComponentDescriptors(m_descriptors);
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<metapulseWorldSystemComponent>(),
            };
        }
    };
}// namespace metapulseWorld

AZ_DECLARE_MODULE_CLASS(Gem_metapulseWorld, metapulseWorld::metapulseWorldModule)
