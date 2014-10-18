import FWCore.ParameterSet.Config as cms

hcalTupleHODigis = cms.EDProducer("HcalTupleMaker_HODigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("horeco"),
  Prefix  = cms.untracked.string ( "HODigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)

hcalTupleHOCosmicsDigis = cms.EDProducer("HcalTupleMaker_HODigis",
  source  = cms.untracked.InputTag("hcalCosmicDigis"),
  recHits = cms.untracked.InputTag("horeco"),
  Prefix  = cms.untracked.string ( "HOCosmicDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)
