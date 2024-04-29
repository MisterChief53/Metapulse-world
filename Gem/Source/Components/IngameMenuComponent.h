#include <AzCore/Component/Component.h>
#include <AzCore/Math/Vector2.h>
#include <LyShine/Bus/UiDropdownBus.h>

namespace metapulseWorld {
	/*
	* Component to control behavior of the ingame menu, which holds
	* basic graphics settings and is able to logout and trigger the 
	* inventory menu
	*/
	class IngameMenuComponent
		: public AZ::Component
		, public UiDropdownNotificationBus::Handler {
	public:

		AZ_COMPONENT(metapulseWorld::IngameMenuComponent, "{E39BFD71-AFE5-4EF6-AAE9-95E117C228F4}", AZ::Component);

		// Component Overrides

		void Init() override;

		/*
		* This component notably registers the logout button, that quits the client whenever
		* it is pressed.
		*/
		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// dropdown overrides

		void OnDropdownExpanded() override;

		void OnDropdownCollapsed() override;

		/*
		* Whenever the user selects one of the dropdown elements (in this case, one of many
		* resolutions), we use the console to set the new resolution, by also getting the
		* window handle.
		*/
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

		/*
		* This vector of pairs holds the currently supported resolutions
		*/
		const AZStd::vector<AZStd::pair<uint32_t, uint32_t>> m_resolutionsVector = { {800, 600}, {1280, 720}, {1920,1080} };

		/*
		* This returns the active camera, which is retrieved by getting the client's
		* active camera, of which there can only be one.
		*/
		AZ::Entity* GetActiveCamera();

		// Button Setup
		/*
		* Sets up the functionality of the FOV slider. We set a callback that tells the 
		* slider to change the FOV whenever the slider's value is changed.
		*/
		void setupSlider();

		/*
		* Set the callback to trigger the inventory menu and close this one.
		*/
		void setupInventoryButton();

	};
}