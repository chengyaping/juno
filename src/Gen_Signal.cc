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
#include "Gen_Signal.h"

bool SortBytOffset(const int& tOffset1,const int& tOffset2){
    return tOffset1 < tOffset2; 
}  // the sort function can not in a struct


void Gen_Signal::initial(){

    BaseFrequencyHz = int(1e8);  //for m_simFrequency to 1 GHz
    TdcCycles = 16;  
    EsumCycles = 4;
    NhitCycles = 2;
    AdcCycles = 10;//fangxiao modify 1 to 10

    m_nHitTriggerThreshold = 10;  
    m_eSumTriggerThreshold = 0.005; //unit volt 
    m_eSumFrequency = BaseFrequencyHz * EsumCycles; 

    m_enablePretrigger = true; 
    m_enableFastSimMode = false;
    m_enableOvershoot =true; 
    m_enableRinging = true;
    m_enableSaturation =true;
    m_enableDynamicWaveform = true;
    m_enableNoise = false;
    m_enableESumH = true; //Turns on/off upper ESum simulation
    m_enableESumL = true; //Turns on/off lower ESum simulation
    m_linearityThreshold = 20;
    //    m_simFrequency = BaseFrequencyHz * TdcCycles; 
    m_simFrequency = BaseFrequencyHz * 10; //fangxiao modify to 1GHz
    m_noiseAmp = 0.5e-3; //unit V
    m_pulseCountSlotWidth = 10; //unit ns
    m_speAmp = 0.0035; //Spe pulse height in V at 1e7 gain
    m_gainFactor = 1.0; //Mean PMT gain (in units of 10^7) 
    m_pulseCountWindow = 1; // Number of counted slots before and after a pulse

    m_adcRange = 1; // Esum Range
    m_adcBits = 10; // Esum Bits
    m_adcOffset = 500; // Esum Frequency

    ////// fangxiao add /////////////////////////////////////////
    debug = true;
    verbose = false; 
    m_SignalDistance = 10;//if two hitTime distance<10ns,the two hit produce one waveform 
    m_SigTimeWindow = 50;//set signal Time Window to 50ns
    m_testTrigger = 200;// the trigger hit num is 200, time window is 300ns
    m_sliceTime = 25;   // the slice time for trigger is 25 ns
    m_sliceNum = 12;    // the slice time for trigger time window is 12, so time window is 300ns.
    m_preTriggerTime = 200;//ns

    m_postTriggerTime = 800; //ns

    ///////////////////////////////////////////////////////////////

    m_PmtTotal = 16720;
    preTimeTolerance = 300; // unit ns
    postTimeTolerance = 2000;// unit ns, 10us

    // Set up pretrigger threshold
    int preThr = int(0.8 * m_nHitTriggerThreshold + 0.5);
    int esumPreThr = int (0.8 * 1500 * m_eSumTriggerThreshold + 0.5);
    if(preThr > esumPreThr) preThr = esumPreThr;
    m_pretriggerThreshold = preThr;
    if( m_enablePretrigger ){
        std::cout << "Pretrigger threshold is " << m_pretriggerThreshold << " pe " << std::endl;
    }
    else {
        std::cout << "Pretrigger disabled" << std::endl;
        std::cout << "Convolution threshold is " << m_pretriggerThreshold << " pe " << std::endl;
    }

    if( m_enableFastSimMode){
        std::cout << "Fast electronics simulation mode enabled: Note that Upper and Lower ESum trigger do not yield meaningful results" << std::endl;
        m_enableDynamicWaveform = false;
        m_enableESumL = false;
        m_enableESumL = false;
    }

    if( !m_enableOvershoot ) 
        std::cout << "Overshoot disabled" << std::endl; 
    else 
        std::cout << "Overshoot enabled" << std::endl;

    if( !m_enableRinging ) 
        std::cout << "Ringing disabled" << std::endl; 
    else 
        std::cout << "Ringing enabled" << std::endl;

    if( !m_enableSaturation ) 
        std::cout << "Saturation disabled" << std::endl; 
    else 
        std::cout << "Saturation enabled" << std::endl;

    if( !m_enableDynamicWaveform ) {
        std::cout << "Dynamic waveform disabled" << std::endl; 
        m_linearityThreshold = 1e6;
    }
    else {
        std::cout << "Dynamic waveform enabled for channels with more than " 
            << m_linearityThreshold << " hits " << std::endl;
    }

}



void Gen_Signal::generateSignals( std::vector<Pulse>& pulse_vector, JM::ElecFeeCrate& crate,  std::vector<FeeSimData>& fsd_vector, Root_IO& m_IO){

    // Initialize crate if necessary

    if(crate.channelData().size() == 0){

        int pmtBoardId = 1; //boardId come from 1

        for(int pmtIdx = 0; pmtIdx < m_PmtTotal; pmtIdx++){

            crate.channelData()[pmtIdx];

/*
            if((pmtIdx+1)%16 == 0 && m_PmtTotal%16 != 0){    //a FeeBoard contain 16 pmts
                crate.m_nhit[pmtBoardId];
                crate.m_esum[pmtBoardId];
                pmtBoardId++;
            }    


            if((pmtIdx+1)%16 == 0 && m_PmtTotal%16 == 0){   //consider the remaining pmts

                crate.m_nhit[pmtBoardId];
                crate.m_esum[pmtBoardId];
                pmtBoardId++; 

                if(pmtIdx == m_PmtTotal-1){
                    pmtBoardId--; 
                }

            }
*/
        } 

    } else {
        std::cout << "does not initialize crate" << std::endl;
        assert(false);
    }

    // Prepare time range of the simulation
    long double earliest = m_simTimeEarliest; //unit ns 
    long double latest = m_simTimeLatest;
    double m_TimeStamp = double(m_simTimeEarliest);
    cout<<"TimeStamp: "<<m_TimeStamp<<endl;

    // save m_simTimeLatest as TimeStamp
    crate.setTimeStamp(m_TimeStamp);

    // The time window for the crate signals
    double simTime = double(latest - earliest); // unit ns
    cout<<"simTime in Gen_Sig:  " << simTime <<endl;

    // The number of samples in time given the simulation frequency
    int simSamples = int(simTime * 1.e-9 *  m_simFrequency);

    std::cout << "simSamples: " << simSamples  <<"   Simulation time is: "<< simTime << std::endl;

    if(verbose){
        std::cout << "Simulation has " << simSamples << " samples at frequency "
            << m_simFrequency << " Hz" << std::endl;
    };



    // Compute time windows that will possibly be readout
    int nTimeSlices = int(simTime / m_sliceTime) + 1;  // let time window length to 150 ns
    std::vector<int> hitCountSlices(nTimeSlices,0);

    for ( int i = 0; i < nTimeSlices; ++i ) {
        hitCountSlices[i] = 0 ;
    };

    // Organize Pulses by Channel
    std::map<int, std::vector<Pulse> > pulseMap;
    mapPulsesByChannel(pulse_vector, pulseMap, hitCountSlices );      



    // Pretrigger:
    // Compute map of time windows that will possibly read out  
    m_adcConvolutionWindows.clear();
    m_esumConvolutionWindows.clear();
    int adcConvStart, adcConvEnd;
    int esumConvStart, esumConvEnd;
    std::map<int,int>::reverse_iterator sliceIt;

    int hitCount;   // the hit count in 300ns;
    int triggerTime_temp=0;  


////////////////////////////////////////////////////////////
//
//fangxiao add for trigger

    vector<int>::iterator vec_it;
    m_triggerSliceTime.clear();

    //    for(int i=0; i<nTimeSlices; i++){
    //        cout<<"hitCountSlices: " << hitCountSlices[i]<<endl;
    //    }

    for(int i=m_sliceNum; i<nTimeSlices; i++){
        hitCount = 0; 
        //cout<<"test PE: "<< hitCountSlices[i]<<endl;

        for(int j=0; j<=m_sliceNum; j++){
            hitCount += hitCountSlices[i-j];  

        }
        // cout<<"hitCount: "<< hitCount<<endl;
        if(hitCount>m_testTrigger && triggerTime_temp==0){
            m_triggerSliceTime.push_back(i*25); 
            triggerTime_temp = i*25;
        }

        if(hitCount>m_testTrigger && (i*25-triggerTime_temp) > m_postTriggerTime+m_preTriggerTime){
            m_triggerSliceTime.push_back(i*25); 
            triggerTime_temp = i*25;
        }

    }

    for(vec_it = m_triggerSliceTime.begin();
            vec_it != m_triggerSliceTime.end(); vec_it++){
        crate.triggerTime().push_back(*vec_it);
        cout<<"triggerTime: " << *vec_it<<endl;
    }


    //End fangxiao add

    ////////////////////////////////////////////////////////////
    for(int i = 1; i < nTimeSlices; i++){

        if( hitCountSlices[i] + hitCountSlices[i-1] < m_pretriggerThreshold ) continue;

        // cout<<"test PE: "<< hitCountSlices[i] + hitCountSlices[i-1]<<endl;
        if (i - 5 < 0) adcConvStart = 0;
        else adcConvStart = (i - 5);
        if (i - 1 < 0) esumConvStart = 0;
        else esumConvStart = (i - 1);

        if (i + 5 > nTimeSlices) adcConvEnd = nTimeSlices;
        else adcConvEnd = (i + 5);
        if (i + 1 > nTimeSlices) esumConvEnd = nTimeSlices;
        else esumConvEnd = (i + 1);

        if( m_adcConvolutionWindows.empty() ) {
            m_adcConvolutionWindows[adcConvStart] = adcConvEnd;
            m_esumConvolutionWindows[esumConvStart] = esumConvEnd;
            continue;
        }
        sliceIt = m_adcConvolutionWindows.rbegin();
        if( (adcConvStart - sliceIt->second > 2) ) 
            m_adcConvolutionWindows[adcConvStart ] = adcConvEnd;
        else
            m_adcConvolutionWindows[sliceIt->first] = adcConvEnd;

        sliceIt = m_esumConvolutionWindows.rbegin();
        if( (esumConvStart - sliceIt->second > 2) ) 
            m_esumConvolutionWindows[esumConvStart ] = esumConvEnd;
        else                                         
            m_esumConvolutionWindows[sliceIt->first] = esumConvEnd;

    }

    if(debug){
        //    std::cout << "Number of time windows to be possibly read out: " << m_adcConvolutionWindows.size() << std::endl;
    };



    // If no time windows is considered of possible interest, skip the entire event
    if(m_adcConvolutionWindows.size()==0 && m_enablePretrigger) {
        if(debug){
            std::cout << "Event below pretrigger threshold, no waveform simulation" << std::endl;
        };
        return ;
    }



    // Loop over channels, and fill with the simulated signals
    const map<int,JM::ElecFeeChannel>& channelData = crate.channelData();
    map<int,JM::ElecFeeChannel>::const_iterator channelIter,
        done = channelData.end();

    m_IO.set_output_simTimeEarliest(m_simTimeEarliest);


    for(channelIter = channelData.begin(); channelIter != done; ++channelIter){

        int channelId = channelIter->first;
        JM::ElecFeeChannel& channel = crate.channelData()[channelId];

        //Fill each channel with signals
        //    if(pulseMap[channelId].size() > 0)
        if(pulseMap[channelId].size() > 0 )
        {
            //cout<<"pmtID: "<< channelId<<endl;
            generateOneChannel(channelId, pulseMap[channelId], channel, simTime, fsd_vector, m_IO); 

        }


    };  //End loop over channels


    cout<<"SigNum: "<< m_IO.SigIdx()<<endl;
    m_IO.set_SigNum( m_IO.SigIdx() );

    //    m_IO.get_output_tree() -> Fill();

    m_IO.set_output_GlobalEvtID();

    //   updateBoardSignals(crate);

    //   updateEsumSignals(crate);

    std::cout << "END Generate Signal /////////////////////// " << std::endl;


    /*
    // test crate 
    int test_pmtID = 5;
    int tdc_size = crate.m_channelData[test_pmtID].m_tdc.size();
    cout<<"tdc_size: " <<tdc_size<<endl;

    double tdc[tdc_size];
    double adc[tdc_size];
    vector<int>::iterator tdc_it;
    //    for(tdc_it = crate.m_channelData[0].m_tdc.begin();
    //            tdc_it != crate.m_channelData[0].m_tdc.end();
    //            tdc_it++){
    //        cout<<"tdc: " <<*tdc_it<<endl; 
    //    }


    for(int i=0; i<tdc_size; i++){
    tdc[i] = double(crate.m_channelData[test_pmtID].m_tdc[i]);
    //  cout<<"tdc: "<< tdc[i]<<endl;
    adc[i] = crate.m_channelData[test_pmtID].m_adc[i]; 
    //  cout<<"adc: "<< adc[i]<<endl;
    }

    TCanvas* c1 = new TCanvas();
    TGraph* gr1 = new TGraph(tdc_size, tdc, adc);
    gr1 -> Draw("AC");

    c1 -> Print("adc_tdc.png","png");

*/






    return; 
}



void Gen_Signal::loadResponse(){


    double dT_seconds = (1. / m_simFrequency);
    int nShapingSamples = int(1.2*preTimeTolerance*1e-9/dT_seconds);
    int nPulseSamples = int(1.2*preTimeTolerance*1e-9/dT_seconds);
    int nOvershootSamples = int(16000/(dT_seconds*1e9)); // CHLEP::second == 1e9
    AnalogSignal overshootAdc;
    AnalogSignal ringingAdc;
    /*
       std::cout << "sim frequency: " << m_simFrequency << std::endl;
       std::cout << "pretime tolerance: " << preTimeTolerance << std::endl;
       std::cout << "DT : " << dT_seconds << std::endl;
       std::cout << "nPulseSamples: " << nPulseSamples << std::endl;
       std::cout << "nOvershootSamples: " << nOvershootSamples << std::endl;
       std::cout << "nShapingSamples: " << nShapingSamples << std::endl;

*/
    m_pmtPulse.resize(nPulseSamples);
    m_overshoot.resize(nOvershootSamples);
    m_ringing.resize(nOvershootSamples);
    m_shapingResponse.resize(nShapingSamples);
    m_esumResponse.resize(nPulseSamples);
    overshootAdc.resize(nOvershootSamples);
    ringingAdc.resize(nOvershootSamples);
    m_shapingSum = 0;
    m_esumShapingSum = 0;
    m_pulseMax = 0;




    for (int i=0; i<nShapingSamples; i++) {
        m_shapingResponse[i] = pulseShaping(i*dT_seconds);
        m_shapingSum += m_shapingResponse[i];
    }

    for (int i=0; i<nPulseSamples; i++){
        m_esumResponse[i] = esumShaping(i*dT_seconds);
        m_esumShapingSum += m_esumResponse[i];
    }

    for (int i=0; i<nPulseSamples; i++) {
        m_pmtPulse[i] = pmtPulse(i*dT_seconds,1);


    }

    for (int i=0; i<nOvershootSamples; i++) {
        m_overshoot[i] = 0;
        m_ringing[i] = 0;
        // Store overshoot/ringing for raw signal
        if(m_enableOvershoot) m_overshoot[i] += overshoot(i*dT_seconds);
        if(m_enableRinging)   m_ringing[i]   += ringing(i*dT_seconds);
        // Overshoot/ringing in ADC signal after FEE shaping has a different shape
        overshootAdc[i] = 0;
        ringingAdc[i]   = 0;
        if(m_enableOvershoot) overshootAdc[i]+= adcOvershoot(i*dT_seconds);
        if(m_enableRinging)   ringingAdc[i]  += adcRinging(i*dT_seconds);
    }

    // Convolve the pmt pulse, overshoot, and ringing 
    // with the CR-(RC)^4 shaping response
    convolve( m_pmtPulse, m_shapingResponse, m_shapedPmtPulse );
    m_shapedPmtPulse.resize( m_pmtPulse.size() );
    convolve( overshootAdc, m_shapingResponse, m_shapedOvershoot );
    m_shapedOvershoot.resize( m_overshoot.size() );
    convolve( ringingAdc, m_shapingResponse, m_shapedRinging );
    m_shapedRinging.resize( m_ringing.size() );
    // Scale the resulting convolution to preserve the pulse area
    for(unsigned int i=0; i < m_pmtPulse.size(); i++) 
        m_shapedPmtPulse[i] /= m_shapingSum;
    for(unsigned int i=0; i < m_overshoot.size(); i++) 
    {
        m_shapedOvershoot[i] /= m_shapingSum;
        m_shapedRinging[i]  /= m_shapingSum;
    }
    // Dump ideal pulse shapes

    if(verbose){ 
        {
            std::stringstream output;
            output << "pmtPulse = [ ";
            for(int i=0; i<nPulseSamples; i++){
                if(i!=0) output << ", ";
                output << m_pmtPulse[i];
            } 
            output << " ]";
            std::cout << output.str() << std::endl;
        }
        {
            std::stringstream output;
            output << "overshoot = [ ";
            for(int i=0; i<nOvershootSamples; i++){
                if(i!=0) output << ", ";
                output << m_overshoot[i];
            } 
            output << " ]";
            std::cout << output.str() << std::endl;
        }

        {
            std::stringstream output;
            output << "Esum Shaping Pulse = [ ";
            for(int i=0; i<nPulseSamples; i++){
                if(i!=0) output << ", ";
                output << m_esumResponse[i];
            }
            output << " ]";
            std::cout << output.str() << std::endl;
        }

        {
            std::stringstream output;
            output << "shapedPmtPulse = [ ";
            for(int i=0; i<nPulseSamples; i++){
                if(i!=0) output << ", ";
                output << m_shapedPmtPulse[i];
            } 
            output << " ]";
            std::cout << output.str() << std::endl;
        }
        {
            std::stringstream output;
            output << "shapedOvershoot = [ ";
            for(int i=0; i<nOvershootSamples; i++){
                if(i!=0) output << ", ";
                output << m_shapedOvershoot[i];
            } 
            output << " ]";
            std::cout << output.str() << std::endl;
        }

    }

    return;
}





double Gen_Signal::pulseShaping(double deltaT) {
    // Gaussian CR-(RC)^4 pulse shaping
    // See Knoll, Radiation Detection and Measurement
    // Returns the pulse amplitude at time deltaT after a positive step
    // function of amplitude 1
    if (deltaT<0)
        return 0.;
    double t0 = 25.0e-9; // RC time constant in seconds
    double r = deltaT/t0;
    return pow( r, 4 ) * exp( -r );
}



double Gen_Signal::esumShaping(double deltaT) {
    //ESum shaping function
    if (deltaT<0)
        return 0.;
    double t0 = 56.0; // RC time constant in seconds
    double r = deltaT/t0;
    return exp( -r )*(1/t0);
}



double Gen_Signal::pmtPulse(double deltaT, int nPulse) {
    // Return ideal single pe pulse with amplitude 1 V
    double width; // pulse width parameter
    double mu; // parameter determining the degree of asymmetry
    if ( nPulse > 1 ){
        width = getPulseWidth( nPulse); 
        mu    = getPulseForm( nPulse);
    }
    else {
        width = 7.5e-9;
        mu    = 0.45;
    }
    double shift = 6e-9 -width/1.5;
    if (deltaT-shift<0) return 0.;

    //fangxiao test
    //    std::cout<<"pmtPulse: "<<- exp( -pow( log( (deltaT-shift)/width),2) 
    //            / ( 2*pow(mu,2) ) ) << std::endl;

    //
    return - exp( -pow( log( (deltaT-shift)/width),2) 
            / ( 2*pow(mu,2) ) ) ;  // unit V
}



double Gen_Signal::overshoot(double deltaT) {
    if (deltaT < 0) return 0.;
    double amp = 0.045; // Relative overshoot amplitude for spe pulses
    // Fermi onset
    double t0   = 50e-9; 
    double t1   = 10e-9;
    double fermi = 1. / (exp( (t0 - deltaT) / t1) + 1.);
    // Exponential overshoot component
    double tau = 145.e-9; // Overshoot decay time in s
    double expoOS = exp(-(deltaT-87e-9)/tau);
    // Slower overshoot component
    double mean = 0.4e-6;
    double sigma = 0.08e-6;
    double t = deltaT -mean;
    double gausOS = 0.12 * exp(pow(t,2)/(-2*pow(sigma,2)));
    // Undershoot 
    mean = 0.65e-6;
    sigma = 0.12e-6;
    t = deltaT -mean;
    double undershoot = -0.03 * exp(pow(t,2)/(-2*pow(sigma,2)));

    return  amp * fermi * (expoOS + gausOS + undershoot)
        ; //unit V
}




double Gen_Signal::ringing(double deltaT){
    double phase = 1.1e-6; //phase shift in raw signal
    return ringingFast(deltaT) + ringingSlow(deltaT,phase);
}




double Gen_Signal::adcOvershoot(double deltaT) {
    if (deltaT < 0) return 0.;
    // Relative overshoot amplitude
    double amp = 0.015; 
    // Fermi onset
    double t0   = 130e-9; 
    double t1   =10e-9;
    double fermi = 0.7 / (exp( (t0 - deltaT) / t1) + 1.);
    // exponential
    double tau = 500.e-9; 
    double expo = 1.5*exp(-(deltaT)/tau);
    // add 2 Gaussian peaks
    double mean = 0.45e-6;
    double sigma = 0.12e-6;
    double gaus1 = 0.45 * exp(pow(deltaT-mean,2)/(-2*pow(sigma,2)));
    mean = 2e-6;
    sigma = 0.7e-6;
    double gaus2 = 0.12 * exp(pow(deltaT-mean,2)/(-2*pow(sigma,2)));

    return amp * (fermi * expo + gaus1 + gaus2);
}



double Gen_Signal::adcRinging(double deltaT){
    double phase = 3.6e-6; //phase shift in shaped signal
    return ringingFast(deltaT) - ringingSlow(deltaT,phase);
}



void Gen_Signal::convolve(const AnalogSignal& signal,
        const AnalogSignal& response,
        AnalogSignal& output) 
{
    // Primitive convolution of signal and response, to produce
    // output.  Assumes values outside of range are zero.  A few simple
    // optimizations are use to speed up the process.
    int sN = signal.size();
    int rN = response.size();
    output.resize(sN+rN);
    const double* sigD = &(signal[0]);
    const double* resD = &(response[0]);
    double* outD = &(output[0]);
    for (int i=0; i<(sN+rN); i++) {
        double sum = 0;
        for (int j=0; j<rN; j++) {
            if (resD[j]==0)
                continue;
            if (i-j>=0 && i-j<sN)
                sum+=sigD[i-j]*resD[j];
        }
        outD[i]=sum;
    }
}



double Gen_Signal::getPulseWidth(int nPulse){
    // Parameter determining the width of the spe pulse
    double x  = double(nPulse);
    double p0 = 2.92682e+01;
    double p1 = 8.71668e+02;
    double p2 = 7.12771e+02;
    double p3 = 2.98393e-04;

    return (p0/(exp((p1-x)/p2)+1.)+p3*x)* 1e-9;
}



double Gen_Signal::getPulseForm(int nPulse){
    // Parameter determining the asymmetry of the spe pulse
    double x = double(nPulse);
    double p0 =  2.03759e-01;
    double p1 = -8.07750e+02;
    double p2 =  5.18878e-02;
    double p3 = -7.48266e+02;
    double p4 =  2.19051e-01;

    return p0*exp(x/p1) + p2*exp(x/p3) + p4;
}



double Gen_Signal::ringingFast(double deltaT){
    double amp = 0.0016; // relative amplitude
    double tau = 0.8e-6; // decay time
    double t0 = 0.45e-6; // oscillation period
    double phase = 0.55e-6; 
    double t = deltaT - phase;
    if (t<0) return 0;
    return -amp * exp(-t/tau) * sin(2*3.14 *t/t0 )
        ;
}




double Gen_Signal::ringingSlow(double deltaT, double phase){
    double amp = 0.0007; // relative amplitude
    double tau = 2.4e-6; // decay time
    double t0 = 3.6e-6;  // oscillation period
    double t = deltaT - phase;
    if (t<0) return 0;
    return amp * exp(-t/tau) * sin(2*3.14 *t/t0 )
        ;
}



void Gen_Signal::SetSimTime(long double earliest_item,long double latest_item){
    //unit is ns
    m_simTimeEarliest = earliest_item - (long double)(preTimeTolerance);
    m_simTimeLatest = latest_item + (long double)(postTimeTolerance);

}



void Gen_Signal::mapPulsesByChannel( std::vector<Pulse>& pulse_vector, 
        std::map<int, std::vector<Pulse> >& pulseMap, 
        std::vector<int>& hitCountSlices ){

    int timeSliceNo;

    for (std::vector<Pulse>::iterator it=pulse_vector.begin(); 
            it != pulse_vector.end(); ++it) {
        (pulseMap[it->pmtID()]).push_back(*it); 

        timeSliceNo = int(it->pulseHitTime()/m_sliceTime ); //the slice window is 25ns.
        if(timeSliceNo<0){
            timeSliceNo=0; 
        }

        hitCountSlices[timeSliceNo]++;
    }

}


void Gen_Signal::generateOneChannel(int& channelId, 
        std::vector<Pulse>& channelPulses, 
        JM::ElecFeeChannel& channel, 
        double simTime, 
        std::vector<FeeSimData>& fsd_vector,
        Root_IO& m_IO){


    // The number of samples in time given the simulation frequency
    unsigned int simSamples = int(simTime * 1.e-9 * m_simFrequency);

    // Prepare Raw Signal
    if(m_rawSignal.size() != simSamples) m_rawSignal.resize( simSamples );
    // if(m_shapedSignal.size() != simSamples) m_shapedSignal.resize( simSamples );
    double* rawStart = &m_rawSignal[0];
    // double* shapedStart = &m_shapedSignal[0];
    for( unsigned int sigIdx = 0; sigIdx!=simSamples; sigIdx++){
        *(rawStart + sigIdx) = 0;
        //    *(shapedStart + sigIdx) = 0;
    } 


    //fangxiao add the toffset vector
    vector<int>  tOffset_vector;
    tOffset_vector.clear();

    // Add noise to raw signal if requested
    if( m_enableNoise ){
        for(unsigned int index=0; index < simSamples; index++){
            *(rawStart + index) += noise();
        }
    }
    int channelPulsesN = channelPulses.size();

    if( channelPulsesN > 0 ){
        if(verbose){
            std::cout << "Channel " << channelId << " has " << channelPulses.size() 
                << " pulses." << std::endl;
        };


        // Prepare time slots for pulse counting
        int numPulseTimeSlots = int(simTime
                /m_pulseCountSlotWidth) + 1;
        if(verbose){
            std::cout << "Number of time slots for pulse counting = " 
                << numPulseTimeSlots << std::endl; 
        };
        std::vector<int> pulseTimeSlots(numPulseTimeSlots);


        // Fill pulse count time slots
        // Count the number of pulses for each time slot to model nonlinearity
        for (int i = 0; i < numPulseTimeSlots; i++ ) pulseTimeSlots[i] = 0;
        std::vector<Pulse>::iterator pulseIter, pulseDone = channelPulses.end();
        for(pulseIter=channelPulses.begin(); pulseIter != pulseDone; ++pulseIter){
            int timeSlot = int(pulseIter->pulseHitTime() 
                    / m_pulseCountSlotWidth);
            int pulseNo = int(pulseIter->amplitude()+0.5);
            if(timeSlot>=numPulseTimeSlots) continue;
            pulseTimeSlots[timeSlot]+=pulseNo;
            if(verbose){
                std::cout << "Added " << pulseNo << " pulses in time slot " << timeSlot << std::endl;
            };
        }

        double* pmtPulseStart = &m_pmtPulse[0];
        double* overshootStart = &m_overshoot[0];
        double* ringingStart = &m_ringing[0];

        //   double* shapedPmtPulseStart = &m_shapedPmtPulse[0];
        //   double* shapedOvershootStart = &m_shapedOvershoot[0];
        //   double* shapedRingingStart = &m_shapedRinging[0];

        unsigned int nPulseSamples = m_pmtPulse.size();
        unsigned int nOvershootSamples = m_overshoot.size();


        // Loop over pulses, add each to raw signal
        for(pulseIter=channelPulses.begin(); pulseIter != pulseDone; ++pulseIter){
            int tOffset = int(pulseIter->pulseHitTime() * 1.e-9 * m_simFrequency);

            tOffset_vector.push_back(tOffset);

            float amplitude = pulseIter->amplitude() * m_speAmp * m_gainFactor;
            unsigned int nPulse=0;
            unsigned int nCoincPulse=0;
            float satAmp = amplitude;
            float satCharge = amplitude;

            // Count the total number of pulses within a nearby time window
            // This number is used to determine the nonlinearity
            if(channelPulsesN>5){
                int timeSlot = int(pulseIter->pulseHitTime()
                        / m_pulseCountSlotWidth);
                nCoincPulse = pulseTimeSlots[timeSlot];
                for (int iSlot = timeSlot - m_pulseCountWindow; 
                        iSlot <= timeSlot + m_pulseCountWindow; iSlot++){
                    if(iSlot>=0 && iSlot<numPulseTimeSlots) 
                        nPulse += pulseTimeSlots[iSlot];
                }
                // Get saturated amplitudes
                satAmp *= ampSatFactor(nPulse);
                satCharge *= chargeSatFactor(nPulse);
            }else{
                nCoincPulse =1;
                nPulse = 1;
            }
            if(verbose){
                std::cout << "Number of pulses in time window: " << nPulse << std::endl;
            };


            // Cut off overshoot/ringing after 1.5 us for small pe numbers
            unsigned int nSamples = int(3.5 * nPulseSamples);
            // Skip overshoot and ringing if there is only 1 pe 
            // For large pe numbers, add separately per cluster


            //            if(channelPulsesN ==1 || nCoincPulse > 5
            //                    || (!m_enableOvershoot && !m_enableRinging))
            //                nSamples = nPulseSamples;


            /////////////////////////

            // Now add pulses to the raw and shaped signal

            for(unsigned int index = 0; index < nSamples; index++){
                unsigned int sampleIdx = tOffset + index;
                double idxTime = index * (1. / m_simFrequency);
                if(sampleIdx>0 && sampleIdx<simSamples){
                    if(index<nPulseSamples) {
                        if(nPulse>m_linearityThreshold){
                            *(rawStart + sampleIdx) -= satAmp * pmtPulse(idxTime, nPulse);
                        }  else

                            *(rawStart + sampleIdx) -= satCharge * (*(pmtPulseStart + index));
                        //    *(shapedStart + sampleIdx) -= satCharge * (*(shapedPmtPulseStart+index));

                    }
                    // Add overshoot/ringing
                    if((m_enableOvershoot || m_enableRinging) && nCoincPulse < 6){
                        *(rawStart + sampleIdx) -= satCharge * (*(overshootStart+index));
                        *(rawStart + sampleIdx) -= satAmp * (*(ringingStart+index));
                        //     *(shapedStart + sampleIdx) -= satCharge * (*(shapedOvershootStart+index));
                        //     *(shapedStart + sampleIdx) -= satAmp * (*(shapedRingingStart+index));


                    }
                }
            }
        }  // End loop over pulses


        // Add overshoot/ringing for pe cluster
        for (int timeSlot = 0; timeSlot < numPulseTimeSlots; timeSlot++ ) {
            if (pulseTimeSlots[timeSlot] > 5) {

                int nPulse=0;
                for (int iSlot = timeSlot - m_pulseCountWindow; 
                        iSlot <= timeSlot + m_pulseCountWindow; iSlot++){
                    if(iSlot>=0 && iSlot<numPulseTimeSlots) nPulse += pulseTimeSlots[iSlot];
                }
                float satCharge = pulseTimeSlots[timeSlot] * chargeSatFactor(nPulse) * m_speAmp * m_gainFactor;
                float satAmp = pulseTimeSlots[timeSlot] * ampSatFactor(nPulse) * m_speAmp * m_gainFactor;
                int osStartIdx = int((timeSlot) * m_pulseCountSlotWidth * m_simFrequency * 1e-9);

                //tOffset_vector.push_back(osStartIdx);

                //   unsigned int osEnd = nOvershootSamples;
                unsigned int osEnd = 50;  //fangxiao modify 50ns time window to pe cluster
                if( osEnd + osStartIdx > simSamples ) 
                    osEnd = simSamples - osStartIdx;
                for (unsigned int index=0; index<osEnd; index++) {
                    int sampleIdx = osStartIdx + index;
                    *(rawStart + sampleIdx)    -= satCharge * (*(overshootStart+index));
                    *(rawStart + sampleIdx)    -= satAmp    * (*(ringingStart+index));
                    //    *(shapedStart + sampleIdx) -= satCharge * (*(shapedOvershootStart+index));
                    //    *(shapedStart + sampleIdx) -= satAmp    * (*(shapedRingingStart+index));

                }
            }
        }

    } //End addition of pulses to raw signal



    // Convert raw signals to TDC, hit, ADC, Esum values
    // TDC values
    //
    //    int TdcFrequencyHz = BaseFrequencyHz * TdcCycles ; 
    //    int NhitFrequencyHz = BaseFrequencyHz * NhitCycles; 
    //    int AdcFrequencyHz = BaseFrequencyHz * AdcCycles;    

    //    sample( m_rawSignal, m_tdcSignal,
    //            m_simFrequency, TdcFrequencyHz );



    //    std::vector<int> tdcValues;
    //    std::string mode = "kAvove"; 

    //    discriminate( m_tdcSignal, fsd_vector[channelId].m_channelThreshold, tdcValues, 
    //            mode);
    //    channel.m_tdc = tdcValues;
    //    if(verbose){
    //        std::cout << "Channel " << channelId << ": " << tdcValues.size() 
    //            << " TDC values." << std::endl;
    //    };



    // Assemble Hit signal (for trigger) based on TDC clock values
    // The hit signal is a binary 0 or 1 vs. time at the Nhit frequency.
    //    unsigned int hitSamples = convertClock( m_tdcSignal.size(),
    //            TdcFrequencyHz,
    //            NhitFrequencyHz ) + 1;
    //    // Resize and clear the buffer
    //    if( m_hitSignal.size() != hitSamples) m_hitSignal.resize( hitSamples );
    //    int* hitStart = &m_hitSignal[0];
    //    for( unsigned int hitIdx = 0; hitIdx != hitSamples; hitIdx++ ){
    //        *(hitStart+hitIdx) = 0;
    //    }
    //    if( tdcValues.size() > 0 ){
    //        std::vector<int>::const_iterator tdcIter, tdcDone = tdcValues.end();
    //        for(tdcIter = tdcValues.begin(); tdcIter != tdcDone; tdcIter++){
    //            int tdcClock = *tdcIter;
    //            int hitClock = convertClock( tdcClock, TdcFrequencyHz, 
    //                    NhitFrequencyHz );
    //            m_hitSignal[hitClock] = 1;
    //        }
    //    }
    //    channel.m_hit = m_hitSignal;
    //    if(verbose){
    //        std::cout << "Hit signal size: " << m_hitSignal.size() << std::endl;
    //    };


    //  cout<<"OK: "<<__LINE__<<endl;

    // ADC Values: Digitized signals vs. time for both the high and low gains
    // Reduce shaped pulse to ADC frequency
    //    sample( m_shapedSignal, m_adcAnalogSignal,
    //            m_simFrequency, AdcFrequencyHz );
    // 12-bit ADC after shaped signals. One for adcHigh and one for adcLow.
    //    int adcBits = 12;
    //    digitize( m_adcAnalogSignal, m_adcHighGain,
    //            fsd_vector[channelId].m_adcRangeHigh, adcBits, 
    //            fsd_vector[channelId].m_adcBaselineHigh );
    //    digitize( m_adcAnalogSignal, m_adcLowGain,
    //            fsd_vector[channelId].m_adcRangeLow, adcBits, 
    //            fsd_vector[channelId].m_adcBaselineLow );
    //    channel.m_adcHigh = m_adcHighGain ;
    //    channel.m_adcLow = m_adcLowGain ;
    //    if(verbose){
    //        std::cout << "ADC signal size: " << m_adcHighGain.size() << std::endl;
    //    };
    //    int maxAdc = 0;
    //    for (unsigned int jj=0;jj<m_adcHighGain.size();jj++){
    //        if( m_adcHighGain[jj]>maxAdc) maxAdc = m_adcHighGain[jj];
    //    }
    // Channel energy: contribution to the Esum signal 
    //  cout<<"OK: "<<__LINE__<<endl;

    //  cout<<"m_energy_size:  "<< channel.m_energy.size()<<"  m_rawSignal_size: " <<m_rawSignal.size()<<endl; 
    //    cout<<"rawSignal_size: "<< channel.m_energy.size()<<endl;
    //    cout<<"adcSignal_size: "<< channel.m_adcAnalogSignal.size()<<endl;

    //channel.setEnergy(m_shapedSignal);   origin code canceled



    //  use signal_start to save the signal start point
    vector<int> signal_start;
    signal_start.clear();
    int tOffset_temp = 0;

    sort(tOffset_vector.begin(),tOffset_vector.end(),SortBytOffset);

    vector<int>::iterator tOffset_it = tOffset_vector.begin();
    signal_start.push_back(*tOffset_it);
    tOffset_temp = *tOffset_it;

    for(; tOffset_it != tOffset_vector.end(); tOffset_it++){

        if(*tOffset_it - tOffset_temp > m_SignalDistance ){
            signal_start.push_back(*tOffset_it); 
            tOffset_temp = *tOffset_it; 
        }

    }

    AnalogSignal::iterator sig_it ;
    vector<int>::iterator sig_temp, triggerTime_it;

    // for all signal start time
    for(sig_temp = signal_start.begin(); sig_temp != signal_start.end(); sig_temp++){

        for(triggerTime_it = m_triggerSliceTime.begin();
                triggerTime_it != m_triggerSliceTime.end();
                triggerTime_it++){

            // for trigger
            if(*sig_temp > *triggerTime_it - m_preTriggerTime && *sig_temp < *triggerTime_it + m_postTriggerTime){

                m_IO.set_TimeStart(m_IO.SigIdx(), *sig_temp);

                int sample_idx = 0;

                //get output signal
                for(sig_it = m_rawSignal.begin() + *sig_temp ;
                        sig_it != m_rawSignal.begin() + *sig_temp + m_SigTimeWindow; 
                        sig_it++){

                    m_IO.set_SigTotal(m_IO.SigIdx(), sample_idx, *sig_it); 

                    sample_idx++; 
                } 
                m_IO.set_PMT_ID(m_IO.SigIdx(), channelId); 
                m_IO.SigIdx() ++;  // sig ID ++
            }
        }
    }

    //  add raw_waveform to channel
    int m_tdc_temp = 0;
    for(sig_it = m_rawSignal.begin();
            sig_it != m_rawSignal.end(); 
            sig_it++){
        if(*sig_it!=0){
            channel.adc().push_back(*sig_it); 
            channel.tdc().push_back(m_tdc_temp); 
        }
        m_tdc_temp++;
    }
    //    cout<<"tdc size: " <<channel.m_tdc.size()<<endl; 



    return ;

} // End generate One channel 




double Gen_Signal::noise(){

    // FIXME: do discrete FT for range of frequencies
    // Simple noise model
    // Approximate Gaussian-distributed noise
    return gRandom->Gaus(0,1) * m_noiseAmp; // no offset

}




double Gen_Signal::ampSatFactor(int nPulse){
    double q = double(nPulse);
    double qSat = 533.98;
    double a = 2.884;
    return saturationModel(q, qSat, a);

}




double Gen_Signal::chargeSatFactor(int nPulse){
    double q = double(nPulse);
    double qSat = 939.77;
    double a = 2.113;
    return saturationModel(q, qSat, a);
}




void Gen_Signal::sample(const AnalogSignal& signal,
        AnalogSignal& output,
        int inputFrequency,
        int outputFrequency)
{
    // Fill the output array using the signal values sampled at a
    // regular frequency
    if(inputFrequency == outputFrequency){
        if( output.size() != signal.size() ) output.resize( signal.size() );
        memcpy(&output[0],&signal[0],output.size()*sizeof(double));
        return;
    }
    unsigned int nSamples = convertClock(signal.size(), 
            inputFrequency, 
            outputFrequency);
    if(output.size() != nSamples){
        if(output.capacity() > nSamples){
            output.resize(nSamples);
        }else{
            // Faster memory allocation
            AnalogSignal* newSignal = new AnalogSignal(nSamples);
            output.swap(*newSignal);
            delete newSignal;
        }
    }
    if(inputFrequency < outputFrequency){
        if(verbose){
            std::cout << "sample(): Input frequency " << inputFrequency 
                                    << " is less than the sampling frequency " << outputFrequency
                                                                                                 << std::endl;
        };
    }
    if( (inputFrequency % outputFrequency) == 0 ){
        // Check for integer relationship between frequencies
        int relativeFrequency = inputFrequency / outputFrequency;
        double* outputStart = &output[0]; 
        const double* inputStart = &signal[0]; 
        int inputIdx = 0;
        unsigned int outputIdx = 0;
        while(outputIdx != nSamples){
            *(outputStart + outputIdx) = *(inputStart + inputIdx);
            outputIdx++;
            inputIdx+=relativeFrequency;
        }
        return;
    }else{
        // Frequencies not coherent.  Use signal sample with nearest lower index.
        double* outputStart = &output[0];
        const double* inputStart = &signal[0];
        int inputIdx = 0;
        unsigned int outputIdx = 0;
        double freqScale = inputFrequency / outputFrequency;
        while( outputIdx != nSamples ){
            inputIdx = int(outputIdx * freqScale);
            *(outputStart + outputIdx) = *(inputStart + inputIdx);
            outputIdx++;
        }
    }
}




double Gen_Signal::saturationModel(double q, double qSat, double a){
    if(q<1) return 1;
    double num = pow(1 + 8*pow(q/qSat,a),0.5) - 1;
    double denom = 4* pow(q/qSat,a);
    return pow(num/denom,0.5);
}




int Gen_Signal::convertClock(int clock, int inputFrequency, int outputFrequency){
    // Convert a clock cycle from one frequency to another.  Round down
    // for incoherent frequencies.
    return int( ( clock / double(inputFrequency) ) * outputFrequency );
}












