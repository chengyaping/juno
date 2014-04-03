#include <vector>


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TRandom.h>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdio.h>
#include <iostream>

#include <TMath.h>

#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"



#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif


const int  nu=16720;
double pmt_pos_x[nu];
double pmt_pos_y[nu];
double pmt_pos_z[nu];

int main(int argc, char** argv)
{
  
  gROOT->ProcessLine("#include <vector>");
  
  TRandom *rd= new TRandom();
  TRandom *rd1= new TRandom();
  TRandom *rd2= new TRandom();

double lengtha=20000;//25m


double sourcex;
double sourcey;
double sourcez;


Float_t pmt_pos_z_temp,
pmt_pos_x_temp,
pmt_pos_y_temp;



TFile *inf=new TFile("/workfs/dyw/chengyp/designparameter/Th.root");
TTree *pmtInfo=(TTree* )inf->Get("pmtInfo");
pmtInfo->SetBranchAddress("pmt_pos_z",&pmt_pos_z_temp);
pmtInfo->SetBranchAddress("pmt_pos_x",&pmt_pos_x_temp);
pmtInfo->SetBranchAddress("pmt_pos_y",&pmt_pos_y_temp);


for(Int_t i=0;i<16720;i++){
pmtInfo->GetEntry(i);
pmt_pos_z[i]=pmt_pos_z_temp;
pmt_pos_x[i]=pmt_pos_x_temp;
pmt_pos_y[i]=pmt_pos_y_temp;
}

inf->Close();



  
 TFile *tf=new TFile("likelihood.root","recreate"); 

 std::vector<float> vpos3;
    std::vector<float> vnpe;

TTree *t = new TTree("tvec","Tree with vectors");
   t->Branch("vpos3",&vpos3);
      t->Branch("vnpe",&vnpe);
      vpos3.clear();
      vnpe.clear();

  //
//energy 0.1MeV 9.1 MeV

double energy=9.1;
double npe=0.;
double r3=0.;


//TH2F *f=new TH2F(Form("h_%d",i),Form("h_%d",i),2501,0,5000,5001,0,5000);
for(int evt=0;evt<1000;evt++)
{
// random generated in ball
double r=rd->Uniform()*17700;
double theta=rd1->Uniform()*3.14;
double phi=rd2->Uniform()*3.14*2;

sourcex=r*TMath::Sin(theta)*TMath::Cos(phi);
sourcey=r*TMath::Sin(theta)*TMath::Sin(phi);
sourcez=r*TMath::Cos(theta);
 npe=0.;


double  possource=sqrt(sourcex*sourcex+
     sourcey*sourcey+
     sourcez*sourcez);
  // pmt id 0,16720
   for(int j=0;j<16720;j++)
   {

   double r1=sqrt((sourcex-pmt_pos_x[j])*(sourcex-pmt_pos_x[j])+(sourcey-pmt_pos_y[j])*(sourcey-pmt_pos_y[j])+(sourcez-pmt_pos_z[j])*(sourcez-pmt_pos_z[j]));
//     r3=pow(sqrt((sourcex-pmt_pos_x[j])*(sourcex-pmt_pos_x[j])+(sourcey-pmt_pos_y[j])*(sourcey-pmt_pos_y[j])+(sourcez-pmt_pos_z[j])*(sourcez-pmt_pos_z[j])),3);
//
  r3=pow(r1,3);
  double tmp1=sourcex*pmt_pos_x[j]+sourcey*pmt_pos_y[j]+sourcez*pmt_pos_z[j];
    double tmp2=sourcex*sourcex+sourcey*sourcey+sourcez*sourcez;
    double tmp3=pmt_pos_x[j]*pmt_pos_x[j]+pmt_pos_y[j]*pmt_pos_y[j]+pmt_pos_z[j]*pmt_pos_z[j];
   double temptheta=tmp1/sqrt(tmp2)/sqrt(tmp3);
    // double temptheta=(sourcex*pmt_pos_x+sourcey*pmt_pos_y+sourcez*pmt_pos_z)/sqrt(sourcex*sourcex+sourcey*sourcey+sourcez*sourcez)/sqrt(pmt_pos_x*pmt_pos_x+pmt_pos_y*pmt_pos_y+pmt_pos_z*pmt_pos_z);

 // double  pmttheta=TMath::ACos(temptheta);
   double r2=pow(r1,2);
     
 double  S_angle = 0.378+0.5093*temptheta+0.1135*pow(temptheta,2);
 //double  S_angle = 1.0164 - pmttheta*(0.1553 - pmttheta*(0.2859 - pmttheta*(0.7805 - 0.2926*pmttheta)));

 if(S_angle<1.0e-5) S_angle = 1.0e-5;
// std::cout<<S_angle<<"  exp  "<<exp(-r1/lengtha)/r2*1.0E+7<<std::endl;
    npe+=energy*S_angle*exp(-r1/lengtha)/r2*1.0E+7;

   }
   if(npe>5000)std::cout<<npe<<std::endl;
 //   f->Fill(pow(possource*0.001,3),npe);
     vpos3.push_back( pow(possource*0.001,3));
     vnpe.push_back(npe);

t->Fill();
}
//f->ReSet();

 tf->Write();
 tf->Close();
return 0;
}
