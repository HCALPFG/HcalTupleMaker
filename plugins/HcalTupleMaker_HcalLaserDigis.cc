#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalLaserDigis.h"
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
  m_hcalLaserDigiToken = consumes<HcalLaserDigi>(inputTag);
}

void HcalTupleMaker_HcalLaserDigis::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  std::unique_ptr<std::vector<int   > > qadc          ( new std::vector<int>    ());
  std::unique_ptr<std::vector<int   > > tdcHitChannel ( new std::vector<int>    ());
  std::unique_ptr<std::vector<int   > > tdcHitRaw     ( new std::vector<int>    ());
  std::unique_ptr<std::vector<double> > tdcHitNS      ( new std::vector<double> ());

  edm::Handle <HcalLaserDigi> digi;
  iEvent.getByToken(m_hcalLaserDigiToken, digi);

  for (int iQADC = 0; iQADC < nQADC; ++iQADC)
    qadc -> push_back ( digi -> qadc(iQADC) );

  int nTDC = digi -> tdcHits();
  for (int iTDC = 0; iTDC < nTDC; ++iTDC){
    tdcHitChannel -> push_back ( digi -> hitChannel ( iTDC ) );
    tdcHitRaw     -> push_back ( digi -> hitRaw     ( iTDC ) );
    tdcHitNS      -> push_back ( digi -> hitNS      ( iTDC ) );
  }

  std::unique_ptr<int> attenuator1 ( new int(digi -> attenuator1()) );
  std::unique_ptr<int> attenuator2 ( new int(digi -> attenuator2()) );
  std::unique_ptr<int> selector    ( new int(digi -> selector   ()) );

  iEvent.put(move( qadc         ) , prefix + "QADC"          + suffix );
  iEvent.put(move( tdcHitChannel) , prefix + "TDCHitChannel" + suffix );
  iEvent.put(move( tdcHitRaw    ) , prefix + "TDCHitRaw"     + suffix );
  iEvent.put(move( tdcHitNS     ) , prefix + "TDCHitNS"      + suffix );
  iEvent.put(move( attenuator1  ) , prefix + "Attenuator1"   + suffix );
  iEvent.put(move( attenuator2  ) , prefix + "Attenuator2"   + suffix );
  iEvent.put(move( selector     ) , prefix + "Selector"      + suffix );
  
}
