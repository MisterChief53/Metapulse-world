#include "HUDComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <aws/core/http/HttpResponse.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiTextBus.h>

namespace metapulseWorld {


	void HUDComponent::Init()
	{
	}

	void HUDComponent::Activate()
	{
		HUDBus::Handler::BusConnect();
	}

	void HUDComponent::Deactivate()
	{
		HUDBus::Handler::BusDisconnect();
	}

	void HUDComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
		{
			serializeContext->Class<HUDComponent, AZ::Component>()
				->Version(1)
				->Field("Money Display Entity", &HUDComponent::m_moneyDisplayEntityId)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<HUDComponent>("HUD Component", "[Implements logic for the heads up display]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &HUDComponent::m_moneyDisplayEntityId, "Money Display Entity", "The Money Display's Entity Id")
					;
			}
		}
	}
	void HUDComponent::DisplayMoney()
	{
		AZStd::string accountsServerUrl, token;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

		HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
			accountsServerUrl + "/auth/userInfo",
			Aws::Http::HttpMethod::HTTP_GET,
			AZStd::map<AZStd::string, AZStd::string>({ 
				{"Authorization", token}
				}),
			[this](const Aws::Utils::Json::JsonView& json, Aws::Http::HttpResponseCode responseCode) {
				AZLOG_INFO("Executing display money callback...");
				if (responseCode == Aws::Http::HttpResponseCode::OK) {
					AZStd::string moneyString = AZStd::to_string((float)json.GetDouble("money"));

					// Ugly hack to get rid of extra zeroes
					moneyString.pop_back();
					moneyString.pop_back();
					moneyString.pop_back();
					moneyString.pop_back();

					UiTextBus::Event(m_moneyDisplayEntityId, &UiTextBus::Events::SetText, "Money: " + moneyString);
				}
				else {
					AZLOG_ERROR("Failed fetching user's money");
				}
			}
		);
	}
}