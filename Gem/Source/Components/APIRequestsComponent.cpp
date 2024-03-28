#include <Components/APIRequestsComponent.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Console/ILogger.h>
#include <aws/core/http/HttpResponse.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace metapulseWorld {
	void APIRequestsComponent::Init() {

	}

	void APIRequestsComponent::Activate() {
		// Globally initialize curl functionality for windows systems. Only do this once
		// Per application lifetime
		//curl_global_init(CURL_GLOBAL_WIN32);
		//m_handle = curl_easy_init();
		APIRequestsBus::Handler::BusConnect();
	}

	void APIRequestsComponent::Deactivate() {
		//curl_easy_setopt(handle, CURLOPT_URL, "http://localhost:8080/");
		//curl_global_cleanup();
		APIRequestsBus::Handler::BusDisconnect();
	}

	void APIRequestsComponent::Reflect(AZ::ReflectContext* context) {
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
			serializeContext->Class<APIRequestsComponent, AZ::Component>()
				->Version(1)
				->Field("Accounts Server URL", &APIRequestsComponent::m_accountsServerUrl)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<APIRequestsComponent>("APIRequestsComponent", "[contains requests handing to metapulse accounts server]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "Accounts Server")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
					->DataElement(AZ::Edit::UIHandlers::Default, &APIRequestsComponent::m_accountsServerUrl)
					;
			}
		}
	}

	void APIRequestsComponent::setToken(const AZStd::string& token)
	{
		m_token = token;
	}

	AZStd::string APIRequestsComponent::getToken()
	{
		return m_token;
	}

	AZStd::string APIRequestsComponent::getUrl()
	{
		return m_accountsServerUrl;
	}

	void APIRequestsComponent::setUsername(const AZStd::string& username)
	{
		m_username = username;
	}

	AZStd::string APIRequestsComponent::getUsername()
	{
		return m_username;
	}

}