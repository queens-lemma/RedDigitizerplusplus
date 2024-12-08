/*
 * Author: Hector Hawley Herrera
 *
 * This example can be used to test the API against any of the supported
 * digitizers. Simply pass a string of the model that wants to be tested.
 * */

#include "RedDigitizer++/red_digitizer_helper.hpp"

using namespace RedDigitizer;

int main(int argc, char* argv[]) {
    std::string model_str;
    if(argc == 1) {
        model_str = "V1740D";
    } else if (argc == 2) {
        model_str = argv[1];
    }

    std::shared_ptr<iostream_wrapper> logger = std::make_shared<iostream_wrapper>();
    RedDigitizer::CAENDigitizerModel model;
    try {
        model = CAENDigitizerModelsMap.at(model_str);
    } catch(std::out_of_range&) {
        logger->error("Input model is not supported. Defaulting to V1740D");
        model = RedDigitizer::CAENDigitizerModel::V1740D;
    }

    
    CAEN resource(logger,
                  model,                        // CAEN Model
                  CAENConnectionType::USB,      // Connection type
                  0,                            // Link number.
                  0,                            // Conet Node.
                  0);                           // VME Address

    if(resource.HasError() || not resource.IsConnected()) {
        logger->error("Failed to connect to the CAEN digitizer.");
        return -1;
    }

    // Setup
    CAENGlobalConfig global_config;
    std::array<CAENGroupConfig, 8> group_configs;
    resource.Setup(global_config, group_configs);

    // That's it for this demo!
    return 0;
}