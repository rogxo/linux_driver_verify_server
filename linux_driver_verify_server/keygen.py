import hashlib
import random
import time

def keygen(filename, day, number):
    f = open(filename,mode='a+')
    gen = {}

    for i in range(number):
        rand = random.randint(0,9999999)
        hash = hashlib.md5(str(rand).encode())
        rand = random.randint(-9999999,0)
        key = hashlib.sha1(str(int(hash.hexdigest(), 16) + rand).encode()).hexdigest()
        gen[hash.hexdigest()] = day

    lines = f.readlines()
    for line in lines:
        for key in gen.keys():
            if key in line:     #same key
                val = gen.pop(key)
                key = hashlib.md5(str(int(hash.hexdigest(), 16) + val).encode()).hexdigest()
                gen[key] = val

    save_file_name = str(int(time.time()))+".txt"
    for key in gen.keys():
        f.write(key + "\t" + str(gen[key]) + "\n")
        with open(save_file_name,mode='a+') as out:
            out.write(key + "\t" + str(gen[key]) + "\n")

    f.close()
    return gen

keygen("data.dat", 30, 10)
