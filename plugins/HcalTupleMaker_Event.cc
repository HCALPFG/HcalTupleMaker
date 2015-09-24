#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Event.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/EventBase.h"

HcalTupleMaker_Event::HcalTupleMaker_Event(const edm::ParameterSet& iConfig) {
  produces <unsigned int> ( "run"    );
  produces <unsigned int> ( "event"  );
  produces <unsigned int> ( "ls"     );
  produces <unsigned int> ( "bx"     );
}

void HcalTupleMaker_Event::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<unsigned int >  run   ( new unsigned int(iEvent.id().run()        ) );
  std::auto_ptr<unsigned int >  event ( new unsigned int(iEvent.id().event()      ) );
  std::auto_ptr<unsigned int >  ls    ( new unsigned int(iEvent.luminosityBlock() ) );

  edm::EventBase const & eventbase = iEvent;
  std::auto_ptr<unsigned int >  bx    ( new unsigned int(eventbase.bunchCrossing() ) );
  
  iEvent.put( run,   "run"   );
  iEvent.put( event, "event" );
  iEvent.put( ls   , "ls"    );
  iEvent.put( bx   , "bx"    );

}
