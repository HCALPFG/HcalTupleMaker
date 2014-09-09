import FWCore.ParameterSet.Config as cms

hcalTupleHFDigis = cms.EDProducer("HcalTupleMaker_HFDigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("hfreco"),
  Prefix  = cms.untracked.string ( "HFDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)
