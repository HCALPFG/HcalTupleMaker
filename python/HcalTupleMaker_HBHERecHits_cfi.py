import FWCore.ParameterSet.Config as cms

hcalTupleHBHERecHits = cms.EDProducer("HcalTupleMaker_HBHERecHits",
  source = cms.untracked.InputTag("hbhereco"),
  Prefix = cms.untracked.string  ("HBHERecHit"),
  Suffix = cms.untracked.string  ("")
)
