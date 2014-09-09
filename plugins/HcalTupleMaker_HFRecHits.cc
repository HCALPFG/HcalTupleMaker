#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HFRecHits.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

HcalTupleMaker_HFRecHits::HcalTupleMaker_HFRecHits(const edm::ParameterSet& iConfig) :
  HcalTupleMaker_HcalRecHits(iConfig)
{}

void HcalTupleMaker_HFRecHits::
produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
  
  //-----------------------------------------------------
  // Prepare to put things into event
  //-----------------------------------------------------
  
  loadAlgo();
  
  //-----------------------------------------------------
  // edm::Handles
  //-----------------------------------------------------

  bool run_algo = true;

  edm::Handle<HFRecHitCollection > hfRecHits;
  bool gotHFRecHits = iEvent.getByLabel(m_hcalRecHitsTag, hfRecHits);
  if (!gotHFRecHits ) {
    std::cout << "Could not find HF RecHits with tag " << m_hcalRecHitsTag << std::endl;
    run_algo = false;
  }
  
  //-----------------------------------------------------
  // Run the algorithm
  //-----------------------------------------------------

  if ( run_algo ) algo.run ( *hfRecHits );
  
  //-----------------------------------------------------
  // Put things into the event
  //-----------------------------------------------------
  
  dumpAlgo(iEvent);
  
}

