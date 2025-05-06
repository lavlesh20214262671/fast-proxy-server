


# Multi-threaded Proxy Server and Client (C++)

## Overview

This project is a **multi-threaded proxy server** written in **C++**. The proxy server acts as an intermediary between the client and the web server, forwarding requests and responses. The project includes two versions of the proxy server:
1. **Proxy server with cache**: Stores and reuses responses to speed up future requests.
2. **Proxy server without cache**: Always fetches fresh data from the origin server.

### Key Features:
- **HTTP Request Parsing**: The proxy parses incoming HTTP requests using a custom parser.
- **Multi-threading**: Handles multiple client requests concurrently using separate threads.
- **Caching**: Optionally caches responses to improve performance for repeated requests.

---

## Project Structure

```

/MultiThreadedProxyServerClient-CPP-WindowsFixed
│
├── bin/                     # Compiled binaries
├── include/                 # Header files
│   └── proxy\_parse.hpp      # Header for parsing HTTP requests
├── pics/                    # Project-related images or diagrams (optional)
├── src/                     # Source code files
│   ├── proxy\_parse.cpp      # Parser for HTTP requests
│   ├── proxy\_server\_with\_cache.cpp # Proxy server with caching
│   ├── proxy\_server\_without\_cache.cpp # Proxy server without caching
├── Makefile                 # Build file to compile the project

````

---

## Getting Started

### Prerequisites

Make sure you have the following installed on your machine:
- A C++ compiler (e.g., `g++`, `clang++` for Linux/Mac, or MSVC for Windows)
- `Make` (for building the project)
- If on Windows, a suitable IDE or command-line toolchain that supports C++11 or higher.

### Building the Project

1. Open a terminal (or command prompt).
2. Navigate to the project directory where the `Makefile` is located.
3. Run the following command to build the project:
    ```bash
    make
    ```

### Running the Proxy Server

Once the project is built, you can run either the **cached** or **uncached** proxy server.

#### Running the Proxy Server (Without Cache)

```bash
./proxy_server_without_cache
````

#### Running the Proxy Server (With Cache)

```bash
./proxy_server_with_cache
```

### Testing the Proxy

To test the proxy, you can configure your browser or use a tool like **curl** or **Postman** to make HTTP requests through the proxy.

Example using **curl**:

```bash
curl --proxy http://localhost:8080 http://example.com
```

This will send a request to the `example.com` website through the proxy running on `localhost` at port `8080`.

---

## How It Works

1. **Client sends HTTP request**: The client (browser, curl, etc.) sends an HTTP request to the proxy server.
2. **Proxy parses the request**: The proxy parses the request using the `ParsedRequest` class, which breaks the raw HTTP string into method, URI, headers, and version.
3. **Caching (Optional)**: The proxy checks if the requested page is already cached. If so, it serves it from the cache.
4. **Proxy forwards the request**: If not cached, the proxy forwards the request to the destination server (e.g., example.com).
5. **Proxy sends response**: The server's response is received and sent back to the client. If caching is enabled, the response is saved for future requests.
6. **Multi-threading**: The server can handle multiple clients at once by using separate threads for each request, allowing concurrent connections.

---

## Components

### 1. **HTTP Request Parsing (`proxy_parse.cpp` and `proxy_parse.hpp`)**

* **`ParsedRequest`**: Represents the full HTTP request, including method, URI, version, and headers.
* **`parse()`**: A static function that parses the raw HTTP request string into a `ParsedRequest` object.
* **`unparse()`**: Rebuilds the HTTP request from the `ParsedRequest` object back into raw string format.

### 2. **Proxy Server Core**

* **With Cache**: Stores previously fetched responses for reuse, speeding up future requests.
* **Without Cache**: Always fetches fresh responses from the web server.

### 3. **Multi-threading**

* Each client connection is handled in a separate thread, allowing multiple clients to interact with the proxy server at the same time without blocking each other.

---

## Future Improvements

* **Support for HTTPS**: Implement SSL/TLS encryption to support secure HTTPS traffic.
* **Advanced Caching**: Implement a more sophisticated cache eviction algorithm (e.g., LRU).
* **Persistent Connections**: Support HTTP/1.1 persistent connections (keep-alive).
* **Error Handling**: Improve error handling and response codes for invalid requests.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```

---

### How to Use the README:
1. Copy and paste the above content into a file named `README.md`.
2. Place the `README.md` file into your project directory (e.g., alongside the `Makefile`).

This README covers the project setup, building, and running instructions, along with an explanation of how the proxy server works and the components involved.

Let me know if you'd like me to make any adjustments or add more details! 😊
```
