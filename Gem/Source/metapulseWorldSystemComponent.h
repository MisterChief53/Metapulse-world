
#pragma once

#include <AzCore/Component/Component.h>

#include <Metapulse-world/Metapulse-worldBus.h>

namespace Metapulse_world
{
    class Metapulse_worldSystemComponent
        : public AZ::Component
        , protected Metapulse_worldRequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(Metapulse_worldSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        Metapulse_worldSystemComponent();
        ~Metapulse_worldSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // Metapulse_worldRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
