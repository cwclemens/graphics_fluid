6.837 Assignment 3
Walter Menendez

Running Code
	The syntax for my command line arguments was intended to be ./a3 SYSTEM NUM SOLVER H
	SYSTEM = S or P or C 
	NUM = number of particles
	SOLVER = E or T or R
	H = step size

Collaboration
	I collaborated with Patricia Saylor, Stephanie Gu, Lili S.

References
	None.

Problems
	The flex springs aren't working. I could tell when as I added the other two kinds incrementally that those were fine and were doing the right things but the flex springs drag strangely and don't contract. I would fix them by changing my spring representation as I was using helper functions instead of a list of springs. I thought not using a separate structure would be easier since I could just pass in the relevant information, but I was wrong.
	It could also be a matter of fiddling with the constants as well. It explodes for a bit, so my values are probably just too big or something, it was hard to tell given the step size.

	I also can't properly configure my UI. I didn't have time to implement the moving of the cloth for trying to debug the cloth so I had trouble also trying to parse the command line arguments.

Extra Credit
	No attempt.