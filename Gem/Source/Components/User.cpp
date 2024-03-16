#include <Source/Components/User.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TransformBus.h>
#include <Multiplayer/Components/NetworkTransformComponent.h>
#include <AzCore/Serialization/SerializeContext.h>
// we still need to include the network components even after the XML config
#include <Multiplayer/Components/NetworkCharacterComponent.h>
#include <Components/Interfaces/UIStatusBus.h>


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
    }

    void UserController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        InputEventNotificationBus::MultiHandler::BusDisconnect();
        metapulseWorld::UserBus::Handler::BusDisconnect();
    }

    // Create input will collect the input for the last input time period
    void UserController::CreateInput([[maybe_unused]] Multiplayer::NetworkInput& input, [[maybe_unused]] float deltaTime)
    {
        // Request to uistatus bus to see of we are allowed to move. This would mean that there is a ui that does not
        // let us move, active.
        UIStatusBus::BroadcastResult(canMove, &UIStatusBus::Events::canMove);
        if (!canMove) {
            return;
        }


        auto playerInput = input.FindComponentInput<UserNetworkInput>();
        playerInput->m_forwardAxis = m_forward;
        playerInput->m_strafeAxis = m_strafe;
        playerInput->m_viewYaw = m_yaw;
        playerInput->m_viewPitch = m_pitch;

        playerInput->m_resetCount = GetNetworkTransformComponentController()->GetResetCount();
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
    }

    void UserController::OnPressed(float value) {
        if (!canMove) {
            return;
        }

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
        if (!canMove) {
            return;
        }

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
        if (!canMove) {
            return;
        }

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
}