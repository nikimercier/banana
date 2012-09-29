        lw      0       2       mcand		// Set mcand
    		lw			0				3				mplier	// Set mplier
				sw      0       2				100
				sw      0       3       101
				add     0       1		    2
				add     0       2     	3
				lw			0				2				100
				lw			0				3				101
exit    halt                         
mcand		.fill		12345               
mplier	.fill		54321
