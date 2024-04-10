#include "HUDComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <aws/core/http/HttpResponse.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiTextBus.h>
#include <LyShine/Bus/UiElementBus.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld {


	void HUDComponent::Init()
	{
	}

	void HUDComponent::Activate()
	{
		HUDBus::Handler::BusConnect();
		UiSpawnerNotificationBus::Handler::BusConnect(m_spawnerEntityId);
		AZ::TickBus::Handler::BusConnect();
	}

	void HUDComponent::Deactivate()
	{
		HUDBus::Handler::BusDisconnect();
		UiSpawnerNotificationBus::Handler::BusDisconnect();
		AZ::TickBus::Handler::BusDisconnect();
	}

	void HUDComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
		{
			serializeContext->Class<HUDComponent, AZ::Component>()
				->Version(1)
				->Field("Money Display Entity", &HUDComponent::m_moneyDisplayEntityId)
				->Field("Notification Spawner Entity", &HUDComponent::m_spawnerEntityId)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<HUDComponent>("HUD Component", "[Implements logic for the heads up display]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &HUDComponent::m_moneyDisplayEntityId, "Money Display Entity", "The Money Display's Entity Id")
					->DataElement(AZ::Edit::UIHandlers::Default, &HUDComponent::m_spawnerEntityId, "Notification Spawner Entity", "The Notification Spawner's Entity Id")
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
	void HUDComponent::SpawnNotification(const AZStd::string& text)
	{
		AZLOG_INFO("HUD spawning notification...");
		m_notificationText = text;
		UiSpawnerBus::Event(m_spawnerEntityId, &UiSpawnerBus::Events::Spawn);
	}
	void HUDComponent::OnSpawnBegin([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
	{
	}
	void HUDComponent::OnEntitySpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZ::EntityId&)
	{
	}
	void HUDComponent::OnEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
	{
	}
	void HUDComponent::OnTopLevelEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>& entities)
	{
		AZ::EntityId child;
		UiElementBus::EventResult(child, entities[0], &UiElementBus::Events::GetChildEntityId, 0);
		UiTextBus::Event(child, &UiTextBus::Events::SetText, m_notificationText);

	}
	void HUDComponent::OnSpawnEnd([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
	{
	}
	void HUDComponent::OnSpawnFailed([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
	{
	}
	void HUDComponent::OnTick([[maybe_unused]] float deltaTime, AZ::ScriptTimePoint time)
	{
		if (time.GetSeconds() - m_prevTime >= m_cooldown) {
			AZStd::string accountsServerUrl, token;
			APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
			APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
				accountsServerUrl + "/trade/hasRequest",
				Aws::Http::HttpMethod::HTTP_GET,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token}
					}),
				[]([[maybe_unused]] const Aws::Utils::Json::JsonView& json, [[maybe_unused]] Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Chekcking if user has request to trade...");
					Aws::Utils::Json::JsonView jsonResponse = json;
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						if (jsonResponse.GetBool("response")) {
							AZLOG_INFO("Can trade");
							HUDBus::Broadcast(&HUDBus::Events::SpawnNotification, "Trade requested");
						}
						else {
							AZLOG_INFO("Cannot trade");
						}
					}
					else {
						AZLOG_ERROR("Failed checking if user has request to trade...");
					}
				}
			);

			m_prevTime = time.GetSeconds();
		}
	}
	int HUDComponent::GetTickOrder()
	{
		return AZ::TICK_UI;
	}
}