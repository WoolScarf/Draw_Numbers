# Draw_Numbers

Imagine a multiplication table. Some numbers appear multiple times. What if we colored in the cells, in which the appearance of a number is closest in position to the 0,0 corner of the table?


This program graphs a function, where (x,y) are the pair of factors of a number that result in the smallest distance (x^2 + y^2) from (0,0). 
	
	For example, 12 has the following factors {1, 2, 3, 4, 6, 12}
	1 * 12 = 12, 1 + 144 = 145
	2 * 6 = 12, 4 + 36 = 40
	3 * 4 = 12, 9 + 16 = 25 <--- Minimal sum of squares.
	
	Thus, for 12 we get the pair (4,3).

This program makes the assumption that the two factors of a number that produce the smallest sqrt(factor1^2 + factor2^2), is the biggest factor under sqrt(number), and its pair (number/factor).

This is the result of the program being run, https://imgur.com/a/ZyP9B5S. 

Picture #1: The graph in black and white

Picture #2: The "sectors" divided by the following lines:
		y = a * x where for each of the numbers 1, 1/1.25, 1/1.5, 1/2, 1/3, 1/4, 1/5
		
Picture #3: This table shows the value for floor(Y/X), amount of numbers counter in that group, and the % of the count out of 2.1m numbers. Lastly, it shows the geometric rate of change from one value to the next. 
		

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
