#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace metapulseWorld {
	
	/*
	* Interface to communicate with the User component.
	* This is mainly used for internal communicatino within the 
	* same user component. 
	*/
	class UserInterface
		: public AZ::ComponentBus
	{
	public:

		/*
		* Gets the value of the pitch rotation of the whole component, in radians.
		*/
		virtual float getPitchValue() = 0;
	};

	using UserBus = AZ::EBus<UserInterface>;
}