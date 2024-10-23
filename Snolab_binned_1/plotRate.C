void getDensity(TString fileName, int color, int lineStyle, double exposureSec, vector<double> &exposure, vector<double> &exposureError, vector<double> &density, vector<double> &densityError, TString options=""){


    //Small differences in the fits may happen ROOT to ROOT

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2","Migrad");
    TF1* doubleGaus = new TF1("doubleGaus", "(1.0-[4])*[3]*(TMath::Gaus(x,[0],[1]*[2],1)) + [4]*[3]*(TMath::Gaus(x,[0]+[1],[1]*[2],1))",-0.5,1.5);
    doubleGaus->FixParameter(0,0.0);
    doubleGaus->FixParameter(1,1.0);
    doubleGaus->SetParameter(2, 0.2);
    doubleGaus->SetParLimits(4,0.0,1.0);
    doubleGaus->SetParName(0,"zero");
    doubleGaus->SetParName(1,"gain");
    doubleGaus->SetParName(2,"noise");
    doubleGaus->SetParName(3,"norm");
    doubleGaus->SetParName(4,"mu");

    //Open the file and fit the charge histogram to get the 1e density

    TFile* file = TFile::Open(fileName);

    TTree *tree = (TTree*)file->Get("calPixTree");
    int unMaskedPixels = tree->Draw("ePix>>histoUnmaskedTMP(400,-1.1,1.6)", "y>0&&y<=16&&!(mask&0x067d)", "goff");
    float nroImages = tree->GetEntries()/(3200*20);
    float allPixels = nroImages*3072*16;

    cout << "Unmasked Pixels: " << unMaskedPixels << " | All Pixels in active area: " << allPixels << endl;


    TH1F* histo = (TH1F*) gROOT->FindObject("histoUnmaskedTMP");
    histo->Fit(doubleGaus,"QL0","",-1.0,1.5);

    density.push_back(doubleGaus->GetParameter(4));
    densityError.push_back(doubleGaus->GetParError(4));

    // Change the binning and normalize only for visualization

    histo->SetTitle("");
    histo->SetLineColor(color);
    histo->SetLineStyle(lineStyle);
    histo->SetLineWidth(2);
    histo->GetXaxis()->SetTitle("Electrons");
    histo->GetYaxis()->SetTitle("Counts");

    histo->Rebin(4);
    histo->GetXaxis()->SetRangeUser(-0.95,1.55);
    histo->DrawNormalized(options);

    //Normalize fit
    doubleGaus->SetParameter(3, doubleGaus->GetParameter(3)/unMaskedPixels*4.0);
    doubleGaus->SetLineColor(color);
    doubleGaus->Draw("SAME");

    //Calculate exposure

    // Image read out time is 16m 5s = 965s
    // Row readout time is 965/20
    // Number of columns is 3200

    TString exposureFormula = Form("(%f+%f*(y + x/%f))>>histoExp", exposureSec, 965.0/20.0/3600/24, 3200.);
    tree->Draw(exposureFormula, "y>0&&y<=16&&!(mask&0x067d)", "goff");
    TH1F* histoExp = (TH1F*) gROOT->FindObject("histoExp");
    exposure.push_back(histoExp->GetMean());
    exposureError.push_back(histoExp->GetStdDev());


}


void plotRate(){

    vector<double> density;
    vector<double> densityError;
    vector<double> exp;
    vector<double> expError;

    TCanvas *canvas = new TCanvas("c1", "c1",10,59,800,1300);
    gStyle->SetOptStat(0);
    canvas->Divide(1,2);


    TPad *p1 = (TPad *)(canvas->cd(1));
    p1->SetLogy();


    cout << "****************** 0 HS ********************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP0_13.root", kGreen+2, 1, 0.0, exp, expError, density, densityError);
    canvas->Update();


    cout << "****************** 2 HS ********************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP7200_13.root", kRed+2, 9, 7200.0/3600.0/24.0, exp, expError, density, densityError, "SAME");
    canvas->Update();


    cout << "****************** 6 HS ********************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP21600_13.root", kBlue, 2, 21600.0/3600.0/24.0, exp, expError, density, densityError, "SAME");
    canvas->Update();


    cout << "****************** 20 HS *******************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP72000_13.root", kBlack, 3, 72000.0/3600.0/24.0, exp, expError, density, densityError, "SAME");
    canvas->Update();

    cout << "--------------------------------------------" << endl;
    canvas->cd(2);

    TGraphErrors *gr = new TGraphErrors(density.size(), &exp[0], &density[0], &expError[0], &densityError[0]);
    gr->SetMarkerStyle(20);
    gr->SetLineWidth(2);

    TF1 *pol1= new TF1("pol1","[0]+[1]*x",0,10000);
    pol1->SetParName(0,"exp-indep [e-/superpix]");
    pol1->SetParName(1,"exp-dep [e-/superpix/day]");

    gr->Draw("AP");
    gr->Fit("pol1", "Q");

    gr->GetXaxis()->SetTitle("Exposure / days");
    gr->GetYaxis()->SetTitle("1e density (electrons/superpix)");
    gr->SetTitle("");

    TString form;
    cout << Form("exp-indep: (%.2f +/- %.2f) 10^(-5) e-/superpix/image", pol1->GetParameter(0)*pow(10,5),  pol1->GetParError(0) *pow(10,5)) << endl;
    cout << Form("exp-dep:   (%.2f +/- %.2f) 10^(-4) e-/superpix/day", pol1->GetParameter(1)*pow(10,4),  pol1->GetParError(1) *pow(10,4)) << endl;
    cout << Form("1e- Rate:  (%.2f +/- %.2f) 10^(-5) e-/pix/day", pol1->GetParameter(1)/32*pow(10,5),  pol1->GetParError(1)/32 *pow(10,5)) << endl;
    cout << "--------------------------------------------" << endl;


}
