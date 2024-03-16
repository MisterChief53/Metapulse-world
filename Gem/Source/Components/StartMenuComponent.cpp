#include "StartMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>

namespace metapulseWorld {

	void metapulseWorld::StartMenuComponent::Init()
	{
	}

	void metapulseWorld::StartMenuComponent::Activate()
	{
		metapulseWorld::UIStatusBus::Handler::BusConnect();
		canPlayerMove = false;
	}

	void metapulseWorld::StartMenuComponent::Deactivate()
	{
		canPlayerMove = true;
		metapulseWorld::UIStatusBus::Handler::BusDisconnect();
	}

	void StartMenuComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
		{
			serializeContext->Class<StartMenuComponent, AZ::Component>()
				->Version(1)
				//->Field("Token", &AuthClientHandlerComponent::token)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<StartMenuComponent>("StartMenuComponent", "[placeholder]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("UI"))
					//->DataElement(AZ::Edit::UIHandlers::Default, &AuthClientHandlerComponent::token, "Auth Token", "This auth token is used for constant auth with world server.")
					;
			}
		}
	}

	bool metapulseWorld::StartMenuComponent::canMove()
	{
		return canPlayerMove;
	}
}


