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
        * We manipulate the rotation quaternion depending on the yaw mouse changes.
        * This modifies the network input, so that means that these changes are 
        * reflected on the server and are not client-side only.
        */
        void UpdateRotation(const UserNetworkInput* input);

        /*
        * Updates the entity's velocity from the axis that we get from combining
        * the forward and strave axis. This will tell our entity where to move once we
        * later apply the changes.
        */
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

        /*
        * The velocity is first initialized at zero because we
        * will not move unless we get an input telling us otherwise
        */
        AZ::Vector3 m_velocity = AZ::Vector3::CreateZero();

        /*
        * Specifies the items' name and corresponding color value, which in this case
        * is the material's base RGB color.
        */
        AZStd::unordered_map<AZStd::string, AZStd::vector<float>> m_itemMap = { {"red", {1.0f, 0.0f, 0.0f}}, {"blue", {0.0f, 1.0f, 0.0f}}, {"green", {0.0f, 0.0f, 1.0f}} };
    };
    
    /*
    * Component for client-side interaction of the user multiplayer component, that]
    * provides read-only acccess to our relevant network properties.
    */
    class User
        : public UserBase
    {
    public:
        AZ_MULTIPLAYER_COMPONENT(metapulseWorld::User, s_userConcreteUuid, metapulseWorld::UserBase);

        static void Reflect(AZ::ReflectContext* context);

        void OnInit() override;
        /*
        * On activation, we are going to trigger the fetching of the server's 
        * configured color for this entity. This is because, the user might
        * have set their item while we were offline. So, we update the material.
        * 
        * TODO: UpdateMaterial fails whenever the Material component is not set
        * (which is almost always). Try to make this component depend on the
        * Material service.
        */
        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        /*
        * The constructor defines a handler for the OnColorChanged event. This, as 
        * is specified by the autogen XML, hsa to have the index of the array (since
        * our network property is an array of two values), and the float value itself.
        */
        User();


    protected:
        /*
        * Our player's current color (item)
        */
        AZ::Color m_materialBaseColor;

        /*
        * The handler for when the color property has been changed.
        */
        AZ::Event<int32_t, float>::Handler m_ColorChanged;

        /*
        * Whenever a color is changed, we update the entity's material.
        */
        void OnColorChanged(int32_t index, float newColor);

        /*
        * Get this entity's material slots, then for those, we change the property
        * baseColor.color, which is the one that controls the base color for
        * the material.
        */
        void UpdateMaterial(float r, float g, float b);
    };
}