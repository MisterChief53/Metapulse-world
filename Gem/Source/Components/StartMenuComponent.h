#include <AzCore/Component/Component.h>
#include <Components/Interfaces/StartMenuBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace metapulseWorld {

	/*
	* Component that controls the behavior of the start menu, by defining what is
	* to happen whenever one clicks the signup or the login buttons, and on succesful
	* login, registering the necessary credentials in the engine so that subsequent
	* requests are easier.
	*/
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
		/*
		* This implementation deactivates the start menu by directly calling the 
		* Entity's methods.
		*/
		void closeStartMenu() override;

		AZ::EntityId m_canvasEntity;

	private:

		AZ::EntityId m_loginButtonEntity;
		AZ::EntityId m_signupButtonEntity;
		AZ::EntityId m_statusTextEntityId;
		AZ::EntityId m_usernameInputTextEntityId;
		AZ::EntityId m_passwordInputTextEntityId;
		
		/*
		* We will assign each entity a callback function that will execute a custom function depending on the button's nature.
		* since we are handling login and signup, both of them will require username an password, and the function itself will
		* depend on if we are dealing with loginButtonEntity or signupButtonEntity
		*/
		void InitializeButton(AZ::EntityId buttonEntity, AZStd::function<void(AZ::EntityId&,AZ::EntityId&,AZ::EntityId&,AZ::EntityId&)> buttonUpdateFunc,
			AZ::EntityId& usernameInputTextEntityId,
			AZ::EntityId& passwordInputTextEntityId, 
			AZ::EntityId& statusTextEntityId, 
			AZ::EntityId& canvasEntity);

		/*
		* This is the callback that will be assigned for the login button. We make a request
		* to the accounts server trying to login the user, and if it succeeds, we save the token
		* using the APIRequests bus and close the start menu. If it does not succeed, we 
		* display an error message in the UI
		*/
		static void OnLoginButtonPressed(AZ::EntityId& usernameInputTextEntityId,
			AZ::EntityId& passwordInputTextEntityId, AZ::EntityId& statusTextEntityId, AZ::EntityId& canvasEntity);

		/*
		* This is the callback that will be assigned to the signup button. We make a
		* request to the accounts server trying to register the user in the database.
		* If it succeds, we show a message communicat
		*/
		static void OnSignUpButtonPressed(AZ::EntityId& usernameInputTextEntityId,
			AZ::EntityId& passwordInputTextEntityId, AZ::EntityId& statusTextEntityId, [[maybe_unused]] AZ::EntityId& canvasEntity);
	};
}