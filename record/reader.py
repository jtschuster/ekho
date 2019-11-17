with open("jacksonsrecord.raw", "rb") as f:
    byte = b'\x01'
    i = 0
    while byte!=b"":
        byte = f.read(8)
        # Do stuff with byte.
        print(i, bytes(byte).hex())
        i+=8
        try:
            input("Press enter to continue")
        except SyntaxError:
            pass
print("UHH")
