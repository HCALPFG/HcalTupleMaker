import FWCore.ParameterSet.Config as cms

from TrackingTools.TrackAssociator.default_cfi import *

hcalCosmicDigis = cms.EDProducer("HcalCosmicDigisProducer",
   TrackAssociatorParameterBlock,
   HBHEDigis  = cms.InputTag("hcalDigis"),
   HODigis    = cms.InputTag("hcalDigis"),
   HFDigis    = cms.InputTag("hcalDigis"),
   RecoTracks = cms.InputTag("cosmicMuons"),
)                                 
