#ifndef ElecSimClass_h
#define ElecSimClass_h
#include <set>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TRandom.h>
#include <string>
#include <sstream>
#include <cassert>
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <algorithm>
#include <queue>

using namespace std;

// the class used for the buffer_hit_vector 

class Hit {

    public:
        Hit(int o_pmtID,
                long double o_hitTime,
                double o_weight):
            m_pmtID(o_pmtID),
            m_hitTime(o_hitTime),
            m_weight(o_weight)
    {}

        Hit(const Hit& others)
            :
                m_pmtID(others.m_pmtID),
                m_hitTime(others.m_hitTime),
                m_weight(others.m_weight)
    {}

        int pmtID();
        long double hitTime();
        long double hitTime() const;
        double weight();
        double weight() const;

    private:

        int m_pmtID;
        long double m_hitTime;
        double m_weight;
};



class Compare{
    public:
        bool operator() (Hit a, Hit b){
            return a.hitTime()>b.hitTime(); 
        }

};




class Root_IO{

    public:
        void initial_Sig();
        void initial_BK1(const char* BK1, double EvtRate);
        void initial_BK2(const char* BK2, double EvtRate);
        void initial_BK3(const char* BK3, double EvtRate);
        void reset();
        void clear_unit_hit_vector();
        void add_sig_to_unit_hit_vector(vector<Hit>& Sig_sub_vector);
        vector<Hit>& get_unit_hit_vector();

        int get_Sig_nEntries();
        int get_Sig_nPhotons();
        double get_Sig_evt_GlobalTime();
        int* get_Sig_pmtID();
        float* get_Sig_hitTime();
        TTree* get_Sig_tree();
        TTree* get_output_tree();
        TFile* get_input_file();
        TFile* get_output_file();


        void mixed_Sig_BK(vector<Hit>& Sig_sub_vector);

        int& SigIdx();

        ///////////////////////////////////////////////////////////
        void set_sig_evt_idx(int SigEvtIdx);
        void set_output_GlobalEvtID();
        void set_output_EventID(int event_id);
        void set_subEvtID(int i);
        void set_output_simTimeEarliest(long double m_simTimeEarliest);
        void set_SigNum(int i);
        void set_SigTotal(int SigIdx,int TimeSample, double value);
        void set_TimeStart(int SigIdx, int value);
        void set_PMT_ID(int SigIdx, int id);
        void set_testTdc();

    private:


        void Add_BK(vector<Hit>& Sig_sub_vector,
                int& EntryNum_BK,
                TTree* n_BK,
                int& nPhotons_BK,
                double& BK_tau,
                float hitTime_BK[],
                int& BK_index
                );


        double first_bin;

        map<int, set<int> >  Sig_Mark;
        set<long> Sig_Mark_Set;
        set<long> test_BK_Set;

        vector<Hit> unit_hit_vector; // the unit vector for produce pulse vector

        ////////// the mixed Sig and BK buffer
        priority_queue<Hit, vector<Hit>, Compare> buffer_hit_vector;


        ////////// the Signal 
        int sig_evt_idx;  //  just sig evt idx
        int nEntries_Sig;
        int nPhotons_Sig;
        int pmtID_Sig[250000];
        float hitTime_Sig[250000];
        double evt_GlobalTime;
        double Sig_tau;


        ///////// the BK1
        int nEntries_BK1;
        int nPhotons_BK1;
        int EntryNum_BK1;
        int pmtID_BK1[250000];
        float hitTime_BK1[250000];
        double BK1_tau;
        int BK1_index;

        ///////// the BK2
        int nEntries_BK2;
        int nPhotons_BK2;
        int EntryNum_BK2;
        int pmtID_BK2[250000];
        float hitTime_BK2[250000];
        double BK2_tau;
        int BK2_index;


        ///////// the BK3
        int nEntries_BK3;
        int nPhotons_BK3;
        int EntryNum_BK3;
        int pmtID_BK3[250000];
        float hitTime_BK3[250000];
        double BK3_tau;
        int BK3_index;

        ////////
        TFile* f_Sig;
        TFile* f_BK1;
        TFile* f_BK2;
        TFile* f_BK3;

        TTree* n_Sig;
        TTree* n_BK1;
        TTree* n_BK2;
        TTree* n_BK3;

        ////////
        TFile* f2;
        TTree* tree;

        int m_SigIdx; // each subEvt need a Idx for all signal

        //output parameter
        double simTimeEarliest;
        int GlobalEvtID;
        int Event_ID;
        int subEvtID;
        int SigNum; // the signal num of one event
        int PMT_ID[200000]; //key is signal id, vaule is pmt id
        int TimeStart[100000]; // key is signal id, value is pulseHitTime
        int testTdc[50]; //just for draw the waveform
        double SigTotal[100000][50]; // first key is signal id, second key is 500ns for save waveform amplitude

};




class Pulse {

    public:
        Pulse(int o_pmtID,
                double o_amplitude,
                double o_pulseHitTime)
            :
                m_pmtID(o_pmtID),
                m_amplitude(o_amplitude),
                m_pulseHitTime(o_pulseHitTime)
    {}

        Pulse(const Pulse& others)
            :
                m_amplitude(others.m_amplitude),
                m_pulseHitTime(others.m_pulseHitTime),
                m_pmtID(others.m_pmtID)
    {}

        double amplitude();
        double pulseHitTime();
        int pmtID();

    private:

        double m_amplitude;
        double m_pulseHitTime;
        int m_pmtID;

};










class PmtData{

    public:
        PmtData(int id,
                double efficiency,
                double gain,
                double sigmaGain,
                double afterPulseProb,
                double prePulseProb,
                double darkRate,
                double timeSpread,
                double timeOffset): 
            m_pmtId (id),
            m_efficiency (efficiency),
            m_gain (gain),
            m_sigmaGain (sigmaGain),
            m_afterPulseProb (afterPulseProb),   
            m_prePulseProb (prePulseProb), 
            m_darkRate (darkRate),
            m_timeSpread (timeSpread),
            m_timeOffset (timeOffset)
    {
    }

        PmtData(const PmtData& others)
            :
                m_pmtId(others.m_pmtId),
                m_efficiency(others.m_efficiency),
                m_gain(others.m_gain),
                m_sigmaGain(others.m_sigmaGain),
                m_afterPulseProb(others.m_afterPulseProb),
                m_prePulseProb(others.m_prePulseProb), 
                m_darkRate(others.m_darkRate),
                m_timeSpread(others.m_timeSpread),
                m_timeOffset(others.m_timeOffset)    
    {
    }


        int pmtId();
        double efficiency();
        double gain();
        double sigmaGain();
        double afterPulseProb();
        double prePulseProb();
        double darkRate();
        double timeSpread();
        double timeOffset();

    private:
        int m_pmtId;
        double m_efficiency;
        double m_gain;
        double m_sigmaGain;
        double m_afterPulseProb;
        double m_prePulseProb; 
        double m_darkRate;
        double m_timeSpread;
        double m_timeOffset;
};



class FeeSimData{

    public:
        FeeSimData(int id) :
            m_channelId ( id),
            m_channelThreshold ( 0.15),
            m_adcRangeHigh ( 0.0554),
            m_adcRangeLow ( 1),
            m_adcBaselineHigh ( gRandom->Gaus(110,19.46)),
            m_adcBaselineLow ( gRandom->Gaus(110,19.46))
    {
    }

        FeeSimData(const FeeSimData& others):
            m_channelId(        others.m_channelId),  
            m_channelThreshold( others.m_channelThreshold), 
            m_adcRangeHigh(     others.m_adcRangeHigh),     
            m_adcRangeLow(      others.m_adcRangeLow),      
            m_adcBaselineHigh(  others.m_adcBaselineHigh),  
            m_adcBaselineLow(   others.m_adcBaselineLow)
    {

    }

        int channelId();
        double channelThreshold();
        double adcRangeHigh();
        double adcRangeLow();
        double adcBaselineHigh();
        double adcBaselineLow();

    private:

        int m_channelId;  // Electronics channel ID number, m_channelId == m_pmtId
        double m_channelThreshold; // FEE discriminator threshold
        double m_adcRangeHigh;     // Full voltage range of high-gain ADC
        double m_adcRangeLow;      // Full voltage range of low-gain ADC
        double m_adcBaselineHigh;  // High-gain baseline ADC value
        double m_adcBaselineLow;   // Low-gain baseline ADC value
};



typedef vector<int> DigitalSignal;
typedef vector<double> AnalogSignal;

/*
class ElecFeeChannel{
    public:
        DigitalSignal m_hit;     ///< Discriminator signal
        DigitalSignal m_adcHigh; ///< High-Gain ADC signal
        DigitalSignal m_adcLow;  ///< Low-Gain ADC signal
        AnalogSignal  m_energy;  ///< Channel contribution to analog esum  raw_signal


        //fangxiao add
        vector<int> m_tdc;
        vector<double> m_adc;

};
*/


/// Map of FEE channel data by pmt ID
//typedef map<int,ElecFeeChannel> ChannelData;
/// Map of Digital Signals data by pmt ID
typedef map<int,DigitalSignal> DigitalMap;
/// Map of Analog Signals data by pmt ID
typedef map<int,AnalogSignal> AnalogMap;



/*
class ElecFeeCrate : public TObject{

    public:
        void reset();

        ChannelData   m_channelData; ///< The set of channels in this crate.
        DigitalMap    m_nhit;        ///< Nhit signals for each FEE board
        AnalogMap     m_esum;        ///< unshaped Esum signals for each FEE board
        AnalogSignal  m_esumUpper;   ///< Esum signal for Upper ESum
        AnalogSignal  m_esumLower;   ///< Esum signal for Lower ESum
        AnalogSignal  m_esumTotal;   ///< ESum signal for Total ESum (Upper + Lower)
        DigitalSignal m_esumADC;     ///< ESum signal for ADC ESum Completion

        //fangxiao add
        int m_second;
        int m_nanosecond;
        long double m_TimeStamp;    // ie. m_simTimeEarliest
        vector<int> m_triggerTime;  // the vector is used to save triggerTime in one event

};
*/



class Hit_Collection{

    public:
        void reset();
        void create_vector(int nPhotons,vector<int>& pmtID,vector<double>& hitTime, double Sig_evt_GlobalTime);
        vector<Hit>& get_sub_vector(int i);
        double get_earliest();
        double get_latest();
        map<int, vector<Hit> >& get_hit_map();

        void check_hitTime(int pmtID, int map_num, double earliest_item);

    private:  
        vector<Hit> total_hit_vector; //contain the all hits of one event
        map<int, vector<Hit> > hit_map; //key is the subEvtId, value is the hit vector
        double earliest_item;
        double latest_item;
};


class Pulse_Collection{

    public:
        void reset();
        vector<Pulse>& get_vector();
        void check_pulseHitTime(int pmtID);

    private: 
        vector<Pulse> pulse_vector;

};


class PmtData_Collection{

    public:
        void create_vector(const char* PmtData, int PmtTotal);
        vector<PmtData>& get_vector();

    private: 
        vector<PmtData> pd_vector;

};


class FeeSimData_Collection{

    public:
        void create_vector(int PmtTotal);
        vector<FeeSimData>& get_vector();

    private: vector<FeeSimData> fsd_vector;
             int PmtTotal;

};










#endif
