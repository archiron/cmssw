import FWCore.ParameterSet.Config as cms

electronMcSignalHistosCfg = cms.PSet()

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

chichiRECOAnalyzer = DQMEDAnalyzer('chichiRECOAnalyzer',
  Verbosity = cms.untracked.int32(0),
  FinalStep = cms.string("AtJobEnd"),
  InputFile = cms.string(""),
  OutputFile = cms.string(""),

  mcTruthCollection = cms.InputTag("genParticles"),
  electronCollection = cms.InputTag("gedGsfElectrons"),
  electronCoreCollection = cms.InputTag("gedGsfElectronCores"),
  electronTrackCollection = cms.InputTag("electronGsfTracks"),
  electronSeedCollection = cms.InputTag("electronMergedSeeds"),
  offlinePrimaryVertices = cms.InputTag("offlinePrimaryVertices"),
  beamSpot = cms.InputTag("offlineBeamSpot"),
  readAOD = cms.bool(False),

  isoFromDepsTk03            = cms.InputTag(""),
  isoFromDepsTk04            = cms.InputTag(""),
  isoFromDepsEcalFull03      = cms.InputTag(""),
  isoFromDepsEcalFull04      = cms.InputTag(""),
  isoFromDepsEcalReduced03   = cms.InputTag(""),
  isoFromDepsEcalReduced04   = cms.InputTag(""),
  isoFromDepsHcal03          = cms.InputTag(""),
  isoFromDepsHcal04          = cms.InputTag(""),
  
  MaxPt = cms.double(100.0),
  DeltaR = cms.double(0.05),
  MaxAbsEta = cms.double(2.5),
  MatchingID = cms.vint32(11,-11),
  MatchingMotherID = cms.vint32(23,24,-24,32),
  histosCfg = cms.PSet(electronMcSignalHistosCfg)
)

from Configuration.Eras.Modifier_phase2_hgcal_cff import phase2_hgcal
phase2_hgcal.toModify(
    chichiRECOAnalyzer,
    electronCollection = 'ecalDrivenGsfElectronsFromMultiCl',
    electronCoreCollection = 'ecalDrivenGsfElectronCoresFromMultiCl',
    electronTrackCollection = 'electronGsfTracksFromMultiCl',
    electronSeedCollection = 'electronMergedSeedsFromMultiCl',
    MaxAbsEta = cms.double(3.0),
)
