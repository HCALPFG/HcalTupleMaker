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
  m_l1JetsTag        (iConfig.getParameter<edm::InputTag>("L1Jets")),
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

  edm::ESHandle<HcalTrigTowerGeometry> geometry;
  iSetup.get<CaloGeometryRecord>().get(geometry);
  
  edm::Handle<l1extra::L1JetParticleCollection> l1Jets;
  iEvent.getByLabel(m_l1JetsTag, l1Jets);

  l1extra::L1JetParticleCollection::const_iterator l1Jet     = l1Jets -> begin();
  l1extra::L1JetParticleCollection::const_iterator l1Jet_end = l1Jets -> end();
  
  edm::Handle<HBHEDigiCollection> hbheInputDigis;
  iEvent.getByLabel(m_hbheDigisTag, hbheInputDigis);
  
  edm::Handle<HFDigiCollection> hfInputDigis;
  iEvent.getByLabel(m_hfDigisTag, hfInputDigis);

  edm::Handle<HcalTrigPrimDigiCollection> hcalInputTPs;
  iEvent.getByLabel(m_tpsTag, hcalInputTPs);

  HcalTrigPrimDigiCollection::const_iterator iTP;
  HBHEDigiCollection::const_iterator iHBHE;
  HFDigiCollection::const_iterator iHF;
  
  std::auto_ptr<HBHEDigiCollection>         hbheOutputDigis(new HBHEDigiCollection()); 
  std::auto_ptr<HFDigiCollection>           hfOutputDigis  (new HFDigiCollection());
  std::auto_ptr<HcalTrigPrimDigiCollection> hcalOutputTPs  (new HcalTrigPrimDigiCollection());
  
  std::vector<HcalTrigTowerDetId> tpDetIDs;
  std::vector<HcalDetId> hcalDetIDs;
  
  for(; l1Jet != l1Jet_end; ++l1Jet){

    tpDetIDs.clear();
    L1CaloRegionDetId regionId = l1Jet -> gctJetCand() -> regionId();
    hcalpfg::getTriggerTowerIDs(regionId.ieta(), regionId.iphi(), tpDetIDs);

    int ntpDetIDs = tpDetIDs.size();
    for (int itpDetID = 0; itpDetID < ntpDetIDs; ++itpDetID){
      iTP = hcalInputTPs -> find ( tpDetIDs[itpDetID] );
      if ( iTP == hcalInputTPs -> end()) continue;

      hcalOutputTPs -> push_back (*iTP);

      hcalDetIDs.clear();
      hcalDetIDs = geometry -> detIds( tpDetIDs[itpDetID] );
      int nHcalDetIDs = hcalDetIDs.size();

      for (int iHcalDetID = 0; iHcalDetID < nHcalDetIDs; ++iHcalDetID){

	HcalSubdetector subdet = hcalDetIDs[iHcalDetID].subdet();
	
	if ( subdet == HcalBarrel || subdet == HcalEndcap ){
	  iHBHE = hbheInputDigis -> find ( hcalDetIDs[iHcalDetID] );
	  if ( iHBHE == hbheInputDigis -> end() ) continue;
	  hbheOutputDigis -> push_back ( *iHBHE );
	}
	
	else if ( subdet == HcalForward ) {
	  iHF = hfInputDigis -> find ( hcalDetIDs[iHcalDetID] );
	  if ( iHF == hfInputDigis -> end() ) continue;
	  hfOutputDigis -> push_back ( *iHF );
	}
	
      }
    }
  }

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
