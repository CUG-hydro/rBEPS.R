
// #include <stdio.h>
#include "rBEPS.h"


// read_metro(d, m_rad, m_tem, m_hum, m_pre, m_wind) 
void read_metro(DataFrame d, 
    float *m_rad, float *m_tem, float *m_hum, float *m_pre, float *m_wind) 
{
    const int nday = 365;
    // float m_rad[nday][24], m_tem[nday][24], m_hum[nday][24], m_pre[nday][24], m_wind[nday][24];
    NumericVector v_rad = d["rad"];
    NumericVector v_tem = d["tem"];
    NumericVector v_hum = d["hum"];
    NumericVector v_pre = d["pre"];
    NumericVector v_wind = d["wind"];

    int k = 0;
    // Read daily lai and hourly meteor. data
    for (int jday = 0; jday < 365; jday++) {
        // fscanf(laif_ptr, "%f ", &lai_p[jday]);  // for fixed Vcmax
        for (int rstep = 0; rstep < 24; rstep++) {
            k = jday*24 + rstep;
            // read climate data
            m_rad[k] = v_rad[k];
            m_tem[k] = v_tem[k];
            m_hum[k] = v_hum[k];
            m_pre[k] = v_pre[k] / 1000;
            m_wind[k] = v_wind[k];
            // k += 1;
        }  // end of hour loop
    }      // end of day loop
}

//' @export
// [[Rcpp::export]]
void beps_main(String inp_dir, DataFrame d_metro, NumericVector LAI, NumericVector opts) {
    // int layer = 5;
    const char *indir = (char*)inp_dir.get_cstring();

    // 8 parameters
    int landcover = opts["LC"];
    int soil_type    = opts["soil_type"];
    
    double lon       = opts["long"];
    double lat       = opts["lat"];
    double clumping  = opts["clumping"];
    double st        = opts["soiltemp"];
    double sw        = opts["soilwater"];
    double snowdepth = opts["snowdepth"];

    // declare variables
    const int nday = 365;
    float m_rad[nday][24], m_tem[nday][24], m_hum[nday][24], m_pre[nday][24], m_wind[nday][24];
    read_metro(d_metro, (float *)m_rad, (float *)m_tem, (float *)m_hum, (float *)m_pre, (float *)m_wind);

    // double es, esd;
    double theta_vfc[layer + 1],
    theta_vwp[layer + 1], thermal_s[layer + 1];
    double psi_sat[layer + 1], bb[layer + 1], fei[layer + 1];
    double Ksat[layer + 1], theta[layer + 1];

    double CosZs;

    double var_o[41], var_n[41];
    double v2last[41];
    double outp[10]; //total[10]

    struct climatedata *meteo;
    struct results *mid_res;
    struct Soil *p_soil;

    meteo = (struct climatedata *)malloc(1 * sizeof(struct climatedata));
    mid_res = (struct results *)malloc(1 * sizeof(struct results));
    p_soil = (struct Soil *)malloc(sizeof(struct Soil));

    NumericVector total(11);
    double parameter[50];
    readparam(landcover, parameter);

    double coef[100];
    readcoef(landcover, soil_type, coef);

    // Open output file
    char site[33], outp_fn[240];
    sprintf(site, "p1");
    sprintf(outp_fn, "inst/examples/output/%s_02.txt", site);

    FILE *outp_ptr;
    if ((outp_ptr = fopen(outp_fn, "w")) == NULL) {
        printf("\nUnable to open file <%s>,  exiting ...\n", outp_fn);
        return;
    }

    /*****  start main simulation *****/
    printf("simulation under progress...\n");
    int i;

    // Day loop begin
    for (int jday = 1; jday <= 365; jday++) {
        /***** re-calculate LAI & renew clump index *****/
        double lai = LAI[jday-1] * parameter[2] / clumping;
        // Hour loop begin
        for (int rstep = 0; rstep < 24; rstep++) {
            int flag = (jday == 1 && rstep == 0) ? 0 : 1;

            meteo->Srad = m_rad[jday - 1][rstep];
            meteo->temp = m_tem[jday - 1][rstep];
            meteo->rain = m_pre[jday - 1][rstep];
            meteo->wind = m_wind[jday - 1][rstep];
            meteo->LR = -200.0;  //  -200.0 means no measured long-wave radiation, the value will be calculated later

            double tem = m_tem[jday - 1][rstep];
            double hum = m_hum[jday - 1][rstep];

            // Vapour pressure in mbar
            double es = 0.46 * hum * (tem + 273.16) / 100;
            double esd = 6.1078 * exp((17.269 * tem) / (237.3 + tem));

            // Calculate relative humidity when reading in VPD
            meteo->rh = (es / esd >= 1) ? 100 : 100 * es / esd;
            // meteo->rh = hum;  // when reading in relative humidity, percentage
            if (flag == 0) {
                // for 1st time step, to initialize var.
                /***** initialize soil conditions, read soil parameters and set depth *****/
                Init_Soil_Parameters(landcover, soil_type, parameter[27], p_soil);
                p_soil->r_drainage = parameter[26];
                Init_Soil_Status(p_soil, st, tem, sw, snowdepth);  // LHE
                // Initialize intermediate variables array
                for (i = 0; i <= 40; i++) var_o[i] = 0;
                for (i = 3; i <= 8; i++) var_o[i] = tem;

                for (i = 9; i <= 14; i++) var_o[i] = p_soil->temp_soil_p[i - 9];
                for (i = 21; i <= 26; i++) var_o[i] = p_soil->thetam_prev[i - 21];
                for (i = 27; i <= 32; i++) var_o[i] = p_soil->ice_ratio[i - 27];

            } else  //  for other time steps, assigned from the temp variables array
                for (i = 0; i <= 40; i++) var_o[i] = v2last[i];

            /***** calculate cos_solar zenith angle Z *****/
            s_coszs(jday, rstep, lat, lon, &CosZs);

            /***** start simulation modules *****/
            //printf("%d, %d, %f\n", jday, rstep, p_soil->thetam_prev[0]); // in-process check
            inter_prg(jday, rstep, lai, clumping, parameter, meteo, CosZs, var_o, var_n, p_soil, mid_res);

            //printf("%d, %d, %f, %f\n", jday, rstep, p_soil->thetam_prev[0], p_soil->f_soilwater); // in-process check
            // Store updated variables array in temp array
            for (i = 0; i <= 40; i++) v2last[i] = var_n[i];

            /***** save data for output *****/
            // Hourly output
            outp[1] = mid_res->GPP;
            outp[2] = mid_res->Trans + mid_res->Evap;
            outp[3] = mid_res->NEP;
            outp[4] = mid_res->npp_o + mid_res->npp_u;
            
            // Write hourly output to files
            fprintf(outp_ptr,"%d %d gpp= %f tr= %f Ev= %f \n",jday,rstep,outp[1],outp[2],outp[3]);
            // Sum of output
            total[1] = total[1] + outp[1];
            total[2] = total[2] + outp[2];
            total[3] = total[3] + outp[3];
        }  // End of hourly loop
    }      // End of daily loop
    fprintf(outp_ptr, "total GPP: %f \t ET: %f \tNEP: %f \n", total[1], total[2], total[3]);
    printf("total GPP: %f \t ET: %f \tNEP: %f \n", total[1], total[2], total[3]);
    fclose(outp_ptr);

    free(meteo);
    free(mid_res);
    free(p_soil);
    // return 1;
}

