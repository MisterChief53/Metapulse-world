#pragma once

#include <Source/AutoGen/User.AutoComponent.h>
#include <StartingPointInput/InputEventNotificationBus.h>
#include <Components/Interfaces/UserBus.h>
#include <AtomLyIntegration/CommonFeatures/Material/MaterialComponentBus.h>
#if AZ_TRAIT_CLIENT
#include <Components/Interfaces/ItemBus.h>
#endif

namespace metapulseWorld
{
    const StartingPointInput::InputEventNotificationId MoveFwdEventId("move_fwd");
    const StartingPointInput::InputEventNotificationId MoveRightEventId("move_right");
    const StartingPointInput::InputEventNotificationId RotateYawEventId("rotate_yaw");
    const StartingPointInput::InputEventNotificationId RotatePitchEventId("rotate_pitch");

    class UserController
        : public UserControllerBase
        , public StartingPointInput::InputEventNotificationBus::MultiHandler
        , public metapulseWorld::UserBus::Handler
#if AZ_TRAIT_CLIENT
        , public ItemBus::Handler
#endif
    {
    public:
        explicit UserController(User& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        //! Common input creation logic for the NetworkInput.
        //! Fill out the input struct and the MultiplayerInputDriver will send the input data over the network
        //!    to ensure it's processed.
        //! @param input  input structure which to store input data for sending to the authority
        //! @param deltaTime amount of time to integrate the provided inputs over
        void CreateInput(Multiplayer::NetworkInput& input, float deltaTime) override;

        //! Common input processing logic for the NetworkInput.
        //! @param input  input structure to process
        //! @param deltaTime amount of time to integrate the provided inputs over
        void ProcessInput(Multiplayer::NetworkInput& input, float deltaTime) override;

        // AZ::InputEventNotificationBus interface
        void OnPressed(float value) override;
        void OnReleased(float value) override;
        void OnHeld(float value) override;

        // UserBus Implementations
        float getPitchValue() override;

#if AZ_TRAIT_CLIENT
        // item bus overrides
        void executeItem(AZStd::string itemName) override;
#endif

    protected:
        void UpdateRotation(const UserNetworkInput* input);
        void UpdateVelocity(const UserNetworkInput* input);

        float m_forward = 0;
        float m_strafe = 0;
        float m_yaw = 0;
        float m_pitch = 0;
        float m_color = 0;

        // this is the transformation needed to be applied
        // to the camera to move it vertically
        float pitch_transform = 0;

        AZ::Vector3 m_velocity = AZ::Vector3::CreateZero();

        AZStd::unordered_map<AZStd::string, float> m_itemMap = { {"red", 0.255f}, {"blue", 0.5f}, {"green", 0.01f} };
    };

    class User
        : public UserBase
    {
    public:
        AZ_MULTIPLAYER_COMPONENT(metapulseWorld::User, s_userConcreteUuid, metapulseWorld::UserBase);

        static void Reflect(AZ::ReflectContext* context);

        void OnInit() override;
        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        User();


    protected:
        AZ::Color m_materialBaseColor;
        AZ::Event<float>::Handler m_ColorChanged;
        void OnColorChanged(float newColor);
        void UpdateMaterial(float newColor);
    };
}