#include <math.h>
#include "polynomials.h"
#include "parameters.h"

#define PARAMETERS 0
#define SECONDS_IN_ONE_HOUR 3600

double linear(double p, double q, double r)
{
    double *theta;
    
    if(PARAMETERS == 1)
    {
        p = p / SECONDS_IN_ONE_HOUR;
        r = r / SECONDS_IN_ONE_HOUR;
        theta = temporal_normalized_lin_parameters; 
    }
    else if(PARAMETERS == 2)
    {
        theta = lublin_256_lin_parameters;
    }
    else if(PARAMETERS == 3)
    {
        theta = ctc_sp2_lin_parameters;
    }
    else if(PARAMETERS == 4)
    {
        theta = sdsc_blue_lin_parameters;
    }
    else
    {
        theta = default_lin_parameters;
    }
    
    return theta[0] \
            + theta[1]*p + theta[2]*q + theta[3]*r;
}

double quadratic(double p, double q, double r)
{
    double *theta;

    if(PARAMETERS == 1)
    {
        p = p / SECONDS_IN_ONE_HOUR;
        r = r / SECONDS_IN_ONE_HOUR;
        theta = temporal_normalized_qdr_parameters; 
    }
    else if(PARAMETERS == 2)
    {
        theta = lublin_256_qdr_parameters;
    }
    else if(PARAMETERS == 3)
    {
        theta = ctc_sp2_qdr_parameters;
    }
    else if(PARAMETERS == 4)
    {
        theta = sdsc_blue_qdr_parameters;
    }
    else
    {
        theta = default_qdr_parameters;
    }
    
    return theta[0] \
            + theta[1]*p + theta[2]*q + theta[3]*r \
            + theta[4]*pow(p,2) + theta[5]*pow(q,2) + theta[6]*pow(r,2) \
            + theta[7]*p*q;
}

double cubic(double p, double q, double r)
{
    double *theta;

    if(PARAMETERS == 1)
    {
        p = p / SECONDS_IN_ONE_HOUR;
        r = r / SECONDS_IN_ONE_HOUR;
        theta = temporal_normalized_cub_parameters; 
    }
    else if(PARAMETERS == 2)
    {
        theta = lublin_256_cub_parameters;
    }
    else if(PARAMETERS == 3)
    {
        theta = ctc_sp2_cub_parameters;
    }
    else if(PARAMETERS == 4)
    {
        theta = sdsc_blue_cub_parameters;
    }
    else
    {
        theta = default_cub_parameters;
    }

    return theta[0] \
            + theta[1]*p + theta[2]*q + theta[3]*r \
            + theta[4]*pow(p,2) + theta[5]*pow(q,2) + theta[6]*pow(r,2) \
            + theta[7]*p*q \
            + theta[8]*pow(p,3) + theta[9]*pow(q,3) + theta[10]*pow(r,3) \
            + theta[11]*pow(p,2)*q + theta[12]*p*pow(q,2);
}

double quartic(double p, double q, double r)
{
    double *theta;

    if(PARAMETERS == 1)
    {
        p = p / SECONDS_IN_ONE_HOUR;
        r = r / SECONDS_IN_ONE_HOUR;
        theta = temporal_normalized_qua_parameters; 
    }
    else if(PARAMETERS == 2)
    {
        theta = lublin_256_qua_parameters;
    }
    else if(PARAMETERS == 3)
    {
        theta = ctc_sp2_qua_parameters;
    }
    else if(PARAMETERS == 4)
    {
        theta = sdsc_blue_qua_parameters;
    }
    else
    {
        theta = default_qua_parameters;
    }

    return theta[0] \
            + theta[1]*p + theta[2]*q + theta[3]*r \
            + theta[4]*pow(p,2) + theta[5]*pow(q,2) + theta[6]*pow(r,2) \
            + theta[7]*p*q \
            + theta[8]*pow(p,3) + theta[9]*pow(q,3) + theta[10]*pow(r,3) \
            + theta[11]*pow(p,2)*q + theta[12]*p*pow(q,2) \
            + theta[13]*pow(p,4) + theta[14]*pow(q,4) + theta[15]*pow(r,4) \
            + theta[16]*pow(p,3)*q + theta[17]*pow(p,2)*pow(q,2) + theta[18]*p*pow(q,3);
}

double quintic(double p, double q, double r)
{
    double *theta;

    if(PARAMETERS == 1)
    {
        p = p / SECONDS_IN_ONE_HOUR;
        r = r / SECONDS_IN_ONE_HOUR;
        theta = temporal_normalized_qui_parameters; 
    }
    else if(PARAMETERS == 2)
    {
        theta = lublin_256_qui_parameters;
    }
    else if(PARAMETERS == 3)
    {
        theta = ctc_sp2_qui_parameters;
    }
    else if(PARAMETERS == 4)
    {
        theta = sdsc_blue_qui_parameters;
    }
    else
    {
        theta = default_qui_parameters;
    }

    return theta[0] \
            + theta[1]*p + theta[2]*q + theta[3]*r \
            + theta[4]*pow(p,2) + theta[5]*pow(q,2) + theta[6]*pow(r,2) \
            + theta[7]*p*q \
            + theta[8]*pow(p,3) + theta[9]*pow(q,3) + theta[10]*pow(r,3) \
            + theta[11]*pow(p,2)*q + theta[12]*p*pow(q,2) \
            + theta[13]*pow(p,4) + theta[14]*pow(q,4) + theta[15]*pow(r,4) \
            + theta[16]*pow(p,3)*q + theta[17]*pow(p,2)*pow(q,2) + theta[18]*p*pow(q,3) \
            + theta[19]*pow(p,5) + theta[20]*pow(q,5) + theta[21]*pow(r,5) \
            + theta[22]*pow(p,4)*q + theta[23]*pow(p,3)*pow(q,2) \
            + theta[24]*pow(p,2)*pow(q,3) + theta[25]*p*pow(q,4);
}

double sextic(double p, double q, double r)
{
    double *theta;
    
    if(PARAMETERS == 1)
    {
        p = p / SECONDS_IN_ONE_HOUR;
        r = r / SECONDS_IN_ONE_HOUR;
        theta = temporal_normalized_sex_parameters; 
    }
    else if(PARAMETERS == 2)
    {
        theta = lublin_256_sex_parameters;
    }
    else if(PARAMETERS == 3)
    {
        theta = ctc_sp2_sex_parameters;
    }
    else if(PARAMETERS == 4)
    {
        theta = sdsc_blue_sex_parameters;
    }
    else
    {
        theta = default_sex_parameters;
    }

    return theta[0] \
            + theta[1]*p + theta[2]*q + theta[3]*r \
            + theta[4]*pow(p,2) + theta[5]*pow(q,2) + theta[6]*pow(r,2) \
            + theta[7]*p*q \
            + theta[8]*pow(p,3) + theta[9]*pow(q,3) + theta[10]*pow(r,3) \
            + theta[11]*pow(p,2)*q + theta[12]*p*pow(q,2) \
            + theta[13]*pow(p,4) + theta[14]*pow(q,4) + theta[15]*pow(r,4) \
            + theta[16]*pow(p,3)*q + theta[17]*pow(p,2)*pow(q,2) + theta[18]*p*pow(q,3) \
            + theta[19]*pow(p,5) + theta[20]*pow(q,5) + theta[21]*pow(r,5) \
            + theta[22]*pow(p,4)*q + theta[23]*pow(p,3)*pow(q,2) \
            + theta[24]*pow(p,2)*pow(q,3) + theta[25]*p*pow(q,4) \
            + theta[26]*pow(p,6) + theta[27]*pow(q,6) + theta[28]*pow(r,6) \
            + theta[29]*pow(p,5)*q + theta[30]*pow(p,4)*pow(q,2) \
            + theta[31]*pow(p,3)*pow(q,3) \
            + theta[32]*pow(p,2)*pow(q,4) + theta[33]*p*pow(q,5);
}
double new_3_1 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_1_parameters;

    return theta[0] \
            + theta[1]*pow(r,4) + theta[2]*pow(q,4)*p + theta[3]*pow(p,4)*r;
}
double new_3_2 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_2_parameters;

    return theta[0] \
            + theta[1]*pow(r,4) + theta[2]*pow(q,3)*p + theta[3]*pow(p,4)*r;
}
double new_3_3 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_3_parameters;

    return theta[0] \
            + theta[1]*pow(p,4) + theta[2]*pow(r,4) + theta[3]*pow(q,4)*p;
}
double new_3_4 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_4_parameters;

    return theta[0] \
            + theta[1]*pow(p,4) + theta[2]*pow(q,4) + theta[3]*pow(r,4);
}
double new_3_5 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_5_parameters;

    return theta[0] \
            + theta[1]*pow(p,4) + theta[2]*pow(q,4)*p + theta[3]*pow(r,4)*p;
}
double new_3_6 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_6_parameters;

    return theta[0] \
            + theta[1]*pow(p,4) + theta[2]*pow(r,4) + theta[3]*pow(q,3)*p;
}
double new_3_7 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_7_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2)) + theta[2]*pow(r,(1/2)) + theta[3]*p;
}
double new_3_8 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_8_parameters;

    return theta[0] \
            + theta[1]*pow(r,4) + theta[2]*pow(q,2)*p + theta[3]*pow(p,4)*r;
}
double new_3_9 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_9_parameters;

    return theta[0] \
            + theta[1]*pow(p,4) + theta[2]*pow(r,3)*p + theta[3]*pow(q,4)*p;
}
double new_3_10 (double p, double q, double r)
{
    double *theta;
    theta = default_new_3_10_parameters;

    return theta[0] \
            + theta[1]*pow(r,4) + theta[2]*pow(p,3)*r + theta[3]*pow(q,4)*p;
}


double ser_1(double p, double q, double r, double p_mean, double q_mean, double r_mean){
    double *theta;
    theta = default_ser_1_parameters;

    return theta[0] \
            + theta[1]*p + theta[2]*q + theta[3]*r+theta[4]*p_mean+theta[5]*q_mean+theta[6]*r_mean;
    
}
// CORRELATION ANALYSIS
double s3_vif_1_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_1_3_parameters;

    return theta[0] \
            + theta[1]*pow(r,3) + theta[2]*pow(q,2)*p + theta[3]*pow(p,3)*r;
}
double s3_vif_1_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_1_4_parameters;

    return theta[0] \
            + theta[1]*pow(p,4) + theta[2]*pow(q,4)*p + theta[3]*pow(r,4)*p;
}
double s3_vif_2_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_2_1_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2)) + theta[2]*p*q + theta[3]*q*r;
}
double s3_vif_2_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_2_2_parameters;

    return theta[0] \
            + theta[1]*q + theta[2]*pow(r,2)*q + theta[3]*pow(q,3)*p;
}
double s3_vif_2_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_2_3_parameters;

    return theta[0] \
            + theta[1]*pow(p,2)*q + theta[2]*pow(p,3)*r + theta[3]*pow(p,4)*q;
}
double s3_vif_2_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_2_4_parameters;

    return theta[0] \
            + theta[1]*pow(p,4)+ theta[2]*pow(p,4)*r + theta[3]*pow(r,4)*q;
}
double s3_vif_3_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_3_1_parameters;

    return theta[0] \
            + theta[1]*r+ theta[2]*p*q + theta[3]*pow(p,2)*q;
}
double s3_vif_3_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_3_2_parameters;

    return theta[0] \
            + theta[1]*pow(p,2)*q+ theta[2]*pow(r,2)*q + theta[3]*pow(p,3)*q;
}
double s3_vif_3_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_3_3_parameters;

    return theta[0] \
            + theta[1]*pow(r,2)*q+ theta[2]*pow(q,3)*p + theta[3]*pow(q,3)*r;
}
double s3_vif_3_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_3_4_parameters;

    return theta[0] \
            + theta[1]*pow(q,2)+ theta[2]*pow(q,4)*p + theta[3]*pow(r,4)*p;
}
double s3_vif_4_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_4_1_parameters;

    return theta[0] \
            + theta[1]*p*r+ theta[2]*q*r + theta[3]*pow(q,3)*r;
}
double s3_vif_4_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_4_2_parameters;

    return theta[0] \
            + theta[1]*pow(q,2)+ theta[2]*pow(q,2)*r + theta[3]*pow(q,3)*r;
}
double s3_vif_4_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_4_3_parameters;

    return theta[0] \
            + theta[1]*pow(q,4)+ theta[2]*pow(r,2)*q + theta[3]*pow(q,3)*r;
}
double s3_vif_4_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_4_4_parameters;

    return theta[0] \
            + theta[1]*pow(r,2)*q+ theta[2]*pow(p,4)*r + theta[3]*pow(r,4)*q;
}
double s3_vif_5_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_5_1_parameters;

    return theta[0] \
            + theta[1]*pow(q,(1/2))+ theta[2]*q + theta[3]*pow(r,2)*p;
}
double s3_vif_5_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_5_2_parameters;

    return theta[0] \
            + theta[1]*pow(p,2)*r+ theta[2]*pow(r,2)*q + theta[3]*pow(r,4)*q;
}

double s3_vif_5_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_5_3_parameters;

    return theta[0] \
            + theta[1]*pow(r,2)*q+ theta[2]*pow(r,3)*p + theta[3]*pow(r,4)*q;
}
double s3_vif_5_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_5_4_parameters;

    return theta[0] \
            + theta[1]*pow(r,2)*q+ theta[2]*pow(r,4)*p + theta[3]*pow(r,4)*q;
}
double s3_vif_6_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_6_1_parameters;

    return theta[0] \
            + theta[1]*q+ theta[2]*p*q + theta[3]*pow(q,3)*p;
}
double s3_vif_6_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_6_2_parameters;

    return theta[0] \
            + theta[1]*q*r+ theta[2]*pow(q,2)*r + theta[3]*pow(p,3)*r;
}
double s3_vif_6_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_6_3_parameters;

    return theta[0] \
            + theta[1]*pow(p,2)*r+ theta[2]*pow(p,3)*r + theta[3]*pow(q,4)*p;
}
double s3_vif_7_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_7_1_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2))+ theta[2]*q*r + theta[3]*pow(q,2)*r;
}
double s3_vif_7_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_7_2_parameters;

    return theta[0] \
            + theta[1]*pow(q,3)+ theta[2]*p*q + theta[3]*pow(p,2)*q;
}
double s3_vif_7_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_7_3_parameters;

    return theta[0] \
            + theta[1]*q*r+ theta[2]*pow(q,2)*r + theta[3]*pow(q,4)*p;
}
double s3_vif_7_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_7_3_parameters;

    return theta[0] \
            + theta[1]*pow(p,2)*r+ theta[2]*pow(p,3)*r + theta[3]*pow(p,4)*q;
}
double s3_vif_8_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_8_1_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2))*r+ theta[2]*p + theta[3]*p*r;
}
double s3_vif_8_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_8_2_parameters;

    return theta[0] \
            + theta[1]*pow(r,2)+ theta[2]*q*r + theta[3]*pow(q,2)*r;
}
double s3_vif_8_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_8_3_parameters;

    return theta[0] \
            + theta[1]*pow(r,2)*p+ theta[2]*pow(r,3)*p+ + theta[3]*pow(q,3)*r;
}
double s3_vif_8_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_8_4_parameters;

    return theta[0] \
            + theta[1]*pow(p,3)*q+ theta[2]*pow(p,4)*q+ + theta[3]*pow(p,4)*r;
}
double s3_vif_9_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_9_1_parameters;

    return theta[0] \
            + theta[1]*pow(r,(1/2))*r+ theta[2]*p*q + theta[3]*pow(q,2)*p;
}
double s3_vif_9_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_9_2_parameters;

    return theta[0] \
            + theta[1]*p*q+ theta[2]*p*r + theta[3]*pow(q,2)*p;
}
double s3_vif_9_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_9_3_parameters;

    return theta[0] \
            + theta[1]*p*q+ theta[2]*pow(q,2)*p+ + theta[3]*pow(q,3)*r;
}
double s3_vif_9_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_9_4_parameters;

    return theta[0] \
            + theta[1]*pow(p,3)*q+ theta[2]*pow(p,4)*q+ + theta[3]*pow(r,4)*q;
}
double s3_vif_10_1(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_10_1_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2)) + theta[2]*pow(r,(1/2)) + theta[3]*p;
}
double s3_vif_10_2(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_10_2_parameters;

    return theta[0] \
            + theta[1]*q + theta[2]*p*q + theta[3]*pow(p,2)*q;
}
double s3_vif_10_3(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_10_3_parameters;

    return theta[0] \
            + theta[1]*q*r + theta[2]*pow(p,3)*q + theta[3]*pow(p,4)*q;
}
double s3_vif_10_4(double p, double q, double r){
    double *theta;
    theta =default_s_3_vif_10_4_parameters;

    return theta[0] \
            + theta[1]*pow(q,2)*r + theta[2]*pow(p,3)*q + theta[3]*pow(p,4)*q;
}



double s4_vif_7_1(double p, double q, double r){
    double *theta;
    theta =default_s_4_vif_7_1_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2)) + theta[2]*pow(r,(1/2)) + theta[3]*r+theta[4]*pow(q,4);
}
double s4_vif_4_3(double p, double q, double r){
    double *theta;
    theta =default_s_4_vif_4_3_parameters;

    return theta[0] \
            + theta[1]*q + theta[2]*q*r + theta[3]*pow(q,4)*p+theta[4]*pow(q,4)*r;
}
double s4_vif_9_2(double p, double q, double r){
    double *theta;
    theta =default_s_4_vif_9_2_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2)) + theta[2]*pow(q,3) + theta[3]*p*q+theta[4]*pow(q,3)*p;
}
double s4_vif_3_1(double p, double q, double r){
    double *theta;
    theta =default_s_4_vif_3_1_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2)) + theta[2]*pow(r,(1/2)) + theta[3]*q*r+theta[4]*pow(r,2)*q;
}
double s4_vif_9_4(double p, double q, double r){
    double *theta;
    theta =default_s_4_vif_9_4_parameters;

    return theta[0] \
            + theta[1]*pow(p,(1/2)) + theta[2]*pow(r,(1/2)) + theta[3]*q*r+theta[4]*pow(r,2)*q;
}