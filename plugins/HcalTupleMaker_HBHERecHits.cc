#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HBHERecHits.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

HcalTupleMaker_HBHERecHits::HcalTupleMaker_HBHERecHits(const edm::ParameterSet& iConfig) :
  HcalTupleMaker_HcalRecHits(iConfig)
{}

void HcalTupleMaker_HBHERecHits::
produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
  
  //-----------------------------------------------------
  // Prepare to put things into event
  //-----------------------------------------------------
  
  loadAlgo();
  
  //-----------------------------------------------------
  // edm::Handles
  //-----------------------------------------------------

  bool run_algo = true;

  edm::Handle<HBHERecHitCollection > hbheRecHits;
  bool gotHBHERecHits = iEvent.getByLabel(m_hcalRecHitsTag, hbheRecHits);
  if (!gotHBHERecHits ) {
    std::cout << "Could not find HBHE RecHits with tag " << m_hcalRecHitsTag << std::endl;
    run_algo = false;
  }
  
  //-----------------------------------------------------
  // Run the algorithm
  //-----------------------------------------------------

  if ( run_algo ) algo.run ( *hbheRecHits );
  
  //-----------------------------------------------------
  // Put things into the event
  //-----------------------------------------------------
  
  dumpAlgo(iEvent);
  
}

