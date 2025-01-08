#include "DSelector_kskpi.h"

void DSelector_kskpi::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "kskpi.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = ""; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = ""; //if blank, default name will be chosen

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	Get_ComboWrappers();
	dPreviousRunNumber = 0;

	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	std::deque<Particle_t> MyPhi;
	MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.4, 9.05));

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Initialize my histograms ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char txt[100];

	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	//		1D histograms		       //
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////

	dHist_RFTime = new TH1F("RFTime", "; RF Time (ns)", 200, -20.0, 20.0);

	im_ks_meas = new TH1F("im_ks_meas", "M(K_{S}) of measured variables, only accidental subtracted ; M(K_{S}) (GeV)", mks_nbin, mks_lbin, mks_ubin );
		sprintf(txt, "Counts / %.3f", im_ks_meas->GetBinWidth(1));
		im_ks_meas->GetYaxis()->SetTitle(txt);
	im_ks_kinf = new TH1F("im_ks_kinf", "M(K_{S}) of KinFit variables, only accidental subtracted ; M(K_{S}) (GeV) ;", mks_nbin, mks_lbin, mks_ubin );
		sprintf(txt, "Counts / %.3f", im_ks_kinf->GetBinWidth(1));
		im_ks_kinf->GetYaxis()->SetTitle(txt);

	im_kskpi_meas = new TH1F("im_kskpi_meas", "; M(K_{S}K^{+}#pi^{-}) (GeV);", mkskpi_nbin, mkskpi_lbin, mkskpi_ubin);
		sprintf(txt, "Counts / %.3f", im_kskpi_meas->GetBinWidth(1));
		im_kskpi_meas->GetYaxis()->SetTitle(txt);
	im_kskpi = new TH1F("im_kskpi", "; M(K_{S}K^{+}#pi^{-}) (GeV);", mkskpi_nbin, mkskpi_lbin, mkskpi_ubin);
		sprintf(txt, "Counts / %.3f", im_kskpi->GetBinWidth(1));
		im_kskpi->GetYaxis()->SetTitle(txt);

//	im_kppim = new TH1F("im_kppim", "; M(K^{+}#pi^{-}) (GeV)", 225, 0.6, 1.05);
//		sprintf(txt, "Counts / %.3f", im_kppim->GetBinWidth(1));
//		im_kppim->GetYaxis()->SetTitle(txt);
//	im_kspim = new TH1F("im_kspim", "; M(K_{S}#pi^{-}) (GeV)", 225, 0.6, 1.05);
//		sprintf(txt, "Counts / %.3f", im_kspim->GetBinWidth(1));
//		im_kspim->GetYaxis()->SetTitle(txt);

	h1_rftime = new TH1F("h1_rftime", ";RF Time (ns);Counts", 200, -20.0, 20.0);
	h1_mandelstam_t = new TH1F("h1_mandelstam_t", "; -t (GeV^{2}) ;", 200, 0, 2);
	h1_flight_significance = new TH1F("flight_significance", "; FS ; Counts", 100, 0, 10);
	h1_chisq_ndf = new TH1F("h1_chisq_ndf", "; #chi^{2}/ndf;Counts", 200, 0, 20);
	h1_KinFitCl = new TH1F("h1_KinFitCl", ";Kinematic Fit Confidence Level;Counts", 100, 0, 1);

	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	//		2D histograms		       //
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////

//	h2_mkskpi_mkspim = new TH2F("h2_mkskpi_mkspim", "; M(K_{S}^{0}K^{+}#pi^{-}) ; M(K_{S}^{0}#pi^{-})", mkskpi_nbin, mkskpi_lbin, mkskpi_ubin, mkpi_nbin, mkpi_lbin, mkpi_ubin);
//	h2_mkskpi_mkppim = new TH2F("h2_mkskpi_mkppim", "; M(K_{S}^{0}K^{+}#pi^{-}) ; M(K^{+}#pi^{-})", mkskpi_nbin, mkskpi_lbin, mkskpi_ubin, mkpi_nbin, mkpi_lbin, mkpi_ubin);

	h2_ks_mom_theta = new TH2F("h2_ks_mom_theta", "; K_{S} Momentum (GeV) ; K_{S} polar angle", 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);
	h2_kp_mom_theta = new TH2F("h2_kp_mom_theta", "; K^{+} Momentum (GeV) ; K^{+} polar angle", 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);
	h2_pr_mom_theta = new TH2F("h2_pr_mom_theta", "; recoil proton Momentum (GeV) ; recoil proton  polar angle", 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);
	h2_pip_mom_theta = new TH2F("h2_pip_mom_theta", "; #pi^{+} Momentum (GeV) ; #pi^{+} polar angle", 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);
	h2_pim1_mom_theta = new TH2F("h2_pim1_mom_theta", "; #pi^{-}_{1} Momentum (GeV) ; #pi^{-}_{1} polar angle", 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);
	h2_pim2_mom_theta = new TH2F("h2_pim2_mom_theta", "; #pi^{-}_{2} Momentum (GeV) ; #pi^{-}_{2} polar angle", 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);

	h2_mks_ksmom_meas = new TH2F("h2_mks_ksmom_meas", "measured values ; M(K_{S}^{0}) (GeV) ; K_{S} momentum", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7);
	h2_mks_ksmom_kinf = new TH2F("h2_mks_ksmom_kinf", "KinFit values ; M(K_{S}^{0}) (GeV) ; K_{S} momentum", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7);

	h2_mks_kstheta_meas = new TH2F("h2_mks_kstheta_meas", "measured values ; M(K_{S}^{0}) (GeV) ; #theta(K_{S})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin); 
	h2_mks_kstheta_kinf = new TH2F("h2_mks_kstheta_kinf", "KinFit values ; M(K_{S}^{0}) (GeV) ; #theta(K_{S})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin); 

	h2_mks_ksphi_meas = new TH2F("h2_mks_ksphi_meas", "measured values ; M(K_{S}^{0}) (GeV) ; #phi(K_{S})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin); 
	h2_mks_ksphi_kinf = new TH2F("h2_mks_ksphi_kinf", "KinFit values ; M(K_{S}^{0}) (GeV) ; #phi(K_{S})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin); 

	h2_mks_ksphi_kinf_fdc_noDeadZone = new TH2F("h2_mks_ksphi_kinf_fdc_noDeadZone", "KinFit values ; M(K_{S}^{0}) (GeV) ; #phi(K_{S})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin); 

	h2_mks_pimom_meas = new TH2F("h2_mks_pimom_meas", "measured values ; M(K_{S}^{0}) ; #pi^{-} momentum", mks_nbin, mks_lbin, mks_ubin, 200, 0, 20);
	h2_mks_pimom_kinf = new TH2F("h2_mks_pimom_kinf", "KinFit values ; M(K_{S}^{0}) ; #pi^{-} momentum", mks_nbin, mks_lbin, mks_ubin, 200, 0, 20);

	h2_mks_pitheta_meas = new TH2F("h2_mks_pitheta_meas", "reconstructed ; M(K_{S}^{0}) (GeV) ; #theta(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin);
	h2_mks_pitheta_kinf = new TH2F("h2_mks_pitheta_kinf", "Kinematic Fit ; M(K_{S}^{0}) (GeV) ; #theta(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin);

	h2_mks_piphi_meas = new TH2F("h2_mks_piphi_meas", "reconstructed ; M(K_{S}^{0}) (GeV) ; #phi(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);
	h2_mks_piphi_kinf = new TH2F("h2_mks_piphi_kinf", "Kinematic Fit ; M(K_{S}^{0}) (GeV) ; #phi(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);

	h2_mks_kpmom_meas = new TH2F("h2_mks_kpmom_meas", "; M(K_{S}^{0}) ; K^{+} momentum", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7);
	h2_mks_kpmom_kinf = new TH2F("h2_mks_kpmom_kinf", "; M(K_{S}^{0}) ; K^{+} momentum", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7);

	h2_mks_kptheta_meas = new TH2F("h2_mks_kptheta_meas", "; M(K_{S}^{0}) ; #theta(K^{+})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin);
	h2_mks_kptheta_kinf = new TH2F("h2_mks_kptheta_kinf", "; M(K_{S}^{0}) ; #theta(K^{+})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin);

	h2_mks_kpphi_meas = new TH2F("h2_mks_kpphi_meas", "; M(K_{S}^{0}) ; #phi(K^{+})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);
	h2_mks_kpphi_kinf = new TH2F("h2_mks_kpphi_kinf", "; M(K_{S}^{0}) ; #phi(K^{+})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);

	h2_mks_proton_phi_meas = new TH2F("h2_mks_proton_phi_meas", "; M(K_{S}^{0}) ; #phi(recoil proton)", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);
	h2_mks_proton_phi_kinf = new TH2F("h2_mks_proton_phi_kinf", "; M(K_{S}^{0}) ; #phi(recoil proton)", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);

	h2_mks_kspip_phi_meas = new TH2F("h2_mks_kspip_phi_meas", "; M(K_{S}^{0}) ; #phi(#pi^{+})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);
	h2_mks_kspip_phi_kinf = new TH2F("h2_mks_kspip_phi_kinf", "; M(K_{S}^{0}) ; #phi(#pi^{+})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);

	h2_mks_kspim_phi_meas = new TH2F("h2_mks_kspim_phi_meas", "; M(K_{S}^{0}) ; #phi(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);
	h2_mks_kspim_phi_kinf = new TH2F("h2_mks_kspim_phi_kinf", "; M(K_{S}^{0}) ; #phi(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, phi_nbin, phi_lbin, phi_ubin);

	h2_kspiptheta_kspimtheta = new TH2F("h2_kspiptheta_kspimtheta", "; #theta(#pi^{+}_{K_{S}}) ; #theta(#pi^{-}_{K_{S}})", theta_nbin, theta_lbin, theta_ubin, theta_nbin, theta_lbin, theta_ubin);
	h2_kspipphi_kspimphi = new TH2F("h2_kspipphi_kspimphi", "; #phi(#pi^{+}_{K_{S}}) ; #phi(#pi^{-}_{K_{S}})", phi_nbin, phi_lbin, phi_ubin, phi_nbin, phi_lbin, phi_ubin);

	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	//		3D histograms		       //
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////

	h3_mks_ksmom_kstheta_meas = new TH3F("h3_mks_ksmom_kstheta_meas", "reconstructed ; M(K_{S}^{0}) ; K_{S} momentum ; #theta(K_{S})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);
	h3_mks_ksmom_kstheta_kinf = new TH3F("h3_mks_ksmom_kstheta_kinf", "Kinematic Fit ; M(K_{S}^{0}) ; K_{S} momentum ; #theta(K_{S})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);

	h3_mks_ksmom_ksphi_meas = new TH3F("h3_mks_ksmom_ksphi_meas", "reconstructed ; M(K_{S}^{0}) ; K_{S} momentum ; #phi(K_{S})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_ksmom_ksphi_kinf = new TH3F("h3_mks_ksmom_ksphi_kinf", "kinematic fit ; M(K_{S}^{0}) ; K_{S} momentum ; #phi(K_{S})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, phi_nbin, phi_lbin, phi_ubin);

	h3_mks_kstheta_ksphi_meas = new TH3F("h3_mks_kstheta_ksphi_meas", "reconstructed ; M(K_{S}^{0}) ; #theta(K_{S}) ; #phi(K_{S})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_kstheta_ksphi_kinf = new TH3F("h3_mks_kstheta_ksphi_kinf", "Kinematic Fit ; M(K_{S}^{0}) ; #theta(K_{S}) ; #phi(K_{S})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);

	h3_mks_kspiptheta_kspipphi_meas = new TH3F("h3_mks_kspiptheta_kspipphi_meas", "reconstructed ; M(K_{S}^{0}) ; K_{S} momentum ; #phi(#pi^{+}_{K_{S}})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_kspiptheta_kspipphi_kinf = new TH3F("h3_mks_kspiptheta_kspipphi_kinf", "kinematic fit ; M(K_{S}^{0}) ; K_{S} momentum ; #phi(#pi^{+}_{K_{S}})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);

	h3_mks_kspimtheta_kspimphi_meas = new TH3F("h3_mks_kspimtheta_kspimphi_meas", "reconstructed ; M(K_{S}^{0}) ; K_{S} momentum ; #phi(#pi^{-}_{K_{S}})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_kspimtheta_kspimphi_kinf = new TH3F("h3_mks_kspimtheta_kspimphi_kinf", "kinematic fit ; M(K_{S}^{0}) ; K_{S} momentum ; #phi(#pi^{-}_{K_{S}})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);

	h3_mks_pimom_pitheta_meas = new TH3F("h3_mks_pimom_pitheta_meas", "reconstructed ; M(K_{S}^{0}) ; #pi^{-} momentum ; #theta(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);
	h3_mks_pimom_pitheta_kinf = new TH3F("h3_mks_pimom_pitheta_kinf", "Kinematic Fit ; M(K_{S}^{0}) ; #pi^{-} momentum ; #theta(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, theta_nbin, theta_lbin, theta_ubin);

	h3_mks_pimom_piphi_meas = new TH3F("h3_mks_pimom_piphi_meas", "reconstructed ; M(K_{S}^{0}) ; #pi^{-} momentum ; #phi(#pi^{-})",  mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_pimom_piphi_kinf = new TH3F("h3_mks_pimom_piphi_kinf", "kinematic fit ; M(K_{S}^{0}) ; #pi^{-} momentum ; #phi(#pi^{-})",  mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, phi_nbin, phi_lbin, phi_ubin);

	h3_mks_pitheta_piphi_meas = new TH3F("h3_mks_pitheta_piphi_meas", "reconstructed ; M(K_{S}^{0}) ; #theta(#pi^{-}) ; #phi(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_pitheta_piphi_kinf = new TH3F("h3_mks_pitheta_piphi_kinf", "kinematic fit ; M(K_{S}^{0}) ; #theta(#pi^{-}) ; #phi(#pi^{-})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);

	h3_mks_kpmom_kpphi_meas = new TH3F("h3_mks_kpmom_kpphi_meas", "; M(K_{S}^{0}) ; K^{+} momentum ; #phi(K^{+})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_kpmom_kpphi_kinf = new TH3F("h3_mks_kpmom_kpphi_kinf", "; M(K_{S}^{0}) ; K^{+} momentum ; #phi(K^{+})", mks_nbin, mks_lbin, mks_ubin, 70, 0, 7, phi_nbin, phi_lbin, phi_ubin);

	h3_mks_kptheta_kpphi_meas = new TH3F("h3_mks_kptheta_kpphi_meas", "; M(K_{S}^{0}) : #theta(K^{+}) ; #phi(K^{+})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);
	h3_mks_kptheta_kpphi_kinf = new TH3F("h3_mks_kptheta_kpphi_kinf", "; M(K_{S}^{0}) : #theta(K^{+}) ; #phi(K^{+})", mks_nbin, mks_lbin, mks_ubin, theta_nbin, theta_lbin, theta_ubin, phi_nbin, phi_lbin, phi_ubin);

	//////////////////////////////////////////
	//////////////////////////////////////////
	//			     		//
	//		TNtuple 		//
	//			     		//
	//////////////////////////////////////////
	//////////////////////////////////////////

	ntuple_mkskpi = new TNtuple("ntuple_mkskpi", "ntuple_mkskpi", "run_number:mks:mkp:mpim:mkskppim:mkspim:mkppim:chisq_ndf:fs:beam_w");

//tmp = new TH2F("tmp", "", 70, 0, 7, 120, 0, 12);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dTreeInterface->Create_Branch_Fundamental<Int_t>("my_int"); //fundamental = char, int, float, double, etc.
	dTreeInterface->Create_Branch_FundamentalArray<Int_t>("my_int_array", "my_int");
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("my_combo_array", "NumCombos");
	dTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("my_p4");
	dTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("my_p4_array");
	*/

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("flat_my_int"); //fundamental = char, int, float, double, etc.
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("flat_my_int_array", "flat_my_int");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("flat_my_p4");
	dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("flat_my_p4_array");
	*/

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
}

Bool_t DSelector_kskpi::Process(Long64_t locEntry)
{
	// The Process() function is called for each entry in the tree. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	// Use fStatus to set the return value of TTree::Process().
	// The return value is currently not used.

	//CALL THIS FIRST
	DSelector::Process(locEntry); //Gets the data from the tree for the entry
	//cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;
	TLorentzVector locProductionX4 = Get_X4_Production();

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
		//Sometimes, some content is the exact same between one combo and the next
			//e.g. maybe two combos have different beam particles, but the same data for the final-state
		//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
		//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
		//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// ANALYSIS UNIQUENESS TRACKING  //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	set<map<Particle_t, set<Int_t> > > locUsedSoFar_KsKPiMass;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

	/*
	Int_t locMyInt = 7;
	dTreeInterface->Fill_Fundamental<Int_t>("my_int", locMyInt);

	TLorentzVector locMyP4(4.0, 3.0, 2.0, 1.0);
	dTreeInterface->Fill_TObject<TLorentzVector>("my_p4", locMyP4);

	for(int loc_i = 0; loc_i < locMyInt; ++loc_i)
		dTreeInterface->Fill_Fundamental<Int_t>("my_int_array", 3*loc_i, loc_i); //2nd argument = value, 3rd = array index
	*/

	/************************************************* LOOP OVER COMBOS *************************************************/

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
			continue; // Combo has been cut previously

		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locPiMinus1TrackID = dPiMinus1Wrapper->Get_TrackID();
		Int_t locKPlusTrackID = dKPlusWrapper->Get_TrackID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPiMinus2TrackID = dPiMinus2Wrapper->Get_TrackID();
		Int_t locPiPlusTrackID = dPiPlusWrapper->Get_TrackID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locPiMinus1P4 = dPiMinus1Wrapper->Get_P4();
		TLorentzVector locKPlusP4 = dKPlusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locPiMinus2P4 = dPiMinus2Wrapper->Get_P4();
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiMinus1P4_Measured = dPiMinus1Wrapper->Get_P4_Measured();
		TLorentzVector locKPlusP4_Measured = dKPlusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPiMinus2P4_Measured = dPiMinus2Wrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiMinus1P4_Measured + locKPlusP4_Measured + locProtonP4_Measured + locPiMinus2P4_Measured + locPiPlusP4_Measured;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Define vectors and variables //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		TLorentzVector locBeamX4 = dComboBeamWrapper->Get_X4();
		TLorentzVector locKsKPiP4 = locKPlusP4 + locPiMinus1P4 + (locPiMinus2P4 + locPiPlusP4);
		TLorentzVector locKsKPiP4_Measured = locKPlusP4_Measured + locPiMinus1P4_Measured + (locPiMinus2P4_Measured + locPiPlusP4_Measured);
		TLorentzVector locPiPiP4_Measured = locPiMinus2P4_Measured + locPiPlusP4_Measured;

		// Define more 4-vectors

			// Missing mass 4 vector
		TLorentzVector locMissingP4 = locBeamP4 + dTargetP4;
		locMissingP4 -= locPiMinus1P4 + locKPlusP4 + locProtonP4 + locPiMinus2P4 + locPiPlusP4;

		// PiMinus1
			// Position 4-vector
		TLorentzVector locPiMinus1X4 = dPiMinus1Wrapper->Get_X4();
		TLorentzVector locPiMinus1X4_Measured = dPiMinus1Wrapper->Get_X4_Measured();

		// KPlus
			// Position 4-cvector
		TLorentzVector locKPlusX4 = dKPlusWrapper->Get_X4();
		TLorentzVector locKPlusX4_Measured = dKPlusWrapper->Get_X4_Measured();

		// PiPlus
			// Position 4-vector
		TLorentzVector locPiPlusX4 = dPiPlusWrapper->Get_X4();
		TLorentzVector locPiPlusX4_Measured = dPiPlusWrapper->Get_X4_Measured();

		// PiMinus2
			// Position 4-vector
		TLorentzVector locPiMinus2X4 = dPiMinus2Wrapper->Get_X4();
		TLorentzVector locPiMinus2X4_Measured = dPiMinus2Wrapper->Get_X4_Measured();

		// KShort
			// Position 4-vector
		TLorentzVector locKShortX4 = locPiPlusX4;
		TLorentzVector locKShortX4_Measured = locPiPlusX4_Measured;

			// Momentum 4-vector
		TLorentzVector locPiPiP4 = locPiMinus2P4 + locPiPlusP4;

			// Vertex
		TLorentzVector locVertexKShort = locPiMinus2X4 - locPiPlusX4;
		TLorentzVector locVertexKShort_Measured = locPiMinus2X4_Measured - locPiPlusX4_Measured;

		// Proton
			// Position 4-vector
		TLorentzVector locProtonX4 = dProtonWrapper->Get_X4();
		TLorentzVector locProtonX4_Measured = dProtonWrapper->Get_X4_Measured();

		// Proton + pi+
		TLorentzVector locPPiPlusP4 = locPiPlusP4 + locProtonP4;

		// Flight significance
		TLorentzVector locPiPi_X4 = dTreeInterface->Get_TObject<TLorentzVector>("DecayingKShort__X4", loc_i);
		TLorentzVector locDeltaSpacetime = locProtonX4 - locPiPi_X4;

		Double_t locPathLength = locDeltaSpacetime.Vect().Mag(); //pathlength is just the magnitude
		Float_t locPathLengthSigma = Get_Fundamental<Float_t>("DecayingKShort__PathLengthSigma", loc_i); //Get Sigma from analysis tree

		Double_t locPathLengthSignificance = locPathLength/locPathLengthSigma; //Calculate significance
		Double_t locDetachedLifeTime = locDeltaSpacetime.T(); //Easy to get lifetime!

		// K+ + pi- and Ks + pi-
		TLorentzVector locKPiP4 = locKPlusP4 + locPiMinus1P4;
		TLorentzVector locKsPiP4 = locPiPiP4 + locPiMinus1P4;

		// Mandelstam-t
		Double_t t = (locProtonP4 - dTargetP4).M2();

		// invariant mass variables
		double mks_meas = locPiPiP4_Measured.M();
		double mks_kinf = locPiPiP4.M();

		double mkp_kinf = locKPlusP4.M();
		double mpim_kinf = locPiMinus1P4.M();
		double mkskppim_kinf = locKsKPiP4.M();
		double mkppim_kinf = locKPiP4.M();
		double mkspim_kinf = locKsPiP4.M();

		double ksmom_meas = locPiPiP4_Measured.Vect().Mag();
		double ksmom_kinf = locPiPiP4.Vect().Mag();

		double pimom_meas = locPiMinus1P4_Measured.Vect().Mag();
		double pimom_kinf = locPiMinus1P4.Vect().Mag();

		double kpmom_meas = locKPlusP4_Measured.Vect().Mag();
		double kpmom_kinf = locKPlusP4.Vect().Mag();

		// angle variables
		double conv = (180 / TMath::Pi());

		double ks_meas_theta = locPiPiP4_Measured.Theta() * conv;
		double ks_kinf_theta = locPiPiP4.Theta() * conv;
		double ks_meas_phi = locPiPiP4_Measured.Phi() * conv;
		double ks_kinf_phi = locPiPiP4.Phi() * conv;

		double pi_meas_theta = locPiMinus1P4_Measured.Theta() * conv;
		double pi_kinf_theta = locPiMinus1P4.Theta() * conv;
		double pi_meas_phi = locPiMinus1P4_Measured.Phi() * conv;
		double pi_kinf_phi = locPiMinus1P4.Phi() * conv;

		double kp_meas_theta = locKPlusP4_Measured.Theta() * conv;
		double kp_kinf_theta = locKPlusP4.Theta() * conv;
		double kp_meas_phi = locKPlusP4_Measured.Phi() * conv;
		double kp_kinf_phi = locKPlusP4.Phi() * conv;

		double proton_phi_meas = locProtonP4_Measured.Phi() * conv;
		double proton_phi_kinf = locProtonP4.Phi() * conv;

		// cuts
		double chisq_ndf = dComboWrapper->Get_ChiSq_KinFit("")/dComboWrapper->Get_NDF_KinFit("");
		double fs = locPathLengthSignificance;

		bool MMS = ((locMissingP4_Measured.M2() > -0.04) || (locMissingP4_Measured.M2() < 0.04));
		bool KinFitCl = (dComboWrapper->Get_ConfidenceLevel_KinFit("") > 0.01);
		bool ChiSq_ndf = (dComboWrapper->Get_ChiSq_KinFit("")/dComboWrapper->Get_NDF_KinFit("") < 5);

		double locDeltaTRF = locBeamX4.T() - (dComboWrapper->Get_RFTime() 
						  + (locBeamX4.Z() - dComboWrapper->Get_TargetCenter().Z() )/29.9792458);
		Double_t beam_w = 0.0;
		if(fabs(locDeltaTRF) < 2.)	beam_w = 1.0;
		else 				beam_w = -1.0/8.0;
	
		// find distance from vertex to fdc
		Float_t dz = 176.939-(Float_t)locProductionX4.Z();

		Float_t fdc_dr_kp = dz*tan(locKPlusP4.Theta());
		Float_t fdc_dr_pip = dz*tan(locPiPlusP4.Theta());
		Float_t fdc_dr_pim1 = dz*tan(locPiMinus1P4.Theta());
		Float_t fdc_dr_pim2 = dz*tan(locPiMinus2P4.Theta());

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E());
			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

//		//if(dComboWrapper->Get_ConfidenceLevel_KinFit() <= 0.) {
//		//if(dComboWrapper->Get_ConfidenceLevel_KinFit() < 1.E-20) { 
//		//if(dComboWrapper->Get_ConfidenceLevel_KinFit() < 5.73303E-7) {  // 5 sigma
//		if(dComboWrapper->Get_ConfidenceLevel_KinFit("") < 0.01) { 
//		//if(dComboWrapper->Get_ConfidenceLevel_KinFit() < 0.1) { 
//			dComboWrapper->Set_IsComboCut(true);
//			continue;
//		}
		
		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinus1TrackID);
		locUsedThisCombo_MissingMass[KPlus].insert(locKPlusTrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinus2TrackID);
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
//			dHist_MissingMassSquared->Fill(locMissingMassSquared);
			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

//		//E.g. Cut
//		if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
//		{
//			dComboWrapper->Set_IsComboCut(true);
//			continue;
//		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// Create maps  ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		map<Particle_t, set<Int_t> > locUsedThisCombo_KsKPiMass;
		locUsedThisCombo_KsKPiMass[Unknown].insert(locBeamID);
		locUsedThisCombo_KsKPiMass[PiMinus].insert(locPiMinus1TrackID);
		locUsedThisCombo_KsKPiMass[KPlus].insert(locKPlusTrackID);
		locUsedThisCombo_KsKPiMass[PiMinus].insert(locPiMinus2TrackID);
		locUsedThisCombo_KsKPiMass[PiPlus].insert(locPiPlusTrackID);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// Fill Histograms  //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//compare to what's been used so far
		if(locUsedSoFar_KsKPiMass.find(locUsedThisCombo_KsKPiMass) == locUsedSoFar_KsKPiMass.end()) {
			if(MMS && fs > 3 && (mks_kinf > 0.48 && mks_kinf < 0.52))		h1_chisq_ndf->Fill(dComboWrapper->Get_ChiSq_KinFit("")/dComboWrapper->Get_NDF_KinFit("") , beam_w);
			if(MMS && fs > 3 && (mks_kinf > 0.48 && mks_kinf < 0.52))		h1_KinFitCl->Fill(dComboWrapper->Get_ConfidenceLevel_KinFit(""), beam_w);
			if(KinFitCl && fs > 3 && (mks_kinf > 0.48 && mks_kinf < 0.52))		dHist_MissingMassSquared->Fill(locMissingMassSquared, beam_w);
			if(MMS && KinFitCl && (mks_kinf > 0.48 && mks_kinf < 0.52))		h1_flight_significance->Fill(locPathLengthSignificance, beam_w);

			if(MMS && KinFitCl && fs > 3) {

//				if(mks_kinf > 0.48 && mks_kinf < 0.52)	cout << Get_RunNumber() << " " << Get_EventNumber() << endl;

				h1_rftime->Fill(locDeltaTRF);

				im_ks_meas->Fill(mks_meas, beam_w);
				im_ks_kinf->Fill(mks_kinf, beam_w);

				if(mks_kinf > 0.48 && mks_kinf < 0.52) {
					im_kskpi_meas->Fill(locKsKPiP4_Measured.M(), beam_w);
					im_kskpi->Fill(locKsKPiP4.M(), beam_w);

					dHist_RFTime->Fill(locDeltaTRF);
					h1_mandelstam_t->Fill(-t, beam_w);
	
					h2_ks_mom_theta->Fill(ksmom_kinf, ks_kinf_theta, beam_w);
					h2_kp_mom_theta->Fill(kpmom_kinf, kp_kinf_theta, beam_w);
					h2_pr_mom_theta->Fill(locProtonP4.Vect().Mag(), locProtonP4.Theta() * conv, beam_w);
					h2_pip_mom_theta->Fill(locPiPlusP4.Vect().Mag(), locPiPlusP4.Theta() * conv, beam_w);
					h2_pim1_mom_theta->Fill(locPiMinus1P4.Vect().Mag(), locPiMinus1P4.Theta() * conv, beam_w);
					h2_pim2_mom_theta->Fill(locPiMinus2P4.Vect().Mag(), locPiMinus2P4.Theta() * conv, beam_w);
				}

				h2_mks_ksmom_meas->Fill(mks_meas, ksmom_meas, beam_w);
				h2_mks_ksmom_kinf->Fill(mks_kinf, ksmom_kinf, beam_w);
	
				h2_mks_kstheta_meas->Fill(mks_meas, ks_meas_theta, beam_w);
				h2_mks_kstheta_kinf->Fill(mks_kinf, ks_kinf_theta, beam_w);
	
				h2_mks_ksphi_meas->Fill(mks_meas, ks_meas_phi, beam_w);
				h2_mks_ksphi_kinf->Fill(mks_kinf, ks_kinf_phi, beam_w);
	
				if(fdc_dr_kp > 4 || fdc_dr_pip > 4 || fdc_dr_pim1 > 4 || fdc_dr_pim2 > 4)	h2_mks_ksphi_kinf_fdc_noDeadZone->Fill(mks_kinf, ks_kinf_phi, beam_w);
	
				h2_mks_pimom_meas->Fill(mks_meas, pimom_meas, beam_w);
				h2_mks_pimom_kinf->Fill(mks_kinf, pimom_kinf, beam_w);
	
				h2_mks_pitheta_meas->Fill(mks_meas, pi_meas_theta, beam_w);
				h2_mks_pitheta_kinf->Fill(mks_kinf, pi_kinf_theta, beam_w);

				h2_mks_piphi_meas->Fill(mks_meas, pi_meas_phi, beam_w);
				h2_mks_piphi_kinf->Fill(mks_kinf, pi_kinf_phi, beam_w);

				h2_mks_kpmom_meas->Fill(mks_meas, kpmom_meas, beam_w);
				h2_mks_kpmom_kinf->Fill(mks_kinf, kpmom_kinf, beam_w);
				
				h2_mks_kptheta_meas->Fill(mks_meas, kp_meas_theta, beam_w);
				h2_mks_kptheta_kinf->Fill(mks_kinf, kp_kinf_theta, beam_w);

				h2_mks_kpphi_meas->Fill(mks_meas, kp_meas_phi, beam_w);
				h2_mks_kpphi_kinf->Fill(mks_kinf, kp_kinf_phi, beam_w);

				h2_mks_proton_phi_meas->Fill(mks_meas, proton_phi_meas, beam_w);
				h2_mks_proton_phi_kinf->Fill(mks_kinf, proton_phi_kinf, beam_w);

				if(mks_kinf > 0.48 && mks_kinf < 0.52) {
					h2_kspiptheta_kspimtheta->Fill(locPiPlusP4.Theta()*conv, locPiMinus2P4.Theta()*conv, beam_w);
					h2_kspipphi_kspimphi->Fill(locPiPlusP4.Phi()*conv, locPiMinus2P4.Phi()*conv, beam_w);
				}

				//if(mks_kinf > 0.48 && mks_kinf < 0.52) {
//					h2_mks_kspip_phi_meas->Fill(mks_meas, locPiPlusP4_Measured.Phi()*conv, beam_w);
//					h2_mks_kspip_phi_kinf->Fill(mks_kinf, locPiPlusP4.Phi()*conv, beam_w);
//
//					h2_mks_kspim_phi_meas->Fill(mks_meas, locPiMinus2P4_Measured.Phi()*conv, beam_w);
//					h2_mks_kspim_phi_kinf->Fill(mks_kinf, locPiMinus2P4.Phi()*conv, beam_w);
				//}

				h3_mks_ksmom_kstheta_meas->Fill(mks_meas, ksmom_meas, ks_meas_theta, beam_w);
				h3_mks_ksmom_kstheta_kinf->Fill(mks_kinf, ksmom_kinf, ks_kinf_theta, beam_w);

				h3_mks_ksmom_ksphi_meas->Fill(mks_meas, ksmom_meas, ks_meas_phi, beam_w);
				h3_mks_ksmom_ksphi_kinf->Fill(mks_kinf, ksmom_kinf, ks_kinf_phi, beam_w);

				h3_mks_kspiptheta_kspipphi_meas->Fill(mks_meas, locPiPlusP4_Measured.Theta()*conv, locPiPlusP4_Measured.Phi()*conv, beam_w);
				h3_mks_kspiptheta_kspipphi_kinf->Fill(mks_kinf, locPiPlusP4.Theta()*conv, locPiPlusP4.Phi()*conv, beam_w);

				h3_mks_kspimtheta_kspimphi_meas->Fill(mks_meas, locPiMinus2P4_Measured.Theta()*conv, locPiMinus2P4_Measured.Phi()*conv, beam_w);
				h3_mks_kspimtheta_kspimphi_kinf->Fill(mks_kinf, locPiMinus2P4.Theta()*conv, locPiMinus2P4.Phi()*conv, beam_w);

				h3_mks_kstheta_ksphi_meas->Fill(mks_meas, ks_meas_theta, ks_meas_phi, beam_w);
				h3_mks_kstheta_ksphi_kinf->Fill(mks_kinf, ks_kinf_theta, ks_kinf_phi, beam_w);

				h3_mks_pimom_pitheta_meas->Fill(mks_meas, pimom_meas, pi_meas_theta, beam_w);
				h3_mks_pimom_pitheta_kinf->Fill(mks_kinf, pimom_kinf, pi_kinf_theta, beam_w);

				h3_mks_pimom_piphi_meas->Fill(mks_meas, pimom_meas, pi_meas_phi, beam_w);
				h3_mks_pimom_piphi_kinf->Fill(mks_kinf, pimom_kinf, pi_kinf_phi, beam_w);

				h3_mks_pitheta_piphi_meas->Fill(mks_meas, pi_meas_theta, pi_meas_phi, beam_w);
				h3_mks_pitheta_piphi_kinf->Fill(mks_kinf, pi_kinf_theta, pi_kinf_phi, beam_w);

				h3_mks_kpmom_kpphi_meas->Fill(mks_meas, kpmom_meas, kp_meas_phi, beam_w);
				h3_mks_kpmom_kpphi_kinf->Fill(mks_kinf, kpmom_kinf, kp_kinf_phi, beam_w);

				h3_mks_kptheta_kpphi_meas->Fill(mks_meas, kp_meas_theta, kp_meas_phi, beam_w);
				h3_mks_kptheta_kpphi_kinf->Fill(mks_meas, kp_kinf_theta, kp_kinf_phi, beam_w);
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		/*
		//FILL ANY CUSTOM BRANCHES FIRST!!
		Int_t locMyInt_Flat = 7;
		dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int", locMyInt_Flat);

		TLorentzVector locMyP4_Flat(4.0, 3.0, 2.0, 1.0);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4", locMyP4_Flat);

		for(int loc_j = 0; loc_j < locMyInt_Flat; ++loc_j)
		{
			dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int_array", 3*loc_j, loc_j); //2nd argument = value, 3rd = array index
			TLorentzVector locMyComboP4_Flat(8.0, 7.0, 6.0, 5.0);
			dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4_array", locMyComboP4_Flat, loc_j);
		}
		*/

		//FILL FLAT TREE
		//Fill_FlatTree(); //for the active combo
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
/*
	//Thrown beam: just use directly
	if(dThrownBeam != NULL)
		double locEnergy = dThrownBeam->Get_P4().E();

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/
	/****************************************** LOOP OVER OTHER ARRAYS (OPTIONAL) ***************************************/
/*
	//Loop over beam particles (note, only those appearing in combos are present)
	for(UInt_t loc_i = 0; loc_i < Get_NumBeam(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dBeamWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over charged track hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumNeutralHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dNeutralHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/

	/************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/
/*
	Bool_t locIsEventCut = true;
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);
		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut())
			continue;
		locIsEventCut = false; // At least one combo succeeded
		break;
	}
	if(!locIsEventCut && dOutputTreeFileName != "")
		Fill_OutputTree();
*/

	return kTRUE;
}

void DSelector_kskpi::Finalize(void)
{
	//Save anything to output here that you do not want to be in the default DSelector output ROOT file.

	//Otherwise, don't do anything else (especially if you are using PROOF).
		//If you are using PROOF, this function is called on each thread,
		//so anything you do will not have the combined information from the various threads.
		//Besides, it is best-practice to do post-processing (e.g. fitting) separately, in case there is a problem.

	//DO YOUR STUFF HERE

	//CALL THIS LAST
	DSelector::Finalize(); //Saves results to the output file
}
