import datetime
from collections import Counter
from typing import List


def most_common_months(dates: List[str], n) -> List[int]:
    new_list = [datetime.datetime.strptime(x, '%Y-%m-%dT%H:%M:%S').month for x in dates]
    cont = Counter(new_list)
    most_common = [x[0] for x in cont.most_common()]
    return most_common[:n]


code = []
while data := input():
    code.append(data)
code = "\n".join(code)
exec(code)
