# P4ALL - Affect Sensing Web Services

The web services described here are correlated to the [Affect Sensing Module](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Module), a module that enables stress detection through features extracted by Emphatical Sensors via web services.

This Repository contains all the necessary files needed for installing and using the "server" and the "client" side of the affect sensing web service module.

## User Instructions

1) The following libraries were used to build and test the module.

[Boost 1.61.1](http://www.boost.org/)

[C++ Rest SDK (Casablanca)](https://casablanca.codeplex.com/)
The C++ REST SDK is a Microsoft project for cloud-based client-server communication in native code using a modern asynchronous C++ API design.

2) Create the following folder tree or modify the paths which lead to the corresponding files inside the project properties.

```
lib ->	boost_1_61_0(32)		->	include
									lib

		cpprest.x86.120.2.8.0	->	Debug
									Release
									Include ->  cpprest
												pplx
									
```

3) Run Microsoft Visual Studio 2013 or newer edition under administrative rights
   The project has been tested with Microsoft Visual Studio 2015 Community Edition.

4) In the "Server.cpp" file change [this](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Web-Services/blob/master/Affect_Sensing_Web_Service/Server.cpp#L76) line of code according to your pcs IP adress and this line for the [port](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Web-Services/blob/master/Affect_Sensing_Web_Service/Server.cpp#L74) you want to use.

5) Build and run the programm.
   The Server window should appear.

6) Every one of the three web services provided require a different JSON file in order to work.
   Templates of these JSON files exist inside the [JSON Template Files](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Web-Services/tree/master/JSON%20Files/JSON%20Files%20with%20Various%20Intervals) folder.

i) The first Web Service is a one-time process. The user provides the data and the timestamps for both the Rest and the Normal states and the server returns the stress results for every segment of time inside the user defined seconds between segments.
For the Detect Stress (http://160.40.50.209:8080/Affect_Sensing_Module/detect_stress) web service, the next JSON File must be created according to [this template](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Web-Services/blob/master/JSON%20Files/Templates/template_detect_stress.json):
   
```
{
	"Time_GSR_Rest":"",
	"Time_IBI_Rest":"",
	"Data_GSR_Rest":"",
	"Data_IBI_Rest":"",
	"Segment_Seconds":"",
	"Time_GSR_Normal":"",
	"Time_IBI_Normal":"",
	"Data_GSR_Normal":"",
	"Data_IBI_Normal":""
}
```

   In particular :
   - *Time_GSR_Rest* : the Timestamp for the GSR values in the Rest state
   - *Time_IBI_Rest* : the Timestamp for the IBI values in the Rest state
   - *Data_GSR_Rest* : the values from the GSR Rest recordings
   - *Data_IBI_Rest* : the values from the IBI Rest recordings
   - *Segment_Seconds* : the seconds between every interval for the Normal state
   - *Time_GSR_Normal* : the Timestamp for the GSR values in the Normal state
   - *Time_IBI_Normal* : the Timestamp for the IBI values in the Normal state
   - *Data_GSR_Normal* : the values from the GSR Normal recordings
   - *Data_IBI_Normal* : the values from the IBI Normal recordings
   

ii) The second Web Service is creating the a feature file with the provided Rest state data inside the server database for faster use in the future.
For the Post_stress_data_to_DB (http://160.40.50.209:8080/Affect_Sensing_Module/post_stress_data_to_DB) web service, the [template JSON File](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Web-Services/blob/master/JSON%20Files/Templates/template_post_stress_data_to_DB.json)  looks like this :
	
```
{
	"Time_GSR_Rest":"",
	"Time_IBI_Rest":"",
	"Data_GSR_Rest":"",
	"Data_IBI_Rest":"",
	"Subject_ID":""
}
```
In particular :
- *Time_GSR_Rest* : the Timestamp for the GSR values in the Rest state
- *Time_IBI_Rest* : the Timestamp for the IBI values in the Rest state
- *Data_GSR_Rest* : the values from the GSR Rest recordings
- *Data_IBI_Rest* : the values from the IBI Rest recordings
- *Subject_ID* : the name of the patient and the name of the file that will be created inside the server database.
   
iii) The third and last Web Service uses the stored feature file for the specific patients rest state recording, to detect stress for the Normal state recording provided.
For the Detect_stress_from_DB (http://160.40.50.209:8080/Affect_Sensing_Module/detect_stress_from_DB) web saerbice, the next JSON File must be created according to [this template](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Web-Services/blob/master/JSON%20Files/Templates/template_detect_stress_from_DB.json):

```
{
	"Subject_ID":"",
	"Segment_Seconds":"",
	"Time_GSR_Normal":"",
	"Time_IBI_Normal":"",
	"Data_GSR_Normal":"",
	"Data_IBI_Normal":""
}
```
In particular :
- *Subject_ID* : the name of the patient whom`s feature file is stored in the server database
- *Segment_Seconds* : the seconds between every interval for the Normal state
- *Time_GSR_Normal* : the Timestamp for the GSR values in the Normal state
- *Time_IBI_Normal* : the Timestamp for the IBI values in the Normal state
- *Data_GSR_Normal* : the values from the GSR Normal recordings
- *Data_IBI_Normal* : the values from the IBI Normal recordings
	
## Funding Acknowledgement

The research leading to these results has received funding from the European Union's Seventh Framework Programme (FP7) under grant agreement No.610510
