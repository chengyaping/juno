#ifndef Gen_Signal_h
#define Gen_Signal_h
#include "ElecSimClass.h"
#include "Event/ElecHeader.h"
class Gen_Signal{

    public:
        void initial();
        void loadResponse();
        void generateSignals( std::vector<Pulse>& pulse_vector, JM::ElecFeeCrate& crate,  std::vector<FeeSimData>& fsd_vector, Root_IO& m_IO);

        void SetSimTime(long double earliest_item,long double latest_item);

    private:
        int BaseFrequencyHz;
        int TdcCycles;
        int AdcCycles;
        int EsumCycles;
        int NhitCycles;

        // Trigger Window Cycles
        int m_triggerWindowCycles;
        // Mean PMT gain (in units of 10^7)
        double m_gainFactor;
        // Simulation Frequency (in Hz)
        int m_simFrequency;
        // Esum Frequency (in Hz)
        int m_eSumFrequency;
        // Bool to turn on (true) or off (false) all nonlinear effects
        bool m_enableNonlinearity;
        // Maximum number of pulses before nonlinear effects begin
        double m_linearityThreshold;

        // Bool to turn on (true) or off (false) pretrigger
        bool m_enablePretrigger;
        // Thresholds as set in TrigSim
        double m_eSumTriggerThreshold;
        int m_nHitTriggerThreshold;
        // Pretrigger threshold
        int m_pretriggerThreshold;
        // Bool to individually turn on (true) or off (false) noise
        bool m_enableNoise;
        // Bool to individually turn on (true) or off (false) dynamic single p.e. waveform model
        bool m_enableDynamicWaveform;
        // Bool to individually turn on (true) or off (false) saturation
        bool m_enableSaturation;
        // Bool to individually turn on (true) or off (false) ringing
        bool m_enableRinging;
        // Bool to individually turn on (true) or off (false) overshoot
        bool m_enableOvershoot;
        // Bool to turn on( true) or off (false) simulation of ESum waveforms
        bool m_enableESumTotal;
        bool m_enableESumH;
        bool m_enableESumL;
        // Bool to turn on (true) or off (false) fast simulation mode
        bool m_enableFastSimMode;
        // Pulse counting time slot width.
        double m_pulseCountSlotWidth;
        // Number of counted slots before and after the pulse.
        int m_pulseCountWindow;
        // Voltage amplitude of noise.
        double m_noiseAmp;
        // Spe pulse height in V at 1e7 gain
        double m_speAmp;
        // Factor to scale all channels' discriminator threshold
        double m_discThreshScale;

        double m_adcRange;
        int m_adcBits;
        double m_adcOffset;
        // Ideal PMT pulse shape.
        AnalogSignal m_pmtPulse;
        // Ideal overshoot shape
        AnalogSignal m_overshoot;
        // Ideal ringing shape
        AnalogSignal m_ringing;
        // Ideal CR-(RC)^4 shaped PMT pulse.
        AnalogSignal m_shapedPmtPulse;
        // Ideal CR-(RC)^4 shaped overshoot.
        AnalogSignal m_shapedOvershoot;
        // Ideal CR-(RC)^4 shaped ringing.
        AnalogSignal m_shapedRinging;
        //ESum response after RC shaping
        AnalogSignal m_esumResponse;
        // CR-(RC)^4 shaping response.
        AnalogSignal m_shapingResponse;
        // Area of ideal CR-(RC)^4 shaped PMT pulse.
        double m_shapingSum;
        //Area of shaped ESum pulse
        double m_esumShapingSum;
        // PMT pulse peak amplitude.
        double m_pulseMax;


        bool debug; 
        bool verbose;

        // Transient buffers for signal processing
        DigitalSignal m_sampleIdx;//fangxiao add to save tdc
        AnalogSignal m_Signal;   //fangxiao add to save  adc
        AnalogSignal m_rawSignal;
        AnalogSignal m_esumL;
        AnalogSignal m_esumH;
        AnalogSignal m_esumTotal;
        DigitalSignal m_shapedEsumADC;
        AnalogSignal m_shapedEsumH;
        AnalogSignal m_shapedEsumL;
        AnalogSignal m_shapedEsumTotal;
        AnalogSignal m_discriminatorSignal;
        AnalogSignal m_shapedSignal;
        AnalogSignal m_tdcSignal;
        AnalogSignal m_adcAnalogSignal;
        DigitalSignal m_hitSignal;
        DigitalSignal m_adcHighGain;
        DigitalSignal m_adcLowGain;
        AnalogSignal m_energySignal;
        DigitalSignal m_hitSync;
        DigitalSignal m_hitHold;
        std::map<int,int> m_adcConvolutionWindows;
        std::map<int,int> m_esumConvolutionWindows;

        //fangxiao add

        double preTimeTolerance;
        long double m_simTimeEarliest;
        long double m_simTimeLatest;
        double postTimeTolerance;
        int m_PmtTotal;
        int m_SignalDistance;
        int m_SigTimeWindow;
        int m_testTrigger;
        double m_sliceTime;
        int m_sliceNum;
        vector<int> m_triggerSliceTime;
        int m_preTriggerTime;
        int m_postTriggerTime;
        /////////////////////////////////////////////////////////////


        double pulseShaping(double deltaT); 
        double esumShaping(double deltaT); 
        double pmtPulse(double deltaT, int nPulse);
        double overshoot(double deltaT);
        double ringing(double deltaT);
        double adcOvershoot(double deltaT);
        double adcRinging(double deltaT);
        void convolve(const AnalogSignal& signal,
                const AnalogSignal& response,
                AnalogSignal& output); 
        double getPulseWidth(int nPulse);
        double getPulseForm(int nPulse);
        double ringingFast(double deltaT);
        double ringingSlow(double deltaT, double phase);

        void mapPulsesByChannel( std::vector<Pulse>& pulse_vector, 
                std::map<int, std::vector<Pulse> >& pulseMap, 
                std::vector<int>& hitCountSlices );


        void generateOneChannel(int& channelId, 
                std::vector<Pulse>& channelPulses, 
                JM::ElecFeeChannel& channel, 
                double simTime, 
                std::vector<FeeSimData>& fsd_vector,
                Root_IO& m_IO);


        double noise();

        double ampSatFactor(int nPulse);

        double chargeSatFactor(int nPulse);

        void sample(const AnalogSignal& signal,
                AnalogSignal& output,
                int inputFrequency,
                int outputFrequency);

        double saturationModel(double q, double qSat, double a);

        int convertClock(int clock, int inputFrequency, int outputFrequency);

        ///////////////////////////////////////////////
//fangxiao add: 


};




#endif
