##################
### MACROS
##################
#$(1) : Compiler
#$(2) : Object file to generate
#$(3) : Source file
#$(4) : Additional dependencies
#$(5) : Compiler flags
define COMPILER
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5) $(ADDSAN)
endef
# $(1) : Source file
define C20
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef
##################
### CONFIG
##################



APP 	:= DisasterGauntlet.exe
#CMMFLAG := -Wall -pedantic -ggdb3
CMMFLAG := -O2
#ADDSAN  := -fsanitize=address
CCFLAGS := $(CMMFLAG) -std=c++17
CFLAGS	:= $(CMMFLAG)
MKDIR	:= mkdir -p
PTHREAD := -pthread
SRC  	:= src
OBJ 	:= obj
LIBDIR  := lib
# para que busque la librería en la carpeta lib del proyecto, no del sistema
# si hay una igual en el sistema cogerá esa porque tiene preferencia
LIBSFMOD:= -L$(LIBDIR) -lfmod -lfmodstudio
LIBSGRH := -lglew32 -lassimp -lglfw3 -lopengl32 -limm32
LIBS 	:= -lBulletCollision -lBullet3Common -lBulletDynamics -lLinearMath $(LIBSGRH) $(LIBSFMOD)
#DIRECTORIOS DE INCLUDES DE LIBRERIAS ESTÁTICAS
INCRNDM := -I$(LIBDIR)/randomyEngine
#DIRECTORIOS DE INCLUDES DE LIBRERIAS DINÁMICAS
INCFMOD := -I$(LIBDIR)/fmod/
INCDIRS := -I$(LIBDIR) $(INCRNDM) $(INCFMOD) $(INCPYBULLET)
PATHS   := 

# todos los .cpp dentro de src (con -iname no importa si el nombre tiene mayus)
ALLCPPS := $(shell find src/ -type f -iname *.cpp)
# todos los .c dentro de src (con -iname no importa si el nombre tiene mayus)
ALLCS 	:= $(shell find src/ -type f -iname *.c)
# todos los .a dentro de lib (con -iname no importa si el nombre tiene mayus)
ALLSTATIC:= $(shell find lib/ -type f -iname "*.a")
# todos los .so dentro de subdirectorios de lib
ALLDYNAMICFMODSTUDIO:= $(shell find $(LIBDIR) -type f -iname *.so)
ALLOBJ	:= $(foreach F,$(ALLCPPS) $(ALLCS),$(call C20,$(F)))

# compiladores (recomendado utilizar varios)
CC 		:= g++
C		:= gcc

# para tener la lista de directorios que hay en src
SUBDIRS := $(shell find $(SRC) -type d)

OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

# con phony le decimos al make que dir no es un fichero de salida
.PHONY: info libs libs-clean libs-cleanall

# primero se generan los directorios despues los .o
$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
# y entonces se ejecutan estos comandos
#incluimos tambien los ficheros .a, ya que están precompilados
	$(CC) $(PTHREAD) -o $(APP) $(ALLOBJ) $(ALLSTATIC) $(PATHS) $(LIBS) $(ADDSAN)

#GENERATE RULES FOR ALL OBJECTS
$(foreach F,$(ALLCPPS),$(eval $(call COMPILER,$(CC),$(call C20,$(F)),$(F),$(call C2H,$(F)),$(CCFLAGS) $(INCDIRS))))
$(foreach F,$(ALLCS),$(eval $(call COMPILER,$( C),$(call C20,$(F)),$(F),$(call C2H,$(F)),$(CFLAGS) $(INCDIRS))))

##$(OBJ)/%.o : $(SRC)/%.cpp
##	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS)




## CLEAN RULES
clean:
	$(RM) -r "./$(OBJ)"


cleanall: clean
	$(RM) -r "./$(APP)"

libs:
	$(MAKE) -C $(LIBDIR)

libs-clean:
	$(MAKE) -C $(LIBDIR) clean

libs-cleanall:
	$(MAKE) -C $(LIBDIR) cleanall
	

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCS))
	$(info $(ALLOBJ))

$(OBJSUBDIRS):
	$(MKDIR) $(OBJSUBDIRS)