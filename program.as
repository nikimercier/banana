        lw      0       1       five 			// This is the given test case
        lw      1       2       3    
start   add     1       2       1    
        beq     0       1       2    
        beq     0       0       start
        noop                         
done    halt                         
five    .fill   5                    
neg1    .fill   -1                   
stAddr  .fill   start                


// *r1 = M[*r0 + *five]
// *r2 = M[*r1 + 3]
// *r1 += *r2

// while (*r0 != *r1)
// *r1 += *r2