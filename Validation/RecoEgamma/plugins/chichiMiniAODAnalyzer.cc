
// user include files
#include "Validation/RecoEgamma/plugins/chichiMiniAODAnalyzer.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// user include files

using namespace reco;
using namespace pat;
using namespace edm;

typedef edm::Ptr<pat::Electron> PatElectronPtr;

chichiMiniAODAnalyzer::chichiMiniAODAnalyzer(const edm::ParameterSet& iConfig)
    : ElectronDqmAnalyzerBase(iConfig) {
  mcTruthCollection_ = consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("mcTruthCollection"));  // prunedGenParticles
  electronToken_ =
      consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"));  // slimmedElectrons
  electronTokenEndcaps_ =
      consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons_endcaps"));  // slimmedElectrons

  edm::ParameterSet histosSet = iConfig.getParameter<edm::ParameterSet>("histosCfg");
  edm::ParameterSet isolationSet = iConfig.getParameter<edm::ParameterSet>("isolationCfg");

  maxPt_ = iConfig.getParameter<double>("MaxPt");
  maxAbsEta_ = iConfig.getParameter<double>("MaxAbsEta");
  deltaR_ = iConfig.getParameter<double>("DeltaR");
  deltaR2_ = deltaR_ * deltaR_;
  matchingIDs_ = iConfig.getParameter<std::vector<int> >("MatchingID");
  matchingMotherIDs_ = iConfig.getParameter<std::vector<int> >("MatchingMotherID");

}

chichiMiniAODAnalyzer::~chichiMiniAODAnalyzer() {}

void chichiMiniAODAnalyzer::bookHistograms(DQMStore::IBooker& iBooker, edm::Run const&, edm::EventSetup const&) {}

void chichiMiniAODAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup&) {
  // get collections
  auto electrons = iEvent.getHandle(electronToken_);
  auto electrons_endcaps = iEvent.getHandle(electronTokenEndcaps_);

  Handle<edm::View<reco::GenParticle> > genParticles;
  iEvent.getByToken(mcTruthCollection_, genParticles);
  //auto genParticles = iEvent::getHandle(mcTruthCollection_);

  edm::Handle<pat::ElectronCollection> mergedElectrons;

  LogInfo("chichiMiniAODAnalyzer::analyze")
      << "Treating event " << iEvent.id() << " with " << electrons.product()->size() << " electrons";
  LogInfo("chichiMiniAODAnalyzer::analyze")
      << "Treating event " << iEvent.id() << " with " << electrons_endcaps.product()->size()
      << " multi slimmed electrons";

  //===============================================
  // all rec electrons
  //===============================================

  pat::Electron gsfElectron;

  bool matchingMotherID;

  std::cout << " " << std::endl;
  for (size_t i = 0; i < genParticles->size(); i++) {
        /*            // DEBUG LINES - KEEP IT !
        //std::cout << "evt ID = " << iEvent.id() ;
        //std::cout << ",  mcIter position : " << i << std::endl;
        //std::cout << "pdgID : " << (*genParticles)[i].pdgId() << ", Pt : " << (*genParticles)[i].pt() ;
        //std::cout << ", eta : " << (*genParticles)[i].eta() << ", phi : " << (*genParticles)[i].phi() << std::endl;
                // DEBUG LINES - KEEP IT !  */
        auto gp = (*genParticles)[i];
        //std::cout << "genParticles : " << "eta : " << gp.eta() << ", pt : " << gp.pt();
        //std::cout << ", phi : " << gp.phi() << std::endl;

    // select requested mother matching gen particle
    // always include single particle with no mother
    const Candidate* mother = gp.mother(0);
    matchingMotherID = false;
    for (unsigned int ii = 0; ii < matchingMotherIDs_.size(); ii++) {

      if (mother == nullptr) {
        matchingMotherID = true;
      } else if (mother->pdgId() == matchingMotherIDs_[ii]) {
        if (mother->numberOfDaughters() <= 2) {
          matchingMotherID = true;
          std::cout << "evt ID = " << iEvent.id() << std::endl;                                                                               // debug lines
          std::cout << "genParticles : " << "eta : " << gp.eta() << ", pt : " << gp.pt() << ", p : " << gp.p();
          //std::cout << ", phi : " << gp.phi() << ", sigmaEtaEta" << gp.sigmaEtaEta() << std::endl;
          std::cout << ", phi : " << gp.phi() << std::endl;
          //std::cout << " - nb of Daughters : " << mother->numberOfDaughters() << " - pdgId() : " << mother->pdgId() << std::endl; // debug lines
          std::cout << "   mother eta : " << mother->eta() << ", pt : " << mother->pt() <<  ", phi : " << mother->phi() << std::endl;
        }
      }  // end of mother if test

    }  // end of for loop
    if (!matchingMotherID) {
      continue;
    }

    // electron preselection
    if (gp.pt() > maxPt_ || std::abs(gp.eta()) > maxAbsEta_) {
      continue;
    }

    // find best matched electron
    double gsfOkRatio = 999999.;
    pat::Electron bestGsfElectron;

    for (unsigned i_elec = 0; i_elec < 2; ++i_elec) {
      mergedElectrons = (i_elec == 0) ? electrons : electrons_endcaps;
      std::string eleName = (i_elec == 0) ? "electrons" : "electrons_endcaps";
      //std::cout << "   \033[33m" + eleName + "\033[0m" << std::endl;

      for (const pat::Electron& el : *mergedElectrons) {  // *electrons
        if (i_elec == 0 && !el.isEB())
          continue;
        double dphi = el.phi() - gp.phi();
        if (std::abs(dphi) > CLHEP::pi) {
          dphi = dphi < 0 ? (CLHEP::twopi) + dphi : dphi - CLHEP::twopi;
        }
        double deltaR2 = (el.eta() - gp.eta()) * (el.eta() - gp.eta()) + dphi * dphi;
        if (deltaR2 < deltaR2_) {
          if (((gp.pdgId() == 11) && (el.charge() < 0.)) ||
              ((gp.pdgId() == -11) && (el.charge() > 0.))) {
            double tmpGsfRatio = el.p() / gp.p();
            if (std::abs(tmpGsfRatio - 1) < std::abs(gsfOkRatio - 1)) {
              gsfOkRatio = tmpGsfRatio;
              bestGsfElectron = el;
              //PatElectronPtr elePtr(electrons, &el - &(*electrons)[0]);
              //std::cout << "evt ID = " << iEvent.id() ;   
              std::cout << "   \033[33m" + eleName + "\033[0m" << std::endl;   
              std::cout << "   mother eta : " << mother->eta() << ", pt : " << mother->pt() <<  ", phi : " << mother->phi() << std::endl;
              std::cout << "     eta : " << el.eta() << ", pt : " << el.pt() <<  ", phi : " << el.phi();
              std::cout << ", sigmaEtaEta : " << el.sigmaEtaEta() << ", p : " << el.p() << std::endl;
              std::cout << "     bestGsfElectron.eta() : " << bestGsfElectron.eta() << ", bestGsfElectron.p() : " << bestGsfElectron.p();
              std::cout << ", (*genParticles)[i].p() : " << (*genParticles)[i].p() << std::endl; // << mcIter->p()
             std::cout << "     rapport 1 : " << gp.pt() * bestGsfElectron.p() / (*genParticles)[i].p() << std::endl;
            }
          }
        }
      }
    }  // end loop i_elec

  }  // end loop size_t i
}