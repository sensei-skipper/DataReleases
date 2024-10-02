void getDensity(TString fileName, int color, int lineStyle, vector<double> &density, vector<double> &densityError, TString options=""){

    TF1* doubleGaus = new TF1("doubleGaus", "(1.0-[4])*[3]*(TMath::Gaus(x,[0],[1]*[2],1)) + [4]*[3]*(TMath::Gaus(x,[0]+[1],[1]*[2],1))");
    doubleGaus->FixParameter(0,0.0);
    doubleGaus->FixParameter(1,1.0);
    doubleGaus->SetParameter(2, 0.2);
    doubleGaus->SetParLimits(4,0.0,1.0);
    doubleGaus->SetParName(0,"zero");
    doubleGaus->SetParName(1,"gain");
    doubleGaus->SetParName(2,"noise");
    doubleGaus->SetParName(3,"norm");
    doubleGaus->SetParName(4,"mu");


    TFile* file = TFile::Open(fileName);

    TTree *tree = (TTree*)file->Get("calPixTree");
    int maskedPixels = tree->Draw("ePix>>histo", "x<3072&&y<16&&!(mask&0x967d)", options);
    //tree->Draw("ePix>>histo", "!(mask&0x967d)", options);
    TH1F* histo = (TH1F*) gROOT->FindObject("histo");
    doubleGaus->SetLineColor(color);
    histo->Fit(doubleGaus,"QSL","",-0.5,1.5);//Fit(fitToy,"QL","",-1.0,2.0);
    histo->SetLineColor(color);
    histo->SetLineStyle(lineStyle);
    histo->SetLineWidth(2);
    histo->GetXaxis()->SetTitle("Electrons");
    histo->GetYaxis()->SetTitle("Counts");
    density.push_back(doubleGaus->GetParameter(4));
    densityError.push_back(doubleGaus->GetParError(4));

    int unmaskedPixels = tree->Draw("ePix>>histoTMP", "x<3072&&y<16","goff");
    cout << "Masked Pixels: " << maskedPixels << " | Unmasked Pixels: " << unmaskedPixels << endl;

}


void plotRate(){

    vector<double> density;
    vector<double> densityError;
    vector<double> exp;
    vector<double> expError;

    TCanvas *canvas = new TCanvas("c1", "c1",10,59,800,1500);
    gStyle->SetOptStat(0);
    canvas->Divide(1,2);


    TPad *p1 = (TPad *)(canvas->cd(1));
    p1->SetLogy();



    cout << "****************** 0 HS ********************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP0_13.root", kGreen+2, 1, density, densityError);
    exp.push_back(0.00501457);
    expError.push_back(0);
    canvas->Update();


    cout << "****************** 2 HS ********************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP7200_13.root", kRed+2, 9, density, densityError, "SAME");
    exp.push_back(0.0883767);
    expError.push_back(0);
    canvas->Update();


    cout << "****************** 6 HS ********************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP21600_13.root", kBlue, 2, density, densityError, "SAME");
    exp.push_back(0.255046);
    expError.push_back(0);
    canvas->Update();


    cout << "****************** 20 HS *******************" << endl;
    getDensity("./RELEASE_hits_blinded_EXP72000_13.root", kBlack, 3, density, densityError, "SAME");
    exp.push_back(0.838429);
    expError.push_back(0);
    canvas->Update();
    //legend->Draw();

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

    cout << "1e Rate: (" << pol1->GetParameter(1) / 32.0 << " +/- " << pol1->GetParError(1) / 32.0  << ") e-/pix/day" << endl;
    cout << "--------------------------------------------" << endl;


}
