import FWCore.ParameterSet.Config as cms

cscFilter = cms.EDFilter("cscFilter",
		reverseDecision = cms.bool(False),
		)
