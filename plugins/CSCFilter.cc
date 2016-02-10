#ifndef HcalTupleMaker_CSCFilter_h
#define HcalTupleMaker_CSCFilter_h

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/EventBase.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"

class CSCFilter : public HcalTupleFilter {
  public:
    explicit CSCFilter(const edm::ParameterSet&);
    ~CSCFilter();

  private:
    virtual void compute(edm::Event&, const edm::EventSetup&);

};

CSCFilter::CSCFilter(const edm::ParameterSet& iConfig) : HcalTupleFilter(iConfig) {

};

CSCFilter::~CSCFilter(){};

void CSCFilter::compute(edm::Event& iEvent, const edm::EventSetup&){

  edm::Handle<CSCSegmentCollection> hCSCSegments;
  iEvent.getByLabel("cscSegments", hCSCSegments); 
  
  if ( hCSCSegments -> size() == 0){
    result_ = true;
  } else {
    result_ = false;
  };
};

DEFINE_FWK_MODULE( CSCFilter );

#endif
