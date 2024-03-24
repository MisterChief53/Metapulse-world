#include <AzCore/Component/Component.h>
#include <Components/Interfaces/StartMenuBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace metapulseWorld {
	class StartMenuComponent
		: public AZ::Component
		, public metapulseWorld::StartMenuBus::Handler
	{
	public:

		AZ_COMPONENT(metapulseWorld::StartMenuComponent, "{A6B34B6B-B955-48AA-BF94-4321A1943323}", AZ::Component);

		// Component overrides
		void Init() override;

		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// StartMenuBus override
		void closeStartMenu() override;

		AZ::EntityId m_canvasEntity;

	private:

		AZ::EntityId m_loginButtonEntity;
		AZ::EntityId m_signupButtonEntity;
		AZ::EntityId m_statusTextEntityId;
		AZ::EntityId m_usernameInputTextEntityId;
		AZ::EntityId m_passwordInputTextEntityId;
		

		// We will assign each entity a callback function that will execute a custom function depending on the button's nature.
		// since we are handling login and signup, both of them will require username an password, and the function itself will
		// depend on if we are dealing with loginButtonEntity or signupButtonEntity
		void InitializeButton(AZ::EntityId buttonEntity, AZStd::function<void(AZ::EntityId&,AZ::EntityId&,AZ::EntityId&,AZ::EntityId&)> buttonUpdateFunc,
			AZ::EntityId& usernameInputTextEntityId,
			AZ::EntityId& passwordInputTextEntityId, 
			AZ::EntityId& statusTextEntityId, 
			AZ::EntityId& canvasEntity);

		static void OnLoginButtonPressed(AZ::EntityId& usernameInputTextEntityId,
			AZ::EntityId& passwordInputTextEntityId, AZ::EntityId& statusTextEntityId, AZ::EntityId& canvasEntity);

		void loginCallback(const AZStd::string& response, Aws::Http::HttpResponseCode responseCode);
	};
}