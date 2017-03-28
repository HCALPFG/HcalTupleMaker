HcalTupleMaker
==============

EDM analyzer for making ntuples from HCAL DIGIs and RecHits:

## How to build it 
``` 
cmsrel CMSSW_9_0_0_pre5
cd CMSSW_9_0_0_pre5/src/
msenv
mkdir HCALPFG
cd HCALPFG
git clone git@github.com:HCALPFG/HcalTupleMaker.git
cd HcalTupleMaker
git checkout tags/9XY-Noise-v0
cd -
scram b clean; scram b -j9
``` 

## How to run
``` 
cd test
cmsRun noiseAnalysis_CommissioningData_RECO_cfg.py
``` 
At this moment, only the `noiseAnalysis_CommissioningData_RECO_cfg.py` has been validated in `CMSSW_9_0_0_pre5`.  

Contact: Jae Hyeok Yoo (jae.hyeok.yoo@cern.ch) 

-------------
Original author: Edmund Berry
