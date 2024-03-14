#include <Source/Components/UserCamera.h>

#include <AzCore/Serialization/SerializeContext.h>

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzFramework/Components/TransformComponent.h>
#include <AzFramework/Components/CameraBus.h>
#include <Components/Interfaces/UserBus.h>

namespace metapulseWorld
{
    UserCameraController::UserCameraController(UserCamera& parent)
        : UserCameraControllerBase(parent)
    {
    }

    void UserCameraController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        if (IsNetEntityRoleAutonomous()) {
            AZ::TickBus::Handler::BusConnect();
        }
    }

    void UserCameraController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        AZ::TickBus::Handler::BusDisconnect();
    }

    void UserCameraController::OnTick([[maybe_unused]] float deltaTime, AZ::ScriptTimePoint)
    {
        if (!m_activeCameraEntity) {
            AZLOG_INFO("could not get active camera entity for the user camera");
            m_activeCameraEntity = GetActiveCamera();
            return;
        }

        if (IsNetEntityRoleAutonomous())
        {
            float pitchRotation;
            metapulseWorld::UserBus::EventResult(pitchRotation, GetEntityId(),
                &metapulseWorld::UserBus::Events::getPitchValue);
            AZLOG_INFO("%0.2f pitch", pitchRotation);


            // the position of the camera is the user's position plus the camera offset!
            AZ::Transform user = GetParent().GetTransformComponent()->GetWorldTM();
            AZ::Vector3 camera = user.GetTranslation() + user.GetRotation().TransformVector(GetCameraOffset());
            user.SetTranslation(camera);

            AZ::Vector3 currentCameraRotation = m_activeCameraEntity->GetTransform()->GetWorldRotation();

            currentPitchValue += pitchRotation;

            AZ::Quaternion userRotation = user.GetRotation();
            AZ::Quaternion pitchRotationQuat = AZ::Quaternion::CreateRotationX(currentPitchValue);
            userRotation *= pitchRotationQuat;

            user.SetRotation(userRotation);

            m_activeCameraEntity->GetTransform()->SetWorldTM(user);
        }
    }

    AZ::Entity* UserCameraController::GetActiveCamera()
    {
        using namespace AZ;
        using namespace Camera;

        EntityId activeCameraId;
        CameraSystemRequestBus::BroadcastResult(activeCameraId, &CameraSystemRequestBus::Events::GetActiveCamera);

        auto ca = Interface<ComponentApplicationRequests>::Get();
        return ca->FindEntity(activeCameraId);
    }
}