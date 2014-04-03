#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys

# From PyCintex in ROOT
dlflags = sys.getdlopenflags()
sys.setdlopenflags( 0x100 | 0x2 )    # RTLD_GLOBAL | RTLD_NOW       给出了加载动态链接库的方法

import libSniperMgr as SM
import libSniperPython as SP


if __name__ == "__main__":     #如果是直接执行这个脚本则__name__等于__main__ 否则不执行以下程序。  

    mgr = SM.SniperMgr()
    mgr.setProp("EvtMax", 5)
    mgr.setProp("InputSvc", "NONE")
    mgr.setProp("LogLevel", 2)
    mgr.setProp("Dlls", ["ElecSim"])

    mgr.configure()
    algMgr = SP.AlgMgr.instance()
    svcMgr = SP.SvcMgr.instance()
    # TODO

    esa = SP.AlgMgr.get("ElecSimAlg/esa",True)

    kvofBKG = {"U":"/path1", "Th":"/path2","K":"/path3"}      #将python里面设置的字典关联到c++中定义的map中
    esa.setProp("BK2File", kvofBKG)
    algMgr.add(esa.name())

    if mgr.initialize():
        mgr.run()
    mgr.finalize()
