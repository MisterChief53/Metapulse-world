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
    }

    void UserController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        InputEventNotificationBus::MultiHandler::BusDisconnect();
        metapulseWorld::UserBus::Handler::BusDisconnect();
        UserRegistryBus::Broadcast(&UserRegistryBus::Events::UnregisterUser, this->GetEntityId().ToString());
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


        std::random_device rd;
        std::mt19937 gen(rd());

        // Create a uniform real distribution between 0.0 and 1.0
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        playerInput->m_colorInput = dis(gen);
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
        SetColorProperty(playerInput->m_colorInput);
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
    }

    void User::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }
    User::User()
        : m_ColorChanged([this](float newColor) {
        OnColorChanged(newColor);
        })
    {
    }
    void User::OnColorChanged(float newColor)
    {
        AZLOG_INFO("Color changed to: %0.2f", newColor);
        UpdateMaterial(newColor);
    }
    void User::UpdateMaterial(float newColor)
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

        AZ::Color color = AZ::Color(newColor);
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