#include "ElecSimTopAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/DataBufSvc.h"

#include "Event/SimHeader.h"

DECLARE_ALGORITHM(ElecSimTopAlg);

ElecSimTopAlg::ElecSimTopAlg(const std::string& name)
    : AlgBase(name)
    , m_simheader(NULL)
    , m_bufsvc(NULL)
    , m_sim_header_buf(NULL)
{
}

ElecSimTopAlg::~ElecSimTopAlg()
{
}

bool
ElecSimTopAlg::initialize()
{
    return true;
}

bool
ElecSimTopAlg::execute()
{
    m_bufsvc = dynamic_cast<DataBufSvc*>(service("DataBufSvc"));
    // Get Sim Header from Buffer
    m_sim_header_buf = m_bufsvc->getDataBuf<JM::SimHeader>("/Event/Sim/SimHeader");
    m_simheader = m_sim_header_buf->curEvt();
    return true;
}

bool
ElecSimTopAlg::finalize()
{
    return true;
}
