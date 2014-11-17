#ifndef HcalTupleMaker_FEDs_h
#define HcalTupleMaker_FEDs_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_FEDs : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_FEDs(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
  const int minFEDID;
  const int maxFEDID;
};

#endif
