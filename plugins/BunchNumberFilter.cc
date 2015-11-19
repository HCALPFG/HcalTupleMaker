#ifndef HcalTupleMaker_BunchNumberFilter_h
#define HcalTupleMaker_BunchNumberFilter_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/EventBase.h"

class BunchNumberFilter : public HcalTupleFilter {
  public:
      explicit BunchNumberFilter(const edm::ParameterSet&);
      ~BunchNumberFilter();

  private:    
      virtual void compute(edm::Event&, const edm::EventSetup&) override;
      //virtual void getCustomParameter(const edm::ParameterSet&) override;

      std::vector< unsigned int > selectedBXs;
      

};

BunchNumberFilter::BunchNumberFilter(const edm::ParameterSet& iConfig) : HcalTupleFilter(iConfig) {
  
  reverse_ = iConfig.getParameter < bool > ("reverseDecision");
  selectedBXs = iConfig.getUntrackedParameter < std::vector < unsigned int > > ("selectedBXs");

};

BunchNumberFilter::~BunchNumberFilter(){};

void BunchNumberFilter::compute(edm::Event& iEvent, const edm::EventSetup&){

  edm::EventBase const & eventbase = iEvent;
  unsigned int bx = (unsigned int)eventbase.bunchCrossing();
  result_ = false;
  std::vector< unsigned int>::iterator it;
  if (std::find( selectedBXs.begin() , selectedBXs.end() , bx ) != selectedBXs.end() ) {
    result_ = true;
  };
};

DEFINE_FWK_MODULE( BunchNumberFilter );

#endif
