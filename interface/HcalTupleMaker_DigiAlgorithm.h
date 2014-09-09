#ifndef HCALTUPLEMAKER_DIGIALGORITHM_H
#define HCALTUPLEMAKER_DIGIALGORITHM_H

#include <iostream>

// HCAL conditions
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"

// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class HcalTupleMaker_DigiAlgorithm { 

 public:

  HcalTupleMaker_DigiAlgorithm();

  void run ();
  void setDoChargeReco ( bool b ) { m_doChargeReco = b; }
  void setDoEnergyReco ( bool b ) { m_doEnergyReco = b; }
  
  std::auto_ptr<std::vector<int> > ieta;           
  std::auto_ptr<std::vector<int> > iphi;           
  std::auto_ptr<std::vector<int> > depth;          
  std::auto_ptr<std::vector<int> > presamples;     
  std::auto_ptr<std::vector<int> > size;           
  std::auto_ptr<std::vector<int> > fiberIdleOffset;
  
  std::auto_ptr<std::vector<std::vector<int  > > > dv;	     	
  std::auto_ptr<std::vector<std::vector<int  > > > er;	     	
  std::auto_ptr<std::vector<std::vector<int  > > > raw;	     	
  std::auto_ptr<std::vector<std::vector<int  > > > adc;	     	
  std::auto_ptr<std::vector<std::vector<float> > > nomFC;    	
  std::auto_ptr<std::vector<std::vector<int  > > > fiber;    	
  std::auto_ptr<std::vector<std::vector<int  > > > fiberChan;	
  std::auto_ptr<std::vector<std::vector<int  > > > capid;    	
						                
  std::auto_ptr<std::vector<std::vector<float> > > allFC;    	
  std::auto_ptr<std::vector<std::vector<float> > > pedFC;    	
  std::auto_ptr<std::vector<std::vector<float> > > gain;    	
  std::auto_ptr<std::vector<std::vector<float> > > rcgain;    	
  std::auto_ptr<std::vector<std::vector<float> > > FC;		
  std::auto_ptr<std::vector<std::vector<float> > > energy;    	
						                
  std::auto_ptr<std::vector<float> > rec_energy;    
  std::auto_ptr<std::vector<float> > rec_time;    
  
  template <class DigiCollection, class RecoCollection > 
    void run ( const HcalDbService    & conditions,  
	       const DigiCollection   & digis     , 
	       const RecoCollection   & recos     ){
    
    //-----------------------------------------------------
    // Get iterators
    //-----------------------------------------------------
    
    typename RecoCollection::const_iterator reco;
    typename RecoCollection::const_iterator reco_end;
    typename DigiCollection::const_iterator digi     = digis.begin();
    typename DigiCollection::const_iterator digi_end = digis.end(); 
   
    if ( m_doEnergyReco ) reco_end = recos.end();
    
    HcalDetId       * hcalDetId    = 0;
    HcalQIECoder    * channelCoder = 0;
    HcalCalibrations* calibrations = 0;
    HcalQIEShape    * shape        = 0;
    CaloSamples tool;

    //-----------------------------------------------------
    // Loop through digis
    //-----------------------------------------------------
    
    for (; digi != digi_end ; ++digi ) {

      //-----------------------------------------------------
      // Save the detector id, no matter what
      //-----------------------------------------------------
      
      hcalDetId = const_cast<HcalDetId*> (& digi -> id());

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
      // Get digi-specific values
      //-----------------------------------------------------
      
      ieta            -> push_back ( hcalDetId -> ieta            () );
      iphi            -> push_back ( hcalDetId -> iphi            () );
      depth           -> push_back ( hcalDetId -> depth           () );
      presamples      -> push_back ( digi      -> presamples      () );
      size            -> push_back ( digi      -> size            () );
      fiberIdleOffset -> push_back ( digi      -> fiberIdleOffset () );

      dv              -> push_back ( std::vector<int  >() ) ;	     
      er              -> push_back ( std::vector<int  >() ) ;	     
      raw             -> push_back ( std::vector<int  >() ) ;	     
      adc             -> push_back ( std::vector<int  >() ) ;	     
      nomFC           -> push_back ( std::vector<float>() ) ;    
      fiber           -> push_back ( std::vector<int  >() ) ;    
      fiberChan       -> push_back ( std::vector<int  >() ) ;
      capid           -> push_back ( std::vector<int  >() ) ;    
		      
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
      
      int digi_nTS = digi -> size();

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
  
 private:
  
  bool m_doChargeReco;
  bool m_doEnergyReco;

};

#endif
