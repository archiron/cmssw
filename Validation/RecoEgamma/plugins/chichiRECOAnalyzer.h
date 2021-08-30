
#ifndef Validation_RecoEgamma_chichiRECOAnalyzer_h
#define Validation_RecoEgamma_chichiRECOAnalyzer_h

#include "DQMOffline/EGamma/interface/ElectronDqmAnalyzerBase.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
class MagneticField;

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

namespace reco {
  class BeamSpot;
}

class chichiRECOAnalyzer : public ElectronDqmAnalyzerBase {
public:
  explicit chichiRECOAnalyzer(const edm::ParameterSet &conf);
  ~chichiRECOAnalyzer() override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
  void analyze(const edm::Event &e, const edm::EventSetup &c) override;

private:
  edm::EDGetTokenT<reco::GenParticleCollection> mcTruthCollection_;           // genParticles
  edm::EDGetTokenT<reco::GsfElectronCollection> electronCollection_;          // gedGsfElectrons
  edm::EDGetTokenT<reco::GsfElectronCoreCollection> electronCoreCollection_;  // gedGsfElectronCore
  edm::EDGetTokenT<reco::GsfTrackCollection> electronTrackCollection_;
  edm::EDGetTokenT<reco::ElectronSeedCollection> electronSeedCollection_;
  edm::EDGetTokenT<reco::VertexCollection> offlineVerticesCollection_;
  edm::EDGetTokenT<reco::BeamSpot> beamSpotTag_;

  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsTk03Tag_;
  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsTk04Tag_;
  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsEcalFull03Tag_;
  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsEcalFull04Tag_;
  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsEcalReduced03Tag_;
  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsEcalReduced04Tag_;
  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsHcal03Tag_;
  edm::EDGetTokenT<edm::ValueMap<double> > isoFromDepsHcal04Tag_;

  edm::ESHandle<TrackerGeometry> pDD;
  edm::ESHandle<MagneticField> theMagField;

  double maxPt_;
  double maxAbsEta_;
  double deltaR_, deltaR2_;
  std::vector<int> matchingIDs_;
  std::vector<int> matchingMotherIDs_;

};

#endif
