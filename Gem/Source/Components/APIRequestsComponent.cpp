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
		APIRequestsBus::Handler::BusConnect();
	}

	void APIRequestsComponent::Deactivate() {
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