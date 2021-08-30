import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaElectronProducers.gedGsfElectronFinalizer_cfi import gedGsfElectrons as _gedGsfElectrons

electronMcSignalHistosCfg = cms.PSet()

electronPFIsolationCfg = cms.PSet(
    pfSumChargedHadronPtTmp=cms.InputTag("miniAODElectronIsolation",
                                         _gedGsfElectrons.pfIsolationValues.pfSumChargedHadronPt.getProductInstanceLabel()),
    pfSumNeutralHadronEtTmp=cms.InputTag("miniAODElectronIsolation",
                                         _gedGsfElectrons.pfIsolationValues.pfSumNeutralHadronEt.getProductInstanceLabel()),
    pfSumPhotonEtTmp=cms.InputTag("miniAODElectronIsolation",
                                  _gedGsfElectrons.pfIsolationValues.pfSumPhotonEt.getProductInstanceLabel()),  #
)

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

chichiMiniAODAnalyzer = DQMEDAnalyzer('chichiMiniAODAnalyzer',
    Verbosity=cms.untracked.int32(0),
    FinalStep=cms.string("AtJobEnd"),
    InputFile=cms.string(""),
    OutputFile=cms.string(""),

    mcTruthCollection=cms.InputTag("prunedGenParticles"),
    electrons=cms.InputTag("slimmedElectrons"),
    electrons_endcaps=cms.InputTag("slimmedElectrons"),

    MaxPt=cms.double(100.0),
    DeltaR=cms.double(0.05),
    MaxAbsEta=cms.double(2.5),
    MatchingID=cms.vint32(11, -11),
    MatchingMotherID=cms.vint32(23, 24, -24, 32),
    histosCfg=cms.PSet(electronMcSignalHistosCfg),
    isolationCfg=cms.PSet(electronPFIsolationCfg),
)

from Configuration.Eras.Modifier_phase2_hgcal_cff import phase2_hgcal
phase2_hgcal.toModify(
    chichiMiniAODAnalyzer,
    electrons_endcaps=cms.InputTag("slimmedElectronsFromMultiCl"),
    MaxAbsEta=cms.double(3.0),
)