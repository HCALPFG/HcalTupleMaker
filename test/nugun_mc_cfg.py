import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('TEST',eras.Run2_2018)

### RANDOM setting (change last digit(s) to make runs different !)
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("SimGeneral.MixingModule.mixNoPU_cfi")
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['phase1_2018_realistic']
###process.GlobalTag.globaltag = "102X_upgrade2018_realistic_v3"  ### if concrete GT


process.load("IOMC.EventVertexGenerators.VtxSmearedGauss_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.g4SimHits.UseMagneticField = False

process.load("DQMServices.Core.DQMStore_cfi")
process.load("DQMServices.Components.MEtoEDMConverter_cfi")

# PFG tuple
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Tree_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_Event_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HBHEDigis_cfi")
process.hcalTupleHBHEDigis.DoEnergyReco = False
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HODigis_cfi")
process.hcalTupleHODigis.DoEnergyReco = False
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HFDigis_cfi")
process.hcalTupleHFDigis.DoEnergyReco = False
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_HcalUnpackerReport_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_QIE10Digis_cfi")
process.load("HCALPFG.HcalTupleMaker.HcalTupleMaker_QIE11Digis_cfi")



process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

process.source = cms.Source("EmptySource")
process.generator = cms.EDProducer("FlatRandomEGunProducer",
    PGunParameters = cms.PSet(
        PartID = cms.vint32(12),
        MinEta = cms.double(-5.0),
        MaxEta = cms.double(5.0),
        MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359),
        MinE   = cms.double(10.0),
        MaxE   = cms.double(10.0)
    ),
    firstRun = cms.untracked.uint32(1),
    AddAntiParticle = cms.bool(False)
)

process.VtxSmeared.SigmaX = 0.00001
process.VtxSmeared.SigmaY = 0.00001
process.VtxSmeared.SigmaZ = 0.00001


#------------- CUSTOMIZATION - replace hbhereco with hbheprereco
delattr(process,"hbhereco")
process.hbhereco = process.hbheprereco.clone()
process.hcalLocalRecoSequence = cms.Sequence(process.hbhereco+process.hfprereco+process.hfreco+process.horeco)


#---------- PATH
# -- NB: for vertex smearing the Label should be: "unsmeared"
# for GEN produced since 760pre6, for older GEN - just "":

process.VtxSmeared.src = cms.InputTag("generator", "unsmeared")
process.generatorSmeared = cms.EDProducer("GeneratorSmearedProducer")
process.g4SimHits.Generator.HepMCProductLabel = cms.InputTag('VtxSmeared')

##-- NZS
process.simHcalDigis.useConfigZSvalues = cms.int32(1)
process.simHcalDigis.HBlevel = cms.int32(-999)
process.simHcalDigis.HElevel = cms.int32(-999)
process.simHcalDigis.HFlevel = cms.int32(-999)
process.simHcalDigis.HOlevel = cms.int32(-999)

# DB HcalSiPMParameters
process.es_ascii = cms.ESSource('HcalTextCalibrations',
    input = cms.VPSet(
      cms.PSet(
        object = cms.string('SiPMParameters'),
        file = cms.FileInPath('HcalSiPMParameters_2018_v6.0_mc.txt')
        #file = cms.FileInPath('HcalSiPMParameters_2018_v0.0_mc.txt')
        ),
      )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')

# DB Pedestal
#process.es_ascii = cms.ESSource('HcalTextCalibrations',
#    input = cms.VPSet(
#      cms.PSet(
#        object = cms.string('SiPMParameters'),
#        file = cms.FileInPath('HcalPedestals_2018_v2.0_mc.txt')
#        ),
#      )
#    )
#process.es_prefer = cms.ESPrefer('HcalTextCalibrations','es_ascii')


# output
process.TFileService = cms.Service("TFileService",
     fileName = cms.string("/eos/cms/store/user/jaehyeok/HcalTupleMaker_ped_mc_50ifb_crosstalk.root")
     #fileName = cms.string("/eos/cms/store/user/jaehyeok/HcalTupleMaker_ped_mc_50ifb.root")
     #fileName = cms.string("/eos/cms/store/user/jaehyeok/HcalTupleMaker_ped_mc_0ifb.root")
)

process.tuple_step = cms.Sequence(
    ### Make HCAL tuples: Event, run, ls number
    process.hcalTupleEvent*
    ### Make HCAL tuples: FED info
#    process.hcalTupleFEDs*
    ### Make HCAL tuples: unpacker info
#    process.hcalTupleUnpackReport*
    ### Make HCAL tuples: digi info
    process.hcalTupleHBHEDigis* # HB digis
    process.hcalTupleHODigis*
    process.hcalTupleQIE10Digis* # HE digis
    process.hcalTupleQIE11Digis* # HF digis
    ### Make HCAL tuples: reco info
#    process.hcalTupleHBHERecHits*
#    process.hcalTupleHFRecHits*
#    process.hcalTupleHORecHits*
#    process.hcalTupleHFRecHits*
    ### Make HCAL tuples: tree
    process.hcalTupleTree
)


process.p = cms.Path(
 process.generator *
 process.VtxSmeared *
 process.generatorSmeared *
 process.g4SimHits *
 process.mix *
 process.ecalDigiSequence *
 process.hcalDigiSequence *
 process.addPileupInfo *
 process.bunchSpacingProducer *
 process.ecalPacker *
 process.esDigiToRaw *
 process.hcalRawData *
 process.rawDataCollector *
 process.ecalDigis *
 process.ecalPreshowerDigis *
 process.hcalDigis *
 process.castorDigis *
 process.calolocalreco *
 process.caloTowersRec *
 process.hcalnoise *
 process.tuple_step
)

### process.outpath = cms.EndPath(process.FEVT)

