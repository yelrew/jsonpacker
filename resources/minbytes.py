
def bit(num):
    bin_ = bin(num) if num > 0 else bin(num+(1<<32))
    res = bin_.replace('0b','').rjust(32, '0')
    out = ''
    for k in range(0, len(res), 8):
        out += res[k:k+8]
        if k < len(res) -8:
            out += ' '
    return out

def hex_(num):
    res = hex(num) if num > 0 else hex(num+(1<<32))
    return '0x' + res.replace('0x','').rjust(8, '0')


def minbytes(num, szint=4):
    sign_bit = (num & (0x1 << (szint*8-1)) ) >> (szint*8-1)
    first_oct_msk = 0xff<<((szint-1)*8) #0xff000000 
    second_oct_b8_msk = 0x80<<((szint-2)*8) # 0x00800000
    nbytes = szint
    while (nbytes > 1 and ((num & first_oct_msk) == first_oct_msk*sign_bit) and \
          ((num & second_oct_b8_msk) == second_oct_b8_msk*sign_bit)):
        first_oct_msk >>= 8
        second_oct_b8_msk >>= 8
        nbytes -= 1
    return nbytes

def prc(num):
    print ('{:5}:'.format(num), bit(num) , '({:>5})'.format(hex_(num)),  minbytes(num) )    


print()

prc(39)
prc(-39)
prc(-128)
prc(255)
prc(223498)
prc(-1098)
prc(1)
prc(0)
prc(-1)

print()

#print (bit(-r1) , contract(-r1))

