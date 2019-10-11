ALL:             model tags check
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

DAT 			 = $(wildcard $(DAT_DIR)/*/*_in.dat)
PAR              = $(patsubst %_in.dat, %.par, $(DAT))

SRC 			 = $(wildcard $(SRC_DIR)/*.c)
OBJ 			 = $(patsubst %.c,%.o,$(SRC))

TEST_SRC 		 = $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJ 		 = $(patsubst %.c,%.o,$(TEST_SRC))

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

tags: $(SRC) 
	@echo "Rebuilding tags file"
	ctags -R .

%.par: %_in.dat 
	cd $(PAR_DIR); ./generate_par_file.py $<
# @echo "Generated $@ from $<" 

parfiles: $(PAR)
	@echo "Generating par files $(PAR)"
	@echo "[WARNING] Needs python3 alias!" 

check:
	./auto_file_check.sh "lmb"
	./auto_file_check.sh "rnd2D"
	./auto_file_check.sh "lat3D"

model: $(OBJ) $(TEST_OBJ) chkopts
	${CLINKER} -o model $(OBJ) $(TEST_OBJ) ${PETSC_KSP_LIB} 
	${RM} $(OBJ) $(TEST_OBJ)

runmodel:
	-@${MPIEXEC} -n 1 ./model

runmodelinfo:
	-@${MPIEXEC} -n 1 ./model -info
