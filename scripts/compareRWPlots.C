#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdio.h>

using std::cout;
using std::string;
using std::vector;


// ---------- DECLARATIONS ---------- //

// convert string "file1 file2 ..." to vector of strings and return its size
UInt_t makeList (const string &input, vector <string> &listFiles);

// open and check input files
bool openFiles (TFile **inputFiles, vector <string> &listFiles);

// get the same histogram from several files
void getHistograms (TFile **inputFiles, TH1D **histograms, const UInt_t &nFiles,
                    const char *histoDir, const char *histoName);
// draw several histograms at one plot
void drawAtOnePlot (TH1D **histograms, const UInt_t &nHistograms,
                    const char *histoName);

// ---------- MAIN FUNCTION ---------- //

// create plot with the same histograms from several files
// input = "file1 file2 file3 ..."
void compareRWPlots (const string input, const UInt_t firstUniverse = 0,
                     const UInt_t lastUniverse = 99)
{
  vector <string> listFiles;                         // list of input files
  const UInt_t nFiles = makeList (input, listFiles); // nof of input files

  const TFile **inputFiles = new TFile*[nFiles];     // array of input TFiles

  // read input files; stop script if could not open files
  if (!openFiles (inputFiles, listFiles)) return;

  gROOT->SetBatch();        // go to batch mode (lots of plots will be created)
  gErrorIgnoreLevel = 2000; // do not print info messages (like eps was created)

  // storage for histograms from different input files
  TH1D **histograms = new TH1D*[nFiles];

  // get nominal beam for each file and draw at one plot
  getHistograms (inputFiles, histograms, nFiles, "nom", "hnom_numu");
  drawAtOnePlot (histograms, nFiles, "numu_nominal");
  // get corrected beam for each file and draw at one plot
  getHistograms (inputFiles, histograms, nFiles, "nom_corr", "hnom_corr_numu");
  drawAtOnePlot (histograms, nFiles, "numu_corrected");

  // for each universe get nominal beam and draw at one plot
  for (UInt_t i = firstUniverse; i <= lastUniverse; i++)
  {
    getHistograms (inputFiles, histograms, nFiles, "numu",
                   Form ("h_rw_numu_%i", i));
    drawAtOnePlot (histograms, nFiles, Form ("numu_nominal_universe_%i", i));
  }
  // for each universe get corrected beam and draw at one plot
  for (UInt_t i = firstUniverse; i <= lastUniverse; i++)
  {
    getHistograms (inputFiles, histograms, nFiles, "numu_corr",
                   Form ("h_rw_numu_corr_%i", i));
    drawAtOnePlot (histograms, nFiles, Form ("numu_corrected_universe_%i", i));
  }  
}

// ---------- DEFINITIONS ---------- //

// convert string "file1 file2 ..." to vector of strings and return its size
UInt_t makeList (const string &input, vector <string> &listFiles)
{
  std::istringstream issFiles(input); // convert char* to iss

  string temp; // temporary store
  while (issFiles >> temp) listFiles.push_back(temp);

  return listFiles.size();
}

// open and check input files
bool openFiles (TFile **inputFiles, vector <string> &listFiles)
{
  cout << "Loading files:\n";
   
  for (auto file = listFiles.begin(); file != listFiles.end(); ++file)
  {
    // open a file
    *inputFiles = new TFile ((*file).c_str(), "READ");
    // check if file is ok
    if ((TFile*)(*inputFiles)->IsZombie())
    {
      cout << "\nERROR: " << *file << " is zombie.\n\n";
      return false;
    }
    else cout << *file << " loaded\n";
    // move pointer
    inputFiles++;
  }

  return true;
}

// get the same histogram from several files
void getHistograms (TFile **inputFiles, TH1D **histograms, const UInt_t &nFiles,
                    const char *histoDir, const char *histoName)
{
  for (UInt_t i = 0; i < nFiles; i++)
  {
    // get histogram
    *histograms = (TH1D*)(*inputFiles)->Get (Form ("%s/%s", histoDir,
                                                   histoName));
    // set histogram name and move pointers
    (*histograms++)->SetName ((*inputFiles++)->GetName());
  }
}

// draw several histograms at one plot
void drawAtOnePlot (TH1D **histograms, const UInt_t &nHistograms,
                    const char *histoName)
{
  static UInt_t plotID = 1; // to be used in output eps filename
   
  // create multihistogram to plot
  THStack multiHistogram (histoName, histoName);

  //create canvas
  TCanvas *c = new TCanvas (histoName);
  TLegend *legend = new TLegend(.6,.6,.85,.9);

  // create legend
  //legend = new TLegend (0.6,0.6,0.9,0.9);

  for (UInt_t i = 0; i < nHistograms; i++) // loop over histograms
  {
    // set line color
    (*histograms)->SetLineColor (1 + i);
    // add histogram to legend
    legend->AddEntry ((*histograms), (*histograms)->GetName(), "L");
    //add histogram to stack and move pointer
    multiHistogram.Add (*histograms++);
  }
  
  // draw multihistogram and set axis titles etc
  multiHistogram.Draw ("nostack"); 
  multiHistogram.GetXaxis()->SetTitle ("Neutrino energy [GeV]");
  multiHistogram.GetYaxis()->SetTitle ("Arbitrary units");
  multiHistogram.GetXaxis()->SetTitleOffset (1.25);
  multiHistogram.GetYaxis()->SetTitleOffset (1.25);
  multiHistogram.GetXaxis()->SetRangeUser(0.0, 20.0);

  // draw legend
  legend->Draw();

  // save plot
  c->SaveAs(Form("plots/%05i.%s.eps", plotID++, histoName));
}
