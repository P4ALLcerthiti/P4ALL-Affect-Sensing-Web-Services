#pragma once

#include "affect_sensing_module_structures.h"

#include <vector>
#include <string>
#include <windows.h>

using namespace std;

class affect_sensing_module
{
public:
	affect_sensing_module( );
	affect_sensing_module( int secondsBetweenIntervals );
	affect_sensing_module( std::string fileName, int secondsBetweenIntervals );
	
	~affect_sensing_module();

	void	set_seconds_between_Intervals( int secs );
	void	set_fileName( std::string fileName_str );
	std::string	get_fileName();
	void	reset_fileName( std::string fileName );
	void	enable_print_txt();
	void	enable_print_features();

	// GSR
	void calculate_GSR_mean();
	void calculate_GSR_Standard_Deviation();
	void calculate_GSR_1stDerivative_Average();
	void calculate_GSR_1stDerivative_RootMeanSquare();
	void calculate_GSR_SCR_Features();
	void calculate_GSR_Picard_1();
	void calculate_GSR_Picard_2();
	void calculate_GSR_Picard_3();
	void calculate_GSR_Picard_F2();
	void calculate_GSR_SCR_Percentile_Features();
	void calculate_GSR_MIN_MAX_Features();
	void calculate_GSR_Kurtosis_Skewness();
	void calculate_GSR_1st_Derivative_negativeSamplesProportion();
	void calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion();
	void calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg();

	// IBI
	void calculate_EKG_LFdivHF_Avg();
	void calculate_EKG_RMSSD();
	void calculate_EKG_SD1_SD2();
	void calculate_EKG_Picard_F2();
	void calculate_EKG_Mean();
	void calculate_EKG_SD();
	void calculate_EKG_Picard_1();
	void calculate_EKG_Picard_2();
	void calculate_EKG_max();
	void calculate_EKG_kurtosis();

	void write_to_features_txt( std::string txt , std::string subName , int trialNum , std::vector<double> vec );

	void normalize_feature( std::string featureName );

	results *m_stress_results ;
	
	void set_data_GSR( std::vector<double> vec ) { m_file->GSR_data = vec; };
	void set_data_IBI( std::vector<double> vec ) { m_file->IBI_data = vec; };
	void set_time_GSR( std::vector<double> vec ) { m_file->time_GSR = vec; };
	void set_time_IBI( std::vector<double> vec ) { m_file->time_IBI = vec; };


private:
	
	txt_file *m_file;

	int m_seconds_between_segments;
	bool m_print_to_txt_enabled ;
	bool m_print_to_features_enabled ;

	int skinCond_ColNr ;
	int ekgIBI_ColNr ;
	int ekgHR_ColNr ;
	int ekgLF_ColNr ;
	int ekgHF_ColNr ;

	void			load_file( std::string fileName );
	void			load_file_2( std::string fileName );
	void			load_file_ITI_2010( std::string fileName );
	void			load_file_from_sensor( std::string fileName );
	void			load_file_from_sensor_2( std::string fileName );
	void			set_Column_Numbers();
	vector<int>		segmentTime_GSR( int seconds );
	vector<int>		segmentTime_IBI( int seconds );
	vector<string>	tokenize_path( std::string path, const char delimiter);
	vector<double>	tokenize_string( std::string str);
	string			loose_Extension( std::string str);
	string			replaceFilePath( std::string inPath , std::string toBeReplacesWith );
	vector<string>	getSubNameAndTrialNum( std::string inPath);
	void			Create_Directory_all( std::string in_file_name );
	
	// GSR
	double			calculate_GSR_mean( int start, int stop );
	double			calculate_GSR_Standard_Deviation( int start, int stop );
	double			calculate_GSR_1stDerivative_RootMeanSquare( int start, int stop );
	double			calculate_GSR_1stDerivative_Average( int start, int stop );
	vector<double>  calculate_GSR_SCR_Features( int start, int stop );
	double			calculate_GSR_Picard_1( int start , int stop );
	double			calculate_GSR_Picard_2( int start , int stop );
	double			calculate_GSR_Picard_3( int start , int stop );
	double			calculate_GSR_Picard_F2( int start , int stop );
	vector<double>	calculate_GSR_SCR_Percentile_Features( int start , int stop );
	vector<double>	calculate_GSR_MIN_MAX_Features( int start , int stop );
	vector<double>	calculate_GSR_Kurtosis_Skewness( int start , int stop );
	double			calculate_GSR_1st_Derivative_negativeSamplesProportion( int start , int stop );
	vector<double>	calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion( int start , int stop );
	double			calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg( int start , int stop );
	
	// IBI
	double			calculate_EKG_Mean( int start, int stop );
	double			calculate_EKG_SD( int start, int stop );
	double			calculate_EKG_Picard_1( int start, int stop );
	double			calculate_EKG_Picard_2( int start, int stop );
	double			calculate_EKG_max( int start, int stop );
	double			calculate_EKG_kurtosis( int start, int stop );
	double			calculate_EKG_LFdivHF_Avg( int start , int stop );
	double			calculate_EKG_RMSSD( int start , int stop );
	vector<double>	calculate_EKG_SD1_SD2( int start , int stop );
	double			calculate_EKG_Picard_F2( int start , int stop );

	// SUPPORT
	vector<double>	convolute  ( vector<vector<double>> signal, int sigColNum , vector<double> windowSignal );
	vector<double>	convolute_2( vector<vector<double>> signal, int sigColNum , vector<double> windowSignal );
	double			getMaxGSRValueForSpecificStartandStop( double start , double stop );


};