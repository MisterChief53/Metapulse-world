#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld {
	class NotificationComponent
		: public AZ::Component
		, public AZ::TickBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::NotificationComponent, "{049FB77F-81DC-4EE5-848D-27D4D038CFEE}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// Tick bus overrides

		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		int GetTickOrder() override;

	private:
		double m_prevTime = 0;
		const double m_cooldown = 3;
	};
}