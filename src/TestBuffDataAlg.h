#ifndef TestBuffDataAlg_h
#define TestBuffDataAlg_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "BufWithIO/DataBuffer.h"
#include "BufWithIO/RWDataBuffer.h"

class ElecFeeCrate;
class DataBufSvcV2;

namespace JM{
    class ElecFeeCrate;
}
class TestBuffDataAlg: public AlgBase
{
    public:
        TestBuffDataAlg(const std::string& name);
        ~TestBuffDataAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        JM::ElecFeeCrate* m_crate;
    private:
        DataBufSvcV2* m_bufsvc;
        BufWithIO::DataBuffer<JM::ElecFeeCrate>* m_elec_sim_header_buf;
};

#endif
