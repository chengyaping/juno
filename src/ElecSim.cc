#include "ElecSim.h"
#include "SniperKernel/AlgFactory.h"
#include "BufWithIO/DataBuffer.h"
#include "BufWithIO/DataBufSvc.h"
#include "Event/SimHeader.h"
#include "Event/ElecHeader.h"
#include "Identifier/CdID.h"
#include <iostream>
#include <cassert>

DECLARE_ALGORITHM(ElecSimAlg);

    ElecSimAlg::ElecSimAlg(const std::string& name)
: AlgBase(name)
{
    declProp("BK2File", m_BKtype2filename);

    m_split_related.count = 0;
    m_split_related.is_first = true;
    m_pmtdata_file = "PmtData.root";

    declProp("PmtDataFile", m_pmtdata_file);
}

ElecSimAlg::~ElecSimAlg()
{
}

bool ElecSimAlg::initialize()
{
    m_gen_pulse.initial();
    m_gen_signal.initial();
    m_count = 0;
//    m_IO.initial_Sig("MC-IBD_eplus-1000-10000.root",  //input root
//            "AdcSignals.root");                       //output root 
    m_IO.initial_Sig();
//    m_IO.initial_BK1("MC-K40-1000000.root", 0.14099e6);  // intput BK root file and BK evt rate, unit Hz
//    m_IO.initial_BK2("MC-Thorium-50000.root", 0.12341e6);
//    m_IO.initial_BK3("MC-Uranium-50000.root", 0.57714e6);

    int PmtTotal = m_gen_pulse.get_PmtTotal();


    m_pd_vector.create_vector(m_pmtdata_file.c_str(),PmtTotal);// create  a PmtData vector
    m_fsd_vector.create_vector(PmtTotal);// create a FeeSimData vector


    int Sig_nEntries = m_IO.get_Sig_nEntries();


    return true;
}

bool ElecSimAlg::execute() {
    //create ElecFeeCrate
    // if we need a full simulation
    if (m_split_related.is_first) {
        m_split_related.is_first = false;
        // Full simulation
        execute_();
        //execute_();
        LogDebug << "After Execute_ twice, m_split_related.count: "
                 << m_split_related.count 
                 << std::endl;
    }

    // yield data
    // put data into buffer
    // if there is no data any more, this should loop without anything.
    if (not m_split_related.count) {
        m_split_related.is_first = true;
        return false;
    }
    --m_split_related.count;

    DataBufSvcV2* m_bufsvc = dynamic_cast<DataBufSvcV2*>(service("DataBufSvcV2"));
    BufWithIO::RWDataBuffer<JM::ElecFeeCrate>* m_elecfeecratebuf = m_bufsvc->getRWDataBuf<JM::ElecFeeCrate>("/Event/Sim/ElecFeeCrate");
    //RWDataBuffer<JM::ElecHeader>* m_elecfeecratebuf = m_bufsvc->getRWDataBuf<JM::ElecHeader>("/Event/Sim/ElecHeader");
    LogDebug<<"Before clear buf , size : "<< m_elecfeecratebuf->totalSize()<<std::endl;
    // TODO: in the future, we will not clear the buffer directly.
    //m_elecfeecratebuf->clear();
    LogDebug<<"After clear buf , size : "<< m_elecfeecratebuf->totalSize()<<std::endl;

    m_elecfeecratebuf->push_back( m_split_related.buffer.front() );
    m_split_related.buffer.pop_front();

    assert( m_split_related.count == m_split_related.buffer.size() );
    return true;
}

bool ElecSimAlg::execute_()
{

    
    int Entry_index_Sig; 
    Entry_index_Sig = m_count;
    cout<<"Event_ID: "<<m_count<<endl;
    //m_IO.get_Sig_tree()->GetEntry(Entry_index_Sig);

    m_IO.set_sig_evt_idx(Entry_index_Sig);
    m_IO.set_output_EventID(Entry_index_Sig);

    m_hit_vector_Sig.reset();


    //get data from buffer 

    DataBufSvcV2* m_bufsvc = dynamic_cast<DataBufSvcV2*>(service("DataBufSvcV2"));
    // Get Sim Header from Buffer
    BufWithIO::DataBuffer<JM::SimHeader>* m_sim_header_buf = m_bufsvc->getDataBuf<JM::SimHeader>("/Event/Sim/SimHeader");
    assert(m_sim_header_buf);
    JM::SimHeader* m_simheader = m_sim_header_buf->curEvt();
    assert(m_simheader);
     
    typedef std::map<int, JM::SimPMTHeader*> PmtId2Head;
    const PmtId2Head& sp = m_simheader->simPMT();

    int Sig_nPhotons = 0;
    vector<int> Sig_pmtID;
    vector<double> Sig_hitTime;

    for(PmtId2Head::const_iterator it = sp.begin(); it!=sp.end(); ++it) {
        int pmtid = it->first;
        JM::SimPMTHeader* pmthead = it->second;

        Sig_nPhotons += pmthead->nPE();
        typedef std::list<JM::SimPMTHit*> HitsCol;
        const HitsCol& simPMTHits = pmthead->simPMTHits();
        for (HitsCol::const_iterator it_hit = simPMTHits.begin(); 
                it_hit != simPMTHits.end(); ++it_hit) {
            JM::SimPMTHit* hit = (*it_hit);
            int hit_pmtId = hit->pmtId();
            double hit_hitTime = hit->hitTime();
            // TODO
//            std::cout << "PMTID (in main):" 
//                      << static_cast<int>(CdID::module(Identifier(hit_pmtId)))
//                      << std::endl;
            Sig_pmtID.push_back(static_cast<int>(CdID::module(Identifier(hit_pmtId))));
            Sig_hitTime.push_back(hit_hitTime);
        }

    }


    m_hit_vector_Sig.create_vector(Sig_nPhotons,
            Sig_pmtID,
            Sig_hitTime,
            m_IO.get_Sig_evt_GlobalTime());

    // loop the Sig sub_hit_vector map: map<int id, vector<Hit> > . 
    // loop a total Sig hit vector ,if two hit has a distance > 10000ns we divide the total Sig hit vector to two sub_vector.
    // the map'key is the sub_vector id, the value is the sub_vector

    for(int i=0; i < m_hit_vector_Sig.get_hit_map().size(); i++){      

        JM::ElecFeeCrate *m_crate = new JM::ElecFeeCrate;  //create a crate to save signals
        //JM::ElecHeader* m_ehd = new JM::ElecHeader;
        //m_ehd->setElecFeeCrate(*m_crate);

        cout<<"subEvtID_Sig: "<< i <<endl;
        cout<<"sub_hit_vector_Sig size: "<<m_hit_vector_Sig.get_sub_vector(i).size()<<endl;        

        m_IO.set_subEvtID(i);// set the subEventID_Sig,e.g mapnum

        m_IO.clear_unit_hit_vector(); //clear the unit hit vector
        m_IO.add_sig_to_unit_hit_vector(m_hit_vector_Sig.get_sub_vector(i));

        //        m_IO.mixed_Sig_BK(m_hit_vector_Sig.get_sub_vector(i));

        //one Sig evt sub_hit_vector should add  many BK , we use the mixed_sub_hit_vector as the minimum unit

        m_pulse_vector.reset();       
        //   m_crate.reset(); //every min unit(sub_mixed_hit_vector) for waveform should clear crate
        m_IO.reset();
        m_IO.set_testTdc(); // just for draw waveform

        if(m_IO.get_unit_hit_vector().size() > 0){

            vector<Hit>::iterator it, unit_hit_vector_first, unit_hit_vector_end ;

            unit_hit_vector_first = m_IO.get_unit_hit_vector().begin();
            unit_hit_vector_end = m_IO.get_unit_hit_vector().end() ;

            long double earliest_item = (*unit_hit_vector_first).hitTime();  // the earliest hitTime  unit ns
            long double latest_item = (*(unit_hit_vector_end-1)).hitTime();  // the latest hitTime
            cout<<"earliest hit time of unit_hit_vector:  " << earliest_item<<endl;
            cout<<"latest hit time of unit_hit_vector:  " << latest_item<<endl;

            m_gen_pulse.SetSimTime(earliest_item,latest_item);
            m_gen_signal.SetSimTime(earliest_item,latest_item);


            m_gen_pulse.generatePulses(m_pulse_vector.get_vector(),
                    m_IO.get_unit_hit_vector(),
                    m_pd_vector.get_vector()
                    );

            cout<<"////////////  END Generate pulse"<<endl;
            ////////////////////////////////////////////////////////////////////////////////////
            m_gen_signal.loadResponse();

            m_gen_signal.generateSignals(m_pulse_vector.get_vector(),*m_crate,m_fsd_vector.get_vector(),m_IO);

        }



        m_split_related.buffer.push_back(m_crate);
        ++m_split_related.count;
        //m_elecfeecratebuf ->push_back(m_crate);
        //LogDebug<<"After pushback to buf , size : "<< m_elecfeecratebuf->totalSize()<<std::endl;
        //m_elecfeecratebuf ->push_back(m_ehd);




    } 

    m_count++;
    return true;
}

bool ElecSimAlg::finalize()
{
    //  m_IO.get_input_file() -> Close();
    //  m_IO.get_output_file() -> Write();
    //  m_IO.get_output_file() -> Close();
    return true;
}
