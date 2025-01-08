#ifndef DSelector_kskpi_h
#define DSelector_kskpi_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TH3F.h"

#include "TNtuple.h"

class DSelector_kskpi : public DSelector
{
	public:

		DSelector_kskpi(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_kskpi(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dPiMinus1Wrapper;
		DChargedTrackHypothesis* dKPlusWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DChargedTrackHypothesis* dPiMinus2Wrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// My stuff //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		TH1F* dHist_RFTime;

		double	mks_nbin = 300, mks_lbin = 0.35, mks_ubin = 0.65,
			mkpi_nbin = 225, mkpi_lbin = .6, mkpi_ubin = 1.05,
			mkskpi_nbin = 300, mkskpi_lbin = 1, mkskpi_ubin = 4,
			theta_nbin = 360, theta_lbin = 0, theta_ubin = 180,
			phi_nbin = 360, phi_lbin = -180, phi_ubin = 180;

		TH1F *im_ks_meas;
		TH1F *im_ks_kinf;

		TH1F *im_kskpi_meas;
		TH1F *im_kskpi;

//		TH1F *im_kppim;
//		TH1F *im_kspim;
//
//		TH1F *mom_pi_meas;
//		TH1F *mom_pi_kinf;

		TH1F *h1_rftime;
		TH1F *h1_mandelstam_t;
		TH1F *h1_flight_significance;
		TH1F *h1_chisq_ndf;
		TH1F *h1_KinFitCl;

//		TH2F 	*h2_mkskpi_mkspim,
//			*h2_mkskpi_mkppim;

		TH2F *h2_ks_mom_theta;
		TH2F *h2_kp_mom_theta;
		TH2F *h2_pr_mom_theta;
		TH2F *h2_pip_mom_theta;
		TH2F *h2_pim1_mom_theta;
		TH2F *h2_pim2_mom_theta;

		TH2F *h2_mks_ksmom_meas;
		TH2F *h2_mks_ksmom_kinf;

		TH2F *h2_mks_kstheta_meas;
		TH2F *h2_mks_kstheta_kinf;

		TH2F *h2_mks_ksphi_meas;
		TH2F *h2_mks_ksphi_kinf;

		TH2F *h2_mks_ksphi_kinf_fdc_noDeadZone;

		TH2F *h2_mks_pimom_meas;
		TH2F *h2_mks_pimom_kinf;

		TH2F *h2_mks_pitheta_meas;
		TH2F *h2_mks_pitheta_kinf;

		TH2F *h2_mks_piphi_meas;
		TH2F *h2_mks_piphi_kinf;

		TH2F *h2_mks_kpmom_meas;
		TH2F *h2_mks_kpmom_kinf;

		TH2F *h2_mks_kptheta_meas;
		TH2F *h2_mks_kptheta_kinf;

		TH2F *h2_mks_kpphi_meas;
		TH2F *h2_mks_kpphi_kinf;

		TH2F *h2_mks_proton_phi_meas;
		TH2F *h2_mks_proton_phi_kinf;

		TH2F *h2_mks_kspip_phi_meas;
		TH2F *h2_mks_kspip_phi_kinf;

		TH2F *h2_mks_kspim_phi_meas;
		TH2F *h2_mks_kspim_phi_kinf;

		TH2F *h2_kspiptheta_kspimtheta;
		TH2F *h2_kspipphi_kspimphi;

		TH3F *h3_mks_ksmom_kstheta_meas;
		TH3F *h3_mks_ksmom_kstheta_kinf;

		TH3F *h3_mks_ksmom_ksphi_meas;
		TH3F *h3_mks_ksmom_ksphi_kinf;

		TH3F *h3_mks_kspiptheta_kspipphi_meas;
		TH3F *h3_mks_kspiptheta_kspipphi_kinf;

		TH3F *h3_mks_kspimtheta_kspimphi_meas;
		TH3F *h3_mks_kspimtheta_kspimphi_kinf;

		TH3F *h3_mks_kstheta_ksphi_meas;
		TH3F *h3_mks_kstheta_ksphi_kinf;

		TH3F *h3_mks_pimom_pitheta_meas;
		TH3F *h3_mks_pimom_pitheta_kinf;

		TH3F *h3_mks_pimom_piphi_meas;
		TH3F *h3_mks_pimom_piphi_kinf;

		TH3F *h3_mks_pitheta_piphi_meas;
		TH3F *h3_mks_pitheta_piphi_kinf;

		TH3F *h3_mks_kpmom_kpphi_meas;
		TH3F *h3_mks_kpmom_kpphi_kinf;

		TH3F *h3_mks_kptheta_kpphi_meas;
		TH3F *h3_mks_kptheta_kpphi_kinf;

		TNtuple *ntuple_mkskpi;

TH2F *tmp;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ClassDef(DSelector_kskpi, 0);
};

void DSelector_kskpi::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPiMinus1Wrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dKPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(3));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dPiMinus2Wrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_kskpi_h
