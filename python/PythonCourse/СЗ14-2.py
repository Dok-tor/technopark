import datetime

days, seconds = int(input()), int(input())


def shift_time(days: int, seconds: int):
    default = datetime.datetime(2023, 1, 1, 12, 30, 00 )
    adding = datetime.timedelta(days=days, seconds=seconds)
    result = default + adding
    return result.day, result.second


print(shift_time(days, seconds))
