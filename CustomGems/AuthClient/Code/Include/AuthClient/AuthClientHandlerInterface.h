
#pragma once

#include <AzCore/Component/ComponentBus.h>
//#include <AzCore/Interface/Interface.h>

namespace AuthClient
{
    class AuthClientHandlerRequests
    {
    public:
        AZ_RTTI(AuthClient::AuthClientHandlerRequests, "{6D3726A1-F9D1-40A3-BCDC-7578DA19410E}");
        /*AZ_RTTI requires to specify virtual destructor!, however it does
        not makae sene
        to implement one, since this class in an interface, so the data 
        is not here*/
        //virtual ~AuthClientHandlerRequests() = 0;

        // Put your public request methods here.
        virtual int getToken() = 0;
        virtual int login() = 0;
        virtual int signup() = 0;
        // Put notification events here. Examples:
        // void RegisterEvent(AZ::EventHandler<...> notifyHandler);
        // AZ::Event<...> m_notifyEvent1;
        
    };

    //using AuthClientHandlerRequestBus = AZ::EBus<AuthClientHandlerRequests>;

} // namespace AuthClient
