void modifytree()
{
TFile f("/publicfs/dyb/data/userdata/chengyp/temp/juno/offline/Reconstruction/IntegralPmtRec/share/gain.root","update");
TFile f1("/workfs/dyw/chengyp/juno/offline/Simulation/ElecSim/share/PmtData.root","read");
double timeSpread;
double timeOffset;
TTree *t3 = (TTree* )f->Get("gain");
TTree *t4 = (TTree* )f1->Get("PmtData");
t4->SetBranchAddress("timeSpread",&timeSpread);
t4->SetBranchAddress("timeOffset",&timeOffset);

double time;
TBranch *newBranch = t3-> Branch("time",&time,"time/D");
Int_t nentries = (Int_t)t3->GetEntries();cout<<nentries<<endl;
for (Int_t i = 0; i < nentries; i++){
t4->GetEntry(i);
time=gRandom->Gaus(0,1)*timeSpread + timeOffset;
cout<<time<<endl;
	
  newBranch->Fill();
}
f1.Close();
t3->Print();
f.cd();
t3->Write("",TObject::kOverwrite);     // save only the new version of the tree
}
