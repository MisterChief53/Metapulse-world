#pragma once

#include <AzCore/EBus/EBus.h>

namespace metapulseWorld {
	/*
	* Interface to interact with the HUD of the player.
	*/
	class HUDRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* This tells the HUD to fetch the player's money and display it.
		* This exists because we cannot get a player's valid money ammount until
		* they log in, so someone else will tell the HUD to fetch the money once 
		* it is appropiate (usually means that APIRequestsComponent has the necessary
		* credentials to fetch the right amount.
		*/
		virtual void DisplayMoney() = 0;

		/*
		* This tells the local HUD to spawn a notification in the top left 
		* with a certain text on it.
		* The notification spawned will last a few seconds before going away.
		*/
		virtual void SpawnNotification(const AZStd::string& text) = 0;
	};

	using HUDBus = AZ::EBus<HUDRequests>;
}