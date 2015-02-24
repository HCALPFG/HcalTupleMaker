HcalTupleMaker
==============

EDM analyzer for making ntuples from HCAL DIGIs and RecHits

Validated for:
* CMSSW_7_3_X

To build:
* scram p CMSSW CMSSW_7_3_1_patch2
* mkdir CMSSW_7_3_1_patch2/src/HCALPFG/
* cd CMSSW_7_3_1_patch2/src/HCALPFG/
* cmsenv
* git clone git@github.com:HCALPFG/HcalTupleMaker.git
* scram b -j9

To run:
* cd $CMSSW_BASE/src/HCALPFG/HcalTupleMaker
* cmsenv
* cmsRun analysis_cfg.py

Maintained by Edmund Berry: Edmund.A.Berry(at)CERN.CH
