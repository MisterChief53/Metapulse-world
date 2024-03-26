#include <AzCore/Component/Component.h>
#include <AzCore/Math/Vector2.h>

namespace metapulseWorld {

	class IngameMenuComponent
		: public AZ::Component {
	public:

		AZ_COMPONENT(metapulseWorld::IngameMenuComponent, "{E39BFD71-AFE5-4EF6-AAE9-95E117C228F4}", AZ::Component);

		// Component Overrides

		void Init() override;

		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

	private:
		AZ::EntityId m_closeButtonEntityId;
		AZ::EntityId m_logoutButtonEntityId;
		AZ::EntityId m_fovSliderEntityId;

		AZStd::string m_canvasPath;

		AZ::Entity* GetActiveCamera();

		void setupSlider();

	};
}