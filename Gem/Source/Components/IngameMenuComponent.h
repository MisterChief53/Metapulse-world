#include <AzCore/Component/Component.h>
#include <AzCore/Math/Vector2.h>
#include <LyShine/Bus/UiDropdownBus.h>

namespace metapulseWorld {

	class IngameMenuComponent
		: public AZ::Component
		, public UiDropdownNotificationBus::Handler {
	public:

		AZ_COMPONENT(metapulseWorld::IngameMenuComponent, "{E39BFD71-AFE5-4EF6-AAE9-95E117C228F4}", AZ::Component);

		// Component Overrides

		void Init() override;

		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// dropdown overrides

		void OnDropdownExpanded() override;

		void OnDropdownCollapsed() override;

		void OnDropdownValueChanged([[maybe_unused]] AZ::EntityId option) override;

	private:
		AZ::EntityId m_closeButtonEntityId;
		AZ::EntityId m_logoutButtonEntityId;
		AZ::EntityId m_fovSliderEntityId;
		AZ::EntityId m_resolutionDropdownEntityId;
		AZ::EntityId m_inventoryButtonEntityId;

		// Options for resolutions
		AZ::EntityId m_resolutionOption1;
		AZ::EntityId m_resolutionOption2;
		AZ::EntityId m_resolutionOption3;

		AZStd::string m_canvasPath;

		const AZStd::vector<AZStd::pair<uint32_t, uint32_t>> m_resolutionsVector = { {800, 600}, {1280, 720}, {1920,1080} };

		AZ::Entity* GetActiveCamera();

		// Button Setup
		void setupSlider();

		void setupInventoryButton();

	};
}