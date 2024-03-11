#include <Source/Components/User.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace metapulseWorld
{
    UserController::UserController(User& parent)
        : UserControllerBase(parent)
    {
    }

    void UserController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }

    void UserController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }

    void UserController::CreateInput([[maybe_unused]] Multiplayer::NetworkInput& input, [[maybe_unused]] float deltaTime)
    {
    }

    void UserController::ProcessInput([[maybe_unused]] Multiplayer::NetworkInput& input, [[maybe_unused]] float deltaTime)
    {
    }
}