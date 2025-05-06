#include "proxy_parse.hpp"
#include <algorithm>

void ParsedRequest::setHeader(const std::string& key, const std::string& value) {
    removeHeader(key);
    headers.emplace_back(key, value);
}

void ParsedRequest::removeHeader(const std::string& key) {
    headers.erase(std::remove_if(headers.begin(), headers.end(), [&](const ParsedHeader& h) {
        return h.key == key;
    }), headers.end());
}

void ParsedRequest::clearHeaders() {
    headers.clear();
}

std::string ParsedRequest::unparse() const {
    std::string result = method + " " + requestURI + " " + version + "\r\n";
    for (const auto& header : headers) {
        result += header.key + ": " + header.value + "\r\n";
    }
    result += "\r\n";
    return result;
}

std::unique_ptr<ParsedRequest> ParsedRequest::parse(const std::string& data) {
    auto pr = std::make_unique<ParsedRequest>();
    size_t method_end = data.find(' ');
    size_t uri_end = data.find(' ', method_end + 1);
    size_t version_end = data.find("\r\n", uri_end + 1);

    if (method_end == std::string::npos || uri_end == std::string::npos || version_end == std::string::npos)
        return nullptr;

    pr->method = data.substr(0, method_end);
    pr->requestURI = data.substr(method_end + 1, uri_end - method_end - 1);
    pr->version = data.substr(uri_end + 1, version_end - uri_end - 1);

    size_t header_start = version_end + 2;
    while (header_start < data.size() && data.substr(header_start, 2) != "\r\n") {
        size_t colon = data.find(':', header_start);
        size_t line_end = data.find("\r\n", header_start);
        if (colon == std::string::npos || line_end == std::string::npos)
            break;

        std::string key = data.substr(header_start, colon - header_start);
        std::string value = data.substr(colon + 2, line_end - colon - 2);
        pr->headers.emplace_back(key, value);

        header_start = line_end + 2;
    }

    return pr;
}

size_t ParsedRequest::requestLineLength() const {
    return method.size() + 1 + requestURI.size() + 1 + version.size() + 2;
}

void ParsedRequest::printRequestLine(std::string& out) const {
    out = method + " " + requestURI + " " + version + "\r\n";
}