#include "study_2d.C"

void run_analysis() {
	string root_file = "hist.root";
	string hist_name;
	string xtitle;
	vector<double> bins;

	bins = {-180, -156, -132, -108, -84, -60, -36, -12, 12, 36, 60, 84, 108, 132, 156, 180};
	hist_name = "h2_mks_ksphi_meas";
	xtitle = "K_{S} azimuthal angle (degrees)";
	study_2d(root_file, hist_name, bins, xtitle);

	hist_name = "h2_mks_ksphi_kinf";
	study_2d(root_file, hist_name, bins, xtitle);

		hist_name = "h2_mks_ksphi_kinf_fdc_noDeadZone";
		study_2d(root_file, hist_name, bins, xtitle);

	hist_name = "h2_mks_kpphi_meas";
	xtitle = "K^{+} azimuthal angle (degrees)";
	study_2d(root_file, hist_name, bins, xtitle);

	hist_name = "h2_mks_kpphi_kinf";
	study_2d(root_file, hist_name, bins, xtitle);

	hist_name = "h2_mks_piphi_meas";
	xtitle = "#pi^{-} azimuthal angle (degrees)";
	study_2d(root_file, hist_name, bins, xtitle);

	hist_name = "h2_mks_piphi_kinf";
	study_2d(root_file, hist_name, bins, xtitle);

	bins = {0.0, 0.7, 1.4, 2.1, 2.8, 3.5, 4.2, 4.9, 5.6, 6.3, 7.0};
	hist_name = "h2_mks_ksmom_meas";
	xtitle = "K_{S} momentum (GeV)";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_ksmom_kinf";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_kpmom_meas";
	xtitle = "K^{+} momentum (GeV)";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_kpmom_kinf";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_pimom_meas";
	xtitle = "#pi^{-} momentum (GeV)";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_pimom_kinf";
	study_2d(root_file, hist_name, bins, xtitle);
	
	bins = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30};
	hist_name = "h2_mks_kstheta_meas";
	xtitle = "K_{S} polar angle (degrees)";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_kstheta_kinf";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_kptheta_meas";
	xtitle = "K^{+} polar angle (degrees)";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_kptheta_kinf";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_pitheta_meas";
	xtitle = "#pi^{-} polar angle (degrees)";
	study_2d(root_file, hist_name, bins, xtitle);
	
	hist_name = "h2_mks_pitheta_kinf";
	study_2d(root_file, hist_name, bins, xtitle);
}
