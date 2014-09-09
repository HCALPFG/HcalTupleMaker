#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HODigis.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

HcalTupleMaker_HODigis::HcalTupleMaker_HODigis(const edm::ParameterSet& iConfig) :
  HcalTupleMaker_HcalDigis(iConfig)
{}

void HcalTupleMaker_HODigis::
produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
  
  //-----------------------------------------------------
  // Prepare to put things into event
  //-----------------------------------------------------
  
  loadAlgo();
  
  //-----------------------------------------------------
  // edm::Handles
  //-----------------------------------------------------

  bool run_algo = true;

  edm::Handle<HODigiCollection  >  hoDigis;
  bool gotHODigis = iEvent.getByLabel(m_hcalDigisTag, hoDigis);
  if (!gotHODigis ) {
    std::cout << "Could not find HO Digis with tag " << m_hcalDigisTag << std::endl;
    run_algo = false;
  }
  
  edm::Handle<HORecHitCollection>  hoRecos;
  if ( m_doEnergyReco ){
    bool gotHORecos = iEvent.getByLabel(m_hcalRecHitsTag, hoRecos);
    if (!gotHORecos ) {
      std::cout << "Could not find HO Recos with tag " << m_hcalRecHitsTag << std::endl;
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

  if ( run_algo ) algo.run ( *conditions, *hoDigis, *hoRecos );
  
  //-----------------------------------------------------
  // Put things into the event
  //-----------------------------------------------------
  
  dumpAlgo(iEvent);
  
}

