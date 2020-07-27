#ifndef __HTTP_H__
#define __HTTP_H__

#include <map>
#include <string>
#include <optional>

namespace Http {
	typedef std::map<std::string, std::string> Headers;

	// see https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
	enum class StatusCode {
		Continue = 100, //This means that the server has received the request headers, and that the client should proceed to send the request body (in the case of a request for which a body needs to be sent; for example, a POST request). If the request body is large, sending it to a server when a request has already been rejected based upon inappropriate headers is inefficient. To have a server check if the request could be accepted based on the request's headers alone, a client must send Expect: 100-continue as a header in its initial request and check if a 100 Continue status code is received in response before continuing (or receive 417 Expectation Failed and not continue).
		SwitchingProtocols = 101, //This means the requester has asked the server to switch protocols and the server is acknowledging that it will do so.
		OK = 200, //Standard response for successful HTTP requests. The actual response will depend on the request method used. In a GET request, the response will contain an entity corresponding to the requested resource. In a POST request, the response will contain an entity describing or containing the result of the action.
		Created = 201, //The request has been fulfilled and resulted in a new resource being created.
		Accepted = 202, //The request has been accepted for processing, but the processing has not been completed. The request might or might not eventually be acted upon, as it might be disallowed when processing actually takes place.
		NonAuthoritativeInformation = 203, //The server successfully processed the request, but is returning information that may be from another source.
		NoContent = 204, //The server successfully processed the request, but is not returning any content. Usually used as a response to a successful delete request.
		ResetContent = 205, //The server successfully processed the request, but is not returning any content. Unlike a 204 response, this response requires that the requester reset the document view.
		PartialContent = 206, //The server is delivering only part of the resource (byte serving) due to a range header sent by the client. The range header is used by tools like wget to enable resuming of interrupted downloads, or split a download into multiple simultaneous streams.
		MultiStatus = 207, //The message body that follows is an XML message and can contain a number of separate response codes, depending on how many sub-requests were made.[4]
		AlreadyReported = 208, //The members of a DAV binding have already been enumerated in a previous reply to this request, and are not being included again.
		IMUsed = 226, //The server has fulfilled a request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance.[5]
		MultipleChoices = 300, //Indicates multiple options for the resource that the client may follow. It, for instance, could be used to present different format options for video, list files with different extensions, or word sense disambiguation.
		MovedPermanently = 301, //This and all future requests should be directed to the given URI.
		Found = 302, //This is an example of industry practice contradicting the standard. The HTTP/1.0 specification (RFC 1945) required the client to perform a temporary redirect (the original describing phrase was "Moved Temporarily"),[6] but popular browsers implemented 302 with the functionality of a 303 See Other. Therefore, HTTP/1.1 added status codes 303 and 307 to distinguish between the two behaviours.[7] However, some Web applications and frameworks use the 302 status code as if it were the 303.[8]
		SeeOther = 303, //The response to the request can be found under another URI using a GET method. When received in response to a POST (or PUT/DELETE), it should be assumed that the server has received the data and the redirect should be issued with a separate GET message.
		NotModified = 304, //Indicates that the resource has not been modified since the version specified by the request headers If-Modified-Since or If-None-Match. This means that there is no need to retransmit the resource, since the client still has a previously-downloaded copy.
		UseProxy = 305, //The requested resource is only available through a proxy, whose address is provided in the response. Many HTTP clients (such as Mozilla[9] and Internet Explorer) do not correctly handle responses with this status code, primarily for security reasons.[10]
		SwitchProxy = 306, //No longer used. Originally meant "Subsequent requests should use the specified proxy."[11]
		TemporaryRedirect = 307, //In this case, the request should be repeated with another URI; however, future requests should still use the original URI. In contrast to how 302 was historically implemented, the request method is not allowed to be changed when reissuing the original request. For instance, a POST request should be repeated using another POST request.[12]
		PermanentRedirect = 308, //The request, and all future requests should be repeated using another URI. 307 and 308 (as proposed) parallel the behaviours of 302 and 301, but do not allow the HTTP method to change. So, for example, submitting a form to a permanently redirected resource may continue smoothly.[13]
		ResumeIncomplete = 308, //This code is used in the Resumable HTTP Requests Proposal to resume aborted PUT or POST requests.[14]
		BadRequest = 400, //The server cannot or will not process the request due to something that is perceived to be a client error (e.g., malformed request syntax, invalid request message framing, or deceptive request routing).[15]
		Unauthorized = 401, //Similar to 403 Forbidden, but specifically for use when authentication is required and has failed or has not yet been provided. The response must include a WWW-Authenticate header field containing a challenge applicable to the requested resource. See Basic access authentication and Digest access authentication.
		PaymentRequired = 402, //Reserved for future use. The original intention was that this code might be used as part of some form of digital cash or micropayment scheme, but that has not happened, and this code is not usually used. YouTube uses this status if a particular IP address has made excessive requests, and requires the person to enter a CAPTCHA.[citation needed]
		Forbidden = 403, //The request was a valid request, but the server is refusing to respond to it. Unlike a 401 Unauthorized response, authenticating will make no difference.
		NotFound = 404, //The requested resource could not be found but may be available again in the future. Subsequent requests by the client are permissible.
		MethodNotAllowed = 405, //A request was made of a resource using a request method not supported by that resource; for example, using GET on a form which requires data to be presented via POST, or using PUT on a read-only resource.
		NotAcceptable = 406, //The requested resource is only capable of generating content not acceptable according to the Accept headers sent in the request.
		ProxyAuthenticationRequired = 407, //The client must first authenticate itself with the proxy.
		RequestTimeout = 408, //The server timed out waiting for the request. According to HTTP specifications: "The client did not produce a request within the time that the server was prepared to wait. The client MAY repeat the request without modifications at any later time."
		Conflict = 409, //Indicates that the request could not be processed because of conflict in the request, such as an edit conflict in the case of multiple updates.
		Gone = 410, //Indicates that the resource requested is no longer available and will not be available again. This should be used when a resource has been intentionally removed and the resource should be purged. Upon receiving a 410 status code, the client should not request the resource again in the future. Clients such as search engines should remove the resource from their indices.[16] Most use cases do not require clients and search engines to purge the resource, and a "404 Not Found" may be used instead.
		LengthRequired = 411, //The request did not specify the length of its content, which is required by the requested resource.
		PreconditionFailed = 412, //The server does not meet one of the preconditions that the requester put on the request.
		RequestEntityTooLarge = 413, //The request is larger than the server is willing or able to process.
		RequestURITooLong = 414, //The URI provided was too long for the server to process. Often the result of too much data being encoded as a query-string of a GET request, in which case it should be converted to a POST request.
		UnsupportedMediaType = 415, //The request entity has a media type which the server or resource does not support. For example, the client uploads an image as image/svg+xml, but the server requires that images use a different format.
		RequestedRangeNotSatisfiable = 416, //The client has asked for a portion of the file (byte serving), but the server cannot supply that portion. For example, if the client asked for a part of the file that lies beyond the end of the file.
		ExpectationFailed = 417, //The server cannot meet the requirements of the Expect request-header field.
		Imateapot = 418, //This code was defined in 1998 as one of the traditional IETF April Fools' jokes, in RFC 2324, Hyper Text Coffee Pot Control Protocol, and is not expected to be implemented by actual HTTP servers. The RFC specifies this code should be returned by tea pots requested to brew coffee.
		AuthenticationTimeout = 419, //Not a part of the HTTP standard, 419 Authentication Timeout denotes that previously valid authentication has expired. It is used as an alternative to 401 Unauthorized in order to differentiate from otherwise authenticated clients being denied access to specific server resources.[citation needed]
		MethodFailure = 420, //Not part of the HTTP standard, but defined by Spring in the HttpStatus class to be used when a method failed. This status code is deprecated by Spring.
		EnhanceYourCalm = 420, //Not part of the HTTP standard, but returned by version 1 of the Twitter Search and Trends API when the client is being rate limited.[17] Other services may wish to implement the 429 Too Many Requests response code instead.
		MisdirectedRequest = 421, //The request was directed at a server that is not able to produce a response (for example because a connection reuse).[18]
		UnprocessableEntity = 422, //The request was well-formed but was unable to be followed due to semantic errors.[4]
		Locked = 423, //The resource that is being accessed is locked.[4]
		FailedDependency = 424, //The request failed due to failure of a previous request (e.g., a PROPPATCH).[4]
		UpgradeRequired = 426, //The client should switch to a different protocol such as TLS/1.0, given in the Upgrade header field.
		PreconditionRequired = 428, //The origin server requires the request to be conditional. Intended to prevent "the 'lost update' problem, where a client GETs a resource's state, modifies it, and PUTs it back to the server, when meanwhile a third party has modified the state on the server, leading to a conflict."[19]
		TooManyRequests = 429, //The user has sent too many requests in a given amount of time. Intended for use with rate limiting schemes.[19]
		RequestHeaderFieldsTooLarge = 431, //The server is unwilling to process the request because either an individual header field, or all the header fields collectively, are too large.[19]
		UnavailableForLegalReasons = 451, //Defined in the internet draft "A New HTTP Status Code for Legally-restricted Resources".[23] Intended to be used when resource access is denied for legal reasons, e.g. censorship or government-mandated blocked access. A reference to the 1953 dystopian novel Fahrenheit 451, where books are outlawed.[24]
		InternalServerError = 500, //A generic error message, given when an unexpected condition was encountered and no more specific message is suitable.
		NotImplemented = 501, //The server either does not recognize the request method, or it lacks the ability to fulfill the request. Usually this implies future availability (e.g., a new feature of a web-service API).
		BadGateway = 502, //The server was acting as a gateway or proxy and received an invalid response from the upstream server.
		ServiceUnavailable = 503, //The server is currently unavailable (because it is overloaded or down for maintenance). Generally, this is a temporary state.
		GatewayTimeout = 504, //The server was acting as a gateway or proxy and did not receive a timely response from the upstream server.
		HTTPVersionNotSupported = 505, //The server does not support the HTTP protocol version used in the request.
		NotExtended = 510, //Further extensions to the request are required for the server to fulfil it.[32]
		NetworkAuthenticationRequired = 511, //The client needs to authenticate to gain network access. Intended for use by intercepting proxies used to control access to the network (e.g., "captive portals" used to require agreement to Terms of Service before granting full Internet access via a Wi-Fi hotspot).[19]
	};

	class StatusDescriptions {
	public:
		static const std::string& Get(StatusCode code) {
			return _descriptions[code];
		}

	private:
		static std::map<StatusCode, std::string> _descriptions;
	};

	enum class Method {
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		PATCH
	};

	class Methods {
	public:
		static std::optional<Method> Get(const std::string &method) {
			if (method == "GET") return Method::GET;
			if (method == "HEAD") return Method::HEAD;
			if (method == "POST") return Method::POST;
			if (method == "PUT") return Method::POST;
			if (method == "DELETE") return Method::DELETE;
			if (method == "PATCH") return Method::PATCH;
			return std::optional<Method> { };
		}
	};

	class ContentTypes {
	public:
		static const std::optional<std::string> GetForExtension(const std::string &extension) {
			auto match = _contentTypes.find(extension);

			if (match != _contentTypes.end()) {
				return match->second;
			} else {
				return std::optional<std::string> { };
			}
		}

	public:
		static const std::string& Html() { return _html; }
		static const std::string& Css() { return _css; }
		static const std::string& PlainText() { return _plainText; }
		static const std::string& Png() { return _png; }
		static const std::string& Ico() { return _ico; }
		static const std::string& Manifest() { return _manifest; }

	private:
		static std::string _html;
		static std::string _css;
		static std::string _plainText;
		static std::string _png;
		static std::string _ico;
		static std::string _manifest;
		static std::map<std::string, std::string> _contentTypes;
	};
}

#endif //__HTTP_H__