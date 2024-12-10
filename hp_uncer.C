
#include <vector>

const int Nhel = 4;
const char* hel[Nhel]     = {"numu","numubar","nue","nuebar"};
const char* fhel[Nhel]    = {"#nu_{#mu}","#bar{#nu}_{#mu}","#nu_{e}","#bar{#nu}_{e}"};

const int Nuniv = 100;

const int Ncorr = 7;  
const char* corr[Ncorr]  = {"thintarget","thintargetmeson","pionincident","mippnumi","attenuation","others","total"};
const char* fcorr[Ncorr] = {"ThinTargetAll","ThinTargetMesonIncident","ThinTargetpionincident","MIPPNuMI","TargetAttenuation","Other","total"};
const int corr_col[Ncorr] = {4,kOrange+1,43,3,kRed,38,1};

const int corr_sty[Ncorr] = {1,2,1,2,1,2,1};

const char* cleg[Ncorr]   = {"pC #rightarrow XYZ","meson inc.","Pion Incident","MIPP NuMI","target att.","others","total HP"};

std::vector<TH1D*> vhist[Ncorr];

std::vector<TH1D*> getvhist(TFile* fIn, int icorr, const char* chel);
TH1D* get_cv_with_error_band(std::vector<TH1D*> vhistos);
TH1D* get_fract_error(TH1D* hin);
void plotting(int ihel);
void drawleg(TLegend* leg, TH1D* href, int id);

void hp_uncer(const char* fnameIn){
  
  std::cout<<"starting() "<<fnameIn<<std::endl;
  TFile* fileIn  = new TFile(fnameIn,"read");
  for(int i=0;i<Nhel;i++){
    std::cout<<"plotting "<<hel[i]<<std::endl;
    
    for(int j=0;j<Ncorr;j++){
      vhist[j] = getvhist(fileIn,j,hel[i]);
    }    
    plotting(i);

    for(int j=0;j<Ncorr;j++)vhist[j].clear();   
  }
  
}
std::vector<TH1D*> getvhist(TFile* fIn, int icorr, const char* chel){

  std::vector<TH1D*> vtmp;
  TDirectory* udir = fIn->GetDirectory(Form("%s_%s",chel,corr[icorr]));
  std::cout<<"=> Reading: "<<Form("%s_%s",chel,corr[icorr])<<std::endl;
  TIter next(udir->GetListOfKeys());
  TKey* key;
  while((key= (TKey*)next())){
    TClass* cl = gROOT->GetClass(key->GetClassName());
    if(!cl->InheritsFrom("TH1D"))continue;
    vtmp.push_back((TH1D*)key->ReadObj());
  }
  return vtmp;
}

void plotting(int ihel){
 
  TGaxis::SetMaxDigits(5);
 
  TH1D* herr[Ncorr];
  for(int kk=0;kk<Ncorr;kk++)herr[kk] = get_cv_with_error_band(vhist[kk]);
  
  //fractional error:
  TH1D* hfes[Ncorr];
  std::vector<TH1D*> vec_fe;
  for(int kk=0;kk<Ncorr;kk++){
    hfes[kk] = get_fract_error(herr[kk]);
    vec_fe.push_back(hfes[kk]);
  }

  gROOT->SetStyle("Plain");
  gStyle->SetLegendBorderSize(0);
  
  double x1leg  = 0.50;   
  double deltax = 0.15;   
  double sizex  = 0.12; 
  double y1leg  = 0.65;
  double y1aleg = 0.59;
  double y2leg  = 0.84;
  
  TCanvas* cfe = new TCanvas();
  TLatex* lat = new TLatex();
  lat->SetNDC();
  
  TLegend* leg1 = new TLegend(x1leg,y1leg,x1leg+sizex,y2leg);
  
  x1leg += deltax;
  TLegend* leg2 = new TLegend(x1leg,y1leg,x1leg+sizex,y2leg);
  
  TLegend* aleg = new TLegend(x1leg,y1aleg,x1leg+sizex,y1leg);

  x1leg += deltax;
  TLegend* leg3 = new TLegend(x1leg,y1leg,x1leg+sizex,y2leg);
  
  leg1->SetFillColor(0);
  leg2->SetFillColor(0);
  leg3->SetFillColor(0);
  aleg->SetFillColor(0);
  
  for(int kk=0;kk<Ncorr;kk++){
    hfes[kk]->SetLineColor(corr_col[kk]);
    hfes[kk]->SetLineStyle(corr_sty[kk]);
    hfes[kk]->SetLineWidth(3);
  }
  
  for(int kk=0;kk<Ncorr;kk++){
    if(kk==0){
      hfes[kk]->GetXaxis()->SetRangeUser(0, 12-0.0001);
      hfes[kk]->GetYaxis()->SetRangeUser(0,0.30);
      hfes[kk]->GetXaxis()->SetTitle("Neutrino energy (GeV)");
      hfes[kk]->GetYaxis()->SetTitle("Fractional Uncertainties");
      hfes[kk]->Draw("hist");      
    }
    else hfes[kk]->Draw("histsame");
  }
  
  drawleg(leg1,hfes[4],4); drawleg(leg1,hfes[0],0); drawleg(leg1,hfes[1],1);
  drawleg(leg2,hfes[6],6); drawleg(leg2,hfes[2],2); drawleg(leg2,hfes[3],3);
  drawleg(leg3,hfes[5],5); 
  
  
  double size_leg = 0.035;
  leg1->SetTextSize(size_leg);
  leg2->SetTextSize(size_leg);
  
  leg3->SetTextSize(size_leg);
  aleg->SetTextSize(size_leg);
  
  leg1->Draw();  
  leg2->Draw();  
  leg3->Draw();  
  aleg->Draw(); 
  
  lat->DrawLatex(0.2,0.86,"Hadron Production Uncertainties");
  lat->DrawLatex(0.78,0.85,Form("#scale[1.2]{%s}",fhel[ihel]));
  
  std::cout<<"=> "<<Form("tot_fe_%s.png",hel[ihel])<<std::endl;
  cfe->SaveAs(Form("tot_fe_%s.png",hel[ihel])); 
  
}

TH1D* get_cv_with_error_band(std::vector<TH1D*> vhistos){
  int Nuniv = vhistos.size();
  if(Nuniv==0)exit(1);  
  TH1D* hband = (TH1D*)vhistos[0]->Clone();
  
    int Nbins   = hband->GetXaxis()->GetNbins(); 
    
    for(int jj=1;jj<=Nbins;jj++){
      
      //mean:
      double mean = 0;
      for(int ii=0;ii<Nuniv;ii++){
	mean += vhistos[ii]->GetBinContent(jj);
      }
      mean /= double(Nuniv);

      //error:
      double err = 0.0;
      for(int ii=0;ii<Nuniv;ii++){
	err += pow(vhistos[ii]->GetBinContent(jj) - mean,2);
      }
      err /= double(Nuniv);
      err = sqrt(err);
      hband->SetBinContent(jj,mean);
      hband->SetBinError(jj,err);
    }
    
    return hband;

  }

TH1D* get_fract_error(TH1D* hin){

    TH1D* hfe = (TH1D*)hin->Clone();

    int nbins   = hfe->GetXaxis()->GetNbins(); 
    for(int jj=1;jj<=nbins;jj++){
      
      double cont = hin->GetBinContent(jj);
      double err = hin->GetBinError(jj);
      if(cont>0)hfe->SetBinContent(jj,err/cont);
      
    }
    
    return hfe;
  }

void drawleg(TLegend* leg, TH1D* href, int id){
  leg->AddEntry(href,Form("%s",cleg[id]),"l");  
}

