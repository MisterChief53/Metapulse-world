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

		static void GerRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

		// APIRequestsBus overrides
		void login(AZStd::string& responseText, bool& succeed,
			const AZStd::string& username, const AZStd::string& password) override;

		// curl functions
		//static size_t WriteMemoryCallback(char* contents, size_t size, size_t nmemb, std::string* userp);
		AZStd::string m_token;
	private:
		//CURL* m_handle;
		
		const AZStd::string m_accountsServerUrl = "http://localhost:8080";
	};
}