
#include <AuthClientHandlerComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace AuthClient
{
    /*int AuthClientHandlerComponent::getToken() {
        return this->token;
    }*/

    void AuthClientHandlerComponent::Activate()
    {
        //AuthClientHandlerRequestBus::Handler::BusConnect(GetEntityId());
        //this->token = 123;
        //this->interface = AZStd::make_unique<AuthClient::AuthClientInterfaceImplementation>(); //call the constructor
    }

    void AuthClientHandlerComponent::Deactivate()
    {
        //AuthClientHandlerRequestBus::Handler::BusDisconnect(GetEntityId());
        //this->interface.reset();
    }

    AuthClientHandlerComponent::AuthClientHandlerComponent()
    {
        if (AZ::Interface<AuthClient::AuthClientHandlerRequests>::Get() == nullptr) {
            AZLOG_INFO("Auth interface has been registered");
            AZ::Interface<AuthClient::AuthClientHandlerRequests>::Register(this);
        }
    }

    AuthClientHandlerComponent::~AuthClientHandlerComponent()
    {
        if (AZ::Interface<AuthClient::AuthClientHandlerRequests>::Get() == this) {
            AZLOG_INFO("Auth interface has been unregistered");
            AZ::Interface<AuthClient::AuthClientHandlerRequests>::Unregister(this);
        }
    }

    int AuthClientHandlerComponent::getToken()
    {
        return 123;
    }

    int AuthClientHandlerComponent::login()
    {
        return 0;
    }

    int AuthClientHandlerComponent::signup()
    {
        return 0;
    }

    void AuthClientHandlerComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AuthClientHandlerComponent, AZ::Component>()
                ->Version(1)
                //->Field("Token", &AuthClientHandlerComponent::token)
                ;

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<AuthClientHandlerComponent>("AuthClientHandlerComponent", "[Client to manage sign up/login connection through a token]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    //->DataElement(AZ::Edit::UIHandlers::Default, &AuthClientHandlerComponent::token, "Auth Token", "This auth token is used for constant auth with world server.")
                    ;
            }
        }

        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<AuthClientHandlerComponent>("AuthClientHandler Component Group")
                ->Attribute(AZ::Script::Attributes::Category, "AuthClient Gem Group")
                ;
            behaviorContext->Class<AuthClient::AuthClientInterfaceClient>()
                ->Method("RequestToken", &AuthClient::AuthClientInterfaceClient::requestToken)
                ->Method("LoginRequest", &AuthClient::AuthClientInterfaceClient::loginRequest)
                ->Method("SignUpRequest",&AuthClient::AuthClientInterfaceClient::signupRequest);
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
