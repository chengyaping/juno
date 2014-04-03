#include<TRandom.h>
#include<TMath.h>
#include<iostream>

void b(){
c=new TCanvas("c","c",800,600);
c->Divide(4,3);
  for(int i=0;i<10;i++){
   stringstream tmp;
    tmp<<i;
        string hname = "/publicfs/dyb/data/userdata/chengyp/dyb2_v2/ep/less/less_positron_1000"+tmp.str()+".root";
	    const char* gname=hname.c_str();

	    f=new TFile(gname);
        

	   evt=(TTree*)f.Get("evt");
	   c->cd(i+1);




//*evt->Draw("nPhotons:pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9>>gr","pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9<3870");
//* TGraph *gr = new TGraph(evt->GetSelectedRows(),
//*evt->GetV2(), evt->GetV1());
//*gr->Draw("ap"); //draw graph in current pad
//*
//*
//*gr->Fit("pol1","Q");
//*TF1 *tf1=gr->GetFunction("pol1");
//*Double_t p0=tf1->GetParameter(0);
//*Double_t p1=tf1->GetParameter(1);



//(
// TString("nPhotons/(") +
// ((Double_t)p0)+((Double_t)p1)+
// TString(*(pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9))
//  ).Data()


//(TString::Format("slatc==%i", i)).Data()
	    
p0=1155.06;
p1=0.123515;

evt->Draw(TString::Format("nPhotons/m_edep[0]/(%f+%f*(pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9))",p0,p1).Data()
		,"pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9<3870");



//evt->Draw("m_edep[0]","pow(sqrt(edep_pos_x[0]*edep_pos_x[0]+edep_pos_y[0]*edep_pos_y[0]+edep_pos_z[0]*edep_pos_z[0]),3)*1.0e-9<3870");
TH1F *htemp = (TH1F*)gPad->GetPrimitive("htemp");
htemp->Fit("gaus","Q");
TF1 *fitgaus=htemp->GetFunction("gaus");
cout<<htemp->GetMean()<<endl;
//cout<<fitgaus->GetParameter(1)<<endl;
//cout<<fitgaus->GetParameter(1)<<"  error "<<fitgaus->GetParError(2)<<endl;


  }


}
