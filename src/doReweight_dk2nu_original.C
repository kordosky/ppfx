#include "dk2nu/tree/dkmeta.h"
#include "dk2nu/tree/dk2nu.h"
#include "MakeReweight.h"
#include "NuWeight.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

#include "TH1D.h"

//PDG codes
#define PI0  111
#define PIP  211
#define PIM -211
#define K0L  130
#define K0S  310
#define KP   321
#define KM  -321
#define MUP -13
#define MUM  13

const bool USE_NUCLEUS = true;

//Some constants::

const int NbinsE   = 200;
const double emin  =   0.;
const double emax  = 20.;
const int Nnuhel   = 4;
const char* nuhel[Nnuhel] = {"numu","numubar","nue","nuebar"};
const char* nulabel[Nnuhel] = {"#nu_{#mu}","#bar#nu_{#mu}","#nu_{e}","#bar#nu_{e}"};
const int Nparent = 5;
const char* parent_name[Nparent] = {"", "pipm", "kpm", "k0l", "mu" };
const char* parent_label[Nparent] = {"", "#pi^{#pm}", "K^{#pm}", "K^{0}_{L}", "#mu^{#pm}" };

using namespace NeutrinoFluxReweight;

class nu_g4numi;
class Numi2Pdg;

int idx_hel(int pdgdcode);

/*!
 * Run the reweighting for a single file (inputFile) for
 * particular MIPP covariance matrix given in input.xml file. 
 */
void doReweight_dk2nu(const char* inputFile, const char* outputFile, const char* optionsFile, const char* cxxdet, const char* cyydet, const char* czzdet){ 
  
  TH1::SetDefaultSumw2();
  
  const char* thisDir = getenv("PPFX_DIR");
  int idet = -1;
  bool doing_precalculated_pos = false;
  
  NeutrinoFluxAuxiliar::NuWeight* nuweight;			  
  if( std::string(cyydet) == "none" && std::string(czzdet) == "none" ){
    idet = atoi(cxxdet);
    doing_precalculated_pos = true;
  }
  else{
    std::vector<double> vdet;
    double xxdet = atof(cxxdet); vdet.push_back(xxdet);
    double yydet = atof(cyydet); vdet.push_back(yydet);
    double zzdet = atof(czzdet); vdet.push_back(zzdet);
    nuweight = new NeutrinoFluxAuxiliar::NuWeight(vdet);
  }
  
  std::cout<< "Instance of MakeReweight()" <<std::endl;
  MakeReweight* makerew = MakeReweight::getInstance();
  makerew->SetOptions(optionsFile); 
  
  std::cout<<"Making an output file to store histograms"<<std::endl;
  TFile* fOut = new TFile(outputFile,"recreate");
  std::cout<<"File name: "<<fOut->GetName()<<std::endl;
  
  const int Nuniverses = makerew->GetNumberOfUniversesUsed();
  
  TH1D* hnom   [Nnuhel][Nparent];

  TH1D* hcv    [Nnuhel];
  TH1D* hthin  [Nnuhel][Nuniverses];
  TH1D* hmipp  [Nnuhel][Nuniverses];
  TH1D* hatt   [Nnuhel][Nuniverses];
  TH1D* hothers[Nnuhel][Nuniverses];
  TH1D* htotal [Nnuhel][Nuniverses];

  TH1D* hthin_pCpi  [Nnuhel][Nuniverses];
  TH1D* hthin_pCk   [Nnuhel][Nuniverses];
  TH1D* hthin_nCpi  [Nnuhel][Nuniverses];
  TH1D* hthin_pCnu  [Nnuhel][Nuniverses];
  TH1D* hthin_mesinc[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_projectile_pip[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_projectile_pim[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_projectile_Kp[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_projectile_Km[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_projectile_K0[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_daughter_pip[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_daughter_pim[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_daughter_Kp[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_daughter_Km[Nnuhel][Nuniverses];
  TH1D* hthin_mesinc_daughter_K0[Nnuhel][Nuniverses];
  TH1D* hthin_nua   [Nnuhel][Nuniverses];
  TH1D* hthin_nuAlFe[Nnuhel][Nuniverses];

  TH1D* hpCQEL [Nnuhel][Nuniverses];
  
  for(int ii=0;ii<Nnuhel;ii++){
    const TString xtitle = "E_{#nu} [GeV]";
    const TString ytitle = "##nu_{unoscillated} [m^{-2}]";
   

    for(int kk=0;kk<Nparent; kk++){
      if(kk == 0)
        hnom[ii][kk] = new TH1D(Form("hnom_%s",nuhel[ii]), Form("Uncorrected %s flux",nulabel[ii]), NbinsE,emin,emax);
      else
        hnom[ii][kk] = new TH1D(Form("hnom_%s_%s",nuhel[ii], parent_name[kk]), Form("Uncorrected flux of %s from %s decays",nulabel[ii], parent_label[kk]), NbinsE,emin,emax);
      hnom[ii][kk]->SetXTitle(xtitle);
      hnom[ii][kk]->SetYTitle(ytitle);
    }

    hcv[ii]  = new TH1D(Form("hcv_%s",nuhel[ii]), Form("Fully PPFX-corrected %s flux (central value)", nulabel[ii]),NbinsE,emin,emax);
    hcv[ii]->SetXTitle(xtitle);
    hcv[ii]->SetYTitle(ytitle);


    for(int jj=0;jj< Nuniverses; jj++){
      hthin[ii][jj]   = new TH1D(Form("hthin_%s_%d",  nuhel[ii],jj),Form("%s flux corrected based on thin target data, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin[ii][jj]->SetXTitle(xtitle);
      hthin[ii][jj]->SetYTitle(ytitle);

      hmipp[ii][jj]   = new TH1D(Form("hmipp_%s_%d",  nuhel[ii],jj),Form("%s flux corrected based on MIPP NuMI target data, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      hmipp[ii][jj]->SetXTitle(xtitle);
      hmipp[ii][jj]->SetYTitle(ytitle);

      hatt[ii][jj]    = new TH1D(Form("hatt_%s_%d",   nuhel[ii],jj),Form("%s flux corrected for attenuation in target, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      hatt[ii][jj]->SetXTitle(xtitle);
      hatt[ii][jj]->SetYTitle(ytitle);

      hothers[ii][jj] = new TH1D(Form("hothers_%s_%d",nuhel[ii],jj),Form("%s flux corrected for other effects, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      hothers[ii][jj]->SetXTitle(xtitle);
      hothers[ii][jj]->SetYTitle(ytitle);

      htotal[ii][jj]  = new TH1D(Form("htotal_%s_%d", nuhel[ii],jj),Form("%s flux corrected for all effects, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      htotal[ii][jj]->SetXTitle(xtitle);
      htotal[ii][jj]->SetYTitle(ytitle);


      hthin_pCpi[ii][jj]   = new TH1D(Form("hthin_pCpi_%s_%d",   nuhel[ii],jj),Form("%s flux corrected based on thin target p+C#rightarrow#pi+X data, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_pCpi[ii][jj]->SetXTitle(xtitle);
      hthin_pCpi[ii][jj]->SetYTitle(ytitle);

      hthin_pCk[ii][jj]    = new TH1D(Form("hthin_pCk_%s_%d",    nuhel[ii],jj),Form("%s flux corrected based on thin target p+C#rightarrowK+X data, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_pCk[ii][jj]->SetXTitle(xtitle);
      hthin_pCk[ii][jj]->SetYTitle(ytitle);

      hthin_nCpi[ii][jj]   = new TH1D(Form("hthin_nCpi_%s_%d",   nuhel[ii],jj),Form("%s flux corrected based on thin target n+C#rightarrow#pi+X data, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_nCpi[ii][jj]->SetXTitle(xtitle);
      hthin_nCpi[ii][jj]->SetYTitle(ytitle);

      hthin_pCnu[ii][jj]   = new TH1D(Form("hthin_pCnu_%s_%d",   nuhel[ii],jj),Form("%s flux corrected based on thin target p+C#rightarrowN+X data, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_pCnu[ii][jj]->SetXTitle(xtitle);
      hthin_pCnu[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc[ii][jj] = new TH1D(Form("hthin_mesinc_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on thin target data on meson interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_projectile_pip[ii][jj] = new TH1D(Form("hthin_mesinc_projectile_pip_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on #pi^{+} interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_projectile_pip[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_projectile_pip[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_projectile_pim[ii][jj] = new TH1D(Form("hthin_mesinc_projectile_pim_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on #pi^{-} interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_projectile_pim[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_projectile_pim[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_projectile_Kp[ii][jj] = new TH1D(Form("hthin_mesinc_projectile_Kp_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on K^{+} interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_projectile_Kp[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_projectile_Kp[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_projectile_Km[ii][jj] = new TH1D(Form("hthin_mesinc_projectile_Km_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on K^{-} interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_projectile_Km[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_projectile_Km[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_projectile_K0[ii][jj] = new TH1D(Form("hthin_mesinc_projectile_K0_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on K^{0}_{L,S} interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_projectile_K0[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_projectile_K0[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_daughter_pip[ii][jj] = new TH1D(Form("hthin_mesinc_daughter_pip_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on #pi^{+} production in meson interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_daughter_pip[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_daughter_pip[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_daughter_pim[ii][jj] = new TH1D(Form("hthin_mesinc_daughter_pim_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on #pi^{-} production in meson interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_daughter_pim[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_daughter_pim[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_daughter_Kp[ii][jj] = new TH1D(Form("hthin_mesinc_daughter_Kp_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on K^{+} production in meson interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_daughter_Kp[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_daughter_Kp[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_daughter_Km[ii][jj] = new TH1D(Form("hthin_mesinc_daughter_Km_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on K^{-} production in meson interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_daughter_Km[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_daughter_Km[ii][jj]->SetYTitle(ytitle);

      hthin_mesinc_daughter_K0[ii][jj] = new TH1D(Form("hthin_mesinc_daughter_K0_%s_%d", nuhel[ii],jj),Form("%s flux corrected based on sadly nonexistent thin target data on K^{0}_{L,S} production in meson interaction, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_mesinc_daughter_K0[ii][jj]->SetXTitle(xtitle);
      hthin_mesinc_daughter_K0[ii][jj]->SetYTitle(ytitle);

      hthin_nua[ii][jj]    = new TH1D(Form("hthin_nua_%s_%d",    nuhel[ii],jj),Form("%s flux corrected based on thin target data on N+detector material, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_nua[ii][jj]->SetXTitle(xtitle);
      hthin_nua[ii][jj]->SetYTitle(ytitle);
      
      hpCQEL[ii][jj]    = new TH1D(Form("hpCQEL_%s_%d",    nuhel[ii],jj),Form("%s flux affected by p+C QEL: N#rightarrowN+X at x_{F}>0.95 or x_{F} > p_{T}/(GeV/c) + 0.5, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hpCQEL[ii][jj]->SetXTitle(xtitle);
      hpCQEL[ii][jj]->SetYTitle(ytitle);
      
      hthin_nuAlFe[ii][jj]    = new TH1D(Form("hthin_nuAlFe_%s_%d",    nuhel[ii],jj),Form("%s flux affected by N+Al and N+Fe interactions, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin_nuAlFe[ii][jj]->SetXTitle(xtitle);
      hthin_nuAlFe[ii][jj]->SetYTitle(ytitle);
    }
  }
  
  //Loading ntuples:
  TChain* chain_evts   = new TChain("dk2nuTree");
  TChain* chain_meta   = new TChain("dkmetaTree");
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;
  bsim::DkMeta* dkmeta = new bsim::DkMeta;
  
  std::cout<<" Adding ntuple at: "<<inputFile<<std::endl;

  chain_evts->Add(inputFile);
  chain_evts->SetBranchAddress("dk2nu",&dk2nu);
  int nentries  = chain_evts->GetEntries();

  chain_meta->Add(inputFile);
  chain_meta->SetBranchAddress("dkmeta",&dkmeta);
  chain_meta->GetEntry(0); //all entries are the same
  
  std::string detname = "UserPosition";
  if(doing_precalculated_pos){
    detname = (dkmeta->location)[idet].name;
  }
  std::cout<<"=> Doing the analysis for: "<< detname <<std::endl;

  std::cout<<"#POT = "<<dkmeta->pots<<std::endl;
  TH1D hpot("hpot", "", 1, 0, 1);
  hpot.SetYTitle("#POT");
  hpot.SetBinContent(1, dkmeta->pots);

  std::vector<double> vwgt_mipp_pi;
  std::vector<double> vwgt_mipp_K;
  std::vector<double> vwgt_abs;
  std::vector<double> vwgt_att;
  std::vector<double> vwgt_ttpCpi;
  std::vector<double> vwgt_ttpCk;
  std::vector<double> vwgt_ttnCpi;
  std::vector<double> vwgt_ttpCnu;
  std::vector<double> vwgt_ttnua;
  std::vector<double> vwgt_pCQEL;
  std::vector<double> vwgt_ttnuAlFe;
  std::vector<double> vwgt_ttmesinc;
  std::vector<double> vwgt_ttmesinc_projectile_pip;
  std::vector<double> vwgt_ttmesinc_projectile_pim;
  std::vector<double> vwgt_ttmesinc_projectile_Kp;
  std::vector<double> vwgt_ttmesinc_projectile_Km;
  std::vector<double> vwgt_ttmesinc_projectile_K0;
  std::vector<double> vwgt_ttmesinc_daughter_pip;
  std::vector<double> vwgt_ttmesinc_daughter_pim;
  std::vector<double> vwgt_ttmesinc_daughter_Kp;
  std::vector<double> vwgt_ttmesinc_daughter_Km;
  std::vector<double> vwgt_ttmesinc_daughter_K0;
  std::vector<double> vwgt_oth;
 
  std::cout<<"N of entries: "<<nentries<<std::endl;
  double fluxWGT  = 0;
  double nuenergy = 0;

  for(int ii=0;ii<nentries;ii++) {
    if(ii%1000==0)std::cout<<ii/1000<<" k / "<<nentries/1000<<" k evts"<<std::endl;
    
    vwgt_mipp_pi.clear();
    vwgt_mipp_K.clear();
    vwgt_abs.clear();
    vwgt_att.clear();
    vwgt_ttpCpi.clear();
    vwgt_ttpCk.clear();
    vwgt_ttnCpi.clear();
    vwgt_ttpCnu.clear();
    vwgt_ttmesinc.clear();
    vwgt_ttmesinc_projectile_pip.clear();
    vwgt_ttmesinc_projectile_pim.clear();
    vwgt_ttmesinc_projectile_Kp.clear();
    vwgt_ttmesinc_projectile_Km.clear();
    vwgt_ttmesinc_projectile_K0.clear();
    vwgt_ttmesinc_daughter_pip.clear();
    vwgt_ttmesinc_daughter_pim.clear();
    vwgt_ttmesinc_daughter_Kp.clear();
    vwgt_ttmesinc_daughter_Km.clear();
    vwgt_ttmesinc_daughter_K0.clear();
    vwgt_ttnua.clear();
    vwgt_ttnuAlFe.clear();
    
    vwgt_pCQEL.clear();

    vwgt_oth.clear();
     
    chain_evts->GetEntry(ii);
    const int nuidx = idx_hel(dk2nu->decay.ntype);

    /**
     * Cut on parents
     */
    const int parent_id = dk2nu->decay.ptype;
    makerew->calculateWeights(dk2nu,dkmeta);
    if(doing_precalculated_pos){
      fluxWGT = ( (dk2nu->nuray)[idet].wgt )*(dk2nu->decay.nimpwt)/3.1416;
      nuenergy = (dk2nu->nuray)[idet].E;
    }
    else{
      nuweight->calculate_weight(dk2nu);
      fluxWGT  = (nuweight->wgt)*(dk2nu->decay.nimpwt)/3.1416;
      nuenergy = nuweight->enu;
    }
    
    if(nuidx<0){
      std::cout<<"=> Wrong neutrino file"<<std::endl;
    }
    switch(parent_id) {
      case PIP:
      case PIM:
        hnom[nuidx][1]->Fill(nuenergy,fluxWGT);
        break;
      case KP:
      case KM:
        hnom[nuidx][2]->Fill(nuenergy,fluxWGT);
        break;
      case K0L:
        hnom[nuidx][3]->Fill(nuenergy,fluxWGT);
        break;
      case MUP:
      case MUM:
        hnom[nuidx][4]->Fill(nuenergy,fluxWGT);
        break;
      default:
        std::cout<<"Unexpected neutrino parent id "<<parent_id<<std::endl;
    }
    hnom[nuidx][0]->Fill(nuenergy,fluxWGT); //sum of all parents
    hcv[nuidx]->Fill(nuenergy,fluxWGT*makerew->GetCVWeight());

    vwgt_mipp_pi  = makerew->GetWeights("MIPPNumiPionYields");
    vwgt_mipp_K   = makerew->GetWeights("MIPPNumiKaonYields"); 
    vwgt_abs      = makerew->GetWeights("TotalAbsorption");
    vwgt_att      = makerew->GetWeights("TargetAttenuation");
    vwgt_ttpCpi   = makerew->GetWeights("ThinTargetpCPion");
    vwgt_ttpCk    = makerew->GetWeights("ThinTargetpCKaon");
    vwgt_ttnCpi   = makerew->GetWeights("ThinTargetnCPion");
    vwgt_ttpCnu   = makerew->GetWeights("ThinTargetpCNucleon");
    vwgt_ttmesinc = makerew->GetWeights("ThinTargetMesonIncident");
    vwgt_ttmesinc_projectile_pip = makerew->GetWeights("ThinTargetMesonIncident_ProjectilePip");
    vwgt_ttmesinc_projectile_pim = makerew->GetWeights("ThinTargetMesonIncident_ProjectilePim");
    vwgt_ttmesinc_projectile_Kp = makerew->GetWeights("ThinTargetMesonIncident_ProjectileKp");
    vwgt_ttmesinc_projectile_Km = makerew->GetWeights("ThinTargetMesonIncident_ProjectileKm");
    vwgt_ttmesinc_projectile_K0 = makerew->GetWeights("ThinTargetMesonIncident_ProjectileK0");
    vwgt_ttmesinc_daughter_pip = makerew->GetWeights("ThinTargetMesonIncident_DaughterPip");
    vwgt_ttmesinc_daughter_pim = makerew->GetWeights("ThinTargetMesonIncident_DaughterPim");
    vwgt_ttmesinc_daughter_Kp = makerew->GetWeights("ThinTargetMesonIncident_DaughterKp");
    vwgt_ttmesinc_daughter_Km = makerew->GetWeights("ThinTargetMesonIncident_DaughterKm");
    vwgt_ttmesinc_daughter_K0 = makerew->GetWeights("ThinTargetMesonIncident_DaughterK0");
    vwgt_ttnua    = makerew->GetWeights("ThinTargetnucleonA");
    vwgt_ttnuAlFe = makerew->GetWeights("ThinTargetnucleonAlFe");
    vwgt_oth      = makerew->GetWeights("Other"); 
    
    vwgt_pCQEL  = makerew->GetWeights("ThinTargetpCQEL");

    
    for(int jj=0;jj<Nuniverses;jj++){
      const double wgt_thin = vwgt_ttpCpi[jj]*vwgt_ttpCk[jj]*vwgt_ttnCpi[jj]*vwgt_ttpCnu[jj]
        *vwgt_ttmesinc[jj]
/*        *vwgt_ttmesinc_projectile_pip[jj]
        *vwgt_ttmesinc_projectile_pim[jj]
        *vwgt_ttmesinc_projectile_Kp[jj]
        *vwgt_ttmesinc_projectile_Km[jj]
        *vwgt_ttmesinc_projectile_K0[jj] 
        *vwgt_ttmesinc_daughter_pip[jj]
        *vwgt_ttmesinc_daughter_pim[jj]
        *vwgt_ttmesinc_daughter_Kp[jj]
        *vwgt_ttmesinc_daughter_Km[jj]
        *vwgt_ttmesinc_daughter_K0[jj] */
        *vwgt_ttnua[jj]*vwgt_ttnuAlFe[jj];
      const double wgt_mipp = vwgt_mipp_pi[jj]*vwgt_mipp_K[jj];
      const double wgt_att = vwgt_att[jj]*vwgt_abs[jj];
      hthin  [nuidx][jj]->Fill(nuenergy, fluxWGT*wgt_thin);
      hmipp  [nuidx][jj]->Fill(nuenergy, fluxWGT*wgt_mipp);
      hatt   [nuidx][jj]->Fill(nuenergy, fluxWGT*wgt_att);
      hothers[nuidx][jj]->Fill(nuenergy, fluxWGT*vwgt_oth[jj]);
      htotal [nuidx][jj]->Fill(nuenergy, fluxWGT*wgt_thin*wgt_mipp*wgt_att*vwgt_oth[jj]);

      hthin_pCpi  [nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttpCpi[jj]);
      hthin_pCk   [nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttpCk[jj]);
      hthin_nCpi  [nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttnCpi[jj]);
      hthin_pCnu  [nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttpCnu[jj]);
      hthin_mesinc[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc[jj]);
      hthin_mesinc_projectile_pip[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_projectile_pip[jj]);
      hthin_mesinc_projectile_pim[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_projectile_pim[jj]);
      hthin_mesinc_projectile_Kp[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_projectile_Kp[jj]);
      hthin_mesinc_projectile_Km[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_projectile_Km[jj]);
      hthin_mesinc_projectile_K0[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_projectile_K0[jj]);
      hthin_mesinc_daughter_pip[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_daughter_pip[jj]);
      hthin_mesinc_daughter_pim[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_daughter_pim[jj]);
      hthin_mesinc_daughter_Kp[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_daughter_Kp[jj]);
      hthin_mesinc_daughter_Km[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_daughter_Km[jj]);
      hthin_mesinc_daughter_K0[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttmesinc_daughter_K0[jj]);
      hthin_nua   [nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttnua[jj]);
      hthin_nuAlFe[nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_ttnuAlFe[jj]);
      
      hpCQEL [nuidx][jj]->Fill(nuenergy, fluxWGT * vwgt_pCQEL[jj]);
    }
  }

  std::cout<<"storing general histos"<<std::endl;
  fOut->cd();
  
  fOut->mkdir("nom");
  fOut->mkdir("nom/parent");
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->mkdir(Form("%s_thintarget",  nuhel[ii]));
    fOut->mkdir(Form("%s_mippnumi",    nuhel[ii]));
    fOut->mkdir(Form("%s_attenuation", nuhel[ii]));
    fOut->mkdir(Form("%s_others",      nuhel[ii]));
    fOut->mkdir(Form("%s_total",       nuhel[ii]));

    fOut->mkdir(Form("%s_thintarget/pCpi",   nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/pCk",    nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/nCpi",   nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/pCnu",   nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_projectile_pip", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_projectile_pim", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_projectile_Kp", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_projectile_Km", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_projectile_K0", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_daughter_pip", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_daughter_pim", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_daughter_Kp", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_daughter_Km", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/mesinc_daughter_K0", nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/nua",    nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/pCfwd",  nuhel[ii]));
    fOut->mkdir(Form("%s_thintarget/nuAlFe", nuhel[ii]));
    
    fOut->mkdir(Form("%s_pCQEL",  nuhel[ii]));
  }
  
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->cd("nom");
    hcv[ii]->Write();
    hnom[ii][0]->Write();
    fOut->cd("nom/parent");
    for(int kk=1;kk<Nparent; kk++){
      hnom[ii][kk]->Write();
    }
    for(int jj=0;jj< Nuniverses; jj++){
      fOut->cd(Form("%s_thintarget" ,nuhel[ii]));  hthin[ii][jj]->Write();
      fOut->cd(Form("%s_mippnumi"   ,nuhel[ii]));  hmipp[ii][jj]->Write();
      fOut->cd(Form("%s_attenuation",nuhel[ii]));  hatt[ii][jj]->Write();
      fOut->cd(Form("%s_others"     ,nuhel[ii]));  hothers[ii][jj]->Write();
      fOut->cd(Form("%s_total"      ,nuhel[ii]));  htotal[ii][jj]->Write();

      fOut->cd(Form("%s_thintarget/pCpi"   , nuhel[ii])); hthin_pCpi  [ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/pCk"    , nuhel[ii])); hthin_pCk   [ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/nCpi"   , nuhel[ii])); hthin_nCpi  [ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/pCnu"   , nuhel[ii])); hthin_pCnu  [ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc" , nuhel[ii])); hthin_mesinc[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_projectile_pip" , nuhel[ii])); hthin_mesinc_projectile_pip[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_projectile_pim" , nuhel[ii])); hthin_mesinc_projectile_pim[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_projectile_Kp" , nuhel[ii])); hthin_mesinc_projectile_Kp[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_projectile_Km" , nuhel[ii])); hthin_mesinc_projectile_Km[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_projectile_K0" , nuhel[ii])); hthin_mesinc_projectile_K0[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_daughter_pip" , nuhel[ii])); hthin_mesinc_daughter_pip[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_daughter_pim" , nuhel[ii])); hthin_mesinc_daughter_pim[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_daughter_Kp" , nuhel[ii])); hthin_mesinc_daughter_Kp[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_daughter_Km" , nuhel[ii])); hthin_mesinc_daughter_Km[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/mesinc_daughter_K0" , nuhel[ii])); hthin_mesinc_daughter_K0[ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/nua"    , nuhel[ii])); hthin_nua   [ii][jj]->Write();
      fOut->cd(Form("%s_thintarget/nuAlFe" , nuhel[ii])); hthin_nuAlFe[ii][jj]->Write();
      
      fOut->cd(Form("%s_pCQEL"  , nuhel[ii])); hpCQEL [ii][jj]->Write();
    }
  }
  fOut->cd();
  hpot.Write();
  
  //Releasing memory:
  makerew->resetInstance();
  
  std::cout<<"End of run()"<<std::endl;
}
  
int idx_hel(int pdgcode){
  int idx = -1;
  if(pdgcode ==  14)idx = 0;
  if(pdgcode == -14)idx = 1;
  if(pdgcode ==  12)idx = 2;
  if(pdgcode == -12)idx = 3;
  return idx;
}
void usage(){
  std::cout<<"This script calculates the flux at one position in the NuMI beamline: "<<std::endl;
  std::cout<<"Using a precalculated detector positions:"<<std::endl;
  std::cout<<"  bin/doReweight_dk2nu [inputFile] [outputFile] [optionsFile] [idet]"<<std::endl;
  std::cout<<"Using a user input position:"<<std::endl;
  std::cout<<"  bin/doReweight_dk2nu [inputFile] [outputFile] [optionsFile] [xpos] [ypos] [zpos]"<<std::endl;
  std::cout<<"  "<<std::endl;
  std::cout<<"Inputs  "<<std::endl;
  std::cout<<"[inputFile] : g4numi ntuple in dk2nu/dkmeta format (v6 minerva branch is recommended)"<<std::endl;
  std::cout<<"[outputFile] : user definied output file name."<<std::endl;
  std::cout<<"[optionsFile] :xml file with the ppfx input parameters (look at ${PPFX_DIR}/script/input_default.xml)"<<std::endl;
  std::cout<<"[idet] : index of the precalculated detector (look at the location.name in the dkmeta tree of the g4numi ntuple)"<<std::endl;
  std::cout<<"[xpos], [ypos], [zpos] : position (cm) respect to the MC NuMI coordinate system to calculate the flux"<<std::endl;
  std::cout<<"  "<<std::endl;
}
//////////////

#ifndef __CINT__
int main(int argc, const char* argv[]){

  if(argc==5){
    doReweight_dk2nu(argv[1], argv[2], argv[3], argv[4], "none", "none");
  }
  else if(argc==7){
    doReweight_dk2nu(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
  }
  else{
    std::cout<<"Error: Invalid number of arguments ("<<(argc-1)<<")"<<std::endl;
    usage();
    exit (1);
    }
  
  return 0;
}
#endif
