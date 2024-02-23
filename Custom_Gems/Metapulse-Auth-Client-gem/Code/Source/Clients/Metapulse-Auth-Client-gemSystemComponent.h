
#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <Metapulse-Auth-Client-gem/Metapulse-Auth-Client-gemBus.h>

namespace Metapulse_Auth_Client_gem
{
    class Metapulse_Auth_Client_gemSystemComponent
        : public AZ::Component
        , protected Metapulse_Auth_Client_gemRequestBus::Handler
        , public AZ::TickBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(Metapulse_Auth_Client_gemSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        Metapulse_Auth_Client_gemSystemComponent();
        ~Metapulse_Auth_Client_gemSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // Metapulse_Auth_Client_gemRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZTickBus interface implementation
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        ////////////////////////////////////////////////////////////////////////
    };

} // namespace Metapulse_Auth_Client_gem
