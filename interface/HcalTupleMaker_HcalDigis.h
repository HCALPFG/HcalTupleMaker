#ifndef HcalTupleMaker_HcalDigis_h
#define HcalTupleMaker_HcalDigis_h

#include <iostream>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalDigiAlgorithm.h"

template <class DigiCollection, class RecHitCollection, class DetIdClass, class DetIdClassWrapper> 
class HcalTupleMaker_HcalDigis : public edm::EDProducer {
  
protected:
 
  edm::EDGetTokenT<DigiCollection> m_hcalDigisToken;
  edm::EDGetTokenT<RecHitCollection> m_hcalRecHitsToken;
  const edm::InputTag            m_hcalDigisTag;
  const edm::InputTag            m_hcalRecHitsTag;
  const std::string              m_prefix;
  const std::string              m_suffix;
  const bool                     m_doChargeReco;
  const bool                     m_doEnergyReco;
  const double                   m_totalFCthreshold;
  const bool                     m_filterChannels;
  std::vector<edm::ParameterSet> m_channelFilterList;
  
  HcalTupleMaker_HcalDigiAlgorithm algo;
  
  void produce( edm::Event & iEvent, const edm::EventSetup & iSetup ) { 
    
    //-----------------------------------------------------
    // Prepare to put things into event
    //-----------------------------------------------------
    
    loadAlgo();
    
    //-----------------------------------------------------
    // edm::Handles
    //-----------------------------------------------------
    
    bool run_algo = true;
    
    edm::Handle<DigiCollection>  hcalDigis;
    bool gotHCALDigis = iEvent.getByToken(m_hcalDigisToken, hcalDigis);
    if (!gotHCALDigis ) {
      std::cout << "Could not find HCAL Digis with tag " << m_hcalDigisTag << std::endl;
      run_algo = false;
    }
    edm::Handle<RecHitCollection>  hcalRecos;
    if ( m_doEnergyReco ){
      bool gotHCALRecos = iEvent.getByToken(m_hcalRecHitsToken, hcalRecos);
      if (!gotHCALRecos ) {
    	std::cout << "Could not find HCAL RecHits with tag " << m_hcalRecHitsTag << std::endl;
    	run_algo = false;
      }
    }
    
    //-----------------------------------------------------
    // edm::ESHandles
    //-----------------------------------------------------
    
    edm::ESHandle<HcalDbService> conditions;
    if ( m_doChargeReco ) iSetup.get<HcalDbRecord >().get(conditions);

    edm::ESHandle<CaloGeometry> geometry;
    iSetup.get<CaloGeometryRecord>().get(geometry);

    edm::ESHandle<HcalTPGCoder> inputCoder;
    iSetup.get<HcalTPGRecord>().get(inputCoder);
    
    //-----------------------------------------------------
    // Run the algorithm
    //-----------------------------------------------------
    
    if ( run_algo ) algo.run<DigiCollection, RecHitCollection, DetIdClass, DetIdClassWrapper > ( *conditions, *hcalDigis, *hcalRecos, *geometry, inputCoder.product() );
    
    //-----------------------------------------------------
    // Put things into the event
    //-----------------------------------------------------
    
    dumpAlgo(iEvent);
    
  }

public:
  
  HcalTupleMaker_HcalDigis(const edm::ParameterSet& iConfig) :
    m_hcalDigisTag      (iConfig.getUntrackedParameter<edm::InputTag>                  ("source")),
    m_hcalRecHitsTag    (iConfig.getUntrackedParameter<edm::InputTag>                  ("recHits")),
    m_prefix            (iConfig.getUntrackedParameter<std::string>                    ("Prefix")),
    m_suffix            (iConfig.getUntrackedParameter<std::string>                    ("Suffix")),
    m_doChargeReco      (iConfig.getUntrackedParameter<bool>                           ("DoChargeReco")),
    m_doEnergyReco      (iConfig.getUntrackedParameter<bool>                           ("DoEnergyReco")),
    m_totalFCthreshold  (iConfig.getUntrackedParameter<double>                         ("TotalFCthreshold")),
    m_filterChannels    (iConfig.getUntrackedParameter<bool>                           ("FilterChannels")),
    m_channelFilterList (iConfig.getUntrackedParameter<std::vector<edm::ParameterSet> >("ChannelFilterList"))
    {
    
      produces<std::vector<int>   >               ( m_prefix + "IEta"            + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "IPhi"            + m_suffix );
      produces<std::vector<float> >               ( m_prefix + "Eta"             + m_suffix );
      produces<std::vector<float> >               ( m_prefix + "Phi"             + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "Subdet"          + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "Depth"           + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "Presamples"      + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "Size"            + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "FiberIdleOffset" + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "ElectronicsID"   + m_suffix );
      produces<std::vector<int>   >               ( m_prefix + "RawID"           + m_suffix );
    
      produces<std::vector<std::vector<int>   > > ( m_prefix + "DV"              + m_suffix );	     	
      produces<std::vector<std::vector<int>   > > ( m_prefix + "ER"              + m_suffix );	     	
      produces<std::vector<std::vector<int>   > > ( m_prefix + "Raw"             + m_suffix );	     	
      produces<std::vector<std::vector<int>   > > ( m_prefix + "ADC"             + m_suffix );	     	
      produces<std::vector<std::vector<float> > > ( m_prefix + "NomFC"           + m_suffix );    	
      produces<std::vector<std::vector<int>   > > ( m_prefix + "Fiber"           + m_suffix );    	
      produces<std::vector<std::vector<int>   > > ( m_prefix + "FiberChan"       + m_suffix );	
      produces<std::vector<std::vector<int>   > > ( m_prefix + "CapID"           + m_suffix );    	
      produces<std::vector<std::vector<int>   > > ( m_prefix + "LADC"            + m_suffix );	     	
    
      produces<std::vector<std::vector<float> > > ( m_prefix + "AllFC"           + m_suffix );    	
      produces<std::vector<std::vector<float> > > ( m_prefix + "PedFC"           + m_suffix );    	
      produces<std::vector<std::vector<float> > > ( m_prefix + "Gain"            + m_suffix );    	
      produces<std::vector<std::vector<float> > > ( m_prefix + "RCGain"          + m_suffix );    	
      produces<std::vector<std::vector<float> > > ( m_prefix + "FC"              + m_suffix );		
      produces<std::vector<std::vector<float> > > ( m_prefix + "Energy"          + m_suffix );    	
    
      produces<std::vector<float> >               ( m_prefix + "RecEnergy"       + m_suffix );    	
      produces<std::vector<float> >               ( m_prefix + "RecTime"         + m_suffix );      

      m_hcalDigisToken = consumes<DigiCollection>(m_hcalDigisTag);
      m_hcalRecHitsToken = consumes<RecHitCollection>(m_hcalRecHitsTag);

      algo.setTotalFCthreshold ( m_totalFCthreshold );    
      algo.setDoChargeReco ( m_doChargeReco );
      algo.setDoEnergyReco ( m_doEnergyReco );
      algo.setFilterChannels ( m_filterChannels );
      // algo.setChannelFilterList( m_channelFilterList );
    }

protected:
  
  void loadAlgo(){
    algo.ieta            = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.iphi            = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.eta             = std::unique_ptr<std::vector<float> >               ( new std::vector<float> ());
    algo.phi             = std::unique_ptr<std::vector<float> >               ( new std::vector<float> ());
    algo.depth           = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.subdet          = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.presamples      = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.size            = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.fiberIdleOffset = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.electronicsId   = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.rawId           = std::unique_ptr<std::vector<int> >                 ( new std::vector<int>   ());
    algo.rec_energy      = std::unique_ptr<std::vector<float> >               ( new std::vector<float> ());
    algo.rec_time        = std::unique_ptr<std::vector<float> >               ( new std::vector<float> ());

    algo.dv 	         = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());
    algo.er 	         = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());
    algo.raw 	         = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());
    algo.adc 	         = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());
    algo.nomFC           = std::unique_ptr<std::vector<std::vector<float> > > ( new std::vector<std::vector<float> > ());
    algo.fiber           = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());
    algo.fiberChan       = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());
    algo.capid           = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());
    algo.ladc            = std::unique_ptr<std::vector<std::vector<int  > > > ( new std::vector<std::vector<int  > > ());

    algo.allFC           = std::unique_ptr<std::vector<std::vector<float> > > ( new std::vector<std::vector<float> > ());
    algo.pedFC           = std::unique_ptr<std::vector<std::vector<float> > > ( new std::vector<std::vector<float> > ());
    algo.gain            = std::unique_ptr<std::vector<std::vector<float> > > ( new std::vector<std::vector<float> > ());
    algo.rcgain          = std::unique_ptr<std::vector<std::vector<float> > > ( new std::vector<std::vector<float> > ());
    algo.FC 	         = std::unique_ptr<std::vector<std::vector<float> > > ( new std::vector<std::vector<float> > ());
    algo.energy          = std::unique_ptr<std::vector<std::vector<float> > > ( new std::vector<std::vector<float> > ());
    
  }

  void dumpAlgo ( edm::Event & iEvent ){

    iEvent.put( move(algo.ieta            ), m_prefix + "IEta"            + m_suffix );
    iEvent.put( move(algo.iphi            ), m_prefix + "IPhi"            + m_suffix );
    iEvent.put( move(algo.eta             ), m_prefix + "Eta"             + m_suffix );
    iEvent.put( move(algo.phi             ), m_prefix + "Phi"             + m_suffix );
    iEvent.put( move(algo.depth           ), m_prefix + "Depth"           + m_suffix );
    iEvent.put( move(algo.subdet          ), m_prefix + "Subdet"          + m_suffix );
    iEvent.put( move(algo.presamples      ), m_prefix + "Presamples"      + m_suffix );
    iEvent.put( move(algo.size            ), m_prefix + "Size"            + m_suffix );
    iEvent.put( move(algo.fiberIdleOffset ), m_prefix + "FiberIdleOffset" + m_suffix );
    iEvent.put( move(algo.electronicsId   ), m_prefix + "ElectronicsID"   + m_suffix );
    iEvent.put( move(algo.rawId           ), m_prefix + "RawID"           + m_suffix );
   
    iEvent.put( move(algo.dv              ), m_prefix + "DV"              + m_suffix );
    iEvent.put( move(algo.er              ), m_prefix + "ER"              + m_suffix );
    iEvent.put( move(algo.raw  	          ), m_prefix + "Raw"             + m_suffix );
    iEvent.put( move(algo.adc   	  ), m_prefix + "ADC"             + m_suffix );
    iEvent.put( move(algo.nomFC           ), m_prefix + "NomFC"           + m_suffix );
    iEvent.put( move(algo.fiber           ), m_prefix + "Fiber"           + m_suffix );
    iEvent.put( move(algo.fiberChan       ), m_prefix + "FiberChan"       + m_suffix );
    iEvent.put( move(algo.capid           ), m_prefix + "CapID"           + m_suffix );
    iEvent.put( move(algo.ladc   	  ), m_prefix + "LADC"            + m_suffix );
   
    iEvent.put( move(algo.allFC           ), m_prefix + "AllFC"           + m_suffix );
    iEvent.put( move(algo.pedFC           ), m_prefix + "PedFC"           + m_suffix );
    iEvent.put( move(algo.gain            ), m_prefix + "Gain"            + m_suffix );
    iEvent.put( move(algo.rcgain          ), m_prefix + "RCGain"          + m_suffix );
    iEvent.put( move(algo.FC 	          ), m_prefix + "FC"              + m_suffix );
    iEvent.put( move(algo.energy          ), m_prefix + "Energy"          + m_suffix );

    iEvent.put( move(algo.rec_energy      ), m_prefix + "RecEnergy"       + m_suffix );
    iEvent.put( move(algo.rec_time        ), m_prefix + "RecTime"         + m_suffix );
   
  }
};

class HcalCalibDetIdWrapper : public HcalCalibDetId { 
public:
  int depth() { return -1; }
};

typedef HcalTupleMaker_HcalDigis<HBHEDigiCollection     , HBHERecHitCollection, HcalDetId     , HcalDetId            > HcalTupleMaker_HBHEDigis;
typedef HcalTupleMaker_HcalDigis<HODigiCollection       , HORecHitCollection  , HcalDetId     , HcalDetId            > HcalTupleMaker_HODigis;
typedef HcalTupleMaker_HcalDigis<HFDigiCollection       , HFRecHitCollection  , HcalDetId     , HcalDetId            > HcalTupleMaker_HFDigis;
typedef HcalTupleMaker_HcalDigis<HcalCalibDigiCollection, HBHERecHitCollection, HcalCalibDetId, HcalCalibDetIdWrapper> HcalTupleMaker_CalibDigis;

#endif
