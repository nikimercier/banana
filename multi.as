        lw      0       2       mcand		// Set mcand
    		lw			0				3				mplier	// Set mplier
loop		beq			3				0				exit
				lw			0				4				one
				lw			0				5				neg1
				nand		3				4				6				// store the mask in reg[5]
				beq			6				5				sll
				add			1				2				1				// product += mcand	
sll			add			2				2				2				// mcand << 1
				sw      0				1				100			// save product, mcand
				sw      0       2				101
				add			0				0				1				// sum
				lw			0				2				two			// two
				lw			0				4				one			// one
				lw			0				5				max			// max
				lw			0				6				neg1		// neg1				                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
srrlp		beq			4				5				done
				nand		3				2				7				// nand mplier and reg[2]
				beq			7				6				1
				add			1				4				1				// sum += reg[3]
				add			2				2				2				// increment two
				add			4				4				4				// increment one
				beq			0				0				srrlp
done		add			0				1				3				// multiplier = sum
				lw			0				1				100			// restore product, mcand
				lw			0				2				101
				beq			0				0				loop
exit    halt                         
mcand		.fill		3               
mplier	.fill		4
two			.fill		2
one			.fill		1
max			.fill		32768
neg1		.fill		-1
