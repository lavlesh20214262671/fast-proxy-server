#ifndef PROXY_PARSE_HPP
#define PROXY_PARSE_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdarg>

class ParsedHeader {
public:
    std::string key;
    std::string value;

    ParsedHeader() = default;
    ParsedHeader(const std::string& k, const std::string& v) : key(k), value(v) {}
};

class ParsedRequest {
public:
    std::string method;
    std::string requestURI;
    std::string version;
    std::string path;
    std::vector<ParsedHeader> headers;

    ParsedRequest() = default;
    ~ParsedRequest() = default;

    void setHeader(const std::string& key, const std::string& value);
    void removeHeader(const std::string& key);
    void clearHeaders();
    std::string unparse() const;
    static std::unique_ptr<ParsedRequest> parse(const std::string& data);

private:
    size_t requestLineLength() const;
    void printRequestLine(std::string& out) const;
};

inline void debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

#endif // PROXY_PARSE_HPP
