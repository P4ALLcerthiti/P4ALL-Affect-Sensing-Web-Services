// Affect_Sensing_Web_Service.cpp : Defines the entry point for the console application.
//
#pragma warning( disable : 4018 )
#pragma warning( disable : 4996 )

#include "stdafx.h"
#include "Affect_Sensing_Web_Service.h"
#include "affect_sensing_module.h"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
//#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lexical_cast.hpp>



Affect_Sensing_Services::Affect_Sensing_Services()
{
}
///////////////////////////////////////////////////////////////////////////////
Affect_Sensing_Services::Affect_Sensing_Services(utility::string_t url) : m_listener(url)
{
	m_listener.support( methods::GET,  std::bind( &Affect_Sensing_Services::handle_get,    this, std::placeholders::_1 ) );
	m_listener.support( methods::PUT,  std::bind( &Affect_Sensing_Services::handle_put,    this, std::placeholders::_1 ) );
	m_listener.support( methods::POST, std::bind( &Affect_Sensing_Services::handle_post,   this, std::placeholders::_1 ) );
	m_listener.support( methods::DEL,  std::bind( &Affect_Sensing_Services::handle_delete, this, std::placeholders::_1 ) );
}
///////////////////////////////////////////////////////////////////////////////
Affect_Sensing_Services::~Affect_Sensing_Services()
{
}
///////////////////////////////////////////////////////////////////////////////
void Affect_Sensing_Services::handle_post(http_request &message)
{
	std::vector< utility::string_t > paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
	if (paths.empty())
	{
		message.reply(status_codes::OK, "Not Acceptable URI");
	}

	try
	{
		if (paths[0].compare( utility::conversions::to_string_t("testing_post")) == 0 )
		{
			std::cout << "handle_post -> testing_post " << std::endl;
			testing_post( message );
		}
		else if ( paths[0].compare(utility::conversions::to_string_t("detect_stress")) == 0 )
		{
			std::cout << "handle_post -> detect_stress " << std::endl;
			detect_stress( message );
		}
		else if ( paths[0].compare( utility::conversions::to_string_t( "detect_stress_from_DB" ) ) == 0 )
		{
			std::cout << "handle_post -> detect_stress_from_DB " << std::endl;
			detect_stress_from_DB( message );
		}
		else if ( paths[0].compare( utility::conversions::to_string_t( "post_stress_data_to_DB" ) ) == 0 )
		{
			std::cout << "handle_post -> post_stress_data_to_DB " << std::endl;
			post_stress_data_to_DB( message );
		}

	}
	catch (std::exception const & ex)
	{
		cout << ex.what();
	}
	catch (...)
	{
		cout << "exception";
	}

	paths.clear();

}
///////////////////////////////////////////////////////////////////////////////
void Affect_Sensing_Services::handle_put(http_request &message)
{
}
///////////////////////////////////////////////////////////////////////////////
void Affect_Sensing_Services::handle_delete(http_request &message)
{
}
///////////////////////////////////////////////////////////////////////////////
void Affect_Sensing_Services::handle_get(http_request &message)
{
	ucout << "Inside handle_get" << endl;

	std::vector< utility::string_t > paths = http::uri::split_path( http::uri::decode( message.relative_uri().path() ) );
	if (paths.empty())
	{
		message.reply(status_codes::OK, "Wrong query - More attributes required !");
		return;
	}

	affect_sensing_module * afsm = new affect_sensing_module();
	if (afsm == NULL)
	{
		ucout << "AffectSensingModule = NULL " << endl;
		paths.clear(); 
		return; 
	}

	if (paths[0].compare(utility::conversions::to_string_t("detect_stress")) == 0)
	{
		ucout << "Inside handle_get.detect_stress " << endl;
		//detect_stress(message, afsm);
	}
	else if (paths[0].compare(utility::conversions::to_string_t("display_number")) == 0)
	{
		ucout << "Inside handle_get.display_number " << endl;
		//display_Number(message, afsm);
	}
	else
	{
		ucout << "Inside handle_get.else " << endl;
		message.reply(status_codes::Forbidden, U("Wrong query"));
		paths.clear();
		delete &afsm;
		return;
	}

	paths.clear();
	delete &afsm;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool Affect_Sensing_Services::checkQueryComponent(http_request &message, utility::string_t i_componentName, std::string &sensor_id_url)
{
	try
	{
		std::map<utility::string_t, utility::string_t> query = uri::split_query(uri::decode(message.relative_uri().query()));
		std::map<utility::string_t, utility::string_t>::iterator queryItr = query.find(i_componentName);
		
		if (queryItr == query.end())
		{
			query.clear();
			return false;
		}
		utility::string_t sensor_id_url = queryItr->second;
		query.clear();
	}
	catch (...)
	{
		return false;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool Affect_Sensing_Services::getValueFromKey(http_request &message, utility::string_t key, std::string value)
{
	try
	{
		std::map< utility::string_t, utility::string_t > query = uri::split_query( uri::decode(message.relative_uri().query()) );
		std::map< utility::string_t, utility::string_t > ::iterator queryItr = query.find(key);

		if (queryItr == query.end())
		{
			query.clear();
			return false;
		}
		utility::string_t value1 = queryItr->second;
		wstring ws(value1);
		std::string temp(ws.begin(), ws.end());
		value = temp;
	}
	catch (...)
	{
		return false;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
void Affect_Sensing_Services::JSONreply(json::value &item, http_request &message, bool isOK/*=true*/)
{
	string_t json_reply = item.serialize();
	message.reply(status_codes::OK, json_reply);
}
///////////////////////////////////////////////////////////////////////////////
double Affect_Sensing_Services::GetCurrentTimeStamp()
{
	SYSTEMTIME oT;
	GetLocalTime(&oT);

	std::time_t i_rawtime = time(0);
	double i_timestamp = i_rawtime + oT.wMilliseconds / 1000.0;
	return i_timestamp;
}
///////////////////////////////////////////////////////////////////////////////
std::vector<double> Affect_Sensing_Services::tokenize_boost(std::string line, std::string delimiter)
{
	std::vector<std::string> parts;
	std::vector<double> toks;

	boost::split(parts, line, boost::is_any_of(delimiter));

	for (unsigned int i = 0; i<parts.size(); i++)
	{
		if (parts[i].length() != 0)
			toks.push_back(::atof(parts[i].c_str()));
	}

	parts.clear();
	return toks;
}
///////////////////////////////////////////////////////////////////////////////
std::string Affect_Sensing_Services::GetCurrentTimeStampStr()
{
	SYSTEMTIME oT;
	GetLocalTime(&oT);

	char tmpStr[1024];
	sprintf_s(tmpStr, "%02d-%02d-%04d %02d:%02d:%02d.%03d", oT.wDay, oT.wMonth, oT.wYear, oT.wHour, oT.wMinute, oT.wSecond, oT.wMilliseconds);
	return std::string(tmpStr);
}
///////////////////////////////////////////////////////////////////////////////
std::wstring Affect_Sensing_Services::string2wstring(std::string i_msg)
{
	std::wstring str2(i_msg.length(), L' ');
	std::copy(i_msg.begin(), i_msg.end(), str2.begin());
	return str2;
}
///////////////////////////////////////////////////////////////////////////////
std::wstring Affect_Sensing_Services::addTime2Message(std::string i_msg)
{
	std::string i_message = GetCurrentTimeStampStr() + ": " + i_msg;
	return string2wstring(i_message);
}
///////////////////////////////////////////////////////////////////////////////
//int Affect_Sensing_Services::numberOfFilesInDirectory( std::string directory, std::string ext )
//{
//	int num = 0;
//
//	boost::filesystem::path _path( directory );
//
//	boost::filesystem::directory_iterator end_iter;
//
//	for ( boost::filesystem::directory_iterator iter( _path ); iter != end_iter ; iter++ )
//	{
//		if ( iter->path().extension() == ext )
//			num++;
//	}
//
//	return num;
//
//}
///////////////////////////////////////////////////////////////////////////////
std::string Affect_Sensing_Services::store_in_DB( affect_sensing_module afsm, std::string subName )
{
	//afsm.m_stress_results->
	using boost::property_tree::ptree;

	std::string directory = ".\\..\\DataBase\\";
	std::string filename = directory + subName + ".ini" ;

	ptree ptr;

	ptr.put( "Subject.Name", subName );

	ptr.put( "GSR.SD",					 afsm.m_stress_results->m_GSR_sd[0] );
	ptr.put( "GSR.1stDer_AVG",			 afsm.m_stress_results->m_GSR_1stDer_AVG[0] );
	ptr.put( "GSR.1stDer_RMS",			 afsm.m_stress_results->m_GSR_1stDer_RMS[0] );
	ptr.put( "GSR.Rate",				 afsm.m_stress_results->m_GSR_Rate[0] );
	ptr.put( "GSR.Amplitude",			 afsm.m_stress_results->m_GSR_Amplitude[0] );
	ptr.put( "GSR.min",					 afsm.m_stress_results->m_GSR_min[0] );
	ptr.put( "GSR.max",					 afsm.m_stress_results->m_GSR_max[0] );
	ptr.put( "GSR.Amplitude_75",		 afsm.m_stress_results->m_GSR_Amplitude_Percentiles[2][0] );
	ptr.put( "GSR.Amplitude_85",		 afsm.m_stress_results->m_GSR_Amplitude_Percentiles[3][0] );
	ptr.put( "GSR.Smooth_Dervative_RMS", afsm.m_stress_results->m_GSR_Smooth_Dervative_RMS[0] );
	
	ptr.put( "EKG.mean",		afsm.m_stress_results->m_EKG_mean[0] );
	ptr.put( "EKG.SD",			afsm.m_stress_results->m_EKG_SD[0] );
	ptr.put( "EKG.Picard_1",	afsm.m_stress_results->m_EKG_Picard_1[0] );
	ptr.put( "EKG.Picard_2",	afsm.m_stress_results->m_EKG_Picard_2[0] );
	ptr.put( "EKG.Picard_F2",	afsm.m_stress_results->m_EKG_Picard_F2[0] );
	ptr.put( "EKG.max",			afsm.m_stress_results->m_EKG_max[0] );
	ptr.put( "EKG.Kurtosis",	afsm.m_stress_results->m_EKG_Kurtosis[0] );
	ptr.put( "EKG.SD2",			afsm.m_stress_results->m_EKG_SD2[0] );

	boost::property_tree::write_ini( filename, ptr, 0 );

	//int subID = numberOfFilesInDirectory(directory, ".ini");

	return filename;

}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Affect_Sensing_Services::testing_post(http_request &message)
{

	web::json::value temp_json = message.extract_json().get();

	utility::string_t par1 = temp_json[L"param1"].as_string();
	utility::string_t par2 = temp_json[L"param2"].as_string();
	utility::string_t par3 = temp_json[L"param3"].as_string();
	utility::string_t par4 = temp_json[L"param4"].as_string();

	std::string param1 = std::string( par1.begin(), par1.end() );
	std::string param2 = std::string( par2.begin(), par2.end() );
	std::string param3 = std::string( par3.begin(), par3.end() );
	std::string param4 = std::string( par4.begin(), par4.end() );

	param1.erase( remove( param1.begin(), param1.end(), '\"'), param1.end() );
	param2.erase( remove( param2.begin(), param2.end(), '\"'), param2.end() );
	param3.erase( remove( param3.begin(), param3.end(), '\"'), param3.end() );
	param4.erase( remove( param4.begin(), param4.end(), '\"'), param4.end() );


	json::value item;
	if (param1.length() != 0 && param2.length() != 0 && param3.length() != 0 && param4.length() != 0)
	{ 
		item[L"info"] = json::value::string(utility::conversions::to_string_t("All is good !"));
	}
	else
	{
		item[L"error"] = json::value::string(utility::conversions::to_string_t("some param is NULL"));
	}
	
	JSONreply(item, message);
}

void Affect_Sensing_Services::detect_stress(http_request &message)
{

	web::json::value JsonMessage = message.extract_json().get();

	json::value item;
	size_t size = JsonMessage.size();
	if ( JsonMessage.size() != 9 )
	{
		std::string reply = "Not enough values in JSON String !" ;
		item[L"error"] = json::value::string( utility::conversions::to_string_t( reply ) );
		JSONreply( item, message );
		return;
	}


	std::cout << "Acquiring JSON data !" << std::endl;
	// from JSON

		utility::string_t s_Time_GSR_Rest		= JsonMessage[L"Time_GSR_Rest"].as_string();
		utility::string_t s_Time_IBI_Rest		= JsonMessage[L"Time_IBI_Rest"].as_string();
		utility::string_t s_Data_GSR_Rest		= JsonMessage[L"Data_GSR_Rest"].as_string();
		utility::string_t s_Data_IBI_Rest		= JsonMessage[L"Data_IBI_Rest"].as_string();
		
		utility::string_t s_TimeSegmentTime		= JsonMessage[L"Segment_Seconds"].as_string();

		utility::string_t s_Time_GSR_Normal		= JsonMessage[L"Time_GSR_Normal"].as_string();
		utility::string_t s_Time_IBI_Normal		= JsonMessage[L"Time_IBI_Normal"].as_string();
		utility::string_t s_Data_GSR_Normal		= JsonMessage[L"Data_GSR_Normal"].as_string();
		utility::string_t s_Data_IBI_Normal		= JsonMessage[L"Data_IBI_Normal"].as_string();

	// to string

		std::string h_Time_GSR_Rest		= std::string( s_Time_GSR_Rest.begin(), s_Time_GSR_Rest.end() );
		std::string h_Time_IBI_Rest		= std::string( s_Time_IBI_Rest.begin(), s_Time_IBI_Rest.end() );
		std::string h_Data_GSR_Rest		= std::string( s_Data_GSR_Rest.begin(), s_Data_GSR_Rest.end() );
		std::string h_Data_IBI_Rest		= std::string( s_Data_IBI_Rest.begin(), s_Data_IBI_Rest.end() );

		std::string h_TimeSegmentTime   = std::string( s_TimeSegmentTime.begin(), s_TimeSegmentTime.end() );

		std::string h_Time_GSR_Normal	= std::string( s_Time_GSR_Rest.begin(), s_Time_GSR_Rest.end() );
		std::string h_Time_IBI_Normal	= std::string( s_Time_IBI_Rest.begin(), s_Time_IBI_Rest.end() );
		std::string h_Data_GSR_Normal	= std::string(s_Data_GSR_Normal.begin()	, s_Data_GSR_Normal.end());
		std::string h_Data_IBI_Normal	= std::string(s_Data_IBI_Normal.begin()	, s_Data_IBI_Normal.end());
	
	// to vectors

		std::vector<double> m_time_GSR_REST = tokenize_boost( h_Time_GSR_Rest, "," );
		std::vector<double> m_time_IBI_REST = tokenize_boost( h_Time_IBI_Rest, "," );
		std::vector<double> m_Data_GSR_REST = tokenize_boost( h_Data_GSR_Rest, "," );
		std::vector<double> m_Data_IBI_REST = tokenize_boost( h_Data_IBI_Rest, "," );
		
		int m_timeSegment = ::atoi( h_TimeSegmentTime.c_str() );

		std::vector<double> m_time_GSR_NORMAL	 = tokenize_boost( h_Time_GSR_Normal, "," );
		std::vector<double> m_time_IBI_NORMAL	 = tokenize_boost( h_Time_IBI_Normal, "," );
		std::vector<double> m_Data_GSR_NORMAL	 = tokenize_boost(h_Data_GSR_Normal, ",");
		std::vector<double> m_Data_IBI_NORMAL	 = tokenize_boost(h_Data_IBI_Normal, ",");
	
	// check REST values
	
	if ( ( m_time_GSR_REST.size() != m_Data_GSR_REST.size() ) ||
		 ( m_time_IBI_REST.size() != m_Data_IBI_REST.size() ) )
	{
		std::string reply = "REST Data are not the same size !" ;
		item[L"error"] = json::value::string(utility::conversions::to_string_t( reply ));
		JSONreply(item, message);
		return;
	}
	// check NORMAL values
	else if ( ( m_time_GSR_NORMAL.size() != m_Data_GSR_NORMAL.size() ) ||
			  ( m_time_IBI_NORMAL.size() != m_Data_IBI_NORMAL.size() ) )
	{
		std::string reply = "NORMAL Data are not the same size !" ;
		item[L"error"] = json::value::string(utility::conversions::to_string_t( reply ));
		JSONreply(item, message);
		return;
	}

	std::cout << "Processing REST data !" << std::endl;
	// to structs
	affect_sensing_module *afsm_REST = new affect_sensing_module(0);

	afsm_REST->set_time_GSR( m_time_GSR_REST );
	afsm_REST->set_time_IBI( m_time_IBI_REST );
	afsm_REST->set_data_GSR( m_Data_GSR_REST );
	afsm_REST->set_data_IBI( m_Data_IBI_REST );

	afsm_REST->calculate_GSR_Standard_Deviation();								// m_GSR_sd
	afsm_REST->calculate_GSR_1stDerivative_RootMeanSquare();					// m_GSR_1stDer_RMS
	afsm_REST->calculate_GSR_1stDerivative_Average();							// m_GSR_1stDer_AVG
	afsm_REST->calculate_GSR_SCR_Features();									// m_GSR_Amplitude, m_GSR_Rate
	afsm_REST->calculate_GSR_SCR_Percentile_Features();							// m_GSR_Amplitude_Percentiles
	afsm_REST->calculate_GSR_MIN_MAX_Features();								// m_GSR_min, m_GSR_max
	afsm_REST->calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion();	// m_GSR_Smooth_Dervative_RMS

	afsm_REST->calculate_EKG_Mean();			// m_EKG_mean
	afsm_REST->calculate_EKG_SD1_SD2();			// m_EKG_SD2
	afsm_REST->calculate_EKG_Picard_1();		// m_EKG_Picard_1
	afsm_REST->calculate_EKG_Picard_2();		// m_EKG_Picard_2
	afsm_REST->calculate_EKG_Picard_F2();		// m_EKG_Picard_F2
	afsm_REST->calculate_EKG_SD();				// m_EKG_SD
	afsm_REST->calculate_EKG_max();				// m_EKG_max
	afsm_REST->calculate_EKG_kurtosis();		// m_EKG_Kurtosis

	
	std::cout << "Processing NORMAL data !" << std::endl;
	affect_sensing_module *afsm_NORMAL = new affect_sensing_module (m_timeSegment);

	afsm_NORMAL->set_time_GSR( m_time_GSR_NORMAL );
	afsm_NORMAL->set_time_IBI( m_time_IBI_NORMAL );
	afsm_NORMAL->set_data_GSR( m_Data_GSR_NORMAL );
	afsm_NORMAL->set_data_IBI( m_Data_IBI_NORMAL );

	afsm_NORMAL->calculate_GSR_Standard_Deviation ();								// m_GSR_sd
	afsm_NORMAL->calculate_GSR_1stDerivative_RootMeanSquare ();						// m_GSR_1stDer_RMS
	afsm_NORMAL->calculate_GSR_1stDerivative_Average ();							// m_GSR_1stDer_AVG
	afsm_NORMAL->calculate_GSR_SCR_Features ();										// m_GSR_Amplitude, m_GSR_Rate
	afsm_NORMAL->calculate_GSR_SCR_Percentile_Features ();							// m_GSR_Amplitude_Percentiles
	afsm_NORMAL->calculate_GSR_MIN_MAX_Features ();									// m_GSR_min, m_GSR_max
	afsm_NORMAL->calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion ();	// m_GSR_Smooth_Dervative_RMS

	afsm_NORMAL->calculate_EKG_Mean ();			// m_EKG_mean
	afsm_NORMAL->calculate_EKG_SD1_SD2 ();		// m_EKG_SD2
	afsm_NORMAL->calculate_EKG_Picard_1 ();		// m_EKG_Picard_1
	afsm_NORMAL->calculate_EKG_Picard_2 ();		// m_EKG_Picard_2
	afsm_NORMAL->calculate_EKG_Picard_F2 ();	// m_EKG_Picard_F2
	afsm_NORMAL->calculate_EKG_SD ();			// m_EKG_SD
	afsm_NORMAL->calculate_EKG_max ();			// m_EKG_max
	afsm_NORMAL->calculate_EKG_kurtosis ();		// m_EKG_Kurtosis

	///
	
	std::cout << "Constructing results !" << std::endl;
	std::vector<int> results;

	for ( unsigned int i = 0 ; i<afsm_NORMAL->m_stress_results->m_num_of_segments ; i++ )
	{
		afsm_NORMAL->m_stress_results->m_GSR_sd[i]			/= afsm_REST->m_stress_results->m_GSR_sd[0];
		afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i]	/= afsm_REST->m_stress_results->m_GSR_1stDer_RMS[0];
		afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i]	/= afsm_REST->m_stress_results->m_GSR_1stDer_AVG[0];
		afsm_NORMAL->m_stress_results->m_GSR_Rate[i]		/= afsm_REST->m_stress_results->m_GSR_Rate[0];
		afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i]	/= afsm_REST->m_stress_results->m_GSR_Amplitude[0];

		for ( unsigned int j = 0 ; j < 5 ; j++ )
		{
			afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[j][i] /= afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[j][0];
		}

		afsm_NORMAL->m_stress_results->m_GSR_min[i]						/= afsm_REST->m_stress_results->m_GSR_min[0];
		afsm_NORMAL->m_stress_results->m_GSR_max[i]						/= afsm_REST->m_stress_results->m_GSR_max[0];
		afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i]	/= afsm_REST->m_stress_results->m_GSR_Smooth_Dervative_RMS[0];

		afsm_NORMAL->m_stress_results->m_EKG_mean[i]		/= afsm_REST->m_stress_results->m_EKG_mean[0];
		afsm_NORMAL->m_stress_results->m_EKG_SD[i]			/= afsm_REST->m_stress_results->m_EKG_SD[0];
		afsm_NORMAL->m_stress_results->m_EKG_SD2[i]			/= afsm_REST->m_stress_results->m_EKG_SD2[0];
		afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i]	/= afsm_REST->m_stress_results->m_EKG_Picard_1[0];
		afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i]	/= afsm_REST->m_stress_results->m_EKG_Picard_2[0];
		afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i]	/= afsm_REST->m_stress_results->m_EKG_Picard_F2[0];
		afsm_NORMAL->m_stress_results->m_EKG_max[i]			/= afsm_REST->m_stress_results->m_EKG_max[0];
		afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i]	/= afsm_REST->m_stress_results->m_EKG_Kurtosis[0];

		// Estimate stress level by weights

		Weights_struct *ww = new Weights_struct ();
		double sum = 0.;

		sum += ( ww->weights[0] * afsm_NORMAL->m_stress_results->m_GSR_sd[i] );							/*01*/
		sum += ( ww->weights[1] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i] );					/*02*/
		sum += ( ww->weights[2] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i] );					/*03*/
		sum += ( ww->weights[3] * afsm_NORMAL->m_stress_results->m_GSR_Rate[i] );						/*04*/
		sum += ( ww->weights[4] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i] );					/*05*/
		sum += ( ww->weights[5] * afsm_NORMAL->m_stress_results->m_GSR_min[i] );						/*06*/
		sum += ( ww->weights[6] * afsm_NORMAL->m_stress_results->m_GSR_max[i] );						/*07*/
		sum += ( ww->weights[7] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[2][i] );	/*08*/
		sum += ( ww->weights[8] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[3][i] );	/*09*/
		sum += ( ww->weights[9] * afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i] );		/*10*/

		sum += ( ww->weights[10] * afsm_NORMAL->m_stress_results->m_EKG_mean[i] );						/*11*/
		sum += ( ww->weights[11] * afsm_NORMAL->m_stress_results->m_EKG_SD[i] );						/*12*/
		sum += ( ww->weights[12] * afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i] );					/*13*/
		sum += ( ww->weights[13] * afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i] );					/*14*/
		sum += ( ww->weights[14] * afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i] );					/*15*/
		sum += ( ww->weights[15] * afsm_NORMAL->m_stress_results->m_EKG_max[i] );						/*16*/
		sum += ( ww->weights[16] * afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i] );					/*17*/
		sum += ( ww->weights[17] * afsm_NORMAL->m_stress_results->m_EKG_SD2[i] );						/*18*/

		delete ww;

		if ( sum > 3. )
		{
			results.push_back( 1 );
		}
		else
		{
			results.push_back( 0 ) ;
		}

	}
	
	std::cout << "Clearing memory !" << std::endl;
	delete afsm_REST;
	delete afsm_NORMAL;

	m_time_GSR_REST.clear();
	m_time_IBI_REST.clear();
	m_Data_GSR_REST.clear();
	m_Data_IBI_REST.clear();

	m_time_GSR_NORMAL.clear();
	m_time_IBI_NORMAL.clear();
	m_Data_GSR_NORMAL.clear();
	m_Data_IBI_NORMAL.clear();

	for ( unsigned int i = 0 ; i < results.size () ; i++ )
	{
		std::string key = "result_" + boost::lexical_cast<std::string>( i + 1 );;
		std::wstring w_key = std::wstring( key.begin(), key.end() );
		std::string value = boost::lexical_cast<std::string>( results[i] );
		std::wstring w_value = std::wstring( value.begin(), value.end() );

		item[w_key] = json::value::string( utility::conversions::to_string_t( w_value ) );
	}

	results.clear();

	std::cout << "Finished !" << std::endl;
	JSONreply(item, message);
}

void Affect_Sensing_Services::post_stress_data_to_DB( http_request &message )
{
	web::json::value JsonMessage = message.extract_json().get();

	json::value item;
	size_t size = JsonMessage.size();
	if ( JsonMessage.size() != 5 )
	{
		std::string reply = "Not enough values in JSON String !" ;
		item[L"error"] = json::value::string( utility::conversions::to_string_t( reply ) );
		JSONreply( item, message );
		return;
	}

	std::cout << "Acquiring JSON data !" << std::endl;

	utility::string_t s_Time_GSR_Rest = JsonMessage[L"Time_GSR_Rest"].as_string();
	utility::string_t s_Data_GSR_Rest = JsonMessage[L"Data_GSR_Rest"].as_string();
	utility::string_t s_Time_IBI_Rest = JsonMessage[L"Time_IBI_Rest"].as_string();
	utility::string_t s_Data_IBI_Rest = JsonMessage[L"Data_IBI_Rest"].as_string();
	utility::string_t s_Subject_ID = JsonMessage[L"Subject_ID"].as_string();

	std::string h_Time_GSR_Rest = std::string( s_Time_GSR_Rest.begin(), s_Time_GSR_Rest.end() );
	std::string h_Data_GSR_Rest = std::string( s_Data_GSR_Rest.begin(), s_Data_GSR_Rest.end() );
	std::string h_Time_IBI_Rest = std::string( s_Time_IBI_Rest.begin(), s_Time_IBI_Rest.end() );
	std::string h_Data_IBI_Rest = std::string( s_Data_IBI_Rest.begin(), s_Data_IBI_Rest.end() );

	std::string h_Subject_ID = std::string( s_Subject_ID.begin(), s_Subject_ID.end() );

	std::vector<double> m_time_GSR_REST = tokenize_boost( h_Time_GSR_Rest, "," );
	std::vector<double> m_Data_GSR_REST = tokenize_boost( h_Data_GSR_Rest, "," );
	std::vector<double> m_time_IBI_REST = tokenize_boost( h_Time_IBI_Rest, "," );
	std::vector<double> m_Data_IBI_REST = tokenize_boost( h_Data_IBI_Rest, "," );
	std::string m_Subject_ID = h_Subject_ID;

	//json::value item ;
	if ( m_time_GSR_REST.size() == 0 )
	{
		item[L"error"] = json::value::string( utility::conversions::to_string_t( "Not Enough GSR data !" ) );
		JSONreply( item, message );
		return;
	}
	if ( m_time_IBI_REST.size() == 0 )
	{
		item[L"error"] = json::value::string( utility::conversions::to_string_t( "Not Enough IBI data !" ) );
		JSONreply( item, message );
		return;
	}

	// normalize time to Zero
	double temp = m_time_GSR_REST[0];
	for ( unsigned int i = 0 ; i < m_time_GSR_REST.size() ; i++ )
	{
		m_time_GSR_REST[i] -= temp;
	}
	temp = m_time_IBI_REST[0];
	for ( unsigned int i = 0 ; i < m_time_IBI_REST.size() ; i++ )
	{
		m_time_IBI_REST[i] -= temp;
	}

	if ( ( m_time_GSR_REST.size() != m_Data_GSR_REST.size() ) ||
		( m_time_IBI_REST.size() != m_Data_IBI_REST.size() ) )
	{
		item[L"error"] = json::value::string( utility::conversions::to_string_t( "REST Data are not the same size !" ) );
		JSONreply( item, message );
		return;
	}

	std::cout << "Processing REST data !" << std::endl;
	affect_sensing_module *afsm_REST = new affect_sensing_module( 0 );

	afsm_REST->set_time_GSR( m_time_GSR_REST );
	afsm_REST->set_data_GSR( m_Data_GSR_REST );
	afsm_REST->set_time_IBI( m_time_IBI_REST );
	afsm_REST->set_data_IBI( m_Data_IBI_REST );

	afsm_REST->calculate_GSR_Standard_Deviation();								// m_GSR_sd
	afsm_REST->calculate_GSR_1stDerivative_RootMeanSquare();					// m_GSR_1stDer_RMS
	afsm_REST->calculate_GSR_1stDerivative_Average();							// m_GSR_1stDer_AVG
	afsm_REST->calculate_GSR_SCR_Features();									// m_GSR_Amplitude, m_GSR_Rate
	afsm_REST->calculate_GSR_SCR_Percentile_Features();							// m_GSR_Amplitude_Percentiles
	afsm_REST->calculate_GSR_MIN_MAX_Features();								// m_GSR_min, m_GSR_max
	afsm_REST->calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion();	// m_GSR_Smooth_Dervative_RMS

	afsm_REST->calculate_EKG_Mean();			// m_EKG_mean
	afsm_REST->calculate_EKG_SD1_SD2();			// m_EKG_SD2
	afsm_REST->calculate_EKG_Picard_1();		// m_EKG_Picard_1
	afsm_REST->calculate_EKG_Picard_2();		// m_EKG_Picard_2
	afsm_REST->calculate_EKG_Picard_F2();		// m_EKG_Picard_F2
	afsm_REST->calculate_EKG_SD();				// m_EKG_SD
	afsm_REST->calculate_EKG_max();				// m_EKG_max
	afsm_REST->calculate_EKG_kurtosis();		// m_EKG_Kurtosis

	std::string res = store_in_DB( *afsm_REST, m_Subject_ID );

	std::cout << "Clearing Memory !" << std::endl;
	// clear memory
	m_time_GSR_REST.clear();
	m_Data_GSR_REST.clear();
	m_time_IBI_REST.clear();
	m_Data_IBI_REST.clear();

	//delete afsm_REST;

	std::string result = "REST Data stored in Database by the name " + res ;
	item[L"info"] = json::value::string( utility::conversions::to_string_t( result ) );
	
	std::cout << "Finished !" << std::endl;
	JSONreply( item, message );

}

void Affect_Sensing_Services::detect_stress_from_DB( http_request &message )
{
	web::json::value JsonMessage = message.extract_json().get();

	json::value item;
	size_t size = JsonMessage.size();
	if ( JsonMessage.size() != 6 )
	{
		std::string reply = "Not enough values in JSON String !" ;
		item[L"error"] = json::value::string( utility::conversions::to_string_t( reply ) );
		JSONreply( item, message );
		return;
	}


	// from database ini

	std::cout << "Acquiring REST data from database !" << std::endl;

	utility::string_t s_subID = JsonMessage[L"Subject_ID"].as_string();
	std::string h_subID = std::string( s_subID.begin(), s_subID.end() );

	std::string filename = ".\\..\\DataBase\\" + h_subID + ".ini" ;

	boost::property_tree::ptree ptr;
	boost::property_tree::ini_parser::read_ini( filename, ptr );

	affect_sensing_module *afsm_REST = new affect_sensing_module(0);
	
	afsm_REST->m_stress_results->m_GSR_sd			.push_back( ptr.get<double>( "GSR.SD" ) );
	afsm_REST->m_stress_results->m_GSR_1stDer_AVG	.push_back( ptr.get<double>( "GSR.1stDer_AVG" ) );
	afsm_REST->m_stress_results->m_GSR_1stDer_RMS	.push_back( ptr.get<double>( "GSR.1stDer_RMS" ) );
	afsm_REST->m_stress_results->m_GSR_Rate			.push_back( ptr.get<double>( "GSR.Rate" ) );
	afsm_REST->m_stress_results->m_GSR_Amplitude	.push_back( ptr.get<double>( "GSR.Amplitude" ) );
	afsm_REST->m_stress_results->m_GSR_min			.push_back( ptr.get<double>( "GSR.min" ) );
	afsm_REST->m_stress_results->m_GSR_max			.push_back( ptr.get<double>( "GSR.max" ) );
	afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles.resize( 5 );
	afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[0].push_back( 0. );	//[0][i]
	afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[1].push_back( 0. );	//[1][i]
	afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[2].push_back( ptr.get<double>( "GSR.Amplitude_75" ) );
	afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[3].push_back( ptr.get<double>( "GSR.Amplitude_85" ) );
	afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[4].push_back( 0. );	//[4][i]
	afsm_REST->m_stress_results->m_GSR_Smooth_Dervative_RMS.push_back( ptr.get<double>( "GSR.Smooth_Dervative_RMS" ) );


	afsm_REST->m_stress_results->m_EKG_mean			.push_back( ptr.get<double>( "EKG.mean" ) );
	afsm_REST->m_stress_results->m_EKG_SD			.push_back( ptr.get<double>( "EKG.SD" ) );
	afsm_REST->m_stress_results->m_EKG_Picard_1		.push_back( ptr.get<double>( "EKG.Picard_1" ) );
	afsm_REST->m_stress_results->m_EKG_Picard_2		.push_back( ptr.get<double>( "EKG.Picard_2" ) );
	afsm_REST->m_stress_results->m_EKG_Picard_F2	.push_back( ptr.get<double>( "EKG.Picard_F2" ) );
	afsm_REST->m_stress_results->m_EKG_max			.push_back( ptr.get<double>( "EKG.max" ) );
	afsm_REST->m_stress_results->m_EKG_Kurtosis		.push_back( ptr.get<double>( "EKG.Kurtosis" ) );
	afsm_REST->m_stress_results->m_EKG_SD2			.push_back( ptr.get<double>( "EKG.SD2" ) );

	// from JSON
	std::cout << "Acquiring JSON data !" << std::endl;

	utility::string_t s_TimeSegmentTime = JsonMessage[L"Segment_Seconds"].as_string();

	utility::string_t s_Time_GSR_Normal = JsonMessage[L"Time_GSR_Normal"].as_string();
	utility::string_t s_Data_GSR_Normal = JsonMessage[L"Data_GSR_Normal"].as_string();
	utility::string_t s_Time_IBI_Normal = JsonMessage[L"Time_IBI_Normal"].as_string();
	utility::string_t s_Data_IBI_Normal = JsonMessage[L"Data_IBI_Normal"].as_string();

	// to string

	std::string h_TimeSegmentTime = std::string( s_TimeSegmentTime.begin(), s_TimeSegmentTime.end() );

	std::string h_Time_GSR_Normal = std::string( s_Time_GSR_Normal.begin(), s_Time_GSR_Normal.end() );
	std::string h_Time_IBI_Normal = std::string( s_Time_IBI_Normal.begin(), s_Time_IBI_Normal.end() );
	std::string h_Data_GSR_Normal = std::string( s_Data_GSR_Normal.begin(), s_Data_GSR_Normal.end() );
	std::string h_Data_IBI_Normal = std::string( s_Data_IBI_Normal.begin(), s_Data_IBI_Normal.end() );

	// to vectors

	int m_timeSegment = ::atoi( h_TimeSegmentTime.c_str() );
	
	std::vector<double> m_time_GSR_NORMAL = tokenize_boost( h_Time_GSR_Normal, "," );
	std::vector<double> m_time_IBI_NORMAL = tokenize_boost( h_Time_IBI_Normal, "," );
	std::vector<double> m_Data_GSR_NORMAL = tokenize_boost( h_Data_GSR_Normal, "," );
	std::vector<double> m_Data_IBI_NORMAL = tokenize_boost( h_Data_IBI_Normal, "," );


	std::cout << "Processing NORMAL data !" << std::endl;
	
	affect_sensing_module *afsm_NORMAL = new affect_sensing_module(m_timeSegment);

	afsm_NORMAL->set_time_GSR( m_time_GSR_NORMAL );
	afsm_NORMAL->set_data_GSR( m_Data_GSR_NORMAL );
	afsm_NORMAL->set_time_IBI( m_time_IBI_NORMAL );
	afsm_NORMAL->set_data_IBI( m_Data_IBI_NORMAL );

	afsm_NORMAL->calculate_GSR_Standard_Deviation();							// m_GSR_sd
	afsm_NORMAL->calculate_GSR_1stDerivative_RootMeanSquare();					// m_GSR_1stDer_RMS
	afsm_NORMAL->calculate_GSR_1stDerivative_Average();							// m_GSR_1stDer_AVG
	afsm_NORMAL->calculate_GSR_SCR_Features();									// m_GSR_Amplitude, m_GSR_Rate
	afsm_NORMAL->calculate_GSR_SCR_Percentile_Features();						// m_GSR_Amplitude_Percentiles
	afsm_NORMAL->calculate_GSR_MIN_MAX_Features();								// m_GSR_min, m_GSR_max
	afsm_NORMAL->calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion();// m_GSR_Smooth_Dervative_RMS

	afsm_NORMAL->calculate_EKG_Mean();			// m_EKG_mean
	afsm_NORMAL->calculate_EKG_SD1_SD2();		// m_EKG_SD2
	afsm_NORMAL->calculate_EKG_Picard_1();		// m_EKG_Picard_1
	afsm_NORMAL->calculate_EKG_Picard_2();		// m_EKG_Picard_2
	afsm_NORMAL->calculate_EKG_Picard_F2();		// m_EKG_Picard_F2
	afsm_NORMAL->calculate_EKG_SD();			// m_EKG_SD
	afsm_NORMAL->calculate_EKG_max();			// m_EKG_max
	afsm_NORMAL->calculate_EKG_kurtosis();		// m_EKG_Kurtosis

	std::cout << "Constructing results !" << std::endl;
	std::vector<int> results;

	for ( unsigned int i = 0 ; i<afsm_NORMAL->m_stress_results->m_num_of_segments ; i++ )
	{
		afsm_NORMAL->m_stress_results->m_GSR_sd[i] /= afsm_REST->m_stress_results->m_GSR_sd[0];
		afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i] /= afsm_REST->m_stress_results->m_GSR_1stDer_RMS[0];
		afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i] /= afsm_REST->m_stress_results->m_GSR_1stDer_AVG[0];
		afsm_NORMAL->m_stress_results->m_GSR_Rate[i] /= afsm_REST->m_stress_results->m_GSR_Rate[0];
		afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i] /= afsm_REST->m_stress_results->m_GSR_Amplitude[0];

		for ( unsigned int j = 0 ; j<5 ; j++ )
			afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[j][i] /= afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[j][0];

		afsm_NORMAL->m_stress_results->m_GSR_min[i] /= afsm_REST->m_stress_results->m_GSR_min[0];
		afsm_NORMAL->m_stress_results->m_GSR_max[i] /= afsm_REST->m_stress_results->m_GSR_max[0];
		afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i] /= afsm_REST->m_stress_results->m_GSR_Smooth_Dervative_RMS[0];

		afsm_NORMAL->m_stress_results->m_EKG_mean[i] /= afsm_REST->m_stress_results->m_EKG_mean[0];
		afsm_NORMAL->m_stress_results->m_EKG_SD[i] /= afsm_REST->m_stress_results->m_EKG_SD[0];
		afsm_NORMAL->m_stress_results->m_EKG_SD2[i] /= afsm_REST->m_stress_results->m_EKG_SD2[0];
		afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i] /= afsm_REST->m_stress_results->m_EKG_Picard_1[0];
		afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i] /= afsm_REST->m_stress_results->m_EKG_Picard_2[0];
		afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i] /= afsm_REST->m_stress_results->m_EKG_Picard_F2[0];
		afsm_NORMAL->m_stress_results->m_EKG_max[i] /= afsm_REST->m_stress_results->m_EKG_max[0];
		afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i] /= afsm_REST->m_stress_results->m_EKG_Kurtosis[0];

		// Estimate stress level by weights

		Weights_struct *ww = new Weights_struct();
		double sum = 0.;

		sum += ( ww->weights[0] * afsm_NORMAL->m_stress_results->m_GSR_sd[i] );							/*01*/
		sum += ( ww->weights[1] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i] );					/*02*/
		sum += ( ww->weights[2] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i] );					/*03*/
		sum += ( ww->weights[3] * afsm_NORMAL->m_stress_results->m_GSR_Rate[i] );						/*04*/
		sum += ( ww->weights[4] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i] );					/*05*/
		sum += ( ww->weights[5] * afsm_NORMAL->m_stress_results->m_GSR_min[i] );						/*06*/
		sum += ( ww->weights[6] * afsm_NORMAL->m_stress_results->m_GSR_max[i] );						/*07*/
		sum += ( ww->weights[7] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[2][i] );	/*08*/
		sum += ( ww->weights[8] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[3][i] );	/*09*/
		sum += ( ww->weights[9] * afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i] );		/*10*/

		sum += ( ww->weights[10] * afsm_NORMAL->m_stress_results->m_EKG_mean[i] );						/*11*/
		sum += ( ww->weights[11] * afsm_NORMAL->m_stress_results->m_EKG_SD[i] );						/*12*/
		sum += ( ww->weights[12] * afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i] );					/*13*/
		sum += ( ww->weights[13] * afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i] );					/*14*/
		sum += ( ww->weights[14] * afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i] );					/*15*/
		sum += ( ww->weights[15] * afsm_NORMAL->m_stress_results->m_EKG_max[i] );						/*16*/
		sum += ( ww->weights[16] * afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i] );					/*17*/
		sum += ( ww->weights[17] * afsm_NORMAL->m_stress_results->m_EKG_SD2[i] );						/*18*/

		delete ww;

		if ( sum > 3. )
		{
			results.push_back( 1 );
		}
		else
		{
			results.push_back( 0 ) ;
		}

	}

	// clear memory
	std::cout << "Clearing Memory !" << std::endl;
	
	m_time_GSR_NORMAL.clear();
	m_time_IBI_NORMAL.clear();
	m_Data_GSR_NORMAL.clear();
	m_Data_IBI_NORMAL.clear();

	delete afsm_REST;
	delete afsm_NORMAL;

	//json::value item;

	for ( unsigned int i = 0 ; i < results.size() ; i++ )
	{
		std::string key = "result_" + boost::lexical_cast<std::string>( i + 1 );;
		std::wstring w_key = std::wstring( key.begin(), key.end() );
		std::string value = boost::lexical_cast<std::string>( results[i] );
		std::wstring w_value = std::wstring( value.begin(), value.end() );

		item[w_key] = json::value::string( utility::conversions::to_string_t( w_value ) );;

	}

	results.clear();
	
	std::cout << "Finished !" << std::endl;

	JSONreply( item, message );
}