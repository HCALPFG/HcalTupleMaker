
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleFilter.h"

HcalTupleFilter::HcalTupleFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   reverse_ = iConfig.getParameter<bool>("reverseDecision");
   
}


HcalTupleFilter::~HcalTupleFilter()
{
 
}

void HcalTupleFilter::beginJob(){
}

void HcalTupleFilter::endJob(){
}

void HcalTupleFilter::compute(edm::Event& iEvent, const edm::EventSetup& iSetup){};

bool HcalTupleFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){

  compute( iEvent , iSetup );
 
  if (reverse_){
    return !(result_);
  };
  
  return result_;

}


DEFINE_FWK_MODULE( HcalTupleFilter );
