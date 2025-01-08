void make_legend(TLegend *l, TF1 *f) {
	char s[50];
	sprintf(s, "#chi^{2}/ndf = %.0f/%d = %.2f", f->GetChisquare(), f->GetNDF(), f->GetChisquare()/f->GetNDF());
	l->AddEntry((TObject*)0, s, "");
	sprintf(s, "Prob = %.3f", f->GetProb());
	l->AddEntry((TObject*)0, s, "");
	for(int i = 0; i < f->GetNpar(); i++) {
		sprintf(s, "%s = %.3f #pm %.4f", f->GetParName(i), f->GetParameter(i), f->GetParError(i));
		l->AddEntry((TObject*)0, s, "");
	}
}

double calc_weighted_average(double N1, double x1, double N2, double x2) {
	return (N1*x1 + N2*x2) / (N1 + N2);
}

double calc_weighted_uncertainty(double N1, double x1, double N2, double x2) {
	double N = N1 + N2;
	double w1 = N1/N;
	double w2 = N2/N;

	return sqrt( w1*w1*x1*x1 + w2*w2*x2*x2 )/N;
}

void study_2d(string root_file = "h2.root", string hist_name = "reconstructed", vector<double> bins = {}, string xtitle = ""){
	char name[50];
	double  x, ex, peak, epeak, width, ewidth;
	
	TFile *inf = new TFile(root_file.c_str());
	TFile *opf_graphs = TFile::Open(("graphs_"+hist_name+".root").c_str(), "RECREATE");
	TFile *opf_fits = TFile::Open(("fits_"+hist_name+".root").c_str(), "RECREATE");
	TCanvas *c = new TCanvas();
	TH2F *hist = (TH2F*)inf->Get(hist_name.c_str());
	TGraphErrors *gr1 = new TGraphErrors(bins.size() - 1), *gr2 = new TGraphErrors(bins.size() - 1);
	TLatex t;
	
	//TF1 *f = new TF1("f", "gausn(0) + pol0(3)", 0.48, 0.52);
	//f->SetParameters(10, 0.49, 0.01, 1);
	TF1 *f = new TF1("f", "gausn(0) + gausn(3) + pol2(6)", 0.35, 0.65);
	f->SetParameters(10, 0.497, 0.01, 10, 0.497, 0.001, -1, 1, -1);
	
	TF1 *g1 = new TF1("g1", "gausn(0) + gausn(3)", 0.35, 0.65);
	TF1 *g2 = new TF1("g2", "gausn", 0.35, 0.65);
	TF1 *bk = new TF1("bk", "pol2",  0.35, 0.65);
	g1->SetLineColor(kBlue);
	g2->SetLineColor(kViolet);
	bk->SetLineColor(kBlack);
	
	for(int i = 0; i < bins.size() - 1; i++) {
		cout << "######################" << endl;
		cout << "######################" << endl;
		cout << "######################" << endl;
		cout << "Fit #" << i << endl;
		cout << "(" << bins[i] << "," << bins[i+1] << ") " << endl;
		
		TH1F *h = (TH1F*)hist->ProjectionX(("bin"+to_string(i)).c_str(),
		            hist->GetYaxis()->FindFixBin(bins[i]),
		            hist->GetYaxis()->FindFixBin(bins[i+1]));
		//sprintf(name, "bin (%.2f,%.2f)", bins[i], bins[i+1]);
		//h->SetTitle(name);
		//sprintf(name, "Counts / %.3f", h->GetBinWidth(1));
		h->Fit(f, "ER");
		
		sprintf(name, "bin (%.2f,%.2f)", bins[i], bins[i+1]);
		t.DrawLatex(0.37, 0.8*h->GetMaximum(), name);
		sprintf(name, "Counts / %.3f", h->GetBinWidth(1));
		h->GetYaxis()->SetTitle(name);
		h->GetYaxis()->SetRangeUser(0, 1.1*h->GetMaximum());
		
		g1->SetParameters(f->GetParameter(0), f->GetParameter(1), f->GetParameter(2), f->GetParameter(3), f->GetParameter(4), f->GetParameter(5));
		g1->Draw("SAME");
		
		bk->SetParameters(f->GetParameter(6), f->GetParameter(7), f->GetParameter(8));
		bk->Draw("SAME");
		
		TLegend *l = new TLegend(.6,.6,.9,.9);
		make_legend(l, f);
		l->Draw();
		
		c->Write(("bin"+to_string(i)).c_str());
		
		x = (bins[i+1] + bins[i])/2.;
		ex = (bins[i+1] - bins[i])/2.;
		//peak = f->GetParameter(1);
		peak = 1000*calc_weighted_average(f->GetParameter(0), f->GetParameter(1), f->GetParameter(3), f->GetParameter(4));
		epeak = calc_weighted_uncertainty(f->GetParameter(0), f->GetParameter(1), f->GetParameter(3), f->GetParameter(4));
		//epeak = f->GetParError(1);
		//width = 1000*f->GetParameter(2);
		width = 1000*calc_weighted_average(f->GetParameter(0), f->GetParameter(2), f->GetParameter(3), f->GetParameter(5));
		//ewidth = f->GetParError(2);
		ewidth = calc_weighted_uncertainty(f->GetParameter(0), f->GetParameter(2), f->GetParameter(3), f->GetParameter(5));
		
		gr1->SetPoint(i, x, peak);
		gr1->SetPointError(i, ex, epeak);
		gr2->SetPoint(i, x, width);
		gr2->SetPointError(i, ex, ewidth);
	}

	gr1->GetYaxis()->SetRangeUser(494, 506);
	gr1->SetTitle((";"+xtitle+";K_{S} Invariant Mass #mu (MeV)").c_str());

	gr2->SetTitle((";"+xtitle+";K_{S} Resolution #sigma (MeV)").c_str());
	gr2->GetYaxis()->SetRangeUser(0, 20);

	opf_graphs->cd();
	gr1->Write("mean");
	gr2->Write("width");
}

