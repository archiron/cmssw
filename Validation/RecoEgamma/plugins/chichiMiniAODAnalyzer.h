#ifndef Validation_RecoEgamma_chichiMiniAODAnalyzer_h
#define Validation_RecoEgamma_chichiMiniAODAnalyzer_h

#include "DQMOffline/EGamma/interface/ElectronDqmAnalyzerBase.h"

// user include files
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/PatCandidates/interface/Electron.h"

//
// class declaration
//
class chichiMiniAODAnalyzer : public ElectronDqmAnalyzerBase {
public:
  explicit chichiMiniAODAnalyzer(const edm::ParameterSet &);
  ~chichiMiniAODAnalyzer() override;
  bool isAncestor(const reco::Candidate *ancestor, const reco::Candidate *particle);

private:
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
  void analyze(const edm::Event &, const edm::EventSetup &) override;

  // ----------member data MiniAOD ---------------------------
  edm::EDGetTokenT<edm::View<reco::GenParticle> > mcTruthCollection_;  // prunedGenParticles
  edm::EDGetTokenT<pat::ElectronCollection> electronToken_;            // slimmedElectrons
  edm::EDGetTokenT<pat::ElectronCollection> electronTokenEndcaps_;     // slimmedElectrons

  double maxPt_;
  double maxAbsEta_;
  double deltaR_, deltaR2_;
  std::vector<int> matchingIDs_;
  std::vector<int> matchingMotherIDs_;

};

#endif