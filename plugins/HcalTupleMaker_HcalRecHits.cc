#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalRecHits.h"
#include "FWCore/Framework/interface/Event.h"

HcalTupleMaker_HcalRecHits::HcalTupleMaker_HcalRecHits(const edm::ParameterSet& iConfig) :
  m_hcalRecHitsTag (iConfig.getUntrackedParameter<edm::InputTag>("source")),
  m_prefix         (iConfig.getUntrackedParameter<std::string>  ("Prefix")),
  m_suffix         (iConfig.getUntrackedParameter<std::string>  ("Suffix"))
{
  produces<std::vector<int>   > ( m_prefix + "IEta"   + m_suffix );
  produces<std::vector<int>   > ( m_prefix + "IPhi"   + m_suffix );
  produces<std::vector<int>   > ( m_prefix + "Depth"  + m_suffix );
  produces<std::vector<int>   > ( m_prefix + "Flags"  + m_suffix );
  produces<std::vector<int>   > ( m_prefix + "Aux"    + m_suffix );
  produces<std::vector<float> > ( m_prefix + "Energy" + m_suffix );
  produces<std::vector<float> > ( m_prefix + "Time"   + m_suffix );
}

void HcalTupleMaker_HcalRecHits::loadAlgo(){
  algo.ieta   = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
  algo.iphi   = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
  algo.depth  = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
  algo.flags  = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
  algo.aux    = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
  algo.energy = std::auto_ptr<std::vector<float> > ( new std::vector<float> ());
  algo.time   = std::auto_ptr<std::vector<float> > ( new std::vector<float> ());
}

void HcalTupleMaker_HcalRecHits::dumpAlgo( edm::Event & iEvent ){
  iEvent.put ( algo.ieta   , m_prefix + "IEta"   + m_suffix );
  iEvent.put ( algo.iphi   , m_prefix + "IPhi"   + m_suffix );
  iEvent.put ( algo.depth  , m_prefix + "Depth"  + m_suffix );
  iEvent.put ( algo.flags  , m_prefix + "Flags"  + m_suffix );
  iEvent.put ( algo.aux    , m_prefix + "Aux"    + m_suffix );
  iEvent.put ( algo.energy , m_prefix + "Energy" + m_suffix );
  iEvent.put ( algo.time   , m_prefix + "Time"   + m_suffix );
}
