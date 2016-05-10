import FWCore.ParameterSet.Config as cms

hcalTupleMuonTrack = cms.EDProducer("HcalTupleMaker_MuonTrack",
         #inputMuonCollection     = cms.InputTag("muonsFromCosmics"),
         inputMuonCollection     = cms.InputTag("muons"),
         inputTrackCollection    = cms.InputTag("generalTracks"),
         CSCSegmentsCollection   = cms.InputTag("cscSegments"),                                  
         CSC2DRecHitsCollection  = cms.InputTag("csc2DRecHits"),
         RPCRecHitsCollection    = cms.InputTag("rpcRecHits"),
         DT1DRecHitsCollection   = cms.InputTag("dt1DRecHits"),
         DT1DCosmicRecHitsCollection   = cms.InputTag("dt1DCosmicRecHits"),
         DTRecSegmentsCollection       = cms.InputTag("dt4DSegments"),
         DTRecCosmicSegmentsCollection = cms.InputTag("dt4DCosmicSegments"),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)
