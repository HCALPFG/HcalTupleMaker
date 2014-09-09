#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HORecHits.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

HcalTupleMaker_HORecHits::HcalTupleMaker_HORecHits(const edm::ParameterSet& iConfig) :
  HcalTupleMaker_HcalRecHits(iConfig)
{}

void HcalTupleMaker_HORecHits::
produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
  
  //-----------------------------------------------------
  // Prepare to put things into event
  //-----------------------------------------------------
  
  loadAlgo();
  
  //-----------------------------------------------------
  // edm::Handles
  //-----------------------------------------------------

  bool run_algo = true;

  edm::Handle<HORecHitCollection > hoRecHits;
  bool gotHORecHits = iEvent.getByLabel(m_hcalRecHitsTag, hoRecHits);
  if (!gotHORecHits ) {
    std::cout << "Could not find HO RecHits with tag " << m_hcalRecHitsTag << std::endl;
    run_algo = false;
  }
  
  //-----------------------------------------------------
  // Run the algorithm
  //-----------------------------------------------------

  if ( run_algo ) algo.run ( *hoRecHits );
  
  //-----------------------------------------------------
  // Put things into the event
  //-----------------------------------------------------
  
  dumpAlgo(iEvent);
  
}

