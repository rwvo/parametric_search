#---------------------------------------------------------------------#
# Include the appropriate Makefile. See the CGAL documentation
#---------------------------------------------------------------------#

# Either define CGAL_MAKEFILE as an environment variable, or define it
# here, for example:

# CGAL_MAKEFILE = /data2/packages/CGAL-2.3/make/makefile_i686_Linux-2.2.16-22_g++-3-3.0.1

include $(CGAL_MAKEFILE)

#---------------------------------------------------------------------#
#      path to the parametric search framework files
#---------------------------------------------------------------------#

PMS_INCL_DIR = $(CGAL_INCL_DIR)/CEP/parametric_search


#---------------------------------------------------------------------#
#                    compiler flags
#---------------------------------------------------------------------#

CXXFLAGS = $(CGAL_CXXFLAGS) -I. -I$(PMS_INCL_DIR) -O4 \
           $(LONG_NAME_PROBLEM_CXXFLAGS) \
           $(DEBUG_OPT) \
	   -Wunused-value -Wunused-parameter -Wunused-function \
           -Wswitch -Wno-long-long 


#---------------------------------------------------------------------#
#                    linker flags
#---------------------------------------------------------------------#

LIBPATH = $(CGAL_LIBPATH)

LDFLAGS = $(LONG_NAME_PROBLEM_LDFLAGS) \
          $(CGAL_LDFLAGS)

#---------------------------------------------------------------------#
#                    object files
#---------------------------------------------------------------------#


INSTANCES = scheduler_inst.o \
            spawner_inst.o \
            spawnable_inst.o \
            process_base_inst.o \
            comparison_base_inst.o \
            root_inst.o \
	    solver_base_inst.o \
	    quick_sorter_inst.o \
	    frechet_preprocessor_inst.o \
	    frechet_comp_inst.o \
	    frechet_cell_inst.o \
            frechet_polynomial_inst.o \
	    frechet_solver_inst.o \
	    frechet_distance_inst.o

OBJECT_FILES = frechet_distance_driver.o

#---------------------------------------------------------------------#
#                    target entries
#---------------------------------------------------------------------#

frechet_distance_driver$(EXE_EXT): $(OBJECT_FILES) ${INSTANCES}
	$(CGAL_CXX) $(LIBPATH) $(EXE_OPT)frechet_distance_driver \
	$(OBJECT_FILES) ${INSTANCES} $(LDFLAGS)



#---------------------------------------------------------------------#
#                    suffix rules
#---------------------------------------------------------------------#

%_inst.o: $(PMS_INCL_DIR)/%_inst.cpp $(PMS_INCL_DIR)/%.cpp instantiate.h
	$(CGAL_CXX) $(CXXFLAGS) $(OBJ_OPT) $(PMS_INCL_DIR)/$*_inst.cpp

%_inst.o: %_inst.cpp %.cpp instantiate.h
	$(CGAL_CXX) $(CXXFLAGS) $(OBJ_OPT) $*_inst.cpp

%.o: %.cpp 
	$(CGAL_CXX) $(CXXFLAGS) $(OBJ_OPT) $<



#---------------------------------------------------------------------

clean:
	rm -f *.o *~


#---------------------------------------------------------------------#
#       Dependencies inferred by makedepend -Y. -Y../.. *.cpp *.h
#---------------------------------------------------------------------#
# DO NOT DELETE

frechet_distance_driver.o: $(PMS_INCL_DIR)/frechet_traits.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/traits_base.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/scheduler.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/process_base.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/spawner.h $(PMS_INCL_DIR)/spawnable.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/wrapper.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/comparison_base.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/root.h $(PMS_INCL_DIR)/solver_base.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/frechet_polynomial.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/frechet_comp.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/frechet_solver.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/frechet_cell.h cgal_kernel.h
frechet_distance_driver.o: $(PMS_INCL_DIR)/frechet_distance.h instantiate.h
frechet_distance_inst.o: instantiate.h $(PMS_INCL_DIR)/frechet_traits.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/traits_base.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/scheduler.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/process_base.h $(PMS_INCL_DIR)/spawner.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/spawnable.h $(PMS_INCL_DIR)/wrapper.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/comparison_base.h $(PMS_INCL_DIR)/root.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/solver_base.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/frechet_polynomial.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/frechet_comp.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/frechet_solver.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/frechet_cell.h cgal_kernel.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/frechet_distance.cpp
frechet_distance_inst.o: $(PMS_INCL_DIR)/frechet_distance.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/frechet_preprocessor.h
frechet_distance_inst.o: $(PMS_INCL_DIR)/quick_sorter.h
instantiate.o: $(PMS_INCL_DIR)/frechet_traits.h $(PMS_INCL_DIR)/traits_base.h
instantiate.o: $(PMS_INCL_DIR)/scheduler.h $(PMS_INCL_DIR)/process_base.h
instantiate.o: $(PMS_INCL_DIR)/spawner.h $(PMS_INCL_DIR)/spawnable.h
instantiate.o: $(PMS_INCL_DIR)/wrapper.h $(PMS_INCL_DIR)/comparison_base.h
instantiate.o: $(PMS_INCL_DIR)/root.h $(PMS_INCL_DIR)/solver_base.h
instantiate.o: $(PMS_INCL_DIR)/frechet_polynomial.h
instantiate.o: $(PMS_INCL_DIR)/frechet_comp.h $(PMS_INCL_DIR)/frechet_solver.h
instantiate.o: $(PMS_INCL_DIR)/frechet_cell.h cgal_kernel.h
