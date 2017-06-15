import FWCore.ParameterSet.Config as cms

hcalTupleHFPhase1RecHits = cms.EDProducer("HcalTupleMaker_HFPhase1RecHits",
  source = cms.untracked.InputTag("hfprereco"),
  #isHBHE = cms.untracked.bool(False),
  energyThresholdHF = cms.untracked.double(-999.9),
  Prefix = cms.untracked.string  ("HFPhase1RecHit"),
  Suffix = cms.untracked.string  ("")
)
