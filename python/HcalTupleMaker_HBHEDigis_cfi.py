import FWCore.ParameterSet.Config as cms

hcalTupleHBHEDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("hbheprereco"),
  Prefix  = cms.untracked.string ( "HBHEDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)
