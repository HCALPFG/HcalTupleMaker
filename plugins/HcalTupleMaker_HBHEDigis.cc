#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HBHEDigis.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

HcalTupleMaker_HBHEDigis::HcalTupleMaker_HBHEDigis(const edm::ParameterSet& iConfig) :
  HcalTupleMaker_HcalDigis(iConfig)
{}

void HcalTupleMaker_HBHEDigis::
produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
  
  //-----------------------------------------------------
  // Prepare to put things into event
  //-----------------------------------------------------
  
  loadAlgo();
  
  //-----------------------------------------------------
  // edm::Handles
  //-----------------------------------------------------

  bool run_algo = true;

  edm::Handle<HBHEDigiCollection  >  hbheDigis;
  bool gotHBHEDigis = iEvent.getByLabel(m_hcalDigisTag, hbheDigis);
  if (!gotHBHEDigis ) {
    std::cout << "Could not find HBHE Digis with tag " << m_hcalDigisTag << std::endl;
    run_algo = false;
  }
  
  edm::Handle<HBHERecHitCollection>  hbheRecos;
  if ( m_doEnergyReco ){
    bool gotHBHERecos = iEvent.getByLabel(m_hcalRecHitsTag, hbheRecos);
    if (!gotHBHERecos ) {
      std::cout << "Could not find HBHE Recos with tag " << m_hcalRecHitsTag << std::endl;
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

  if ( run_algo ) algo.run ( *conditions, *hbheDigis, *hbheRecos );
  
  //-----------------------------------------------------
  // Put things into the event
  //-----------------------------------------------------
  
  dumpAlgo(iEvent);
  
}

