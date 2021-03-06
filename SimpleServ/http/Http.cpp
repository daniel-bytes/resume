#include "Http.h"
#include "shared/Utilities.h"

namespace Http {
	const Headers::const_iterator FindHeader(const Headers &headers, const std::string &key)
	{
		auto lowercaseKey = Utilities::ToLowerCase(key);
		auto i = std::cbegin(headers);

		while (i != headers.end()) {
			if (lowercaseKey == Utilities::ToLowerCase(i->first)) {
				break;
			}
			++i;
		}

		return i;
	}

	std::unordered_map<StatusCode, std::string> StatusDescriptions::_descriptions = {
		{ StatusCode::Continue, "Continue" },
		{ StatusCode::SwitchingProtocols, "Switching Protocols" },
		{ StatusCode::OK, "OK" },
		{ StatusCode::Created, "Created" },
		{ StatusCode::Accepted, "Accepted" },
		{ StatusCode::NonAuthoritativeInformation, "Non-Authoritative Information" },
		{ StatusCode::NoContent, "No Content" },
		{ StatusCode::ResetContent, "Reset Content" },
		{ StatusCode::PartialContent, "Partial Content" },
		{ StatusCode::MultiStatus, "Multi-Status" },
		{ StatusCode::AlreadyReported, "Already Reported " },
		{ StatusCode::IMUsed, "IM Used " },
		{ StatusCode::MultipleChoices, "Multiple Choices" },
		{ StatusCode::MovedPermanently, "Moved Permanently" },
		{ StatusCode::Found, "Found" },
		{ StatusCode::SeeOther, "See Other" },
		{ StatusCode::NotModified, "Not Modified " },
		{ StatusCode::UseProxy, "Use Proxy " },
		{ StatusCode::SwitchProxy, "Switch Proxy" },
		{ StatusCode::TemporaryRedirect, "Temporary Redirect " },
		{ StatusCode::PermanentRedirect, "Permanent Redirect " },
		{ StatusCode::ResumeIncomplete, "Resume Incomplete " },
		{ StatusCode::BadRequest, "Bad Request" },
		{ StatusCode::Unauthorized, "Unauthorized " },
		{ StatusCode::PaymentRequired, "Payment Required" },
		{ StatusCode::Forbidden, "Forbidden" },
		{ StatusCode::NotFound, "Not Found" },
		{ StatusCode::MethodNotAllowed, "Method Not Allowed" },
		{ StatusCode::NotAcceptable, "Not Acceptable" },
		{ StatusCode::ProxyAuthenticationRequired, "Proxy Authentication Required" },
		{ StatusCode::RequestTimeout, "Request Timeout" },
		{ StatusCode::Conflict, "Conflict" },
		{ StatusCode::Gone, "Gone" },
		{ StatusCode::LengthRequired, "Length Required" },
		{ StatusCode::PreconditionFailed, "Precondition Failed" },
		{ StatusCode::RequestEntityTooLarge, "Request Entity Too Large" },
		{ StatusCode::RequestURITooLong, "Request-URI Too Long" },
		{ StatusCode::UnsupportedMediaType, "Unsupported Media Type" },
		{ StatusCode::RequestedRangeNotSatisfiable, "Requested Range Not Satisfiable" },
		{ StatusCode::ExpectationFailed, "Expectation Failed" },
		{ StatusCode::Imateapot, "I'm a teapot" },
		{ StatusCode::AuthenticationTimeout, "Authentication Timeout" },
		{ StatusCode::MethodFailure, "Method Failure" },
		{ StatusCode::EnhanceYourCalm, "Enhance Your Calm" },
		{ StatusCode::MisdirectedRequest, "Misdirected Request" },
		{ StatusCode::UnprocessableEntity, "Unprocessable Entity" },
		{ StatusCode::Locked, "Locked" },
		{ StatusCode::FailedDependency, "Failed Dependency " },
		{ StatusCode::UpgradeRequired, "Upgrade Required" },
		{ StatusCode::PreconditionRequired, "Precondition Required " },
		{ StatusCode::TooManyRequests, "Too Many Requests" },
		{ StatusCode::RequestHeaderFieldsTooLarge, "Request Header Fields Too Large " },
		{ StatusCode::UnavailableForLegalReasons, "Unavailable For Legal Reasons " },
		{ StatusCode::InternalServerError, "Internal Server Error" },
		{ StatusCode::NotImplemented, "Not Implemented" },
		{ StatusCode::BadGateway, "Bad Gateway" },
		{ StatusCode::ServiceUnavailable, "Service Unavailable" },
		{ StatusCode::GatewayTimeout, "Gateway Timeout" },
		{ StatusCode::HTTPVersionNotSupported, "HTTP Version Not Supported" },
		{ StatusCode::NotExtended, "Not Extended" },
		{ StatusCode::NetworkAuthenticationRequired, "Network Authentication Required" },
	};

	std::string ContentTypes::_html = "text/html; charset=utf-8";
	std::string ContentTypes::_css = "text/css; charset=utf-8";
	std::string ContentTypes::_plainText = "text/plain; charset=utf-8";
	std::string ContentTypes::_png = "image/png";
	std::string ContentTypes::_ico = "image/xicon";
	std::string ContentTypes::_manifest = "application/xml; charset=utf-8";

	std::unordered_map<std::string, std::string> ContentTypes::_contentTypes = {
		{ ".html", _html },
		{ ".css", _css },
		{ ".txt", _plainText },
		{ ".png", _png },
		{ ".ico", _ico },
		{ ".manifest", _manifest }
	};
}