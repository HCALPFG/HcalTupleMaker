#include <algorithm>

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Trigger.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

HcalTupleMaker_Trigger::HcalTupleMaker_Trigger(const edm::ParameterSet& iConfig) :
  l1InputToken   (consumes<L1GlobalTriggerReadoutRecord>(iConfig.getParameter<edm::InputTag>("L1InputTag"))),
  hltInputTag    (iConfig.getParameter<edm::InputTag>("HLTInputTag")),
  hltInputToken  (consumes<edm::TriggerResults>(edm::InputTag(hltInputTag))),
  sourceName     (iConfig.getParameter<std::string>("SourceName")),
  sourceType     (NOT_APPLICABLE),
  hltPrescaleProvider(iConfig, consumesCollector(), *this) 
{
  // Source is either a stream or a dataset (mutually exclusive)
  if (sourceName.length() > 0) {
    if (sourceName.length() >= 2 && sourceName[0]=='S' && sourceName[1]==':') {
      sourceType      = STREAM;
      sourceName      = sourceName.substr(2);
    }
    else if (sourceName.length() >= 3 && sourceName[0]=='D' && sourceName[1]=='S' && sourceName[2]==':') {
      sourceType      = DATASET;
      sourceName      = sourceName.substr(3);
    }
    else throw edm::Exception(edm::errors::Configuration)
	   << "Invalid SourceName = '" << sourceName 
	   << "' -- must start with either 'S:' for streams or 'DS:' for datasets."
	   << std::endl;
  }
  
  produces <std::string> ("HLTKey");

  produces <std::vector<std::string> > ("HLTInsideDatasetTriggerNames"      );
  produces <std::vector<std::string> > ("HLTOutsideDatasetTriggerNames"     );
  produces <std::vector<bool > >       ("HLTInsideDatasetTriggerDecisions"  );
  produces <std::vector<bool > >       ("HLTOutsideDatasetTriggerDecisions" );
  produces <std::vector<int> >         ("HLTInsideDatasetTriggerPrescales"  );
  produces <std::vector<int> >         ("HLTOutsideDatasetTriggerPrescales" );

  produces <std::vector<std::vector<int> > > ( "L1PhysBits" );
  produces <std::vector<std::vector<int> > > ( "L1TechBits" );
  produces <std::vector<int> > ( "HLTBits" );
}


void HcalTupleMaker_Trigger::
printNames(const std::vector<std::string>& names) {
  for (unsigned int i = 0; i < names.size(); ++i)
    edm::LogProblem( "HcalTupleMaker_TriggerProblem" ) << "  " << names[i] << std::endl;
}


void HcalTupleMaker_Trigger::
getDataSource() {
  dataSource.clear();
  if (sourceType == NOT_APPLICABLE) return;
  
  if (sourceType == STREAM) {
    unsigned int  index   = hltConfig.streamIndex(sourceName);
    if (index >= hltConfig.streamNames().size()) {
      edm::LogError( "HcalTupleMaker_TriggerError" ) << "Streams in '" << hltInputTag.process() << "' HLT menu:";
      printNames(hltConfig.streamNames());
      throw edm::Exception(edm::errors::Configuration) << "Stream with name '" << sourceName << "' does not exist." << std::endl;
    }
    dataSource    = hltConfig.streamContent(sourceName);
  }
  else {
    unsigned int  index   = hltConfig.datasetIndex(sourceName);
    if (index >= hltConfig.datasetNames().size()) {
      edm::LogError( "HcalTupleMaker_TriggerError" ) << "Datasets in '" << hltInputTag.process() << "' HLT menu:";
      printNames(hltConfig.datasetNames());
      throw edm::Exception(edm::errors::Configuration) << "Dataset with name '" << sourceName << "' does not exist." << std::endl;
    }
    dataSource    = hltConfig.datasetContent(sourceName);
  }
}


void HcalTupleMaker_Trigger::
beginRun(edm::Run& iRun, const edm::EventSetup& iSetup) {

  //
  // The access to prescales, i.e. hltConfig.prescaleValue(...), seems to have changed going from 750_pre3 to 750_pre5.
  // See: 
  //      http://cmslxr.fnal.gov/lxr/source/HLTrigger/HLTcore/interface/HLTConfigProvider.h?v=CMSSW_7_5_0_pre3#0226
  //      http://cmslxr.fnal.gov/lxr/source/HLTrigger/HLTcore/interface/HLTConfigProvider.h?v=CMSSW_7_5_0_pre5#0222
  // The code now succesfully compiles in 750_pre5, but these values are untested. 
  // Detailed feedback is required.
  // 

  bool changed = true;
  if ( hltConfig.init(iRun, iSetup, hltInputTag.process(), changed) && hltPrescaleProvider.init(iRun, iSetup, hltInputTag.process(), changed)  ) {
    //if (hltConfig.init(iRun, iSetup, hltInputTag.process(), changed)) {
    // if init returns TRUE, initialisation has succeeded!
    edm::LogInfo("HcalTupleMaker_TriggerInfo") << "HLT config with process name " << hltInputTag.process() << " successfully extracted";
  } else {
    // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
    // with the file and/or code and needs to be investigated!
    edm::LogError("HcalTupleMaker_TriggerError") << "Error! HLT config extraction with process name " << hltInputTag.process() << " failed";
    // In this case, all access methods will return empty values!
  }

  getDataSource();
}

void HcalTupleMaker_Trigger::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<std::vector<int> > > l1physbits   ( new std::vector<std::vector<int> >(5) );
  std::auto_ptr<std::vector<std::vector<int> > > l1techbits   ( new std::vector<std::vector<int> >(3) );
  std::auto_ptr<std::vector<int> >               hltbits      ( new std::vector<int>() );

  std::auto_ptr<std::vector < std::string > > v_hlt_insideDataset_names             (new std::vector<std::string>  ());
  std::auto_ptr<std::vector < std::string > > v_hlt_outsideDataset_names            (new std::vector<std::string>  ());
  std::auto_ptr<std::vector < bool > >        v_hlt_insideDataset_decisions         (new std::vector<bool>         ());
  std::auto_ptr<std::vector < bool > >        v_hlt_outsideDataset_decisions        (new std::vector<bool>         ());
  std::auto_ptr<std::vector < int > >         v_hlt_insideDataset_prescales         (new std::vector<int>          ());
  std::auto_ptr<std::vector < int > >         v_hlt_outsideDataset_prescales        (new std::vector<int>          ());

  /*
  std::auto_ptr<std::map<std::string,bool > > m_hlt_insideDataset_namesToDecisions  (new std::map<std::string,bool>());
  std::auto_ptr<std::map<std::string,bool > > m_hlt_outsideDataset_namesToDecisions (new std::map<std::string,bool>());
  std::auto_ptr<std::map<std::string,int  > > m_hlt_insideDataset_namesToPrescales  (new std::map<std::string,int >());
  std::auto_ptr<std::map<std::string,int  > > m_hlt_outsideDataset_namesToPrescales (new std::map<std::string,int >());
  */

  //-----------------------------------------------------------------
  edm::Handle<L1GlobalTriggerReadoutRecord> l1GtReadoutRecord;
  //iEvent.getByLabel(l1InputTag, l1GtReadoutRecord);
  iEvent.getByToken(l1InputToken, l1GtReadoutRecord);

  if(l1GtReadoutRecord.isValid()) {
    edm::LogInfo("HcalTupleMaker_TriggerInfo") << "Successfully obtained " << l1InputTag;

    unsigned int NmaxL1AlgoBit = l1GtReadoutRecord->decisionWord().size();
    unsigned int NmaxL1TechBit = l1GtReadoutRecord->technicalTriggerWord().size();

    for (unsigned int bx = 0; bx < 5; ++bx){
      for (unsigned int i = 0; i < NmaxL1AlgoBit; ++i) {
	l1physbits->at(bx).push_back( l1GtReadoutRecord->decisionWord(bx-2)[i] ? 1 : 0 );
      }
    }
    
    for (unsigned int bx = 0; bx < 3; ++bx){
      for (unsigned int i = 0; i < NmaxL1TechBit; ++i) {
	l1techbits->at(bx).push_back( l1GtReadoutRecord->technicalTriggerWord(bx-1)[i] ? 1 : 0 );
      }
    }
  } else {
    edm::LogError("HcalTupleMaker_TriggerError") << "Error! Can't get the product " << l1InputTag;
  }

  edm::Handle<edm::TriggerResults> triggerResults;
  //iEvent.getByLabel(hltInputTag, triggerResults);
  iEvent.getByToken(hltInputToken, triggerResults);


  if(triggerResults.isValid()) {
    edm::LogInfo("HcalTupleMaker_TriggerInfo") << "Successfully obtained " << hltInputTag;

    const edm::TriggerNames& names = iEvent.triggerNames(*triggerResults);

    for (int i = 0; i < (int) triggerResults->size() ; ++i) {
      if (dataSource.empty() || std::find(dataSource.begin(), dataSource.end(), names.triggerName(i)) != dataSource.end()) {
	v_hlt_insideDataset_names->push_back ( names.triggerName(i) );
	//v_hlt_insideDataset_prescales->push_back ( hltConfig.prescaleValue(iEvent,iSetup,names.triggerName(i)));
	v_hlt_insideDataset_prescales->push_back ( hltConfig.prescaleValue(hltPrescaleProvider.prescaleSet(iEvent,iSetup),names.triggerName(i)) );
	v_hlt_insideDataset_decisions->push_back ( triggerResults->accept(i) );
      } else {
	v_hlt_outsideDataset_names->push_back ( names.triggerName(i) );
	//v_hlt_outsideDataset_prescales->push_back ( hltConfig.prescaleValue(iEvent,iSetup,names.triggerName(i)));
	v_hlt_outsideDataset_prescales->push_back (  hltConfig.prescaleValue(hltPrescaleProvider.prescaleSet(iEvent,iSetup),names.triggerName(i)) );
	v_hlt_outsideDataset_decisions->push_back ( triggerResults->accept(i) );
      }      
    }
    
    for( unsigned int i = 0; i < triggerResults->size(); i++ ){
      hltbits->push_back( triggerResults->at(i).accept() ? 1 : 0 );
    }

  } else {
    edm::LogError("HcalTupleMaker_TriggerError") << "Error! Can't get the product " << hltInputTag;
  }
  
  //-----------------------------------------------------------------
  // put vectors in the event
  //-----------------------------------------------------------------

  iEvent.put( l1physbits, "L1PhysBits" );
  iEvent.put( l1techbits, "L1TechBits" );
  iEvent.put( hltbits,    "HLTBits" );
  
  iEvent.put( std::auto_ptr<std::string>(new std::string(hltConfig.tableName())), "HLTKey");
  
  iEvent.put ( v_hlt_insideDataset_names      , "HLTInsideDatasetTriggerNames"      ) ;
  iEvent.put ( v_hlt_outsideDataset_names     , "HLTOutsideDatasetTriggerNames"     ) ;
  iEvent.put ( v_hlt_insideDataset_decisions  , "HLTInsideDatasetTriggerDecisions"  ) ;
  iEvent.put ( v_hlt_outsideDataset_decisions , "HLTOutsideDatasetTriggerDecisions" ) ;
  iEvent.put ( v_hlt_insideDataset_prescales  , "HLTInsideDatasetTriggerPrescales"  ) ;
  iEvent.put ( v_hlt_outsideDataset_prescales , "HLTOutsideDatasetTriggerPrescales" ) ;

  
}
 
