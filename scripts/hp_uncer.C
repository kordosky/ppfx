#include <vector>

const int Nhel = 4;
const char* hel[Nhel]     = {"numu","numubar","nue","nuebar"};
const char* fhel[Nhel]    = {"#nu_{#mu}","#bar{#nu}_{#mu}","#nu_{e}","#bar{#nu}_{e}"};

const int Nuniv = 100;

const int Ncorr = 11;  // Initially it was 10

//const char* corr[Ncorr]  = {"pcpi","pck","ncpi","pcnu","pipinc","mes","nua","att","abs","oth","tot"};
const int corr_col[Ncorr] = {4,4,kOrange+1,kOrange+1,kRed,43,43,3,3,38,1};

const int corr_sty[Ncorr] = {1,2,1,2,1,1,2,1,2,1,1};


std::vector<TH1D*> vhist[Ncorr];

std::vector<TH1D*> getvhist(TFile* fIn, int icorr, const char* chel);
TH1D* get_cv_with_error_band(std::vector<TH1D*> vhistos);
TH1D* get_fract_error(TH1D* hin);
TH1D* get_rebinned_fract_error(TH1D* hin, int rebin_factor);
void plotting(int ihel);
void drawleg(TLegend* leg, TH1D* href, int id);

// Main function
void hp_uncer(const char* fnameIn) {

  std::cout << "starting() " << fnameIn << std::endl;
  TFile* fileIn = new TFile(fnameIn, "read");
  for(int i = 0; i < Nhel; i++) {
    std::cout << "plotting " << hel[i] << std::endl;

    for(int j = 0; j < Ncorr; j++) {
      vhist[j] = getvhist(fileIn, j, hel[i]);
    }
    plotting(i);

    for(int j = 0; j < Ncorr; j++) vhist[j].clear();
  }
}

// Define get_rebinned_fract_error function
TH1D* get_rebinned_fract_error(TH1D* hin, int rebin_factor) {

  // Clone and re-bin the histogram
  TH1D* hrebinned = (TH1D*)hin->Clone();
  hrebinned->Rebin(rebin_factor);

  // Calculate fractional error on the re-binned histogram
  int nbins = hrebinned->GetXaxis()->GetNbins();
  for(int jj = 1; jj <= nbins; jj++) {
    double cont = hrebinned->GetBinContent(jj);
    double err = hrebinned->GetBinError(jj);
    if(cont > 0) hrebinned->SetBinContent(jj, err / cont); // Fractional error calculation
  }

  return hrebinned;
}

// Example usage in plotting function
void plotting(int ihel) {

  TGaxis::SetMaxDigits(5);

  TH1D* herr[Ncorr];
  for(int kk = 0; kk < Ncorr; kk++) herr[kk] = get_cv_with_error_band(vhist[kk]);

  // Fractional error:
  TH1D* hfes[Ncorr];
  std::vector<TH1D*> vec_fe;
  int rebin_factor = 10; // Change to desired re-binning factor

  for(int kk = 0; kk < Ncorr; kk++) {
    hfes[kk] = get_rebinned_fract_error(herr[kk], rebin_factor); // Re-bin and get fractional error
    vec_fe.push_back(hfes[kk]);
  }

  // Visualization settings
  gROOT->SetStyle("Plain");
  gStyle->SetLegendBorderSize(0);

  double x1leg = 0.31;
  double deltax = 0.12;
  double sizex = 0.40;
  double y1leg = 0.65;
  double y1aleg = 0.75;
  double y2leg = 0.85;

  TCanvas* cfe = new TCanvas();
  TLatex* lat = new TLatex();
  lat->SetNDC();

  TLegend* leg1 = new TLegend(x1leg, y1leg, x1leg + sizex, y2leg);
  x1leg += deltax;
  TLegend* leg2 = new TLegend(x1leg, y1leg, x1leg + sizex, y2leg);
  x1leg += deltax;
  TLegend* aleg = new TLegend(x1leg, y1leg, x1leg + sizex, y2leg);
  x1leg += deltax;
  TLegend* leg3 = new TLegend(x1leg, y1leg, x1leg + sizex, y2leg);

  leg1->SetFillColor(0);
  leg2->SetFillColor(0);
  leg3->SetFillColor(0);
  aleg->SetFillColor(0);

  for(int kk = 0; kk < Ncorr; kk++) {
    hfes[kk]->SetLineColor(corr_col[kk]);
    hfes[kk]->SetLineStyle(corr_sty[kk]);
    hfes[kk]->SetLineWidth(3);
  }

  for(int kk = 0; kk < Ncorr; kk++) {
    if(kk == 0) {
      hfes[kk]->GetXaxis()->SetRangeUser(0, 12 - 0.0001);
      hfes[kk]->GetYaxis()->SetRangeUser(0, 0.25);
      hfes[kk]->GetXaxis()->SetTitle("Neutrino energy (GeV)");
      hfes[kk]->GetYaxis()->SetTitle("Fractional Uncertainties");
      hfes[kk]->Draw("hist");
    } else {
      hfes[kk]->Draw("histsame");
    }
  }

  drawleg(leg1, hfes[4], 4); drawleg(leg1, hfes[0], 0); drawleg(leg1, hfes[1], 1);
  drawleg(leg2, hfes[6], 6); drawleg(leg2, hfes[2], 2); drawleg(leg2, hfes[3], 3);
  drawleg(leg3, hfes[7], 7); drawleg(leg3, hfes[5], 5); drawleg(leg3, hfes[8], 8);
  drawleg(aleg, hfes[9], 9); drawleg(aleg, hfes[10], 10);

  double size_leg = 0.025;
  leg1->SetTextSize(size_leg);
  leg2->SetTextSize(size_leg);
  leg3->SetTextSize(size_leg);
  aleg->SetTextSize(size_leg);

  leg1->Draw();
  leg2->Draw();
  leg3->Draw();
  aleg->Draw();

  lat->DrawLatex(0.2, 0.86, "Hadron Production Uncertainties");
  lat->DrawLatex(0.78, 0.85, Form("#scale[1.2]{%s}", fhel[ihel]));

  std::cout << "=> " << Form("tot_fe_%s.png", hel[ihel]) << std::endl;
  cfe->SaveAs(Form("tot_fe_%s.png", hel[ihel]));
}
