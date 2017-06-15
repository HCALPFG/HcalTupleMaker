#ifndef HcalTupleMaker_HcalPhase1RecHits_h
#define HcalTupleMaker_HcalPhase1RecHits_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalPhase1RecHitAlgorithm.h"

template <class RecHitCollection> 
class HcalTupleMaker_HcalPhase1RecHits : public edm::EDProducer {
 protected:
  
  const edm::InputTag   m_hfRecHitsTag;

  edm::EDGetTokenT<RecHitCollection> m_hfRecHitsToken;
  const double          m_energyThresholdHF;
  const std::string     m_prefix;
  const std::string     m_suffix;

  HcalTupleMaker_HcalPhase1RecHitAlgorithm algo;
  
  void produce( edm::Event & iEvent, const edm::EventSetup & iSetup) { 
    
    //-----------------------------------------------------
    // Prepare to put things into event
    //-----------------------------------------------------
    
    loadAlgo();
    
    //-----------------------------------------------------
    // edm::Handles
    //-----------------------------------------------------
    
    bool run_algo = true;

    edm::Handle<RecHitCollection> hfRecHits;
    bool gotHFRecHits = iEvent.getByToken(m_hfRecHitsToken, hfRecHits);
    
    if (!gotHFRecHits ) {
      std::cout << "Could not find HCAL RecHits with tag " << m_hfRecHitsTag << std::endl;
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
    
    if ( run_algo ) algo.run ( *hfRecHits, *geometry, m_energyThresholdHF );

    //-----------------------------------------------------
    // Put things into the event
    //-----------------------------------------------------
    
    dumpAlgo(iEvent);
    
  }
  
 public:
 
 HcalTupleMaker_HcalPhase1RecHits(const edm::ParameterSet& iConfig) :
    m_hfRecHitsTag      (iConfig.getUntrackedParameter<edm::InputTag>("source")),
    m_hfRecHitsToken    (consumes<RecHitCollection>(iConfig.getUntrackedParameter<edm::InputTag>("source"))),
    m_energyThresholdHF (iConfig.getUntrackedParameter<double>("energyThresholdHF")),
    m_prefix              (iConfig.getUntrackedParameter<std::string>  ("Prefix")),
    m_suffix              (iConfig.getUntrackedParameter<std::string>  ("Suffix")) {
    produces<std::vector<int>   > ( m_prefix + "IEta"   + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "IPhi"   + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Eta"    + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Phi"    + m_suffix );
    produces<std::vector<int>   > ( m_prefix + "Depth"  + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Energy" + m_suffix );
    produces<std::vector<float> > ( m_prefix + "Charge" + m_suffix );
    
    produces<std::vector<std::vector<float> > >( m_prefix + "Qie10Time"   + m_suffix );
    produces<std::vector<std::vector<float> > >( m_prefix + "Qie10Charge" + m_suffix );
    produces<std::vector<std::vector<float> > >( m_prefix + "Qie10Energy" + m_suffix );
    produces<std::vector<std::vector<int>   > >( m_prefix + "Qie10Soi"    + m_suffix );
    
  }

 protected:

  void loadAlgo(){
    algo.ieta   = std::unique_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.iphi   = std::unique_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.eta    = std::unique_ptr<std::vector<float> > ( new std::vector<float> ());
    algo.phi    = std::unique_ptr<std::vector<float> > ( new std::vector<float> ());
    algo.depth  = std::unique_ptr<std::vector<int  > > ( new std::vector<int  > ());
    algo.energy = std::unique_ptr<std::vector<float> > ( new std::vector<float> ());
    algo.charge = std::unique_ptr<std::vector<float> > ( new std::vector<float> ());

    algo.qie10time   = std::unique_ptr<std::vector<std::vector<float> > >( new std::vector<std::vector<float> >());
    algo.qie10charge = std::unique_ptr<std::vector<std::vector<float> > >( new std::vector<std::vector<float> >());
    algo.qie10energy = std::unique_ptr<std::vector<std::vector<float> > >( new std::vector<std::vector<float> >());
    algo.qie10soi    = std::unique_ptr<std::vector<std::vector<int  > > >( new std::vector<std::vector<int  > >());
   
  }
  
  void dumpAlgo( edm::Event & iEvent ){
    iEvent.put ( move( algo.ieta   ), m_prefix + "IEta"   + m_suffix );
    iEvent.put ( move( algo.iphi   ), m_prefix + "IPhi"   + m_suffix );
    iEvent.put ( move( algo.eta    ), m_prefix + "Eta"    + m_suffix );
    iEvent.put ( move( algo.phi    ), m_prefix + "Phi"    + m_suffix );
    iEvent.put ( move( algo.depth  ), m_prefix + "Depth"  + m_suffix );
    iEvent.put ( move( algo.energy ), m_prefix + "Energy" + m_suffix );
    iEvent.put ( move( algo.charge ), m_prefix + "Charge" + m_suffix );

    iEvent.put ( move( algo.qie10time   ), m_prefix + "Qie10Time"   + m_suffix );
    iEvent.put ( move( algo.qie10charge ), m_prefix + "Qie10Charge" + m_suffix );
    iEvent.put ( move( algo.qie10energy ), m_prefix + "Qie10Energy" + m_suffix );
    iEvent.put ( move( algo.qie10soi    ), m_prefix + "Qie10Soi"    + m_suffix );
    
  }
  
  
};

typedef HcalTupleMaker_HcalPhase1RecHits<HFPreRecHitCollection> HcalTupleMaker_HFPhase1RecHits;

#endif
