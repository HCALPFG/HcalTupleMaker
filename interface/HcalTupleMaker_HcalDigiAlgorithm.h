#ifndef HCALTUPLEMAKER_DIGIALGORITHM_H
#define HCALTUPLEMAKER_DIGIALGORITHM_H

#include <iostream>

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

// HcalHPDRBXMap
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"

class HcalTupleMaker_HcalDigiAlgorithm { 

 public:

  HcalTupleMaker_HcalDigiAlgorithm();

  void run ();
  void setTotalFCthreshold ( double totFCcut ) { m_totalFCthreshold = totFCcut; }
  void setDoChargeReco ( bool b ) { m_doChargeReco = b; }
  void setDoEnergyReco ( bool b ) { m_doEnergyReco = b; }
  
  std::auto_ptr<std::vector<int> > ieta;           
  std::auto_ptr<std::vector<int> > iphi;           
  std::auto_ptr<std::vector<float> > eta;           
  std::auto_ptr<std::vector<float> > phi;          
  std::auto_ptr<std::vector<int> > subdet;          
  std::auto_ptr<std::vector<int> > depth;          
  std::auto_ptr<std::vector<int  > > rbxid;
  std::auto_ptr<std::vector<int  > > hpdid;
  std::auto_ptr<std::vector<int> > presamples;     
  std::auto_ptr<std::vector<int> > size;           
  std::auto_ptr<std::vector<int> > fiberIdleOffset;
  std::auto_ptr<std::vector<int> > electronicsId;
  std::auto_ptr<std::vector<int> > rawId;

  std::auto_ptr<std::vector<std::vector<int  > > > dv;	     	
  std::auto_ptr<std::vector<std::vector<int  > > > er;	     	
  std::auto_ptr<std::vector<std::vector<int  > > > raw;	     	
  std::auto_ptr<std::vector<std::vector<int  > > > adc;	     	
  std::auto_ptr<std::vector<std::vector<float> > > nomFC;    	
  std::auto_ptr<std::vector<std::vector<int  > > > fiber;    	
  std::auto_ptr<std::vector<std::vector<int  > > > fiberChan;	
  std::auto_ptr<std::vector<std::vector<int  > > > capid;   
  std::auto_ptr<std::vector<std::vector<int  > > > ladc;   
						                
  std::auto_ptr<std::vector<std::vector<float> > > allFC;    	
  std::auto_ptr<std::vector<std::vector<float> > > pedFC;    	
  std::auto_ptr<std::vector<std::vector<float> > > gain;    	
  std::auto_ptr<std::vector<std::vector<float> > > rcgain;    	
  std::auto_ptr<std::vector<std::vector<float> > > FC;		
  std::auto_ptr<std::vector<std::vector<float> > > energy;    	
						                
  std::auto_ptr<std::vector<float> > rec_energy;    
  std::auto_ptr<std::vector<float> > rec_time;    
  
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
      // Get HPD and RBX IDs
      //-----------------------------------------------------
      //   http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h
      //   description: Algorithm which isomorphically maps HPD/RBX locations to
      //                integers ranging from 0 to NUM_HPDS-1/NUM_RBXS-1.  The HPDs/RBXs
      //                are ordered from lowest to highest: HB+, HB-, HE+, HE-.
      // "magic numbers"
      // total number of HPDs in the HB and HE: 288
      // total number of HPDs per subdetector (HB+, HB-, HE+, HE-): 72
      // number of HPDs per RBX: 4
      // total number of RBXs in the HB and HE: 72
      // total number of RBXs per subdetector (e.g. HB+, HB-, HE+, HE-): 18

      int RBXIndex = HcalHPDRBXMap::indexRBX(*hcalDetId);
      int HPDIndex = HcalHPDRBXMap::indexHPD(*hcalDetId);
      rbxid -> push_back ( RBXIndex );
      hpdid -> push_back ( HPDIndex );
      
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

};

#endif
