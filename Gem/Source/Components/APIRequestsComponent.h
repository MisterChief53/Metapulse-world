#include <AzCore/Component/Component.h>
#include <Components/Interfaces/APIRequestsBus.h>
//#include <curl/curl.h>
//#include <curl/easy.h>

namespace metapulseWorld {
	/*
	* This component holds necessary information for the system's HTTP requests
	* to be performed, as well as diverse operations related to a user's credentials
	*/
	class APIRequestsComponent
		: public AZ::Component
		, public metapulseWorld::APIRequestsBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::APIRequestsComponent, "{F0BFA415-E2D9-49BE-8B55-A1AD97F0C704}", AZ::Component);

		// Component Overrides
		void Init() override;

		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// Requests Bus overrides

		/*
		* This implementation sets the token to a member variable
		*/
		void setToken(const AZStd::string& token) override;

		/*
		* Returns the token attribute
		*/
		AZStd::string getToken() override;

		/*
		* Returns the URL
		*/
		AZStd::string getUrl() override;

		/*
		* Receives a username string and sets it.
		*/
		void setUsername(const AZStd::string& username) override;

		/*
		* Returns the username we set
		*/
		AZStd::string getUsername() override;

	private:
	
		/*
		* The server URL is reflected, since it will not change at runtime.
		*/
		AZStd::string m_accountsServerUrl;
		AZStd::string m_token;
		AZStd::string m_username;
	};
}