#ifndef HCAL_PFG_GEOMETRY_H
#define HCAL_PFG_GEOMETRY_H

#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include <vector>

namespace hcalpfg {

  // Method to get HCAL trigger tower IDs from an L1CaloRegion ieta and iphi
  // Uses RCT Map: https://twiki.cern.ch/twiki/bin/viewauth/CMS/RCTMap
  void getTriggerTowerIDs  ( int gctEta, int rctPhi, std::vector<HcalTrigTowerDetId> & ids );

  // Helper functions for getTriggerTowerIDs
  void getTriggerTowerIEtas( int gctEta, std::vector<int> & ttIEtas );
  void getTriggerTowerIPhis( int rctPhi, std::vector<int> & ttIPhis );

};

#endif
