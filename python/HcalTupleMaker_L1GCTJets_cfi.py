import FWCore.ParameterSet.Config as cms

hcalTupleL1GCTJets = cms.EDProducer("HcalTupleMaker_L1GCTJets",
  source  = cms.untracked.VInputTag( cms.InputTag( "gctDigis", "cenJets" ),
                                     cms.InputTag( "gctDigis", "forJets" ),
                                     cms.InputTag( "gctDigis", "tauJets"     )),
  hcalTPs = cms.untracked.InputTag("hcalL1JetDigis"),
  Prefix  = cms.untracked.string("L1Jet"),
  Suffix  = cms.untracked.string("")                                 
)                                 

