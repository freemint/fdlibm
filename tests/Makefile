srcdir = .

CC = m68k-atari-mint-gcc
CPPFLAGS = -I.. -DTEST_DOUBLE
CFLAGS = -pipe -O2 -fomit-frame-pointer -fno-builtin
LDFLAGS = -s
WARN = -Wall -Wstrict-prototypes -Wmissing-prototypes -Wundef -Wdeclaration-after-statement -Werror

AM_DEFAULT_V = 0

am__v_GEN_0    = @echo "  GEN     " $@;
am__v_GEN_1    = @echo generating $@;
AM_V_GEN       = $(am__v_GEN_$(V))
am__v_GEN_     = $(am__v_GEN_$(AM_DEFAULT_V))

am__v_LD_0     = @echo "  LD      " 
am__v_LD_1     = :
AM_V_LD        = $(am__v_LD_$(V))
am__v_LD_      = $(am__v_LD_$(AM_DEFAULT_V))

C_TESTS = \
	fabs.c \
	acos.c \
	acosh.c \
	asin.c \
	asinh.c \
	atan.c \
	atan2.c \
	atanh.c \
	cbrt.c \
	cos.c \
	cosh.c \
	exp.c \
	exp10.c \
	exp2.c \
	expm1.c \
	fmod.c \
	frexp.c \
	ldexp.c \
	ilogb.c \
	round.c \
	lround.c \
	log.c \
	log1p.c \
	log10.c \
	log2.c \
	logb.c \
	modf.c \
	pow.c \
	scalb.c \
	scbn.c \
	scbln.c \
	sin.c \
	sincos.c \
	sinh.c \
	sqrt.c \
	hypot.c \
	tan.c \
	tanh.c \
	trunc.c \
	remain.c \
	drem.c \
	remquo.c \
	fadd.c \
	fdiv.c \
	fmul.c \
	fneg.c \
	fsub.c \
	fcmp.c \
	fmax.c \
	fmin.c \
	fdim.c \
	csign.c \
	\
	erf.c \
	erfc.c \
	j0.c \
	j1.c \
	jn.c \
	y0.c \
	y1.c \
	yn.c \
	lgamma.c \
	tgamma.c \
	ceil.c \
	floor.c \
	nearby.c \
	rint.c \
	lrint.c \
	llrint.c \
	llround.c \
	signi.c \
	nextaf.c \
	nextto.c \
	fma.c \
	fpclass.c \
	isinf.c \
	isnan.c \
	finite.c \
	signb.c \
	isfini.c \
	isnorm.c \
	issign.c \
	isunor.c \
	isgr.c \
	isgreq.c \
	isls.c \
	islseq.c \
	islsgr.c \
	\
	$(empty)

MATH_FUNCS = \
	acos asin atan atan2 cos sin tan sincos \
	cosh sinh tanh acosh asinh atanh \
	exp exp2 exp10 log log2 log10 frexp ldexp modf expm1 log1p logb \
	pow pow10 sqrt hypot cbrt \
	ceil fabs floor fmod finite __finite drem significand copysign nan \
	j0 j1 jn y0 y1 yn \
	erf erfc lgamma tgamma gamma lgamma_r \
	rint nextafter nexttoward nextdown nextup remainder scalb scalbn scalbln ilogb llogb nearbyint round trunc remquo \
	lrint llrint lround llround fdim fmax fmin fma roundeven fromfp ufromfp fromfpx ufromfpx \
	fpclassify __fpclassify \
	signbit __signbit \
	isfinite __isfinite \
	isnormal __isnormal \
	isnan __isnan \
	isinf __isinf \
	issignaling __issignaling \
	issubnormal __issubnormal \
	iszero __iszero \
	iscanonical __iscanonical \
	isgreater isgreaterequal isless islessequal islessgreater isunordered iseqsig \
	fmaxmag fminmag totalorder totalordermag canonicalize getpayload setpayload setpayloadsig \
	matherr math_error \
	feclearexcept fegetexceptflag feraiseexcept fesetexcept fetestexcept fetestexceptflag feenableexcept fedisableexcept fegetexcept \
	fegetround fesetround \
	fegetenv feholdexcept fesetenv feupdateenv \
	fegetmode fesetmode \
	cacos casin catan ccos csin ctan \
	ccosh csinh ctanh cacosh casinh catanh \
	cexp clog clog10 \
	cpow csqrt crcp \
	cabs carg conj cproj cimag creal \
	$(empty)


LIB_000 = ../68000.soft-float/libm.a
LIB_020 = ../68020-60.68881/libm.a
LIB_V4E = ../5475.hard-float/libm.a

CPU_FLAGS_000 = -m68000
CPU_FLAGS_020 = -m68020-60 -mhard-float
CPU_FLAGS_V4E = -mcpu=5475 -mhard-float

TESTS = \
	$(addsuffix .ttp, $(addprefix 000/, $(basename $(C_TESTS)))) \
	$(addsuffix .ttp, $(addprefix 881/, $(basename $(C_TESTS)))) \
	$(addsuffix .ttp, $(addprefix v4e/, $(basename $(C_TESTS))))

BUILT_SOURCES = $(TESTS) runall.sh runv4e.sh runall.sc runall.h

EXTRA_DIST = \
	$(C_TESTS) \
	testdriver.h testconfig.h

all: .dirstamp $(TESTS) runall.sh runv4e.sh

define compile_000 =
000/$(1).ttp: $(1).c
	$(AM_V_LD) 000/$(1).ttp; $(CC) $(CFLAGS) $(WARN) $(CPU_FLAGS_000) $(CPPFLAGS) $(LDFLAGS) -o 000/$(1).ttp $(1).c $(LIB_000)
endef

define compile_881 =
881/$(1).ttp: $(1).c
	$(AM_V_LD) 881/$(1).ttp; $(CC) $(CFLAGS) $(WARN) $(CPU_FLAGS_020) $(CPPFLAGS) $(LDFLAGS) -o 881/$(1).ttp $(1).c $(LIB_020)
endef

define compile_v4e =
v4e/$(1).ttp: $(1).c
	$(AM_V_LD) v4e/$(1).ttp; $(CC) $(CFLAGS) $(WARN) $(CPU_FLAGS_V4E) $(CPPFLAGS) $(LDFLAGS) -o v4e/$(1).ttp $(1).c $(LIB_V4E)
endef

$(foreach c,$(basename $(C_TESTS)),$(eval $(call compile_000,$(c))))

$(foreach c,$(basename $(C_TESTS)),$(eval $(call compile_881,$(c))))

$(foreach c,$(basename $(C_TESTS)),$(eval $(call compile_v4e,$(c))))

$(TESTS): testdriver.h testconfig.h config.h

config.h: $(LIB_000) $(LIB_020) $(LIB_V4E) Makefile
	$(AM_V_GEN):
	@( \
	 for i in $(MATH_FUNCS); do \
	   tmp=conftest.c; \
	   echo "char $${i}();" > $$tmp; \
	   echo "int main() {" >> $$tmp; \
	   echo "return $${i}();" >> $$tmp; \
	   echo "}" >> $$tmp; \
	   fn=`echo $$i | tr '[a-z]' '[A-Z]'`; \
	   if $(CC) $(CFLAGS) $(CPU_FLAGS_000) $(CPPFLAGS) $(LDFLAGS) $$tmp $(LIB_000) 2>/dev/null; then \
	      echo "#define HAVE_$${fn} 1"; \
	   else \
	      echo "#undef HAVE_$${fn}"; \
	   fi; \
	   $(RM) $$tmp; \
	 done; \
	) > $@

runall.sh: Makefile
	$(AM_V_GEN):
	@( \
	 echo '#!/bin/sh'; \
	 echo "all_status=0"; \
	 echo "PATH=.:\$$PATH"; \
	 echo -n "for i in"; \
	 for i in $(C_TESTS:.c=); do echo -n " $$i"; done; \
	 echo "; do "; \
	 echo '  logfile=000/$$i.log'; \
	 echo '  000/$$i.ttp >$$logfile 2>&1'; \
	 echo '  status=$$?'; \
	 echo '  if test $$status = 77; then'; \
	 echo '    status=SKIPPED'; \
	 echo '  elif test $$status = 0; then'; \
	 echo '    status=OK'; \
	 echo '  else'; \
	 echo '    status=FAILED'; \
	 echo '    all_status=1'; \
	 echo '  fi'; \
	 echo '  echo result: $$status >>$$logfile'; \
	 echo '  echo 000/$$i: $$status'; \
	 echo '  logfile=881/$$i.log'; \
	 echo '  881/$$i.ttp >$$logfile 2>&1'; \
	 echo '  status=$$?'; \
	 echo '  if test $$status = 77; then'; \
	 echo '    status=SKIPPED'; \
	 echo '  elif test $$status = 0; then'; \
	 echo '    status=OK'; \
	 echo '  else'; \
	 echo '    status=FAILED'; \
	 echo '    all_status=1'; \
	 echo '  fi'; \
	 echo '  echo result: $$status >>$$logfile'; \
	 echo '  echo 881/$$i: $$status'; \
	 echo done; \
	 echo 'exit $$all_status' \
	) > $@
	$(AM_V_at)chmod 755 $@

runv4e.sh: Makefile
	$(AM_V_GEN):
	@( \
	 echo '#!/bin/sh'; \
	 echo "all_status=0"; \
	 echo "PATH=.:\$$PATH"; \
	 echo -n "for i in"; \
	 for i in $(C_TESTS:.c=); do echo -n " $$i"; done; \
	 echo "; do "; \
	 echo '  logfile=v4e/$$i.log'; \
	 echo '  v4e/$$i.ttp >$$logfile 2>&1'; \
	 echo '  status=$$?'; \
	 echo '  if test $$status = 77; then'; \
	 echo '    status=SKIPPED'; \
	 echo '  elif test $$status = 0; then'; \
	 echo '    status=OK'; \
	 echo '  else'; \
	 echo '    status=FAILED'; \
	 echo '    all_status=1'; \
	 echo '  fi'; \
	 echo '  echo result: $$status >>$$logfile'; \
	 echo '  echo v4e/$$i: $$status'; \
	 echo done; \
	 echo 'exit $$all_status' \
	) > $@
	$(AM_V_at)chmod 755 $@

check:: all

runall.h: Makefile
	$(AM_V_GEN)( \
	 for i in $(C_TESTS:.c=); do \
	   echo "TEST($$i)"; \
         done \
	) > $@

install::

.dirstamp:
	-@mkdir 000 881 v4e
	@touch $@

clean::
	$(RM) */*.o *.o */*.ttp *.ttp */*.exe *.log */*.log runall.sh runv4e.sh runall.h config.h
	-rmdir 000 881 v4e 2>/dev/null
	-@$(RM) .dirstamp

distclean: clean
