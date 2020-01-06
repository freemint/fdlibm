	/*
	 * Coefficients for approximation to  erf on [0,0.84375]
	 */
	static const double pp0 =  1.28379167095512558561e-01;	/* 0x3FC06EBA, 0x8214DB68 */
	static const double pp1 = -3.25042107247001499370e-01;	/* 0xBFD4CD7D, 0x691CB913 */
	static const double pp2 = -2.84817495755985104766e-02;	/* 0xBF9D2A51, 0xDBD7194F */
	static const double pp3 = -5.77027029648944159157e-03;	/* 0xBF77A291, 0x236668E4 */
	static const double pp4 = -2.37630166566501626084e-05;	/* 0xBEF8EAD6, 0x120016AC */
	static const double qq1 =  3.97917223959155352819e-01;	/* 0x3FD97779, 0xCDDADC09 */
	static const double qq2 =  6.50222499887672944485e-02;	/* 0x3FB0A54C, 0x5536CEBA */
	static const double qq3 =  5.08130628187576562776e-03;	/* 0x3F74D022, 0xC4D36B0F */
	static const double qq4 =  1.32494738004321644526e-04;	/* 0x3F215DC9, 0x221C1A10 */
	static const double qq5 = -3.96022827877536812320e-06;	/* 0xBED09C43, 0x42A26120 */
	/*
	 * Coefficients for approximation to  erf  in [0.84375,1.25]
	 */
	static const double pa0 = -2.36211856075265944077e-03;	/* 0xBF6359B8, 0xBEF77538 */
	static const double pa1 =  4.14856118683748331666e-01;	/* 0x3FDA8D00, 0xAD92B34D */
	static const double pa2 = -3.72207876035701323847e-01;	/* 0xBFD7D240, 0xFBB8C3F1 */
	static const double pa3 =  3.18346619901161753674e-01;	/* 0x3FD45FCA, 0x805120E4 */
	static const double pa4 = -1.10894694282396677476e-01;	/* 0xBFBC6398, 0x3D3E28EC */
	static const double pa5 =  3.54783043256182359371e-02;	/* 0x3FA22A36, 0x599795EB */
	static const double pa6 = -2.16637559486879084300e-03;	/* 0xBF61BF38, 0x0A96073F */
	static const double qa1 =  1.06420880400844228286e-01;	/* 0x3FBB3E66, 0x18EEE323 */
	static const double qa2 =  5.40397917702171048937e-01;	/* 0x3FE14AF0, 0x92EB6F33 */
	static const double qa3 =  7.18286544141962662868e-02;	/* 0x3FB2635C, 0xD99FE9A7 */
	static const double qa4 =  1.26171219808761642112e-01;	/* 0x3FC02660, 0xE763351F */
	static const double qa5 =  1.36370839120290507362e-02;	/* 0x3F8BEDC2, 0x6B51DD1C */
	static const double qa6 =  1.19844998467991074170e-02;	/* 0x3F888B54, 0x5735151D */
	/*
	 * Coefficients for approximation to  erfc in [1.25,1/0.35]
	 */
	static const double ra0 = -9.86494403484714822705e-03;	/* 0xBF843412, 0x600D6435 */
	static const double ra1 = -6.93858572707181764372e-01;	/* 0xBFE63416, 0xE4BA7360 */
	static const double ra2 = -1.05586262253232909814e+01;	/* 0xC0251E04, 0x41B0E726 */
	static const double ra3 = -6.23753324503260060396e+01;	/* 0xC04F300A, 0xE4CBA38D */
	static const double ra4 = -1.62396669462573470355e+02;	/* 0xC0644CB1, 0x84282266 */
	static const double ra5 = -1.84605092906711035994e+02;	/* 0xC067135C, 0xEBCCABB2 */
	static const double ra6 = -8.12874355063065934246e+01;	/* 0xC0545265, 0x57E4D2F2 */
	static const double ra7 = -9.81432934416914548592e+00;	/* 0xC023A0EF, 0xC69AC25C */
	static const double sa1 =  1.96512716674392571292e+01;	/* 0x4033A6B9, 0xBD707687 */
	static const double sa2 =  1.37657754143519042600e+02;	/* 0x4061350C, 0x526AE721 */
	static const double sa3 =  4.34565877475229228821e+02;	/* 0x407B290D, 0xD58A1A71 */
	static const double sa4 =  6.45387271733267880336e+02;	/* 0x40842B19, 0x21EC2868 */
	static const double sa5 =  4.29008140027567833386e+02;	/* 0x407AD021, 0x57700314 */
	static const double sa6 =  1.08635005541779435134e+02;	/* 0x405B28A3, 0xEE48AE2C */
	static const double sa7 =  6.57024977031928170135e+00;	/* 0x401A47EF, 0x8E484A93 */
	static const double sa8 = -6.04244152148580987438e-02;	/* 0xBFAEEFF2, 0xEE749A62 */
	/*
	 * Coefficients for approximation to  erfc in [1/.35,28]
	 */
	static const double rb0 = -9.86494292470009928597e-03;	/* 0xBF843412, 0x39E86F4A */
	static const double rb1 = -7.99283237680523006574e-01;	/* 0xBFE993BA, 0x70C285DE */
	static const double rb2 = -1.77579549177547519889e+01;	/* 0xC031C209, 0x555F995A */
	static const double rb3 = -1.60636384855821916062e+02;	/* 0xC064145D, 0x43C5ED98 */
	static const double rb4 = -6.37566443368389627722e+02;	/* 0xC083EC88, 0x1375F228 */
	static const double rb5 = -1.02509513161107724954e+03;	/* 0xC0900461, 0x6A2E5992 */
	static const double rb6 = -4.83519191608651397019e+02;	/* 0xC07E384E, 0x9BDC383F */
	static const double sb1 =  3.03380607434824582924e+01;	/* 0x403E568B, 0x261D5190 */
	static const double sb2 =  3.25792512996573918826e+02;	/* 0x40745CAE, 0x221B9F0A */
	static const double sb3 =  1.53672958608443695994e+03;	/* 0x409802EB, 0x189D5118 */
	static const double sb4 =  3.19985821950859553908e+03;	/* 0x40A8FFB7, 0x688C246A */
	static const double sb5 =  2.55305040643316442583e+03;	/* 0x40A3F219, 0xCEDF3BE6 */
	static const double sb6 =  4.74528541206955367215e+02;	/* 0x407DA874, 0xE79FE763 */
	static const double sb7 = -2.24409524465858183362e+01;	/* 0xC03670E2, 0x42712D62 */
