import FWCore.ParameterSet.Config as cms

hcalTupleHcalIsoNoiseFilterParameters = cms.EDProducer("HcalTupleMaker_HcalIsoNoiseFilterParameters",
         noiseSummaryInputTag = cms.untracked.InputTag("hcalnoise"),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)
