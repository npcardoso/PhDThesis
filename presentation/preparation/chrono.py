#!/usr/bin/python3


from sys import stdin
from datetime import datetime, timedelta


times = []
start = datetime.now()
last = datetime.now()

for line in stdin:
    now = datetime.now()
    times.append(now - last)
    i = len(times)
    print('@%s: %d, %s' % (now-start, i, times[i - 1]))

    last = now

print("========================")
for i in range(len(times)):
    print('%d, %s' % (i + 1, times[i]))
print("========================")
print(sum(times, timedelta(0)))
