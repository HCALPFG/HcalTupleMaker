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
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"

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
  edm::EDGetTokenT<HBHEDigiCollection> hbheToken_;
  edm::EDGetTokenT<HFDigiCollection> hfToken_;
  edm::EDGetTokenT<HcalTrigPrimDigiCollection> tpToken_;
  std::vector<edm::EDGetTokenT<l1extra::L1JetParticleCollection>> l1JetsTokens_;
};
