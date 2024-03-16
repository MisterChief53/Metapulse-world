#include <AzCore/Component/Component.h>
#include <Components/Interfaces/UIStatusBus.h>

namespace metapulseWorld {
	class StartMenuComponent
		: public AZ::Component
		, public metapulseWorld::UIStatusBus::Handler
	{
	public:

		AZ_COMPONENT(metapulseWorld::StartMenuComponent, "{A6B34B6B-B955-48AA-BF94-4321A1943323}", AZ::Component);

		// Component overrides
		void Init() override;

		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// UIStatusBus override
		bool canMove() override;

	protected:
		bool canPlayerMove;
	};
}