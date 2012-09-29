				lw			0				5				ones
				lw			0				6				ones
				nand		5				6				7					// result should be 0
				beq			7				0				1			// this should happen
        noop                         
done    halt                         
five    .fill   5                    
neg1    .fill   -1
ones		.fill		4294967295                   
stAddr  .fill   start
