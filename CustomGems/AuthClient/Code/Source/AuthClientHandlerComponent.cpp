
#include <AuthClientHandlerComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace AuthClient
{
    //AZ_COMPONENT_IMPL(AuthClientHandlerComponent, "AuthClientHandlerComponent", "{EE4780C2-5B98-4AEA-9FD2-A37AD662F3B5}");

    void AuthClientHandlerComponent::Activate()
    {
        AuthClientHandlerRequestBus::Handler::BusConnect(GetEntityId());
        this->token = 123;
    }

    void AuthClientHandlerComponent::Deactivate()
    {
        AuthClientHandlerRequestBus::Handler::BusDisconnect(GetEntityId());
    }

    void AuthClientHandlerComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AuthClientHandlerComponent, AZ::Component>()
                ->Version(1)
                ->Field("Token", &AuthClientHandlerComponent::token)
                ;

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<AuthClientHandlerComponent>("AuthClientHandlerComponent", "[Client to manage sign up/login connection through a token]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AuthClientHandlerComponent::token, "Auth Token", "This auth token is used for constant auth with world server.")
                    ;
            }
        }

        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<AuthClientHandlerComponent>("AuthClientHandler Component Group")
                ->Attribute(AZ::Script::Attributes::Category, "AuthClient Gem Group")
                ;
        }
    }

    void AuthClientHandlerComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("AuthClientHandlerComponentService"));
    }

    void AuthClientHandlerComponent::GetIncompatibleServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
    }

    void AuthClientHandlerComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void AuthClientHandlerComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }
} // namespace AuthClient
