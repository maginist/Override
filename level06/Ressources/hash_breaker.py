import sys
str = sys.argv[1]
print("ARG: ",str)
tmp = (ord(str[3]) ^ 4919) + 6221293
# print("TMP: ", tmp)

for c in str:
	if ord(c) <= 31:
		exit
	A =  ord(c) ^ int(tmp)
	# print("A: ", A)
	B = A * 0x88233b2b >> 32
	# print("B: ", B)
	C = A - B
	# print("C: ", C)
	D = C >> 1
	# print("D: ", D)
	E = D + B 
	# print("E: ", E)-+-
	F = E >> 10
	# print("F: ", F)
	G = F * 1337
	# print("G: ", G)
	H = A - G
	# print("H: ", H)
	tmp += H
print("hash: ", tmp)