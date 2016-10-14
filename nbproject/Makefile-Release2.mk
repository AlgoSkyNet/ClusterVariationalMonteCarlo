#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release2
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/newmain.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/newsimpletest.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 -msse3 -fopenmp -O3
CXXFLAGS=-m64 -msse3 -fopenmp -O3

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=/usr/lib/liblapack.a /usr/lib/atlas-base/atlas/libblas.a /usr/lib/gcc/x86_64-linux-gnu/4.6.1/libgfortran.a /opt/gmp-5.0.2/lib/libgmp.a /opt/gmp-5.0.2/lib/libgmpxx.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7: /usr/lib/liblapack.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7: /usr/lib/atlas-base/atlas/libblas.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7: /usr/lib/gcc/x86_64-linux-gnu/4.6.1/libgfortran.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7: /opt/gmp-5.0.2/lib/libgmp.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7: /opt/gmp-5.0.2/lib/libgmpxx.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7 ${OBJECTFILES} ${LDLIBSOPTIONS} -fopenmp -static

${OBJECTDIR}/src/newmain.o: src/newmain.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/opt/gmp-5.0.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/newmain.o src/newmain.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/newsimpletest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/newsimpletest.o: tests/newsimpletest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -I/opt/gmp-5.0.2/include -I. -I. -I. -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newsimpletest.o tests/newsimpletest.c


${OBJECTDIR}/src/newmain_nomain.o: ${OBJECTDIR}/src/newmain.o src/newmain.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/newmain.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/opt/gmp-5.0.2/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/newmain_nomain.o src/newmain.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/newmain.o ${OBJECTDIR}/src/newmain_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clustervariationalmontecarloperf7

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
