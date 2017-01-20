#pragma once

#include <vector>
#include <string>

using namespace std;

struct txt_file
{
	string fileName;
	vector < double > time_GSR ;
	vector < double > time_IBI ;

	vector < string > valueNames ;
	
	vector < vector < double > > data ;
	vector < double > GSR_data ;
	vector < double > IBI_data ;

	string subName;
	int    trialNum;
	
	txt_file()
	{
		fileName = "";
	};

	~txt_file()
	{
		time_GSR.clear();
		time_IBI.clear();

		//valueNames.clear();
		//
		//for ( unsigned int i = 0 ; i < data.size() ; i++ )
		//	data[i].clear();
		//data.clear();

		GSR_data.clear();
		IBI_data.clear();

	};

};

struct results
{
	int m_interval_seconds ;
	int m_num_of_segments ;

	// Galvanic Skin Responses (GSR) Features

	vector<double> m_GSR_mean ;
	vector<double> m_GSR_sd ;
	vector<double> m_GSR_1stDer_RMS ;
	vector<double> m_GSR_1stDer_AVG ;
	vector<double> m_GSR_Rate ;
	vector<double> m_GSR_Amplitude ;
	vector<double> m_GSR_Duration ;
	vector<vector<double>> m_GSR_Amplitude_Percentiles ;
	vector<vector<double>> m_GSR_Duration_Percentiles ;

	// ä_norm(x) = (1/(N-1)) * SUM_[i=1,i=N-1] ( abs(Xi+1 - Xi) )
	// mean of the absolute values of the first differences of the normalized signals
	vector<double> m_GSR_Picard_1 ;

	vector<double> m_GSR_Picard_2 ;
	vector<double> m_GSR_Picard_3 ;

	// f_d(x) = (1/(N-1)) * (Sn - S1)
	// Mean of the convoluted with Hanning Window signal first differences
	vector<double> m_GSR_Picard_F2 ;

	vector<double> m_GSR_min ;
	vector<double> m_GSR_max ;
	vector<double> m_GSR_Kurtosis;
	vector<double> m_GSR_Skewness;
	vector<double> m_GSR_1stDer_NegSampProp ;
	vector<double> m_GSR_Smooth_Dervative_Avg ;
	vector<double> m_GSR_Smooth_Dervative_RMS ;
	vector<double> m_GSR_Smooth_Dervative_NegativeSamplesProportion ;
	vector<double> m_GSR_Aeria_Under_Avg ;

	// ElectroCardioGram (EKG) Inter Beat Intervals (IBI) Features

	vector<double> m_EKG_mean ;
	vector<double> m_EKG_SD ;
	vector<double> m_EKG_LF_HF_Avg ;
	vector<double> m_EKG_RMSSD ;
	vector<double> m_EKG_SD1 ;
	vector<double> m_EKG_SD2 ;
	vector<double> m_EKG_Picard_1 ;
	vector<double> m_EKG_Picard_2 ;
	vector<double> m_EKG_Picard_F2 ;
	vector<double> m_EKG_max ;
	vector<double> m_EKG_Kurtosis ;


	results()
	{
	}

	~results()
	{
		m_GSR_mean.clear() ;
		m_GSR_sd.clear() ;
		m_GSR_1stDer_RMS.clear() ;
		m_GSR_1stDer_AVG.clear() ;
		m_GSR_Rate.clear() ;
		m_GSR_Amplitude.clear() ;
		m_GSR_Duration.clear() ;

		for ( unsigned int i=0 ; i<m_GSR_Amplitude_Percentiles.size() ; i++ )
			m_GSR_Amplitude_Percentiles[i].clear() ;
		m_GSR_Amplitude_Percentiles.clear() ;

		for ( unsigned int i=0 ; i<m_GSR_Duration_Percentiles.size() ; i++ )
			m_GSR_Duration_Percentiles[i].clear() ;
		m_GSR_Duration_Percentiles.clear() ;

		m_GSR_Picard_1.clear() ;
		m_GSR_Picard_2.clear() ;
		m_GSR_Picard_3.clear() ;
		m_GSR_Picard_F2.clear() ;
		m_GSR_min.clear() ;
		m_GSR_max.clear() ;
		m_GSR_Kurtosis.clear() ;
		m_GSR_Skewness.clear() ;
		m_GSR_1stDer_NegSampProp.clear() ;
		m_GSR_Smooth_Dervative_Avg.clear() ;
		m_GSR_Smooth_Dervative_RMS.clear() ;
		m_GSR_Smooth_Dervative_NegativeSamplesProportion.clear() ;
		m_GSR_Aeria_Under_Avg.clear() ;


		m_EKG_mean.clear() ;
		m_EKG_SD.clear() ;
		m_EKG_LF_HF_Avg.clear();
		m_EKG_RMSSD.clear() ;
		m_EKG_SD1.clear() ;
		m_EKG_SD2.clear() ;
		m_EKG_Picard_1.clear() ;
		m_EKG_Picard_2.clear() ;
		m_EKG_Picard_F2.clear() ;
		m_EKG_max.clear() ;
		m_EKG_Kurtosis.clear() ;

	}

};

struct Weights_struct
{
	std::vector<double> weights;

	Weights_struct()
	{
		weights.push_back(-0.0111914);		// 1   GSR\\gsr2_GSR_SD_PerTrial
		weights.push_back(-0.000362708);	// 2   GSR\\gsr3_GSR_Deriv1_Avg_PerTrial
		weights.push_back(-0.00140547);		// 3   GSR\\gsr4_GSR_Deriv1_RMS_PerTrial
		weights.push_back(-0.0135678);		// 4   GSR\\gsr5_GSR_Rate
		weights.push_back(2.78315e-005);	// 5   GSR\\gsr6_GSR_SCR_Amplitude_Avg_PerTrial
		weights.push_back(0.150793);		// 6   GSR\\gsr23_GSR_Trial_Min
		weights.push_back(-0.0128821);		// 7   GSR\\gsr24_GSR_Trial_Max
		weights.push_back(-0.202056);		// 8   GSR\\gsr15_SCR_Amp_p75
		weights.push_back(0.294769);		// 9   GSR\\gsr16_SCR_Amp_p85
		weights.push_back(0.000537793);		// 10  GSR\\gsr29_smoothDeriv_RMS
		
		weights.push_back(0.826621);		// 11  IBI\\ecg1_IBI_Mean_PerTrial
		weights.push_back(0.262804);		// 12  IBI\\ecg2_IBI_SD_PerTrial
		weights.push_back(-0.176833);		// 13  IBI\\ecg6_IBI_picard1_Delta
		weights.push_back(0.0366696);		// 14  IBI\\ecg7_IBI_picard2_DeltaNorm
		weights.push_back(-0.112791);		// 15  IBI\\ecg9_IBI_picardF2
		weights.push_back(0.207486);		// 16  IBI\\ecg11_IBI_Trial_Max
		weights.push_back(0.0768245);		// 17  IBI\\ecg12_IBI_Kurtosis
		weights.push_back(-0.0490983);		// 18  IBI\\ecg15_IBI_Poincare_SD2
	}

	~Weights_struct()
	{
		weights.clear();
	}
};