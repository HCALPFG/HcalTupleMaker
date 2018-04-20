HcalTupleMaker
==============

EDM analyzer for making ntuples from HCAL DIGIs and RecHits:

## How to build it 
``` 
cmsrel CMSSW_10_1_1
cd CMSSW_10_1_1/src/
cmsenv
mkdir HCALPFG
cd HCALPFG
git clone -b 10XY-Noise git@github.com:HCALPFG/HcalTupleMaker.git
cd HcalTupleMaker
cd -
scram b clean; scram b -j9
``` 

## How to run 
To run a config locally, do 
``` 
cd test
cmsRun noiseAnalysis_CollisionData_RECO_cfg.py
cmsRun noiseAnalysis_CollisionData_RAW_cfg.py
``` 

To submit crab jobs, do  
``` 
cd test
crab submit -c crabConfig_noiseAnalysis_CollisionData_RECO_cfg.py
crab submit -c crabConfig_noiseAnalysis_CollisionData_RAW_cfg.py
``` 

Contact: Jae Hyeok Yoo (jae.hyeok.yoo@cern.ch) 

-------------
Original author: Edmund Berry
