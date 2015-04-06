#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_Event.h"
#include "FWCore/Framework/interface/Event.h"

HcalTupleMaker_Event::HcalTupleMaker_Event(const edm::ParameterSet& iConfig) {
  produces <unsigned int> ( "run"    );
  produces <unsigned int> ( "event"  );
  produces <unsigned int> ( "ls"     );
  produces <unsigned int> ( "bcn"    );
  produces <unsigned int> ( "orn"    );
}

void HcalTupleMaker_Event::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<unsigned int >  run   ( new unsigned int(iEvent.id().run()        ) );
  std::auto_ptr<unsigned int >  event ( new unsigned int(iEvent.id().event()      ) );
  std::auto_ptr<unsigned int >  ls    ( new unsigned int(iEvent.luminosityBlock() ) );
  std::auto_ptr<unsigned int >  bcn   ( new unsigned int(iEvent.bunchCrossing()   ) );
  std::auto_ptr<unsigned int >  orn   ( new unsigned int(iEvent.orbitNumber()     ) );

  iEvent.put( run,   "run"   );
  iEvent.put( event, "event" );
  iEvent.put( ls   , "ls"    );
  iEvent.put( bcn  , "bcn"   );
  iEvent.put( orn  , "orn"   );

}
