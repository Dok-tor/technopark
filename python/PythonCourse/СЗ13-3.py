from itertools import zip_longest
from typing import List, Tuple


def fill_missing_values(values_list_1: List[int], values_list_2: List[int]) -> List[Tuple[int, int]]:
    result = []
    for elem1, elem2 in zip_longest(values_list_1, values_list_2):
        result.append((elem1 if elem1 is not None else 1, elem2 if elem2 is not None else 1))
    return result


code = []
while data := input():
    code.append(data)
code = "\n".join(code)
exec(code)
