# beps.make
# make file to compile beps program

# CFLAGS=-std=c99 -O3
CFLAGS=-std=c99 -g -O3 -Iinclude

ODIR=build
IDIR=src

# first name a variable OBJSects for all OBJSect files
_OBJS = aerodynamic_conductance.o bepsmain_pnt.o calc_temp_leaf.o \
  inter_prg.o evaporation_canopy.o evaporation_soil.o init_soil.o \
  lai_calc.o meteo_pack.o netRadiation.o photosyn_gs.o plant_respir.o \
  rainfall.o readcoef.o readparam.o s_coszs.o \
  sensible_heat.o snowpack.o soil_thermal_regime.o \
  soil_water_stress.o soilresp.o surface_temp.o \
  transpiration.o updatesoilmoisture.o vcmax_jmax_mod.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# # name a variable sources for all source files
# sources = aerodynamic_conductance.c bepsmain_pnt.c calc_temp_leaf.c \
#   inter_prg.c evaporation_canopy.c evaporation_soil.c init_soil.c \
#   lai_calc.c meteo_pack.c netRadiation.c photosyn_gs.c plant_respir.c \
#   rainfall.c readcoef.c readparam.c s_coszs.c \
#   sensible_heat.c snowpack.c soil_thermal_regime.c \
#   soil_water_stress.c soilresp.c surface_temp.c \
#   transpiration.c updatesoilmoisture.c vcmax_jmax_mod.c 


# now give target as makebeps with OBJSects as variable dependencies + command line

# gcc=/usr/bin/gcc-9
CC=gcc
#	icc -g -lm -o bepsp $(OBJSects)
# echo $(OBJS) 

bepsp.exe: $(OBJS) 
	echo $(OBJS) 
	$(CC) $(CFLAGS) -o bepsp.exe $(OBJS) -lm
	# gcc $(CFLAGS) -o bepsp $(OBJSects) -lm

# list the dependencies for OBJSect files - those header files which help build OBJSects
# how to build all OBJSect files from all dependent source files

$(ODIR)/%.o: $(IDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(OBJS) bepsp.exe

# $(OBJSects): $(sources)
#	icc -g -c $(sources)
# gcc $(CFLAGS) -c $(sources)


