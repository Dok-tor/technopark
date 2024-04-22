from collections import deque
from typing import List


def rotate_list(nums: List[int], n: int):
    deq = deque(nums)
    for i in range(n):
        deq.appendleft(deq.pop())
    return list(deq)


code = []
while data := input():
    code.append(data)
code = "\n".join(code)
exec(code)
