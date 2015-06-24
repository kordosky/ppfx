
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <string>
#include <iostream> // for cout
#include <stdlib.h> // for atoi

using namespace std;

int draw(string file = "/minerva/app/users/kleykamp/interactions_study/flux3_4gev_1.root", string savefile = "/minerva/app/users/kleykamp/interactions_study/Plots/h_in_vs_mat.png", string hist_name = "Projectile/h_in_vs_mat", int dim = 2)
{
  // This makes it so that it doesn't show the user the picture before saving
  gROOT->SetBatch();
  
  TCanvas c;
  //c.SetCanvasSize(1400,800);
  c.SetCanvasSize(1050,600);
  
  // Open the file
  TFile f1(file.c_str());
  
  if (dim == 2)
  {
    TH2D * h = dynamic_cast<TH2D*>(f1.Get(hist_name.c_str()));
    if (h == NULL)
    {
      cout<<"The returned histogram was null. Please check that it exists"<<endl;
      cout<<hist_name<<endl;
      return 1;
    }
    // Log axis
    c.SetLogz(1);
    
    // FindFirstBinAbove(Double_t threshold = 0, Int_t axis = 1)
    int xstart = h->FindFirstBinAbove(0,1);
    int xend = h->FindLastBinAbove(0,1);
    h->GetXaxis()->SetRange(xstart, xend);
    h->GetYaxis()->SetRange(h->FindFirstBinAbove(0,2), h->FindLastBinAbove(0,2));
    
    // Sort bin labels alphabetically
    // See ftp://root.cern.ch/root/doc/ROOTUsersGuideHTML/ch03s13.html
    //h->LabelsOption("ah","X");
    //h->LabelsOption("a","Y");
    
    string options;
    if (xend - xstart > 15) options = string("colz");
    else options = string("colz text");
    h->Draw(options.c_str());
    
  }
  else if (dim == 1)
  {
    TH1F * h = dynamic_cast<TH1F*>(f1.Get(hist_name.c_str()));
    if (h == NULL)
    {
      cout<<"The returned histogram was null. Please check that it exists"<<endl;
      cout<<hist_name<<endl;
      return 1;
    }
    // Log axis
    c.SetLogy(1);
    
    // FindFirstBinAbove(Double_t threshold = 0, Int_t axis = 1)
    h->GetXaxis()->SetRange(h->FindFirstBinAbove(0,1), h->FindLastBinAbove(0,1));
    
    // alpha sort, see above
    //h->LabelsOption("a","X");
    
    h->Draw();
  }
  else
  {
    cout<<"Didn't understand a dimension of "<<dim<<endl;
    cout<<"Please use a dimension of 1 or 2"<<endl;
    return 1;
  }
  
  // Save the canvas
  c.Print(savefile.c_str());
  
  return 0;
}

int main( int argc, char *argv[])
{
  //! Default parameters
  std::vector<std::string> par;
  par.push_back("CreatePlots");
  par.push_back("/minerva/app/users/kleykamp/interactions_study/flux3_4gev_1.root");
  par.push_back("/minerva/app/users/kleykamp/interactions_study/Plots/h_in_vs_mat_3_4GeV.png");
  par.push_back("Projectile/h_in_vs_mat");
  par.push_back("2");

  //! Set user parameters
  for( int i=0; i<argc; ++i){
    if (string(argv[i]) != string("-1")){
      par.at(i) = argv[i];
    }
  }
  
  if (argc < 1) {
    return 0;
  }
  return draw(par[1], par[2], par[3], atoi(par[4].c_str()));
}
