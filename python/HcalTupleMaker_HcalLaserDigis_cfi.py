import FWCore.ParameterSet.Config as cms

hcalTupleLaserDigis = cms.EDProducer("HcalTupleMaker_HcalLaserDigis",
  source  = cms.untracked.InputTag( "hcalLaserReco" ),
  Prefix  = cms.untracked.string("HcalLaserDigi"),
  Suffix  = cms.untracked.string("")                      
)
