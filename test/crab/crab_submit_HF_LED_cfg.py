
# example crab configuration file for one single run

##________________________________________________________________________________||
#Configurables

dataset = '/MinimumBias/Commissioning2016-v1/RAW'
#run = '266150'
run = '267590'

##________________________________________________________________________________||

jobname = dataset[1:].replace('/','_')
jobname = jobname.replace(':','_')
jobname = jobname.replace('RAW','RAW_'+run)
jobname += "_v3"
##________________________________________________________________________________||

from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

##________________________________________________________________________________||

config.General.requestName = jobname
config.General.workArea = 'crab_workarea'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/uscms/home/dryu/HCAL/CMSSW_8_0_1/src/HCALPFG/HcalTupleMaker/test/crab/crab_analysis_HF_LED_cfg.py'
config.JobType.pyCfgParams = ['outputFile=hcalTupleTree.root']

config.Data.inputDataset = dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
config.Data.runRange = run # '193093-194075'
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputDatasetTag = 'PFG_test_HcalTupleMaker'

config.Site.storageSite = "T3_US_FNALLPC"
