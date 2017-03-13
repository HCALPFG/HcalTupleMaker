#ifndef HCALTUPLEMAKER_DIGIALGORITHM_H
#define HCALTUPLEMAKER_DIGIALGORITHM_H

// HCAL conditions
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGCoder.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGRecord.h"
#include "CalibCalorimetry/HcalTPGAlgos/interface/HcaluLUTTPGCoder.h"

// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

// Geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

class HcalTupleMaker_HcalDigiAlgorithm { 

public:

  HcalTupleMaker_HcalDigiAlgorithm();

  void run ();
  void setTotalFCthreshold ( double totFCcut ) { m_totalFCthreshold = totFCcut; }
  void setDoChargeReco ( bool b ) { m_doChargeReco = b; }
  void setDoEnergyReco ( bool b ) { m_doEnergyReco = b; }
  void setFilterChannels (bool b ) { m_filterChannels = b; }
  void setChannelFilterList ( std::vector<edm::ParameterSet> vps ) { m_channelFilterList = vps; }
  
  std::unique_ptr<std::vector<int> > ieta;
  std::unique_ptr<std::vector<int> > iphi;
  std::unique_ptr<std::vector<float> > eta;
  std::unique_ptr<std::vector<float> > phi;
  std::unique_ptr<std::vector<int> > subdet;
  std::unique_ptr<std::vector<int> > depth;
  std::unique_ptr<std::vector<int> > presamples;
  std::unique_ptr<std::vector<int> > size;
  std::unique_ptr<std::vector<int> > fiberIdleOffset;
  std::unique_ptr<std::vector<int> > electronicsId;
  std::unique_ptr<std::vector<int> > rawId;

  std::unique_ptr<std::vector<std::vector<int  > > > dv;
  std::unique_ptr<std::vector<std::vector<int  > > > er;
  std::unique_ptr<std::vector<std::vector<int  > > > raw;
  std::unique_ptr<std::vector<std::vector<int  > > > adc;
  std::unique_ptr<std::vector<std::vector<float> > > nomFC;
  std::unique_ptr<std::vector<std::vector<int  > > > fiber;
  std::unique_ptr<std::vector<std::vector<int  > > > fiberChan;
  std::unique_ptr<std::vector<std::vector<int  > > > capid;
  std::unique_ptr<std::vector<std::vector<int  > > > ladc;
						                
  std::unique_ptr<std::vector<std::vector<float> > > allFC;
  std::unique_ptr<std::vector<std::vector<float> > > pedFC;
  std::unique_ptr<std::vector<std::vector<float> > > gain;
  std::unique_ptr<std::vector<std::vector<float> > > rcgain;
  std::unique_ptr<std::vector<std::vector<float> > > FC;
  std::unique_ptr<std::vector<std::vector<float> > > energy;
						                
  std::unique_ptr<std::vector<float> > rec_energy;
  std::unique_ptr<std::vector<float> > rec_time;
  
  template <class DigiCollection, class RecoCollection, class DetIdClass, class DetIdClassWrapper > 
  void run ( const HcalDbService    & conditions,  
	     const DigiCollection   & digis     , 
	     const RecoCollection   & recos     ,
	     const CaloGeometry     & geometry  ,
	     const HcalTPGCoder     * inCoder   ){

    //-----------------------------------------------------
    // Get iterators
    //-----------------------------------------------------
    
    typename RecoCollection::const_iterator reco;
    typename RecoCollection::const_iterator reco_end;
    typename DigiCollection::const_iterator digi     = digis.begin();
    typename DigiCollection::const_iterator digi_end = digis.end(); 

    //-----------------------------------------------------
    // Setup input coder for linear ADC calculation
    //-----------------------------------------------------
    
    m_inputCoder = dynamic_cast<const HcaluLUTTPGCoder *>(inCoder);
    
    if ( m_doEnergyReco ) reco_end = recos.end();
    
    DetIdClass        * hcalDetId    = 0;
    DetIdClassWrapper * hcalDetIdW   = 0;
    HcalQIECoder      * channelCoder = 0;
    HcalCalibrations  * calibrations = 0;
    HcalQIEShape      * shape        = 0;
    
    
    CaloSamples tool;
    IntegerCaloSamples itool;
    
    //-----------------------------------------------------
    // Loop through digis
    //-----------------------------------------------------

    for (; digi != digi_end ; ++digi ) {

      //-----------------------------------------------------
      // Save the detector id, no matter what
      //-----------------------------------------------------
      
      hcalDetId  = const_cast <DetIdClass*>        (& digi -> id());
      hcalDetIdW = static_cast<DetIdClassWrapper*> (hcalDetId);
      
      //-----------------------------------------------------
      // Get the position
      //-----------------------------------------------------
      
      const GlobalPoint& position = geometry.getPosition(*hcalDetId);
      
      //-----------------------------------------------------
      // Get linear ADC
      //-----------------------------------------------------
      
      adc2Linear(*digi, itool);

      //-----------------------------------------------------
      // If desired, get objects to reconstruct charge
      //-----------------------------------------------------
      
      if ( m_doChargeReco ){
	channelCoder = const_cast<HcalQIECoder    *> (  conditions.getHcalCoder        (*hcalDetId));  
	calibrations = const_cast<HcalCalibrations*> (& conditions.getHcalCalibrations (*hcalDetId));
	shape        = const_cast<HcalQIEShape    *> (  conditions.getHcalShape        (*hcalDetId));
	HcalCoderDb coder (*channelCoder, *shape); 
	coder.adc2fC ( * digi, tool );
      }

      //-----------------------------------------------------
      // Skip digis with totalFC less than m_totalFCthreshold
      //-----------------------------------------------------

      int  digi_nTS = digi -> size();
      float totalFC = 0;

      for ( int iTS = 0; iTS < digi_nTS ; ++iTS ) {
        const HcalQIESample * qieSample = & digi -> sample (iTS);
	int   tmp_capid = qieSample -> capid();
	float tmp_allFC = tool[iTS];
	float tmp_pedFC = calibrations -> pedestal     ( tmp_capid );
	float tmp_FC    = tmp_allFC - tmp_pedFC;
	totalFC+=tmp_FC;
      }

      if( totalFC < m_totalFCthreshold ) continue;

      //-----------------------------------------------------
      // Filter all channels not in the input tag
      //-----------------------------------------------------

      bool filterChannel = true;
      
      std::vector<std::vector<int> > channelFilterList;

      for (std::vector<edm::ParameterSet>::iterator it = m_channelFilterList.begin(); it != m_channelFilterList.end(); ++it) {
	std::vector<int> channel;
	channel.push_back( (*it).getParameter<int>("iEta")  );
	channel.push_back( (*it).getParameter<int>("iPhi")  );
	channel.push_back( (*it).getParameter<int>("depth") );
	channelFilterList.push_back(channel);
      }

      for (std::vector<std::vector<int> >::iterator it = channelFilterList.begin(); it != channelFilterList.end(); ++it) {
	if ( (*it).at(0) == hcalDetIdW->ieta() &&
	     (*it).at(1) == hcalDetIdW->iphi() &&
	     (*it).at(2) == hcalDetIdW->depth() ) filterChannel = false;
      }

      if (m_filterChannels && filterChannel) continue;
      
      //-----------------------------------------------------
      // Get digi-specific values
      //-----------------------------------------------------

      eta             -> push_back ( position  .  eta             () );
      phi             -> push_back ( position  .  phi             () );
      ieta            -> push_back ( hcalDetIdW -> ieta           () );
      iphi            -> push_back ( hcalDetIdW -> iphi           () );
      depth           -> push_back ( hcalDetIdW -> depth          () );
      subdet          -> push_back ( hcalDetIdW -> subdet         () );
      presamples      -> push_back ( digi      -> presamples      () );
      size            -> push_back ( digi      -> size            () );
      fiberIdleOffset -> push_back ( digi      -> fiberIdleOffset () );
      electronicsId   -> push_back ( digi -> elecId().rawId() );
      rawId           -> push_back ( hcalDetIdW -> rawId() );

      dv              -> push_back ( std::vector<int  >() ) ;	     
      er              -> push_back ( std::vector<int  >() ) ;	     
      raw             -> push_back ( std::vector<int  >() ) ;	     
      adc             -> push_back ( std::vector<int  >() ) ;	     
      nomFC           -> push_back ( std::vector<float>() ) ;    
      fiber           -> push_back ( std::vector<int  >() ) ;    
      fiberChan       -> push_back ( std::vector<int  >() ) ;
      capid           -> push_back ( std::vector<int  >() ) ;    
      ladc            -> push_back ( std::vector<int  >() ) ;    

      allFC           -> push_back ( std::vector<float>() ) ;    
      pedFC           -> push_back ( std::vector<float>() ) ;    
      gain            -> push_back ( std::vector<float>() ) ;    
      rcgain          -> push_back ( std::vector<float>() ) ;    
      FC              -> push_back ( std::vector<float>() ) ;    
      energy          -> push_back ( std::vector<float>() ) ;    

      size_t last_entry = energy -> size() - 1;
      
      //-----------------------------------------------------
      // Loop through digi time slices
      //-----------------------------------------------------
      
      //int digi_nTS = digi -> size();

      for ( int iTS = 0; iTS < digi_nTS ; ++iTS ) {

	//-----------------------------------------------------
	// Get slice-specific cc objects
	//-----------------------------------------------------
	
	const HcalQIESample * qieSample = & digi -> sample (iTS);

	//-----------------------------------------------------
	// Standard stuff without charge reconstruction
	//-----------------------------------------------------

	int tmp_capid = qieSample -> capid();

	(*dv       )[last_entry].push_back ( (int) qieSample -> dv()         );
	(*er       )[last_entry].push_back ( (int) qieSample -> er()         );
	(*raw      )[last_entry].push_back (       qieSample -> raw()        );
	(*adc      )[last_entry].push_back (       qieSample -> adc()        );
	(*nomFC    )[last_entry].push_back (       qieSample -> nominal_fC() );
	(*fiber    )[last_entry].push_back (       qieSample -> fiber()      );
	(*fiberChan)[last_entry].push_back (       qieSample -> fiberChan()  );
	(*capid    )[last_entry].push_back (       tmp_capid                 );
	(*ladc     )[last_entry].push_back (       itool[iTS]                );

	//-----------------------------------------------------
	// Charge reconstruction values
	//-----------------------------------------------------
	
	if ( m_doChargeReco ){

	  float tmp_allFC = tool[iTS];
	  float tmp_pedFC = calibrations -> pedestal     ( tmp_capid );
	  float tmp_gain  = calibrations -> respcorrgain ( tmp_capid );
	  float tmp_FC    = tmp_allFC - tmp_pedFC;

	  (*allFC )[last_entry].push_back (tmp_allFC);
	  (*pedFC )[last_entry].push_back (tmp_pedFC);
	  (*rcgain)[last_entry].push_back (tmp_gain);
	  (*FC    )[last_entry].push_back (tmp_FC);
	  (*energy)[last_entry].push_back (tmp_FC * tmp_gain);
	  (*gain  )[last_entry].push_back (calibrations -> rawgain ( tmp_capid ));
	  

	}
      }
      
      //-----------------------------------------------------
      // For each digi, try to find a rechit
      //-----------------------------------------------------

      if ( m_doEnergyReco ){

      	reco = recos.find ( * hcalDetId ) ;
      	
      	float reco_energy = -999.;
      	float reco_time   = -999.;
      	
      	if ( reco != reco_end ) {
      	  reco_energy = reco -> energy();
      	  reco_time   = reco -> time();
      	}
      
      	rec_energy -> push_back ( reco_energy );
      	rec_time   -> push_back ( reco_time   );
      
      }
    } // end of loop over digis
  }

  void adc2Linear ( const HBHEDataFrame      & frame, IntegerCaloSamples & tool ){ m_inputCoder -> adc2Linear ( frame, tool ); }
  void adc2Linear ( const HFDataFrame        & frame, IntegerCaloSamples & tool ){ m_inputCoder -> adc2Linear ( frame, tool ); }
  void adc2Linear ( const HODataFrame        & frame, IntegerCaloSamples & tool ){ return; }
  void adc2Linear ( const HcalCalibDataFrame & frame, IntegerCaloSamples & tool ){ return; }
  
  
private:
  

  bool m_doChargeReco;
  bool m_doEnergyReco;
  const HcaluLUTTPGCoder * m_inputCoder;
  double m_totalFCthreshold;
  bool m_filterChannels;
  std::vector<edm::ParameterSet> m_channelFilterList;
  
};

#endif
