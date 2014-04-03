#ifndef ElecSimTopAlg_h
#define ElecSimTopAlg_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/DataBuffer.h"
#include "SniperKernel/RWDataBuffer.h"

namespace JM {
    class SimHeader;
}
class DataBufSvc;

class ElecSimTopAlg: public AlgBase
{
    public:
        ElecSimTopAlg(const std::string& name);
        ~ElecSimTopAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        JM::SimHeader* m_simheader;
    private:
        DataBufSvc* m_bufsvc;
        DataBuffer<JM::SimHeader>* m_sim_header_buf;
};

#endif
