
#pragma once

namespace AuthClient
{
    // System Component TypeIds
    inline constexpr const char* AuthClientSystemComponentTypeId = "{9B9919A4-8D48-4371-84C3-C799027E648B}";
    inline constexpr const char* AuthClientEditorSystemComponentTypeId = "{B680F64D-986D-4749-A00C-621C59D65D9D}";

    // Module derived classes TypeIds
    inline constexpr const char* AuthClientModuleInterfaceTypeId = "{83DE1D96-C169-4C09-B4D8-019F64D10451}";
    inline constexpr const char* AuthClientModuleTypeId = "{F17EC536-24AD-4894-B269-97F49A2EB50E}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* AuthClientEditorModuleTypeId = AuthClientModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* AuthClientRequestsTypeId = "{407039B9-E011-4595-88FC-2AFF2F88981C}";
} // namespace AuthClient
