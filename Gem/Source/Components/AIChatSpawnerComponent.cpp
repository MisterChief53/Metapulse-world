#include "AIChatSpawnerComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiTextBus.h>
#include <LyShine/Bus/UiElementBus.h>

void metapulseWorld::AIChatSpawnerComponent::Init()
{
}

void metapulseWorld::AIChatSpawnerComponent::Activate()
{
	AIChatSpawnerBus::Handler::BusConnect();
	UiSpawnerNotificationBus::Handler::BusConnect(m_spawnerEntityId);
}

void metapulseWorld::AIChatSpawnerComponent::Deactivate()
{
	AIChatSpawnerBus::Handler::BusDisconnect();
	UiSpawnerNotificationBus::Handler::BusDisconnect(m_spawnerEntityId);
}

void metapulseWorld::AIChatSpawnerComponent::Reflect(AZ::ReflectContext* context)
{
	if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
		serializeContext->Class<AIChatSpawnerComponent, AZ::Component>()
			->Version(1)
			->Field("Spawner entity", &AIChatSpawnerComponent::m_spawnerEntityId)
			->Field("Message list", &AIChatSpawnerComponent::m_messagesList)
			;

		if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
			editContext->Class<AIChatSpawnerComponent>("AI Chat Spawner Component", "[Spawns messages specifically for AI chat]")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, "Accounts Server")
				->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("UI"))
				->DataElement(AZ::Edit::UIHandlers::Default, &AIChatSpawnerComponent::m_spawnerEntityId, "Spawner entity", "The id of the spawner")
				->DataElement(AZ::Edit::UIHandlers::Default, &AIChatSpawnerComponent::m_messagesList, "Mesasge List", "the id of the message list")
				;
		}
	}
}

void metapulseWorld::AIChatSpawnerComponent::SpawnMessage(AZStd::pair<size_t, AZStd::string> message)
{
	AzFramework::SliceInstantiationTicket messageInstantiationTicket;
	UiSpawnerBus::EventResult(messageInstantiationTicket, m_spawnerEntityId, &UiSpawnerBus::Events::Spawn);
	m_spawnMap[messageInstantiationTicket.GetRequestId()] = AZStd::make_pair(message.first, message.second);
}

void metapulseWorld::AIChatSpawnerComponent::ClearMessages()
{
	for (auto entityPair : m_itemMap) {
		UiElementBus::Event(entityPair.first, &UiElementBus::Events::DestroyElement);
	}
	m_itemMap = {};
}

void metapulseWorld::AIChatSpawnerComponent::OnEntitySpawned(const AzFramework::SliceInstantiationTicket& ticket, const AZ::EntityId& spawnedEntity)
{
	AZLOG_INFO("OnEntitySpawned triggered for a message");
	m_itemMap[spawnedEntity] = AZStd::make_pair(m_spawnMap[ticket.GetRequestId()].first, m_spawnMap[ticket.GetRequestId()].second);
	m_spawnMap.erase(ticket.GetRequestId());

	AZLOG_INFO("Text to set: %s", m_itemMap[spawnedEntity].second.c_str());

	UiTextBus::Event(spawnedEntity, &UiTextBus::Events::SetText, m_itemMap[spawnedEntity].second);

	UiElementBus::Event(spawnedEntity, &UiElementBus::Events::ReparentByEntityId, m_messagesList, AZ::EntityId());
}
