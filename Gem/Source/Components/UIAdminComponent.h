#include <AzCore/Component/Component.h>
#include <StartingPointInput/InputEventNotificationBus.h>
#include <Components/Interfaces/UIAdminBus.h>

namespace metapulseWorld {

	const StartingPointInput::InputEventNotificationId ToggleIngameMenu("toggle_menu");

	class UIAdminComponent
		: public AZ::Component
		, public StartingPointInput::InputEventNotificationBus::MultiHandler 
		, public metapulseWorld::UIAdminBus::Handler
	{
	public:
		AZ_COMPONENT(metapulseWorld::UIAdminComponent, "{D7579DDA-4644-4F9F-8FB9-27239770AD34}", AZ::Component);

		// Component overrides
		void Init() override;

		void Activate() override;
		
		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// InputEvent notification bus overrides
		void OnPressed(float value) override;
		void OnReleased(float value) override;
		void OnHeld(float value) override;

		// UIAdminBus overrides
		void UnloadStartMenu() override;
		void LoadInventoryMenu() override;

	private:
		AZStd::string m_ingameMenuPath;
		AZ::EntityId m_ingameMenuEntityId;

		AZStd::string m_inventoryMenuPath;
		AZ::EntityId m_inventoryMenuEntityId;
	};
}