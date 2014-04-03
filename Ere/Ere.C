#include<TRandom.h>
#include<TMath.h>
#include<iostream>

void Ere(){
c=new TCanvas("c","c",800,600);
c->Divide(4,3);
  for(int i=0;i<10;i++){
   stringstream tmp;
    tmp<<i;
        string hname = "/publicfs/dyb/data/userdata/chengyp/temp/juno/offline/Reconstruction/IntegralPmtRec/share/addnoise_less_positron_1000"+tmp.str()+".root";
	    const char* gname=hname.c_str();

	    f=new TFile(gname);
        
	   evt=(TTree*)f.Get("tree");
	   evt->AddFriend("tree1",gname);
	   evt->AddFriend("tree2",gname);
	   c->cd(i+1);
  //evt->Draw("tree.totalpe/tree1.edep/(1041.99+0.111526*(tree2.r3)*1.0e-9)","(tree2.r3)*1.0e-9<3870");
  evt->Draw("tree1.edep","(tree2.r3)*1.0e-9<3870");
  TH1F *htemp = (TH1F*)gPad->GetPrimitive("htemp");
cout<<htemp->GetRMS()<<endl;
//cout<<htemp->GetMean()<<endl;
  //  htemp->Fit("gaus","Q");
//  TF1 *fitgaus=htemp->GetFunction("gaus");
//  cout<<fitgaus->GetParameter(2)<<"  error "<<fitgaus->GetParError(2)<<endl;


//**  evt->Draw("tree.totalpe/tree1.edep:((tree2.r3)*1.0e-9)","(tree2.r3)*1.0e-9<3870");
//**
//**evt->Draw("nPhotons/m_edep[0]:pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9","pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9<3870");
//** TGraph *gr = new TGraph(evt->GetSelectedRows(),
//**evt->GetV2(), evt->GetV1());
//**gr->Draw("ap"); //draw graph in current pad
//**
//**
//**gr->Fit("pol1","Q");
//**TF1 *tf1=gr->GetFunction("pol1");
//**Double_t p0=tf1->GetParameter(0);
//**Double_t p1=tf1->GetParameter(1);
//**cout<<"p0  "<<p0;
//**cout<<"    p1  "<<p1<<endl;
//**
//(TString::Format("slatc==%i", i)).Data()
//*	    evt->Draw(TString::Format("nPhotons/m_edep[0]/(1155.06+0.123515*(pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9)):m_edep[0]").Data()
//*		,"pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9<3870");
//*
//*TGraph *gr = new TGraph(evt->GetSelectedRows(),
//*evt->GetV2(), evt->GetV1());
//*gr->Draw("ap"); 




//TH1F *htemp = (TH1F*)gPad->GetPrimitive("htemp");
//htemp->Fit("gaus","Q");
//TF1 *fitgaus=htemp->GetFunction("gaus");
//cout<<fitgaus->GetParameter(2)<<endl;


  }


}
