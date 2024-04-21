# Написать функцию get_indexes которая принимает два списка и возвращает список индексов,
# в которых элемент из первого списка меньше элемента из второго списка по данному индексу.
# Желательно проходиться сразу по двум массивам одновременно (вспомните функцию zip).
# Для нахождения индексов можно использовать enumerate вместе с zip.

from typing import List


def get_indexes(nums1: List[int], nums2: List[int]) -> List[int]:
    li = []
    for ind, (first, second) in enumerate(zip(nums1, nums2)):
        if first < second:
            li.append(ind)
    return li


code = []
while data := input():
    code.append(data)
code = "\n".join(code)
exec(code)
