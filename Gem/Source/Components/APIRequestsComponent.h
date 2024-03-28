#include <AzCore/Component/Component.h>
#include <Components/Interfaces/APIRequestsBus.h>
//#include <curl/curl.h>
//#include <curl/easy.h>

namespace metapulseWorld {
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
		void setToken(const AZStd::string& token) override;

		AZStd::string getToken() override;

		AZStd::string getUrl() override;

		void setUsername(const AZStd::string& username) override;

		AZStd::string getUsername() override;

	private:
	
		AZStd::string m_accountsServerUrl;
		AZStd::string m_token;
		AZStd::string m_username;
	};
}