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
    /*
    * These are the input bindings that we set on editor which are relevant for
    * user movement and control.
    */
    const StartingPointInput::InputEventNotificationId MoveFwdEventId("move_fwd");
    const StartingPointInput::InputEventNotificationId MoveRightEventId("move_right");
    const StartingPointInput::InputEventNotificationId RotateYawEventId("rotate_yaw");
    const StartingPointInput::InputEventNotificationId RotatePitchEventId("rotate_pitch");

    /*
    * Controller for the User multiplayer component. Handles server side and autonomous side
    * item execution logic and player movement.
    */
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
        //! 
        //! We only set the color inputs client-side, since that is where
        //! the color variables are going to be manipulated
        void CreateInput(Multiplayer::NetworkInput& input, float deltaTime) override;

        //! Common input processing logic for the NetworkInput.
        //! @param input  input structure to process
        //! @param deltaTime amount of time to integrate the provided inputs over
        //! 
        //! For the items, the server is the one who is going to change the colors 
        //! from the input changes it god, triggering the replication to all clients
        void ProcessInput(Multiplayer::NetworkInput& input, float deltaTime) override;

        // AZ::InputEventNotificationBus interface
        /*
        * We update translation values on "pressing" one of our relevant inputs.
        */
        void OnPressed(float value) override;
        /*
        * On the release of one of our buttons or mouse movement, we still update
        * those values for them to eventually be sent to the server
        */
        void OnReleased(float value) override;
        /*
        * We only handle camera rotation events since the movement does not need
        * OnHeld event handling.
        */
        void OnHeld(float value) override;

        // UserBus Implementations
        /*
        * The pitch value is the rotation that we calculated from the inputs. It is what
        * resulted after all the manipuations we have made.
        */
        float getPitchValue() override;

#if AZ_TRAIT_CLIENT
        // item bus overrides
        /*
        * We updat the *local* rgb values, since on each event time, we use these to update
        * the network inputs, and if they change, the material will change.
        */
        void executeItem(AZStd::string itemName) override;
#endif

    protected:
        /*
        * We manipulate the rotatin quaternion depending on the mouse input values
        */
        void UpdateRotation(const UserNetworkInput* input);
        void UpdateVelocity(const UserNetworkInput* input);

        float m_forward = 0;
        float m_strafe = 0;
        float m_yaw = 0;
        float m_pitch = 0;
        float m_colorR = 0;
        float m_colorG = 0;
        float m_colorB = 0;

        // this is the transformation needed to be applied
        // to the camera to move it vertically
        float pitch_transform = 0;

        AZ::Vector3 m_velocity = AZ::Vector3::CreateZero();

        AZStd::unordered_map<AZStd::string, AZStd::vector<float>> m_itemMap = { {"red", {1.0f, 0.0f, 0.0f}}, {"blue", {0.0f, 1.0f, 0.0f}}, {"green", {0.0f, 0.0f, 1.0f}} };
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
        AZ::Event<int32_t, float>::Handler m_ColorChanged;
        void OnColorChanged(int32_t index, float newColor);
        void UpdateMaterial(float r, float g, float b);
    };
}