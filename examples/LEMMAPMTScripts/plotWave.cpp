void waveplot() {

        // Open file
	TFile *file = TFile::Open("PMTData.root");
	TTree *tree = (TTree*)file->Get("tree");

	std::vector<double>* sampleTS = nullptr;
	std::vector<int>* signal = nullptr;
	double trig;
        
        // Get branches
	tree->SetBranchAddress("sampleTS", &sampleTS);
	tree->SetBranchAddress("signal", &signal);
        
        // Single entry now. Loop for profit
	tree->GetEntry(0);

      // These are variables from Monika
	const double offset = 8192.0; // offset sits in middle of 14-bit range 
	const double fullRange = 2000.0; // 2000mV = 2.0Vpp
	const double adcResolution = 16384.0; // 14-bit ADC - values range from 0 to 16383 (2^14 = 16384)

	TGraph *graph = new TGraph(sampleTS->size());
	// Loop the samples and plot
	for (int i = 0; i < (int)sampleTS->size(); i++) {
        //for (int i = 0; i < 15; i++) {
	
		double mV = ((*signal)[i] - offset) * (fullRange / adcResolution); //ADC counts to mV conversion
		graph->SetPoint(i, (*sampleTS)[i], mV);
		//cout << (*signal)[i] << endl;
	}
	graph->GetXaxis()->SetLimits(0.062, 0.064);
	graph->SetMaximum(10);
	graph->SetMinimum(-30);
	graph->SetLineColor(kBlue);
	graph->SetTitle("PMT pulse waveform - ...V;Time(s);Voltage(mV)");
	TCanvas *c = new TCanvas("c", "waveform", 800, 600);
	graph->Draw("AL");

	file->Close();

}
