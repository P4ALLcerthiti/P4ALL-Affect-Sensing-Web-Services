#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>

#include "Affect_Sensing_Web_Service.h"

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#else
# include <sys/time.h>
#endif

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

unique_ptr<Affect_Sensing_Services> g_httpServiceProvider;


bool on_initialize(const string_t& address)
{
	try
	{
		// People Counting
		uri_builder uri(address);
		uri.append_path(U("Affect_Sensing_Module/"));

		utility::string_t addr = uri.to_uri().to_string();
		g_httpServiceProvider = unique_ptr<Affect_Sensing_Services>(new Affect_Sensing_Services(addr));
		g_httpServiceProvider->open().wait();
		
		ucout << utility::string_t( U("Affect Sensing Server is listening for requests at: ") ) << addr << endl;
		return true;
	}
	catch (...)
	{
		ucout << utility::string_t(U("Cannot initialize the web server!!!")) << endl;
		ucout << utility::string_t(U("Probably another instance is running or the port is used by another application!\n\n")) << endl;
		return false;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
void on_shutdown()
{
	g_httpServiceProvider->close().wait();
	return;
}


///////////////////////////////////////////////////////////////////////////////
// To start the server, run the below command with admin privileges:
// Server.exe <port>
// If port is not specified, will listen on 34568
#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	utility::string_t port = U("8080");

	utility::string_t address = U("http://160.40.50.209:");
	//utility::string_t address = U("http://localhost:");

	address.append(port);

	bool retVal = on_initialize(address);

	cout << "Press ENTER to exit." << endl;
	string line;
	getline(cin, line);

	if (retVal == true)
		on_shutdown();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
