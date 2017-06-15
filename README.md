HcalTupleMaker
==============

EDM analyzer for making ntuples from HCAL DIGIs and RecHits:

## How to build it 
``` 
cmsrel CMSSW_9_2_0_patch5
cd CMSSW_9_2_0_patch5/src/
cmsenv
mkdir HCALPFG
cd HCALPFG
git clone -b 9XY-Noise git@github.com:HCALPFG/HcalTupleMaker.git
cd HcalTupleMaker
git checkout tags/9XY-Noise-v1
cd -
scram b clean; scram b -j9
``` 

## How to run
``` 
cd test
cmsRun noiseAnalysis_CollisionData_RAW_cfg.py
``` 
At this moment, only the `noiseAnalysis_CollisionData_RAW_cfg.py` has been validated in `CMSSW_9_2_0_patch5`.  

Contact: Jae Hyeok Yoo (jae.hyeok.yoo@cern.ch) 

-------------
Original author: Edmund Berry
