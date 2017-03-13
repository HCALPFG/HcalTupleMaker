#include "HCALPFG/HcalTupleMaker/interface/HcalTupleMaker_HcalUnpackerReport.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include <iostream>

HcalTupleMaker_HcalUnpackerReport::HcalTupleMaker_HcalUnpackerReport(const edm::ParameterSet& iConfig) :
  inputTag (iConfig.getUntrackedParameter<edm::InputTag>("source")),
  prefix   (iConfig.getUntrackedParameter<std::string>  ("Prefix")),
  suffix   (iConfig.getUntrackedParameter<std::string>  ("Suffix"))
{  
  produces <unsigned int>      (prefix + "ErrorFree"           + suffix );
  produces <unsigned int>      (prefix + "AnyValid"            + suffix );
  produces <unsigned int>      (prefix + "SpigotFormatErrors"  + suffix );
  produces <unsigned int>      (prefix + "BadQualityDigis"     + suffix );
  produces <unsigned int>      (prefix + "TotalDigis"          + suffix );
  produces <unsigned int>      (prefix + "TotalTPDigis"        + suffix );
  produces <unsigned int>      (prefix + "TotalHOTPDigis"      + suffix );
  produces <unsigned int>      (prefix + "EmptySpigots"        + suffix );
  produces <unsigned int>      (prefix + "OFWSpigots"          + suffix );
  produces <unsigned int>      (prefix + "BSYSpigots"          + suffix );
  produces <unsigned int>      (prefix + "NZS"                 + suffix );
  produces <unsigned int>      (prefix + "HasCalib"            + suffix );
  produces <std::vector<int> > (prefix + "BadDigiIEta"         + suffix );
  produces <std::vector<int> > (prefix + "BadDigiIPhi"         + suffix );
  produces <std::vector<int> > (prefix + "BadDigiDepth"        + suffix );
  produces <std::vector<int> > (prefix + "BadDigiSubdet"       + suffix );
  token_report_ = consumes<HcalUnpackerReport>(inputTag);
  
}

void HcalTupleMaker_HcalUnpackerReport::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::unique_ptr<unsigned int>      errorFree          ( new unsigned int()      );
  std::unique_ptr<unsigned int>      anyValid           ( new unsigned int()      );
  std::unique_ptr<unsigned int>      spigotFormatErrors ( new unsigned int()      );
  std::unique_ptr<unsigned int>      badDigis           ( new unsigned int()      );
  std::unique_ptr<unsigned int>      totalDigis         ( new unsigned int()      );
  std::unique_ptr<unsigned int>      totalTPDigis       ( new unsigned int()      );
  std::unique_ptr<unsigned int>      totalHOTPDigis     ( new unsigned int()      );
  std::unique_ptr<unsigned int>      emptySpigots       ( new unsigned int()      );
  std::unique_ptr<unsigned int>      OFWSpigots         ( new unsigned int()      );
  std::unique_ptr<unsigned int>      BSYSpigots         ( new unsigned int()      );
  std::unique_ptr<unsigned int>      unsuppressed       ( new unsigned int()      );
  std::unique_ptr<unsigned int>      calibrationPresent ( new unsigned int()      );
  std::unique_ptr<std::vector<int> > badDigiIEta        ( new std::vector<int> () );
  std::unique_ptr<std::vector<int> > badDigiIPhi        ( new std::vector<int> () );
  std::unique_ptr<std::vector<int> > badDigiDepth       ( new std::vector<int> () );
  std::unique_ptr<std::vector<int> > badDigiSubdet      ( new std::vector<int> () );
  
  edm::Handle<HcalUnpackerReport> report;

  //bool gotReport = iEvent.getByLabel(inputTag, report);
  bool gotReport = iEvent.getByToken(token_report_, report);

  if ( gotReport ) { 
    (*errorFree)          = report -> errorFree   ();
    (*anyValid )          = report -> anyValidHCAL();
    (*spigotFormatErrors) = report -> spigotFormatErrors();
    (*badDigis)           = report -> badQualityDigis();
    (*totalDigis)         = report -> totalDigis();
    (*totalTPDigis)       = report -> totalTPDigis();
    (*totalHOTPDigis)     = report -> totalHOTPDigis();
    (*emptySpigots)       = report -> emptyEventSpigots();
    (*OFWSpigots)         = report -> OFWSpigots();
    (*BSYSpigots)         = report -> busySpigots();
    (*unsuppressed)       = report -> unsuppressedChannels();
    (*calibrationPresent) = report -> hasFedWithCalib();

    std::vector<DetId>::const_iterator bad_detid     = report -> bad_quality_begin();
    std::vector<DetId>::const_iterator bad_detid_end = report -> bad_quality_end  ();
    for(; bad_detid != bad_detid_end; ++bad_detid ){
      
      if ( bad_detid -> subdetId() == HcalOther ||
	   bad_detid -> subdetId() == HcalTriggerTower ) continue;

      HcalDetId id (*bad_detid);
      badDigiIEta   -> push_back ( id.ieta ()  );
      badDigiIPhi   -> push_back ( id.iphi ()  );
      badDigiDepth  -> push_back ( id.depth()  );
      badDigiSubdet -> push_back ( id.subdet() );
    }
    
  }
  else { 
    std::cout << "Could not find HcalUnpackerReport with tag: " << inputTag << std::endl;
    return;
  }
  
  iEvent.put ( move( errorFree          ), prefix + "ErrorFree"           + suffix );
  iEvent.put ( move( anyValid           ), prefix + "AnyValid"            + suffix );
  iEvent.put ( move( spigotFormatErrors ), prefix + "SpigotFormatErrors"  + suffix );
  iEvent.put ( move( badDigis           ), prefix + "BadQualityDigis"     + suffix );
  iEvent.put ( move( totalDigis         ), prefix + "TotalDigis"          + suffix );
  iEvent.put ( move( totalTPDigis       ), prefix + "TotalTPDigis"        + suffix );
  iEvent.put ( move( totalHOTPDigis     ), prefix + "TotalHOTPDigis"      + suffix );
  iEvent.put ( move( emptySpigots       ), prefix + "EmptySpigots"        + suffix );
  iEvent.put ( move( OFWSpigots         ), prefix + "OFWSpigots"          + suffix );
  iEvent.put ( move( BSYSpigots         ), prefix + "BSYSpigots"          + suffix );
  iEvent.put ( move( unsuppressed       ), prefix + "NZS"                 + suffix );
  iEvent.put ( move( calibrationPresent ), prefix + "HasCalib"            + suffix );
  iEvent.put ( move( badDigiIEta        ), prefix + "BadDigiIEta"         + suffix );
  iEvent.put ( move( badDigiIPhi        ), prefix + "BadDigiIPhi"         + suffix );
  iEvent.put ( move( badDigiDepth       ), prefix + "BadDigiDepth"        + suffix );
  iEvent.put ( move( badDigiSubdet      ), prefix + "BadDigiSubdet"       + suffix );

}
