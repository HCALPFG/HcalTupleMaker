#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_L1GCTJets.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalPFGGeometry.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/DataRecord/interface/L1JetEtScaleRcd.h"


HcalTupleMaker_L1GCTJets::HcalTupleMaker_L1GCTJets(const edm::ParameterSet& iConfig):
  inputTags  (iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("source")),
  tpInputTag (iConfig.getUntrackedParameter<edm::InputTag>("hcalTPs")),
  prefix     (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix     (iConfig.getUntrackedParameter<std::string>("Suffix"))
{
  produces <std::vector<int> >               (prefix + "BX"            + suffix );
  produces <std::vector<std::vector<int> > > (prefix + "TrigPrimIndex" + suffix );
}

void HcalTupleMaker_L1GCTJets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  std::auto_ptr<std::vector<int   > >            bx      ( new std::vector<int>              ());
  std::auto_ptr<std::vector<std::vector<int> > > tpIndex ( new std::vector<std::vector<int> >());
  
  
  edm::ESHandle< L1CaloEtScale > jetScale ;
  iSetup.get< L1JetEtScaleRcd >().get( jetScale ) ;

  int nL1JetCollections = inputTags.size();
  std::vector<edm::Handle<L1GctJetCandCollection> > l1Jets ( nL1JetCollections );

  edm::Handle<HcalTrigPrimDigiCollection> tps;
  iEvent.getByLabel(tpInputTag, tps);

  HcalTrigPrimDigiCollection::const_iterator itp;
  HcalTrigPrimDigiCollection::const_iterator first_tp = tps -> begin();
  
  std::vector<HcalTrigTowerDetId> detids;
  
  for (int iL1JetCollection = 0; iL1JetCollection < nL1JetCollections; ++iL1JetCollection){

    // Get jet collection from event
    iEvent.getByLabel(inputTags[iL1JetCollection], l1Jets[iL1JetCollection]);
    L1GctJetCandCollection::const_iterator l1Jet     = l1Jets[iL1JetCollection] -> begin();
    L1GctJetCandCollection::const_iterator l1Jet_end = l1Jets[iL1JetCollection] -> end();
    
    int i = 0;
    for(; l1Jet != l1Jet_end; ++l1Jet){
      i++;
      bx      -> push_back ( l1Jet -> bx  ());
      tpIndex -> push_back ( std::vector<int>() );
      double et = jetScale -> et ( l1Jet -> rank() );
      std::cout << "\t" << i << "\t" << et << "\t" << l1Jet -> rank() << std::endl;
      size_t last_entry = tpIndex -> size() - 1;
      
      L1CaloRegionDetId regionId = l1Jet -> regionId();
      hcalpfg::getTriggerTowerIDs(regionId.ieta(), regionId.iphi(), detids);
      
      int ndetids = detids.size();
      for (int idetid = 0; idetid < ndetids; ++idetid){
	itp = tps -> find ( detids[idetid] );
	int index = itp - first_tp;
	if ( itp != tps -> end()){
	  (*tpIndex)[last_entry].push_back ( index );
	}
      }
    }
  }

  iEvent.put(bx     , prefix + "BX"            + suffix );
  iEvent.put(tpIndex, prefix + "TrigPrimIndex" + suffix );
  
}
