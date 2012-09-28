# mcand = 32766
# mplier = 10383
mcand = 32766
mplier = 10383

result = 0
count = 1

# 340209378

15.times do 
  puts((count & mplier) != 0)
  
  if ((count & mplier) != 0)
    result = result + mcand
  end
  count += count
  mcand = mcand << 1
end

puts(result)