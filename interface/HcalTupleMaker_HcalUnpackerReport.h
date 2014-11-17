#ifndef HcalTupleMaker_HcalUnpackerReport_h
#define HcalTupleMaker_HcalUnpackerReport_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class HcalTupleMaker_HcalUnpackerReport : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_HcalUnpackerReport(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
