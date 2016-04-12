import FWCore.ParameterSet.Config as cms

hcalTupleHORecHits = cms.EDProducer("HcalTupleMaker_HORecHits",
  source = cms.untracked.InputTag("horeco"),
  isHBHE = cms.untracked.bool(False),
  Prefix = cms.untracked.string  ("HORecHit"),
  Suffix = cms.untracked.string  ("")
)
