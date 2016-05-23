HcalTupleMaker
==============

EDM analyzer for making ntuples from HCAL DIGIs and RecHits

Validated for:
* CMSSW_7_4_X

To build:
* scram p CMSSW CMSSW_7_3_1_patch2
* mkdir CMSSW_7_3_1_patch2/src/HCALPFG/
* cd CMSSW_7_3_1_patch2/src/HCALPFG/
* cmsenv

For the use of CMSSW_7_4_X:
```
 git clone git@github.com:HCALPFG/HcalTupleMaker.git --branch CMSSW_7_4_X
```
For the use of CMSSW_7_5_X or later, please use master branch:
```
 git clone git@github.com:HCALPFG/HcalTupleMaker.git
```
* scram b -j9


To run:
* cd $CMSSW_BASE/src/HCALPFG/HcalTupleMaker
* cmsenv
* cmsRun analysis_cfg.py

To run local runs:

1)Data of local runs can be found in hcal dpg space on eos with USC_XXXXXX.root
* root://eoscms.cern.ch//eos/cms/store/group/dpg_hcal/comm_hcal/LS1/

2) Check the global tags from:
* https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions

For CMSSW_7_4_10, use 74X_dataRun2_Express_v1. (Tested 16Feb2016)

Maintained by Edmund Berry: Edmund.A.Berry(at)CERN.CH

=======

* CMSSW_8_X_Y

Instructions:
* https://twiki.cern.ch/twiki/bin/viewauth/CMS/HcalNoiseAnalyzer#Recommended_CMSSW_8_0_0_patch1_i

Contact: Halil Saka, David Yu

-------------
Original author: Edmund Berry
