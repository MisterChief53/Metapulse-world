#include "NotificationComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiElementBus.h>
#include <AzCore/Console/ILogger.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <HttpRequestor/HttpRequestorBus.h>

void metapulseWorld::NotificationComponent::Init()
{
}

void metapulseWorld::NotificationComponent::Activate()
{
	m_prevTime = AZ::ScriptTimePoint().GetSeconds();
	AZ::TickBus::Handler::BusConnect();
}

void metapulseWorld::NotificationComponent::Deactivate()
{
	AZ::TickBus::Handler::BusDisconnect();
}

void metapulseWorld::NotificationComponent::Reflect(AZ::ReflectContext* context)
{
	if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
		serializeContext->Class<NotificationComponent, AZ::Component>()
			->Version(1)
			;

		if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
			editContext->Class<NotificationComponent>("NotificationComponent", "Manages behavior of the inventory menu")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, "UI")
				->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("UI"))
				;
		}
	}
}

void metapulseWorld::NotificationComponent::OnTick([[maybe_unused]] float deltaTime, AZ::ScriptTimePoint time)
{
	if (time.GetSeconds() - m_prevTime >= m_cooldown) {
		AZLOG_INFO("Deactivating notification...");
		UiElementBus::Event(this->GetEntityId(), &UiElementBus::Events::DestroyElement);
	}
}

int metapulseWorld::NotificationComponent::GetTickOrder()
{
	return AZ::TICK_LAST;
}
