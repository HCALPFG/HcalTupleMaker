import FWCore.ParameterSet.Config as cms

# See: HCALPFG/HcalTupleMaker/python/HcalTupleMaker_HBHERecHits_cfi.py
# for: hbherecoMethod0 

############################################################################################################
# Change input rechit collection to Calo Towers (input to "hcalnoise" module below)                        #
# http://cmslxr.fnal.gov/lxr/source/RecoJets/JetProducers/python/CaloTowerSchemeB_cfi.py                   #
# http://cmslxr.fnal.gov/lxr/source/RecoLocalCalo/CaloTowersCreator/python/calotowermaker_cfi.py           #
############################################################################################################
from RecoJets.JetProducers.CaloTowerSchemeB_cfi import towerMaker
towerMakerMethod0 = towerMaker.clone()
towerMakerMethod0.hbheInput = cms.InputTag("hbherecoMethod0")
towerMakerMethod2 = towerMaker.clone()
towerMakerMethod2.hbheInput = cms.InputTag("hbherecoMethod2")


############################################################################################################
# Change input rechit collection for noise variable calculations                                           #
# http://cmslxr.fnal.gov/lxr/source/RecoMET/METProducers/python/hcalnoiseinfoproducer_cfi.py               #
############################################################################################################
from RecoMET.METProducers.hcalnoiseinfoproducer_cfi import *
hcalnoiseMethod0 = hcalnoise.clone()
hcalnoiseMethod0.recHitCollName = cms.string("hbherecoMethod0") 
hcalnoiseMethod0.caloTowerCollName = cms.string("towerMakerMethod0")
hcalnoiseMethod2 = hcalnoise.clone()
hcalnoiseMethod2.recHitCollName = cms.string("hbherecoMethod2") 
hcalnoiseMethod2.caloTowerCollName = cms.string("towerMakerMethod2")


############################################################################################################
# Change input rechit collection for noise filter result decision                                          #
# http://cmslxr.fnal.gov/lxr/source/CommonTools/RecoAlgos/python/HBHENoiseFilterResultProducer_cfi.py      #
############################################################################################################
from CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi import *
HBHENoiseFilterResultProducerMethod0 = HBHENoiseFilterResultProducer.clone()
HBHENoiseFilterResultProducerMethod0.noiselabel = cms.InputTag("hcalnoiseMethod0")
HBHENoiseFilterResultProducerMethod2 = HBHENoiseFilterResultProducer.clone()
HBHENoiseFilterResultProducerMethod2.noiselabel = cms.InputTag("hcalnoiseMethod2")


hcalTupleHcalNoiseFilters = cms.EDProducer("HcalTupleMaker_HcalNoiseFilters",
         noiseSummaryInputTag = cms.untracked.InputTag("hcalnoiseMethod2"),
         noiseResultInputTag  = cms.untracked.string("HBHENoiseFilterResultProducerMethod2"),
         recoInputTag         = cms.untracked.string("hbherecoMethod2"),
         isRAW  = cms.untracked.bool(True),
         isRECO = cms.untracked.bool(False),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("")
)

hcalTupleHcalNoiseFiltersMethod0 = cms.EDProducer("HcalTupleMaker_HcalNoiseFilters",
         noiseSummaryInputTag = cms.untracked.InputTag("hcalnoiseMethod0"),
         noiseResultInputTag  = cms.untracked.string("HBHENoiseFilterResultProducerMethod0"),
         recoInputTag         = cms.untracked.string("hbherecoMethod0"),
         isRAW  = cms.untracked.bool(True),
         isRECO = cms.untracked.bool(False),
         Prefix = cms.untracked.string(""),
         Suffix = cms.untracked.string("Method0")
)

