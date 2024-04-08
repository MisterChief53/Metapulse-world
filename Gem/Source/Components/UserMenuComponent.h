#include <AzCore/Component/Component.h>

namespace metapulseWorld {
	class UserMenuComponent
		: public AZ::Component {
	public:
		AZ_COMPONENT(metapulseWorld::UserMenuComponent, "{FFBE6B9C-B0F8-4467-A98C-1B4857D37765}", AZ::Component);

		// Component Overrides

		void Init() override;

		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

	private:
		AZ::EntityId m_closeButtonEntityId;
	};
}