#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/utility/pair.h>

namespace metapulseWorld {
	/*
	* Interface specifically to communicate with the spawner that handle's
	* a player's communication with the AI NPC.
	* 
	* This works as a globally accessible EBus that you can invoke with Broadcast()
	*/
	class AIChatSpawnerInterface
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* Tells the spawner for the AI chat to spawn a message.
		* It receives a pair with the message's ID (from Accounts server)
		* and a string containing the message itself.
		*/
		virtual void SpawnMessage(AZStd::pair<size_t, AZStd::string> message) = 0;


		/*
		* Tells the messsage history to delete all the messages inside of it.
		* This is normally used whenever we want to update the message list and draw
		* all of them again. 
		*/
		virtual void ClearMessages() = 0;

	};

	using AIChatSpawnerBus = AZ::EBus<AIChatSpawnerInterface>;
}