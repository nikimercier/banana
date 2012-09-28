mcand = 6
mplier = 19
# mcand = 3
# mplier = 4
product = 0

# 340209378
  
while (mplier != 0)
  mask = ~(mplier & 1)
  
  puts(mask == -2)
  
  if (mask == -2)         # originally 1
    product = product + mcand
  end
  
  mcand = mcand << 1
  mplier = mplier >> 1

end

puts(product)