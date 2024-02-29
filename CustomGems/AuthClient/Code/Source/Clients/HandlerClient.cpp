#include "HandlerClient.h"

int requestToken() {
	int token = -1;
	if (AuthClient::AuthClientHandlerRequests* handlerInterface = AZ::Interface<AuthClient::AuthClientHandlerRequests>::Get()) {
		token = handlerInterface->getToken();
	}
	return token;
}