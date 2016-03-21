import FWCore.ParameterSet.Config as cms

hcalTupleMuonTrack = cms.EDProducer("HcalTupleMaker_MuonTrack",
         inputMuonCollection  = cms.InputTag("muonsFromCosmics"),
         inputTrackCollection = cms.InputTag("generalTracks"),
         #inputTrackCollection = cms.InputTag("cosmicMuons"),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)
