ALL:			 model
CFLAGS	         = -I $(INC_DIR) -I $(TEST_INC_DIR)
FFLAGS	         =
CPPFLAGS         =
FPPFLAGS         =
CLEANFILES       = model

SRC_DIR			 = src
INC_DIR			 = include

TEST_SRC_DIR 	 = tests/src
TEST_INC_DIR 	 = tests/include

SRC 			 = $(wildcard $(SRC_DIR)/*.c)
OBJ 			 = $(patsubst %.c,%.o,$(SRC))

TEST_SRC 		 = $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJ 		 = $(patsubst %.c,%.o,$(TEST_SRC))

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

model: $(OBJ) $(TEST_OBJ) chkopts
	${CLINKER} -o model $(OBJ) $(TEST_OBJ) ${PETSC_KSP_LIB} 
	${RM} $(OBJ) $(TEST_OBJ)

runmodel:
	-@${MPIEXEC} -n 1 ./model -info