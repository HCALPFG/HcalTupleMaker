HcalTupleMaker
==============

EDM analyzer for making ntuples from HCAL DIGIs and RecHits:

## How to build it 
``` 
cmsrel CMSSW_9_2_4
cd CMSSW_9_2_4/src/
cmsenv
mkdir HCALPFG
cd HCALPFG
git clone -b 9XY-Noise git@github.com:HCALPFG/HcalTupleMaker.git
cd HcalTupleMaker
git checkout tags/9XY-Noise-v2
cd -
scram b clean; scram b -j9
``` 

## How to run
``` 
cd test
cmsRun noiseAnalysis_CollisionData_RAW_cfg.py
``` 

Contact: Jae Hyeok Yoo (jae.hyeok.yoo@cern.ch) 

-------------
Original author: Edmund Berry
