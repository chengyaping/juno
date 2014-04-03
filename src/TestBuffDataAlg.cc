#include "TestBuffDataAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "BufWithIO/DataBufSvc.h"
#include "ElecSimClass.h"
#include "Event/ElecHeader.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>



DECLARE_ALGORITHM(TestBuffDataAlg);

    TestBuffDataAlg::TestBuffDataAlg(const std::string& name)
    : AlgBase(name)
    , m_crate(NULL)
    , m_bufsvc(NULL)
                    , m_elec_sim_header_buf(NULL)
{
}

TestBuffDataAlg::~TestBuffDataAlg()
{
}

    bool
TestBuffDataAlg::initialize()
{
    return true;
}

    bool
TestBuffDataAlg::execute()
{
    m_bufsvc = dynamic_cast<DataBufSvcV2*>(service("DataBufSvcV2"));
    // Get Sim Header from Buffer
    m_elec_sim_header_buf = m_bufsvc->getDataBuf<JM::ElecFeeCrate>("/Event/Sim/ElecFeeCrate");
    m_crate = m_elec_sim_header_buf->curEvt();

    int count = m_elec_sim_header_buf->getOutCount();


    int test_pmtID = 0;
    int tdc_size = 0;
    for (int i = 0; i < 10000; ++i) {
         tdc_size = (*m_crate).channelData()[i].tdc().size();
         if (tdc_size>30) {
             test_pmtID = i;
             break;
         }
    }
    cout << "found pmt: " << test_pmtID << endl;
    cout<<"tdc_size: " <<tdc_size<<endl;

    double tdc[tdc_size];
    double adc[tdc_size];
    // vector<int>::iterator tdc_it;
    //    for(tdc_it = crate.m_channelData[0].m_tdc.begin();
    //            tdc_it != crate.m_channelData[0].m_tdc.end();
    //            tdc_it++){
    //        cout<<"tdc: " <<*tdc_it<<endl; 
    //    }


    for(int i=0; i<tdc_size; i++){
        tdc[i] = double((*m_crate).channelData()[test_pmtID].tdc()[i]);
        //  cout<<"tdc: "<< tdc[i]<<endl;
        adc[i] = (*m_crate).channelData()[test_pmtID].adc()[i];
        //  cout<<"adc: "<< adc[i]<<endl;
    }

    TCanvas* c1 = new TCanvas();
    TGraph* gr1 = new TGraph(tdc_size, tdc, adc);
    gr1 -> Draw("AC");

    c1 -> Print(Form("adc_tdc_%d.png", count),"png");







    delete gr1;
    delete c1;
    return true;
}

    bool
TestBuffDataAlg::finalize()
{
    return true;
}
