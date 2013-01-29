import sys
import random

if not (len(sys.argv)):
    sys.exit('Usage: %s <c_1> ... <c_n>' % sys.argv[0])


candidate = set((int(x) for x in sys.argv[1:]))

print(candidate)

try:
    buffer = str()
    while True:
        buffer += " "+input()
except EOFError:
    pass

buffer = buffer.split();
print(buffer)

component_count = int(buffer[0])
transaction_count = int(buffer[1])

hit = [False] * transaction_count

i = 2
for transaction in range(transaction_count):
    for component in range(component_count):
        if(int(buffer[i]) != 0 and (component + 1)  in candidate):
            hit[transaction] = True
        i += 1
print(hit)

is_candidate = True

for transaction in range(transaction_count):
    if(int(buffer[i]) and not hit[transaction]):
        is_candidate = False
        break
    i += 1

print(is_candidate)
