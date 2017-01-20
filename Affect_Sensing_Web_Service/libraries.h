#ifndef LIBRARIES_H
#define LIBRARIES_H

// STL
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <istream>
#include <ctime>
#include <set>
#include <Windows.h>

using namespace std;

// C++ REST SDK
#include <cpprest/http_client.h>				// HTTP client
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios

using namespace utility;						// Common utilities like string conversions
using namespace web;							// Common features like URIs.
using namespace http;
using namespace web::http;						// Common HTTP functionality
using namespace web::http::client;				// HTTP client features
using namespace concurrency::streams;			// Asynchronous streams
using namespace web::http::experimental::listener;          // HTTP server
using namespace web::experimental::web_sockets::client;     // WebSockets client
using namespace web::json;

#endif // LIBRARIES_H