from collections import defaultdict
from typing import List, Tuple


def fill_specializations(specializations: List[Tuple[str, str]]):
    d = defaultdict(list)
    for spec, name in specializations:
        d[spec].append(name)
    return dict(d)


code = []
while data := input():
    code.append(data)
code = "\n".join(code)
exec(code)
