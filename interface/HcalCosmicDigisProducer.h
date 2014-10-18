
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// track association
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

class HcalCosmicDigisProducer : public edm::EDProducer {
 public:
  explicit HcalCosmicDigisProducer(const edm::ParameterSet&);
  ~HcalCosmicDigisProducer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
 private:
  virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  edm::InputTag             m_hbheDigisTag;
  edm::InputTag             m_hoDigisTag;
  edm::InputTag             m_hfDigisTag;
  edm::InputTag             m_recoTracksTag;
  TrackDetectorAssociator   m_trackAssociator;
  TrackAssociatorParameters m_trackParameters;
  edm::ParameterSet         m_trackParameterSet;
};
