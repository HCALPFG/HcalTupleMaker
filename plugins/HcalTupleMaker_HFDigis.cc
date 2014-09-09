#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HFDigis.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

HcalTupleMaker_HFDigis::HcalTupleMaker_HFDigis(const edm::ParameterSet& iConfig) :
  HcalTupleMaker_HcalDigis(iConfig)
{}

void HcalTupleMaker_HFDigis::
produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
  
  //-----------------------------------------------------
  // Prepare to put things into event
  //-----------------------------------------------------
  
  loadAlgo();
  
  //-----------------------------------------------------
  // edm::Handles
  //-----------------------------------------------------

  bool run_algo = true;

  edm::Handle<HFDigiCollection  >  hfDigis;
  bool gotHFDigis = iEvent.getByLabel(m_hcalDigisTag, hfDigis);
  if (!gotHFDigis ) {
    std::cout << "Could not find HF Digis with tag " << m_hcalDigisTag << std::endl;
    run_algo = false;
  }
  
  edm::Handle<HFRecHitCollection>  hfRecos;
  if ( m_doEnergyReco ){
    bool gotHFRecos = iEvent.getByLabel(m_hcalRecHitsTag, hfRecos);
    if (!gotHFRecos ) {
      std::cout << "Could not find HF Recos with tag " << m_hcalRecHitsTag << std::endl;
      run_algo = false;
    }
  }
  
  //-----------------------------------------------------
  // edm::ESHandles
  //-----------------------------------------------------
  
  edm::ESHandle<HcalDbService> conditions;
  if ( m_doChargeReco ) iSetup.get<HcalDbRecord >().get(conditions);
  
  //-----------------------------------------------------
  // Run the algorithm
  //-----------------------------------------------------

  if ( run_algo ) algo.run ( *conditions, *hfDigis, *hfRecos );
  
  //-----------------------------------------------------
  // Put things into the event
  //-----------------------------------------------------
  
  dumpAlgo(iEvent);
  
}

