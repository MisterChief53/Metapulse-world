#include <AzCore/Component/Component.h>
#include <Components/Interfaces/HUDBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace metapulseWorld {
	class HUDComponent
		: public AZ::Component
		, public metapulseWorld::HUDBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::HUDComponent, "{5D064869-46DB-4BD8-9C41-019316F580F6}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// HUD Bus Overrides
		void DisplayMoney() override;

	public:
		AZ::EntityId m_moneyDisplayEntityId;

	};
}