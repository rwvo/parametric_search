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

CXXFLAGS = -c -I. -I$(PMS_INCL_DIR) -O4 \
	   -Wunused-value -Wunused-parameter -Wunused-function \
           -Wswitch -Wno-long-long


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
#                    target entries
#---------------------------------------------------------------------#

median_of_lines: $(OBJECT_FILES) ${INSTANCES}
	$(CXX) -o median_of_lines $(OBJECT_FILES) ${INSTANCES}



#---------------------------------------------------------------------#
#                    suffix rules
#---------------------------------------------------------------------#

%_inst.o: $(PMS_INCL_DIR)/%_inst.cpp $(PMS_INCL_DIR)/%.cpp instantiate.h
	$(CXX) $(CXXFLAGS) $(PMS_INCL_DIR)/$*_inst.cpp

%_inst.o: %_inst.cpp %.cpp instantiate.h
	$(CXX) $(CXXFLAGS) $*_inst.cpp

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $<

#---------------------------------------------------------------------

clean:
	rm -f *.o *~


#---------------------------------------------------------------------#
#      Dependencies inferred by makedepend -Y. -Y../.. *.cpp *.h
#---------------------------------------------------------------------#

# DO NOT DELETE

line_comp.o: line_comp.h
line_comp_inst.o: instantiate.h line_traits.h $(PMS_INCL_DIR)/traits_base.h
line_comp_inst.o: $(PMS_INCL_DIR)/scheduler.h $(PMS_INCL_DIR)/process_base.h
line_comp_inst.o: $(PMS_INCL_DIR)/spawner.h $(PMS_INCL_DIR)/spawnable.h
line_comp_inst.o: $(PMS_INCL_DIR)/wrapper.h $(PMS_INCL_DIR)/comparison_base.h
line_comp_inst.o: $(PMS_INCL_DIR)/root.h $(PMS_INCL_DIR)/solver_base.h line.h
line_comp_inst.o: line_comp.h line_solver.h line_comp.cpp
line_solver.o: line_solver.h line.h
line_solver_inst.o: instantiate.h line_traits.h $(PMS_INCL_DIR)/traits_base.h
line_solver_inst.o: $(PMS_INCL_DIR)/scheduler.h $(PMS_INCL_DIR)/process_base.h
line_solver_inst.o: $(PMS_INCL_DIR)/spawner.h $(PMS_INCL_DIR)/spawnable.h
line_solver_inst.o: $(PMS_INCL_DIR)/wrapper.h $(PMS_INCL_DIR)/comparison_base.h
line_solver_inst.o: $(PMS_INCL_DIR)/root.h $(PMS_INCL_DIR)/solver_base.h line.h
line_solver_inst.o: line_comp.h line_solver.h line_solver.cpp
median_of_lines.o: line_traits.h $(PMS_INCL_DIR)/traits_base.h
median_of_lines.o: $(PMS_INCL_DIR)/scheduler.h $(PMS_INCL_DIR)/process_base.h
median_of_lines.o: $(PMS_INCL_DIR)/spawner.h $(PMS_INCL_DIR)/spawnable.h
median_of_lines.o: $(PMS_INCL_DIR)/wrapper.h $(PMS_INCL_DIR)/comparison_base.h
median_of_lines.o: $(PMS_INCL_DIR)/root.h $(PMS_INCL_DIR)/solver_base.h line.h
median_of_lines.o: line_comp.h line_solver.h $(PMS_INCL_DIR)/quick_sorter.h
instantiate.o: line_traits.h $(PMS_INCL_DIR)/traits_base.h
instantiate.o: $(PMS_INCL_DIR)/scheduler.h $(PMS_INCL_DIR)/process_base.h
instantiate.o: $(PMS_INCL_DIR)/spawner.h $(PMS_INCL_DIR)/spawnable.h
instantiate.o: $(PMS_INCL_DIR)/wrapper.h $(PMS_INCL_DIR)/comparison_base.h
instantiate.o: $(PMS_INCL_DIR)/root.h $(PMS_INCL_DIR)/solver_base.h line.h
instantiate.o: line_comp.h line_solver.h
line_solver.o: line.h
line_traits.o: $(PMS_INCL_DIR)/traits_base.h $(PMS_INCL_DIR)/scheduler.h
line_traits.o: $(PMS_INCL_DIR)/process_base.h $(PMS_INCL_DIR)/spawner.h
line_traits.o: $(PMS_INCL_DIR)/spawnable.h $(PMS_INCL_DIR)/wrapper.h
line_traits.o: $(PMS_INCL_DIR)/comparison_base.h $(PMS_INCL_DIR)/root.h
line_traits.o: $(PMS_INCL_DIR)/solver_base.h line.h line_comp.h line_solver.h
