
#include <vector>
#include <string>

TH1D* getBand(std::vector<TH1D*> vhIn);
TH1D* getFractionalError(TH1D* hIn);

void nu_error(const char* fname, const char* nuhel){
  //nuhel: numu, numubar, nue, nuebar
  std::string fhel = "";
  if(std::string(nuhel)=="numu")         fhel = "#nu_{#mu}";
  else if(std::string(nuhel)=="numubar") fhel = "#bar{#nu}_{#mu}";
  else if(std::string(nuhel)=="nue")     fhel = "#nu_{e}";
  else if(std::string(nuhel)=="nuebar")  fhel = "#bar{#nu}_{e}";
  else exit (1);

  TFile* fIn = new TFile(Form("%s",fname),"read");
  TH1D* hnom = (TH1D*)fIn->Get(Form("nom/hnom_%s",nuhel));
  std::vector<TH1D*> vhuniv;
  
  TDirectory *udir = fIn->GetDirectory(nuhel);
  TIter next(udir->GetListOfKeys());
  TKey* key;
  while((key= (TKey*)next())){
    TClass* cl = gROOT->GetClass(key->GetClassName());
    if(!cl->InheritsFrom("TH1D"))continue;
    vhuniv.push_back((TH1D*)key->ReadObj());
  }

  TH1D* herror = getBand(vhuniv);
  TH1D* hcv  = herror->Clone();
  TH1D* hfe  = getFractionalError(herror);
  
  //Error band:
  TCanvas* cband = new TCanvas();
  TLegend* leg = new TLegend(0.60,0.70,0.90,0.90);

  hnom->SetLineWidth(2);
  hnom->SetLineColor(1);
  
  herror->SetFillColor(18);
  herror->SetFillStyle(1001);
  herror->SetLineColor(kRed);
  
  hcv->SetLineWidth(2);
  hcv->SetMarkerSize(0);
  hcv->SetLineColor(2);
  
  herror->GetXaxis()->SetRangeUser(0,10);
  herror->SetStats(0);
  herror->SetMarkerSize(0);
  herror->SetTitle(Form("Error band for %s",nuhel));
  herror->GetXaxis()->SetTitle("#nu energy (GeV)");
  
  //XPOT: write here the pot of the input flux file.
  herror->GetYaxis()->SetTitle(Form("%s/m^{2}/XPOT",fhel.c_str()));
  
  herror->Draw("e2");
  hcv->Draw("samehist");
  hnom->Draw("samehist");

  leg->AddEntry(hcv,"MIPP corrected flux","l");
  leg->AddEntry(hnom,"FTFP flux","l");
  leg->Draw();
  
  cband->SaveAs(Form("err_band_%s.png",nuhel));
  
  //Fractional error:
  TCanvas* cfe = new TCanvas();
  hfe->SetLineWidth(2);
  hfe->SetLineColor(1);
  hfe->SetTitle(Form("Fractional error for %s",nuhel));
  hfe->GetXaxis()->SetRangeUser(0,10);
  hfe->SetStats(0);
  hfe->GetXaxis()->SetTitle("#nu energy (GeV)");
  hfe->GetYaxis()->SetTitle("fractional error");
  hfe->Draw("hist");
  cfe->SaveAs(Form("fe_%s.png",nuhel));
    
}


TH1D* getBand(std::vector<TH1D*> vhIn){
  
  int Nuniv = vhIn.size();
  if(Nuniv==0)exit(1);  
  TH1D* hband = (TH1D*) vhIn[0]->Clone();
  int Nbins   = hband->GetXaxis()->GetNbins(); 
  
  for(int jj=1;jj<=Nbins;jj++){
    
    //mean:
    double mean = 0;
    for(int ii=0;ii<Nuniv;ii++){
      mean += vhIn[ii]->GetBinContent(jj);
    }
    mean /= double(Nuniv);
    
    //error:
    double err = 0.0;
    for(int ii=0;ii<Nuniv;ii++){
      err += pow(vhIn[ii]->GetBinContent(jj) - mean,2);
    }
    err /= double(Nuniv);
    err = sqrt(err);
    hband->SetBinContent(jj,mean);
    hband->SetBinError(jj,err);
  }
  return hband;
}

TH1D* getFractionalError(TH1D* hIn){
  
  TH1D* hfe = (TH1D*)hIn->Clone();
  
  int Nbins   = hfe->GetXaxis()->GetNbins(); 
  for(int jj=1;jj<=Nbins;jj++){
    double cont = hIn->GetBinContent(jj);
    double err = hIn->GetBinError(jj);
    if(cont>0)hfe->SetBinContent(jj,err/cont);  
  }
  return hfe;
}

