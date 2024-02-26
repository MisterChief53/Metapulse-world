
#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace AuthClient
{
    class AuthClientHandlerRequests
        : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(AuthClient::AuthClientHandlerRequests, "{6D3726A1-F9D1-40A3-BCDC-7578DA19410E}");

        // Put your public request methods here.
        
        // Put notification events here. Examples:
        // void RegisterEvent(AZ::EventHandler<...> notifyHandler);
        // AZ::Event<...> m_notifyEvent1;
        
    };

    using AuthClientHandlerRequestBus = AZ::EBus<AuthClientHandlerRequests>;

} // namespace AuthClient
