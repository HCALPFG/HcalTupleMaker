#ifndef HcalTupleMaker_HcalTupleFilter_h
#define HcalTupleMaker_HcalTupleFilter_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class HcalTupleFilter : public edm::EDFilter {
   public:
      explicit HcalTupleFilter(const edm::ParameterSet&);
      ~HcalTupleFilter();

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
  protected:
      bool reverse_;
      bool result_;
      virtual void compute(edm::Event&, const edm::EventSetup&);
      //virtual void getCustomParameter(const edm::ParameterSet&);
};

#endif
