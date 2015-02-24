import FWCore.ParameterSet.Config as cms

hcalTupleCaloJetMet = cms.EDProducer("HcalTupleMaker_CaloJetMet",
         recoInputTag         = cms.untracked.string("hbherecoMethod2"),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)

hcalTupleCaloJetMetMethod0 = cms.EDProducer("HcalTupleMaker_CaloJetMet",
         recoInputTag         = cms.untracked.string("hbherecoMethod0"),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("Method0")
)



