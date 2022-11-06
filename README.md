# Draw_Numbers

Imagine a multiplication table. Some numbers appear multiple times. What if we colored in the cells, in which the appearance of a number is closest in position to the 0,0 corner of the table?


This program graphs a function, where (x,y) are the pair of factors of a number that result in the smallest distance (x^2 + y^2) from (0,0). 
	
	For example, 12 has the following factors {1, 2, 3, 4, 6, 12}
	1 * 12 = 12, 1 + 144 = 145
	2 * 6 = 12, 4 + 36 = 40
	3 * 4 = 12, 9 + 16 = 25 <--- Minimal sum of squares.
	
	Thus, for 12 we get the pair (4,3).

This program makes the assumption that the two factors of a number that produce the smallest sqrt(factor1^2 + factor2^2), is the biggest factor under sqrt(number), and its pair (number/factor).

This is a hobby-level excercise at what I could cram into a program that might logically need it.

What I learned during this project:

std::chrono: Timekeeping, benchmarking, casting time types

std::fstream: reading, writing, and creating files through code.

win32: Manipulation of the console, from size, window properties (like removing elements for a true fullscreen)
	   Cursor position, ``std::cout << '\r'`` etc.
	   
In general: Structuring code in a more intuitive order leads to less brein-teaser situations when bug-hunting.
			Not using one function with 100 arguments for literally everything
			segmenting code into functions with descriptive names
			
To do: Implement threading, currently 26 seconds to calculate and 116 seconds reading from file..
