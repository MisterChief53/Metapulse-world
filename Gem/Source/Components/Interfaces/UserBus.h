#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace metapulseWorld {
	class UserInterface
		: public AZ::ComponentBus
	{
	public:
		virtual float getPitchValue() = 0;
	};

	using UserBus = AZ::EBus<UserInterface>;
}