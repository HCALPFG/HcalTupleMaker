import FWCore.ParameterSet.Config as cms

hcalTupleHORecHits = cms.EDProducer("HcalTupleMaker_HORecHits",
  source = cms.untracked.InputTag("horeco"),
  Prefix = cms.untracked.string  ("HORecHit"),
  Suffix = cms.untracked.string  ("")
)
