# Webserv – High-Performance C++ Web Server

## Introduction
Webserv is a custom HTTP/1.1 web server built from scratch in C++98, implementing core web server features without external libraries​

. It was developed as a systems programming project to deeply understand how web servers like Nginx or Apache work at a low level. Webserv runs on Linux and macOS, handling multiple client connections asynchronously in a single process. It supports real-world use cases such as serving static websites, handling file uploads, and executing CGI scripts for dynamic content​


. The server is non-blocking and event-driven, meaning it can serve many clients concurrently without threading or blocking calls​
. Webserv has been tested for compatibility with modern browsers (Chrome, Firefox, etc.)​
, making it suitable for showcasing in a portfolio or deployment in a small-scale web application.

## Features
**HTTP/1.1 Compliance:**  
Supports essential HTTP methods – GET, POST, and DELETE – with correct response codes and headers​
. Handles persistent connections and chunked transfer encoding as required for browser compatibility​
. Provides accurate default error responses (e.g. 404 Not Found, 500 Internal Server Error) with customizable error pages​


**Static File Serving:**  
Serves static files from designated root directories. Supports configurable index files (e.g. index.html) for directory requests and optional directory listing (autoindex) for browsing folder contents​

**Virtual Hosting & Routing:**  
Supports multiple server blocks on different ports or hostnames in one configuration, enabling basic virtual hosting​


. Within each server, multiple route locations can be defined with longest-prefix matching for efficient request routing (implemented via a trie structure for fast lookup)​


**Configurable Request Limits:**  
Allows setting a maximum client request body size per server or route to prevent abuse (e.g. client_max_body_size directive)​
GITHUB.COM. Requests exceeding the limit are rejected with appropriate errors.

**File Uploads:**  
Clients can upload files to the server via HTTP POST. Specific routes can be configured to accept file uploads and store them in a designated directory on the server​
. This is useful for implementing features like image uploads or form submissions.

**CGI Support:**  
Executes external programs via the Common Gateway Interface (CGI) for dynamic content. You can designate file extensions (e.g. .php, .py) that trigger CGI execution, and the server will run the corresponding interpreter (PHP, Python, Bash, etc.) to generate a response​

​
GITHUB.COM. Both GET and POST methods are supported for CGI endpoints.

**Asynchronous Event-Driven I/O:**  
Utilizes a single I/O multiplexing loop (poll, with OS-specific variants like kqueue on macOS or epoll on Linux) to handle all client connections without blocking​
. This architecture efficiently serves multiple clients in parallel on one thread, using non-blocking sockets and readiness notifications for read/write events.

**High Performance and Optimization:**  
Built with system-level efficiency in mind. A custom buffer management system is used for network I/O to minimize syscalls and allocations, improving data throughput​
 Frequently requested resources can be cached in-memory with an LRU (Least Recently Used) cache to reduce disk reads​
 The routing lookup uses a Trie to achieve O(m) lookup (where m is URL length) for the best-matching location, which scales well even as configuration grows​


**Multi-Platform Support:**  
Designed to run on both Linux and macOS. It uses epoll on Linux and kqueue on BSD-based systems like macOS for efficient event polling, with fallbacks to poll/select for portability​

​
. File descriptor flags (e.g. O_NONBLOCK) are used on macOS to ensure non-blocking behavior consistent with Linux​
. The code is compliant with POSIX standards and was tested in both environments.

**Robustness:**  
The server is engineered to be stable under stress – it will not crash or hang even under high load or malformed requests​
. It gracefully handles errors (with default or custom error pages) and cleans up resources. Extensive testing was done with automated scripts and tools to verify stability, memory safety, and correct concurrency handling.

## Technical Stack
**Language & Standards:**  
C++98 (compiled with -Wall -Wextra -Werror flags)​
. The project uses only the C++ standard library and OS system calls – no external frameworks or libraries​
 – demonstrating low-level programming skills.

**Operating Systems:**  
Developed and tested on Linux (Ubuntu) and macOS (Darwin/BSD). The code uses conditional compilation to invoke Linux-specific APIs (e.g. epoll) or BSD APIs (e.g. kqueue) as available, ensuring optimized performance on each platform.

**Network Programming:**  
POSIX sockets API (syscalls like socket, bind, listen, accept, send, recv) forms the basis of connection handling​
. Non-blocking socket mode is used along with I/O multiplexing (poll/select/epoll/kqueue) for event-driven behavior.

**Concurrency Model:**  
Single-threaded, asynchronous event loop architecture. All client I/O is handled in one thread via multiplexing​
. For executing CGI programs, subprocesses are spawned using fork and execve as needed​
, with pipes used to capture output without blocking the main server.

**Data Structures:**  
Custom-built structures and algorithms were used for performance:  
- **Buffer** – a custom buffer class for reading/writing sockets efficiently (inspired by analyzing STL containers)​
FILE-EAOQ3MUHEKKEIT94EHNTW2, which reduces allocations and copies during HTTP message transfer.  
- **Trie** – used for parsing and storing the configuration of route locations, enabling quick longest-prefix match for incoming request URIs​
  
- **LRU Cache** – caches frequently accessed files or generated responses in memory, with eviction of least-recently-used items to cap memory usage​


**HTTP Parsing & Generation:**  
The server includes an HTTP request parser that reads incoming data byte-by-byte and builds the request structure (method, URI, headers, body). It carefully follows HTTP/1.1 requirements for parsing, including handling chunked transfer encoding and connection persistence. The response generator can form proper HTTP responses (status line, headers, body) and supports content types, content-length or chunked encoding, and persistent or closed connections.

**Configuration:**  
A custom configuration file parser is implemented to read the server configuration (see below). This involves lexical analysis of the config text and populating internal data structures for servers and routes. The design was inspired by Nginx’s configuration grammar​
 but simplified (no regex in routes, only prefix matching).

**Build System:**  
Uses a Makefile for compilation. Simply running make builds the webserv executable. The codebase is organized into modules (HTTP parsing, response handling, config parsing, CGI, utils, etc.) for clarity. No Boost or external build tools are used.

## Architecture Overview
**Event Loop & Connection Handling:**  
On startup, Webserv opens listening sockets on the configured ports (it supports multiple listen sockets for different ports or addresses)​
. All sockets (listening and client connections) are set to non-blocking mode. The server enters an event loop using poll() (or epoll/kqueue depending on OS) to monitor all sockets for readability or writability in a single system call​
. When poll indicates one or more sockets are ready, the server:
- Accepts new connections on listening sockets (creating a new client socket for each incoming connection).
- Reads incoming data from client sockets that have new data. Data is read into the custom buffer structure, which can handle partial reads and buffer the data until a full HTTP request is received.
- Writes outgoing data to client sockets that are ready to be written (e.g., sending a response back), using the buffer to handle partial writes (sending the data in chunks if necessary until complete).
This non-blocking, asynchronous design means the server can handle many clients concurrently with minimal overhead, interleaving I/O operations efficiently.

**HTTP Request Lifecycle:**  
Each client connection in the server is managed by a stateful handler:
- **Parsing Request:** As data comes in, the HTTP parser incrementally parses the request line, headers, and body. If the request is invalid or violates HTTP specs (e.g., too large, bad syntax), the server prepares an HTTP error response (400, 413, etc.) and flags the connection for closure after sending the error​


.
- **Routing:** Once a complete request is parsed, the server determines which configuration route should handle it. This is done by comparing the request URI against the route prefixes in the config. A Trie is used to efficiently find the longest prefix match, which corresponds to the most specific location block in the configuration for that request​
 The route configuration provides settings like the filesystem root for that route, allowed methods, whether to generate a directory listing, and if a CGI or upload handling is required.
- **Generating Response:** Based on the route and request:
  - For a static file request (e.g., GET on a regular file path), the server constructs the file path by appending the request URI to the route’s root directory. If the path is a directory and autoindex is on, it generates an HTML index of the directory. If the path is a directory and autoindex is off, it attempts to serve a default index file (e.g. index.html)​
GITHUB.COM

GITHUB.COM. The file is read from disk (unless cached) and sent as the response body with the appropriate Content-Type. If the file is not found, a 404 error is returned (with a custom error page if defined in config)​
GITHUB.COM.
  - For a file upload request (POST method to an upload-configured route), the server saves the request body to a file in the designated upload directory on the server​
. It then responds with a confirmation (e.g., 201 Created or a redirect to a success page). The upload handling ensures that only allowed file sizes (respecting client_max_body_size) are accepted​
GITHUB.COM.
  - For a CGI request (URI targeting a script like .php or .py configured for CGI), the server prepares the CGI environment variables (as required by the CGI protocol, such as QUERY_STRING, CONTENT_LENGTH, etc.) and uses fork() to create a subprocess. In the child process, it executes the script via the appropriate interpreter (set by cgi_path) using execve()​
. The request body (for POST) is passed to the CGI via STDIN, and the CGI’s output is read via a pipe. The parent process (server) integrates the CGI output into an HTTP response and sends it back to the client​
. This allows dynamic content generation (for example, running a PHP script to access a database and produce HTML).
  - For a redirect (if the route is configured with a return/redirection), the server responds with a 301/302 status and Location header as per config.
  - For a DELETE request, the server attempts to delete the target file from the server’s file system (if allowed and within the served directories) and returns an appropriate status (200 OK or 204 No Content on success, 404 if file not found, 403 if forbidden).
- **Response Transmission:**  
  The response (header and body) is stored in the connection’s buffer. The event loop will detect the socket is ready for writing and send out the response bytes. Thanks to non-blocking sends, large responses are split as needed and sent over multiple iterations without stalling the server. If the connection is persistent (Keep-Alive) and the client has not signaled closure, the server will then wait for the next request on the same socket. If the connection is to be closed (e.g., after sending an error or as requested), the server will close the socket after sending the last bytes.

**Buffer and Resource Management:**  
The internal buffer system manages memory for incoming and outgoing data per connection, reusing allocations to minimize overhead. The LRU cache, if enabled, stores recently used files or CGI outputs – for instance, if many clients request the same CSS or image file, it can be served from memory quickly on subsequent requests. Cache entries are invalidated on file changes or after a configurable size/time to keep content fresh. All resources (sockets, file descriptors, memory) are carefully freed or closed after use. The server never crashes even under memory pressure; if resources are low, it will refuse new connections or requests gracefully rather than terminate​


. 

**Error Handling and Logging:**  
Webserv includes robust error handling. If a request cannot be fulfilled (due to client error or server error), it responds with an appropriate HTTP error code and message. Default error pages are built-in, and the configuration allows overriding the error page per status code (e.g. a custom 404 page)​
. The server logs key events (requests, errors) to the console or a log file for debugging. This helps in monitoring the server’s behavior and diagnosing issues, especially during stress tests or when integrating with other systems. Overall, the architecture follows an event-driven design similar to modern high-concurrency servers. By combining non-blocking I/O, efficient algorithms, and careful resource management, Webserv achieves a high level of performance and stability within the constraints of a school project environment.

## Configuration File Example
Below is an example webserv.conf configuration illustrating how to set up the server and routes (inspired by Nginx-style syntax):

```conf
# Example webserv configuration

server {
    listen       8080;               # Port to listen on (IPv4)
    host         0.0.0.0;            # Address to bind (0.0.0.0 for all interfaces)
    server_name  mysite.local;       # Virtual host name (optional)
    error_page   404 /errors/404.html;
    error_page   500 /errors/50x.html;
    client_max_body_size  10M;       # Limit upload size to 10 MB
    root         /var/www/mysite;    # Root directory for this server
    index        index.html index.htm;

    location / { 
        # Default route serving static files from /var/www/mysite
        try_files $uri $uri/ =404;   # Serve file or directory if exists, else 404
        autoindex  off;             # Disable directory listing for the root
        allow_methods GET POST;     # Allowed HTTP methods
    }

    location /uploads/ {
        # Route for handling file uploads
        methods      POST GET;
        root         /var/www/mysite/uploads;
        autoindex    on;            # Enable directory listing for uploads folder
        upload_store /var/www/mysite/uploads;  # Directory to save uploaded files
    }

    location /cgi-bin/ {
        # Route for CGI scripts
        root         /var/www/mysite/cgi-bin;
        cgi_ext      .py;                      # File extension that triggers CGI
        cgi_path     /usr/bin/python;          # Interpreter to execute .py scripts
        allow_methods GET POST;
    }
}
```

In this configuration:
- We define a server listening on port 8080 for host `0.0.0.0` (all network interfaces). The server will respond to requests for `mysite.local` (which should be defined in your hosts file for testing). Custom error pages are specified for 404 and 500 errors, and the maximum request body size is set to 10MB (to limit upload size)​
GITHUB.COM.
- The root directory for files is `/var/www/mysite`, and the default index files are `index.html` or `index.htm` if a directory is requested.
- The `/` location block is the default for all requests on this server. It serves static files and directories from `/var/www/mysite`. Directory listing is turned **off** here (so if a directory has no index file, a 404 is returned). Allowed methods are GET and POST (meaning a POST to `/` could be handled, e.g., for a form submission).
- The `/uploads/` location is set up to accept file uploads. It allows GET (to list or download uploaded files) and POST (to upload new files). The files will be saved under `/var/www/mysite/uploads`. Here autoindex is **on** to allow browsing the uploaded files directory in a browser.
- The `/cgi-bin/` location is designated for CGI scripts. Any request for a `.py` file under this path will invoke the Python interpreter at `/usr/bin/python` to execute the script​
GITHUB.COM. We allow both GET and POST so the CGI scripts can handle form submissions. For example, a request to `/cgi-bin/test.py` will run the script `test.py` and return its output as the HTTP response.
- This is just one example – the configuration is flexible. You can add multiple `server` blocks to host different sites or bind different ports, and each server can have multiple `location` blocks for fine-grained control over request handling​

​
GITHUB.COM.

## How to Build & Run
Requirements: You need a C++ compiler (supporting C++98) and Make. Tested compilers include GCC and Clang on Linux and macOS. No additional libraries are required.

**Clone the repository:**
```bash
git clone https://github.com/42-webserver/webserv.git
cd webserv
```

**Build the server:** Simply run `make` in the project directory. This will produce an executable called `webserv`. (The Makefile includes targets: `all`, `clean`, `fclean`, `re`​
.)
  
**Prepare a configuration:** You can use the provided example configuration (if available in the repository) or create your own. For instance, save the above example as `webserv.conf` or modify it as needed. Ensure the paths (for root, error pages, etc.) exist on your machine, or create the directories/files accordingly.

**Run the server:**
```bash
./webserv webserv.conf
```
If no configuration file is specified, Webserv will attempt to use a default config path (as coded, see documentation). The server will daemonize in the foreground, listening on the specified ports. You should see console output for startup.

**Access the server:** Open a web browser and navigate to the address and port you've configured (e.g. `http://localhost:8080`). You should see the index page or directory listing as configured. You can also use command-line tools like `curl` or `telnet` to send requests.
  
*Note:* On macOS, you might need to allow the program to accept incoming network connections (depending on firewall settings). Also, because Webserv uses low-level system calls, you may need to run it from a Terminal with appropriate permissions to bind to low-numbered ports (<1024) or to create files in certain directories.

## Test & Verification Guide
To ensure Webserv works correctly, it was rigorously tested using various methods:

**Browser Testing:**  
The most straightforward test is using a web browser. For example, after running the server (listening on port 8080), navigate to `http://localhost:8080` in Chrome/Firefox/Edge. Verify that your default page loads. Test navigating to subdirectories to see directory listings or index files, and try accessing a non-existent URL to see the custom 404 error page. This confirms compatibility with real browser HTTP requests​
 (including handling of persistent connections and proper HTTP headers that browsers expect).

**Manual Telnet Testing:**  
For a deeper look at the HTTP protocol, use telnet or netcat:
```bash
telnet localhost 8080
```
Then manually type an HTTP request, for example:
```http
GET /uploads/ HTTP/1.1
Host: localhost
```
(Make sure to include an extra blank line to end the request.) You should see the raw HTTP response from the server, including status line and headers followed by the content (or a directory listing in HTML for the uploads directory in this example). This helps verify the correctness of the HTTP parsing and response formatting.

**cURL Commands:**  
Using [cURL](https://curl.se/) allows automated testing of various scenarios:
- **GET request:**  
  `curl -v http://localhost:8080/path/to/file` will show the response headers and body. This can test static file serving and 404 handling.
- **POST upload:**  
  Prepare a file and use `curl -F 'file=@/path/to/localfile.jpg' http://localhost:8080/uploads/` to simulate an HTML form file upload. After running this, verify (via browser or `ls` command) that the file appears in the upload directory on the server. Also, the HTTP response from the server should indicate success (e.g., 201 Created or redirect).
- **DELETE request:**  
  If you have a file that can be deleted via the server, `curl -X DELETE http://localhost:8080/uploads/oldfile.txt` and then check that the file is removed on the server and the response status is appropriate (204 No Content or similar).
- **CGI execution:**  
  `curl http://localhost:8080/cgi-bin/test.py?param=value` to test a CGI script. The output of the script (which might include the query parameter) should be returned. Also test a POST to CGI (using `curl -d "foo=bar&baz=qux" http://localhost:8080/cgi-bin/test.py` with appropriate content type) to ensure the server passes the request body to the script.

**Concurrent Stress Testing:**  
To ensure the server remains stable under load​
, use a load-testing tool or script:
- The Apache Benchmark tool (`ab`) can be used, e.g.:
  ```bash
  ab -n 1000 -c 100 http://localhost:8080/
  ```
  which will send 1000 requests with up to 100 in parallel. Webserv should handle this without errors or crashes. Monitor memory and CPU usage to ensure it stays within reasonable bounds.
- Alternatively, use `wrk` or a custom Python script using `asyncio`/threads to fire many concurrent requests. During stress tests, Webserv should continue serving all requests promptly and remain responsive.

**Memory and Error Testing:**  
Tools like **Valgrind** were used during development to ensure there are no memory leaks or invalid memory accesses. Additionally, various malformed or edge-case HTTP requests were tested (e.g., very long URLs, invalid HTTP syntax, extremely large bodies) to ensure the server handles them gracefully (either by responding with an error and closing the connection or by safely ignoring bad input)​
. The server’s resilience means it should never crash even in out-of-memory situations or when facing malicious inputs​
.

By following the above testing approaches, we verified that Webserv meets the project requirements and behaves reliably. Feel free to conduct your own tests; the server should handle anything that a standard HTTP/1.1 server is expected to handle within the implemented feature set.

Feel free to reach out for any questions or further details about the project.
