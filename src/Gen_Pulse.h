#ifndef Gen_Pulse_h
#define Gen_Pulse_h
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
#include "ElecSimClass.h"
// DWL: After-pulse timing distribution

#define NUM_BINS_DIST 96

static double afterPusleTimingDist[NUM_BINS_DIST] = {0.0000000, 0.0012950, 0.0030194, 0.0064297, 0.0142225,
    0.0247712, 0.0417949, 0.0696709, 0.1063259, 0.1485606,
    0.1957973, 0.2465997, 0.2957179, 0.3401377, 0.3798086,
    0.4135611, 0.4437462, 0.4702155, 0.4928434, 0.5123140,
    0.5289294, 0.5432514, 0.5556360, 0.5664919, 0.5758642,
    0.5843533, 0.5920903, 0.5990121, 0.6052796, 0.6111246,
    0.6165560, 0.6215807, 0.6261028, 0.6304032, 0.6346304,
    0.6387545, 0.6425872, 0.6465577, 0.6500606, 0.6534989,
    0.6568080, 0.6602148, 0.6635239, 0.6667457, 0.6700025, 
    0.6737148, 0.6780693, 0.6829002, 0.6879599, 0.6935379, 
    0.6994667, 0.7058650, 0.7121394, 0.7187122, 0.7255085, 
    0.7324600, 0.7394151, 0.7467314, 0.7546551, 0.7630884, 
    0.7718917, 0.7816427, 0.7914601, 0.8023700, 0.8137512, 
    0.8255861, 0.8375781, 0.8498354, 0.8621276, 0.8741685, 
    0.8855410, 0.8962833, 0.9061268, 0.9156126, 0.9243357, 
    0.9320726, 0.9392022, 0.9459897, 0.9519587, 0.9574180, 
    0.9623468, 0.9667851, 0.9708308, 0.9744296, 0.9777980, 
    0.9807231, 0.9835296, 0.9861040, 0.9885404, 0.9906504, 
    0.9926209, 0.9943923, 0.9959998, 0.9974658, 0.9987678, 
    1.0000000};

using namespace std;



class Gen_Pulse{
    
    public:
    void initial();
    void SetSimTime(long double earliest_item,long double latest_item);
    void generatePulses(vector<Pulse>& pulse_vector, vector<Hit>& hit_vector, vector<PmtData>& pd_vector);

    int get_PmtTotal();



    private:

    // user-defined PDF and associated bin edges for pre- and after-pulses
    // PDFs defined wrt main pulse time
    // default is single count in [-50ns,0ns] and [0us,10us], respectively
    vector<double> m_prePulsePdf;
    vector<double> m_prePulseEdges;
    vector<double> m_afterPulsePdf;
    vector<double> m_afterPulseEdges;
    vector<double> m_afterPulseTime;

    bool m_enablePrePulse;
    bool m_enableAfterPulse;
    bool m_enableDarkPulse;

    // Mode for afterpulse amplitude distribution:
    // SinglePE: Fixed single p.e. amplitude with Gaussian smearing
    // PDF: Use amplitude PDF with peak at single p.e and long tail

    string m_afterPulseAmpMode;
    string m_darkPulseAmpMode;

    // PDFs for afterpulse amplitude
    vector<double> m_afterPulseAmpPdf;
    vector<double> m_afterPulseAmpEdges;

    // Property: EnableNonlinearAfterpulse
    // Applies nonlinear afterpulsing rate model if true
    bool m_enableNonlinearAfterpulse;
    // Property: LinearAfterpulseThreshold
    // Upper limit of linear afterpulsing in number of PE.
    double m_linearAfterpulseThreshold;

    // Property: EnableVeryLongTimeSuppression
    // Enable suppression of hit times in the far future
    bool m_enableVeryLongTimeSuppression;
    // Property: VeryLongTime
    // Definition of very long time for hit time suppression
    double m_veryLongTime; 

    //Weight of the exponential contribution to the spe response function
    double m_expWeight;
    // Decay time of the exponential
    double m_speExpDecay;
    double m_speCutoff;

    // electronic simulation beginning and end times
    long double m_simTimeEarliest;
    long double m_simTimeLatest;
    double m_simDeltaT;

    // DWL: Size of PMT hits counting window
    double m_timeInterval;


    double vertexTime;

    long double preTimeTolerance;
    long double postTimeTolerance;

    int m_PmtTotal;

    ///////////////////////////////////////////////////////////

    void getAfterPulseAmpPdf(vector<double>& pdf);
    void getAfterPulseAmpEdges(vector<double>& edges);
    double PulseAmp(double weight,double gain, double sigmaGain);
    Pulse makeAfterPulse(Pulse pulse,PmtData pd);
    Pulse makePrePulse( Pulse pulse, PmtData pd);
    Pulse makeDarkPulse( int pmtID,  PmtData& pd);
    double NumAfterPulse( const int numPmtHit);
    int PoissonRand(double mean);
    double ConvertPdfRand01 (double rand,vector<double> pdf, vector<double> edges);

};

#endif
