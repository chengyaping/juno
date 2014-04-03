#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import sys

# From PyCintex in ROOT
sys.setdlopenflags( 0x100 | 0x2 )    # RTLD_GLOBAL | RTLD_NOW

import libSniperMgr
import libSniperPython as sp

if __name__ == "__main__":

    mgr = libSniperMgr.SniperMgr()
    mgr.setProp("EvtMax", 10)
    mgr.setProp("InputSvc", "NONE")
    mgr.setProp("LogLevel", 2)
    mgr.setProp("Dlls", ["DetSimResultCnv", "ElecSim","TopAlg"])
    mgr.configure()

    svcMgr = sp.SvcMgr.instance()
    algMgr = sp.AlgMgr.instance()

    # Create Data Buffer Svc
    bufsvc=sp.SvcMgr.get("DataBufSvc", True)
    svcMgr.add(bufsvc.name())

    vp = ["/Topology/TopHeader",
          "/Event/Sim/SimHeader",
          "/Event/Calib/CalibHeader",
          "/Event/Sim/ElecFeeCrate"
          ]
    bufsvc.setProp("ValidPaths", vp)
    #op = ["/Event/Calib/CalibHeader"]
    op = ["/Event/Sim/ElecFeeCrate"]
    bufsvc.setProp("OutputItems", op)
    ip = ["/Event/Sim/SimHeader"]
    bufsvc.setProp("InputItems", ip)

    # Create IO Svc
    sios=sp.SvcMgr.get("SniperIOSvc", True)
    svcMgr.add(sios.name())
    sios.setProp("InputFile",["simheader.root"])
    sios.setProp("OutputFile", "ElecFeeCrate.root")

#    pullSimAlg = sp.AlgMgr.get("ElecSimTopAlg", True)
#    algMgr.add(pullSimAlg.name())

    topalg = sp.AlgMgr.get("TopAlg",True)
    algMgr.add(topalg.name())

    esa = sp.AlgMgr.get("ElecSimAlg/esa",True)

    kvofBKG = {"U":"/path1", "Th":"/path2","K":"/path3"}      #将python里面设置的字典关联到c++中定义的map中
    esa.setProp("BK2File", kvofBKG)
    algMgr.add(esa.name())

    test_buff = sp.AlgMgr.get("TestBuffDataAlg",True)
    algMgr.add(test_buff.name())

    if mgr.initialize():
        mgr.run()
    mgr.finalize()
