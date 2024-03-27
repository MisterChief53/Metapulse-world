#include <AzCore/Component/Component.h>
#include <LyShine/Bus/UiDropTargetBus.h>

namespace metapulseWorld {
	class InventoryMenuComponent
		: public AZ::Component
		, public UiDropTargetNotificationBus::MultiHandler {
	public:
		AZ_COMPONENT(metapulseWorld::InventoryMenuComponent, "{9EAAE2FF-9265-4BA0-98EE-ADACEAE57911}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// Drop target overrides
		void OnDropHoverStart(AZ::EntityId draggable) override;
		void OnDropHoverEnd(AZ::EntityId draggable) override;
		void OnDrop(AZ::EntityId draggable) override;

	private:
		AZ::EntityId m_closeButtonEntityId;

		AZ::EntityId m_unequippedItemsListEntityId;
		AZ::EntityId m_unequippedDropTargetEntityId;

		AZ::EntityId m_equippedItemsListEntityId;
		AZ::EntityId m_equippedDropTargetEntityId;
	};
}