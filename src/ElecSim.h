#ifndef ElecSim_h
#define ElecSim_h

#include "SniperKernel/AlgBase.h"
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
#include "Gen_Pulse.h"
#include "Gen_Signal.h"

#include <map>
#include <list>

namespace JM {
    class ElecFeeCrate;
}

class ElecSimAlg: public AlgBase
{
    public:
        ElecSimAlg(const std::string& name);

        ~ElecSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        Gen_Signal m_gen_signal;
        Gen_Pulse m_gen_pulse;
        Root_IO m_IO;
        Hit_Collection m_hit_vector_Sig;
        Pulse_Collection m_pulse_vector;
      //  ElecFeeCrate m_crate;  //create a crate to save signals
        PmtData_Collection m_pd_vector ;
        FeeSimData_Collection m_fsd_vector;
    private:
        int m_count;
        std::string m_pmtdata_file;

        std::map<std::string, std::string> m_BKtype2filename;

    private:
        // split related
        bool execute_();
        struct m_split_related {
            int count;
            bool is_first;
            std::list<JM::ElecFeeCrate*> buffer;
        } m_split_related;
};

#endif
