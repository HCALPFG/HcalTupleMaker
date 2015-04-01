#include "HCALPFG/HcalTupleMaker/interface/HcalL1JetDigisProducer.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalPFGGeometry.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"


HcalL1JetDigisProducer::HcalL1JetDigisProducer(const edm::ParameterSet& iConfig):
  m_l1JetsTags       (iConfig.getParameter<std::vector<edm::InputTag> >("L1Jets")),
  m_hbheDigisTag     (iConfig.getParameter<edm::InputTag>("HBHEDigis")),
  m_hfDigisTag       (iConfig.getParameter<edm::InputTag>("HFDigis")),
  m_tpsTag           (iConfig.getParameter<edm::InputTag>("TrigPrims"))
{
  produces<HcalTrigPrimDigiCollection>();
  produces<HBHEDigiCollection>();
  produces<HFDigiCollection>  ();
}

HcalL1JetDigisProducer::~HcalL1JetDigisProducer(){}

void HcalL1JetDigisProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  // Declare ESHandles
  edm::ESHandle<HcalTrigTowerGeometry> geometry;

  // Declare Handles
  edm::Handle<HBHEDigiCollection>         hbheInputDigis;
  edm::Handle<HFDigiCollection>           hfInputDigis;
  edm::Handle<HcalTrigPrimDigiCollection> hcalInputTPs;
  int nL1JetCollections = m_l1JetsTags.size();
  std::vector<edm::Handle<l1extra::L1JetParticleCollection> > l1Jets ( nL1JetCollections );

  // Get ESHandles
  iSetup.get<CaloGeometryRecord>().get(geometry);

  // Get Handles
  iEvent.getByLabel(m_hbheDigisTag, hbheInputDigis);
  iEvent.getByLabel(m_hfDigisTag  , hfInputDigis  );
  iEvent.getByLabel(m_tpsTag      , hcalInputTPs  );

  // Declare output collections
  std::auto_ptr<HBHEDigiCollection>         hbheOutputDigis(new HBHEDigiCollection()); 
  std::auto_ptr<HFDigiCollection>           hfOutputDigis  (new HFDigiCollection());
  std::auto_ptr<HcalTrigPrimDigiCollection> hcalOutputTPs  (new HcalTrigPrimDigiCollection());

  // Get iterators for various input collections
  HcalTrigPrimDigiCollection::const_iterator iTP;
  HBHEDigiCollection::const_iterator         iHBHE;
  HFDigiCollection::const_iterator           iHF;
  
  // Declare arrays of detector IDs
  std::vector<HcalTrigTowerDetId> tpDetIDs;
  std::vector<HcalDetId> hcalDetIDs;

  // Loop over jet collections

  for (int iL1JetCollection = 0; iL1JetCollection < nL1JetCollections; ++iL1JetCollection){

    // Get jet collection from event
    iEvent.getByLabel(m_l1JetsTags[iL1JetCollection], l1Jets[iL1JetCollection]);
    l1extra::L1JetParticleCollection::const_iterator l1Jet     = l1Jets[iL1JetCollection] -> begin();
    l1extra::L1JetParticleCollection::const_iterator l1Jet_end = l1Jets[iL1JetCollection] -> end();

    // Loop over all jets in the collection
    for(; l1Jet != l1Jet_end; ++l1Jet){

      // Get constituent trigger primitives
      tpDetIDs.clear();
      L1CaloRegionDetId regionId = l1Jet -> gctJetCand() -> regionId();
      hcalpfg::getTriggerTowerIDs(regionId.ieta(), regionId.iphi(), tpDetIDs);

      // Loop over trigger primitives
      int ntpDetIDs = tpDetIDs.size();
      for (int itpDetID = 0; itpDetID < ntpDetIDs; ++itpDetID){

	// If we've already collected this TP, then continue
	iTP = hcalOutputTPs -> find ( tpDetIDs[itpDetID] );
	if ( iTP != hcalOutputTPs -> end()) continue;

	// If we can't find this TP, then continue
	iTP = hcalInputTPs  -> find ( tpDetIDs[itpDetID] );
	if ( iTP == hcalInputTPs  -> end()) continue;

	// Otherwise, save this TP
	hcalOutputTPs -> push_back (*iTP);

	// Loop over HCAL det ids
	hcalDetIDs.clear();
	hcalDetIDs = geometry -> detIds( tpDetIDs[itpDetID] );
	int nHcalDetIDs = hcalDetIDs.size();
	
	for (int iHcalDetID = 0; iHcalDetID < nHcalDetIDs; ++iHcalDetID){

	  // Which subdetector? HBHE or HF?
	  HcalSubdetector subdet = hcalDetIDs[iHcalDetID].subdet();
	  
	  if ( subdet == HcalBarrel || subdet == HcalEndcap ){

	    // If we've already collected this digi, continue
	    iHBHE = hbheOutputDigis -> find ( hcalDetIDs[iHcalDetID] );
	    if ( iHBHE != hbheOutputDigis -> end() ) continue;

	    // If we can't find this digi, then continue
	    iHBHE = hbheInputDigis -> find ( hcalDetIDs[iHcalDetID] );
	    if ( iHBHE == hbheInputDigis  -> end() ) continue;

	    // Otherwise, save this digi
	    hbheOutputDigis -> push_back ( *iHBHE );
	  }
	  
	  else if ( subdet == HcalForward ) {
	    
	    // If we've already collected this digi, continue
	    iHF = hfOutputDigis -> find ( hcalDetIDs[iHcalDetID] );
	    if ( iHF != hfOutputDigis -> end() ) continue;

	    // If we can't find this digi, then continue
	    iHF = hfInputDigis -> find ( hcalDetIDs[iHcalDetID] );
	    if ( iHF == hfInputDigis  -> end() ) continue;

	    // Otherwise, save this digi
	    hfOutputDigis -> push_back ( *iHF );
	  }
	} // End loop over HCAL detector IDs
      } // End loop over trigger primitives
    } // End loop over jets
  } // End loop over jet collections

  // Put new collections into the event
  iEvent.put(hcalOutputTPs  );
  iEvent.put(hbheOutputDigis);
  iEvent.put(hfOutputDigis  );
  
}

void HcalL1JetDigisProducer::beginJob(){}

void HcalL1JetDigisProducer::endJob() {}

void HcalL1JetDigisProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
