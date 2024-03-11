
#pragma once

#include <AzCore/Component/Component.h>

#include <metapulseWorld/metapulseWorldBus.h>

namespace metapulseWorld
{
    class metapulseWorldSystemComponent
        : public AZ::Component
        , protected metapulseWorldRequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(metapulseWorldSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        metapulseWorldSystemComponent();
        ~metapulseWorldSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // metapulseWorldRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
