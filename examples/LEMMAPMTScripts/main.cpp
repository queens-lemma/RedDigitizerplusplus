/*
 * Author: Hector Hawley Herrera
 *
 * This example can be used to test the API against any of the supported
 * digitizers. Simply pass a string of the model that wants to be tested.
 * */
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <chrono>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include "RedDigitizer++/red_digitizer_helper.hpp"
#include "JsonFileWatcher.hpp"

using namespace RedDigitizer;
// From Hector
int main(int argc, char* argv[]) {

	JsonFileWatcher _json_w("daq_config.json");
    	auto console = spdlog::stdout_color_mt("console");
    	auto logger = spdlog::get("console");
	
	std::shared_ptr<CAENWaveforms<uint16_t>> waveforms;
    
	{	
		CAEN resource(logger,
			  _json_w.getModel(),               // CAEN Model
			  _json_w.getConnectionType(),      // Connection type
			  _json_w.getLinkModel(),           // Link number.
			  0,                                // Conet Node.
			  _json_w.getVMEAddress());                           // VME Address

		if(resource.HasError() || not resource.IsConnected()) {
			logger->error("Failed to connect to the CAEN digitizer.");
			return -1;
		}

		// Setup
		CAENGlobalConfig global_config = _json_w.getGlobalConfig();
		std::array<CAENGroupConfig, 8> group_configs = _json_w.getGroupConfigs();
		resource.Setup(global_config, group_configs);

		if(resource.HasError()) {
		    logger->error("Error during setup.");
		    return -1;
		}

		resource.EnableAcquisition();
		// waveform now points to the internal waveform resource
		// This HAS to be called after EnableAcquisition
		waveforms = resource.GetWaveform(0);
		
//////////////////////////////////////////////////////////// Conor Bits
		// Create a TFile to write to

		TFile* file = new TFile("PMTData.root","RECREATE");
		TTree* tree = new TTree("tree","tree");
		double timeStamp;
		std::vector<double>* sampleTS = nullptr;
		std::vector<int>* signal = nullptr;
		double triggerTime;

		tree->Branch("sampleTS",&sampleTS); // Timestamp in the event
                tree->Branch("signal",&signal); // ADC value
                tree->Branch("triggerTime",&triggerTime); // Trig time

		
		// Force an acquisition
		//resource.SoftwareTrigger();

		// Set a measurement ttime
		auto start_time = std::chrono::steady_clock::now();
		int duration_seconds=5; // How long to acquire for

		double ADCrate=80*1000; // This is a guess for now - Check this

		while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-start_time).count() < duration_seconds){
			// Reads the data from the digitizers
			resource.RetrieveData();
			// This decodes and writes to internal buffers
			// Now waveform has the data as well!
			if (resource.GetNumberOfEvents() > 0) {
			    resource.DecodeEvents();

			    // Clears CAEN digitizer internal buffers
			    resource.ClearData();
			}

  		        auto waveform = waveforms->get(2); // THis defines with ADC channel you're reading from 0=0, 1=1 , etc.
			triggerTime=waveforms->getInfo().TriggerTimeTag;
			int counter=0;
			for(auto count : waveform) {
				signal->push_back(count);
				sampleTS->push_back(counter/ADCrate);
				counter++;
   			}


			tree->Fill();
			sampleTS->clear();
        	        signal->clear();



		}

		resource.DisableAcquisition();
    tree->Write();
    file->Close();
    }
    std::cout << "CAEN time tag: " << waveforms->getInfo().TriggerTimeTag << std::endl;
    std::cout << "Waveform size: " << waveforms->getTotalSize() << std::endl;
    // To get data from channel 0
    auto waveform = waveforms->getData();
    std:: cout <<  waveforms->getEnabledChannels()[waveforms->getEnabledChannels().size()-1];

    auto test = waveforms->getRecordLength();
    //for(auto count : waveform) {
    //    std::cout << count << ", "; 
   // }

	std::cout << std::endl;

    // That's it for this demo!
    return 0;
}
