#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Event.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/EventBase.h"

HcalTupleMaker_Event::HcalTupleMaker_Event(const edm::ParameterSet& iConfig) {
  produces <unsigned int> ( "run"    );
  produces <unsigned int> ( "event"  );
  produces <unsigned int> ( "ls"     );
  produces <unsigned int> ( "bx"     );
  produces <unsigned int> ( "orbit"  );
}

void HcalTupleMaker_Event::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::unique_ptr<unsigned int >  run   ( new unsigned int(iEvent.id().run()        ) );
  std::unique_ptr<unsigned int >  event ( new unsigned int(iEvent.id().event()      ) );
  std::unique_ptr<unsigned int >  ls    ( new unsigned int(iEvent.luminosityBlock() ) );

  edm::EventBase const & eventbase = iEvent;
  std::unique_ptr<unsigned int >  bx    ( new unsigned int(eventbase.bunchCrossing() ) );
  std::unique_ptr<unsigned int >  orbit ( new unsigned int(eventbase.orbitNumber()   ) );
  
  iEvent.put(move( run  ), "run"   );
  iEvent.put(move( event), "event" );
  iEvent.put(move( ls   ), "ls"    );
  iEvent.put(move( bx   ), "bx"    );
  iEvent.put(move( orbit), "orbit" );

}
