#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld {
	/*
	* This component controls a notification's behavior whenever it is spawned. It mainly
	* despawns it and sets its text at runtime.
	*/
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
		/*
		* The notification is spawned as soon as we reach the specified cooldown time,
		* set by m_cooldown
		*/
		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		/*
		* Since the notifications are low priority, we decide to use AZ::TICK_LAST
		* for this tick order
		*/
		int GetTickOrder() override;

	private:

		/*
		* Keeps track of the absolute time that has passed, so that we have something to
		* compare the current time to.
		*/
		double m_prevTime = 0;

		/*
		* The cooldown that sets how long the notification will last.
		*/
		const double m_cooldown = 3;
	};
}