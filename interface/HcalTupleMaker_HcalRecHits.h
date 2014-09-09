#ifndef HcalTupleMaker_HcalRecHits_h
#define HcalTupleMaker_HcalRecHits_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_RecHitAlgorithm.h"

// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

class HcalTupleMaker_HcalRecHits : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalRecHits(const edm::ParameterSet&);
  
 protected:
  
  HcalTupleMaker_RecHitAlgorithm algo;

  virtual void produce( edm::Event &, const edm::EventSetup & ) = 0;
  void loadAlgo();
  void dumpAlgo( edm::Event & iEvent );
  
  const edm::InputTag   m_hcalRecHitsTag;
  const std::string     m_prefix;
  const std::string     m_suffix;
  
};

#endif
