# mcand = 32766
# mplier = 10383
mcand = 32766
mplier = 10383
product = 0

# 340209378
  
while (mplier != 0)
  mask = ~(mplier & 1)
  # puts(mask)
  if (mask == -2)         # originally 1
    product = product + mcand
  end
  
  # mcand = mcand << 1
  # mplier = mplier >> 1
  
  mcand += mcand
  
  a = 2
  b = 1
  count = 0
    
  while (a < 2 ** 15)
    if (~(mplier & a) != -1)    # originally != 0
      count += b
    end
    a += a
    b += b
  end
  
  mplier = count
  puts(mplier)
end

puts(product)