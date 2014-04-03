#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import sys

# From PyCintex in ROOT
sys.setdlopenflags( 0x100 | 0x2 )    # RTLD_GLOBAL | RTLD_NOW

import libSniperMgr
import libSniperPython as sp
from MemoryCheck import checkMemory
from FlattenAlg import FlattenAlg

if __name__ == "__main__":

    mgr = libSniperMgr.SniperMgr()
    mgr.setProp("EvtMax", 10)
    mgr.setProp("InputSvc", "NONE")
    mgr.setProp("LogLevel", 0)
    mgr.setProp("Dlls", ["DetSimResultCnv", "ElecSim","BufWithIO"])
    mgr.configure()

    svcMgr = sp.SvcMgr.instance()
    algMgr = sp.AlgMgr.instance()

    # Create Data Buffer Svc
    bufsvc=sp.SvcMgr.get("DataBufSvcV2", True)
    svcMgr.add(bufsvc.name())

    vp = ["/Topology/TopHeader",
          "/Event/Sim/SimHeader",
          "/Event/Calib/CalibHeader",
          "/Event/Sim/ElecFeeCrate"
          ]
    bufsvc.setProp("ValidPaths", vp)
    #op = ["/Event/Calib/CalibHeader"]
    op = ["/Event/Sim/ElecFeeCrate"]
    #op = []
    bufsvc.setProp("OutputItems", op)
    ip = ["/Event/Sim/SimHeader"]
    bufsvc.setProp("InputItems", ip)

    # Create IO Svc
    sios=sp.SvcMgr.get("SniperIOSvc", True)
    svcMgr.add(sios.name())
    sios.setProp("InputFile",["/publicfs/dyb/data/userdata/chengyp/temp/juno/offline/Simulation/ElecSim/share/job0.root"])
    sios.setProp("OutputFile", "ElecFeeCrate.root")

#    pullSimAlg = sp.AlgMgr.get("ElecSimTopAlg", True)
#    algMgr.add(pullSimAlg.name())
    topalg = sp.AlgMgr.get("TopAlgV2",True)

    algMgr.add(topalg.name())

    # create flattern
    FA = FlattenAlg("FA")
    algMgr.reg(FA)
    algMgr.add(FA.name())

    esa = sp.AlgMgr.get("ElecSimAlg/esa",True)

    kvofBKG = {"U":"/path1", "Th":"/path2","K":"/path3"}      #将python里面设置的字典关联到c++中定义的map中
    esa.setProp("BK2File", kvofBKG)
    # algMgr.add(esa.name())

    FA.algs = [esa]

    test_buff = sp.AlgMgr.get("TestBuffDataAlg",True)
    algMgr.add(test_buff.name())

    cm = checkMemory("MemoryCheck")
    cm.interval = 1
    algMgr.add(cm.name())

    if mgr.initialize():
        mgr.run()
    mgr.finalize()
