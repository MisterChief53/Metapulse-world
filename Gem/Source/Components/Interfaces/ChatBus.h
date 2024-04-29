#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/EntityId.h>

namespace metapulseWorld {
	/*
	* Interface to interact with the UI ChatBox component. It is mainly used to manage
	* the chatbox's state when switching between the tabs of the chat.
	*/
	class ChatRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* Returns an EntityId pointing to the entity that holds the active chat.
		*/
		virtual AZ::EntityId GetActiveChat() = 0;

		/*
		* Sets the active chat. It is expected that one sends the ID of the chat
		* that is supposed to now be the active one.
		*/
		virtual void SetActiveChat(AZ::EntityId newActiveChat) = 0;
	};

	using ChatBus = AZ::EBus<ChatRequests>;
}