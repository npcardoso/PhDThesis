import sys
import random

if not (3 <= len(sys.argv) <= 5):
    sys.exit('Usage: %s <#components> <#transactions> [selection_rate] [error_rate]' % sys.argv[0])


component_count = int(sys.argv[1])
transaction_count = int(sys.argv[2])

if(len(sys.argv) > 3):
    selection_rate = float(sys.argv[3])
else:
    selection_rate = 0.3

if(len(sys.argv) > 4):
    error_rate = float(sys.argv[4])
else:
    error_rate = 0.3

print("%d %d\n" % (component_count, transaction_count))

for transaction in range(transaction_count):
    tmp_str = ""
    for component in range(component_count):
        if(random.random() < selection_rate):
            tmp_str += "1 "
        else:
            tmp_str += "0 "
    print(tmp_str)

print()

for transaction in range(transaction_count):
    if(random.random() < selection_rate):
        print("1 ", end="")
    else:
        print("0 ", end="")

print()
