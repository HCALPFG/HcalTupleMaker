import FWCore.ParameterSet.Config as cms

hcalTupleCaloJetMet = cms.EDProducer("HcalTupleMaker_CaloJetMet",
         #recoInputTag         = cms.untracked.string("hbheplan1"),
         recoInputTag         = cms.untracked.string("hbhereco"),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)
