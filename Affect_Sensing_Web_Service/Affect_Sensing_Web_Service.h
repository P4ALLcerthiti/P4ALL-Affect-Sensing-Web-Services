#pragma once

#include "libraries.h"
#include "affect_sensing_module.h"

#include <vector>
#include <deque>
#include <string>



class Affect_Sensing_Services
{
public:
	Affect_Sensing_Services();
	Affect_Sensing_Services( utility::string_t url );
	~Affect_Sensing_Services();

	pplx::task<void> open(){ return m_listener.open(); }
	pplx::task<void> close(){ return m_listener.close(); }

private:

	bool checkQueryComponent(http_request &message, utility::string_t i_componentName, std::string &sensor_id_url);
	bool getValueFromKey(http_request &message, utility::string_t key, std::string value);
	void JSONreply(json::value &item, http_request &message, bool isOK =true);
	double GetCurrentTimeStamp();
	std::vector<double> tokenize_boost(std::string line, std::string delimiter);
	std::string store_in_DB( affect_sensing_module afsm, std::string subName);
	//int numberOfFilesInDirectory( std::string directory, std::string ext );
	
	std::string GetCurrentTimeStampStr();
	std::wstring addTime2Message(std::string i_msg);
	std::wstring string2wstring(std::string i_msg);

	// ... methods::GET
	// ... methods::POST
	
	void testing_post(http_request &message);
	void detect_stress(http_request &message);
	void detect_stress_from_DB(http_request &message);
	void post_stress_data_to_DB( http_request &message );

	// ... methods::PUT
	// ... methods::DEL


private:
	http_listener m_listener;

	int m_iCounter;
	std::vector<double> m_timeStamps;
	std::vector<double> m_SkinCond;
	std::vector<double> m_IBI;
	int m_secBetweenIntervals;

	void handle_get(http_request &message);
	void handle_put(http_request &message);
	void handle_post(http_request &message);
	void handle_delete(http_request &message);


};