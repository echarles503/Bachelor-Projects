import anvil as anv
r1 = anv.Region.from_file("./region/r.-1.-1.mca")

chunk = anv.Chunk.from_region(r1, 19, 22)
print(chunk.x, chunk.z)

block = chunk.get_block(13, 73, 5)

print(block.id)




