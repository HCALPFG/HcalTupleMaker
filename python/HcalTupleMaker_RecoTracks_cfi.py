import FWCore.ParameterSet.Config as cms

hcalTupleCosmicMuons = cms.EDProducer("HcalTupleMaker_RecoTracks",
   source = cms.untracked.InputTag("cosmicMuons"),
   Prefix = cms.untracked.string("CosmicMuon"),
   Suffix = cms.untracked.string("")                                 
)                                 
