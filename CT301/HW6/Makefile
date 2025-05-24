NETID=c837364858
#Used for naming your tar file
ASN=HW6
#Used for naming the executable
SOURCES= grayCorrelation.cpp main.cpp
#List any files you need to make for source files
HEADERS=grayCorrelation.h
#List any Header files
OBJECTS= $(SOURCES:.cpp=.o)
#List any Object files you need to create for your assignment
TARNAME=$(NETID)_$(ASN).tar

#Change to del if on a windows machine
#Used to create a standard tar file naming convention

#Required Content Do not Modify below this line
CXXFLAGS=-Wall -Werror -Wfatal-errors -O2

%.o: %.cpp
		g++ -c -g $(CXXFLAGS) $<

$(ASN): $(OBJECTS)
		g++ $(CXXFLAGS) -o $(ASN) $(OBJECTS)

tar:
		tar -cf $(TARNAME) $(SOURCES) $(HEADERS) Makefile

clean:
		$(RM) -f $(TARNAME) $(ASN) $(OBJECTS) *.gch *.gcov a.out
