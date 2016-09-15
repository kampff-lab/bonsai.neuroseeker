// Nsk_C_DLL.cpp : C library of essential functions for controlling NeuroSeeker (Nsk) Probe

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>  

#include "NeuroseekerAPI.h"
#include "ElectrodePacket.h"
#include "ChannelConfiguration.h"
#include "GeneralConfiguration.h"
#include "NeuroseekerDataLinkFile.h"
#include "CSVParser.h"

// Global Classes
NeuroseekerAPI api;
ElectrodePacket ep;
NeuroseekerDataLinkIntf *DataLink;


// Global variables
unsigned int n_channels = 1440;
unsigned int n_samples;
float *data_matrix; // Channels * Samples
bool stream_recording;
bool testing;

// Namespace decalartion
using namespace std;

// "C" style used for function declarations
extern "C"
{
	// Open NeuroSeeker Probe
	__declspec(dllexport) void NSK_Open(bool LEDs)
	{
		// Error Code containers
		VersionNumber vn;
		OpenErrorCode oec;
		DigitalControlErrorCode dec;

		// Check Nsk API version
		std::cout << "Opening NeuroSeeker Probe: ";
		std::cout << "(Nsk API Version Number - ";
		vn = api.getAPIVersion();
		std::cout << vn.major << "." << vn.minor << ")\n";

		// Open connection to Nsk Probe
		std::cout << "Attempting to connect with Nsk Probe: ";
		oec = api.open();
		if (oec == 0)
		{
			std::cout << "- Nsk conneciton successful: EC " << oec << "\n";
		}
		else {
			std::cout << "- Nsk conneciton failed: EC " << oec << "\n";
		}

		// Start Log
		api.startLog();

		// Initialize Nsk Probe to default values
		oec = api.init();
		std::cout << "Initializing Nsk Probe: " << oec << "\n";

		// Enable or Disbale headstage LEDs
		if (LEDs)
		{
			std::cout << "Enabling headtsage LEDs: ";
		}
		else {
			std::cout << "Disabling headtsage LEDs: ";
		}
		dec = api.ledOff(!LEDs);
		std::cout << dec << "\n\n";
	}

	// Configure NeuroSeeker Probe
	__declspec(dllexport) void NSK_SetBiasVoltage(float biasVoltage)
	{
		// Error Corde containers
		DacControlErrorCode dcec;

		// Check that bias Voltage is in range
		if (biasVoltage < 0.0f) { biasVoltage = 0.0f; }
		if (biasVoltage > 2.5f) { biasVoltage = 2.5f; }
		std::cout << "Setting Bias voltage: ";
		dcec = api.generateDC(DAC_C, biasVoltage);
		std::cout << biasVoltage << "V " << dcec << "\n";
	}

	//Helper function to check if a channel is a reference or active one
	bool CheckIfChannelIsActive(int channel)
	{
		if ((channel >= 57 && channel <= 64) || (channel >= 177 && channel <= 184) || (channel >= 297 && channel <= 304) || (channel >= 417 && channel <= 424) ||
			(channel >= 537 && channel <= 544) || (channel >= 657 && channel <= 664) || (channel >= 777 && channel <= 784) || (channel >= 897 && channel <= 904) ||
			(channel >= 1017 && channel <= 1024) || (channel >= 1137 && channel <= 1144) || (channel >= 1257 && channel <= 1264) || (channel >= 1377 && channel <= 1384))
			return false;
		else
			return true;
	}

	// Configure NeuroSeeker Probe
	__declspec(dllexport) void NSK_Configure(int* activeRegions, bool testMode, float biasVoltage, char *OffsetCSV, char *SlopeCSV, char *CompCSV, char * ChannelsCSV)
	{
		// Error Corde containers
		ReadCsvErrorCode rcec;
		ShiftRegisterAccessErrorCode srac;
		GeneralConfigErrorCode gec;
		DacControlErrorCode dcec;
		ChannelConfigErrorCode ccec;

		// Configure Nsk probe
		std::cout << "Configuring NeuroSeekerrrs Probe\n";

		// Convert calibration CSV file paths to C++ strings
		const std::string off_str(OffsetCSV);
		const std::string slope_str(SlopeCSV);
		const std::string comp_str(CompCSV);
		const char* chanConf_str(ChannelsCSV);

		// Loading calibration parameters from CSVs
		std::cout << "Reading OFFSET calibration CSV file: ";
		rcec = api.readADCOffsetCalibrationFromCsv(off_str);
		std::cout << rcec << "\n";
		std::cout << "Reading SLOPE calibration CSV file: ";
		rcec = api.readADCSlopeCalibrationFromCsv(slope_str);
		std::cout << rcec << "\n";
		std::cout << "Reading COMPARATOR calibration CSV file: ";
		rcec = api.readComparatorCalibrationFromCsv(comp_str);
		std::cout << rcec << "\n";

		// Write Calibration paramters to general register
		std::cout << "Writing calibration parameters: ";
		srac = api.writeChannelRegisters(true);
		std::cout << srac << "\n\n";

		// Activate probe regions 1-12
		for (unsigned int i = 0; i < 12; i++)
		{
			std::cout << "Activating probe region: " << i;
			/*if (i == 0)
			{
				gec = gc.setBiasPixEnBit(i, true);
				std::cout << " " << gec << " True\n";
			}
			else
			{
				gec = gc.setBiasPixEnBit(i, false);
				std::cout << " " << gec << " False\n";
			}*/
			gec = api.generalConfiguration.setBiasPixEnBit(i, activeRegions[i]);
			std::string active = "False\n";
			if (activeRegions[i])
				active = "True\n";
			std::cout << " " << gec << " " << active;
		}
		std::cout << "Activating probe region: " << 12; // Activate 13th region (as is required)
		gec = api.generalConfiguration.setBiasPixEnBit(12, true);
		std::cout << " " << gec << " True\n";

		// Write settings to register
		std::cout << "Writing activation settings: ";
		srac = api.writeGeneralRegister(true);
		std::cout << srac << "\n";

		// Generate a Bias voltage for Probe
		std::cout << "Setting Bias voltage: ";
		// Check that bias Voltage is in range
		if (biasVoltage < 0.0f) { biasVoltage = 0.0f; }
		if (biasVoltage > 2.5f) { biasVoltage = 2.5f; }
		dcec = api.generateDC(DAC_C, biasVoltage);
		std::cout << biasVoltage << "V " << dcec << "\n";

		// Set Gain and Mode for every channel
		CSVParser CsvParser = CSVParser(chanConf_str); 

		std::cout << "Setting channel parameters (Gain, Mode, Ref): ";
		int c = 0;
		for (c = 0; c < n_channels; c++)
		{
			if (CheckIfChannelIsActive(c)) {
				ccec = api.allChannelConfigurations.setRefSel(c, RefSel(CsvParser.ChannelConfigReference[c]));
				if (ccec) { break; }
				ccec = api.allChannelConfigurations.setGain(c, CsvParser.ChannelConfigGain[c]);
				if (ccec) { break; }
				ccec = api.allChannelConfigurations.setMode(c, CsvParser.ChannelConfigMode[c]);
				if (ccec) { break; }
				ccec = api.allChannelConfigurations.setBw(c, CsvParser.ChannelConfigBW[c]);
				if (ccec) { break; }
			}
		}
		// Check/Report for exception (channel config error)
		if (ccec)
		{
			std::cout << "channel config failure - CH# " << c << ", Error: " << ccec << "\n";
		}
		else {
			std::cout << "channel config success.\n";
		}
		

		// Write settings to channel register
		std::cout << "Writing to channel settings: ";
		srac = api.writeChannelRegisters(true);
		std::cout << srac << "\n\n";

		// Enable Test Mode?
		if (testMode)
		{
			// Send a sinewave from Headstage DAC
			std::cout << "Enable Testing: ";
			gec = api.generalConfiguration.setTestInputEnBit(true);
			std::cout << gec << "\n";

			// Write settings to general register
			std::cout << "Writing to general register: ";
			srac = api.writeGeneralRegister(true);
			std::cout << srac << "\n";

			// Start genertating a sinewave on DAC_A
			dcec = api.generateSine(DAC_A, 5, 0.0f, 5);
			std::cout << "Gernating Sinewave Test Signal (1660 Hz): ";
			std::cout << dcec << "\n\n";
			testing = true;
		}

	}

	// Start NeuroSeeker Probe
	__declspec(dllexport) void NSK_Start(int buffer_size, bool stream)
	{
		// Error Code containers
		DigitalControlErrorCode dec;
		ErrorCode ec;

		// Starting Nsk probe
		std::cout << "Starting NeuroSeeker Probe\n";

		// Allocate memory for recording buffer (1440 floats per sample)
		n_samples = buffer_size;
		data_matrix = (float *)malloc(sizeof(float) * n_channels * n_samples);

		// Reset probe
		std::cout << "Resetting probe: ";
		dec = api.nrst(false);
		std::cout << dec << "\n";

		// Reset DataPath (FPGA)
		std::cout << "Resetting Datapath (FPGA): ";
		ec = api.resetDatapath();
		std::cout << ec << "\n";

		// Start probe
		std::cout << "Starting probe: ";
		dec = api.nrst(true);
		std::cout << dec << "\n";

		// Stream Recording (start?)
		stream_recording = stream;
		if (stream_recording)
		{
			std::cout << "Starting Recording Stream: ";
			ec = api.startRecording();
			std::cout << ec << "\n";
		}
	}

	// Read NeuroSeeker Raw Packets
	__declspec(dllexport) float* NSK_Read()
	{
		// Error Code containers
		ReadErrorCode rec;

		// Fill data matrix with channel data from N packets (all_samp_ch0 -> all_samp_ch 1...all_samp_chN)
		for (int i = 0; i < n_samples; i++)
		{
			// Read next packet (sample) from FIFO
			rec = api.readElectrodeData(ep, NULL);
			for (int c = 0; c < n_channels; c++)
			{
				data_matrix[(c * n_samples) + i] = ep.getChannelData(c);
			}
		}
		return data_matrix;

	}


	// Close NeuroSeeker Probe
	__declspec(dllexport) void NSK_Close()
	{
		// Error Code containers
		ErrorCode ec;
		GeneralConfigErrorCode gec;
		DacControlErrorCode dcec;
		ShiftRegisterAccessErrorCode srac;

		// Stream Recording (stop?)
		if (stream_recording)
		{
			std::cout << "Stopping the Recording Stream: ";
			ec = api.stopRecording();
			std::cout << ec << "\n";
			stream_recording = false;
		}

		// Disable Test mode (if testing)
		if (testing)
		{
			std::cout << "Stopping the Test Mode: ";
			dcec = api.generateDC(DAC_A, 0.0f);
			gec = api.generalConfiguration.setTestInputEnBit(false);
			srac = api.writeGeneralRegister(true);
			std::cout << dcec << " " << gec << " " << srac << "\n";
			testing = false;
		}

		// Stop Log
		api.stopLog();

		// Close connection to Nsk Probe
		std::cout << "Attempting to close Nsk Probe: ";
		api.close();
		std::cout << "Closed\n";

		// Free memory from data buffer
		free(data_matrix);
	}




	// Function Library for Loading NSK FIles
	// --------------------------------------
	// Open NeuroSeeker Data File
	__declspec(dllexport) void NSK_Open_File(char *filename, int buffer_size)
	{
		// Error Code containers
		ErrorCode ec;

		// Check Nsk API version
		std::cout << "Opening NeuroSeeker Data File: ";
		const std::string filename_str(filename);
		DataLink = new NeuroseekerDataLinkFile(filename_str);
		ec = api.datamode(true); // set ElectrodeMode
		std::cout << ec << "\n";
		n_samples = buffer_size;

		// Allocate memory for recording buffer (1440 floats per sample)
		n_samples = buffer_size;
		data_matrix = (float *)malloc(sizeof(float) * n_channels * n_samples);

	}

	// Read NeuroSeeker Data File
	__declspec(dllexport) float* NSK_Read_File()
	{
		// Error Code containers
		ReadErrorCode rec;
		unsigned int pos;

		// Fill data matrix with channel data from N packets (all_samp_ch0 -> all_samp_ch 1...all_samp_chN)
		for (int i = 0; i < n_samples; i++)
		{
			// Read next packet (sample) from FIFO
			rec = api.readElectrodeData(ep, DataLink);
			pos = ep.getCounter(0);
			for (int c = 0; c < n_channels; c++)
			{
				data_matrix[(c * n_samples) + i] = ep.getChannelData(c);
			}
		}
		std::cout << rec << " " << pos << "\n";
		return data_matrix;
	}

	// Close NeuroSeeker Data File
	__declspec(dllexport) void NSK_Close_File()
	{
		// Free memory from data buffer
		free(DataLink);
		free(data_matrix);
	}



}