ALL:			 model
CFLAGS	         = -I $(INC_DIR) -I $(TEST_INC_DIR)
FFLAGS	         =
CPPFLAGS         =
FPPFLAGS         =
CLEANFILES       = model $(OBJ) $(TEST_OBJ)

DAT_DIR 		 = data/dat
PAR_DIR 		 = data/par

SRC_DIR			 = src
INC_DIR			 = include

TEST_SRC_DIR 	 = tests/integrationTests/src
TEST_INC_DIR 	 = tests/integrationTests/include

DAT 			 = $(wildcard $(DAT_DIR)/lmb/*_in.dat)
PAR              = $(patsubst %_in.dat, %.par, $(DAT))

SRC 			 = $(wildcard $(SRC_DIR)/*.c)
OBJ 			 = $(patsubst %.c,%.o,$(SRC))

TEST_SRC 		 = $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJ 		 = $(patsubst %.c,%.o,$(TEST_SRC))

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

%.par: %_in.dat 
	@echo "Generating $@ from $<" 

parfiles: $(PAR)
	@echo "Generating par files $(PAR)"

model: $(OBJ) $(TEST_OBJ) chkopts
	${CLINKER} -o model $(OBJ) $(TEST_OBJ) ${PETSC_KSP_LIB} 
	${RM} $(OBJ) $(TEST_OBJ)

runmodel:
	-@${MPIEXEC} -n 1 ./model

runmodelinfo:
	-@${MPIEXEC} -n 1 ./model -info
