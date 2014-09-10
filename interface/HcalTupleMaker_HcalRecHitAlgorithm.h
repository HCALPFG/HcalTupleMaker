#ifndef HCALTUPLEMAKER_RECHITALGORITHM_H
#define HCALTUPLEMAKER_RECHITALGORITHM_H

#include <iostream>

// HCAL objects
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"


class HcalTupleMaker_RecHitAlgorithm { 

 public:

  HcalTupleMaker_RecHitAlgorithm();

  void run ();
  
  std::auto_ptr<std::vector<int  > > ieta;           
  std::auto_ptr<std::vector<int  > > iphi;           
  std::auto_ptr<std::vector<int  > > depth;          
  std::auto_ptr<std::vector<float> > energy;    
  std::auto_ptr<std::vector<float> > time;    
  std::auto_ptr<std::vector<int  > > flags;           
  std::auto_ptr<std::vector<int  > > aux;     
  
  template <class RecoCollection > 
    void run ( const RecoCollection & recos ){
    
    //-----------------------------------------------------
    // Get iterators
    //-----------------------------------------------------
    
    typename RecoCollection::const_iterator reco     = recos.begin();
    typename RecoCollection::const_iterator reco_end = recos.end();

    HcalDetId hcalDetId;

    //-----------------------------------------------------
    // Loop through rechits
    //-----------------------------------------------------
    
    for (; reco != reco_end ; ++reco ) {

      //-----------------------------------------------------
      // Save the detector id, no matter what
      //-----------------------------------------------------
      
      hcalDetId = HcalDetId(reco -> detid());

      //-----------------------------------------------------
      // Get rechit-specific values
      //-----------------------------------------------------
      
      ieta   -> push_back ( hcalDetId.ieta   () );
      iphi   -> push_back ( hcalDetId.iphi   () );
      depth  -> push_back ( hcalDetId.depth  () );
      energy -> push_back ( reco   -> energy () );
      time   -> push_back ( reco   -> time   () );
      flags  -> push_back ( reco   -> flags  () );
      aux    -> push_back ( reco   -> aux    () );
      
    } // end of loop over rechits
  }
};

#endif
