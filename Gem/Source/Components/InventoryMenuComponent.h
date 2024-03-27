#include <AzCore/Component/Component.h>

namespace metapulseWorld {
	class InventoryMenuComponent
		: public AZ::Component {
	public:
		AZ_COMPONENT(metapulseWorld::InventoryMenuComponent, "{9EAAE2FF-9265-4BA0-98EE-ADACEAE57911}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

	private:
		AZ::EntityId m_closeButtonEntityId;
	};
}