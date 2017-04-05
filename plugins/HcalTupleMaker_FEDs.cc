#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_FEDs.h"

// Raw data analysis
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
  m_FEDRawDataToken = consumes<FEDRawDataCollection>(inputTag);
    //(iConfig.getUntrackedParameter<edm::InputTag>("tagRaw", edm::InputTag("rawDataCollector")));
}

void HcalTupleMaker_FEDs::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::unique_ptr<std::vector<int> > number ( new std::vector<int> () );
  std::unique_ptr<std::vector<int> > my_size( new std::vector<int> () );
  std::unique_ptr<std::vector<int> > bcn    ( new std::vector<int> () );
  std::unique_ptr<std::vector<int> > orn    ( new std::vector<int> () );
  
  edm::Handle<FEDRawDataCollection> rawData;
  if (iEvent.getByToken(m_FEDRawDataToken, rawData)) {
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
  
  iEvent.put(move(number ), prefix + "Number" + suffix );
  iEvent.put(move(my_size), prefix + "Size"   + suffix );
  iEvent.put(move(orn    ), prefix + "ORN"    + suffix );
  iEvent.put(move(bcn    ), prefix + "BCN"    + suffix );
 
}
