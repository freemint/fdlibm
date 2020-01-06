/*
 * Coefficients for approximation to  erf on [0,0.84375]
 */
static const float pp0 = 1.2837916613e-01;			/* 0x3e0375d4 */
static const float pp1 = -3.2504209876e-01;			/* 0xbea66beb */
static const float pp2 = -2.8481749818e-02;			/* 0xbce9528f */
static const float pp3 = -5.7702702470e-03;			/* 0xbbbd1489 */
static const float pp4 = -2.3763017452e-05;			/* 0xb7c756b1 */
static const float qq1 = 3.9791721106e-01;			/* 0x3ecbbbce */
static const float qq2 = 6.5022252500e-02;			/* 0x3d852a63 */
static const float qq3 = 5.0813062117e-03;			/* 0x3ba68116 */
static const float qq4 = 1.3249473704e-04;			/* 0x390aee49 */
static const float qq5 = -3.9602282413e-06;			/* 0xb684e21a */
/*
 * Coefficients for approximation to  erf  in [0.84375,1.25]
 */
static const float pa0 = -2.3621185683e-03;			/* 0xbb1acdc6 */
static const float pa1 = 4.1485610604e-01;			/* 0x3ed46805 */
static const float pa2 = -3.7220788002e-01;			/* 0xbebe9208 */
static const float pa3 = 3.1834661961e-01;			/* 0x3ea2fe54 */
static const float pa4 = -1.1089469492e-01;			/* 0xbde31cc2 */
static const float pa5 = 3.5478305072e-02;			/* 0x3d1151b3 */
static const float pa6 = -2.1663755178e-03;			/* 0xbb0df9c0 */
static const float qa1 = 1.0642088205e-01;			/* 0x3dd9f331 */
static const float qa2 = 5.4039794207e-01;			/* 0x3f0a5785 */
static const float qa3 = 7.1828655899e-02;			/* 0x3d931ae7 */
static const float qa4 = 1.2617121637e-01;			/* 0x3e013307 */
static const float qa5 = 1.3637083583e-02;			/* 0x3c5f6e13 */
static const float qa6 = 1.1984500103e-02;			/* 0x3c445aa3 */
/*
 * Coefficients for approximation to  erfc in [1.25,1/0.35]
 */
static const float ra0 = -9.8649440333e-03;			/* 0xbc21a093 */
static const float ra1 = -6.9385856390e-01;			/* 0xbf31a0b7 */
static const float ra2 = -1.0558626175e+01;			/* 0xc128f022 */
static const float ra3 = -6.2375331879e+01;			/* 0xc2798057 */
static const float ra4 = -1.6239666748e+02;			/* 0xc322658c */
static const float ra5 = -1.8460508728e+02;			/* 0xc3389ae7 */
static const float ra6 = -8.1287437439e+01;			/* 0xc2a2932b */
static const float ra7 = -9.8143291473e+00;			/* 0xc11d077e */
static const float sa1 = 1.9651271820e+01;			/* 0x419d35ce */
static const float sa2 = 1.3765776062e+02;			/* 0x4309a863 */
static const float sa3 = 4.3456588745e+02;			/* 0x43d9486f */
static const float sa4 = 6.4538726807e+02;			/* 0x442158c9 */
static const float sa5 = 4.2900814819e+02;			/* 0x43d6810b */
static const float sa6 = 1.0863500214e+02;			/* 0x42d9451f */
static const float sa7 = 6.5702495575e+00;			/* 0x40d23f7c */
static const float sa8 = -6.0424413532e-02;			/* 0xbd777f97 */
/*
 * Coefficients for approximation to  erfc in [1/.35,28]
 */
static const float rb0 = -9.8649431020e-03;			/* 0xbc21a092 */
static const float rb1 = -7.9928326607e-01;			/* 0xbf4c9dd4 */
static const float rb2 = -1.7757955551e+01;			/* 0xc18e104b */
static const float rb3 = -1.6063638306e+02;			/* 0xc320a2ea */
static const float rb4 = -6.3756646729e+02;			/* 0xc41f6441 */
static const float rb5 = -1.0250950928e+03;			/* 0xc480230b */
static const float rb6 = -4.8351919556e+02;			/* 0xc3f1c275 */
static const float sb1 = 3.0338060379e+01;			/* 0x41f2b459 */
static const float sb2 = 3.2579251099e+02;			/* 0x43a2e571 */
static const float sb3 = 1.5367296143e+03;			/* 0x44c01759 */
static const float sb4 = 3.1998581543e+03;			/* 0x4547fdbb */
static const float sb5 = 2.5530502930e+03;			/* 0x451f90ce */
static const float sb6 = 4.7452853394e+02;			/* 0x43ed43a7 */
static const float sb7 = -2.2440952301e+01;			/* 0xc1b38712 */

