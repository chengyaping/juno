#include "ElecSimClass.h"

#include "Identifier/CdID.h"

//class Pulse  

double Pulse::amplitude(){
    return m_amplitude;
}

double Pulse::pulseHitTime(){
    return m_pulseHitTime; 
}

int Pulse::pmtID(){
    return m_pmtID; 
}



//class Hit


int Hit::pmtID(){
    return m_pmtID; 
}

long double Hit::hitTime(){
    return m_hitTime; 
}

long double Hit::hitTime() const{
    return m_hitTime; 
}

double Hit::weight(){
    return m_weight; 
}

double Hit::weight() const{ return m_weight; 
}



//class PmtData


int PmtData::pmtId(){
    return m_pmtId; 
}

double PmtData::efficiency(){
    return m_efficiency; 
}

double PmtData::gain(){
    return m_gain; 
}

double PmtData::sigmaGain(){
    return m_sigmaGain; 
}

double PmtData::afterPulseProb(){
    return m_afterPulseProb;
}

double PmtData::prePulseProb(){
    return m_prePulseProb; 
}

double PmtData::darkRate(){
    return m_darkRate; 
}

double PmtData::timeSpread(){
    return m_timeSpread;
}

double PmtData::timeOffset(){
    return m_timeOffset; 
}


//class FeeSimData


int FeeSimData::channelId(){
    return m_channelId; 
}

double FeeSimData::channelThreshold(){
    return m_channelThreshold; 
}

double FeeSimData::adcRangeHigh(){
    return m_adcRangeHigh; 
}

double FeeSimData::adcRangeLow(){
    return m_adcRangeLow;
}

double FeeSimData::adcBaselineHigh(){
    return m_adcBaselineHigh;
}

double FeeSimData::adcBaselineLow(){
    return m_adcBaselineLow; 
}


//class Hit_Collection


void Hit_Collection::reset(){
    total_hit_vector.clear(); 
    hit_map.clear();
}

bool SortByhitTime(const Hit& hit1,const Hit& hit2){
    return hit1.hitTime() < hit2.hitTime(); 
}  // the sort function can not in a class 


void Hit_Collection::create_vector(int nPhotons,vector<int>& pmtID,vector<double>& hitTime, double Sig_evt_GlobalTime){


    for(int i=0; i<nPhotons; i++){
        long double o_hitTime = (long double)(hitTime[i]) + (long double)(Sig_evt_GlobalTime * 1.e9);  // unit ns

        double o_weight = 1.;
        int o_pmtID = pmtID[i];
        Hit hit(o_pmtID, o_hitTime, o_weight);
        total_hit_vector.push_back(hit);
    }
//    cout<<"nPhotons: " <<nPhotons<<endl;
    std::sort(total_hit_vector.begin(),total_hit_vector.end(), SortByhitTime);

    std::vector<Hit>::iterator a = total_hit_vector.begin();
    double hitTime_temp = 0;
    int hitTime_map_first = 0;

    if(total_hit_vector.size() > 0){
        hit_map[0].push_back(*a); 
        for(a=total_hit_vector.begin()+1;
                a!=total_hit_vector.end();
                a++){
            if( (*a).hitTime() - (*(a-1)).hitTime() > 10000 ){
                hitTime_map_first++; 
            } // if two hitTime at a distance of 10000ns ,so we think there is two subEvt 
            hit_map[hitTime_map_first].push_back(*a);

        }
    //    std::cout<<"subEvt num: "<<hit_map.size()<<std::endl;
    }
}

vector<Hit>& Hit_Collection::get_sub_vector(int i){
    return hit_map[i];
}     

double Hit_Collection::get_earliest(){
    return earliest_item; 
}

double Hit_Collection::get_latest(){
    return latest_item; 
} 

map<int, vector<Hit> >& Hit_Collection::get_hit_map(){
    return hit_map;
}

void Hit_Collection::check_hitTime(int pmtID, int map_num, double earliest_item){

    vector<Hit>::iterator it = hit_map[map_num].begin();
    for(;it != hit_map[map_num].end(); it++){
        if(it->pmtID()==pmtID){
            cout<<"pmtID="<<pmtID<<"  hitTime:  " << it->hitTime() - earliest_item <<endl;
        } 
    }
}


//class Pulse_Collection

void Pulse_Collection::reset(){
    pulse_vector.clear(); 
}

vector<Pulse>& Pulse_Collection::get_vector(){
    return pulse_vector; 
}

void Pulse_Collection::check_pulseHitTime(int pmtID){

    vector<Pulse>::iterator it = pulse_vector.begin();
    for(;it != pulse_vector.end(); it++){
        if(it->pmtID()==pmtID){
            cout<<"pmtID="<<pmtID<<"  PulesHitTime:  " << it->pulseHitTime()<<endl;
        } 
    }
}


//class PmtData_Collection


void PmtData_Collection::create_vector(const char* m_PmtData, int PmtTotal){
    int m_pmtId;
    double m_efficiency;
    double m_gain;
    double m_sigmaGain;
    double m_afterPulseProb;
    double m_prePulseProb; 
    double m_darkRate;
    double m_timeSpread;
    double m_timeOffset;

    TFile* f1 = new TFile(m_PmtData); 
    TTree* t1 = (TTree*) f1->Get("PmtData");
    t1 ->SetBranchAddress("pmtId",&m_pmtId);
    t1 ->SetBranchAddress("efficiency",&m_efficiency);
    t1 ->SetBranchAddress("gain",&m_gain);
    t1 ->SetBranchAddress("sigmaGain",&m_sigmaGain);
    t1 ->SetBranchAddress("afterPulseProb",&m_afterPulseProb);
    t1 ->SetBranchAddress("prePulseProb",&m_prePulseProb);
    t1 ->SetBranchAddress("darkRate",&m_darkRate);
    t1 ->SetBranchAddress("timeSpread",&m_timeSpread);
    t1 ->SetBranchAddress("timeOffset",&m_timeOffset); 

    for(int i=0; i<PmtTotal; i++){
        t1->GetEntry(i);
        PmtData pd(m_pmtId,
                m_efficiency,
                m_gain,
                m_sigmaGain,
                m_afterPulseProb,
                m_prePulseProb,
                m_darkRate,
                m_timeSpread,
                m_timeOffset);

        pd_vector.push_back(pd);
    }
}


vector<PmtData>& PmtData_Collection::get_vector(){
    return pd_vector;
}


//class FeeSimData_Collection


void FeeSimData_Collection::create_vector(int PmtTotal){
    for(int id=0; id<PmtTotal; id++){ 
        FeeSimData fsd(id);
        fsd_vector.push_back(fsd);
    }
}

vector<FeeSimData>& FeeSimData_Collection::get_vector(){
    return fsd_vector; 
}


//class  ElecFeeCrate
/*
void ElecFeeCrate::reset(){

    m_channelData.clear();
    m_nhit.clear();
    m_esum.clear();
    m_esumUpper.clear();
    m_esumLower.clear();
    m_esumTotal.clear();
    m_esumADC.clear();
    m_second = 0;
    m_nanosecond = 0;
    m_TimeStamp = 0;
    m_triggerTime.clear();

}
*/

//class Root_IO

void Root_IO::initial_Sig(){

    EntryNum_BK1 = 0;
    EntryNum_BK2 = 0;
    EntryNum_BK3 = 0;
    evt_GlobalTime = 0;
    GlobalEvtID = 0;
    Sig_tau = 1/(462.963*1.e-6); //the IBD frequency is 462.963 muHZ
    Sig_Mark_Set.clear();
    test_BK_Set.clear();
    Sig_Mark.clear();
    unit_hit_vector.clear();
}


void Root_IO::initial_BK1(const char* BK1, double EvtRate){

    f_BK1 = TFile::Open(BK1); 
    n_BK1 = (TTree*) f_BK1->Get("evt");
    n_BK1->SetBranchAddress("nPhotons",&nPhotons_BK1);
    n_BK1->SetBranchAddress("pmtID",pmtID_BK1);
    n_BK1->SetBranchAddress("hitTime",hitTime_BK1); 

    BK1_tau = 1.0/EvtRate; // the event rate of K is 0.14099 MHz
    nEntries_BK1 = n_BK1->GetEntries();

}


void Root_IO::initial_BK2(const char* BK2, double EvtRate){

    f_BK2 = TFile::Open(BK2); 
    n_BK2 = (TTree*) f_BK2->Get("evt");
    n_BK2->SetBranchAddress("nPhotons",&nPhotons_BK2);
    n_BK2->SetBranchAddress("pmtID",pmtID_BK2);
    n_BK2->SetBranchAddress("hitTime",hitTime_BK2); 

    BK2_tau = 1.0/EvtRate; // the event rate of Th is 0.12341MHz
    nEntries_BK2 = n_BK2->GetEntries();
}


void Root_IO::initial_BK3(const char* BK3, double EvtRate){

    f_BK3 = TFile::Open(BK3); 
    n_BK3 = (TTree*) f_BK3->Get("evt");
    n_BK3->SetBranchAddress("nPhotons",&nPhotons_BK3);
    n_BK3->SetBranchAddress("pmtID",pmtID_BK3);
    n_BK3->SetBranchAddress("hitTime",hitTime_BK3); 

    BK3_tau = 1.0/EvtRate; //the event rate of U is 0.57714MHz
    nEntries_BK3 = n_BK3->GetEntries();
}




void Root_IO::reset(){
    m_SigIdx = 0; // signal index, for one sub evt the id of the sig waveform 
}


int Root_IO::get_Sig_nEntries(){
    return nEntries_Sig;
}

int Root_IO::get_Sig_nPhotons(){
    return nPhotons_Sig;
}

int* Root_IO::get_Sig_pmtID(){
    return pmtID_Sig;
}

float* Root_IO::get_Sig_hitTime(){
    return hitTime_Sig;    
}

TTree* Root_IO::get_Sig_tree(){
    return n_Sig;
}

TTree* Root_IO::get_output_tree(){
    return tree;
}

TFile* Root_IO::get_input_file(){
    return f_Sig;
}

TFile* Root_IO::get_output_file(){
    return f2;
}

int& Root_IO::SigIdx(){
    return m_SigIdx;  
}


double Root_IO::get_Sig_evt_GlobalTime(){
    double Exp = gRandom->Exp(Sig_tau);
    evt_GlobalTime = evt_GlobalTime + Exp;
    //  cout<<"evt_GlobalTime in unit s: "<< evt_GlobalTime<<endl;
    return evt_GlobalTime; 
}



/////////////////////////////////////////////////


void Root_IO::set_sig_evt_idx(int SigEvtIdx){
    sig_evt_idx = SigEvtIdx; 
}

void Root_IO::set_output_GlobalEvtID(){
    GlobalEvtID++ ;
}

void Root_IO::set_output_EventID(int event_id){
    Event_ID = event_id;
}

void Root_IO::set_subEvtID(int i){
    subEvtID = i; 
}

void Root_IO::set_output_simTimeEarliest(long double m_simTimeEarliest){
    simTimeEarliest = double(m_simTimeEarliest);
}


void Root_IO::set_SigNum(int i){
    SigNum = i;
}


void Root_IO::set_SigTotal(int m_SigIdx,int TimeSample, double value){
    SigTotal[m_SigIdx][TimeSample] = value; 
}

void Root_IO::set_TimeStart(int m_SigIdx, int value){
    TimeStart[m_SigIdx] = value; 
}

void Root_IO::set_PMT_ID(int m_SigIdx, int id){
    PMT_ID[m_SigIdx] = id;
}


void Root_IO::set_testTdc(){
    for(int i=0; i<50; i++){
        testTdc[i] = i; 
    }
}

void Root_IO::clear_unit_hit_vector(){
    unit_hit_vector.clear();
}

vector<Hit>& Root_IO::get_unit_hit_vector(){
    return unit_hit_vector; 
}


void Root_IO::add_sig_to_unit_hit_vector(vector<Hit>& Sig_sub_vector){

    vector<Hit>::iterator iter, first_hit, latest_hit;

    for(iter=Sig_sub_vector.begin(); iter != Sig_sub_vector.end(); iter++){
        unit_hit_vector.push_back(*iter);
    }

}



void Root_IO::Add_BK(vector<Hit>& Sig_sub_vector,
        int& EntryNum_BK,
        TTree* n_BK,
        int& nPhotons_BK,
        double& BK_tau,
        float hitTime_BK[],
        int& BK_index
        ){


    vector<Hit>::iterator iter, first_hit, latest_hit;
    first_hit = Sig_sub_vector.begin();
    latest_hit = Sig_sub_vector.end()-1;


    double evtGlobalTime_BK = (*first_hit).hitTime()-10000;// for each Sig hit vector, use the first hitTime - 10000ns as the BK start time , maybe need modify.

    double BK_totalNum = n_BK -> GetEntries();
    for( ; EntryNum_BK < BK_totalNum ; EntryNum_BK++ ){

        n_BK -> GetEntry(EntryNum_BK);
        double Exp = gRandom->Exp(BK_tau)*1e9;
        evtGlobalTime_BK = evtGlobalTime_BK + Exp;


        if(evtGlobalTime_BK < (*latest_hit).hitTime()){

            for(int i=0; i<nPhotons_BK; i++){

                long double hitGlobalTime_BK =(long double)(hitTime_BK[i]) + (long double)(evtGlobalTime_BK); //unit ns


                long double time_delta_1 = ( hitGlobalTime_BK - (*first_hit).hitTime() );
                long double time_delta_2 = ( hitGlobalTime_BK - (*latest_hit).hitTime() );


                if(time_delta_1 > -100 && time_delta_2 < 100 ) 
                {

                    unit_hit_vector.push_back(Hit(pmtID_BK1[i], hitGlobalTime_BK, 1)); 

                    BK_index++;
                }

            }

        }else {break;}


        //if BK not enough,we can recycle the BK root 
        if(sig_evt_idx != nEntries_Sig-1 && EntryNum_BK == BK_totalNum-1){
            EntryNum_BK = 0; 
        }

        if(sig_evt_idx == nEntries_Sig-1){
            break; 
        }


    }


}



void Root_IO::mixed_Sig_BK(vector<Hit>& Sig_sub_vector){

    BK1_index = 0;
    BK2_index = 0;
    BK3_index = 0;


    vector<Hit>::iterator iter, first_hit, latest_hit;
    first_hit = Sig_sub_vector.begin();
    latest_hit = Sig_sub_vector.end()-1;

    cout<<"first hit time: "<< (*first_hit).hitTime()<<endl;
    cout<<"latest hit time: "<< (*latest_hit).hitTime()<<endl;
    cout<<"hit Time delta: "<< (*latest_hit).hitTime()-(*first_hit).hitTime()<<endl;

    //add BK1:

    cout<<"EntryNum_BK1 begin: "<<EntryNum_BK1<<endl;
    Add_BK(Sig_sub_vector,
            EntryNum_BK1,
            n_BK1,
            nPhotons_BK1,
            BK1_tau,
            hitTime_BK1,
            BK1_index);


    cout<<"EntryNum_BK1 end before: "<<EntryNum_BK1<<endl;
    cout<<"Mixed BK1 num is: "<<BK1_index<<endl;

    /////////////////////////////////////////////////////////////

    //add BK2:

    cout<<"EntryNum_BK2 begin: "<<EntryNum_BK2<<endl;
    Add_BK(Sig_sub_vector,
            EntryNum_BK2,
            n_BK2,
            nPhotons_BK2,
            BK2_tau,
            hitTime_BK2,
            BK2_index);

    cout<<"EntryNum_BK2 end before: "<<EntryNum_BK2<<endl;
    cout<<"Mixed BK2 num is: "<<BK2_index<<endl;
    //////////////////////////////////////////////////////////////

    //add BK3:

    cout<<"EntryNum_BK3 begin: "<<EntryNum_BK3<<endl;
    Add_BK(Sig_sub_vector,
            EntryNum_BK3,
            n_BK3,
            nPhotons_BK3,
            BK3_tau,
            hitTime_BK3,
            BK3_index);

    cout<<"EntryNum_BK3 end before: "<<EntryNum_BK3<<endl;
    cout<<"Mixed BK3 num is: "<<BK3_index<<endl;

}


