ALL:			 cUnitTests
CFLAGS	         = -I $(INC_DIR) -I $(TEST_INC_DIR)
FFLAGS	         =
CPPFLAGS         =
FPPFLAGS         =
CLEANFILES       = cUnitTests $(OBJ) $(TEST_OBJ)

SRC_DIR			 = ../../src
INC_DIR			 = ../../include

TEST_SRC_DIR 	 = src
TEST_INC_DIR 	 = include

SRC 			 = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
OBJ 			 = $(filter-out $(SRC_DIR)/main.o, $(patsubst %.c,%.o,$(SRC)))

TEST_SRC 		 = $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJ 		 = $(patsubst %.c,%.o,$(TEST_SRC))

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

cUnitTests: $(OBJ) $(TEST_OBJ) chkopts
	${CLINKER} -o cUnitTests $(OBJ) $(TEST_OBJ) ${PETSC_KSP_LIB} -lcriterion
	${RM} $(OBJ) $(TEST_OBJ)

runcUnitTests:
	-@${MPIEXEC} -n 1 ./cUnitTests