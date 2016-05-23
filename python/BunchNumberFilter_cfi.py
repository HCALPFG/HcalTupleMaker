import FWCore.ParameterSet.Config as cms

bunchNumberFilter = cms.EDFilter("BunchNumberFilter",
		reverseDecision = cms.bool(False),
		selectedBXs = cms.untracked.vuint32(39),
		)
