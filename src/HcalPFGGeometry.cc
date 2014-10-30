#include "HCALPFG/HcalTupleMaker/interface/HcalPFGGeometry.h"

//-----------------------------------------------------------------------------
// Beware: no official mapping from L1CaloRegions to HcalTrigTowerDetId exists
// This "roll your own" mapping is based on this pre-upgrade RCT map:
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/RCTMap
//-----------------------------------------------------------------------------

void hcalpfg::getTriggerTowerIDs( int gctEta,int rctPhi, std::vector<HcalTrigTowerDetId> & ids ){
  ids.clear();
  std::vector<int> ttIEtas, ttIPhis;
  getTriggerTowerIPhis( rctPhi, ttIPhis );
  getTriggerTowerIEtas( gctEta, ttIEtas );
  int nIPhis = ttIPhis.size();
  int nIEtas = ttIEtas.size();
  for (int iiphi = 0; iiphi < nIPhis; ++iiphi){
    for (int iieta = 0; iieta < nIEtas; ++iieta){
      ids.push_back(HcalTrigTowerDetId(ttIEtas[iieta], ttIPhis[iiphi]));
    }
  }
}

//-----------------------------------------------------------------------------
// Helper function for hcalpfg::getTriggerTowerIDs
//-----------------------------------------------------------------------------

void hcalpfg::getTriggerTowerIEtas( int gctEta, std::vector<int> & ttIEtas ) {
  ttIEtas.clear();
  if       (gctEta <= 3 ) ttIEtas.push_back(gctEta - 32);
  else if  (gctEta >= 18) ttIEtas.push_back(gctEta + 11);
  else {
    int sign, start;
    if (gctEta <= 10){
      sign  = -1;
      start = -1 - (4*(10 - gctEta));
    }
    else {
      sign  = 1;
      start = 1 + (4*(gctEta - 11));
    }
    for (int i = 0; i < 4; ++i) ttIEtas.push_back ( start + (i*sign));
  }
}

//-----------------------------------------------------------------------------
// Helper function for hcalpfg::getTriggerTowerIDs
//-----------------------------------------------------------------------------

void hcalpfg::getTriggerTowerIPhis( int rctPhi, std::vector<int> & ttIPhis ){
  ttIPhis.clear();
  int iphi;
  int start = (((rctPhi * 4) - 2) % 72) + 1;
  for (int i = 0; i < 4; ++i){
    iphi = start + i;
    if (iphi > 72) iphi -= 72;
    ttIPhis.push_back( iphi );
  }
}
