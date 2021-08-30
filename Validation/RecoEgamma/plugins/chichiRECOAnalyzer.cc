
// user include files
#include "Validation/RecoEgamma/plugins/chichiRECOAnalyzer.h"

#include "RecoEgamma/EgammaElectronAlgos/interface/ElectronUtilities.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <typeinfo>

using namespace reco;

chichiRECOAnalyzer::chichiRECOAnalyzer(const edm::ParameterSet &conf) : ElectronDqmAnalyzerBase(conf) {
  mcTruthCollection_ = consumes<reco::GenParticleCollection>(conf.getParameter<edm::InputTag>("mcTruthCollection"));
  electronCollection_ = consumes<reco::GsfElectronCollection>(conf.getParameter<edm::InputTag>("electronCollection"));
  electronCoreCollection_ =
      consumes<reco::GsfElectronCoreCollection>(conf.getParameter<edm::InputTag>("electronCoreCollection"));
  electronTrackCollection_ =
      consumes<reco::GsfTrackCollection>(conf.getParameter<edm::InputTag>("electronTrackCollection"));
  electronSeedCollection_ =
      consumes<reco::ElectronSeedCollection>(conf.getParameter<edm::InputTag>("electronSeedCollection"));
  offlineVerticesCollection_ =
      consumes<reco::VertexCollection>(conf.getParameter<edm::InputTag>("offlinePrimaryVertices"));
  beamSpotTag_ = consumes<reco::BeamSpot>(conf.getParameter<edm::InputTag>("beamSpot"));

  isoFromDepsTk03Tag_ = consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsTk03"));
  isoFromDepsTk04Tag_ = consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsTk04"));
  isoFromDepsEcalFull03Tag_ =
      consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsEcalFull03"));
  isoFromDepsEcalFull04Tag_ =
      consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsEcalFull04"));
  isoFromDepsEcalReduced03Tag_ =
      consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsEcalReduced03"));
  isoFromDepsEcalReduced04Tag_ =
      consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsEcalReduced04"));
  isoFromDepsHcal03Tag_ = consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsHcal03"));
  isoFromDepsHcal04Tag_ = consumes<edm::ValueMap<double> >(conf.getParameter<edm::InputTag>("isoFromDepsHcal04"));

  maxPt_ = conf.getParameter<double>("MaxPt");
  maxAbsEta_ = conf.getParameter<double>("MaxAbsEta");
  deltaR_ = conf.getParameter<double>("DeltaR");
  deltaR2_ = deltaR_ * deltaR_;
  matchingIDs_ = conf.getParameter<std::vector<int> >("MatchingID");
  matchingMotherIDs_ = conf.getParameter<std::vector<int> >("MatchingMotherID");

  // histos bining and limits

  edm::ParameterSet histosSet = conf.getParameter<edm::ParameterSet>("histosCfg");

}

void chichiRECOAnalyzer::bookHistograms(DQMStore::IBooker &iBooker, edm::Run const &, edm::EventSetup const &) {}

chichiRECOAnalyzer::~chichiRECOAnalyzer() {}

void chichiRECOAnalyzer::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
  // get collections
  auto gsfElectrons = iEvent.getHandle(electronCollection_);
  auto gsfElectronCores = iEvent.getHandle(electronCoreCollection_);
  auto gsfElectronTracks = iEvent.getHandle(electronTrackCollection_);
  auto gsfElectronSeeds = iEvent.getHandle(electronSeedCollection_);
  auto genParticles = iEvent.getHandle(mcTruthCollection_);
  auto theBeamSpot = iEvent.getHandle(beamSpotTag_);

  edm::Handle<edm::ValueMap<double> > isoFromDepsTk03Handle;
  iEvent.getByToken(isoFromDepsTk03Tag_, isoFromDepsTk03Handle);

  edm::Handle<edm::ValueMap<double> > isoFromDepsTk04Handle;
  iEvent.getByToken(isoFromDepsTk04Tag_, isoFromDepsTk04Handle);

  edm::Handle<edm::ValueMap<double> > isoFromDepsEcalFull03Handle;
  iEvent.getByToken(isoFromDepsEcalFull03Tag_, isoFromDepsEcalFull03Handle);

  edm::Handle<edm::ValueMap<double> > isoFromDepsEcalFull04Handle;
  iEvent.getByToken(isoFromDepsEcalFull04Tag_, isoFromDepsEcalFull04Handle);

  edm::Handle<edm::ValueMap<double> > isoFromDepsEcalReduced03Handle;
  iEvent.getByToken(isoFromDepsEcalReduced03Tag_, isoFromDepsEcalReduced03Handle);

  edm::Handle<edm::ValueMap<double> > isoFromDepsEcalReduced04Handle;
  iEvent.getByToken(isoFromDepsEcalReduced04Tag_, isoFromDepsEcalReduced04Handle);

  edm::Handle<edm::ValueMap<double> > isoFromDepsHcal03Handle;
  iEvent.getByToken(isoFromDepsHcal03Tag_, isoFromDepsHcal03Handle);

  edm::Handle<edm::ValueMap<double> > isoFromDepsHcal04Handle;
  iEvent.getByToken(isoFromDepsHcal04Tag_, isoFromDepsHcal04Handle);

  //edm::Handle<reco::VertexCollection> ;
  //iEvent.getByToken(, vertexCollectionHandle);
  auto vertexCollectionHandle = iEvent.getHandle(offlineVerticesCollection_);
  if (!vertexCollectionHandle.isValid()) {
    edm::LogInfo("ElectronMcSignalValidator::analyze") << "vertexCollectionHandle KO";
  } else {
    edm::LogInfo("ElectronMcSignalValidator::analyze") << "vertexCollectionHandle OK";
  }

  edm::LogInfo("ElectronMcSignalValidator::analyze")
      << "Treating event " << iEvent.id() << " with " << gsfElectrons.product()->size() << " electrons";

  //===============================================
  // all rec electrons
  //===============================================

  reco::GsfElectronCollection::const_iterator gsfIter;
  for (gsfIter = gsfElectrons->begin(); gsfIter != gsfElectrons->end(); gsfIter++) {
    // preselect electrons
    if (gsfIter->pt() > maxPt_ || std::abs(gsfIter->eta()) > maxAbsEta_)
      continue;

  }

  //===============================================
  // charge mis-ID
  //===============================================
  std::cout << " " << std::endl;
  std::cout << "evt ID = " << iEvent.id() << std::endl;  
  std::cout << "association mis-ID" << std::endl;
  bool matchingID, matchingMotherID;

  reco::GenParticleCollection::const_iterator mcIter;
  for (mcIter = genParticles->begin(); mcIter != genParticles->end(); mcIter++) {

    //std::cout << "genParticles : " << "eta : " << mcIter->eta() << ", pt : " << mcIter->pt();
    //std::cout << ", phi : " << mcIter->phi() << std::endl;
    // select requested matching gen particle
    matchingID = false;
    for (unsigned int i = 0; i < matchingIDs_.size(); i++) {
      if (mcIter->pdgId() == matchingIDs_[i]) {
        matchingID = true;
      }
    }
    if (matchingID) {
      // select requested mother matching gen particle
      // always include single particle with no mother
      const Candidate *mother = mcIter->mother();
      matchingMotherID = false;
      for (unsigned int i = 0; i < matchingMotherIDs_.size(); i++) {

        if (mother == nullptr) {
          matchingMotherID = true;
        } else if (mother->pdgId() == matchingMotherIDs_[i]) {
          if (mother->numberOfDaughters() <= 2) {
            matchingMotherID = true;
            /*std::cout << "genParticles : " << "eta : " << mcIter->eta() << ", pt : " << mcIter->pt();
            std::cout << ", phi : " << mcIter->phi() << std::endl;
            std::cout << "   mother eta : " << mother->eta() << ", pt : " << mother->pt() <<  ", phi : " << mother->phi() << std::endl;*/
          }
        }  // end of mother if test
      }
      if (matchingMotherID) {
        if (mcIter->pt() > maxPt_ || std::abs(mcIter->eta()) > maxAbsEta_) {
          continue;
        }

        // looking for the best matching gsf electron
        //bool okGsfFound = false;
        double gsfOkRatio = 999999.;

        // find best matched electron
        reco::GsfElectron bestGsfElectron;
        reco::GsfElectronCollection::const_iterator gsfIter;
        for (gsfIter = gsfElectrons->begin(); gsfIter != gsfElectrons->end(); gsfIter++) {
          double dphi = gsfIter->phi() - mcIter->phi();
          if (std::abs(dphi) > CLHEP::pi) {
            dphi = dphi < 0 ? (CLHEP::twopi) + dphi : dphi - CLHEP::twopi;
          }
          double deltaR2 = (gsfIter->eta() - mcIter->eta()) * (gsfIter->eta() - mcIter->eta()) + dphi * dphi;
          if (deltaR2 < deltaR2_) {
            //double mc_charge = mcIter->pdgId() == 11 ? -1. : 1.;
            // require here a charge mismatch
            if (((mcIter->pdgId() == 11) && (gsfIter->charge() > 0.)) ||
                ((mcIter->pdgId() == -11) && (gsfIter->charge() < 0.))) {
              double tmpGsfRatio = gsfIter->p() / mcIter->p();
              if (std::abs(tmpGsfRatio - 1) < std::abs(gsfOkRatio - 1)) {
                gsfOkRatio = tmpGsfRatio;
                bestGsfElectron = *gsfIter;
                std::cout << "genParticles : " << "eta : " << mcIter->eta() << ", pt : " << mcIter->pt() << ", p : " << mcIter->p();
                //std::cout << ", phi : " << mcIter->phi() << ", sigmaEtaEta" << mcIter->sigmaEtaEta() << std::endl;
                std::cout << ", phi : " << mcIter->phi() << std::endl;
                std::cout << "   mother eta : " << mother->eta() << ", pt : " << mother->pt() <<  ", phi : " << mother->phi() << std::endl;
                if (gsfIter->isEB()) {
                  std::cout << "     barrels mis" << std::endl;
                  std::cout << "     eta : " << gsfIter->eta() << ", pt : " << gsfIter->pt() <<  ", phi : " << gsfIter->phi();
                  std::cout << ", sigmaEtaEta : " << bestGsfElectron.sigmaEtaEta() << ", p : " << gsfIter->p() << std::endl;
                  std::cout << "     bestGsfElectron.eta() : " << bestGsfElectron.eta() << ", bestGsfElectron.p() : " << bestGsfElectron.p();
                  std::cout << ", mcIter-> : " << mcIter->p() << std::endl; // << (*genParticles)[i].p()
                  std::cout << "     rapport 1 : " << mcIter->pt() * bestGsfElectron.p() / mcIter->p() << std::endl;
                }
                else if (gsfIter->isEE()) {
                  std::cout << "endcaps" << std::endl;
                }
                else {
                  std::cout << "others" << std::endl;
                }
                //std::cout << "     eta : " << gsfIter->eta() << ", pt : " << gsfIter->pt() <<  ", phi : " << gsfIter->phi() << std::endl;
                //std::cout << ", sigma_etaeta" << gsfIter->sigmaIetaeta() << std::endl;
              }
            }
          }
        }  // loop over rec ele to look for the best one

      }
    }
  }

  //===============================================
  // association mc-reco
  //===============================================
  std::cout << "association mc-reco" << std::endl;
  int i = 0;
  for (mcIter = genParticles->begin(); mcIter != genParticles->end(); mcIter++) {

    // select requested matching gen particle
    matchingID = false;
    for (unsigned int i = 0; i < matchingIDs_.size(); i++) {
      if (mcIter->pdgId() == matchingIDs_[i]) {
        matchingID = true;
      }
    }
    if (!matchingID)
      continue;

    // select requested mother matching gen particle
    // always include single particle with no mother
    const Candidate *mother = mcIter->mother();
    matchingMotherID = false;
    for (unsigned int i = 0; i < matchingMotherIDs_.size(); i++) {
      if ((mother == nullptr) || ((mother != nullptr) && mother->pdgId() == matchingMotherIDs_[i])) {
        matchingMotherID = true;
        //std::cout << "   P3[" << i << "] : mother eta : " << mother->eta() << ", pt : " << mother->pt() <<  ", phi : " << mother->phi() << std::endl;
      }
    }
    if (!matchingMotherID)
      continue;

    // electron preselection
    if (mcIter->pt() > maxPt_ || std::abs(mcIter->eta()) > maxAbsEta_) {
      continue;
    }

    // find best matched electron
    bool okGsfFound = false;
    double gsfOkRatio = 999999.;
    reco::GsfElectron bestGsfElectron;
    //reco::GsfElectronRef bestGsfElectronRef;
    reco::GsfElectronCollection::const_iterator gsfIter;
    reco::GsfElectronCollection::size_type iElectron;
    for (gsfIter = gsfElectrons->begin(), iElectron = 0; gsfIter != gsfElectrons->end(); gsfIter++, iElectron++) {
      // temporary cut for pt < 5.
      double dphi = gsfIter->phi() - mcIter->phi();
      if (std::abs(dphi) > CLHEP::pi) {
        dphi = dphi < 0 ? (CLHEP::twopi) + dphi : dphi - CLHEP::twopi;
      }
      double deltaR2 = (gsfIter->eta() - mcIter->eta()) * (gsfIter->eta() - mcIter->eta()) + dphi * dphi;
      if (deltaR2 < deltaR2_) {
        if (((mcIter->pdgId() == 11) && (gsfIter->charge() < 0.)) ||
            ((mcIter->pdgId() == -11) && (gsfIter->charge() > 0.))) {
          double tmpGsfRatio = gsfIter->p() / mcIter->p();
          if (std::abs(tmpGsfRatio - 1) < std::abs(gsfOkRatio - 1)) {
            gsfOkRatio = tmpGsfRatio;
            bestGsfElectron = *gsfIter;
            //bestGsfElectronRef = reco::GsfElectronRef(gsfElectrons, iElectron);
            std::cout << "genParticles : " << "eta : " << mcIter->eta() << ", pt : " << mcIter->pt() << ", p : " << mcIter->p();
            //std::cout << ", phi : " << mcIter->phi() << ", sigmaEtaEta" << mcIter->sigmaEtaEta() << std::endl;
            std::cout << ", phi : " << mcIter->phi() << std::endl;
            std::cout << "   mother eta : " << mother->eta() << ", pt : " << mother->pt() <<  ", phi : " << mother->phi() << std::endl;
            if (gsfIter->isEB()) {
              std::cout << "     barrels mc" << std::endl;
              std::cout << "     eta : " << bestGsfElectron.eta() << ", pt : " << bestGsfElectron.pt() <<  ", phi : " << bestGsfElectron.phi();
              std::cout << ", sigmaEtaEta : " << bestGsfElectron.sigmaEtaEta() << ", p : " << gsfIter->p() << std::endl;
              std::cout << "     bestGsfElectron.eta() : " << bestGsfElectron.eta() << ", bestGsfElectron.p() : " << bestGsfElectron.p();
              std::cout << ", mcIter->p() : " << mcIter->p() << std::endl; // << (*genParticles)[i].p()
              std::cout << "     rapport 1 : " << mcIter->pt() * bestGsfElectron.p() / mcIter->p() << std::endl;
            }
            else if (gsfIter->isEE()) {
              std::cout << "endcaps mc" << std::endl;
            }
            else {
              std::cout << "others" << std::endl;
            }
            //std::cout << "     P3 : eta : " << bestGsfElectron.eta() << ", pt : " << bestGsfElectron.pt() <<  ", phi : " << bestGsfElectron.phi() << std::endl;
            //std::cout << "evt ID : " << iEvent.id() << " - Pt : " << bestGsfElectron.pt() << " - eta : " << bestGsfElectron.eta() << " - phi : " << bestGsfElectron.phi() << std::endl; // debug lines
          }
        }
      }
    }  // loop over rec ele to look for the best one
    if (!okGsfFound)
      continue;
    
    i++;
  }  // loop over mc particle
}
