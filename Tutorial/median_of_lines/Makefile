#---------------------------------------------------------------------#
# Basic configuration
#---------------------------------------------------------------------#

CXX=clang++

#---------------------------------------------------------------------#
#      path to the parametric search framework files
#---------------------------------------------------------------------#

PMS_INCL_DIR = ../../include


#---------------------------------------------------------------------#
#                    compiler flags
#---------------------------------------------------------------------#

CXXFLAGS = -I. -I$(PMS_INCL_DIR) -O3 -Werror -Wextra

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
	    line_comp_inst.o \
	    line_solver_inst.o 


OBJECT_FILES = median_of_lines.o

#---------------------------------------------------------------------#
#                    dependencies
#---------------------------------------------------------------------#

DEPENDENCIES = $(patsubst %.o,%.d,$(INSTANCES))
DEPENDENCIES += $(patsubst %.o,%.d,$(OBJECT_FILES))

#---------------------------------------------------------------------#
#                    target entries
#---------------------------------------------------------------------#

median_of_lines: $(OBJECT_FILES) ${INSTANCES}
	$(CXX) -o median_of_lines $(OBJECT_FILES) ${INSTANCES}



#---------------------------------------------------------------------#
#                    suffix rules
#---------------------------------------------------------------------#

%_inst.o: $(PMS_INCL_DIR)/%_inst.cpp $(PMS_INCL_DIR)/%.cpp instantiate.h
	$(CXX) $(CXXFLAGS) -MM $(PMS_INCL_DIR)/$*_inst.cpp -o $*_inst.d
	$(CXX) $(CXXFLAGS) -c $(PMS_INCL_DIR)/$*_inst.cpp -o $@

%_inst.o: %_inst.cpp %.cpp instantiate.h
	$(CXX) -MM $(CXXFLAGS) $*_inst.cpp -o $*_inst.d
	$(CXX) -c $(CXXFLAGS) $*_inst.cpp -o $@

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -MM $< -o $*.d
	$(CXX) $(CXXFLAGS) -c $< -o $@

#---------------------------------------------------------------------#
#                    housekeeping
#---------------------------------------------------------------------#

clean:
	rm -f *.o *.d *~

-include $(DEPENDENCIES)
