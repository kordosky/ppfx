//#include "NeutrinoEventChain.h"
//#include "CreateHists.h"

#include "CommonIMapIncludes.h"
#include "FillIMapHists.h"
#include <iostream>

using namespace std;

// declarations of functions found further down in this file
int CreateHists(const char* output_filename, const char* filename, int elow, int ehigh, int nu_id, bool NA49cuts, bool MIPPcuts);
void make_directories(TFile *f);
void name_hists(HistList * hists, TFile * out_file);
void scale_hists(HistList * hists);
void write_hists(HistList * hists, TFile * out_file);


int main( int argc, char *argv[])
{
  //! Default parameters
  std::vector<std::string> par;
  par.push_back("CreateHists.cpp");
  par.push_back("./rootfiles/testout.root");
  //par.push_back("/minerva/data/flux_hadron_samples/flux/g4numi/v4_test/le010z185i/*_0300_0001.root");
  //par.push_back("/minerva/data/flux_hadron_samples/flux/g4numi/v4_test/le010z185i/*_031*_0001.root");
  par.push_back("/minerva/data/flux_hadron_samples/flux/g4numi/v4_test/le010z185i/*.root");
  par.push_back("0");
  par.push_back("120");
  par.push_back("56");
  par.push_back("0");//NA49
  par.push_back("0");//MIPP

  //! Set user parameters
  for( int i=0; i<argc; ++i){
    if (string(argv[i]) != string("-1")){
      par.at(i) = argv[i];
    }
  }
  
  int min = atoi(par[3].c_str());
  int max = atoi(par[4].c_str());
  int nu_id = atoi(par[5].c_str());
  bool NA49cuts = atoi(par[6].c_str());
  bool MIPPcuts = atoi(par[7].c_str());
	
	//int CreateHists(const char* output_filename, const char* filename, int elow, int ehigh, int nu_id=56, bool NA49cuts=false, bool MIPPcuts=false))
  return CreateHists(par[1].c_str(), par[2].c_str(), min, max, nu_id, NA49cuts, MIPPcuts);
}


int CreateHists(const char* output_filename, const char* filename, int elow, int ehigh, int nu_id=56, bool NA49cuts=false, bool MIPPcuts=false)
{
	//single array histos	
	vector<TH2D *> hmat(9);
	vector<TH2D *> hvol(9);
	vector<TH2D *> hmatbkw(9);
	vector<TH2D *> hxfpt_tot(9);
	vector<TH1F *> henergytotal(9);
	vector<TH1F *> hkepop_tot(9);
	vector<TH1F *> htmpop_tot(9);

	//double array histos
	vector< vector<TH2D *> > hxfpt(9, vector<TH2D*>(9));;
	vector< vector<TH1F *> > henergymaterial(5, vector<TH1F*>(9));;
	vector< vector<TH1F *> > henergyvolume(193, vector<TH1F*>(9));;
	vector< vector<TH1F *> > hkepop(9, vector<TH1F*>(9));;
	vector< vector<TH1F *> > htmpop(9, vector<TH1F*>(9));;
  
	//individual histos
	TH2D * h_in_vs_mat = NULL;

	//collect histos in a list
	HistList hists = {hmat, hvol, hmatbkw, hxfpt_tot, henergytotal, hkepop_tot, htmpop_tot, 
					hxfpt, henergymaterial, henergyvolume, hkepop, htmpop, h_in_vs_mat};

	//make root file
	cout << "Making output file " << output_filename << endl;
  TFile *out_file= new TFile(output_filename,"RECREATE");
	
	//make directories in the output root file.	
	cout << "Making directories in " << output_filename << endl;
	make_directories(out_file);
	
	//name hists
	cout << "Naming Hists" << endl;
	name_hists(& hists, out_file);

	//Make an object of many neutrino ancestral lines	from the ntuples
	cout << "Opening flux ntuples" << endl;

	TChain* tdk2nu   = new TChain("dk2nuTree");  
	TChain* tdkmeta   = new TChain("dkmetaTree");  
	tdk2nu->Add(filename);
	tdkmeta->Add(filename);
	//Loop over the neutrinos and fill histos
	cout << "Filling histograms" << endl;	
	FillIMapHistsOpts opts;
	FillIMapHists(tdk2nu, tdkmeta, &hists, &opts);

	//Scale the histos
	cout << "Scaling Histos" << endl;
	scale_hists(& hists);
 
	//Write hists
 	cout << "Writing Histos" << endl;	
	write_hists(& hists, out_file);
    
  out_file->Close();
	
	return 0;

} //END INT MAP


//Function definitions for:
//make_directories
//name_hists
//scale_hists
//write_hists




static const char* matlist[5] = {"Iron","Aluminum","Carbon","Helium","Steel"};


// ----------------------------------------------
// Make directories in the output root file
// ----------------------------------------------
void make_directories(TFile *f){
//Projectile
//---particle_i
//------Energy_Materials
//------Energy_Volumes
//Produced
//---particle_i
  using namespace IMap;
    f->mkdir("Projectile");
    f->mkdir("Produced");

    for(int j=0;j<9;j++){
      f->cd("Projectile");
      // Projectile/{particle}
      gDirectory->mkdir(popparticle[j]);
      gDirectory->cd(popparticle[j]);
      // Projectile/{particle}/Energy_Materials
      gDirectory->mkdir("Energy_Materials");
      // Projectile/{particle}/Energy_Volumes
      gDirectory->mkdir("Energy_Volumes");
    }
    
		for(int j=0;j<9;j++){
      f->cd("Produced");
      // Produced/{particle}
      gDirectory->mkdir(popparticle[j]);
    }
}

// ----------------------------------------------
// Name all the histograms
// ----------------------------------------------
void name_hists(HistList * hists, TFile * out_file){
		
  char namefile[100];
  using namespace IMap;
    // ----------------------------------------------
    // Make Histos in Projectile/
    // ----------------------------------------------
    // list of histos in Projectile
    // hmat[9]
    // hvol[9]
    // henergytotal[9]
    // henergymaterial[5][9]
    // henergyvolume[193][9]
    
    
    // Will be in Projectile/h_in_vs_mat
    hists->_h_in_vs_mat = new TH2D("h_in_vs_mat",";material ; incident particle",50,0,50,50,0,50);
    
    for(int k=0;k<9;k++)
    {
        sprintf(namefile,"Projectile/%s",popparticle[k]);
        out_file->cd(namefile);
        
        sprintf(namefile,"hmats%s",popparticle[k]);
        // Projectile/{particle}/hmats{particle}
        hists->_hmat[k]=new TH2D(namefile,popparticle[k],50,0,50,50,0,50);
			
        sprintf(namefile,"hvols%s",popparticle[k]);
        // Projectile/{particle}/hvols{particle}
        hists->_hvol[k]=new TH2D(namefile,popparticle[k],50,0,50,50,0,50);
        
        
        sprintf(namefile,"henergy%s",popparticle[k]);
        // Projectile/{particle}/henergy{particle}
        hists->_henergytotal[k] = new TH1F(namefile,popparticle[k],1000,0,200);
        
        hists->_henergytotal[k]->GetXaxis()->SetTitle("Energy (GeV)");
        hists->_henergytotal[k]->GetYaxis()->SetTitle("Fraction of ancestry");
        
        sprintf(namefile,"Projectile/%s/Energy_Materials",popparticle[k]);
        out_file->cd(namefile);
        
        for(int i=0;i<5;i++)
        {
            sprintf(namefile,"henergy%s%s",popparticle[k],matlist[i]);
            // Projectile/{particle}/Energy_Materials/henergy{particle}{material}
            hists->_henergymaterial[i][k] = new TH1F(namefile,"",1000,0,200);
            sprintf(namefile,"%s in %s",popparticle[k],matlist[i]);
						hists->_henergymaterial[i][k]->SetTitle(namefile);
            hists->_henergymaterial[i][k]->GetXaxis()->SetTitle("Energy (GeV)");
            hists->_henergymaterial[i][k]->GetYaxis()->SetTitle("Event Counts");
	
				}
        
        sprintf(namefile,"Projectile/%s/Energy_Volumes",popparticle[k]);
        out_file->cd(namefile);
        
        for(int i=0;i<193;i++)
        {
            
            sprintf(namefile,"henergy%s%s",popparticle[k],vol_list[i]);
            // Projectile/{particle}/Energy_Volumes/henergy{particle}{volume}
            hists->_henergyvolume[i][k]=new TH1F(namefile,"",1000,0,200);
            sprintf(namefile,"%s in %s",popparticle[k],vol_list[i]);
						hists->_henergyvolume[i][k]->SetTitle(namefile);
            hists->_henergyvolume[i][k]->GetXaxis()->SetTitle("Energy (GeV)");
            hists->_henergyvolume[i][k]->GetYaxis()->SetTitle("Event Counts");
				}
    }
    
    
    // ----------------------------------------------
    // Make histos in Produced/
    // ----------------------------------------------
    // list of histos in Produced:
    // hmatbkw[9]
    // hkepop_tot[9]
    // ktmpop_tot[9]
    // hxfpt_tot[9]
    // hkepop[9][9]
    // htmpop[9][9]
    // hxfpt[9][9]
    for(int j=0;j<9;j++)
    {
        sprintf(namefile,"Produced/%s",popparticle[j]);
        out_file->cd(namefile);
        sprintf(namefile,"hmatbkg%s",popparticle[j]);
        // Produced/{particle}/hmatbkg{particle}
        hists->_hmatbkw[j]=new TH2D(namefile,popparticle[j],50,0,50,50,0,50);
        sprintf(namefile,"hke%s",popparticle[j]);
        // Produced/{particle}/hke{particle}
        hists->_hkepop_tot[j]=new TH1F(namefile,"",1000,0,200);
        sprintf(namefile,"htm%s",popparticle[j]);
        // Produced/{particle}/htm{particle}
        hists->_htmpop_tot[j]=new TH1F(namefile,"",1000,0,200);
        sprintf(namefile,"hxfpt%s",popparticle[j]);
        // Produced/{particle}/hxfpt{particle}
        hists->_hxfpt_tot[j]=new TH2D(namefile,"",500,-1,1,100,0,2);
        
        for(int k=0;k<9;k++)
        {
            sprintf(namefile,"hke%s_%s",popparticle[j],popparticle[k]);
            // Produced/{particle}/hke{particle}_{other particle}
            hists->_hkepop[j][k]=new TH1F(namefile,"",1000,0,200);
            sprintf(namefile,"htm%s_%s",popparticle[j],popparticle[k]);
            // Produced/{particle}/htm{particle}_{other particle}
            hists->_htmpop[j][k]=new TH1F(namefile,"",1000,0,200);
            sprintf(namefile,"hxfpt%s_%s",popparticle[j],popparticle[k]);
            // Produced/{particle}/hxfpt{particle}_{other particle}
            hists->_hxfpt[j][k]=new TH2D(namefile,"",500,-1,1,100,0,2);
        }
    }

}


// ----------------------------------------------
// Scale all histograms by the total weight
// ----------------------------------------------
void scale_hists(HistList * hists){
		
		double total_weight = 1.0;
    
    hists->_h_in_vs_mat->Scale(1./total_weight);
    // loop over all particle-specific histograms
    for(int j=0;j<9;j++)
    {
      hists->_hmat[j]->Scale(1./total_weight);
      hists->_hvol[j]->Scale(1./total_weight);
      hists->_henergytotal[j]->Scale(1./total_weight);
      hists->_hmatbkw[j]->Scale(1./total_weight);
      hists->_hkepop_tot[j]->Scale(1./total_weight);
      hists->_htmpop_tot[j]->Scale(1./total_weight);
      hists->_hxfpt_tot[j]->Scale(1./total_weight);
      for(int k=0;k<9;k++)
      {
        hists->_hkepop[j][k]->Scale(1./total_weight);
        hists->_htmpop[j][k]->Scale(1./total_weight);
        hists->_hxfpt[j][k]->Scale(1./total_weight);
      } // for(int k=0;k<9;k++)
      // Loop over materials
      for(int m=0;m<5;m++)
      {
        hists->_henergymaterial[m][j]->Scale(1./total_weight);
      } // for(int m=0;m<5;m++)
    } // for(int j=0;j<9;j++)

}

// ----------------------------------------------
// Write all histos to file
// ----------------------------------------------
void write_hists(HistList * hists, TFile * out_file){
    
  char namefile[100];
  using namespace IMap;
    for(int j=0;j<9;j++)
    {
        sprintf(namefile,"Projectile/%s",popparticle[j]);
        out_file->cd(namefile);
        hists->_hmat[j]->Write();
        hists->_hvol[j]->Write();
        hists->_henergytotal[j]->Write();
        
        gDirectory->cd("Energy_Materials");
        for(int m=0;m<5;m++)
        {
            
            hists->_henergymaterial[m][j]->Write();
            
        }
        gDirectory->cd("..");
        gDirectory->cd("Energy_Volumes");
        for(int m=0;m<193;m++)
        {
            hists->_henergyvolume[m][j]->Write();
        }
    }
    
    
    for(int j=0;j<9;j++)
    {
        sprintf(namefile,"Produced/%s",popparticle[j]);
        out_file->cd(namefile);
        hists->_hmatbkw[j]->Write();
        hists->_hkepop_tot[j]->Write();
        hists->_htmpop_tot[j]->Write();
        hists->_hxfpt_tot[j]->Write();
        
        for(int k=0;k<9;k++)
        {
            hists->_hkepop[j][k]->Write();
            hists->_htmpop[j][k]->Write();
            hists->_hxfpt[j][k]->Write();
        }
		}

  // Save the h_in_vs_mat histogram in Projectile/h_in_vs_mat
  out_file->cd("Projectile");
  hists->_h_in_vs_mat->Write();

}
