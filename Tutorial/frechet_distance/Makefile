#---------------------------------------------------------------------#
# Basic configuration
#---------------------------------------------------------------------#

CXX=clang++
CGAL_INCL_DIR = /usr/include

#---------------------------------------------------------------------#
#      path to the parametric search framework files
#---------------------------------------------------------------------#

PMS_INCL_DIR = ../../include


#---------------------------------------------------------------------#
#                    compiler flags
#---------------------------------------------------------------------#

CXXFLAGS = -I. -I$(PMS_INCL_DIR) -I$(CGAL_INCL_DIR) -O3 \
           -Werror -Wextra

#---------------------------------------------------------------------#
#                    linker flags
#---------------------------------------------------------------------#

LDFLAGS = -lCGAL

#---------------------------------------------------------------------#
# object files
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
#                    dependencies
#---------------------------------------------------------------------#

DEPENDENCIES = $(patsubst %.o,%.d,$(INSTANCES))
DEPENDENCIES += $(patsubst %.o,%.d,$(OBJECT_FILES))

#---------------------------------------------------------------------#
#                    target entries
#---------------------------------------------------------------------#

frechet_distance_driver: $(OBJECT_FILES) ${INSTANCES}
	$(CXX) -o $@ \
	$(OBJECT_FILES) ${INSTANCES} $(LDFLAGS)



#---------------------------------------------------------------------#
#                    suffix rules
#---------------------------------------------------------------------#

%_inst.o: $(PMS_INCL_DIR)/%_inst.cpp $(PMS_INCL_DIR)/%.cpp instantiate.h
	$(CXX) $(CXXFLAGS) -MM $(PMS_INCL_DIR)/$*_inst.cpp -o $*_inst.d
	$(CXX) $(CXXFLAGS) -c $(PMS_INCL_DIR)/$*_inst.cpp -o $@

%_inst.o: %_inst.cpp %.cpp instantiate.h
	$(CXX) $(CXXFLAGS) -MM $*_inst.cpp -o $*_inst.d
	$(CXX) $(CXXFLAGS) -c $*_inst.cpp -o $@

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -MM $< -o $*.d
	$(CXX) $(CXXFLAGS) -c $< -o $@


#---------------------------------------------------------------------#
#                    housekeeping
#---------------------------------------------------------------------#


clean:
	rm -f *.o *.d *~

-include $(DEPENDENCIES)



