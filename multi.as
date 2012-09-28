        lw      0       2       mcand		// Set mcand
    		lw			0				3				mplier	// Set mplier
				lw			0				6				count
				lw			0				4				neg2
				lw			0				5				max
loop		beq			6				5				exit
				nand		3				6				7
				beq			4				7				1
				add			1				2				1				// add mcand to result
				add			2				2				2				// mcand << 1
				add			6				6				6				// increase count
				beq			0				0				loop
exit    halt                         
mcand		.fill		32766               
mplier	.fill		10383
count		.fill		1
max			.fill		1073741824
one			.fill		1
neg2		.fill		-1