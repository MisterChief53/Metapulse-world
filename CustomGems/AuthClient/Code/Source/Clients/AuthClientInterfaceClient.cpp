#include "AuthClientInterfaceClient.h"

int AuthClient::AuthClientInterfaceClient::requestToken() {
	int token = -1;
	if (AuthClient::AuthClientHandlerRequests* handlerInterface = AZ::Interface<AuthClient::AuthClientHandlerRequests>::Get()) {
		token = handlerInterface->getToken();
	}
	return token;
}

int AuthClient::AuthClientInterfaceClient::loginRequest()
{
	if (AuthClient::AuthClientHandlerRequests* handlerInterface = AZ::Interface<AuthClient::AuthClientHandlerRequests>::Get()) {
		return handlerInterface->login();
	}

	return -1;
}

int AuthClient::AuthClientInterfaceClient::signupRequest()
{
	if (AuthClient::AuthClientHandlerRequests* handlerInterface = AZ::Interface<AuthClient::AuthClientHandlerRequests>::Get()) {
		return handlerInterface->signup();
	}

	return -1;
}
