// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"

class HcalL1JetDigisProducer : public edm::EDProducer {
 public:
  explicit HcalL1JetDigisProducer(const edm::ParameterSet&);
  ~HcalL1JetDigisProducer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
 private:
  virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  std::vector<edm::InputTag> m_l1JetsTags;
  edm::InputTag m_hbheDigisTag;
  edm::InputTag m_hfDigisTag;
  edm::InputTag m_tpsTag;
  std::vector<edm::EDGetTokenT<l1extra::L1JetParticleCollection> > m_l1JetsTokens;
  edm::EDGetTokenT<HBHEDigiCollection> m_hbheDigisToken;
  edm::EDGetTokenT<HFDigiCollection> m_hfDigisToken;
  edm::EDGetTokenT<HcalTrigPrimDigiCollection> m_tpsToken;
};
