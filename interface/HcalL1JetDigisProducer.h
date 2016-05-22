// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
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

  std::vector<edm::InputTag> l1_jets_tags_;
  edm::InputTag hbhe_digis_tag_;
  edm::InputTag hf_digis_tag_;
  edm::InputTag tp_digi_tag_;
  edm::EDGetTokenT<HBHEDigiCollection> hbhe_digi_token_;
  edm::EDGetTokenT<HFDigiCollection> hf_digi_token_;
  edm::EDGetTokenT<HcalTrigPrimDigiCollection> tp_digi_token_;
  std::vector<edm::EDGetTokenT<l1extra::L1JetParticleCollection>> l1_jets_tokens_;
};
