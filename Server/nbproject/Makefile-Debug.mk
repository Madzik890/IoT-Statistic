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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/DataBase/DataBase.o \
	${OBJECTDIR}/src/Device/Device.o \
	${OBJECTDIR}/src/ServerCallback.o \
	${OBJECTDIR}/src/SeverSSL.o \
	${OBJECTDIR}/src/pugixml.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server ${OBJECTFILES} ${LDLIBSOPTIONS} -lssl -lcrypto

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/pugixml -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/DataBase/DataBase.o: src/DataBase/DataBase.cpp
	${MKDIR} -p ${OBJECTDIR}/src/DataBase
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/pugixml -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DataBase/DataBase.o src/DataBase/DataBase.cpp

${OBJECTDIR}/src/Device/Device.o: src/Device/Device.cpp
	${MKDIR} -p ${OBJECTDIR}/src/Device
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/pugixml -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Device/Device.o src/Device/Device.cpp

${OBJECTDIR}/src/ServerCallback.o: src/ServerCallback.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/pugixml -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ServerCallback.o src/ServerCallback.cpp

${OBJECTDIR}/src/SeverSSL.o: src/SeverSSL.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/pugixml -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SeverSSL.o src/SeverSSL.cpp

${OBJECTDIR}/src/pugixml.o: src/pugixml.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/pugixml -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pugixml.o src/pugixml.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
