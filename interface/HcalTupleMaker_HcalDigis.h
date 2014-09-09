#ifndef HcalTupleMaker_HcalDigis_h
#define HcalTupleMaker_HcalDigis_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_DigiAlgorithm.h"

// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class HcalTupleMaker_HcalDigis : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalDigis(const edm::ParameterSet&);
  
 protected:
  
  HcalTupleMaker_DigiAlgorithm algo;

  virtual void produce( edm::Event &, const edm::EventSetup & ) = 0;
  void loadAlgo();
  void dumpAlgo( edm::Event & iEvent );
  
  const edm::InputTag   m_hcalDigisTag;
  const edm::InputTag   m_hcalRecHitsTag;
  const std::string     m_prefix;
  const std::string     m_suffix;
  const bool            m_doChargeReco;
  const bool            m_doEnergyReco;

  
};

#endif
