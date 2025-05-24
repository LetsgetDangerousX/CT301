# Optional Flags
opts=-D_FORTIFY_SOURCE=2 -fno-diagnostics-show-option	\
-fstack-protector-all -g -Walloc-zero -Walloca	\
-Wctor-dtor-privacy -Wduplicated-cond -Wduplicated-branches		\
-Wextra -Winit-self -Wlogical-op -Wno-unused-variable -Wno-unused-parameter  \
-Wold-style-cast -Wpedantic -Wshadow -Wunused-const-variable=2	\
-Wzero-as-null-pointer-constant
CXXFLAGS=-Wall -Werror -Wfatal-errors $(opts)

assignment=HW1

$(assignment): 
		g++ $(CXXFLAGS) -o $(assignment) $(assignment).cpp

tar:
		tar -cf $(assignment).tar *.cpp Makefile

clean:
		rm -f $(assignment) $(assignment).tar *.o *.gch *.gcov a.out