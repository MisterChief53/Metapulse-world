
#pragma once

namespace Metapulse_Auth_Client_gem
{
    // System Component TypeIds
    inline constexpr const char* Metapulse_Auth_Client_gemSystemComponentTypeId = "{57F957EE-C95B-4FFE-89CD-449A569AE4EB}";
    inline constexpr const char* Metapulse_Auth_Client_gemEditorSystemComponentTypeId = "{FA174D62-D771-4F33-BADB-DDF556D8AEDD}";

    // Module derived classes TypeIds
    inline constexpr const char* Metapulse_Auth_Client_gemModuleInterfaceTypeId = "{FC208DCD-BE4F-4171-B167-D7E13D1AC06C}";
    inline constexpr const char* Metapulse_Auth_Client_gemModuleTypeId = "{0DF60F21-1DE2-4618-A336-167E5EF8D57B}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* Metapulse_Auth_Client_gemEditorModuleTypeId = Metapulse_Auth_Client_gemModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* Metapulse_Auth_Client_gemRequestsTypeId = "{21B18EE9-A9E4-4AF5-8716-1B05F5239452}";
} // namespace Metapulse_Auth_Client_gem
