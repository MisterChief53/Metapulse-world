#include <Source/Components/User.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TransformBus.h>
#include <Multiplayer/Components/NetworkTransformComponent.h>
#include <AzCore/Serialization/SerializeContext.h>
// we still need to include the network components even after the XML config
#include <Multiplayer/Components/NetworkCharacterComponent.h>
#include <Components/Interfaces/UserRegistryBus.h>
#include <Components/Interfaces/HUDBus.h>
#include <AzCore/Math/Color.h>
#include <random>
namespace metapulseWorld
{
    using namespace StartingPointInput;

    UserController::UserController(User& parent)
        : UserControllerBase(parent)
    {
    }

    void UserController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        InputEventNotificationBus::MultiHandler::BusConnect(MoveFwdEventId);
        InputEventNotificationBus::MultiHandler::BusConnect(MoveRightEventId);
        InputEventNotificationBus::MultiHandler::BusConnect(RotateYawEventId);
        InputEventNotificationBus::MultiHandler::BusConnect(RotatePitchEventId);
        metapulseWorld::UserBus::Handler::BusConnect(GetEntityId());

        // register the user on user registry
        AZLOG_INFO("######################################## Attempting to register a user ########################################");
        UserRegistryBus::Broadcast(&UserRegistryBus::Events::RegisterUser, this->GetEntityId());

#if AZ_TRAIT_CLIENT
        ItemBus::Handler::BusConnect();
#endif
    }

    void UserController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        InputEventNotificationBus::MultiHandler::BusDisconnect();
        metapulseWorld::UserBus::Handler::BusDisconnect();
        UserRegistryBus::Broadcast(&UserRegistryBus::Events::UnregisterUser, this->GetEntityId().ToString());
#if AZ_TRAIT_CLIENT
        ItemBus::Handler::BusDisconnect();
#endif
    }

    // Create input will collect the input for the last input time period
    void UserController::CreateInput([[maybe_unused]] Multiplayer::NetworkInput& input, [[maybe_unused]] float deltaTime)
    {
        auto playerInput = input.FindComponentInput<UserNetworkInput>();
        playerInput->m_forwardAxis = m_forward;
        playerInput->m_strafeAxis = m_strafe;
        playerInput->m_viewYaw = m_yaw;
        playerInput->m_viewPitch = m_pitch;

        playerInput->m_resetCount = GetNetworkTransformComponentController()->GetResetCount();

#if AZ_TRAIT_CLIENT
        playerInput->m_colorInputR = m_colorR;
        playerInput->m_colorInputG = m_colorG;
        playerInput->m_colorInputB = m_colorB;
#endif
    }

    void UserController::ProcessInput([[maybe_unused]] Multiplayer::NetworkInput& input, [[maybe_unused]] float deltaTime)
    {
        auto playerInput = input.FindComponentInput<UserNetworkInput>();

        if (playerInput->m_resetCount != GetNetworkTransformComponentController()->GetResetCount()) {
            return;
        }
        pitch_transform = playerInput->m_viewPitch * GetTurnSpeed();

        UpdateRotation(playerInput);
        UpdateVelocity(playerInput);
        GetNetworkCharacterComponentController()->TryMoveWithVelocity(m_velocity, deltaTime);
#if AZ_TRAIT_SERVER
        // SetColorProperty(playerInput->m_colorInput);
        SetColorProperty(0, playerInput->m_colorInputR);
        SetColorProperty(1, playerInput->m_colorInputG);
        SetColorProperty(2, playerInput->m_colorInputB);
#endif
    }

    void UserController::OnPressed(float value) {

        const InputEventNotificationId* inputId = InputEventNotificationBus::GetCurrentBusId();

        if (inputId == nullptr) {
            return;
        }

        if (*inputId == MoveFwdEventId) {
            m_forward = value;
        }
        else if (*inputId == MoveRightEventId) {
            m_strafe = value;
        }
        else if (*inputId == RotateYawEventId) {
            m_yaw = value;
        }
        else if (*inputId == RotatePitchEventId) {
            m_pitch = value;
        }
    }

    void UserController::OnHeld(float value) {

        const InputEventNotificationId* inputId = InputEventNotificationBus::GetCurrentBusId();

        if (inputId == nullptr) {
            return;
        }

        if (*inputId == RotateYawEventId) {
            m_yaw = value;
        }
        else if (*inputId == RotatePitchEventId) {
            m_pitch = value;
        }
    }

    void UserController::OnReleased(float value) {

        const InputEventNotificationId* inputId = InputEventNotificationBus::GetCurrentBusId();

        if (inputId == nullptr) {
            return;
        }

        if (*inputId == MoveFwdEventId) {
            m_forward = value;
        }
        else if (*inputId == MoveRightEventId) {
            m_strafe = value;
        }
        else if (*inputId == RotateYawEventId) {
            m_yaw = value;
        }
        else if (*inputId == RotatePitchEventId) {
            m_pitch = value;
        }
    }

    float UserController::getPitchValue()
    {
        
        return pitch_transform;
    }

    void UserController::UpdateRotation(const UserNetworkInput* input) {
        AZ::TransformInterface* t = GetEntity()->GetTransform();

        float currentHeading = t->GetWorldRotationQuaternion().GetEulerRadians().GetZ();
        currentHeading += input->m_viewYaw * GetTurnSpeed();

        AZ::Quaternion q = AZ::Quaternion::CreateRotationZ(currentHeading);
        t->SetWorldRotationQuaternion(q);
    }

    void UserController::UpdateVelocity(const UserNetworkInput* input) {
        const float currentHeading = GetEntity()->GetTransform()->GetWorldRotationQuaternion().GetEulerRadians().GetZ();

        const AZ::Vector3 fwd = AZ::Vector3::CreateAxisY(input->m_forwardAxis);
        const AZ::Vector3 strafe = AZ::Vector3::CreateAxisX(input->m_strafeAxis);

        AZ::Vector3 combined = fwd + strafe;
        if (combined.GetLength() > 1.f) {
            combined.Normalize();
        }

        m_velocity = AZ::Quaternion::CreateRotationZ(currentHeading).TransformVector(combined) * GetWalkSpeed() 
            + AZ::Vector3::CreateAxisZ(GetGravity());
    }

#if AZ_TRAIT_CLIENT
    void UserController::executeItem(AZStd::string itemName) {
        m_colorR = m_itemMap[itemName][0];
        m_colorG = m_itemMap[itemName][1];
        m_colorB = m_itemMap[itemName][2];
    }
#endif

    void User::Reflect(AZ::ReflectContext* context)
    {
        AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
        if (serializeContext)
        {
            serializeContext->Class<User, UserBase>()
                ->Version(1);
        }
        UserBase::Reflect(context);
    }

    void User::OnInit()
    {
    }

    void User::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        ColorPropertyAddEvent(m_ColorChanged);
        // UpdateMaterial(GetColorProperty());
        float r = GetColorProperty(0);
        float g = GetColorProperty(1);
        float b = GetColorProperty(2);
        UpdateMaterial(r, g, b);
    }

    void User::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }
    User::User()
        : m_ColorChanged([this](int32_t index, float newColor) {
        OnColorChanged(index, newColor);
        })
    {
    }
    void User::OnColorChanged([[maybe_unused]] int32_t index, float newColor)
    {
        AZLOG_INFO("Color changed to: %0.2f", newColor);
        float r = GetColorProperty(0);
        float g = GetColorProperty(1);
        float b = GetColorProperty(2);
        UpdateMaterial(r, g, b);
    }
    void User::UpdateMaterial(float r, float b, float g)
    {
        AZ::Render::MaterialAssignmentLabelMap materialLabelMap;
        AZ::Render::MaterialConsumerRequestBus::EventResult(materialLabelMap, this->GetEntityId(),
            &AZ::Render::MaterialConsumerRequestBus::Events::GetMaterialLabels);

        if (materialLabelMap.empty()) {
            AZLOG_INFO("Material label map is empty!");
            return;
        }
        AZStd::vector<AZ::Render::MaterialAssignmentId> materialVector;
        AZStd::string propertyName = "baseColor.color";

        for (auto idLabelPair : materialLabelMap) {
            if (idLabelPair.second == "DefaultMaterial") {
                materialVector.push_back(idLabelPair.first);
            }
        }

        AZ::Color color = AZ::Color(r, g, b, 1.0f);
        for (auto materialId : materialVector) {
            AZStd::any gottenAny;
            AZ::Render::MaterialComponentRequestBus::EventResult(gottenAny, this->GetEntityId(),
                &AZ::Render::MaterialComponentRequestBus::Events::GetPropertyValue, materialId, propertyName);
            if (gottenAny.empty()) {
                AZLOG_ERROR("Could not get player's material color!");
            }

            AZ::Render::MaterialComponentRequestBus::Event(this->GetEntityId(),
                &AZ::Render::MaterialComponentRequestBus::Events::SetPropertyValue, materialId, propertyName, AZStd::any(color));
        }
    }
}