#include "UserMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <Components/Interfaces/UserRegistryBus.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <LyShine/Bus/UiElementBus.h>
#include <LyShine/Bus/UiTextBus.h>

void metapulseWorld::UserMenuComponent::Init()
{
}

void metapulseWorld::UserMenuComponent::Activate()
{
	UiButtonBus::Event(m_closeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
		[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
			AZLOG_INFO("Close ui callback triggered");
			AZ::EntityId canvasEntityId;
			// It sounds like an awful idea to directly pass the canvasPath as a hardcoded string, however,
			// currently trying to pass an outer string variable to this lambda that contains the string that the member
			// of this class has crashes evertyhing. 
			AZStd::string canvasPath = "assets/ui/user_selection.uicanvas";
			UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
			AZLOG_INFO("unloading canvas");
			UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
		});

	UiSpawnerNotificationBus::Handler::BusConnect(m_spawnerEntityId);

	/*UserRegistryBus::BroadcastResult(m_userVector, &UserRegistryBus::Events::GetUserVector);*/
	AZStd::string user;
	UserRegistryGettersBus::BroadcastResult(user, &UserRegistryGettersBus::Events::BusGetPlayer1);
	m_userVector.push_back(user);
	UserRegistryGettersBus::BroadcastResult(user, &UserRegistryGettersBus::Events::BusGetPlayer2);
	m_userVector.push_back(user);

	FetchUsers();

}

void metapulseWorld::UserMenuComponent::Deactivate()
{
	UiSpawnerNotificationBus::Handler::BusDisconnect(m_spawnerEntityId);
}

void metapulseWorld::UserMenuComponent::Reflect(AZ::ReflectContext* context)
{
	if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
	{
		serializeContext->Class<UserMenuComponent, AZ::Component>()
			->Version(1)
			->Field("Close Button Entity", &UserMenuComponent::m_closeButtonEntityId)
			->Field("User List Entity", &UserMenuComponent::m_userListEntityId)
			->Field("Spawner Entity", &UserMenuComponent::m_spawnerEntityId)
			;

		if (AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<UserMenuComponent>("User Menu Component", "[Implements logic for user menu]")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
				->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
				->DataElement(AZ::Edit::UIHandlers::Default, &UserMenuComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
				->DataElement(AZ::Edit::UIHandlers::Default, &UserMenuComponent::m_userListEntityId, "User List Entity Id", "The id of the user list to put users on")
				->DataElement(AZ::Edit::UIHandlers::Default, &UserMenuComponent::m_spawnerEntityId, "Spawner Entity", "The id of the spawner that spawns user buttons")
				;
		}
	}
}

void metapulseWorld::UserMenuComponent::OnSpawnBegin([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::UserMenuComponent::OnEntitySpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZ::EntityId& spawnedEntity)
{

	
}

void metapulseWorld::UserMenuComponent::OnEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
{
}

void metapulseWorld::UserMenuComponent::OnTopLevelEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>& spawnedEntities)
{
	AZ::EntityId child;
	UiElementBus::Event(spawnedEntities[0], &UiElementBus::Events::ReparentByEntityId, m_userListEntityId, AZ::EntityId());
	UiElementBus::EventResult(child, spawnedEntities[0], &UiElementBus::Events::GetChildEntityId, 0);
	UiTextBus::Event(child, &UiTextBus::Events::SetText, m_userQueue.front());
	m_userQueue.pop();
}

void metapulseWorld::UserMenuComponent::OnSpawnEnd(const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::UserMenuComponent::OnSpawnFailed(const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::UserMenuComponent::FetchUsers()
{
	AZLOG_INFO("Fetching the users for user list...");
	AzFramework::SliceInstantiationTicket itemInstantiationTicket;
	if (m_userVector.empty()) {
		AZLOG_INFO("User vector is empty!");
		return;
	}
	for (auto user : m_userVector) {
		if (!user.empty()) {
			AZLOG_INFO("Got user succesfully: %s!, now spawning...", user.c_str());
			m_userQueue.push(user);
			UiSpawnerBus::EventResult(itemInstantiationTicket, m_spawnerEntityId, &UiSpawnerBus::Events::Spawn);
		}
		else {
			AZLOG_INFO("User is empty!");
		}
	}
}
