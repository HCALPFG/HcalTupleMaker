#ifndef HcalTupleMaker_HcalRecHits_h
#define HcalTupleMaker_HcalRecHits_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalRecHitAlgorithm.h"

template <class RecHitCollection> 
class HcalTupleMaker_HcalRecHits : public edm::EDProducer {
 protected:
  
  const edm::InputTag   m_hcalRecHitsTag;

  edm::EDGetTokenT<RecHitCollection> m_hcalRecHitsToken;
  const bool            m_isHBHE;
  const double          m_energyThresholdHFHO;
  const std::string     m_prefix;
  const std::string     m_suffix;

  HcalTupleMaker_HcalRecHitAlgorithm algo;
  
  void produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
    
    //-----------------------------------------------------
    // Prepare to put things into event
    //-----------------------------------------------------
    
    loadAlgo();
    
    //-----------------------------------------------------
    // edm::Handles
    //-----------------------------------------------------
    
    bool run_algo = true;

    edm::Handle<RecHitCollection> hcalRecHits;
    bool gotHcalRecHits = iEvent.getByToken(m_hcalRecHitsToken, hcalRecHits);
    
    if (!gotHcalRecHits ) {
      std::cout << "Could not find HCAL RecHits with tag " << m_hcalRecHitsTag << std::endl;
      run_algo = false;
    }
    
    //-----------------------------------------------------
    // edm::ESHandles
    //-----------------------------------------------------
    
    edm::ESHandle<CaloGeometry> geometry;
    iSetup.get<CaloGeometryRecord>().get(geometry);
    
    //-----------------------------------------------------
    // Run the algorithm
    //-----------------------------------------------------
    
    if ( run_algo ) algo.run ( *hcalRecHits, *geometry, m_isHBHE, m_energyThresholdHFHO );
    
    //-----------------------------------------------------
    // Put things into the event
    //-----------------------------------------------------
    
    dumpAlgo(iEvent);
    
  }
  
 public:
  
 HcalTupleMaker_HcalRecHits(const edm::ParameterSet& iConfig) :
    m_hcalRecHitsTag      (iConfig.getUntrackedParameter<edm::InputTag>("source")),
    m_hcalRecHitsToken    (consumes<RecHitCollection>(iConfig.getUntrackedParameter<edm::InputTag>("source"))),
    m_isHBHE              (iConfig.getUntrackedParameter<bool>("isHBHE")),
    m_energyThresholdHFHO (iConfig.getUntrackedParameter<double>("energyThresholdHFHO")),
    m_prefix              (iConfig.getUntrackedParameter<std::string>  ("Prefix")),
    m_suffix              (iConfig.getUntrackedParameter<std::string>  ("Suffix")) {
    produces<std::vector<int>   > ( m_prefix + "IEta"   + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "IPhi"   + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Eta"    + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Phi"    + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "Depth"  + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "RBXid"  + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "HPDid"  + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "Flags"  + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "Aux"    + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Energy" + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Time"   + m_suffix );
  }

 protected:

  void loadAlgo(){
    algo.ieta   = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.iphi   = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.eta    = std::auto_ptr<std::vector<float> > ( new std::vector<float> ());
    algo.phi    = std::auto_ptr<std::vector<float> > ( new std::vector<float> ());
    algo.depth  = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.rbxid  = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.hpdid  = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.flags  = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.aux    = std::auto_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.energy = std::auto_ptr<std::vector<float> > ( new std::vector<float> ());
    algo.time   = std::auto_ptr<std::vector<float> > ( new std::vector<float> ());
  }
  
  void dumpAlgo( edm::Event & iEvent ){
    iEvent.put ( algo.ieta   , m_prefix + "IEta"   + m_suffix );
    iEvent.put ( algo.iphi   , m_prefix + "IPhi"   + m_suffix );
    iEvent.put ( algo.eta    , m_prefix + "Eta"    + m_suffix );
    iEvent.put ( algo.phi    , m_prefix + "Phi"    + m_suffix );
    iEvent.put ( algo.depth  , m_prefix + "Depth"  + m_suffix );
    iEvent.put ( algo.rbxid  , m_prefix + "RBXid"  + m_suffix );
    iEvent.put ( algo.hpdid  , m_prefix + "HPDid"  + m_suffix );
    iEvent.put ( algo.flags  , m_prefix + "Flags"  + m_suffix );
    iEvent.put ( algo.aux    , m_prefix + "Aux"    + m_suffix );
    iEvent.put ( algo.energy , m_prefix + "Energy" + m_suffix );
    iEvent.put ( algo.time   , m_prefix + "Time"   + m_suffix );
  }
  
  
};

typedef HcalTupleMaker_HcalRecHits<HBHERecHitCollection> HcalTupleMaker_HBHERecHits;
typedef HcalTupleMaker_HcalRecHits<HORecHitCollection  > HcalTupleMaker_HORecHits;
typedef HcalTupleMaker_HcalRecHits<HFRecHitCollection  > HcalTupleMaker_HFRecHits;

#endif
