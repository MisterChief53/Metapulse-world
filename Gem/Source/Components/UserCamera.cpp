#include <Source/Components/UserCamera.h>

#include <AzCore/Serialization/SerializeContext.h>

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzFramework/Components/TransformComponent.h>
#include <AzFramework/Components/CameraBus.h>

namespace metapulseWorld
{
    UserCameraController::UserCameraController(UserCamera& parent)
        : UserCameraControllerBase(parent)
    {
    }

    void UserCameraController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        AZ::TickBus::Handler::BusConnect();
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

        // the position of the camera is the user's position plus the camera offset!
        AZ::Transform user = GetParent().GetTransformComponent()->GetWorldTM();
        AZ::Vector3 camera = user.GetTranslation() + user.GetRotation().TransformVector(GetCameraOffset());
        user.SetTranslation(camera);
        m_activeCameraEntity->GetTransform()->SetWorldTM(user);
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