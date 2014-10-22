import FWCore.ParameterSet.Config as cms

hcalTupleHBHEDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHEDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)

hcalTupleHBHECosmicsDigis = cms.EDProducer("HcalTupleMaker_HBHEDigis",
  source  = cms.untracked.InputTag("hcalCosmicDigis"),
  recHits = cms.untracked.InputTag("hbhereco"),
  Prefix  = cms.untracked.string ( "HBHECosmicDigi"),
  Suffix  = cms.untracked.string ( ""),
  DoChargeReco = cms.untracked.bool ( True ) ,
  DoEnergyReco = cms.untracked.bool ( True ) 
)
