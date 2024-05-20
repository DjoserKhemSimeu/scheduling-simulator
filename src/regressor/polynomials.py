from math import *
def lin(x, t0, t1, t2, t3):
    p, q, r = x
    lin_term = (t0 + t1*p + t2*q + t3*r)
    return lin_term

def qdr(x, t0, t1, t2, t3, t4, t5, t6, t7):
    p, q, r = x
    lin_term = lin(x, t0, t1, t2, t3)
    sq_term = (t4*p**2 + t5*q**2 + t6*r**2) + (t7*p*q)

    return lin_term + sq_term

def cub(
    x, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
    t10, t11, t12):
    p, q, r = x
    sq_term = qdr(x, t0, t1, t2, t3, t4, t5, t6, t7)
    cub_term = (t8*p**3 + t9*q**3 + t10*r**3) \
        + (t11*(p**2)*q + t12*p*(q**2))

    return sq_term + cub_term

def qua(
    x, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
    t10, t11, t12, t13, t14, t15, t16, t17, t18):
    p, q, r = x
    cub_term = cub(
        x, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
        t10, t11, t12)
    qua_term = (t13*p**4 + t14*q**4 + t15*r**4) \
        + (t16*(p**3)*q + t17*(p*q)**2 + t18*p*(q**3))
    
    return cub_term + qua_term

def qui(
    x, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
    t10, t11, t12, t13, t14, t15, t16, t17, t18, t19,
    t20, t21, t22, t23, t24, t25):
    p, q, r = x
    qua_term = qua(
        x, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
        t10, t11, t12, t13, t14, t15, t16, t17, t18)
    qui_term = (t19*p**5 + t20*q**5 + t21*r**5) \
        + (t22*(p**4)*q + t23*(p**3)*(q**2) + t24*(p**2)*(q**3) \
            + t25*p*(q**4))

    return qua_term + qui_term

def sex(
    x, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
    t10, t11, t12, t13, t14, t15, t16, t17, t18, t19,
    t20, t21, t22, t23, t24, t25, t26, t27, t28, t29,
    t30, t31, t32, t33):
    p, q, r = x
    qui_term = qui(
        x, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
        t10, t11, t12, t13, t14, t15, t16, t17, t18, t19,
        t20, t21, t22, t23, t24, t25)
    sex_term = (t26*p**6 + t27*q**6 + t28*r**6) \
        + (t29*(p**5)*q + t30*(p**4)*(q**2) + t31*(p*q)**3 \
            + t32*(p**2)*(q**4) + t33*p*(q**5))

    return qui_term + sex_term

#####################################################################################################
#FUNCTION DEFINED BY THE VIF GRID SEARCH
#####################################################################################################
############################## 2 PARAMETERS #########################################

def new_2(x, t0, t1, t2):
    p, q, r = x
    lin_term = (t0 + t1*(p*q) + t2*r)
    return lin_term

def new_2_1(x, t0, t1, t2):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(r**4)*q)
    return lin_term

def new_2_2(x, t0, t1, t2):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(r**3)*q)
    return lin_term

def new_2_3(x, t0, t1, t2):
    p, q, r = x
    lin_term = (t0 + t1*(r**4) + t2*(p**4)*q)
    return lin_term

def new_2_4(x, t0, t1, t2):
    p, q, r = x
    lin_term = (t0 + t1*(p**4)*q + t2*(r**4)*q)
    return lin_term

def new_2_5(x, t0, t1, t2):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(r**4))
    return lin_term
############################## 3 PARAMETERS #########################################
def new_3_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**4) + t2*(q**4)*p +t3*(p**4)*r)
    return lin_term

def new_3_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**4) + t2*(q**3)*p +t3*(p**4)*r)
    return lin_term

def new_3_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(r**4) +t3*(q**4)*p)
    return lin_term

def new_3_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(q**4) +t3*(r**4))
    return lin_term

def new_3_5(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(q**4)*p +t3*(r**4)*p)
    return lin_term

def new_3_6(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(r**4) +t3*(q**3)*p)
    return lin_term
    
def new_3_7(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*p + t2*q +t3*r +t4*(p**7)*q)
    return lin_term

def new_3_8(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**4) + t2*(q**2)*p +t3*(p**4)*r)
    return lin_term

def new_3_9(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(r**3)*p +t3*(q**4)*p)
    return lin_term

def new_3_10(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**4) + t2*(p**3)*r +t3*(q**4)*p)
    return lin_term

############################## INTRODUCTION OF SERIALISM #########################################
def ser_1(x, t0, t1, t2,t3,t4,t5,t6):
    p, q, r, p_mean , q_mean , r_mean= x
    lin_term = (t0 + t1*p + t2*q +t3*r+t4*p_mean+t5*q_mean+t6*r_mean)
    return lin_term

def ser_2(x, t0, t1, t2,t3,t4):
    p, q, r, p_mean , q_mean , r_mean= x
    lin_term = (t0 + t1*p*q +t2*r+t3*p_mean*q_mean+t4*r_mean)
    return lin_term

################################################################################################
#3D MSE VISUALISATION
################################################################################################
############################################ SIZE = 3 #######################################################
######### VIF ~ 1 ###########

def vif_1_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*p + t2*q +t3*r)
    return lin_term

def vif_1_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**3) + t2*(q**2)*p +t3*(p**3)*r)
    return lin_term

def vif_1_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(q**4)*p +t3*(r**4)*p)
    return lin_term

######### VIF ~ 2 ###########

def vif_2_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*p*q +t3*q*r)
    return lin_term

def vif_2_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*(r**2)*q +t3*(q**3)*p)
    return lin_term

def vif_2_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**2)*q + t2*(p**3)*r +t3*(p**4)*q)
    return lin_term

def vif_2_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**4) + t2*(p**4)*r +t3*(r**4)*q)
    return lin_term

######### VIF ~ 3 ###########

def vif_3_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*r + t2*p*q +t3*(p**2)*q)
    return lin_term

def vif_3_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**2)*q + t2*(r**2)*q +t3*(p**3)*q)
    return lin_term

def vif_3_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**2)*q + t2*(q**3)*p +t3*(q**3)*r)
    return lin_term

def vif_3_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(q**2) + t2*(q**4)*p +t3*(r**4)*p)
    return lin_term

######### VIF ~ 4 ###########

def vif_4_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*p*r + t2*q*r +t3*(q**3)*r)
    return lin_term

def vif_4_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(q**2) + t2*(q**2)*r +t3*(q**3)*p)
    return lin_term

def vif_4_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(q**4) + t2*(r**2)*q +t3*(q**3)*r)
    return lin_term

def vif_4_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**2)*q + t2*(p**4)*r +t3*(r**4)*q)
    return lin_term

######### VIF ~ 5 ###########

def vif_5_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(q**(1/2)) + t2*q +t3*(r**2)*q)
    return lin_term

def vif_5_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**2)*r + t2*(r**2)*q +t3*(r**4)*q)
    return lin_term

def vif_5_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**2)*q + t2*(r**3)*p +t3*(r**4)*q)
    return lin_term

def vif_5_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**2)*q + t2*(r**4)*p +t3*(r**4)*q)
    return lin_term

######### VIF ~ 6 ###########

def vif_6_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*p*q +t3*(q**3)*p)
    return lin_term

def vif_6_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*q*r + t2*(q**2)*r +t3*(p**3)*r)
    return lin_term

def vif_6_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**2)*r + t2*(p**3)*r +t3*(q**4)*p)
    return lin_term

######### VIF ~ 7 ###########

def vif_7_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*q*r +t3*(q**2)*r)
    return lin_term

def vif_7_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(q**3) + t2*p*q +t3*(p**2)*q)
    return lin_term

def vif_7_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*q*r + t2*(q**2)*r +t3*(q**4)*p)
    return lin_term

def vif_7_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**2)*r + t2*(p**3)*r +t3*(p**4)*q)
    return lin_term

######### VIF ~ 8 ###########

def vif_8_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*p +t3*p*r)
    return lin_term

def vif_8_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**2) + t2*q*r +t3*(q**2)*r)
    return lin_term

def vif_8_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**2)*p + t2*(r**3)*p +t3*(q**3)*r)
    return lin_term

def vif_8_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**3)*q + t2*(p**4)*q +t3*(p**4)*r)
    return lin_term

######### VIF ~ 9 ###########

def vif_9_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(r**(1/2)) + t2*p*q +t3*(q**2)*p)
    return lin_term

def vif_9_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*p*q + t2*p*r +t3*(q**2)*p)
    return lin_term

def vif_9_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*p*q + t2*(q**2)*p +t3*(q**3)*r)
    return lin_term

def vif_9_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**3)*q + t2*(p**4)*q +t3*(r**4)*q)
    return lin_term

######### VIF ~ 10 ###########

def vif_10_deg_1(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*(r**(1/2)) +t3*p)
    return lin_term

def vif_10_deg_2(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*p*q +t3*(p**2)*q)
    return lin_term

def vif_10_deg_3(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*q*r + t2*(p**3)*q +t3*(p**4)*q)
    return lin_term

def vif_10_deg_4(x, t0, t1, t2,t3):
    p, q, r = x
    lin_term = (t0 + t1*(q**2)*r + t2*(p**3)*q +t3*(p**4)*q)
    return lin_term



############################################ SIZE = 4 #######################################################
######### VIF ~ 1 ###########

def s_4_vif_1_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**(1/2)) + t2*(p**2) +t3*(q**4)+t4*(r**4))
    return lin_term

def s_4_vif_1_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*(p**3) +t3*(q**4)+t4*(r**4)*p)
    return lin_term

def s_4_vif_1_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**(1/2)) + t2*(q**4)*p +t3*(p**4)*q+t4*(r**4)*p)
    return lin_term

######### VIF ~ 2 ###########

def s_4_vif_2_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**(1/2)) + t2*(p**2) +t3*p*q+t4*q*r)
    return lin_term

def s_4_vif_2_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*r +t3*(r**2)*q+t4*(q**4)*p)
    return lin_term

def s_4_vif_2_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*(q**2)*r +t3*(p**3)*q+t4*(q**4)*p)
    return lin_term

def s_4_vif_2_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**4) + t2*(r**3)*q +t3*(p**4)*q+t4*(q**4)*r)
    return lin_term

######### VIF ~ 3 ###########

def s_4_vif_3_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*(r**(1/2)) +t3*q*r+t4*(r**2)*q)
    return lin_term

def s_4_vif_3_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*(q**3) +t3*(r**3)+t4*p*q)
    return lin_term

def s_4_vif_3_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**2) + t2*p*q +t3*(p**2)*r+t4*(r**4)*p)
    return lin_term

def s_4_vif_3_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**2) + t2*(p**2)*r +t3*(r**3)*p+t4*(q**4)*p)
    return lin_term

######### VIF ~ 4 ###########

def s_4_vif_4_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*p*q +t3*(p**3)*q+t4*(q**4)*r)
    return lin_term

def s_4_vif_4_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*q*r +t3*(q**4)*p+t4*(q**4)*r)
    return lin_term

def s_4_vif_4_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**2) + t2*(r**4) +t3*(r**4)*p+t4*(q**4)*r)
    return lin_term

######### VIF ~ 5 ###########

def s_4_vif_5_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*q +t3*(q**3)+t4*q*r)
    return lin_term

def s_4_vif_5_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**(1/2)) + t2*q +t3*(p**3)+t4*(p**3)*r)
    return lin_term

def s_4_vif_5_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*p*q + t2*p*r +t3*(q**3)*p+t4*(r**3)*p)
    return lin_term

def s_4_vif_5_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**3) + t2*(p**2)*q +t3*(r**3)*p+t4*(q**4)*p)
    return lin_term

######### VIF ~ 6 ###########

def s_4_vif_6_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**(1/2)) + t2*p +t3*r+t4*(q**3))
    return lin_term

def s_4_vif_6_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**(1/2)) + t2*p +t3*(r**2)*q+t4*(r**4)*q)
    return lin_term

def s_4_vif_6_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**(1/2)) + t2*(r**2)*q +t3*(p**4)*r+t4*(r**4)*q)
    return lin_term



######### VIF ~ 7 ###########

def s_4_vif_7_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*(r**(1/2)) +t3*r+t4*(q**4))
    return lin_term

def s_4_vif_7_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*q*r +t3*(p**2)*q+t4*(q**2)*r)
    return lin_term

def s_4_vif_7_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*(q**2) +t3*(q**3)*r+t4*(p**4)*r)
    return lin_term

def s_4_vif_7_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**2) + t2*(p**2)*r +t3*(p**3)*r+t4*(r**4)*q)
    return lin_term

######### VIF ~ 8 ###########

def s_4_vif_8_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*p +t3*q*r+t4*(q**2)*r)
    return lin_term

def s_4_vif_8_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**(1/2)) + t2*(p**2) +t3*(p**3)+t4*(r**2)*p)
    return lin_term

def s_4_vif_8_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**2) + t2*(p**3) +t3*(p**2)*r+t4*(p**3)*q)
    return lin_term

def s_4_vif_8_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**2)*q + t2*(r**3)*p +t3*(q**4)*r+t4*(r**4)*q)
    return lin_term

######### VIF ~ 9 ###########

def s_4_vif_9_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**(1/2)) + t2*q +t3*(p**4)+t4*p*q)
    return lin_term

def s_4_vif_9_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*(q**3) +t3*p*q+t4*(q**3)*p)
    return lin_term

def s_4_vif_9_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**3) + t2*(r**3) +t3*(p**2)*q+t4*(q**2)*p)
    return lin_term

def s_4_vif_9_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(r**2)*p + t2*(r**2)*q +t3*(q**4)*r+t4*(r**4)*q)
    return lin_term

######### VIF ~ 10 ###########

def s_4_vif_10_deg_1(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**(1/2)) + t2*(r**(1/2)) +t3*p+t4*(p**3)*q)
    return lin_term

def s_4_vif_10_deg_2(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*q + t2*(q**2) +t3*p*r+t4*(q**2)*p)
    return lin_term

def s_4_vif_10_deg_3(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(q**3) + t2*(p**4) +t3*(p**2)*q+t4*(q**2)*p)
    return lin_term

def s_4_vif_10_deg_4(x, t0, t1, t2,t3,t4):
    p, q, r = x
    lin_term = (t0 + t1*(p**3)*q + t2*(p**4)*q +t3*(q**4)*r+t4*(r**4)*q)
    return lin_term