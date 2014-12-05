import FWCore.ParameterSet.Config as cms

hcalLaserReco = cms.EDProducer("HcalLaserReco",
  fedRawDataCollectionTag = cms.InputTag("rawDataCollector")
)
