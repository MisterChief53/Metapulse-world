#include <AzCore/Component/Component.h>
#include <StartingPointInput/InputEventNotificationBus.h>
#include <Components/Interfaces/UIAdminBus.h>

namespace metapulseWorld {

	/*
	* These are bindings to the input events that we are concerned about. 
	* Meaning, the ones that will invoke the UIs the user needs.
	*/
	const StartingPointInput::InputEventNotificationId ToggleIngameMenu("toggle_menu");
	const StartingPointInput::InputEventNotificationId ToggleUserMenu("toggle_user_menu");
	const StartingPointInput::InputEventNotificationId ToggleTradeMenu("toggle_trade_menu");
	const StartingPointInput::InputEventNotificationId ToggleChatBox("toggle_chat_box");

	/*
	* Controls the invoking and deleting of various UI canvases the player can manage.
	*/
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
		
		/*
		* Tells us which action to perform depending on which input event has been pressed.
		* We have a few canvases that get invoked depending on the keybind set on the 
		* editor.
		*/
		void OnPressed(float value) override;
		void OnReleased(float value) override;
		void OnHeld(float value) override;

		// UIAdminBus overrides
		
		/*
		* Implementation that unloads the startmenu when it is invoked, as long as it 
		* is loaded.
		*/
		void UnloadStartMenu() override;

		/*
		* Loads the inventory menu if it is not loaded, and unloads the start menu, since we
		* are expected that whenever we want to load the inventory menu, we want to unload 
		* the start menu before.
		*/
		void LoadInventoryMenu() override;

	private:
		AZStd::string m_ingameMenuPath;
		AZ::EntityId m_ingameMenuEntityId;

		AZStd::string m_inventoryMenuPath;
		AZ::EntityId m_inventoryMenuEntityId;

		AZStd::string m_userMenuPath;
		AZ::EntityId m_userMenuEntityId;

		AZStd::string m_tradeMenuPath;
		AZ::EntityId m_tradeMenuEntityId;

		AZStd::string m_chatBoxPath;
		AZ::EntityId m_chatBoxEntityId;
	};
}