#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_FEDs.h"

// Raw data analysis
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"

HcalTupleMaker_FEDs::HcalTupleMaker_FEDs(const edm::ParameterSet& iConfig) :
  inputTag (iConfig.getUntrackedParameter<edm::InputTag>("source")),
  prefix   (iConfig.getUntrackedParameter<std::string>  ("Prefix")),
  suffix   (iConfig.getUntrackedParameter<std::string>  ("Suffix")),
  minFEDID (iConfig.getUntrackedParameter<unsigned int> ("MinFED")),
  maxFEDID (iConfig.getUntrackedParameter<unsigned int> ("MaxFED"))
{  
  produces <std::vector<int> > (prefix + "Number" + suffix );
  produces <std::vector<int> > (prefix + "Size"   + suffix );
  produces <std::vector<int> > (prefix + "ORN"    + suffix );
  produces <std::vector<int> > (prefix + "BCN"    + suffix );
}

void HcalTupleMaker_FEDs::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<int> > number ( new std::vector<int> () );
  std::auto_ptr<std::vector<int> > my_size( new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bcn    ( new std::vector<int> () );
  std::auto_ptr<std::vector<int> > orn    ( new std::vector<int> () );
  
  edm::Handle<FEDRawDataCollection> rawData;

  bool gotRawData = iEvent.getByLabel(inputTag, rawData);
  if (gotRawData){

    for (int fed_number=minFEDID; fed_number<=maxFEDID; ++fed_number){
    // for(int fed_number=FEDNumbering::MINHCALFEDID;fed_number<=FEDNumbering::MAXHCALFEDID; fed_number++) {

      const FEDRawData& fedData = rawData->FEDData(fed_number);
      const unsigned char * rawData = fedData.data();
      if ( !rawData ) continue;
      if ( fedData.size() < 24 ) continue;

      number  -> push_back ( fed_number );
      my_size -> push_back ( fedData.size() );

      int tmp_orn  = -999;
      int tmp_bcn  = -999;

      if ( fedData.size() >= 24 && fed_number >= FEDNumbering::MINHCALFEDID && fed_number <= FEDNumbering::MAXHCALFEDID){
	const HcalDCCHeader * dccHeader = (HcalDCCHeader*) fedData.data();
	if ( dccHeader ) {
	  tmp_orn = dccHeader -> getOrbitNumber();
	  tmp_bcn = dccHeader -> getBunchId();
	}
      }

      bcn -> push_back ( tmp_bcn );
      orn -> push_back ( tmp_orn );
    }
  }
  else { 
    std::cout << "Could not find FedRawDataCollection with tag: " << inputTag << std::endl;
    return;
  }
  
  iEvent.put(number , prefix + "Number" + suffix );
  iEvent.put(my_size, prefix + "Size"   + suffix );
  iEvent.put(orn    , prefix + "ORN"    + suffix );
  iEvent.put(bcn    , prefix + "BCN"    + suffix );
 
}
