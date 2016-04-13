#ifndef HcalTupleMaker_Trigger_h
#define HcalTupleMaker_Trigger_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"

class HcalTupleMaker_Trigger : public edm::EDProducer {
 public:
  explicit HcalTupleMaker_Trigger(const edm::ParameterSet&);

 private:
  enum DataSource { NOT_APPLICABLE, STREAM, DATASET };
  void produce( edm::Event &, const edm::EventSetup & );
  void beginRun( edm::Run &, const edm::EventSetup & );
  void getDataSource() ;
  void printNames(const std::vector<std::string>& names);
  const edm::InputTag   l1InputTag;
  const edm::InputTag   hltInputTag;
  const std::vector<std::string> hltPathsOfInterest;
  HLTConfigProvider hltConfig;
  HLTPrescaleProvider hltPrescaleProvider_;

  std::string                 sourceName;
  DataSource                  sourceType;
  std::vector<std::string>    dataSource;

};

#endif
