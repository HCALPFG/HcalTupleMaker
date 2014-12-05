#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalLaserDigis.h"
#include "DataFormats/HcalDigi/interface/HcalLaserDigi.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

HcalTupleMaker_HcalLaserDigis::HcalTupleMaker_HcalLaserDigis(const edm::ParameterSet& iConfig):
  inputTag   (iConfig.getUntrackedParameter<edm::InputTag>("source")),
  prefix     (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix     (iConfig.getUntrackedParameter<std::string>("Suffix")),
  nQADC      (32)
{
  produces <std::vector<int> >               (prefix + "QADC"          + suffix );
  produces <std::vector<int> >               (prefix + "TDCHitChannel" + suffix );
  produces <std::vector<int> >               (prefix + "TDCHitRaw"     + suffix );
  produces <std::vector<double> >            (prefix + "TDCHitNS"      + suffix );
  produces <int>                             (prefix + "Attenuator1"   + suffix );
  produces <int>                             (prefix + "Attenuator2"   + suffix );
  produces <int>                             (prefix + "Selector"      + suffix );
}

void HcalTupleMaker_HcalLaserDigis::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  std::auto_ptr<std::vector<int   > > qadc          ( new std::vector<int>    ());
  std::auto_ptr<std::vector<int   > > tdcHitChannel ( new std::vector<int>    ());
  std::auto_ptr<std::vector<int   > > tdcHitRaw     ( new std::vector<int>    ());
  std::auto_ptr<std::vector<double> > tdcHitNS      ( new std::vector<double> ());

  edm::Handle <HcalLaserDigi> digi;
  iEvent.getByLabel(inputTag, digi);

  for (int iQADC = 0; iQADC < nQADC; ++iQADC)
    qadc -> push_back ( digi -> qadc(iQADC) );

  int nTDC = digi -> tdcHits();
  for (int iTDC = 0; iTDC < nTDC; ++iTDC){
    tdcHitChannel -> push_back ( digi -> hitChannel ( iTDC ) );
    tdcHitRaw     -> push_back ( digi -> hitRaw     ( iTDC ) );
    tdcHitNS      -> push_back ( digi -> hitNS      ( iTDC ) );
  }

  std::auto_ptr<int> attenuator1 ( new int(digi -> attenuator1()) );
  std::auto_ptr<int> attenuator2 ( new int(digi -> attenuator2()) );
  std::auto_ptr<int> selector    ( new int(digi -> selector   ()) );

  iEvent.put( qadc          , prefix + "QADC"          + suffix ); 
  iEvent.put( tdcHitChannel , prefix + "TDCHitChannel" + suffix ); 
  iEvent.put( tdcHitRaw     , prefix + "TDCHitRaw"     + suffix );  
  iEvent.put( tdcHitNS      , prefix + "TDCHitNS"      + suffix );  
  iEvent.put( attenuator1   , prefix + "Attenuator1"   + suffix );
  iEvent.put( attenuator2   , prefix + "Attenuator2"   + suffix );
  iEvent.put( selector      , prefix + "Selector"      + suffix );
  
}
