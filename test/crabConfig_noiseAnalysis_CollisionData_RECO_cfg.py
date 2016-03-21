# -----------------------------------------------------------------------------------------------------------------------------
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial  #**Up-to-date**
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile #
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3CheatSheet        #
#
# Environment setup:
#    cmsenv
#    source /cvmfs/cms.cern.ch/crab3/crab.sh
# To submit: 
#    crab submit -c crabConfig_noiseAnalysis_CollisionData_RECO_cfg.py
# To check status: 
#    crab status -d <CRAB-project-directory> [--jobids <comma-separated-list-of-jobs-and/or-job-ranges>]
# To kill jobs:
#    crab kill -d <CRAB-project-directory> [--jobids <comma-separated-list-of-jobs-and/or-job-ranges>]
# To retrieve output:
#    crab getoutput -d <CRAB-project-directory> [--jobids <comma-separated-list-of-jobs-and/or-job-ranges>]
# -----------------------------------------------------------------------------------------------------------------------------
from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromSiteDB


# Select dataset to crab over
number = 0 # starting at 0

# List of datasets
datasetnames = [
'/NoBPTX/Commissioning2016-PromptReco-v1/RECO',
'/MinimumBias/Commissioning2016-PromptReco-v1/RECO',
'/HcalHPDNoise/Commissioning2016-PromptReco-v1/RECO',
'/HcalNZS/Commissioning2016-PromptReco-v1/RECO',
'/Cosmics/Commissioning2016-PromptReco-v1/RECO'
]

# Storage path for output files - EOS specific
storagepath = '/store/user/'+getUsernameFromSiteDB()+'/HCALnoise2016'
#storagepath = '/store/group/dpg_hcal/comm_hcal/Noise/' # DO NOT USE

# cmsRun file
psetname = 'noiseAnalysis_CollisionData_RECO_cfg.py'

# Output filename
OutputFilename = 'results.root'

# Storage site of output files
storageSite = 'T2_CH_CERN'

# White list sites
whiteList = ['']

# Black list sites
blackList = ['']

# -----------------------------------------------------------------------------------------------------------------------------
# No modifications below this line are necessary

import datetime
timestamp = datetime.datetime.now().strftime("_%Y%m%d_%H%M%S")


dataset = filter(None, datasetnames[number].split('/'))

config = config()

config.General.workArea        = "crabNoiseAnalysis"
config.General.requestName     = dataset[0]+'_'+dataset[1]+'_'+dataset[2]+timestamp
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = psetname 
config.JobType.outputFiles = [OutputFilename]
config.JobType.pyCfgParams = ['outputFile='+OutputFilename]


# DATA specific:
config.Data.inputDataset     = datasetnames[number]
config.Data.inputDBS         = 'global'
#config.Data.splitting       = 'FileBased'
#config.Data.unitsPerJob     = 5
config.Data.splitting        = 'LumiBased'
config.Data.unitsPerJob      = 100
config.Data.ignoreLocality   = True
config.Data.outLFNDirBase    = storagepath
config.Data.publication      = False
config.Data.outputDatasetTag = dataset[1]+'_'+dataset[2]+timestamp
#config.Data.lumiMask         = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt'
#config.Data.runRange         = '254231-260627'
#config.Data.runRange        = '260577-260578' #for test
#config.Data.lumiMask        = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_50ns_JSON.txt'
#config.Data.runRange        = '251244-254833'
# -----------------------------------------------------------------------------------------------------------------------------
# JSON files are available at: /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/
# -----------------------------------------------------------------------------------------------------------------------------

config.Site.storageSite = storageSite

if not whiteList:
  config.Site.whitelist = whiteList

if not blackList:
  config.Site.blacklist = blackList

