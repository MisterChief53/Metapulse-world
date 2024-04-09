#pragma once

#include <AzCore/std/containers/unordered_set.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/Console/ILogger.h>
#include <AzNetworking/Serialization/ISerializer.h>

namespace metapulseWorld {
	struct UserSetClass {
		AZ_TYPE_INFO(metapulseWorld::UserSetClass, "{7CE8BCD1-EACD-404C-9C52-24C45647F16B}");

		AZStd::unordered_set<AZStd::string> m_userSet = {};

		bool Serialize(AzNetworking::ISerializer& serializer) {
			return serializer.Serialize(m_userSet, "UserSet");
		}

		bool operator!=(const UserSetClass& rhs) const {
			for (auto user : rhs.m_userSet) {
				if (this->m_userSet.find(user) == this->m_userSet.end()) {
					return true;
				}
			}

			return false;
		}
	};
}