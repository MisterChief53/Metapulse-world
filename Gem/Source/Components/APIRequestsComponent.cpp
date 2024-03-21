#include <Components/APIRequestsComponent.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Console/ILogger.h>

namespace metapulseWorld {
	void APIRequestsComponent::Init() {

	}

	void APIRequestsComponent::Activate() {
		// Globally initialize curl functionality for windows systems. Only do this once
		// Per application lifetime
		//curl_global_init(CURL_GLOBAL_WIN32);
		//m_handle = curl_easy_init();
	}

	void APIRequestsComponent::Deactivate() {
		//curl_easy_setopt(handle, CURLOPT_URL, "http://localhost:8080/");
		//curl_global_cleanup();
	}

	void APIRequestsComponent::Reflect(AZ::ReflectContext* context) {
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
			serializeContext->Class<APIRequestsComponent, AZ::Component>()
				->Version(1)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<APIRequestsComponent>("APIRequestsComponent", "[contains requests handing to metapulse accounts server]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "Accounts Server")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
					;
			}
		}
	}

	//void APIRequestsComponent::login(AZStd::string& response, bool& succeed, AZStd::string& token, 
	//	const AZStd::string& username, const AZStd::string& password){
	//	curl_easy_setopt(m_handle, CURLOPT_URL, "http://localhost:8080/auth/login");

	//	curl_easy_setopt(m_handle, CURLOPT_POST, 1L);

	//	// form data
	//	struct curl_httppost* formpost = nullptr;
	//	struct curl_httppost* lastptr = nullptr;

	//	curl_formadd(&formpost,
	//		&lastptr,
	//		CURLFORM_COPYNAME, "name",
	//		CURLFORM_COPYCONTENTS, username.c_str(),
	//		CURLFORM_END);
	//	curl_formadd(&formpost,
	//		&lastptr,
	//		CURLFORM_COPYNAME, "password",
	//		CURLFORM_COPYCONTENTS, password.c_str(),
	//		CURLFORM_END);

	//	// set form data
	//	curl_easy_setopt(m_handle, CURLOPT_HTTPPOST, formpost);

	//	curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	//	curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, &response);

	//	auto res = curl_easy_perform(m_handle);
	//	if (res != CURLE_OK) {
	//		AZLOG_INFO("curl_easy_perform failed");
	//		succeed = false;
	//		token = "invalid";
	//	}
	//	else {
	//		AZLOG_INFO("login succesful");
	//		succeed = true;
	//		token = response;
	//	}

	//	//Cleanup
	//	curl_easy_cleanup(m_handle);
	//	curl_formfree(formpost);
	//}

	//size_t APIRequestsComponent::WriteMemoryCallback(char* contents, size_t size, size_t nmemb, std::string* userp)
	//{
	//	size_t totalSize = size * nmemb;
	//	userp->append(contents, totalSize);
	//	return totalSize;
	//}
}